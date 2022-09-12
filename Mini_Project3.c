/*
 * main.c
 *
 *  Created on: Dec 6, 2020
 *      Author: start
 */

#include "INT0.h"
#include "lcd.h"
#include "micro_config.h"
#include "Timer1.h"
#include "std_types.h"

uint8  g_EdgeCount=0;
uint16 g_HighTime=0;
uint16 g_PeriodTime=0;

int main(void)
{
	Timer1_ConfigType Timer1_Config ={F_CPU_CLOCK, Normal_Operation, Normal_WGM_Mode, No_Interrupt};

	Timer1_Init(&Timer1_Config);

	INT0_init();
	LCD_init();

	uint32 Duty_Cycle;
	while(g_EdgeCount!=3);

	LCD_displayString("D.Cycle=");
	Duty_Cycle = (float)g_HighTime/g_PeriodTime*100+1;
	LCD_intgerToString(Duty_Cycle);
	LCD_displayString("%");


	while(1){}


	return 0;
}

ISR(INT0_vect)
{
	if(g_EdgeCount==0 && ((PIND>>2)&1) ) /* Check that first rising edge came */
	{
		TCNT1=0;  /* Start Counting from zero in Timer1 */
		g_EdgeCount++;
	}

	else if(g_EdgeCount==1) /* Falling edge */
	{

		g_HighTime = TCNT1;
		g_EdgeCount++;
	}

	else if(g_EdgeCount==2) /* Rising edge */
	{
		g_PeriodTime = TCNT1;
		g_EdgeCount++;
	CLEAR_BIT(SREG,7);

	}
}


