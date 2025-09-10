#include "SerialUART.h"

namespace arduino {

SerialUART::SerialUART(uint8_t uart_num) : _uart_num(uart_num) {
    // 根据串口号设置默认引脚
    switch (_uart_num) {
        case 0:
            _tx = 23;  // PB7
            _rx = 20;  // PB4
            break;
        case 1:
            _tx = 9;   // PA9
            _rx = 8;   // PA8
            break;
        case 2:
            _tx = 7;   // PA7
            _rx = 6;   // PA6
            break;
        case 3:
            _tx = 5;   // PA5
            _rx = 4;   // PA4
            break;
    }
}

SerialUART::~SerialUART() {}


void SerialUART::setPins(pin_size_t tx, pin_size_t rx)
{
    _tx = tx;
    _rx = rx;
    _pinsSet = true;
}

bool SerialUART::setPinout()
{
    // 如果用户没有设置自定义引脚，则使用默认引脚
    if (!_pinsSet) {
        // 根据串口号设置默认引脚
        switch (_uart_num) {
            case 0:
                _tx = 23;  // PB7
                _rx = 20;  // PB4
                break;
            case 1:
                _tx = 9;   // PA9
                _rx = 8;   // PA8
                break;
            case 2:
                _tx = 7;   // PA7
                _rx = 6;   // PA6
                break;
            case 3:
                _tx = 5;   // PA5
                _rx = 4;   // PA4
                break;
        }
    }
    
    // 根据串口号配置引脚
    switch (_uart_num) {
        case 0:
            // UART0 默认引脚 PB4(RX) PB7(TX)
            // 可以通过GPIOPinRemap映射到PA15(RX) PA14(TX)
            if ((_tx == 14 && _rx == 15) || (_tx == 23 && _rx == 20)) {
                if (_tx == 14 && _rx == 15) {
                    GPIOPinRemap(ENABLE, RB_PIN_UART0); // 映射到PA14/PA15
                } else {
                    GPIOPinRemap(DISABLE, RB_PIN_UART0); // 使用默认引脚PB4/PB7
                }
                // 根据引脚是PA还是PB系列来选择相应的GPIO函数
                if (_tx >= 0 && _tx <= 15) {
                    GPIOA_SetBits(1 << (_tx));
                    GPIOA_ModeCfg(1 << (_rx), GPIO_ModeIN_PU);
                    GPIOA_ModeCfg(1 << (_tx), GPIO_ModeOut_PP_5mA);
                } else if (_tx >= 16 && _tx <= 39) {
                    GPIOB_SetBits(1 << (_tx - 16));
                    GPIOB_ModeCfg(1 << (_rx - 16), GPIO_ModeIN_PU);
                    GPIOB_ModeCfg(1 << (_tx - 16), GPIO_ModeOut_PP_5mA);
                }
            }
            break;
        case 1:
            // UART1 默认引脚 PA8(RX) PA9(TX)
            // 可以通过GPIOPinRemap映射到PB12(RX) PB13(TX)
            if ((_tx == 9 && _rx == 8) || (_tx == 29 && _rx == 28)) {
                if (_tx == 29 && _rx == 28) {
                    GPIOPinRemap(ENABLE, RB_PIN_UART1); // 映射到PB12/PB13
                } else {
                    GPIOPinRemap(DISABLE, RB_PIN_UART1); // 使用默认引脚PA8/PA9
                }
                // 根据引脚是PA还是PB系列来选择相应的GPIO函数
                if (_tx >= 0 && _tx <= 15) {
                    GPIOA_SetBits(1 << (_tx));
                    GPIOA_ModeCfg(1 << (_rx), GPIO_ModeIN_PU);
                    GPIOA_ModeCfg(1 << (_tx), GPIO_ModeOut_PP_5mA);
                } else if (_tx >= 16 && _tx <= 39) {
                    GPIOB_SetBits(1 << (_tx - 16));
                    GPIOB_ModeCfg(1 << (_rx - 16), GPIO_ModeIN_PU);
                    GPIOB_ModeCfg(1 << (_tx - 16), GPIO_ModeOut_PP_5mA);
                }
            }
            break;
        case 2:
            // UART2 默认引脚 PA6(RX) PA7(TX)
            // 可以通过GPIOPinRemap映射到PB22(RX) PB23(TX)
            if ((_tx == 7 && _rx == 6) || (_tx == 39 && _rx == 38)) {
                if (_tx == 39 && _rx == 38) {
                    GPIOPinRemap(ENABLE, RB_PIN_UART2); // 映射到PB22/PB23
                } else {
                    GPIOPinRemap(DISABLE, RB_PIN_UART2); // 使用默认引脚PA6/PA7
                }
                // 根据引脚是PA还是PB系列来选择相应的GPIO函数
                if (_tx >= 0 && _tx <= 15) {
                    GPIOA_SetBits(1 << (_tx));
                    GPIOA_ModeCfg(1 << (_rx), GPIO_ModeIN_PU);
                    GPIOA_ModeCfg(1 << (_tx), GPIO_ModeOut_PP_5mA);
                } else if (_tx >= 16 && _tx <= 39) {
                    GPIOB_SetBits(1 << (_tx - 16));
                    GPIOB_ModeCfg(1 << (_rx - 16), GPIO_ModeIN_PU);
                    GPIOB_ModeCfg(1 << (_tx - 16), GPIO_ModeOut_PP_5mA);
                }
            }
            break;
        case 3:
            // UART3 默认引脚 PA4(RX) PA5(TX)
            // 可以通过GPIOPinRemap映射到PB20(RX) PB21(TX)
            if ((_tx == 5 && _rx == 4) || (_tx == 37 && _rx == 36)) {
                if (_tx == 37 && _rx == 36) {
                    GPIOPinRemap(ENABLE, RB_PIN_UART3); // 映射到PB20/PB21
                } else {
                    GPIOPinRemap(DISABLE, RB_PIN_UART3); // 使用默认引脚PA4/PA5
                }
                // 根据引脚是PA还是PB系列来选择相应的GPIO函数
                if (_tx >= 0 && _tx <= 15) {
                    GPIOA_SetBits(1 << (_tx));
                    GPIOA_ModeCfg(1 << (_rx), GPIO_ModeIN_PU);
                    GPIOA_ModeCfg(1 << (_tx), GPIO_ModeOut_PP_5mA);
                } else if (_tx >= 16 && _tx <= 39) {
                    GPIOB_SetBits(1 << (_tx - 16));
                    GPIOB_ModeCfg(1 << (_rx - 16), GPIO_ModeIN_PU);
                    GPIOB_ModeCfg(1 << (_tx - 16), GPIO_ModeOut_PP_5mA);
                }
            }
            break;
    }

    return true;
}


void SerialUART::begin(unsigned long baud, uint16_t config)
{
    setPinout();
    _baud = baud;

    // 根据串口号初始化对应的UART
    switch (_uart_num) {
        case 0:
            UART0_BaudRateCfg(_baud);
            R8_UART0_FCR = (2 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN; // FIFO打开，触发级别4字节
            R8_UART0_LCR = RB_LCR_WORD_SZ;
            R8_UART0_IER = RB_IER_TXD_EN;
            R8_UART0_DIV = 1;

            UART0_ByteTrigCfg(UART_1BYTE_TRIG);
            UART0_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
            PFIC_EnableIRQ(UART0_IRQn);
            break;
        case 1:
            UART1_BaudRateCfg(_baud);
            R8_UART1_FCR = (2 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN; // FIFO打开，触发级别4字节
            R8_UART1_LCR = RB_LCR_WORD_SZ;
            R8_UART1_IER = RB_IER_TXD_EN;
            R8_UART1_DIV = 1;

            UART1_ByteTrigCfg(UART_1BYTE_TRIG);
            UART1_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
            PFIC_EnableIRQ(UART1_IRQn);
            break;
        case 2:
            UART2_BaudRateCfg(_baud);
            R8_UART2_FCR = (2 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN; // FIFO打开，触发级别4字节
            R8_UART2_LCR = RB_LCR_WORD_SZ;
            R8_UART2_IER = RB_IER_TXD_EN;
            R8_UART2_DIV = 1;

            UART2_ByteTrigCfg(UART_1BYTE_TRIG);
            UART2_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
            PFIC_EnableIRQ(UART2_IRQn);
            break;
        case 3:
            UART3_BaudRateCfg(_baud);
            R8_UART3_FCR = (2 << 6) | RB_FCR_TX_FIFO_CLR | RB_FCR_RX_FIFO_CLR | RB_FCR_FIFO_EN; // FIFO打开，触发级别4字节
            R8_UART3_LCR = RB_LCR_WORD_SZ;
            R8_UART3_IER = RB_IER_TXD_EN;
            R8_UART3_DIV = 1;

            UART3_ByteTrigCfg(UART_1BYTE_TRIG);
            UART3_INTCfg(ENABLE, RB_IER_RECV_RDY | RB_IER_LINE_STAT);
            PFIC_EnableIRQ(UART3_IRQn);
            break;
    }
    
    _running = true;
}

void SerialUART::end()
{
    _running = false;
    
    // 根据串口号禁用对应的UART中断
    switch (_uart_num) {
        case 0:
            PFIC_DisableIRQ(UART0_IRQn);
            break;
        case 1:
            PFIC_DisableIRQ(UART1_IRQn);
            break;
        case 2:
            PFIC_DisableIRQ(UART2_IRQn);
            break;
        case 3:
            PFIC_DisableIRQ(UART3_IRQn);
            break;
    }
}

int SerialUART::peek()
{
    if (_rx_buffer.available())
        return _rx_buffer.peek();
    else
        return -1;
}

int SerialUART::read()
{
    if (_rx_buffer.available())
        return _rx_buffer.read_char();
    else
        return -1;
}

int SerialUART::available()
{
    return _rx_buffer.available();
}

int SerialUART::availableForWrite()
{
    // 根据串口号检查发送FIFO是否满
    switch (_uart_num) {
        case 0:
            return UART_FIFO_SIZE - R8_UART0_TFC;
        case 1:
            return UART_FIFO_SIZE - R8_UART1_TFC;
        case 2:
            return UART_FIFO_SIZE - R8_UART2_TFC;
        case 3:
            return UART_FIFO_SIZE - R8_UART3_TFC;
        default:
            return -1;
    }
}

void SerialUART::flush()
{
    // 等待所有数据发送完成
    switch (_uart_num) {
        case 0:
            while (!(UART0_GetLinSTA() & STA_TXALL_EMP));
            break;
        case 1:
            while (!(UART1_GetLinSTA() & STA_TXALL_EMP));
            break;
        case 2:
            while (!(UART2_GetLinSTA() & STA_TXALL_EMP));
            break;
        case 3:
            while (!(UART3_GetLinSTA() & STA_TXALL_EMP));
            break;
    }
}

size_t SerialUART::write(uint8_t c)
{
    uint8_t* ptmp = &c;
    return write(ptmp , 1);
}

size_t SerialUART::write(const uint8_t *p, size_t len)
{
    if (!_running)
    {
        return 0;
    }
    size_t cnt = len;
    
    // 根据串口号发送数据
    switch (_uart_num) {
        case 0:
            while (cnt)
            {
                if(R8_UART0_TFC != UART_FIFO_SIZE)
                {
                  R8_UART0_THR = *p;
                  cnt--;
                  p++;
                }
            }
            break;
        case 1:
            while (cnt)
            {
                if(R8_UART1_TFC != UART_FIFO_SIZE)
                {
                  R8_UART1_THR = *p;
                  cnt--;
                  p++;
                }
            }
            break;
        case 2:
            while (cnt)
            {
                if(R8_UART2_TFC != UART_FIFO_SIZE)
                {
                  R8_UART2_THR = *p;
                  cnt--;
                  p++;
                }
            }
            break;
        case 3:
            while (cnt)
            {
                if(R8_UART3_TFC != UART_FIFO_SIZE)
                {
                  R8_UART3_THR = *p;
                  cnt--;
                  p++;
                }
            }
            break;
    }
    
    return len;
}

SerialUART::operator bool()
{
    return _running;
}

}

/*********************************************************************
 * @fn      UART0_IRQHandler
 *
 * @brief   UART0中断函数
 *
 * @return  none
 */
extern "C" __INTERRUPT
__HIGH_CODE
void UART0_IRQHandler(void)
{
    volatile uint8_t i;

    switch(UART0_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART0_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            {
            uint8_t data = UART0_RecvByte();
            Serial0._rx_buffer.store_char(data);
            }
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;

        case UART_II_MODEM_CHG: // 只支持串口0
            break;

        default:
            break;
    }
}

/*********************************************************************
 * @fn      UART1_IRQHandler
 *
 * @brief   UART1中断函数
 *
 * @return  none
 */
extern "C" __INTERRUPT
__HIGH_CODE
void UART1_IRQHandler(void)
{
    volatile uint8_t i;

    switch(UART1_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART1_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            {
            uint8_t data = UART1_RecvByte();
            Serial1._rx_buffer.store_char(data);
            }
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;

        case UART_II_MODEM_CHG: // 只支持串口0
            break;

        default:
            break;
    }
}

/*********************************************************************
 * @fn      UART2_IRQHandler
 *
 * @brief   UART2中断函数
 *
 * @return  none
 */
extern "C" __INTERRUPT
__HIGH_CODE
void UART2_IRQHandler(void)
{
    volatile uint8_t i;

    switch(UART2_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART2_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            {
            uint8_t data = UART2_RecvByte();
            Serial2._rx_buffer.store_char(data);
            }
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;

        case UART_II_MODEM_CHG: // 只支持串口0
            break;

        default:
            break;
    }
}

/*********************************************************************
 * @fn      UART3_IRQHandler
 *
 * @brief   UART3中断函数
 *
 * @return  none
 */
extern "C" __INTERRUPT
__HIGH_CODE
void UART3_IRQHandler(void)
{
    volatile uint8_t i;

    switch(UART3_GetITFlag())
    {
        case UART_II_LINE_STAT: // 线路状态错误
        {
            UART3_GetLinSTA();
            break;
        }

        case UART_II_RECV_RDY: // 数据达到设置触发点
        case UART_II_RECV_TOUT: // 接收超时，暂时一帧数据接收完成
            {
            uint8_t data = UART3_RecvByte();
            Serial3._rx_buffer.store_char(data);
            }
            break;

        case UART_II_THR_EMPTY: // 发送缓存区空，可继续发送
            break;

        case UART_II_MODEM_CHG: // 只支持串口0
            break;

        default:
            break;
    }
}


arduino::SerialUART Serial0(0); // UART0
arduino::SerialUART Serial1(1); // UART1
arduino::SerialUART Serial2(2); // UART2
arduino::SerialUART Serial3(3); // UART3