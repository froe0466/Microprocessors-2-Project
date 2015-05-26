#define _XTAL_FREQ 10000000 //10MHz crystal
#include "config.h" //Header file for the configuration bits
#include "Timer.h"

void Timer_Init()
{

    T0CON = 0b11010111;
    TMR0IP = 1;
    TMR0IF = 0;
    TMR0IE = 1;
    TMR0L = 246;

}