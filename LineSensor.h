/*
 * File:   LineQRE1113.h
 * Author: froe0466
 *
 * Created on April 20, 2015, 10:19 AM
 */
#include <stdbool.h>
#include <xc.h>


#ifndef LINESENSOR_H
#define	LINESENSOR_H

#ifdef	__cplusplus
extern "C" {
#endif

void lineSensor_init(void);
int lineSensor_read(unsigned int Channel);
bool lineSensor_isWhiteLine1(void);
bool lineSensor_isWhiteLine2(void);

#ifdef	__cplusplus
}
#endif

#endif	/* LINEQRE1113_H */
