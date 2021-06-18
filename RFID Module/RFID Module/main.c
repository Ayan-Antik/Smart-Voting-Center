/*
 * RFID Module.c
 *
 * Created: 6/18/2021 7:44:12 PM
 * Author : USER
 */ 
#define F_CPU 1000000
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

int total_id = 5;
int id_len = 10;
unsigned char valid_id[5][10] = {
	"1501020304",
	"1501020305",
	"1501020306",
	"1501020307",
	"1501020308",
};



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

void UART_TxChar(unsigned char data){
	while((UCSRA & (1 << UDRE)) == 0x00);
	UDR = data;
	
}
int main(void)
{	
   DDRD = 0b11111110;
   DDRA = 0xFF;
   
   Lcd4_Init();
   UART_init();
   unsigned char id[10];
   int count = 1;
   Lcd4_Clear();
   Lcd4_Write_String("Waiting for  ");
   _delay_ms(500);
   Lcd4_Set_Cursor(2, 1);
   Lcd4_Write_String("RFID Tag");
   _delay_ms(500);
   Lcd4_Clear();
   
   Lcd4_Set_Cursor(1, 0);
   
   int match = 0;
    while (1) 
    {	
		id[count-1] = UART_RxChar();
		Lcd4_Write_Char(id[count-1]);
		if(count < id_len){
			count++;
			continue;
		 }
		 else if(count == id_len){
			 int j;
			 for(int i = 0; i<total_id; i++){
				 for(j = 0; j<id_len; j++){
					 if(valid_id[i][j] != id[j]){
						break;
					 }	 
				 }
				 if(j == id_len){
					match = 1;
					break;
				 }
			 }
		 }
		 if(match == 0){
			 Lcd4_Clear();
			 Lcd4_Write_String("ID match: ");
			 Lcd4_Set_Cursor(2,0);
			 Lcd4_Write_String("Not found. Reset");
		 }
		 else if(match == 1){
			  Lcd4_Clear();
			  Lcd4_Write_String("ID match found!");
			  _delay_ms(2500);
			  Lcd4_Clear();
			  
			  int lock = rand() % (9999 + 1 - 1000) + 1000;
			  unsigned char locks[4];
			  //int div = 99;
			  int mod = 0;
			  while(lock != 0){
				  locks[mod] = (lock % 10) + '0';
				  lock = lock / 10;
				  mod++;
				  
			  }
			  Lcd4_Write_String("Your Lock Code: ");
			  Lcd4_Set_Cursor(2, 0);
			  for(int i = 3; i!=-1; i--){
				  Lcd4_Write_Char(locks[i]);
				  UART_TxChar(locks[i]);
			  }
			  
			  //Lcd4_Clear();
			  //Lcd4_Write_Char(lock);
			 
		 }
	}
}

