/**
 * @file ble_server.hpp
 * @author Niel Cansino (nielcansino@gmail.com)
 * @brief   
 * @version 0.1
 * @date 2020-09-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include "ble_common.hpp"

/****************************************************************
 * Definitions and Constants
 ***************************************************************/
static constexpr char DEVICE_NAME[]  = "Distance_Server";
static unsigned long reconnect_timer_ms = 0;
static constexpr unsigned RECONNECT_PERIOD_MS = 200;
static constexpr unsigned MAX_RECONNECT_ATTEMPTS = 3;
static unsigned int reconnect_attempts = 0;


/****************************************************************
 * Function Prototypes
 ***************************************************************/
/**Empty here */

/****************************************************************
 * Global Variables
 ***************************************************************/
/**Empty here */

/****************************************************************
 * Classes
 ***************************************************************/

class peripheralCallback : public BLEServerCallbacks{
private:
  BLEServer* pServer;
  BLEAdvertising *pAdvertising;
  
public:
  peripheralCallback(BLEServer* server, BLEAdvertising *advertising) : pServer(server), pAdvertising(advertising) {}

  void onConnect(BLEServer* pServer){
    // Client connected
    Serial.println("Client connected");
    Serial.println("THIS IS THE SERVER");
  }

  void onDisconnect(BLEServer* pServer) {
    // Client disconnected
    Serial.println("Client disconnected");

    // Check if we have reached the maximum number of reconnection attempts
    if (reconnect_attempts < MAX_RECONNECT_ATTEMPTS) {
      // Increment the reconnect attempt counter
      reconnect_attempts++;

      // Start the reconnect timer
      reconnect_timer_ms = millis() + RECONNECT_PERIOD_MS;

      // Restart advertising
      pAdvertising->start();
    } else {
      // Stop advertising and reset the device
      pAdvertising->stop();
      ESP.restart();
    }
  }

  void onAdvertisingStart(BLEServer* pServer){
    // Advertising started
    Serial.println("Advertising started");
  }

  void onScanStop(BLEServer* pServer){
    // Scan stopped
    Serial.println("Scan stopped");
  }

  void onServiceAdded(BLEServer* pServer, BLEService* pService){
    // Service added
    Serial.println("Service added");
  }

  void onLoop() {
    // Check if the client has reconnected
    if (reconnect_timer_ms && millis() >= reconnect_timer_ms) {
      Serial.println("Starting advertising...");
      pAdvertising->start();
      reconnect_timer_ms = 0;
    }
  }
};


/****************************************************************
 * Function Definition
 ***************************************************************/

void start_bluetooth_server()
{
    BLEDevice::init(DEVICE_NAME);

    /**Create bluetooth server (advertiser) */
    BLEServer       *pServer        = BLEDevice::createServer();
    BLEService      *pService       = pServer->createService(SERVICE_UUID);
    BLEAdvertising  *pAdvertising   = BLEDevice::getAdvertising();

    /**Bind server callback */
    peripheralCallback *pCallback = new peripheralCallback(pServer, pAdvertising);
    pServer->setCallbacks(pCallback);

    pAdvertising->addServiceUUID(SERVICE_UUID);

    /**Start service */
    pService->start();

    pAdvertising->setScanResponse(true);
    pAdvertising->setMinPreferred(0x06);  // functions that help with iPhone connections issue
    pAdvertising->setMinPreferred(0x12);

    /**Start advertising */
    pAdvertising->start();

    
}
