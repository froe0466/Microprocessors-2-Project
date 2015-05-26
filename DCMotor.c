
#define _XTAL_FREQ 10000000 //10MHz crystal
#include "config.h" //Header file for the configuration bits
#include "DCMotor.h"

unsigned char coastSpeed = 65;

void EPWM_Port_Init(void)
{
    LATA = LATA & 0x00;
    TRISA = TRISA & 0xFF;
    ANSELA = ANSELA & 0x2F;

    LATB = LATB & 0x00;
    TRISB = TRISB & 0xDF;
    ANSELB = ANSELB & 0x1F;
    WPUB = 0x00;

    LATCbits.LC1 = 0; 
    TRISCbits.RC1 = 0;  
    ANSELCbits.ANSC2 = 0;

    LATD = LATD & 0x00;
    TRISD = TRISD & 0xFB;
    ANSELD = ANSELD & 0xFB;

    LATE = LATE & 0x00;
    TRISE = TRISE & 0x05;
    ANSELE = ANSELE & 0x05;

    INTCON2bits.nRBPU = 0x01;
}


void EPWM1_Initialize (void)
{
    // Set the PWM to the options selected in MPLAB® Code Configurator
    // P3M halfbridge; CCP3M P3AP3Chi_P3BP3Dhi; DC3B 48;
    CCP3CON = 0xBC;

    // PSS3AC P3AP3C_0; CCP3ASE operating; CCP3AS disabled; PSS3BD P3BP3D_0;
    ECCP3AS = 0x00;

    // P3DC 0; P3RSEN automatic_restart;
    PWM3CON = 0x80;

    // STR3SYNC start_at_begin; STR3C P3C_to_port; STR3B P3B_to_port; STR3D P3D_to_port; STR3A P3A_to_port;
    PSTR3CON = 0x00;

    // CCPR3L 127;
    CCPR3L = 0x7F;

    // CCPR3H 0x0;
    CCPR3H = 0x00;

    // Selecting Timer2
    CCPTMRS0bits.C3TSEL = 0x0;

    //Initialize at Forward mode
    CCP3CON = 0b11001100;    
}

void EPWM1_LoadDutyValue(int dutyValue)
{
    dutyValue = dutyValue * 6.4;

   // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR3L = ((dutyValue & 0x03FC)>>2);

   // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP3CON = (CCP3CON & 0xCF) | ((dutyValue & 0x0003)<<4);
}

void TMR2_Initialize(void)
{
    // Set TMR2 to the options selected in the User Interface
    // TMR2ON off; T2CKPS 1:1; T2OUTPS 1:1;
    T2CON = 0x00;

    // PR2 255;
    PR2 = 0xA0;

    // TMR2 0x0;
    TMR2 = 0xFF;

    // Clearing IF flag.
    PIR1bits.TMR2IF = 0;

    // Start TMR2
    T2CONbits.TMR2ON = 1;
}

void Switch_Direction1(void)
{
    //Lower Duty Value to 50% - Account for
    EPWM1_LoadDutyValue(40);
    
    //Toggle Bit 8 in the CCP1CON register to change bwteen
    //forward and reverse mode
    CCP3CON = CCP3CON ^ 0b10000000;
}

void EPWM2_Initialize (void)
{
    // Set the PWM to the options selected in MPLAB® Code Configurator
    // CCP2M P2AP2Chi_P2BP2Dhi; P2M halfbridge; DC2B 48;
    CCP2CON = 0xBC;

    // PSS2BD P2BP2D_0; CCP2AS disabled; CCP2ASE operating; PSS2AC P2AP2C_0;
    ECCP2AS = 0x00;

    // P2RSEN automatic_restart; P2DC 0;
    PWM2CON = 0x80;

    // STR2D P2D_to_port; STR2C P2C_to_port; STR2B P2B_to_port; STR2A P2A_to_port; STR2SYNC start_at_begin;
    PSTR2CON = 0x00;

    // CCPR2L 127;
    CCPR2L = 0x7F;

    // CCPR2H 0x0;
    CCPR2H = 0x00;

    // Selecting Timer4
    CCPTMRS0bits.C2TSEL = 0x1;

    //Initialize at Forward mode
    CCP2CON = 0b11001100; ///CHECK TO MAKE SURE THIS IS WORKING
}

