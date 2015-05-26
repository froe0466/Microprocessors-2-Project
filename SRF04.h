/* 
 * File:   SRF04.h
 * Author: froe0466
 *
 * Created on May 8, 2015, 9:54 AM
 */

#ifndef SRF04_H
#define	SRF04_H

#ifdef	__cplusplus
extern "C" {
#endif

    void SRF04_Setup(void);
    int Send_Pulse_Left(void);
    int Send_Pulse_Right(void);


#ifdef	__cplusplus
}
#endif

#endif	/* SRF04_H */

