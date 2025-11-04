#ifndef __UART__
#define __UART__

#define READ_UART       1
#define WRITE_UART      1<<1
#define READ_WRITE_UART 1<<2

#include <SerialPort.h>


/*
enum BaudRateEnum 
{
    BAUD_50    = BAUD_50,
    BAUD_75    = BAUD_75,
    BAUD_110   = BAUD_110,
    BAUD_134   = BAUD_134,
    BAUD_150   = BAUD_150,
    BAUD_200   = BAUD_200,
    BAUD_300   = BAUD_300,
    BAUD_600   = BAUD_600,
    BAUD_1200  = BAUD_1200,
    BAUD_1800  = BAUD_1800,
    BAUD_2400  = BAUD_2400,
    BAUD_4800  = BAUD_4800,
    BAUD_9600  = BAUD_9600,
    BAUD_19200 = BAUD_19200,
    BAUD_38400 = BAUD_38400,
    BAUD_57600 = BAUD_57600,
    BAUD_115200 = BAUD_115200,
    BAUD_230400 = BAUD_230400
};

enum CharSizeEnum 
{
    CHAR_SIZE_5 = CHAR_SIZE_5,
    CHAR_SIZE_6 = CHAR_SIZE_6,
    CHAR_SIZE_7 = CHAR_SIZE_7,
    CHAR_SIZE_8 = CHAR_SIZE_8,
    CHAR_SIZE_DEFAULT = CHAR_SIZE_DEFAULT,
    CHAR_SIZE_INVALID
} ;

enum ParityEnum 
{
    PARITY_EVEN = PARITY_EVEN,
    PARITY_ODD  = PARITY_ODD,
    PARITY_NONE = PARITY_NONE,
    PARITY_DEFAULT = PARITY_DEFAULT,
    PARITY_INVALID   
} ;

enum FlowControlEnum 
{
    FLOW_CONTROL_HARD    = FLOW_CONTROL_HARD,
    FLOW_CONTROL_SOFT    = FLOW_CONTROL_SOFT,
    FLOW_CONTROL_NONE    = FLOW_CONTROL_NONE,
    FLOW_CONTROL_DEFAULT = FLOW_CONTROL_DEFAULT,
    FLOW_CONTROL_INVALID 
} ;
*/
static int ReadTimeOut = 20;
int OpenPort(std::string, int);
int ConfigurePort(LibSerial::BaudRate BaudRate, LibSerial::CharacterSize CharSize,
    LibSerial::Parity Parity, int StopBits, LibSerial::FlowControl FlowControl);
int WriteBytes(char*, int);
int ReadBytes(int, char*);
int ClosePort();
#endif
