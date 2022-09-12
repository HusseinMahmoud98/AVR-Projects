/*
 * main.c
 *
 *  Created on: Dec 9, 2020
 *      Author: start
 */

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "lcd.h"
#include "keypad.h"

#define HMI_READY 0x10

int main (void)
{

uint8 key_value;

LCD_init();

uint8 Str[20];
LCD_displayString("dd");


uint8 Password[5];
uint8 i=0;



UART_sendByte(HMI_READY);
UART_receiveString(Str);
LCD_displayString(Str);


while (1)
{





/*
	key_value=KeyPad_getPressedKey();

	if((key_value>=0) || (key_value<=9))
	{
	Password[i]=key_value;
	i++;
	LCD_intgerToString(key_value);

	}

	else
	{
	LCD_displayCharacter(key_value);
	}

	_delay_ms(300);
*/
}

return 0;
}
