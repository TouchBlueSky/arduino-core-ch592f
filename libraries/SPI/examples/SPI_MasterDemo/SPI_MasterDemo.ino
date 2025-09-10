/*
  SPI Master Demo

  This example demonstrates how to use the CH592 SPI library in master mode.
  It shows how to send and receive data using different transfer methods.

  The circuit:
  - Connect SPI master (CH592) to SPI slave device
  - MISO (PA12) to MISO of slave
  - MOSI (PA13) to MOSI of slave
  - SCK (PA14) to SCK of slave
  - SS (PA15) to SS of slave (or control separately)

  Created by WCH, 2024
  Modified by waterz, 2024
*/

#include <SPI.h>

// Define slave select pin
const int slaveSelectPin = 15; // PA15

void setup() {
  // Initialize serial communication
  Serial.begin(115200);
  Serial.println("SPI Master Demo");

  // Set slave select pin as output
  pinMode(slaveSelectPin, OUTPUT);
  digitalWrite(slaveSelectPin, HIGH);

  // Initialize SPI
  SPI.begin();
  
  // Set SPI settings (optional, default settings are used if not set)
  // SPI.beginTransaction(SPISettings(4000000, MSBFIRST, SPI_MODE0));
}

void loop() {
  // Send single byte
  digitalWrite(slaveSelectPin, LOW);
  uint8_t response = SPI.transfer(0x55);
  digitalWrite(slaveSelectPin, HIGH);
  
  Serial.print("Sent: 0x55, Received: 0x");
  Serial.println(response, HEX);
  
  delay(1000);
  
  // Send and receive multiple bytes
  uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
  digitalWrite(slaveSelectPin, LOW);
  SPI.transfer(data, sizeof(data));
  digitalWrite(slaveSelectPin, HIGH);
  
  Serial.print("Sent: ");
  for (int i = 0; i < sizeof(data); i++) {
    Serial.print("0x");
    Serial.print(data[i], HEX);
    Serial.print(" ");
  }
  Serial.println();
  
  delay(2000);
}