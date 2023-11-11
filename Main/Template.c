/*
* Template.c
*
* Modified: 10/3/2022
* Author: hjeong
* This file includes complete codes for
* 1. Serial communication by polling
* 2. Serial communication by Interrupt
* 3. ADC by polling
* 4. ADC by Interrupt
* 5. Stepper motor
*/ 

#include <avr/io.h>
#define F_CPU 16000000UL
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>

#include "_main.h"
#include "_buzzer.h"
#include "_adc.h"
#include "_eeprom.h"
#include "_init.h"
#include "_interrupt.h"
#include "_port.h"
#include "_timer2.h"
#include "_uart.h"
#include "_glcd.h"

unsigned int Uart1_DataView = 0;
unsigned int INT0_DataView = 0;
unsigned int INT1_DataView = 0;
unsigned int Data_ADC0 = 0;
unsigned int Data_ADC2 = 0;
unsigned int Data_ADC3 = 0;
unsigned int Data_ADC4 = 0;

char Dis_Scr_IO_ON[]={"O"};
char Dis_Scr_IO_OFF[]={"X"};
char Dis_Scr1[]={"#$%&'()*+,-./0123456"};
char Dis_Scr2[]={"789:;<=>?@ABCDEFGHIJ"};
char Dis_Scr3[]={"KLMNOPQRSTUVWXYZ[]{}"};

/* Port Programming: Blinking */
/*
int main_template(void){
	DDRB = 0xFF;
	PORTB = 0xAA;
	while(1){
		PORTB = ~PORTB;
		_delay_ms(1000);
		
	}
	return 0;
}
*/

/* Port Programming: Rotation */
/*
int main_template(void){
	DDRB = 0xFF;
	DDRD = 0x00;
	PORTB = 0x7F;
	while (1){
		if (PIND & 0x01){
			PORTB = (PORTB << 1) | (PORTB >> 7);
		}
		else {
			PORTB = (PORTB >> 1) | (PORTB << 7);	
		}
		_delay_ms(1000);
		
	}
	return 0;
}
*/

/* Accessing Flash Memory */
/*
#include <avr/pgmspace.h>

const unsigned char PROGMEM lookup[] = {0,1,2,3,4,5,6,7,8,9,10,11,12,13,14,15};

int main(void)
{
	unsigned char a;
	DDRB = 0xFF;
	init_devices();
	while (1){
		for (int i=0; i < 16; i++){
			PORTB = ~pgm_read_byte(&lookup[i]);
			_delay_ms(1000);
		}
	}
	return 0;
}
*/

/* Accessing EEPROM */
/**/
#include <avr/io.h>
#include <avr/eeprom.h>

unsigned char EEMEM myVar; //reserve a location in EEPROM

int main_template(void)
{
	DDRD = 0x00;
	DDRB = 0xFF;
	while (1){
		if((PIND&(1<<0)) == 0)  //if PD0 is LOW
			eeprom_write_byte(&myVar,'7'); //write to EEPROM
		else {
			PORTB = eeprom_read_byte(&myVar); //read from EEPROM
			_delay_ms(100);
			lcd_clear();							// clear LCD
			ScreenBuffer_clear();					// clear screen buffer
			lcd_string(0,0,"EEPROM Data");	// output ADC0 Potentiometer
			lcd_xy(1,0); 
			GLCD_4DigitDecimal('1');	// output ADC0
		}
	}

	return 0;
}
/**/

/* ADC  */
/*
int main_template (void)
{
	init_devices();
	while (1){
		Data_ADC0 = Read_Adc_Data(0)/10;		// Read analog port 0
		_delay_ms(100);						
		lcd_clear();							// clear LCD
		ScreenBuffer_clear();					// clear screen buffer
		lcd_string(0,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer 
		GLCD_Rectangle(20,0,30,Data_ADC0);		// output line gage
		lcd_xy(1,0); GLCD_4DigitDecimal(Data_ADC0);	// output ADC0
	}
}
*/






/* UART and ADC Registers ('-' means default 0)
UART Registers:
	UCSRnA: RXCn TXCn UDREn - - - - -
	UCSRnB: RXCIEn TXCIEn UDRIEn RXENn TXENn - - -
	UCSRnC: - - - - - UCSZn1 UCSZn0 -
	
ADC Registers:
	ADMUX: REFS1 REFS0 - - MUX3 MUX2 MUX1 MUX0
	ADCSA: ADEN ADSC - ADIF ADIE ADPS2 ADPS1 ADPS0
*/
	

