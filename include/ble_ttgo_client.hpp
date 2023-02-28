/**
 * @file ble_ttgo_client.hpp
 * @author BK Sannerud
 * @brief 
 * @version 0.1
 * @date 2023-02-28
 * 
 * @copyright Copyright (c) 2023
 * 
 */

#pragma once

#include "ble_common.hpp"

/****************************************************************
 * Definitions and Constants
 ***************************************************************/
static constexpr char DEVICE_NAME[]  = "ttgo_Distance_Client";
static int rssi_measurement = -9999; // initialize to an invalid value
static bool valid_rssi_received = false;
static unsigned long rssi_invalid_start_time = 0;
static int resetThreshold = -110;
static constexpr unsigned MAX_RSSI_INVALID_PERIOD_MS = 100;


/****************************************************************
 * Function Prototypes
 ***************************************************************/

 /* LED Blinking Code */
static void BlinkLED(int r) {
  // Map the RSSI measurement to a blinking rate between 1 and 100 Hz using a logarithmic scale.
  int blink_rate = 1 + (int)(99.0 * (1.0 - log10(-r))); // r is negative, so we need to use log10(-r)

  // Calculate the time period for one on/off cycle based on the blinking rate.
  unsigned long cycle_period_ms = (10000 / (2 * blink_rate));

  // Blink the LED.
  static bool led_state = false;
  static unsigned long last_toggle_ms = millis();
  if (millis() - last_toggle_ms >= cycle_period_ms) {
    led_state = !led_state;
    digitalWrite(ledPin, led_state);
    last_toggle_ms = millis();
  }
}
/****************************************************************
 * Global Variables
 ***************************************************************/
static boolean doConnect        = false;
static boolean connected        = false;
static boolean doScan           = false;

static BLEAdvertisedDevice* myDevice;
static BLEClient*           pClient;

/****************************************************************
 * Classes
 ***************************************************************/


class MyClientCallback : public BLEClientCallbacks {
  void onConnect(BLEClient* pclient) {
      Serial.println("Client Connected");
  }

  void onDisconnect(BLEClient* pclient) {
    connected = false;
    Serial.println("Client Disconnected");
  }
};

/**
 * Scan for BLE servers and find the first one that advertises the service we are looking for.
 */
class MyAdvertisedDeviceCallbacks: public BLEAdvertisedDeviceCallbacks {
 /**
   * Called for each advertising BLE server.
   */
  void onResult(BLEAdvertisedDevice advertisedDevice) {
    Serial.print("BLE Advertised Device found: ");
    Serial.println(advertisedDevice.toString().c_str());

    // We have found a device, let us now see if it contains the service we are looking for.
    if (advertisedDevice.haveServiceUUID() && advertisedDevice.isAdvertisingService(SERVICE_UUID_OBJ)) {

      BLEDevice::getScan()->stop();
      myDevice = new BLEAdvertisedDevice(advertisedDevice);
      doConnect = true;
      doScan = true;

    } // Found our server
  } // onResult
}; // MyAdvertisedDeviceCallbacks

/****************************************************************
 * Function Definition
 ***************************************************************/

void start_ttgo_bluetooth_client()
{
    BLEDevice::init(DEVICE_NAME);

    /* Retrieve a Scanner and set the callback we want to use to be informed when we
     have detected a new device.  Specify that we want active scanning and start the
     scan to run for 5 seconds.*/

    BLEScan* pBLEScan = BLEDevice::getScan();
    pBLEScan->setAdvertisedDeviceCallbacks(new MyAdvertisedDeviceCallbacks());
    pBLEScan->setInterval(1349);
    pBLEScan->setWindow(449);
    pBLEScan->setActiveScan(true);
    pBLEScan->start(5, false);
}

bool connectToServer() {
    Serial.print("Forming a connection to ");
    Serial.println(myDevice->getAddress().toString().c_str());
    
    // create client
    pClient = BLEDevice::createClient();
    Serial.println(" - Created client");

    pClient->setClientCallbacks(new MyClientCallback());

    // Connect to the remove BLE Server.
    pClient->connect(myDevice);  // if you pass BLEAdvertisedDevice instead of address, it will be recognized type of peer device address (public or private)
    Serial.println(" - Connected to server");

    // Obtain a reference to the service we are after in the remote BLE server.
    BLERemoteService* pRemoteService = pClient->getService(SERVICE_UUID);
    if (pRemoteService == nullptr) {
      Serial.print("Failed to find our service UUID: ");
      Serial.println(SERVICE_UUID);
      pClient->disconnect();
      return false;
    }
    Serial.println(" - Found our service");

    /**Do characteristic tasks here upon finding valid services*/
    
    connected = true;

    return connected;
}


void do_client_tasks()
{
    static constexpr unsigned CLIENT_TASK_PERIOD_MS = 100;
    static unsigned long last_called = millis();
    static boolean toPublishRSSI = false;

    if(millis() - last_called < CLIENT_TASK_PERIOD_MS)
    {
        /**Not your turn yet */
        return;
    }

    /* If the flag "doConnect" is true then we have scanned for and found the desired
        BLE Server with which we wish to connect.  Now we connect to it.  Once we are 
        connected we set the connected flag to be true.*/
    if (doConnect == true) {
        if (connectToServer()) {
        Serial.println("We are now connected to the BLE Server.");
         toPublishRSSI = true;
        //Serial.println("Please enter a command: start or end");
        Serial.println("THIS IS THE CLIENT");
        } else {
        Serial.println("We have failed to connect to the server; there is nothin more we will do.");
        }
        doConnect = false;
    }

    /* If we are connected to a peer BLE Server, update the characteristic each time we are reached
     with the current time since boot. */
    if (connected) 
    {
    
        String newValue = "Time since boot: " + String(millis()/500);
    
        /**Do service tasks here */  
        if (myDevice->haveRSSI()) {
    rssi_measurement = (int)pClient->getRssi();
//New shit!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        if (rssi_measurement >= resetThreshold) {
        // valid RSSI measurement
        rssi_invalid_start_time = 0;
        BlinkLED(rssi_measurement);
    } else {
        // invalid RSSI measurement
        unsigned long current_time = millis();
        if (rssi_invalid_start_time == 0) {
            // start of invalid RSSI measurement
            rssi_invalid_start_time = current_time;
        } else if (current_time - rssi_invalid_start_time >= MAX_RSSI_INVALID_PERIOD_MS) {
            // RSSI measurement has been invalid for too long, restart BLE client
            Serial.println("Resetting");
            ESP.restart();
        }
    }
}
    
    }
    else if(doScan)
    {
        BLEDevice::getScan()->start(0);  // this is just example to start scan after disconnect, most likely there is better way to do it in arduino
    }

    last_called = millis();
}

void publish_rssi()
{
    static constexpr unsigned RSSI_PUBLISH_PERIOD_MS = 100;
    static String char_buffer;

    static unsigned long last_publish_ms = millis();
    static boolean toPublishRSSI = true;
    
    if(toPublishRSSI && ((millis() - last_publish_ms) >= RSSI_PUBLISH_PERIOD_MS))
    {
        Serial.printf("Rssi: %d \n", rssi_measurement);
        last_publish_ms = millis();

         
    }

}