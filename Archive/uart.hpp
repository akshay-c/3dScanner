#ifndef __UART__
#define __UART__

#define READ_UART       1
#define WRITE_UART      1<<1
#define READ_WRITE_UART 1<<2

#include <SerialPort.h>



enum BaudRateEnum 
{
    BAUD_50    = SerialPort::BAUD_50,
    BAUD_75    = SerialPort::BAUD_75,
    BAUD_110   = SerialPort::BAUD_110,
    BAUD_134   = SerialPort::BAUD_134,
    BAUD_150   = SerialPort::BAUD_150,
    BAUD_200   = SerialPort::BAUD_200,
    BAUD_300   = SerialPort::BAUD_300,
    BAUD_600   = SerialPort::BAUD_600,
    BAUD_1200  = SerialPort::BAUD_1200,
    BAUD_1800  = SerialPort::BAUD_1800,
    BAUD_2400  = SerialPort::BAUD_2400,
    BAUD_4800  = SerialPort::BAUD_4800,
    BAUD_9600  = SerialPort::BAUD_9600,
    BAUD_19200 = SerialPort::BAUD_19200,
    BAUD_38400 = SerialPort::BAUD_38400,
    BAUD_57600 = SerialPort::BAUD_57600,
    BAUD_115200 = SerialPort::BAUD_115200,
    BAUD_230400 = SerialPort::BAUD_230400
};

enum CharSizeEnum 
{
    CHAR_SIZE_5 = SerialPort::CHAR_SIZE_5,
    CHAR_SIZE_6 = SerialPort::CHAR_SIZE_6,
    CHAR_SIZE_7 = SerialPort::CHAR_SIZE_7,
    CHAR_SIZE_8 = SerialPort::CHAR_SIZE_8,
    CHAR_SIZE_DEFAULT = SerialPort::CHAR_SIZE_DEFAULT,
    CHAR_SIZE_INVALID
} ;

enum ParityEnum 
{
    PARITY_EVEN = SerialPort::PARITY_EVEN,
    PARITY_ODD  = SerialPort::PARITY_ODD,
    PARITY_NONE = SerialPort::PARITY_NONE,
    PARITY_DEFAULT = SerialPort::PARITY_DEFAULT,
    PARITY_INVALID   
} ;

enum FlowControlEnum 
{
    FLOW_CONTROL_HARD    = SerialPort::FLOW_CONTROL_HARD,
    FLOW_CONTROL_SOFT    = SerialPort::FLOW_CONTROL_SOFT,
    FLOW_CONTROL_NONE    = SerialPort::FLOW_CONTROL_NONE,
    FLOW_CONTROL_DEFAULT = SerialPort::FLOW_CONTROL_DEFAULT,
    FLOW_CONTROL_INVALID 
} ;

static int ReadTimeOut = 20;
int OpenPort(std::string, int);
int ConfigurePort(int, int, int, int, int);
int WriteBytes(char*, int);
int ReadBytes(int, char*);
int ClosePort();
#endif
