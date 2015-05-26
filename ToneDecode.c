#define _XTAL_FREQ 10000000 //10MHz crystal
#include "config.h" //Header file for the configuration bits
#include "ToneDecode.h"

void MicInit()
{

    ADCON2=0b10111010;
    TRISAbits.RA2=1;
    TRISAbits.RA3=1;

}

int Mic_read25(void) //East Wing
{
    int Val;
    ADCON0=0x00;
    ADCON0 = 0b00001100;

    ADON=1;

    GODONE=1;

    while(GODONE);

    ADON=0;

    Val =  ADRESL | ((ADRESH&0b00000011)<<8);

    return Val;
}

int Mic_read38(void) //West Wing
{
    ADCON0=0x00;
    ADCON0= 0b00001000;

    ADON=1;

    GODONE=1;

    while(GODONE);

    ADON=0;

    return ADRESL;
}