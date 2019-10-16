/* Partner : Stephanie scabr006@ucr.edu
 * Lab Section: 021
 * assignment: Lab2 Exercise 2 (Switching Leader/Follwer behavior)
 * Created: 2019-10-08 오후 6:03:49
 * Author : Minwhan Oh
 * 
 * I acknowledge all content contained herein, excluding template
 * or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart_1284.h"
#include "Timer.h"

unsigned char cnt = 0x00;
unsigned char sw = 0x00;

enum Transmit_States {Receiver, send1, send0} transmit_state;
void TickFct_transmit()
{
	switch(transmit_state)
	{
		case Receiver:
		if(sw == 0x00)
		{
			transmit_state = Receiver;
		}
		else if(sw == 0x01)
		{
			transmit_state = send1;
			cnt = 0x00;
		}
		break;
		
		case send1:
		if((cnt != 0x32) && (sw == 0x01)) 
		{
			transmit_state = send1;
		}
		else if((cnt == 0x32) && (sw == 0x01))
		{
			transmit_state = send0;
			cnt = 0x00;
		}
		else if(sw == 0x00)
		{
			transmit_state = Receiver;
			cnt = 0x00;
		}
		break;
		
		case send0:
		if((cnt != 0x32) && (sw == 0x01))
		{
			transmit_state = send0;
		}
		else if((cnt == 0x32) && (sw == 0x01))
		{
			transmit_state = send1;
			cnt = 0x00;
		}
		else if(sw == 0x00)
		{
			transmit_state = Receiver;
			cnt = 0x00;
		}
		break;
		
		default:
		break;
	}
	
	switch(transmit_state)
	{
		case Receiver:
		if(USART_HasReceived(0))
		{
			PORTA = USART_Receive(0);
		}
		PORTC = 0x00;
		break;
		
		case send1:
		PORTC = 0x01;
		if(USART_IsSendReady(1))
		{
			USART_Send(0x01, 1);
		}
		if(USART_HasTransmitted(1))
		{
			PORTA = 0x01;
		}
		cnt++;
		break;
		
		case send0:
		PORTC = 0x01;
		if(USART_IsSendReady(1))
		{
			USART_Send(0x00, 1);
		}
		if(USART_HasTransmitted(1))
		{
			PORTA = 0x00;
		}
		cnt++;
		break;
		
		default:
		break;
	}
}

int main(void)
{
	DDRA = 0xFF; PORTA = 0x00;
	DDRB = 0x00; PORTB = 0xFF;
	DDRC = 0xFF; PORTC = 0x00;
	
	TimerSet(20);
	TimerOn();
	
	initUSART(0);
	initUSART(1);
	
	transmit_state = Receiver;
	
    while (1) 
    {
		sw = ~PINB;
		TickFct_transmit();
		while(!TimerFlag){}
		TimerFlag = 0;
    }
}

