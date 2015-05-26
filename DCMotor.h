/* 
 * File:   DCMotor.h
 * Author: froe0466
 *
 * Created on April 14, 2015, 9:58 AM
 */

#ifndef DCMOTOR_H
#define	DCMOTOR_H

#ifdef	__cplusplus
extern "C" {
#endif
void EPWM_Port_Init(void);

void EPWM1_Initialize (void);
void EPWM1_LoadDutyValue(int dutyValue);
void TMR2_Initialize(void);
void Switch_Direction1(void);

void EPWM2_Initialize (void);
void EPWM2_LoadDutyValue(int dutyValue);
void TMR4_Initialize(void);
void Switch_Direction2(void);

void Stop(void);
//Takes a direction of either 1 or -1
void Drive_Forward(int direction, int distance, int speed);
void Drive_Reverse(int distance, int speed);
void Turn_Left(int duration, int speed);
void Turn_Right(int duration, int speed);
void Pivot(int rotations, int speed);
void coast();

#ifdef	__cplusplus
}
#endif

#endif	/* DCMOTOR_H */

