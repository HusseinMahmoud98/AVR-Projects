/*
 * Timer0.c
 *
 *  Created on: Nov 6, 2020
 *      Author: start
 */

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "Timer1.h"

/*
 *
 *
 * */



void Timer1_Init(Timer1_ConfigType *Timer1_Config)
{

	/* Enable Timer1A in Normal Mode */

	TCCR1A = 0;

	TCCR1B= (TCCR1B & 0xF8)|(Timer1_Config-> Prescale);  /* Set Clock pre-scale */

	//DDRB |=(1<<PB3); /* Set PB3 as output Pin */

	TCNT1 = 0; /* Initialize TCNT1 register to zero */

}


void Timer1_sartCounting(uint8 input_voltage)
{
	OCR0 = input_voltage;
}
