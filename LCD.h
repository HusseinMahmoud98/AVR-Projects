/*
 * LCD.h
 *
 *  Created on: Nov 3, 2020
 *      Author: start
 */

#ifndef LCD_H_
#define LCD_H_

#include "std_types.h"


#define RS PD0
#define RW PD1
#define E  PD2

#define LCD_DATA_COMMAND_PORT PORTC
#define LCD_DATA_PORT_DIR     DDRC
#define LCD_CTRL_PORT         PORTD
#define LCD_CTRL_PORT_DIR     DDRD

#define LCD_DATA_MODE 8

#define CLEAR_SCREEN 0x01
#define CURSOR_OFF   0x0C
#define TWO_LINES_EIGHT_BITS_MODE 0x38
#define TWO_LINES_FOUR_BITS_MODE  0x28
#define SET_CURSOR_LOCATION       0x80


void LCD_init(void);
void LCD_sendCommand(uint8 command);
void LCD_displayCharacter(uint8 character);
void LCD_displayString(uint8* String);
void LCD_goToRowColumn(uint8 Row_Num,uint8 Column_Num);
void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str);
void LCD_clearScreen(void);
void LCD_integerToString(int int_value);

#endif /* LCD_H_ */
