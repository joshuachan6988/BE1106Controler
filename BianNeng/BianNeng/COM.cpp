/*********************************************
 * @file Driver.cpp                          *
 *                                           *
 * Motor Contorler & COM port Communication  *
 *                                           *
 * @author Joshua <jan.joshua@iac.com.tw>    *
 *********************************************/
#include "stdafx.h"
#include "COM.h"

#include <math.h>
#include <time.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/* using Windows OS API */
#include <windows.h>

/* Unsigned.  */
typedef unsigned char                uint8_t;
typedef unsigned short int           uint16_t;

COMPort::COMPort(char* Port)
{
    // Record port to be used
    _port = new char[sizeof(Port)];
    for(int i = 0; i <= sizeof(Port); i++)
    {
        _port[i] = Port[i];
    }

    // Open port
    if(openPort(Port))
    {
        printf("[INFO] COMPort: openPort(%s) Successed\n", Port);
    }
}

COMPort::~COMPort()
{
    // Release memory if it's needed
    closePort();
    delete [] _port;

    printf("[INFO] COMPort: COMPort Class Destroyed\n");
    printf("[INFO] COMPort: COMPort Class Memory Release\n");
}

bool COMPort::openPort(const char* Port)
{
    _hComm = NULL;

    wchar_t* wString = new wchar_t[4096];
	MultiByteToWideChar(CP_ACP, 0, Port, -1, wString, 4096);
	_hComm = CreateFile( wString,                       // Port Name
						 GENERIC_READ | GENERIC_WRITE,  // Read/Write
						 0,                             // Exclusive Access or No Sharing
						 NULL,                          // No Security
						 OPEN_EXISTING,                 // Open existing port only
						 FILE_ATTRIBUTE_NORMAL,         // No Overlapped I/O
						 NULL);                         // Null for Comm Devices

    if((int)_hComm == -1)
    {
        printf("[ERROR] COMPort: %s Port Does Not Exist Or Be Using\n", Port);
        return false;
    }

    return true;
}

bool COMPort::closePort()
{
    if(_hComm != INVALID_HANDLE_VALUE)
    {
        CloseHandle(_hComm);
    }
    else
    {
        return false;
    }

    printf("[INFO] COMPort: closePort(%s) Successed\n", _port);

    return true;
}

bool COMPort::setupPort(uint16_t Baud, uint8_t DataBits, uint8_t Parity, uint8_t StopBits)
{
    if((int)_hComm < 0)
    {
        return false;
    }
    
    // Struct termios options
    if(!SetupComm(_hComm, 128, 128))
    {
        printf("[ERROR] COMPort: setupPort(SetupComm) Failed\n");
        return false;
    }

    // SecureZeroMemory(&dcb, sizeof(DCB));
    _dcb.DCBlength = sizeof(DCB);

    if(!GetCommState(_hComm, &_dcb))
    {
        printf("[ERROR] COMPort: setupPort(GetCommState) Failed\n");
        return false;
    }

    _dcb.BaudRate = Baud;
    _dcb.ByteSize = DataBits;
    _dcb.Parity = Parity;
    _dcb.StopBits = StopBits;
    _dcb.fAbortOnError = TRUE;
    _dcb.fBinary = TRUE;

    // set XON/XOFF
    _dcb.fOutX = FALSE;
    _dcb.fInX = FALSE;
    _dcb.fOutxCtsFlow = FALSE;
    _dcb.fRtsControl = RTS_CONTROL_DISABLE;
    _dcb.fOutxDsrFlow = FALSE;
    _dcb.fDtrControl = DTR_CONTROL_DISABLE;

    _bPortReady = SetCommState(_hComm, &_dcb);

    // Communication timeouts are optional 
    _commTimeouts.ReadIntervalTimeout = 100;
    _commTimeouts.ReadTotalTimeoutConstant = 0;
    _commTimeouts.ReadTotalTimeoutMultiplier = 500;
    _commTimeouts.WriteTotalTimeoutConstant = 0;
    _commTimeouts.WriteTotalTimeoutMultiplier = 500;

    if(!SetCommTimeouts (_hComm, &_commTimeouts))
    {
        printf("[ERROR] COMPort: setupPort(SetCommTimeouts) Failed\n");
        return false;
    }

    if(!GetCommState(_hComm, &_dcb))
    {
        printf("[ERROR] COMPort: setupPort(GetCommState) Failed\n");
        return false;
    }

    printf("[INFO] COMPort: setupPort(%s) Successed\n", _port);
    
    return true;
}

/**
    Reading:
    To tell windows that data is going to be send over the serial port, the handle, the data and the amount of data are needed.
    The following transmission is a simple fileIO operation.
 */

DWORD COMPort::uartRX(uint8_t * Buffer, int BufferSize)
{
    DWORD dwBytesRead = 0;

    if(!ReadFile(_hComm, Buffer, BufferSize, &dwBytesRead, NULL))
    {
        //handle error
    }

    return dwBytesRead;
}

/**
    Writing
    The same information is needed when writing to the port.
 */
DWORD COMPort::uartTX(uint8_t * Data, int Length)
{

    DWORD dwBytesRead = 0;

    if(!WriteFile(_hComm, Data, Length, &dwBytesRead, NULL))
    {
        //handle error
    }

    return dwBytesRead;

}

const char* COMPort::getPort()
{
    return _port;
}

const HANDLE COMPort::getHandle()
{
    return _hComm;
}