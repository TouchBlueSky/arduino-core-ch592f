/*
  TwoWire.cpp - TWI/I2C library for Wiring & Arduino
  Copyright (c) 2006 Nicholas Zambetti.  All right reserved.

  This library is free software; you can redistribute it and/or
  modify it under the terms of the GNU Lesser General Public
  License as published by the Free Software Foundation; either
  version 2.1 of the License, or (at your option) any later version.

  This library is distributed in the hope that it will be useful,
  but WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
  Lesser General Public License for more details.

  You should have received a copy of the GNU Lesser General Public
  License along with this library; if not, write to the Free Software
  Foundation, Inc., 51 Franklin St, Fifth Floor, Boston, MA  02110-1301  USA

  Modified 2012 by Todd Krein (todd@krein.org) to implement repeated start
*/
#include <Arduino.h>
extern "C" {
  #include <stdlib.h>
  #include <string.h>
  #include <inttypes.h>
}

#include "Wire.h"

const uint8_t WIRE_BUFFER_LENGTH = 32;

// Initialize static members
uint8_t TwoWire::rxBuffer[WIRE_BUFFER_LENGTH];
uint8_t TwoWire::rxBufferIndex = 0;
uint8_t TwoWire::rxBufferLength = 0;

uint8_t TwoWire::txAddress = 0;
uint8_t TwoWire::txBuffer[WIRE_BUFFER_LENGTH];
uint8_t TwoWire::txBufferIndex = 0;
uint8_t TwoWire::txBufferLength = 0;

uint8_t TwoWire::transmitting = 0;
void (*TwoWire::user_onRequest)(void);
void (*TwoWire::user_onReceive)(int);

// Timeout for I2C operations (in microseconds)
#define I2C_TIMEOUT_US 100000  // 100ms timeout

// Error codes
#define I2C_ERROR_OK 0
#define I2C_ERROR_TIMEOUT 1
#define I2C_ERROR_NACK 2
#define I2C_ERROR_BUS 3

// Constructors
TwoWire::TwoWire() {}

// Public Methods
void TwoWire::begin(void) {

  GPIOB_ModeCfg(GPIO_Pin_14, GPIO_ModeIN_PU);  // SDA
  GPIOB_ModeCfg(GPIO_Pin_15, GPIO_ModeIN_PU);  // SCL
  
  // Enable clock for I2C peripheral
  // Clear the sleep clock off bit for I2C
  sys_safe_access_enable();
  R8_SLP_CLK_OFF1 &= ~RB_SLP_CLK_I2C;
  sys_safe_access_disable();
  
  // Initialize I2C as master
  I2C_Init(I2C_Mode_I2C, 100000, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, 0x00);
  I2C_Cmd(ENABLE);
}

void TwoWire::begin(uint8_t address) {
  // Initialize I2C as slave
  I2C_Init(I2C_Mode_I2C, 100000, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, address);
  I2C_Cmd(ENABLE);
}

void TwoWire::begin(int address) {
  begin((uint8_t)address);
}

void TwoWire::end(void) {
  I2C_Cmd(DISABLE);
}

void TwoWire::setClock(uint32_t frequency) {
  I2C_Cmd(DISABLE);
  I2C_Init(I2C_Mode_I2C, frequency, I2C_DutyCycle_16_9, I2C_Ack_Enable, I2C_AckAddr_7bit, 0x00);
  I2C_Cmd(ENABLE);
}

void TwoWire::beginTransmission(uint8_t address) {
  // Start transmission
  transmitting = 1;
  txAddress = address << 1;
  txBufferIndex = 0;
  txBufferLength = 0;
}

void TwoWire::beginTransmission(int address) {
  beginTransmission((uint8_t)address);
}

uint8_t TwoWire::endTransmission(uint8_t sendStop) {
  uint32_t start_time = SYS_GetSysTickCnt();
  uint8_t result = I2C_ERROR_OK;
  
  // Send data
  I2C_GenerateSTART(ENABLE);
  
  // Wait for start condition with timeout
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
    if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
      result = I2C_ERROR_TIMEOUT;
      goto end;
    }
  }
  
  // Send address
  I2C_Send7bitAddress(txAddress, I2C_Direction_Transmitter);
  
  // Wait for address ack with timeout
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_TRANSMITTER_MODE_SELECTED)) {
    if (I2C_GetFlagStatus(I2C_FLAG_AF) == SET) {
      I2C_ClearFlag(I2C_FLAG_AF);
      result = I2C_ERROR_NACK;
      goto end;
    }
    if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
      result = I2C_ERROR_TIMEOUT;
      goto end;
    }
  }
  
  // Send data
  for (uint8_t i = 0; i < txBufferLength; i++) {
    
    I2C_SendData(txBuffer[i]);
    
    // Wait for byte transmission with timeout
    // Check TxE flag first
    while (!I2C_GetFlagStatus(I2C_FLAG_TXE)) {
      if (I2C_GetFlagStatus(I2C_FLAG_AF) == SET) {
        I2C_ClearFlag(I2C_FLAG_AF);
        result = I2C_ERROR_NACK;
        goto end;
      }
      if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
        result = I2C_ERROR_TIMEOUT;
        goto end;
      }
    }
    
    // Then check BTF flag
    while (!I2C_GetFlagStatus(I2C_FLAG_BTF)) {
      if (I2C_GetFlagStatus(I2C_FLAG_AF) == SET) {
        I2C_ClearFlag(I2C_FLAG_AF);
        result = I2C_ERROR_NACK;
        goto end;
      }
      if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
        result = I2C_ERROR_TIMEOUT;
        goto end;
      }
    }
  }
  
  end:
  // Stop condition
  if (sendStop) {
    I2C_GenerateSTOP(ENABLE);
  }
  
  // Reset buffer
  txBufferIndex = 0;
  txBufferLength = 0;
  transmitting = 0;
  
  return result;
}

