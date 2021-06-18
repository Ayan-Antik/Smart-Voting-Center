/*
 * RFID Module.c
 *
 * Created: 6/18/2021 7:44:12 PM
 * Author : USER
 */ 
#define F_CPU 16000000UL
#define D4 eS_PORTA4
#define D5 eS_PORTA5
#define D6 eS_PORTA6
#define D7 eS_PORTA7
#define RS eS_PORTA0
#define EN eS_PORTA1

#include <avr/io.h>
#include <util/delay.h>
#include <stdlib.h>
#include <stdio.h>
#include "lcd.h"

void UART_init()
{
	UCSRB = 0b00010000;// Turn on the reception
	UCSRC = 0b10000110;
	//UCSRC |= (1 << URSEL) | (1 << UCSZ0) | (1 << UCSZ1);// Use 8-bit character sizes
	UCSRA = 0b00000000;
	UCSRA |= (1 << RXC);
	// | (0<<TXC)|(0<<UDRE)|(0<<FE)|(0<<DOR)|(0<UPE)|(0<<U2X)|(0<<MPCM)

	//UBRRL = BAUD_PRESCALE;		// Load lower 8-bits of the baud rate
	//UBRRH = (BAUD_PRESCALE >> 8);	// Load upper 8-bits
	UBRRL = 0x33;
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
   DDRA = 0xFF;
   Lcd4_Init();
   UART_init();
   unsigned char c;
   Lcd4_Write_String("Waiting for  ");
   Lcd4_Set_Cursor(2, 1);
   Lcd4_Write_String("RFID Tag");
   Lcd4_Clear();
   Lcd4_Set_Cursor(1, 0);
    while (1) 
    {
		c = UART_RxChar();
		Lcd4_Write_Char(c);
    }
}

