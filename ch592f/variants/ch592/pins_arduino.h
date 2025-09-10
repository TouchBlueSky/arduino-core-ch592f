#pragma once

// Pin definitions taken from:

// LEDs
#define PIN_LED        (8u)
#define LED_BUILTIN PIN_LED

// Serial
#define PIN_SERIAL0_TX (23u)
#define PIN_SERIAL0_RX (20u)

#define PIN_SERIAL1_TX (9u)
#define PIN_SERIAL1_RX (8u)

#define PIN_SERIAL2_TX (7u)
#define PIN_SERIAL2_RX (6u)

#define PIN_SERIAL3_TX (5u)
#define PIN_SERIAL3_RX (4u)

// SPI
// SPI0 默认引脚 PA12(MISO) PA13(MOSI) PA14(SCK) PA15(SS)
// 可以通过GPIOPinRemap映射到PB12(MISO) PB13(MOSI) PB14(SCK) PB15(SS)
#define PIN_SPI0_MISO  (12u)
#define PIN_SPI0_MOSI  (13u)
#define PIN_SPI0_SCK   (14u)
#define PIN_SPI0_SS    (15u)

// Wire
#define PIN_WIRE0_SDA  (30u)
#define PIN_WIRE0_SCL  (31u)
#define SDA  (PIN_WIRE0_SDA)
#define SCL  (PIN_WIRE0_SCL)

#define SERIAL_HOWMANY (4u)
#define SPI_HOWMANY    (1u)
#define WIRE_HOWMANY   (1u)

static const uint8_t A0 = (4u);
static const uint8_t A1 = (5u);
static const uint8_t A2 = (12u);
static const uint8_t A3 = (13u);
static const uint8_t A4 = (14u);
static const uint8_t A5 = (15u);