uint8_t TwoWire::endTransmission(void) {
  return endTransmission(true);
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity, uint8_t sendStop) {
  uint32_t start_time = SYS_GetSysTickCnt();
  
  // Check quantity
  if (quantity > WIRE_BUFFER_LENGTH) {
    quantity = WIRE_BUFFER_LENGTH;
  }
  
  // Start reception
  rxBufferIndex = 0;
  rxBufferLength = 0;
  
  I2C_GenerateSTART(ENABLE);
  
  // Wait for start condition with timeout
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_MODE_SELECT)) {
    if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
      I2C_GenerateSTOP(ENABLE);
      return 0;
    }
  }
  
  // Send address
  I2C_Send7bitAddress(address, I2C_Direction_Receiver);
  
  // Wait for address ack with timeout
  while (!I2C_CheckEvent(I2C_EVENT_MASTER_RECEIVER_MODE_SELECTED)) {
    if (I2C_GetFlagStatus(I2C_FLAG_AF) == SET) {
      I2C_ClearFlag(I2C_FLAG_AF);
      I2C_GenerateSTOP(ENABLE);
      return 0;
    }
    if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
      I2C_GenerateSTOP(ENABLE);
      return 0;
    }
  }
  
  // Receive data
  for (uint8_t i = 0; i < quantity; i++) {
    if (i == quantity - 1) {
      // NACK for last byte
      I2C_AcknowledgeConfig(DISABLE);
    }
    
    // Wait for data with timeout
    while (!I2C_GetFlagStatus(I2C_FLAG_RXNE)) {
      if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
        I2C_AcknowledgeConfig(ENABLE);
        I2C_GenerateSTOP(ENABLE);
        return rxBufferLength;
      }
    }
    
    rxBuffer[i] = I2C_ReceiveData();
    rxBufferLength++;
    
    // For the last byte, wait for BTF flag before sending STOP
    if (i == quantity - 1) {
      while (!I2C_GetFlagStatus(I2C_FLAG_BTF)) {
        if ((SYS_GetSysTickCnt() - start_time) > I2C_TIMEOUT_US) {
          I2C_AcknowledgeConfig(ENABLE);
          I2C_GenerateSTOP(ENABLE);
          return rxBufferLength;
        }
      }
    }
  }
  
  // Re-enable ACK
  I2C_AcknowledgeConfig(ENABLE);
  
  // Stop condition
  if (sendStop) {
    I2C_GenerateSTOP(ENABLE);
  }
  
  return rxBufferLength;
}

uint8_t TwoWire::requestFrom(uint8_t address, uint8_t quantity) {
  return requestFrom(address, quantity, true);
}

uint8_t TwoWire::requestFrom(int address, int quantity) {
  return requestFrom((uint8_t)address, (uint8_t)quantity);
}

uint8_t TwoWire::requestFrom(int address, int quantity, int sendStop) {
  return requestFrom((uint8_t)address, (uint8_t)quantity, (uint8_t)sendStop);
}

size_t TwoWire::write(uint8_t data) {
  if (transmitting) {
    // In master transmitter mode
    if (txBufferLength >= WIRE_BUFFER_LENGTH) {
      return 0;
    }
    txBuffer[txBufferIndex] = data;
    ++txBufferIndex;
    txBufferLength = txBufferIndex;
    return 1;
  } else {
    // In slave send mode
    return 0;
  }
}

size_t TwoWire::write(const uint8_t *data, size_t quantity) {
  size_t i;
  for (i = 0; i < quantity; ++i) {
    if(write(data[i]) == 0) {
      break;
    }
  }
  return i;
}

int TwoWire::available(void) {
  return rxBufferLength - rxBufferIndex;
}

int TwoWire::read(void) {
  int value = -1;
  
  if (rxBufferIndex < rxBufferLength) {
    value = rxBuffer[rxBufferIndex];
    ++rxBufferIndex;
  }
  
  return value;
}

int TwoWire::peek(void) {
  int value = -1;
  
  if (rxBufferIndex < rxBufferLength) {
    value = rxBuffer[rxBufferIndex];
  }
  
  return value;
}

void TwoWire::flush(void) {
  // Do nothing, use endTransmission() to force data transfer
}

void TwoWire::onReceive(void (*function)(int)) {
  user_onReceive = function;
}

void TwoWire::onRequest(void (*function)(void)) {
  user_onRequest = function;
}

// Preinstantiate Object
TwoWire Wire = TwoWire();

// Initialize Wire object
void wireInit() {
  Wire.begin();
}