/**
 * @file ble_common.hpp
 * @author Niel Cansino (nielcansino@gmail.com)
 * @brief 
 * @version 0.1
 * @date 2020-09-13
 * 
 * @copyright Copyright (c) 2020
 * 
 */

#pragma once

#include <Arduino.h>
#include <BLEDevice.h>
#include <BLEUtils.h>
#include <BLEServer.h>

/****************************************************************
 * Definitions and Constants
 ***************************************************************/
static constexpr char SERVICE_UUID[] = "4fafc201-1fb5-459e-8fcc-c5c9c331914b";
static BLEUUID SERVICE_UUID_OBJ(SERVICE_UUID);

//////functions
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