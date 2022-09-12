/*
 * Timer0.c
 *
 *  Created on: Nov 6, 2020
 *      Author: start
 */

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "Timer0.h"

void Timer0_Init(void)
{
	TCCR0 = (1<<WGM00)|(1<<WGM01); /* Select fast PWM mode */

	SET_BIT(TCCR0,COM01); /* select non-inverting mode */

	SET_BIT(TCCR0,CS00); /* No clock pre-scaling */

	DDRB |=(1<<PB3); /* Set PB3 as output Pin */

	TCNT0 = 0; /* Initialize TCNT0 register to zero */
}


void Timer0_sartCounting(uint8 input_voltage)
{
	OCR0 = input_voltage;
}
