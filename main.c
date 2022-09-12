/**************************************************************/
/* Author     : Hussein Mahmoud                               */
/* Date       : 22 Oct 2020                                   */
/* Version    : V01                                           */
/* Description: StopWatch (Mini_Project1_AVR) 					  */
/**************************************************************/

#include <avr/io.h>
#include <avr/interrupt.h>

#include <util/delay.h>

unsigned short* _TCNT1_ = &TCNT1L;
unsigned short* _OCR1A_ = &OCR1AL;

unsigned char  G_Segment_Value[6];    /* This global array saves the values of all six 7-Segments at every moment */

void MTimer1_Init(void)
{
	TCCR1B = (1<<CS10)|(1<<CS12); /* Set clock pre-scale CLK/1024 -> 1msec */

	*_TCNT1_ = 0;    /* Set TCNT1 Register initial value to zero  */

	*_OCR1A_ = 1000; /* Set OCR1A to 1 second */

	TIMSK |= (1<<OCIE1A); /* Enable Output Compare Interrupt for Timer1 channel A */


}


void HStopwatch_Init(void)
{
	for(unsigned char Seg_Num = 0; Seg_Num<6; Seg_Num++)
	{
		G_Segment_Value[Seg_Num] = 0;
	}
}


void HStopWatch_Dispaly_7Seg(unsigned char Seg_Num, unsigned char Seg_Value)
{
	PORTA = (1<<Seg_Num); /* Select the 7-Segment required to be displayed */
	PORTC = (Seg_Value);  /* Display the selected 7-Segment value */
	_delay_ms(1);
}

void MINT0_Init(void)
{
	MCUCR |=  (1<<ISC01);
	MCUCR &=~ (1<<ISC00);  /* Generate INT0 on falling-edge */

	DDRD  &=~ (1<<PD2); /* Set PinD2 as Input pin for INT0 */
	PORTD |=  (1<<PD2); /* Enable internal pull-up for PinD2 */

	GICR |= (1<<INT0);     /* Enable INT0 Module */
}

void MINT1_Init(void)
{
	MCUCR |=  (1<<ISC10)|(1<<ISC11); /* Generate INT1 on raising-edge */

	DDRD  &=~ (1<<PD3); /* Set PinD3 as Input pin for INT1 */

	GICR |= (1<<INT1);   		     /* Enable INT1 Module */
}

void MINT2_Init(void)
{
	MCUCSR &=~ (1<<ISC2); /* Generate INT2 on falling-edge */

	DDRB  &=~ (1<<PB2); /* Set PinB2 as Input pin for INT2 */
	PORTB |=  (1<<PB2); /* Enable internal pull-up for PinDB2 */

	GICR |= (1<<INT2);     /* Enable INT2 Module */
}



int main(void)
{

	DDRA  |= (0x3F<<0); /* Set first 6 Pins in PortA as Output Pins to Enable/Disable 7-Segments */

	DDRC  |= (0x0F<<0); /* Set first 4 Pins in PortC as Output Pins for displaying numbers in 7-Segments */

	HStopwatch_Init();

	MINT0_Init();
	MINT1_Init();
	MINT2_Init();

	MTimer1_Init();

	sei(); /* Enable global interrupt */

	unsigned char Seg_Num = 0;

	while(1)
	{
		HStopWatch_Dispaly_7Seg(Seg_Num, G_Segment_Value[Seg_Num]);

		if(Seg_Num!=6)
		{
			Seg_Num++;
		}

		else
		{
			Seg_Num = 0;
		}
	}

	return 0;
}


ISR(TIMER1_COMPA_vect)
{
	*_TCNT1_ = 0;

	if(G_Segment_Value[0] < 9) /* Check Seconds Units */
	{
		G_Segment_Value[0]++;
	}

	else if(G_Segment_Value[1]<5) /* Check Seconds Tens */
	{
		G_Segment_Value[1]++;

		G_Segment_Value[0] = 0;
	}

	else if(G_Segment_Value[2]<9) /* Check Minutes Units */
	{
		G_Segment_Value[2]++;

		G_Segment_Value[0] = 0;
		G_Segment_Value[1] = 0;
	}

	else if(G_Segment_Value[3]<5)  /* Check Minutes Tens */
	{
		G_Segment_Value[3]++;

		G_Segment_Value[0] = 0;
		G_Segment_Value[1] = 0;
		G_Segment_Value[2] = 0;
	}

	else if(G_Segment_Value[4]<9) /* Check Hours Units */
	{
		G_Segment_Value[4]++;

		G_Segment_Value[0] = 0;
		G_Segment_Value[1] = 0;
		G_Segment_Value[2] = 0;
		G_Segment_Value[3] = 0;
	}

	else if(G_Segment_Value[5]<9)  /* Check Hours Tens */
	{
		G_Segment_Value[5]++;

		G_Segment_Value[0] = 0;
		G_Segment_Value[1] = 0;
		G_Segment_Value[2] = 0;
		G_Segment_Value[3] = 0;
		G_Segment_Value[4] = 0;
	}

	else
	{
		TCCR1B = 0; /* No clock source (Timer/Counter stopped) */
	}

}


ISR(INT0_vect) /* Reset StopWatch */
{
	HStopwatch_Init(); /* Set all Segments to zero */

	*_TCNT1_ = 0; /* Set TCNT1 Register initial value to zero  */
}


ISR(INT1_vect) /* Pause the StopWatch */
{
	TCCR1B = 0; /* No clock source (Timer/Counter stopped) */
}

ISR(INT2_vect) /* Resume the StopWatch */
{
	TCCR1B = (1<<CS10)|(1<<CS12); /* Re-enable CLK source with pre-scale = CLK/1024 */
}