void EPWM2_LoadDutyValue(int dutyValue)
{
    dutyValue = dutyValue * 6.4;

   // Writing to 8 MSBs of pwm duty cycle in CCPRL register
    CCPR2L = ((dutyValue & 0x03FC)>>2);

   // Writing to 2 LSBs of pwm duty cycle in CCPCON register
    CCP2CON = (CCP2CON & 0xCF) | ((dutyValue & 0x0003)<<4);
}

void TMR4_Initialize(void)
{
    // Set TMR4 to the options selected in the User Interface
    // T4CKPS 1:1; T4OUTPS 1:1; TMR4ON off;
    T4CON = 0x00;

    // PR4 255;
    PR4 = 0xA0; //WAS FF

    // TMR4 0x0;
    TMR4 = 0xFF; //WAS 00

    // Clearing IF flag.
    PIR5bits.TMR4IF = 0;

    // Start TMR2
    T4CONbits.TMR4ON = 1;
}

void Switch_Direction2(void)
{
    //Lower Duty Value to 50% - Account for
    EPWM2_LoadDutyValue(40);

    //Toggle Bit 8 in the CCP1CON register to change bwteen
    //forward and reverse mode
    CCP2CON = CCP2CON ^ 0b10000000; 
}

void Turn_Right(int duration, int speed)
{
    CCP2CON = 0b11001100;
    CCP3CON = 0b11001100;
    
    EPWM1_LoadDutyValue(speed);
    EPWM2_LoadDutyValue(00);

    for(int i =0; i<duration; i++) { __delay_ms(10); }
    coast();
}

void Turn_Left(int duration, int speed)
{
    CCP2CON = 0b11001100;
    CCP3CON = 0b11001100;

    EPWM1_LoadDutyValue(00);
    EPWM2_LoadDutyValue(speed);

    for(int i =0; i<duration; i++){ __delay_ms(10); }
    coast();
}

//Takes a direction of either 0, 1, -1.
//0 for straight forward
//1 for left
//-1 for right
void Drive_Forward(int direction, int distance, int speed)
{
    CCP2CON = 0b11001100;
    CCP3CON = 0b11001100;

    EPWM1_LoadDutyValue(speed + (direction * 5));
    EPWM2_LoadDutyValue(speed);

    while(distance > 0)
    {
        __delay_ms(10);
        distance--;
    }
    coast();
}

void Drive_Reverse(int distance, int speed)
{
    CCP2CON = 0b01001100;
    CCP3CON = 0b01001100;

    EPWM1_LoadDutyValue(speed);
    EPWM2_LoadDutyValue(speed);

    while(distance > 0)
    {
        __delay_ms(10);
        distance--;
    }
}

void Pivot(int rotations, int speed)
{
    unsigned int bits = 0;

    while(rotations > 0)
    {
        CCP2CON = 0b11001100 ^ bits;
        EPWM1_LoadDutyValue(speed);

        CCP3CON = 0b01001100 ^ bits;
        EPWM2_LoadDutyValue(speed);

        Stop();

        if(rotations % 2 == 0){ bits = 0b10000000; }
        else {  bits = 0; }

        rotations--;
    }
}

void Stop()
{
    CCP2CON = 0b11001100;
    CCP3CON = 0b11001100;

    EPWM1_LoadDutyValue(00);
    EPWM2_LoadDutyValue(00);
}

void coast()
{
    EPWM1_LoadDutyValue(coastSpeed);
    EPWM2_LoadDutyValue(coastSpeed);
}

