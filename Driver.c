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
#include "LCD.h"
#include "DCMotor.h"
#include "i2c.h"
#include "LineSensor.h"
#include "SRF04.h"
#include "ToneDecode.h"
#include "Timer.h"

void lineRoutine(void);

int leftRangeSRF04 = 0;
int rightRangeSRF04 = 0;
unsigned long mScount = 0;
int mSstart = 246;
//0 indicates North, 1 indicates South
int startSide = 0; 
unsigned char wallRange = 35;
unsigned char obstacleRange = 13;
//The set value that the microphone input must drop to in order to determine if the sound was of the correct frequency.
unsigned char soundRange = 10;
bool exit = false; //Boolean for main while loop

void main(void){

    bool activated = false;
    int direction = -1; //-1 indicates a right heading, 1 indicates a left heading
    char i2cResult = 0;
    
    int mic25 = 0;
    int mic38 = 0;
  
    LATAbits.LA4 = 0;
    TRISAbits.RA4 = 1;

    //Fan Toggle Bit
    LATDbits.LD7 = 0;
    TRISDbits.RD7 = 0;
    ANSELDbits.ANSD7 = 0;

    //ECCP Port Initialization
    EPWM_Port_Init();

    //ECCP1 Init Sequence
    EPWM1_Initialize();
    TMR2_Initialize();

    //ECCP2 Init Sequence
    EPWM2_Initialize();
    TMR4_Initialize();
    
    Stop();

    //LCD Init Sequence
    LCD_init();
    place_lcd_cursor(0,0);

    //i2c Initialization
    i2c_Port_Init();
    i2c_Init();

    //Line Sensor Initialization
    lineSensor_init();

    //Confirm that the Robot Initialization is complete
    LCD_write("READY...");

    //Ping Sensor Initilization and use
    SRF04_Setup();

    //Sound Activation Routine
    MicInit();

    while(!activated)
    {
        
        //Reads output of the microphone
        place_lcd_cursor(12,0);
        LCD_convertWrite(Mic_read25());
        
        place_lcd_cursor(12,1);
        LCD_convertWrite(Mic_read38());

        __delay_ms(50);__delay_ms(50);
        
        for(int i = 0; i < 100; i++)
        {
            if(Mic_read25() < soundRange){mic25++;}
            __delay_ms(15);
            if(Mic_read38() < soundRange){mic38++;}
        }

        switch(startSide)
        {
            case 0:
            if(mic25 > 95)
             {
                 LCD_clear();
                 place_lcd_cursor(0,1);
                 LCD_write("E Wing,Go L");
                 direction = 1;
                 activated = true;
             }

             if(mic38 > 95)
             {
                 LCD_clear();
                 place_lcd_cursor(0,1);
                 LCD_write("W Wing,Go R");
                 direction = -1;
                 activated = true;
             }
            break;

            case 1:
             if(mic25 > 95)
             {
                 LCD_clear();
                 place_lcd_cursor(0,1);
                 LCD_write("E Wing,Go R");
                 direction = -1;
                 activated = true;
             }

             if(mic38 > 95)
             {
                 LCD_clear();
                 place_lcd_cursor(0,1);
                 LCD_write("W Wing,Go L");
                 direction = 1;
                 activated = true;
             }
             break;

        }
            mic25=0;
            mic38=0;
    }//End Mic While Loop

    //Initializes and Starts the Field Timer
    //Timer_Init();
 
   while(!exit)
    {

     //Read Front i2c Sensor
     i2c_Command(0xE0,0x00,0x51);
     i2cResult = i2c_Read(0xE0,0x02);

     //Read Right Ping Sensor
     rightRangeSRF04 = Send_Pulse_Right();

     //Read Left Ping Sensor
     leftRangeSRF04 = Send_Pulse_Left();
       
     if(rightRangeSRF04 > wallRange && leftRangeSRF04 > wallRange && i2cResult > obstacleRange)
     {
        Drive_Forward(direction,40,65);
        Stop();
        if(direction == -1) {
            while (Send_Pulse_Right() >= wallRange && !exit)
            {
                //Check to make sure we are not going to hit the wall
                i2c_Command(0xE0,0x00,0x51);
                if (i2c_Read(0xE0,0x02) < 9) { Drive_Reverse(20,60); Turn_Left(30, 75); } //Changed left direction from 20
                
                //Find the wall - Turn in a spirl
                Turn_Right(25, 75); //Changed from 15 direction 1:58pm
                Drive_Forward(direction,10,65); //Changed from 15 at 2:06pm
                Stop();

                //Check for the white line
                lineRoutine();
            }
        }
        else if(direction == 1) {
            while (Send_Pulse_Left() >= wallRange && !exit)
            {
                //Check to make sure we are not going to hit the wall
                i2c_Command(0xE0,0x00,0x51);
                if (i2c_Read(0xE0,0x02) < 9) { Drive_Reverse(25,60); Turn_Right(70, 75); } //Changed right duration from 30
                
                //Find the wall - Turn in a spirl
                Turn_Left(25, 75); //Changed from 15 direction 1:58pm
                Drive_Forward(direction,10,65); //Changed from 15 at 2:06pm
                Stop();

                //Check for the white line
                lineRoutine();
            }
        }
     }

     else if(i2cResult < obstacleRange && !exit)
     {
         //Obstacle Avoidance Algorithm
         if(rightRangeSRF04 < wallRange)
         {
             Drive_Reverse(20,70);
             Turn_Left(50, 65);
         }

         else if(leftRangeSRF04 < wallRange)
         {
             Drive_Reverse(20,70); //Was 60 speed 1:16pm
             Turn_Right(50, 65); //Was 70 speed 1:16pm
         }

         else
         {
             //Turn opposite of the wall you are going to be following
             if(direction == -1)
             {
                Drive_Reverse(30,65);
                Turn_Left(50, 70);
             }
             else
             {
                Drive_Reverse(30,65);
                Turn_Right(50, 70);
             }
         }
     }

     else if(rightRangeSRF04 < wallRange && !exit)
     {
        Turn_Left(50, 70);
     }

     else if(leftRangeSRF04 < wallRange && !exit)
     {
        Turn_Right(50, 70);
     }

    lineRoutine();
 }

    //Turn on fan & Move from side to side
    PORTDbits.RD7 = 1;

    for(int x = 0; x < 75; x++) { __delay_ms(50); }

    Drive_Reverse(15,60); Turn_Right(120,70);
    Stop();
    Drive_Reverse(15,60); Turn_Left(240, 75);
    Stop();
    Drive_Reverse(15,60); Turn_Right(120,70);
    Stop();
    Drive_Reverse(15,60); Turn_Left(240, 75);
    Stop();
   
    for(int x = 0; x < 75; x++) { __delay_ms(50); }

    PORTDbits.RD7 = 0;

    Stop();
   
}

/*    LCD_clear();
////TIMER TEST -- HOW TO START TIMER?????
while(1)
{
    place_lcd_cursor(0,7);
    LCD_write("Timer:");
    LCD_convertWrite((int)(mScount/1000));

    Drive_Forward(0,20,60);
    if(lineSensor_isWhiteLine1())
    {
        Stop();
        place_lcd_cursor(0,7);
        LCD_convertWrite((int)(mScount/1000));

        while(1){}
    }
}

}
*/
void interrupt isr(void)
{    
    if(TMR0IF == 1)
    {
        TMR0L = mSstart;
        TMR0IF = 0;
        TMR0IE = 1;
        ++mScount;
        return;
    }
}

void lineRoutine(void)
{
    //Line Sensor True or False
    if(lineSensor_isWhiteLine1())
    {
        Stop();
        while(!lineSensor_isWhiteLine2()) { Turn_Left(20, 70); } // Changed from 10,60
        Stop();
        exit = true;
    }

    else if(lineSensor_isWhiteLine2())
    {
        Stop();
        while(!lineSensor_isWhiteLine1()) { Turn_Right(20, 70); } // Changed from 10,60
        Stop();
        exit = true;
    }
}



