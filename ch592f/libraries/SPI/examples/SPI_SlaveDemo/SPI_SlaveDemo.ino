/*
  SPI Slave Demo

  This example demonstrates how to use the CH592 SPI library in slave mode.
  It shows how to receive data from a master and send response data.

  The circuit:
  - Connect SPI slave (CH592) to SPI master device
  - MISO (PA12) to MISO of master
  - MOSI (PA13) to MOSI of master
  - SCK (PA14) to SCK of master
  - SS (PA15) to SS of master

  Created by WCH, 2024
  Modified by waterz, 2024
*/

#include <SPI.h>

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("SPI Slave Demo");

  // Initialize SPI in slave mode
  SPI.beginSlave();
  
  Serial.println("SPI Slave initialized");
}

void loop() {
  // Wait for data from master and send response
  // Send a dummy byte (0x00) to receive data from master
  uint8_t received = SPI.transferSlave((uint8_t)0x00);
  
  // Check if we received valid data (not 0x00)
  if (received != 0x00) {
    Serial.print("Received: 0x");
    Serial.println(received, HEX);
    
    // Send response (bitwise complement)
    SPI.transferSlave(~received);
    Serial.print("Sent: 0x");
    Serial.println(~received, HEX);
  }
  
  delay(100);
}