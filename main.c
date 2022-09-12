/*
 * main.c
 *
 *  Created on: Nov 6, 2020
 *      Author: start
 */

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "ADC.h"
#include "Timer0.h"
#include "INT1.h"

#include "LCD.h"



int main(void)
{

	DDRB |= 0x03; /* Set the first two Pins in PortB as output pins for Motor direction */

	SET_BIT(PORTB,1);
	CLEAR_BIT(PORTB,0);

	uint16 voltage_value;

	INT1_init();
	ADC_init();
	LCD_init();
	Timer0_Init();

	LCD_displayString("ADC = ");


	while(1)
	{
	voltage_value = ADC_readChannel(0); /* read channel zero where the potentiometer is connect */
	Timer0_sartCounting(voltage_value/4);

	LCD_goToRowColumn(0,12);
	LCD_integerToString(ADC_readChannel(0)); /*  */

	}

	return 0;
}