/* Serial communications: Loop-back by polling */
/*
#define F_CPU 16000000UL
#define BAUD 9600
void main_template(void)										
{
	unsigned char data;
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;			
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);		// receiver and transmitter
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);	// 8-bit
				
	while (1){
		while (!(UCSR1A & (1<<RXC1)));	// receiver polling
		data = UDR1;
		while(!(UCSR1A & (1<<UDRE1)));	// transmitter polling
		UDR1 = data;
	}	
}
*/
	
////////////////////////////////////////////////////
/* Serial communication: Loop-back by Interrupt	*/
/*
#define F_CPU 16000000UL
#define BAUD 9600
int main_template (void)
{
	cli();
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1); // receiver interrupt
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);			// character size 8
				
	sei();
	while(1);
}
*/
		
/*
SIGNAL(USART1_RX_vect)					// Serial ISR
{
	cli();								//disable all interrupts
	UDR1 = UDR1;			
	sei();								//re-enable interrupts
}
*/


/////////////////////////////////////////////////////////////
/* ADC: ADC to COM by Polling */  
/*
#define F_CPU 16000000UL
#define BAUD 9600
void usart_send (unsigned char ch);
int main_template (void)
{							
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<TXEN1);				//Transmission enable
	UCSR1C = (1<< UCSZ11)|(1<<UCSZ10);	// 8-bit
		
	// ADC Initialization
	ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
	ADCSRA = (1<<ADEN)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x87

	while (1)
	{
		ADCSRA|=(1<<ADSC);				// start conversion
		while((ADCSRA&(1<<ADIF))==0);	// polling conversion to finish		
		ADCSRA |= (1<<ADIF);			// reset interrupt

		int a = ADCL;
		a = a | (ADCH<<8);
			
		if(a < 0)
		{
			usart_send('-');
			a *= -1;
		}
			
		usart_send((a/1000)+'0');
		a = a % 1000;
		usart_send((a/100)+'0');
		a = a % 100;
		usart_send((a/10)+'0');
		a = a % 10;
		usart_send((a)+'0');
		usart_send('\r');
			
		_delay_ms(100);
	}
	return 0;
}

void usart_send (unsigned char ch)
{
	while (!(UCSR1A & (1<<UDRE1))); 	// transmission complete?
	UDR1 = ch;							// transmit ch
}
*/
	
/* ADC: ADC to COM by Interrupt */  
/*
#define F_CPU 16000000UL
#define BAUD 9600

void main_template (void)
{				
	cli();			
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<TXEN1);				// Transmission enable
	UCSR1C = (1<< UCSZ11)|(1<<UCSZ10);	// 8-bit

	// ADC Initialization
	ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x8F
		
	sei();
	ADCSRA |=(1<<ADSC);				// start conversion
		
	while (1);
}

void usart_send (unsigned char ch)
{
	while (! (UCSR1A & (1<<UDRE1))); 	// polling transmission complete?
	UDR1 = ch;							// transmit ch
}
	
SIGNAL(ADC_vect)					// ADC interrupt
{
	cli();
	ADCSRA |= (1<<ADIF);			// reset interrupt
		
	int a = ADCL;					// ADC register
	a = a | (ADCH<<8);
		
	if(a < 0)
	{
		usart_send('-');
		a *= -1;
	}
		
	usart_send((a/1000)+'0');
	a = a % 1000;
	usart_send((a/100)+'0');
	a = a % 100;
	usart_send((a/10)+'0');
	a = a % 10;
	usart_send((a)+'0');
	usart_send('\r');

	sei();
	ADCSRA |=(1<<ADSC);				// start conversion
}
*/

/* Stepper Motor */
/*
#define F_CPU 16000000UL
#include "avr/io.h"
#include "util/delay.h"
int main_template ()
{
	DDRD = 0x00;
	DDRB = 0xFF;
	while (1)
	{
		if( (PIND&0x80) == 0)
		{
			PORTB = 0x66;
			_delay_ms (100);
			PORTB = 0xCC;
			_delay_ms (100);
			PORTB = 0x99;
			_delay_ms (100);
			PORTB = 0x33;
			_delay_ms (100);
		}
		else
		{
			PORTB = 0x66;
			_delay_ms (100);
			PORTB = 0x33;
			_delay_ms (100);
			PORTB = 0x99;
			_delay_ms (100);
			PORTB = 0xCC;
			_delay_ms (100);
		}
	}
}
*/