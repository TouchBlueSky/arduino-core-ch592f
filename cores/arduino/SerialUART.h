#ifdef __cplusplus

#ifndef __SERIALUART_H__
#define __SERIALUART_H__

#include <Arduino.h>
#include "api/HardwareSerial.h"
#include "CH59x_common.h"
#include "api/RingBuffer.h"

namespace arduino {

class SerialUART : public Stream
{
public:
    SerialUART(uint8_t uart_num);
    ~SerialUART();
    
    bool setPinout();
    void setPins(pin_size_t tx, pin_size_t rx);  // 添加设置自定义引脚的方法

    void begin(unsigned long baud = 115200) { begin(baud, SERIAL_8N1); };
    void begin(unsigned long baud, uint16_t config);
    void end();

    virtual int peek();
    virtual int read();
    virtual int available();
    virtual int availableForWrite();
    virtual void flush();
    virtual size_t write(uint8_t c);
    virtual size_t write(const uint8_t *p, size_t len);
    using Print::write;
    operator bool();
    RingBuffer _rx_buffer;

private:
    bool _running = false;
    pin_size_t _tx, _rx;
    int _baud;
    uint8_t _uart_num;
    bool _pinsSet = false;  // 标记是否设置了自定义引脚
    
};

}

extern arduino::SerialUART Serial0; // HW UART 0
extern arduino::SerialUART Serial1; // HW UART 1
extern arduino::SerialUART Serial2; // HW UART 2
extern arduino::SerialUART Serial3; // HW UART 3

#define Serial Serial1

#endif
#endif