/* Partner : Stephanie 
 * Lab Section: 021
 * assignment: Lab2 Exercise 1 (Follower)
 * Created: 2019-10-08 오후 6:03:49
 * Author : Minwhan Oh
 * 
 * I acknowledge all content contained herein, excluding template
 * or example code, is my own original work.
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "usart_1284.h"

int main(void)
{
    DDRA = 0xFF; PORTA = 0x00;
	initUSART(0);
	
	unsigned char received;
	
	while (1) 
    {
		if(USART_HasReceived(0)){
			received = USART_Receive(0);		
		}
		PORTA = received;
    }
}