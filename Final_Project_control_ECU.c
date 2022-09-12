/*
 * main.c
 *
 *  Created on: Dec 9, 2020
 *      Author: start
 */

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "external_eeprom.h"
#include "i2c.h"
#include "uart.h"


#define HMI_READY 0x10

uint8 g_check_First_use = 0;



int main(void)
{
	UART_init();   /* Initialize UART */
	TWI_init();    /* Initialize I2C */
	EEPROM_init(); /* Initialize EEPROM */

	EEPROM_writeByte(0x00, 0);

	uint8 Pin_Num; /* A variable to hold key_num */


	while(UART_recieveByte() != HMI_READY);
	UART_sendString("Please##");

while(1){


	if(EEPROM_readByte(0x00, &g_check_First_use))
	{
		EEPROM_writeByte(0X00, 1);

		g_check_First_use++;
	}

	//	EEPROM_writeByte(0x01, );

}

return 0;
}
