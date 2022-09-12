/*
 * MC1.c
 *
 * Created: 12/12/2020 11:21:37 PM
 * Author: Hussein Mahmoud
 */


#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "uart.h"
#include "Timer1.h"

#include "lcd.h"
#include "external_eeprom.h"
#include "Motor.h"


#define READY 0x10

uint8 g_Time = 1;
uint8 g_counter = 1;
uint8 g_Timer1A_Flag = 1;
uint8 g_Timer1B_Flag = 1;
uint8 g_Status;


uint8 G_Password[5];
uint8 G_LCDPass[5];
uint8 eeprom[5];

Timer1_Config_Type Timer1_Config={F_CPU_CLOCK_1024, OC1A_DISCONNECTED, OC1B_DISCONNECTED, NORMAL};

void Save_Password(void)
{
	/* save password in eeprom */
	for(uint8 i = 0; i <1; i++)
	{
		EEPROM_writeByte(0x0310 + i, G_Password[i]);
		_delay_ms(10);
	}
}


void Receive_Password(void)
{
	/* Receive password using uart and save it in eeprom */
	for(uint8 i = 0; i <1; i++)
	{
		/* Receive a new password and save it and External EEPROM */
		G_Password[i] = UART_recieveByte();

	}
}

void Password_Check(void)
{
	/* a flag to check the correctness the entered password  */
	uint8 flag = 'Y';

	LCD_goToRowColumn(1, 0);
	for(uint8 i = 0; i <1; i++)
	{
		G_Password[i]=UART_recieveByte();
		//LCD_intgerToString(G_Password[i]);

		EEPROM_readByte(0x0310+i, &eeprom[i]);
		_delay_ms(10);

		if(G_Password[i]!=eeprom[i])
		{
			flag = 'N';
			break;
		}
	}

	if(flag=='N') /* Repeat from step1  */
	{
		//flag = 'Y';
		UART_sendByte('N');
		g_Time=1;
		//	LCD_clearScreen();
	}

	else
	{
		UART_sendByte('Y');
		//Timer1_Config_Type Timer1_Config={F_CPU_CLOCK_1024, OC1A_DISCONNECTED, OC1B_DISCONNECTED, NORMAL};
		Timer1_Init(&Timer1_Config, 5859, 0 ); //58594
		SET_BIT(SREG,7);
		Motor_rotateClockwise();
		if( Motor_getMotorState() == Motor_Rotating_Clockwise)
		{
			TOGGLE_BIT(PORTC,7);
		}

	}
}



int main(void)
{


	/* USART, EEPROM, LCD and Motor initialization */
	UART_init();
	EEPROM_init();
	LCD_init();
	Motor_init();


	SREG|=(1<<7); /* Enable Global Interrupt */

	DDRC |=(1<<7)|(1<<6)|(1<<5) ;

	//PORTC |=(1<<7);

	//if(((PORTC>>7)&1)!=1 )

	//{
	//PORTC|=(1<<6);
	//}


	//SET_BIT(PORTC,5);


	//uint8 First_Time_Flag;

	//EEPROM_readByte(0316, &First_Time_Flag);
	//UART_sendByte(First_Time_Flag);

	//	if(First_Time_Flag == 1) /* This condition will be true first-time use only */
	//{
	//EEPROM_writeByte(0x0316, 0); /* Write 0 on EEPROM at certain address to indicate that the password is set before */
	//}



	while(1)
	{
		if(g_Time == 1)
		{
			g_Time++;
			Receive_Password();
			Save_Password();
		}

		else if(g_Time==2)
		{
			g_Time++;
			//Receive_Password();
			Password_Check();
		}

		else if(g_Time==3)
		{
			g_Status =  UART_recieveByte();

			switch (g_Status) {
			case '*':
				Password_Check();
				break;
			case '#':
				Password_Check();
				Receive_Password();
				break;
			default:
				break;
			}
		}

	}
}

ISR(TIMER1_COMPA_vect)
{
	Timer1_Reset(); /* Reset the timer to start a new counting */


	switch (Motor_getMotorState())
	{
	case Motor_Rotating_Clockwise:
		Timer1_Init(&Timer1_Config, 2343, 0 ); //23438
		Motor_stop();
		break;

	case Motor_Stop:
		Motor_rotateAntiClockwise();
		Timer1_Init(&Timer1_Config, 5859, 0 ); //5859
		break;

	default:
		Motor_stop();
		g_Time=3;
		PORTC=0xFF;
		Timer1_Deinitialization();
		break;
	}

}
