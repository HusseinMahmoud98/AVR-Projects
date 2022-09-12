/*
 * INT1.c
 *
 *  Created on: Nov 6, 2020
 *      Author: start
 */

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "INT1.h"

void INT1_init(void)
{
	SET_BIT(MCUCR,ISC10);
	SET_BIT(MCUCR,ISC11); /* Set INT1 to generate an interrupt on the rising edge */

	SET_BIT(GICR,7);   /* Enable INT1 interrupt request */

	CLEAR_BIT(DDRD,PD3); /* Set PinD3 as an input Pin */

	SET_BIT(SREG,7);  /* Enable global interrupt */
}

ISR(INT1_vect)
{
	TOGGLE_BIT(PORTB,0);
	TOGGLE_BIT(PORTB,1);

	CLEAR_BIT(PORTB,7);
}
