/*
 * LCD.c
 *
 *  Created on: Nov 3, 2020
 *      Author: start
 */

#include "stdlib.h"

#include "micro_config.h"
#include "std_types.h"
#include "common_macros.h"

#include "LCD.h"

void LCD_init(void)
{
	LCD_CTRL_PORT_DIR |= (1<<RS)|(1<<RW)|(1<<E);


#if LCD_DATA_MODE == 8
	LCD_DATA_PORT_DIR = 0xFF;

#elif LCD_DATA_MODE == 4
	LCD_DATA_PORT_DIR = 0xF0;
#endif

#if LCD_DATA_MODE == 8
	LCD_sendCommand(TWO_LINES_EIGHT_BITS_MODE);  /* Set LCD, in 8-bits mode, 2 Lines and 5x8 matrix */

#elif LCD_DATA_MODE == 4
	LCD_sendCommand(TWO_LINES_FOUR_BITS_MODE);   /* Set LCD, in 4-bits mode, 2 Lines and 5x8 matrix */
#endif

	LCD_sendCommand(CURSOR_OFF);    /* Display on, cursor-off */
	LCD_sendCommand(CLEAR_SCREEN);  /* Clear screen */
}



void LCD_sendCommand(uint8 command)
{
	CLEAR_BIT(LCD_CTRL_PORT,RS);
	CLEAR_BIT(LCD_CTRL_PORT,RW);    _delay_ms(1);   /* Set LCD to (write) a (command) */

	SET_BIT(LCD_CTRL_PORT,E);        _delay_ms(1);

#if LCD_DATA_MODE == 8
	LCD_DATA_COMMAND_PORT = command; _delay_ms(1);  /* Send the command */

#elif LCD_DATA_MODE == 4
	LCD_DATA_COMMAND_PORT = command;    _delay_ms(1);  /* Send the command Most  Significant 4-bits  */
	//LCD_DATA_COMMAND_PORT = command<<4; _delay_ms(1);  /* Send the command Least Significant 4-bits  */

#endif

	CLEAR_BIT(LCD_CTRL_PORT,E);     _delay_ms(1);
}


void LCD_displayCharacter(uint8 character)
{
	SET_BIT(LCD_CTRL_PORT,RS); 				   /* Data register is selected */
	CLEAR_BIT(PORTD,RW);         _delay_ms(1); /* LCD is in write mode */


	SET_BIT(LCD_CTRL_PORT, E);          _delay_ms(1); /* Enable LCD */

#if LCD_DATA_MODE == 8

	LCD_DATA_COMMAND_PORT = character;  _delay_ms(1); /* Send data */

#elif LCD_DATA_MODE == 4

	LCD_DATA_COMMAND_PORT = character;  _delay_ms(1);    /* Send data Most  Significant 4 bits */
	//LCD_DATA_COMMAND_PORT = character<<4;  _delay_ms(1); /* Send data Least Significant 4 bits */

#endif

	CLEAR_BIT(LCD_CTRL_PORT, E);        _delay_ms(1); /* Disable LCD */
}

void LCD_displayString(uint8* String)
{
	uint8 i = 0;

	while(String[i] != '\0')
	{
		LCD_displayCharacter(String[i]);
		i++;
	}
}


void LCD_goToRowColumn(uint8 row,uint8 col)
{
	uint8 Address;

	/* first of all calculate the required address */
	switch(row)
	{
		case 0:
				Address=col;
				break;
		case 1:
				Address=col+0x40;
				break;
		case 2:
				Address=col+0x10;
				break;
		case 3:
				Address=col+0x50;
				break;
	}
	/* to write to a specific address in the LCD
	 * we need to apply the corresponding command 0b10000000+Address */
	LCD_sendCommand(Address | SET_CURSOR_LOCATION);
}

void LCD_displayStringRowColumn(uint8 row,uint8 col,const char *Str)
{
	LCD_goToRowColumn(row,col); /* go to to the required LCD position */
	LCD_displayString(Str); /* display the string */
}


void LCD_clearScreen(void)
{
	LCD_sendCommand(CLEAR_SCREEN);
}
void LCD_integerToString(int int_value)
{
	uint8 buff[16]; /* String to hold the ascii result */
	itoa(int_value,buff,10); /* 10 for decimal */
	LCD_displayString(buff);
}
