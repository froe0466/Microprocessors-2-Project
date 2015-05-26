/*
 * File:   Driver.c
 * Author: froe0466
 * Created on 4/13/2015
 * ENGR331 - Final Project
 * This code is the main driver of the robot and
 * controls the functionality and integration with
 * the motors and sensors
*/

#define _XTAL_FREQ 10000000 //10MHz crystal
#include "config.h"
#include <i2c.h> 
#include "i2c.h"
#include "LCD.h"

void i2c_Port_Init (void)
{
   LATDbits.LD0 = 0;  //   // prepare PORTD
   LATDbits.LD1 = 0;
   TRISDbits.RD0 = 0;  //    // set as Outputs
   TRISDbits.RD1 = 0;
   ANSELDbits.ANSD0 = 0;  //  // define PINS as Digital
   ANSELDbits.ANSD1 = 0;
 }

 // Initialise MSSP port
 void i2c_Init(void)
 {
    // Initialise I2C MSSP
    // Master 100KHz
    TRISDbits.TRISD0=1;             // set SCL and SDA pins as inputs
    TRISDbits.TRISD1=1;
    SSP2CON1bits.SSPEN = 1;         //Enable SSP Hardware
    SSP2CON1 = 0b00101000;     // I2C enabled, I2C Master mode, clock = FOSC / (4 * (SSPxADD+1))(
    SSP2CON2 = 0;                // reset all conditions
    //Formula for calculating desired ADD-Register value:
    //         <FOSC> / <desired I2C clock> / 4 - 1 = ADD
    SSP2ADD = 39;               //100kHz at 16MHz system clock eg 16,000,000/(4*(ADD+1)) = 100,000

    SSP2STAT = 0b10000000;     // Slew rate disabled (for 100kHz)
 }

 // i2c_Wait - wait for I2C transfer to finish
 void i2c_Wait(void)
 {
     while ( ( SSP2CON2 & 0x1F ) || ( SSP2STAT & 0x04 ) ); //only continues when there is not imortant bit set
 }

 // i2c_Start - Start I2C communication
 void i2c_Start(void)
 {
     i2c_Wait();
     SSP2CON2bits.SEN=1;
     while(SSP2CON2bits.SEN);
 }

 // i2c_Restart - Re-Start I2C communication
 void i2c_Restart(void)
 {
     i2c_Wait();
     SSP2CON2bits.RSEN=1;
     while(SSP2CON2bits.RSEN);
 }

 // i2c_Stop - Stop I2C communication
 void i2c_Stop(void)
 {
     i2c_Wait();
     SSP2CON2bits.PEN=1;
     while(SSP2CON2bits.PEN);
 }

 // i2c_Write - Sends one byte of data
 void i2c_Write(unsigned char data)
 {
     i2c_Wait();
     SSP2BUF = data;
 }

 void i2c_Command(unsigned char deviceAddress,unsigned char reg, unsigned char command)
 {
    i2c_Start();
    i2c_Write(deviceAddress);
    i2c_Wait();
    i2c_Write(reg);
    i2c_Wait();
    i2c_Write(command);
    i2c_Wait();
    i2c_Stop();

    //Wait for Command / Echo to complete
    __delay_ms(50);
    __delay_ms(50);
}

 char i2c_Read(unsigned char deviceAddress, unsigned char reg)
 {
    unsigned int rangeInCm = 0;
    unsigned int rangeInCm1 = 0;
    unsigned int range;

    //Send Read Command & Store in char array
    i2c_Start();
    i2c_Write(deviceAddress);
    i2c_Wait();
    i2c_Write(reg);
    i2c_Restart();
    i2c_Write(deviceAddress + 1);
    i2c_Wait();

    //Receive High Byte
    SSP2CON2bits.RCEN = 1;
    i2c_Wait();
    rangeInCm = SSP2BUF;

    //Send ACK
    SSP2CON2bits.ACKDT = 0;
    SSP2CON2bits.ACKEN = 1;

    i2c_Wait();

    //Receive Low Byte
    SSP2CON2bits.RCEN = 1;
    i2c_Wait();

    rangeInCm1 = SSP2BUF;

    //Send NACK
    SSP2CON2bits.ACKDT = 1;
    SSP2CON2bits.ACKEN = 1;

    i2c_Wait();
    i2c_Stop();

    range = rangeInCm<<8;
    range = range + rangeInCm1;

    //Write to LCD Screen
    place_lcd_cursor(0,1);
    LCD_writeChar((range / 100) + 0x30); //this is the routine for the first decimal place
    place_lcd_cursor(1,1);
    LCD_writeChar(((range % 100) / 10) + 0x30); //this is the routine for the second decimal place
    place_lcd_cursor(2,1);
    LCD_writeChar(((range % 100) % 10) + 0x30); //this is the routine for the third decimal place

    return range;
}