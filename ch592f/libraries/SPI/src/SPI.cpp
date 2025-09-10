/*
 * SPI.cpp
 *
 * Arduino-compatible SPI library for CH592
 *
 * Copyright (c) 2024 Nanjing Qinheng Microelectronics Co., Ltd.
 */

#include "SPI.h"

SPIClass SPI;

uint8_t SPIClass::initialized = 0;
uint8_t SPIClass::interruptMode = 0;
uint8_t SPIClass::interruptMask = 0;
uint8_t SPIClass::interruptSave = 0;
uint8_t SPIClass::bitOrderMode = MSBFIRST;
uint8_t SPIClass::spiDataMode = SPI_MODE0;
uint32_t SPIClass::clockDivider = 4000000;
uint8_t SPIClass::inTransactionFlag = 0;

SPIClass::SPIClass() {
  // Constructor
}

void SPIClass::begin() {
  init();
  SPI0_MasterDefInit();
  applySettings();
  initialized = 1;
}

void SPIClass::beginSlave() {
  // Configure SPI pins for slave mode
  GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeOut_PP_5mA);  // MISO
  GPIOA_ModeCfg(GPIO_Pin_13, GPIO_ModeIN_PU);       // MOSI
  GPIOA_ModeCfg(GPIO_Pin_14, GPIO_ModeIN_PU);       // SCK
  GPIOA_ModeCfg(GPIO_Pin_15, GPIO_ModeIN_PU);       // SS
  
  // Initialize SPI in slave mode
  SPI0_SlaveInit();
  initialized = 1;
}

void SPIClass::init() {
  // Configure SPI pins
  // SPI0 默认引脚 PA12(MISO) PA13(MOSI) PA14(SCK) PA15(SS)
  GPIOA_ModeCfg(GPIO_Pin_12, GPIO_ModeIN_PU);  // MISO
  GPIOA_ModeCfg(GPIO_Pin_13, GPIO_ModeOut_PP_5mA);  // MOSI
  GPIOA_ModeCfg(GPIO_Pin_14, GPIO_ModeOut_PP_5mA);  // SCK
  // SS pin is typically controlled by the user application
}

// Slave mode functions
uint8_t SPIClass::transferSlave(uint8_t data) {
  if (initialized) {
    SPI0_SlaveSendByte(data);
    return SPI0_SlaveRecvByte();
  }
  return 0;
}

void SPIClass::transferSlave(void *buf, size_t count) {
  if (initialized && count > 0) {
    uint8_t *buffer = (uint8_t *)buf;
    for (size_t i = 0; i < count; i++) {
      buffer[i] = transferSlave(buffer[i]);
    }
  }
}

void SPIClass::applySettings() {
  // Apply bit order
  ModeBitOrderTypeDef mode;
  switch (spiDataMode) {
    case SPI_MODE0:
      mode = (bitOrderMode == LSBFIRST) ? Mode0_LowBitINFront : Mode0_HighBitINFront;
      break;
    case SPI_MODE3:
      mode = (bitOrderMode == LSBFIRST) ? Mode3_LowBitINFront : Mode3_HighBitINFront;
      break;
    default:
      mode = Mode0_HighBitINFront;
      break;
  }
  SPI0_DataMode(mode);
  
  // Apply clock divider
  // System clock is 60MHz, calculate divider
  uint8_t divider = 60000000 / clockDivider;
  if (divider < 2) divider = 2;
  SPI0_CLKCfg(divider);
}

void SPIClass::end() {
  SPI0_Disable();
  initialized = 0;
}

uint8_t SPIClass::transfer(uint8_t data) {
  SPI0_MasterSendByte(data);
  return SPI0_MasterRecvByte();
}

uint16_t SPIClass::transfer16(uint16_t data) {
  union { uint16_t val; struct { uint8_t lsb; uint8_t msb; } b; } t;
  
  t.val = data;
  
  if (bitOrderMode == LSBFIRST) {
    t.b.lsb = transfer(t.b.lsb);
    t.b.msb = transfer(t.b.msb);
  } else {
    t.b.msb = transfer(t.b.msb);
    t.b.lsb = transfer(t.b.lsb);
  }
  
  return t.val;
}

void SPIClass::transfer(void *buf, size_t count) {
  if (count == 0) return;
  
  uint8_t *buffer = (uint8_t *)buf;
  for (size_t i = 0; i < count; i++) {
    buffer[i] = transfer(buffer[i]);
  }
}

void SPIClass::usingInterrupt(uint8_t interruptNumber) {
  // Not implemented for now
}

void SPIClass::notUsingInterrupt(uint8_t interruptNumber) {
  // Not implemented for now
}

void SPIClass::setBitOrderBit(uint8_t bitOrder) {
  bitOrderMode = bitOrder;
  if (initialized) {
    applySettings();
  }
}

void SPIClass::setDataModeBit(uint8_t dataMode) {
  spiDataMode = dataMode;
  if (initialized) {
    applySettings();
  }
}

void SPIClass::setClockDividerBit(uint8_t clockDiv) {
  // Convert divider to frequency
  clockDivider = 60000000 / clockDiv;
  if (initialized) {
    applySettings();
  }
}