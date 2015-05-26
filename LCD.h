/* 
 * File:   LCD.h
 * Author: froe0466
 *
 * Created on April 14, 2015, 8:06 AM
 */

#ifndef LCD_H
#define	LCD_H

#ifdef	__cplusplus
extern "C" {
#endif

/*******************************
 * FUNCTION PROTOTYPES
 *******************************/
// LCD
void Delay100ms(void);
void LCD_init(void);
void LCD_write(unsigned char data[]);
void LCD_writeChar(unsigned char data);
void place_lcd_cursor(unsigned char x, unsigned char y);
void LCD_convertWrite(unsigned char data);
void LCD_clear(void);


#ifdef	__cplusplus
}
#endif

#endif	/* LCD_H */

