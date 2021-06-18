/*
 * LockAndVote.c
 *
 * Created: 6/19/2021 12:05:21 AM
 * Author : USER
 */ 

#define F_CPU 1000000
#define D4 eS_PORTD4
#define D5 eS_PORTD5
#define D6 eS_PORTD6
#define D7 eS_PORTD7
#define RS eS_PORTC6
#define EN eS_PORTC7

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"

void UART_init()
{
	
	UCSRB = 0b00011000;// Turn on the reception and Transmission
	UCSRC = 0b10000110;
	//UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);// Use 8-bit character sizes
	UCSRA = 0b00000000;
	UCSRA |= (1 << RXC) | (1<<TXC);
	// | (0<<TXC)|(0<<UDRE)|(0<<FE)|(0<<DOR)|(0<UPE)|(0<<U2X)|(0<<MPCM)

	//UBRRL = BAUD_PRESCALE;		// Load lower 8-bits of the baud rate
	//UBRRH = (BAUD_PRESCALE >> 8);	// Load upper 8-bits
	UBRRL = 0x19;
	UBRRH = 0x00;
}

unsigned char UART_RxChar()
{
	while ((UCSRA & (1 << RXC)) == 0x00);// Wait till data is received
	return UDR;		// Return the byte
}

int main(void)
{	
	DDRD = 0b11111110;
	DDRC = 0xFF;
	Lcd4_Init();
	UART_init();
	unsigned char lock[4];
	int count = 0;
	Lcd4_Write_String("Lock: ");
	Lcd4_Set_Cursor(2,0);
    /* Replace with your application code */
    while (1) 
    {	
		while(count < 4){
			lock[count] = UART_RxChar();
			Lcd4_Write_Char(lock[count]);
			count++;	
		}
		
		
    }
}

