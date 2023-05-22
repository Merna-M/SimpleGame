#include "STD_types.h"
#include "LCD_interface.h"
#include "KYP_interface.h"
#include "TMR0_interface.h"
#define F_CPU 8000000UL
#include "util/delay.h"

uint8 game_arr[6][10]={"3+?=8","6+5=?","6x?=42","9-4=?","9/3=?","9x8=?"};
uint8 answers[6]={5,11,7,5,3,72};
uint8 cursor[6]={3,5,3,5,5,5};
volatile uint16 TMR_counter =0;


void ISR_TMR0(void);

int main(void)
{
	LCD_vInit();
	KYP_vInit();
	TMR0_vInit();
	TMR0_COMP_SetCallBack(ISR_TMR0);
	
	uint8 press,i,user_answer;
	for(i=0 ; i<6 ; i++)
	{
		LCD_vClearScreen();
		LCD_vSendString(game_arr+i);
		TMR_counter=0;
		do{
			press = KYP_u8CheckPress();
		}while(press == NOT_PRESSED && TMR_counter<1000);
		_delay_ms(250);
		LCD_vMoveCursor(1,cursor[i]);
		if(answers[i]>9)
		{
			user_answer = (press-48)*10;
			do{
				press = KYP_u8CheckPress();
			}while(press == NOT_PRESSED && TMR_counter<1000);
			_delay_ms(250);
			user_answer = user_answer+(press-48);
		}
		else
		{
			user_answer = press-48;
		}
		LCD_vSendNumber(user_answer);
		if(TMR_counter >= 1000 || user_answer != answers[i])
		{
			if(TMR_counter >= 1000)
			{
				LCD_vClearScreen();
				LCD_vMoveCursor(2,4);
				LCD_vSendString("Time Over");
				_delay_ms(1000);
			}
			else if(user_answer != answers[i])
			{
				LCD_vClearScreen();
				LCD_vMoveCursor(2,3);
				LCD_vSendString("Wrong Answer");
				_delay_ms(1000);
			}
			break;
		}
		else
		{
			LCD_vMoveCursor(2,4);
			LCD_vSendString("Well Done");
			_delay_ms(1000);
		}
		
	}
	if(TMR_counter<1000 && i==6)
	{
		LCD_vClearScreen();
		LCD_vMoveCursor(2,4);
		LCD_vSendString("Congrats");
	}
		
    while(1)
    {	 
		
    }
}

void ISR_TMR0(void)
{
	TMR_counter++;
}