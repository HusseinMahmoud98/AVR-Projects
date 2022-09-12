/*
 * ADC.h
 *
 *  Created on: Oct 31, 2020
 *      Author: start
 */

#ifndef ADC_C_
#define ADC_C_

#include "std_types.h"
#include "micro_config.h"
#include "common_macros.h"

#include "ADC.h"

void ADC_init(void)
{
	ADMUX =0;          /* Select ADC0 Channel and set ADC Left Adjust */
						/* Connect to external voltage */

	SET_BIT(ADCSRA,0);
	SET_BIT(ADCSRA,1);
	CLEAR_BIT(ADCSRA,2); /* Set Pre-scale division factor: CLK/8  */

	SET_BIT(ADCSRA,ADEN); /* Enable ADC */
}

uint16 ADC_readChannel(uint8 channel_num)
{

	channel_num &=0x07;

	ADMUX &= 0xE0; /* clear first 5 bits in the ADMUX (channel number MUX4:0 bits) before set the required channel */
	ADMUX = ADMUX | channel_num; /* choose the correct channel by setting the channel number in MUX4:0 bits */

	SET_BIT(ADCSRA,6); /* Start Conversion */

	while(BIT_IS_CLEAR(ADCSRA,ADIF)); /* wait until the data is fully reached */

	SET_BIT(ADCSRA,ADIF); /* clear ADIF by writing logic 1 on it */

	return ADC;
}



#endif /* ADC_C_ */
