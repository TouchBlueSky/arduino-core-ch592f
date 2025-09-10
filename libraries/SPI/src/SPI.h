/*
 * SPI.h
 *
 * Arduino-compatible SPI library for CH592
 *
 * Copyright (c) 2024 Nanjing Qinheng Microelectronics Co., Ltd.
 */

#ifndef _SPI_H_INCLUDED
#define _SPI_H_INCLUDED

#include <Arduino.h>
#include <CH59x_SPI.h>

// SPI_HAS_TRANSACTION means SPI has beginTransaction(), endTransaction(),
// usingInterrupt(), and SPISetting(clock, bitOrder, dataMode)
#define SPI_HAS_TRANSACTION 1

// SPI_HAS_NOTUSINGINTERRUPT means that SPI has notUsingInterrupt() method
#define SPI_HAS_NOTUSINGINTERRUPT 1

// SPI_ATOMIC_VERSION means that SPI has atomicity fixes and what version.
// This way when there is a bug fix you can check this define to alert users
// of your code if it uses functionality that is fixed in that same bug fix.
#define SPI_ATOMIC_VERSION 1

// Uncomment this line to add detection of mismatched begin/end transactions.
// A mismatch occurs if other libraries fail to use SPI.endTransaction() for
// each SPI.beginTransaction().  Connect an LED to this pin.  The LED will turn
// on if any mismatch is ever detected.
//#define SPI_TRANSACTION_MISMATCH_LED PIN_LED

// #ifndef LSBFIRST
// #define LSBFIRST 0
// #endif
// #ifndef MSBFIRST
// #define MSBFIRST 1
// #endif

#define SPI_CLOCK_DIV2   2
#define SPI_CLOCK_DIV4   4
#define SPI_CLOCK_DIV8   8
#define SPI_CLOCK_DIV16  16
#define SPI_CLOCK_DIV32  32
#define SPI_CLOCK_DIV64  64

typedef enum {
  SPI_MODE0 = 0,
  SPI_MODE1 = 1,
  SPI_MODE2 = 2,
  SPI_MODE3 = 3,
} SPIMode;


class SPISettings {
  public:
  SPISettings(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    if (__builtin_constant_p(clock)) {
      init_AlwaysInline(clock, bitOrder, dataMode);
    } else {
      init_MightInline(clock, bitOrder, dataMode);
    }
  }
  
  // Default speed set to 4MHz, SPI mode set to MODE 0 and Bit order set to MSB first.
  SPISettings() { init_AlwaysInline(4000000, MSBFIRST, SPI_MODE0); }
  
  private:
  void init_MightInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) {
    init_AlwaysInline(clock, bitOrder, dataMode);
  }
  
  void init_AlwaysInline(uint32_t clock, uint8_t bitOrder, uint8_t dataMode) __attribute__((__always_inline__)) {
    this->clockFreq = clock;
    this->bitOrder = bitOrder;
    this->dataMode = dataMode;
  }
  
  uint32_t clockFreq;
  uint8_t bitOrder;
  uint8_t dataMode;
  
  // Pointers are 32 bits on CH592
  friend class SPIClass;
};


class SPIClass {
  public:
  SPIClass();
  
  // Initialize the SPI library
  static void begin();
  
  // Initialize the SPI library in slave mode
  static void beginSlave();
  
  // If SPI is to be used from within an interrupt, this function registers
  // that interrupt with the SPI library, so beginTransaction() can prevent
  // conflicts.
  static void usingInterrupt(uint8_t interruptNumber);
  static void notUsingInterrupt(uint8_t interruptNumber);
  
  // Before using SPI.transfer() or asserting chip select pins,
  // this function is used to gain exclusive access to the SPI bus
  // and configure the correct settings.
  inline static void beginTransaction(SPISettings settings) {
    if (interruptMode > 0) {
      noInterrupts();
      
      #ifdef SPI_AVR_EIMSK
      if (interruptMode == 1) {
        interruptSave = SPI_AVR_EIMSK;
        SPI_AVR_EIMSK &= ~interruptMask;
      } else
      #endif
      {
        interruptSave = 1;  // Just a flag to indicate interrupts were disabled
      }
    }
    
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    if (inTransactionFlag) {
      pinMode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
      digitalWrite(SPI_TRANSACTION_MISMATCH_LED, HIGH);
    }
    inTransactionFlag = 1;
    #endif
    
    // Set SPI settings
    setBitOrder(settings.bitOrder);
    setDataMode(settings.dataMode);
    setClockDivider(settings.clockFreq);
  }
  
  // After performing a group of transfers and releasing the chip select
  // signal, this function allows others to access the SPI bus
  inline static void endTransaction(void) {
    #ifdef SPI_TRANSACTION_MISMATCH_LED
    if (!inTransactionFlag) {
      pinMode(SPI_TRANSACTION_MISMATCH_LED, OUTPUT);
      digitalWrite(SPI_TRANSACTION_MISMATCH_LED, HIGH);
    }
    inTransactionFlag = 0;
    #endif
    
    if (interruptMode > 0) {
      #ifdef SPI_AVR_EIMSK
      if (interruptMode == 1) {
        SPI_AVR_EIMSK = interruptSave;
      } else
      #endif
      {
        interrupts();  // Re-enable interrupts
      }
    }
  }
  
  // Disable the SPI bus
  static void end();
  
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setBitOrder(uint8_t bitOrder) {
    bitOrderMode = bitOrder;
  }
  
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setDataMode(uint8_t dataMode) {
    spiDataMode = dataMode;
  }
  
  // This function is deprecated.  New applications should use
  // beginTransaction() to configure SPI settings.
  inline static void setClockDivider(uint32_t clockFreq) {
    clockDivider = clockFreq;
  }
  
  // These methods sets the same setting for all transfers for compatibility
  // with older Arduino API
  static void setBitOrderBit(uint8_t bitOrder);
  static void setDataModeBit(uint8_t dataMode);
  static void setClockDividerBit(uint8_t clockDiv);
  
  // Transfer functions
  static uint8_t transfer(uint8_t data);
  static uint16_t transfer16(uint16_t data);
  static void transfer(void *buf, size_t count);
  
  // Slave mode transfer functions
  static uint8_t transferSlave(uint8_t data);
  static void transferSlave(void *buf, size_t count);
  
  // Transaction variables
  static uint8_t inTransactionFlag;
  
  private:
  static uint8_t initialized;
  static uint8_t interruptMode; // 0=none, 1=mask, 2=global
  static uint8_t interruptMask; // which interrupts to mask
  static uint8_t interruptSave; // temp storage, to restore state
  
  // SPI configuration
  static uint8_t bitOrderMode;
  static uint8_t spiDataMode;
  static uint32_t clockDivider;
  
  // Initialize the SPI
  static void init();
  
  // Apply settings to hardware
  static void applySettings();
};

extern SPIClass SPI;

#endif