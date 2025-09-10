# SPI Library for CH592

This library provides an Arduino-compatible interface for the SPI peripheral on CH592 microcontrollers.

## Features

- Supports both master and slave mode operation
- Compatible with Arduino SPI API
- Supports different SPI modes (MODE0, MODE3)
- Supports both MSB and LSB first bit order
- Configurable clock speeds
- Transaction support for thread-safe operation

## Hardware Configuration

The CH592 has one SPI peripheral (SPI0) with the following default pin assignments:

| Pin | Function | GPIO |
|-----|----------|------|
| PA12 | MISO | PIN_SPI0_MISO |
| PA13 | MOSI | PIN_SPI0_MOSI |
| PA14 | SCK | PIN_SPI0_SCK |
| PA15 | SS | PIN_SPI0_SS |

These pins can be remapped to PB12-PB15 using the GPIOPinRemap function.

## Installation

1. Download or clone this library to your Arduino libraries folder
2. Restart the Arduino IDE
3. The library will appear in the Sketch > Include Library menu

## Usage

Include the library in your sketch:

```cpp
#include <SPI.h>
```

Initialize the SPI bus in setup():

```cpp
// For master mode
void setup() {
  SPI.begin();
}

// For slave mode
void setup() {
  SPI.beginSlave();
}
```

Transfer data:

```cpp
// Master mode transfers
// Single byte transfer
uint8_t response = SPI.transfer(0x55);

// Multiple byte transfer
uint8_t data[] = {0x01, 0x02, 0x03, 0x04};
SPI.transfer(data, sizeof(data));

// Slave mode transfers
// Single byte transfer
uint8_t received = SPI.transferSlave(0x00); // Send dummy byte to receive data

// Multiple byte transfer
uint8_t slaveData[] = {0x00, 0x00, 0x00, 0x00};
SPI.transferSlave(slaveData, sizeof(slaveData));
```

Using transactions for thread-safe operation:

```cpp
SPISettings settings(4000000, MSBFIRST, SPI_MODE0);

SPI.beginTransaction(settings);
// Perform SPI transfers
uint8_t response = SPI.transfer(0x55);
SPI.endTransaction();
```

## API Reference

### SPISettings Class

- `SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode)`
- `SPISettings()` - Default constructor (4MHz, MSBFIRST, SPI_MODE0)

### SPIClass Class

- `begin()` - Initialize the SPI bus in master mode
- `beginSlave()` - Initialize the SPI bus in slave mode
- `end()` - Disable the SPI bus
- `transfer(uint8_t data)` - Transfer a single byte in master mode
- `transfer16(uint16_t data)` - Transfer two bytes in master mode
- `transfer(void *buf, size_t count)` - Transfer multiple bytes in master mode
- `transferSlave(uint8_t data)` - Transfer a single byte in slave mode
- `transferSlave(void *buf, size_t count)` - Transfer multiple bytes in slave mode
- `beginTransaction(SPISettings settings)` - Begin a transaction with specific settings
- `endTransaction()` - End a transaction
- `setBitOrder(uint8_t bitOrder)` - Set bit order (deprecated)
- `setDataMode(uint8_t dataMode)` - Set data mode (deprecated)
- `setClockDivider(uint32_t clockFreq)` - Set clock divider (deprecated)

### Constants

- `SPI_MODE0`, `SPI_MODE1`, `SPI_MODE2`, `SPI_MODE3` - SPI data modes
- `LSBFIRST`, `MSBFIRST` - Bit order
- `SPI_CLOCK_DIV2` to `SPI_CLOCK_DIV64` - Clock dividers

## Examples

- `SPI_MasterDemo` - Demonstrates master mode operation
- `SPI_SlaveDemo` - Demonstrates slave mode operation using the SPI library API

## Notes

- This library supports both master and slave mode operation
- The maximum SPI clock speed is limited by the system clock (60MHz)

## License

Copyright (c) 2024 Nanjing Qinheng Microelectronics Co., Ltd.