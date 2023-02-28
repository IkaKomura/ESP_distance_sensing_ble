#include <Arduino.h>

const int ledPin = 13; // LED connected to digital pin 13
int sBlinks = 10; // Number of blinks on startup
int strobe = 10; // Strobe delay in milliseconds
void blinkStartup() {
for (int i = 0; i <= sBlinks; i++) {
  digitalWrite(ledPin,HIGH);
  delay(strobe);
  digitalWrite(ledPin,LOW);
  delay(strobe);
  }
}
#if SERVER_MODE
    #include "ble_server.hpp"
#elif CLIENT_MODE
    #include "ble_client.hpp"
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
#endif
}

void loop() {

#if SERVER_MODE
    /**Nothing to do here */
#elif CLIENT_MODE
    do_client_tasks();
    publish_rssi();
#endif

}

