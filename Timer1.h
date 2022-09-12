/*
 * Timer0.h
 *
 *  Created on: Nov 6, 2020
 *      Author: start
 */

#ifndef TIMER1_H_
#define TIMER1_H_

/*************************************************************************
 *                          Types Declaration
 ************************************************************************/

typedef enum
{
NO_CLOCK, F_CPU_CLOCK, F_CPU_CLOCK_8, F_CPU_CLOCK_64, F_CPU_CLOCK_256, F_CPU_CLOCK_1024
}Timer1_Clock_Prescale;

typedef enum
{
Normal_Operation, Toggle_On_Compare_Match, Clear_On_Compare_Match, Set_On_Compare_Match
}Timer1_Compare_Mod;

typedef enum
{
	Normal_WGM_Mode
}Timer1_WGM_Config;


typedef enum
{
	No_Interrupt, Overflow_Interrupt_enable=4, OutputCompareB_Interrupt_Enable=8,
	OutputCompareA_Interrupt_Enable=16,	OutputCompareA_B_Interrupt_Enable=24
}Timer1_Interrupt_Config;

typedef struct
{
	Timer1_Clock_Prescale   Prescale;
	Timer1_Compare_Mod      Compare_Mode;
	Timer1_WGM_Config       WGM_Config;
	Timer1_Interrupt_Config Timer1_Interrupt;

}Timer1_ConfigType;




/*************************************************************************
 *                          Functions Prototypes
 ************************************************************************/

void Timer1_Init(Timer1_ConfigType *Timer1_Config);


#endif /* TIMER1_H_ */
