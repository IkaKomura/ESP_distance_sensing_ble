# BLE Distance Tracing Device - Custom Fork

This is a fork from: https://github.com/nacansino/distance_sensing_ble

A Distance tracing device prototype using ESP32 devices.

## Goals of this project

This is a custom fork of the BLE Contact Tracing Device project that has been modified for the purpose of tracking people or objects in a theatre space. The main goal of the project is to rougly detect the distance between devices in order to track the location of individuals or objects in the theatre.
I will later implement a way to use that data creatively.

## Changes Made

- Changed the code for automatic connection and reconnection to the server.
- Added a debug function, BlinkLED with a led  that's blinking in a rate based on the RSSI value.
- Added in definitions for Unlikely Maker Feather S3 board
- Added more serial monitor debug outputs.
## Tech Stack

The following materials and software were used for the development:

- ESP32 Development Board (BLE 5.0 capable)
- PlatformIO on Visual Studio Code

## Original Project

https://github.com/nacansino/distance_sensing_ble
This project is based on the BLE Contact Tracing Device project, which aims to develop a device capable of accurately detecting contact between individuals in order to help abate the spread of COVID-19. For more information, please refer to the original project's repository.
## Related Projects

Just throwing out related projects and useful information here:

- [How far can you go?](http://www.davidgyoungtech.com/2020/05/15/how-far-can-you-go)
- [Covid19Radar](https://github.com/Covid-19Radar/Covid19Radar)
