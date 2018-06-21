/*********************************************
 * @file COM.h                               *
 *                                           *
 * COM port Communication                    *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/

#ifndef _COM_H_
#define _COM_H_

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
/* using Windows OS API */
#include <windows.h>

/* Unsigned.  */
typedef unsigned char                uint8_t;
typedef unsigned short int           uint16_t;

class COMPort
{

public:
    // Constructor
    COMPort(char* Port);

    // Destructor
    ~COMPort();

    // Function
    bool openPort(const char* Port);
    bool closePort();
    bool setupPort(uint16_t Baud, uint8_t DataBits, uint8_t Parity, uint8_t StopBits);
    DWORD uartRX(uint8_t * Buffer, int BufferSize);
    DWORD uartTX(uint8_t * Data, int Length);
    const char* getPort();
    const HANDLE getHandle();

private:
    /* variables used with the com port */
    char* _port;
    HANDLE _hComm;
    BOOL _bPortReady;
    DCB  _dcb;
    COMMTIMEOUTS _commTimeouts;
};

#endif // _COM_H_