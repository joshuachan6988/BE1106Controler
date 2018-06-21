#ifndef _BIANNENG_H_
#define _BIANNENG_H_

#include <math.h>
#include <time.h>
#include <stdlib.h>
#include <string.h>
/* using Windows OS API */
#include <windows.h>

/* Unsigned.  */
typedef unsigned char                uint8_t;
typedef unsigned short int           uint16_t;

/**
    Motor Driver Message Structure
*/
typedef struct message {
    int length;
    uint8_t* content;
} Message;


// BianNeng Motor Controler
class BianNeng
{

public:

    // Constructor
    BianNeng();

    // Destructor
    ~BianNeng();

    // Command Transfer
    void Chararray2Hexarray(const char * Input, uint8_t * Output, int Length);
	uint8_t countCRC(uint8_t * Data, int Length);
    Message trans2RTCMD(const char* Prescaler, const char* Address, const char* TotalPulse
                        , const char* AcceleratePulse, const char* BreakPulse);
    Message trans2PRCMD(const char* Prescaler, const char* Address, const char* TotalPulse
                        , const char* AcceleratePulse, const char* BreakPulse
                        , const char* PositiveStay, const char* NegativeStay);
    Message trans2SQCMD(const char* Address, const char* Content);

private:
	// Private Variable

};

#endif // _BIANNENG_H_