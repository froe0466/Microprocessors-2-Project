#define _XTAL_FREQ 10000000 //10MHz crystal
#include "config.h" //Header file for the configuration bits
#include "SRF04.h"
#include "LCD.h"

void SRF04_Setup(){

    LATBbits.LB4 = 0; //Trigger
    LATBbits.LB3 = 0; //Echo
    LATBbits.LB2 = 0; //Trigger
    LATBbits.LB1 = 0; //Echo

    TRISBbits.RB4 = 0; //Trigger
    TRISBbits.RB3 = 1; //Echo
    TRISBbits.RB2 = 0; //Trigger
    TRISBbits.RB1 = 1; //Echo

    ANSELBbits.ANSB4 = 0; //Digital
    ANSELBbits.ANSB3 = 0; //Digital
    ANSELBbits.ANSB2 = 0; //Digital
    ANSELBbits.ANSB1 = 0; //Digital

    T1CON = 0b00000000;
    T3CON = 0b00000000;
}

int Send_Pulse_Left(void){

    int L = 0;

    TMR1H = 0;
    TMR1L = 0;

    PORTBbits.RB2 = 1;
    __delay_us(10);
    PORTBbits.RB2 = 0;

    while(PORTBbits.RB1 == 0);
    T1CONbits.TMR1ON = 1; //Start Running TMR1
    while(PORTBbits.RB1);
    T1CONbits.TMR1ON = 0; //Stop running TMR1

    L = (TMR1L | (TMR1H<<8));

    return L / 58;
}

int Send_Pulse_Right(void){

    int R = 0;

    TMR3H = 0;
    TMR3L = 0;

    PORTBbits.RB4 = 1;
    __delay_us(10);
    PORTBbits.RB4 = 0;

    while(PORTBbits.RB3 == 0);
    T3CONbits.TMR3ON = 1; //Start Running TMR1
    while(PORTBbits.RB3);
    T3CONbits.TMR3ON = 0; //Stop running TMR1

    R = (TMR3L | (TMR3H<<8));

    return R / 58;
}
