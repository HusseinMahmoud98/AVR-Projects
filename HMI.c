/*
 * MC2.c
 *
 * Created: 4/10/2020 11:23:48 PM
 * Author: Hussein Mahmoud
 */

#include "common_macros.h"
#include "micro_config.h"
#include "std_types.h"

#include "uart.h"
#include "Timer1.h"

#include "lcd.h"
#include "keypad.h"


#define READY 0x10

uint8 g_Time = 1;
uint8 g_Warning = 0;
uint8 g_Flag_FirstTimeSeting = 1;
uint8 g_Timer_Flag = 0;
uint8 g_key_value;
uint8 g_FirstTimeuse = 'T';

uint8 G_Password[5];

uint8 g_status;


//void FirstTimeUse(void)
//{
//UART_sendByte(READY); // MC2 is ready
//UART_receiveString(G_Str); // receive the string
//LCD_displayString("Enter Password:"); // display the string on LCD
//LCD_goToRowColumn(1, 0);

//g_Time++;
//}

//void Check_Password(void)
//{
//LCD_displayString("Reenter Password");
//LCD_goToRowColumn(1, 0);
//}

void APP_sendPassword(void)
{
	for(uint8 count = 0;count<1;count++)
	{
		UART_sendByte(G_Password[count]);
		//LCD_intgerToString(G_Password[count]);

		_delay_ms(10); /* THIS DELAY IS IMPORTANT */
	}


	/*
	for(uint8 i = 0;i<16;i++)
	{
		G_Str[i] = UART_recieveByte();
		LCD_displayCharacter(G_Str[i]);
	}
	 */

}


void APP_enterPassword(void)
{
	uint8 i=0;

	uint8 key_value;

	//if(((key_value>=0) || (key_value<=9)) && i!=1)

	while(i<1)

	{
		key_value=KeyPad_getPressedKey();

		G_Password[i]=key_value;
		i++;
		LCD_displayCharacter('*');
		_delay_ms(500);
	}

	//else if (i!=1)
	//{
	//i++;
	//LCD_displayCharacter('*');
	//G_Password[i]=key_value;
	//}
	LCD_clearScreen();
	APP_sendPassword();
	g_Time=3;
}



int main(void)
{


	uint8 i=0;
	uint8 flag_repeat=0;
	uint8 status;

	Timer1_Config_Type Timer1_Config={F_CPU_CLOCK_1024, OC1A_DISCONNECTED, OC1B_DISCONNECTED, NORMAL};



	USART_Config_Type Ptr_USART_Config={DOUBLE_SPEED, MULTI_PROCESSOR_DISABLED,
			RX_INTERRUPT_DISABLED, TX_INTERRUPT_DISABLED, DATA_EMPTY_REG_INTERRUPT_DISABLED,
			RECIEVER_ENABLED, TRANSMITTER_ENABLED, ASYNCHRONOUS_MODE, PARITY_DISABLED,
			ONE_STOP_BIT, ASYNCHRONOUS, EIGHT_BITS };



	SREG|=(1<<7);

	UART_init(&Ptr_USART_Config); // initialize UART driver

	LCD_init(); // initialize LCD driver


	//uint8 Flag_First
	//UART_recieveByte()

	//if(g_Time==1)
	//{
	//FirstTimeUse();
	//_delay_ms(3000);
	LCD_displayString("Enter Password:");
	LCD_goToRowColumn(1, 0);
	g_Time = 2;
	//}


	while(1)
	{
		if(g_Time==2)
		{
			APP_enterPassword();
		}

		if(g_Time==3 && flag_repeat==0 && g_FirstTimeuse == 'T')
		{
			flag_repeat = 1;
			LCD_displayString("Reenter Password:");
			LCD_goToRowColumn(1, 0);
			APP_enterPassword();
		}

		else if( g_Time==3 )//&& flag_repeat ==1)
		{
			status=UART_recieveByte();
			LCD_displayCharacter(status);
			_delay_ms(500);

			if(status == 'N')
			{
				flag_repeat=0; /* Repeat setting password operation */
				LCD_displayString("Enter Password:");
				LCD_goToRowColumn(1, 0);
				i=0;
				g_Time=2;
			}

			else if(status == 'Y')
			{
				g_FirstTimeuse = 'F';
				LCD_clearScreen();
				LCD_displayString("Door is opening");
				g_Time = 10;
				Timer1_Init(&Timer1_Config, 6445, 0 ); //64453
				SET_BIT(SREG,7);
			}
		}
	}
}


ISR(TIMER1_COMPA_vect)
{

	g_status = 'N';

	if(g_Timer_Flag < 1) /* Check if the timer counts 8.25, 16.5, 24.75 or 33 seconds */
	{
		Timer1_Reset();   /* Make the TCNT1 Register 0 */
		g_Timer_Flag++; /* indicate that the timer counts 1st 7.5 seconds */
	}

	else
	{
		LCD_clearScreen();
		LCD_displayString("*:Enter Password");
		LCD_goToRowColumn(1, 0);
		LCD_displayString("#:Change Password");



		g_key_value = KeyPad_getPressedKey();


		LCD_clearScreen();

		UART_sendByte(g_key_value);

		//while(g_status == 'N'){
		switch (g_key_value) {
		case '*':
			LCD_displayString("Enter password:");
			LCD_goToRowColumn(1, 0);
			g_Time = 2;
			break;

		case '#':
			LCD_displayString("Enter old password:");
			LCD_goToRowColumn(1, 0);
			g_Time =2;
			break;

		default: g_status = UART_recieveByte();
		LCD_displayCharacter(g_status);
		}


		Timer1_Deinitialization();
		g_Timer_Flag = 0;
	}

}
