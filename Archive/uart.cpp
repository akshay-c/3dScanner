#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <errno.h>
#include <termios.h>
#include <opencv2/highgui/highgui.hpp>
#include <SerialStream.h>
#include <SerialPort.h>

#include "uart.hpp"
#include "idle.hpp"


// SerialPort serial_port("/dev/ttyACM0");

using namespace LibSerial;
SerialStream serial_port;
int OpenPort(std::string Name, int mode)
{
    std::ios_base::openmode OpenMode;
    switch(mode)
    {
        case READ_UART:
            OpenMode = std::ios::in;
        break;
        case WRITE_UART:
            OpenMode = std::ios::out;
        break;
        case READ_WRITE_UART:
            OpenMode = std::ios::in |  std::ios::out;
        break;
        default:
            return -1;
    }
    serial_port.Open(Name,OpenMode);
    // EventSleep(2);
    // printf("CurrentVTime: %d\n",serial_port.VTime());
    /*int fd;
    
    fd = open("/dev/ttyACM0", O_RDWR | O_NDELAY);
    if(fd == -1)
    {
        printf("open_port: Unable to open /dev/ttyS0. \n");
    }
    else
    {
        fcntl(fd, F_SETFL, 0);
        printf("port is open.\n");
    }*/
    /*serial_port.Open(SerialPort::BAUD_115200) ;
    // sleep(2);
    MarkedTimeStamp =clock();
    EventSleep(2);
    if ( ! serial_port.IsOpen() )
    {
        printf("Could Not Open Serial Port\n");
        return -1;
    }
    printf("port is open.\n");*/


    return(0);
    
    //return(fd);
}

int ConfigurePort(int BaudRate, int CharSize,
    int Parity, int StopBits, int FlowControl)
{

    serial_port.SetBaudRate((SerialStreamBuf::BaudRateEnum)BaudRate);
    serial_port.SetCharSize((SerialStreamBuf::CharSizeEnum)CharSize);
    serial_port.SetParity((SerialStreamBuf::ParityEnum)Parity);
    serial_port.SetNumOfStopBits(StopBits);
    // serial_port.SetNumOfStopBits(1);
    serial_port.SetFlowControl((SerialStreamBuf::FlowControlEnum)FlowControl);
    if(!serial_port.good())
    {
        printf("Error Setting Baudrate\n");
    }
    // EventSleep(2);
    /*struct termios port_settings;
    cfsetispeed(&port_settings, B9600);
    cfsetospeed(&port_settings, B9600);

    port_settings.c_cflag &= ~PARENB;
    port_settings.c_cflag &= ~CSTOPB;
    port_settings.c_cflag &= ~CSIZE;
    port_settings.c_cflag |= CS8;
    
    tcsetattr(fd, TCSANOW, &port_settings);
    return(fd);*/
    // serial_port.SetBaudRate(SerialStreamBuf::BAUD_9600);
    // if ( ! serial_port.isOpen() )
    // {
    //     printf("Could Not Set BaudRate\n");
    // }
    // serial_port.SetCharSize(SerialStreamBuf::CHAR_SIZE_8);
    // serial_port.SetParity(SerialStreamBuf::PARITY_NONE);
    // serial_port.SetNumOfStopBits( 1 );
    // serial_port.SetFlowControl( SerialStreamBuf::FLOW_CONTROL_NONE);
    /*while( serial_port.rdbuf()->in_avail() == 0 )
     {
         usleep(100) ;
     }
     char out_buf[] = "check";*/
    /*char temp;
    try
    {
        temp=serial_port.ReadByte(20000);
    }
    catch(SerialPort::ReadTimeout &e)
    {
        //cout<<"Read Timeout"<<endl;
        printf("Read Timeout");
        return 1;
    }*/

    // if ( ! serial_port.isOpen() )
    // {
    //     printf("Could Not Set BaudRate\n");
    // }

    return 0;

}

int WriteBytes(char *buf, int len)
{
    /*int val = 360;
    unsigned char send_bytes[] = {
        'p', (char)(val>>8), (char)val};
    
    // write(fd, send_bytes, 3);
    printf("Writing bytes. \n");
    serial_port.WriteByte(send_bytes[0]);*/
    // while( 1  )
    //  {
    //      char next_byte;
    //      serial_port.get(next_byte);
    //      printf("Byte Recived %c",next_byte);

    //  }
    serial_port.write(buf, len);
    printf("Wrote the bytes. \n");

    return 0;
    
}
int ReadBytes(int len, char *bytes)
{
    /*char n = -1;
    SerialPort::DataBuffer data;
    //char value;
    int output;

    serial_port.Read(data, 1, 20000);*/
    /*fd_set rdfs;

    struct timeval timeout;
    timeout.tv_sec = 50;
    timeout.tv_usec = 0;
    sleep(2);
    tcflush(fd,TCIOFLUSH);
*/
    /*SerialPort::DataBuffer data;
    try
    {
        // temp=serial_port.ReadByte(20000);
        serial_port.Read(data, 1, 20000);
        printf("Read data %c\n",data[0]);
    }
    catch(SerialPort::ReadTimeout &e)
    {
        //cout<<"Read Timeout"<<endl;
        printf("Read Timeout\n");
        return 1;
    }
    catch(std::runtime_error &e)
    {
        //cout<<"Read Timeout"<<endl;
        printf("Runtime Error\n");
        return 1;
    }
    catch(SerialPort::NotOpen &e)
    {
        //cout<<"Read Timeout"<<endl;
        printf("Not Open");
        return 1;
    }
    catch(std::exception &e)
    {
        //cout<<"Read Timeout"<<endl;
        printf("Exception");
        return 1;
    }*/
    // n = select(fd + 1, &rdfs, NULL, NULL, &timeout);
    // while(1)
    // {
    //     if(read(fd, &value, 1))

        /*output = (data[1]<<8) | data[0];
            printf("Rotated: %d\n",output);*/
    // }
/*    while(1)
        if (read(fd, &value, 1)) printf("%i\n", value);*/

    char next_byte;
    /*if(len>1)
    {
        serial_port.SetVTime(32767);
    }*/

    for(int i = 0; i<len; i++)
    {
        serial_port.get(next_byte);
        *bytes++ = next_byte;
    }
    printf("Read %d Bytes\n",len);
    // printf("inchar:%c\n",bytes[2]);
    // serial_port.SetVTime(0);
}
int ClosePort()
{
    serial_port.Close();
    return 0;
}