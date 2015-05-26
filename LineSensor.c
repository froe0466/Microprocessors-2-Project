#define _XTAL_FREQ 10000000 //10MHz crystal
#include "config.h" //Header file for the configuration bits
#include "LineSensor.h"

char whiteLineThreshold = 125;

void lineSensor_init()
{
    ADCON2=0b10111010;
}

int lineSensor_read(unsigned int Channel)
{
    ADCON0=0x00;
    ADCON0=(Channel<<2);

    ADON=1;

    GODONE=1;

    while(GODONE);

    ADON=0;

    return  ADRESL;
}

bool lineSensor_isWhiteLine1()
{
    bool isWhiteLine = FALSE;

    if (lineSensor_read(0) < whiteLineThreshold) {isWhiteLine = TRUE;}

    return isWhiteLine;
}

bool lineSensor_isWhiteLine2()
{
   bool isWhiteLine = FALSE;

   if (lineSensor_read(1) < whiteLineThreshold) {isWhiteLine = TRUE;}

   return isWhiteLine;
}