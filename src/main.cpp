#include <Arduino.h>


#if SERVER_MODE
    #include "ble_server.hpp"
#elif CLIENT_MODE
    #include "ble_client.hpp"
#elif TTGO_CLIENT_MODE
    #include "ble_ttgo_client.hpp"
    #include <TFT_eSPI.h>
#endif

/****************************************************************
 * Function Definitions
 ***************************************************************/

void setup() {

    Serial.begin(115200);

#if SERVER_MODE
    Serial.print("Starting BLE server...");
    start_bluetooth_server();
    Serial.print("BLE server started...");
    pinMode(ledPin, OUTPUT);
    blinkStartup();

#elif CLIENT_MODE
    Serial.print("Setting up BLE client...");
    start_bluetooth_client();
    Serial.print("BLE client started...");
    pinMode(ledPin, OUTPUT);
    blinkStartup();

#elif TTGO_CLIENT_MODE
    Serial.print("Setting up BLE ttgo client...");
    start_ttgo_bluetooth_client();
    Serial.print("BLE ttgoclient started...");
    pinMode(ledPin, OUTPUT);
    blinkStartup();    
#endif
}

void loop() {

#if SERVER_MODE
    /**Nothing to do here */
#elif CLIENT_MODE
    do_client_tasks();
    publish_rssi();
#elif TTGO_CLIENT_MODE
    do_client_tasks();
    publish_rssi();
#endif

}

