/*
 * INT1.c
 *
 *  Created on: Nov 6, 2020
 *      Author: start
 */

#include "INT0.h"

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"


void INT0_init(void)
{
	MCUCR=1;;  /* Set INT0 to generate an interrupt at any logic edge */

	SET_BIT(GICR,6);      /* Enable INT0 interrupt request */

	CLEAR_BIT(DDRD,PD2);  /* Set PinD2 as an input Pin */

	SET_BIT(SREG,7);      /* Enable global interrupt */
}

