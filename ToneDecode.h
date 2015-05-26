/* 
 * File:   ToneDecode.h
 * Author: elmt0001
 *
 * Created on May 12, 2015, 11:06 AM
 */

#ifndef TONEDECODE_H
#define	TONEDECODE_H

#ifdef	__cplusplus
extern "C" {
#endif

void MicInit(void);
int Mic_read25(void);
int Mic_read38(void);

#ifdef	__cplusplus
}
#endif

#endif	/* TONEDECODE_H */

