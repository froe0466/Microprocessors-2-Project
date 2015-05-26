/*
 * File:   LCDLab.c
 * Author: nepa6953
 * Created on January 21, 2015, 1:48 PM
 * Lab 3 - ENGR331
 * The goal of this lab is help you figure out
 * (on your own) how to interface to a peripheral
 * that you have never used before based on the
 * documentation you find (either datasheet or other
 * available sources/examples online).
 * ENJOY!!! (and I really mean it!)
 */
#define _XTAL_FREQ 10000000 //10MHz crystal
#include <string.h>
#include <delays.h>
#include "config.h"
#include <xc.h>


/*******************************
 * Global variables
 *******************************
 */
//variable to keep track of the current Y value of the display
int yPos = 0;

char binaryCount[8];
char intCount [8];

int display = 0;
/*******************************
 * FUNCTION PROTOTYPES
 *******************************
 */
// LIST Functions
void Delay100ms();
void LCD_port_init();
void LCD_init();
void LCD_write(unsigned char data[]);
void LCD_writeChar(unsigned char data);
void place_lcd_cursor(unsigned char x, unsigned char y);
void InitTimer0();
// END Functions

void main() {

    LCD_port_init();
    LCD_init();
    place_lcd_cursor(0,0);
    LCD_writeChar('h');


    //InitTimer0();
   // TRISDbits.RD3 = 1;

    while(1)
    {
        /*
        for(int i = 0; i < 8; i++)
        {
            binaryCount[i] = '0';
            intCount[i] = ' ';
        }

        char temp = display;
        int j = 7;
        while(temp > 0)
        {
            intCount[j] = temp % 10 + 0b00110000;
            temp /= 10;
            j--;
        }

        temp = display;
        j = 7;
        while(temp > 0)
        {
            binaryCount[j] = temp % 2 + 0b00110000;
            temp /= 2;
            j--;
        }


        if(PORTDbits.RD3 == 0)
        {
           display = TMR0;
           Delay100ms();
           while(PORTDbits.RD3 == 0);
           Delay100ms();
        }

           place_lcd_cursor(0,1);
           LCD_write(intCount);
           place_lcd_cursor(0,0);
           LCD_write(binaryCount);

         Delay100ms();
         */
    }
}

void LCD_port_init(){

    //Initialize Ports as 0
    LATE = 0;
    LATD = 0;

    //Set Ports as outputs
    TRISE = 0;
    TRISD = 0;
}

void LCD_init()
{
    // wait for Power on reset
    Delay100ms();

    //Hold RS in a low state
    PORTEbits.RE0 = 0;

    //Function Set in 8-bit mode
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x30; PORTEbits.RE1 = 0;
    __delay_ms(10);

    //Function Set in 8-bit mode
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x30; PORTEbits.RE1 = 0;
    __delay_ms(10);

    //Function Set in 8-bit mode
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x30; PORTEbits.RE1 = 0;
    __delay_ms(10);

    //4-bit Initialization
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x20; PORTEbits.RE1 = 0;
    __delay_ms(10);

    //Function Set, specify display size and font
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x20; PORTEbits.RE1 = 0;
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x80; PORTEbits.RE1 = 0;
    __delay_ms(10);

    //Display ON
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x00; PORTEbits.RE1 = 0;
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0xF0; PORTEbits.RE1 = 0;
    __delay_ms(10);

    //Clear Display
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x00; PORTEbits.RE1 = 0;
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x10; PORTEbits.RE1 = 0;
    __delay_ms(10);
    
    //Entry Mode Set
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x00; PORTEbits.RE1 = 0;
    PORTEbits.RE1 = 1; PORTD = (PORTD & 0b00001111) | 0x60; PORTEbits.RE1 = 0;
    __delay_ms(10);

    //Set DDRAM Address
    place_lcd_cursor(0,0);
}

void LCD_write(unsigned char data[])
{
    //Create a variable to track the yPos, this only works if we begin on line 0

    for(int i=0; data[i]!='\0'; i++)
    {
        if(data[i] == '\n')
        {
            // if we encounter a new line character, increment display line
            ++yPos;
            place_lcd_cursor(0,yPos);
        }
        else
        {
            // call our character writing function to display the next char
            LCD_writeChar(data[i]);
        }
    }
}


void LCD_writeChar(unsigned char data)
{
    //split incoming data into 2 seperate bytes
    unsigned char upper = data & 0b11110000;
    unsigned char lower = (data << 4);

    //clear pins and send upper and lower bytes using 4-bit interface
    LATEbits.LE0 = 1;
    LATEbits.LE1 = 1; LATD = (LATD & 0b00001111) | upper; LATEbits.LE1 = 0;
    LATEbits.LE1 = 1; LATD = (LATD & 0b00001111) | lower; LATEbits.LE1 = 0;
    __delay_ms(1);
    LATEbits.LE0 = 0;
}


void place_lcd_cursor(unsigned char x, unsigned char y)
{
    //Set global y value to set y value
    yPos = y;

    //calculate hex value of position based on x and y coords
    unsigned char position = (0x40*y)+x;
    LATEbits.LE0 = 0;

    //send upper bits to DDRM, making sure to set MSB to 1
    LATEbits.LE1 = 1;
    LATD = LATD & 0b00001111;
    LATD = (LATD | (position & 0b11110000)) | 0b10000000;
    LATEbits.LE1 = 0;

    //send lower bits to DDRM
    LATEbits.LE1 = 1;
    LATD = (LATD & 0b00001111) | (position << 4);
    LATEbits.LE1 = 0;
    __delay_ms(1);
}

void InitTimer0()
{
    //Set prescaler
    T0PS0 = 1;
    T0PS1 = 1;
    T0PS2 = 1;

    //Timer source is from prescaler
    PSA = 0;

    //Prescaler gets clock from Chip clock
    T0CS = 0;

    //8-bit mode
    T08BIT = 1;

    //Disable interrupt
    TMR0IE = 0;
    PEIE = 0;

    //enable ints
    GIE = 1;

    //Start timer0
    TMR0ON = 1;

}

void Delay100ms()
{   int i;
    for(i=0;i<100;i++)
    {
         __delay_ms(1); // built in delay function
		 // for other functions type __del and press CTRL & SPACE.
    }
}

