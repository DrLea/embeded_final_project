/* ADC  */
#define F_CPU 16000000UL
#define BAUD 9600
#include <avr/io.h>				//<avr/portpins.h> <avr/iom128.h>
#include <avr/interrupt.h>
#include <util/delay.h>
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


/* 1. ADC Polling  */
/*ADC Registers:
 ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
 ADMUX: REFS1 REFS0 ADLAR _ MUX3 MUX2 MUX1 MUX0
*/
/*
unsigned int adc = 0;
void main_adc (void)
{
	init_devices();
	ADCSRA = 0x00;	// disable adc
	ADMUX  = (1<<REFS0); 	// select adc input 0 and Aref
	ADCSRA = (1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);	// Conversion time:  Fosc/8
	while (1){
		adc = 0;	
		
		ADCSRA |= (1<<ADSC);                            // Wait for the AD conversion to complete
		while(!(ADCSRA & (1<<ADIF)));
		ADCSRA |= (1<<ADIF);					//manual reset in polling (auto reset in interrupt)
		
		adc = (ADCL + (ADCH * 256))/10;			
		_delay_ms(100);
		lcd_clear();							// clear LCD
		ScreenBuffer_clear();					// clear screen buffer
		lcd_string(0,0, " 12345 Hong Jeong");
		lcd_string(2,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer
		GLCD_Rectangle(50,0,60,adc);		// output line gage
		lcd_xy(4,0); GLCD_4DigitDecimal(adc);	// output ADC0
	}
}
*/


/* 2. ADC Interrupt */
/*ADC Registers:
 ADCSRA: ADEN ADSC ADATE ADIF ADIE ADPS2 ADPS1 ADPS0
 ADMUX: REFS1 REFS0 ADLAR _ MUX3 MUX2 MUX1 MUX0
*/
/*
unsigned int adc = 0;
void main_adc (void)
{
	init_devices();
	ADCSRA = 0x00;	// disable adc
	ADMUX  = (1<<REFS0); 	// select adc input 0 and Aref
	ADCSRA = (1<<ADSC)|(1<<ADIE)|(1<<ADEN)|(1<<ADPS1)|(1<<ADPS0);	// Conversion time:  Fosc/8
	adc = 0;
	sei();
	while (1);		
}

SIGNAL (ADC_vect){
	adc = (ADCL + (ADCH * 256))/10;
	_delay_ms(100);
	lcd_clear();							// clear LCD
	ScreenBuffer_clear();					// clear screen buffer
	lcd_string(0,0, " 12345 Hong Jeong");
	lcd_string(2,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer
	GLCD_Rectangle(50,0,60,adc);		// output line gage
	lcd_xy(4,0); GLCD_4DigitDecimal(adc);	// output ADC0
	ADCSRA |= (1<<ADSC);					//ADSC reset when conversion completed
}
*/


/* 3. ADC Polling UART Polling*/
/* UART and ADC Registers ('-' means default 0)
UART Registers:
UCSRnA: RXCn TXCn UDREn - - - - -
UCSRnB: RXCIEn TXCIEn UDRIEn RXENn TXENn - - -
UCSRnC: - - - - - UCSZn1 UCSZn0 -

ADC Registers:
ADMUX: REFS1 REFS0 - - MUX3 MUX2 MUX1 MUX0
ADCSA: ADEN ADSC - ADIF ADIE ADPS2 ADPS1 ADPS0
*/
/*
#define F_CPU 16000000UL
#define BAUD 9600
void usart_send (unsigned char ch);
int main_adc (void)
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
ADCSRA |= (1<<ADIF);			// clear ADIF not auto cleared in polling, cleared auto in interrupt

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


/* 4. ADC Interrupt UART Polling */
/* UART and ADC Registers ('-' means default 0)
UART Registers:
UCSRnA: RXCn TXCn UDREn - - - - -
UCSRnB: RXCIEn TXCIEn UDRIEn RXENn TXENn - - -
UCSRnC: - - - - - UCSZn1 UCSZn0 -

ADC Registers:
ADMUX: REFS1 REFS0 - - MUX3 MUX2 MUX1 MUX0
ADCSA: ADEN ADSC - ADIF ADIE ADPS2 ADPS1 ADPS0
*/
/*
#define F_CPU 16000000UL
#define BAUD 9600
void main_adc (void)
{
	cli();
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<TXEN1);				// Transmission enable
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);	// 8-bit

	// ADC Initialization
	ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
	ADCSRA = (1<<ADSC)|(1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x8F

	sei();

	while (1);
}

void usart_send (unsigned char ch)
{
	while (! (UCSR1A & (1<<UDRE1))); 	// polling transmission complete?
	UDR1 = ch;							// transmit ch
}

SIGNAL(ADC_vect)					// ADC interrupt
{
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

	ADCSRA |=(1<<ADSC);				// start conversion
}
*/


/* Lab: ADC Interrupt UART Interrupt */
/*
unsigned char ch ='x';
int a;
void main_adc (void)
{
	init_devices();
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);				// Transmission enable
	UCSR1C = (1<< UCSZ11)|(1<<UCSZ10);	// 8-bit

	// ADC Initialization
	ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x8F

	sei();
	//ADCSRA |=(1<<ADSC);				// start conversion

	DDRB = 0xFF;
	PORTB = 0xFF;

	while (1);
}

void usart_send (unsigned char ch)
{
	while (!(UCSR1A & (1<<UDRE1))); 	// polling transmission complete?
	UDR1 = ch;							// transmit ch
	PORTB ^= (1<<7);
}

SIGNAL(ADC_vect)					// ADC interrupt
{
	a = ADCL;					// ADC register
	a = a | (ADCH<<8);
	
	lcd_clear();	
	lcd_string(0,0, " 12345 Hong Jeong");
	lcd_string(2,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer
		
	ScreenBuffer_clear();					// clear screen buffer
	GLCD_Rectangle(50,0,60,a/10);		// output line gage
	lcd_xy(4,0); GLCD_4DigitDecimal(a);	// output ADC0
	
	if(a < 0)
	{
	ch = '-';
	a *= -1;
	}

	ch = (a/1000)+'0'; 	usart_send(ch);
	a = a % 1000;
	ch = (a/100)+'0'; 	usart_send(ch);
	a = a % 100;
	ch = (a/10)+'0'; 	usart_send(ch);
	a = a % 10;
	ch = (a)+'0'; 		usart_send(ch);
	ch = '\r'; 			usart_send(ch);
}

SIGNAL (USART1_RX_vect){
	unsigned char control = UDR1;
	if (control == 's'){
		PORTB ^= (1<<0);
		ADCSRA |=(1<<ADSC);				// start conversion
	}
}
*/

/* Quiz: ADC Interrupt UART Interrupt: TODO */
/*
unsigned char ch ='x';
char control[5];
int a, iter = 0;
void main_adc (void)
{
	init_devices();
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1);				// Transmission enable
	UCSR1C = (1<< UCSZ11)|(1<<UCSZ10);	// 8-bit

	// ADC Initialization
	ADMUX = (1<<REFS1)|(1<<REFS0);  // 0xC0, 2.56V Vref, right-justified, channel 0
	ADCSRA = (1<<ADEN)|(1<<ADIE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0); // 0x8F

	sei();
	//ADCSRA |=(1<<ADSC);				// start conversion

	DDRB = 0xFF;
	PORTB = 0xFF;

	while (1);
}

void usart_send (unsigned char ch)
{
	while (!(UCSR1A & (1<<UDRE1))); 	// polling transmission complete?
	UDR1 = ch;							// transmit ch
	PORTB ^= (1<<7);
}

SIGNAL(ADC_vect)					// ADC interrupt
{
	a = ADCL;					// ADC register
	a = a | (ADCH<<8);
	
	lcd_clear();
	lcd_string(0,0, " 12345 Hong Jeong");
	lcd_string(2,0,"ADC0 Potentiometer");	// output ADC0 Potentiometer
	
	ScreenBuffer_clear();					// clear screen buffer
	GLCD_Rectangle(50,0,60,a/10);		// output line gage
	lcd_xy(4,0); GLCD_4DigitDecimal(a);	// output ADC0
	
	if(a < 0)
	{
		ch = '-';
		a *= -1;
	}

	ch = (a/1000)+'0'; 	usart_send(ch);
	a = a % 1000;
	ch = (a/100)+'0'; 	usart_send(ch);
	a = a % 100;
	ch = (a/10)+'0'; 	usart_send(ch);
	a = a % 10;
	ch = (a)+'0'; 		usart_send(ch);
	ch = '\r'; 			usart_send(ch);
}

SIGNAL (USART1_RX_vect){
	if (iter == 0){
		control[0] = UDR1;
		iter++;
	}
	if (iter == 1){
		control[1] = UDR1;
		if (control[0] == 's' && control[1] == 't'){	
			iter = 0;
			PORTB ^= (1<<0);
			ADCSRA |= (1<<ADSC);				// start conversion
		}
	}
}
*/










/* Serial communication: ADC to COM and GLCD */
/*
int main (void)
{
init_devices();
lcd_clear();
//ScreenBuffer_clear();
while(1)
{
Data_ADC0 = Read_Adc_Data(0);		// Analog port 0 reading

if(Task1_Of_Timer2 == 1)			// Execute if Task1_Of_Timer2
{									// 50ms interval
ToggleBit(PORTB,PORTB0);		// ~PORTB.0
USART1_putchdecu(Data_ADC0);
puts_USART1("\r\n");
lcd_clear();
lcd_xy(3,3);
GLCD_4DigitDecimal(Data_ADC0);

Task1_Of_Timer2=0;				// Reset Task1_Of_Timer2
}

}
}
*/

/* ADC to COM
void usart_send (unsigned char ch);
int main (void)
{
// UART initialization: 8N1, transmitter
UCSR1B = (1<<TXEN1);				//Transmission enable
UCSR1C = (1<< UCSZ11)|(1<<UCSZ10);	// 8-bit
UBRR1L = F_CPU/16/BAUD-1;			// UBRR
// ADC Initialization
ADCSRA= 0x87; //make ADC enable and select ck/128
ADMUX= 0xC0;  //2.56V Vref, right-justified, channel 0

while (1)
{
ADCSRA|=(1<<ADSC);				// start conversion
while((ADCSRA&(1<<ADIF))==0);	// wait for conversion to finish
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
while (! (UCSR1A & (1<<UDRE1))); 	//wait until UDR0 is empty
UDR1 = ch;							//transmit ch
}

void usart_print(char* str)
{
int i = 0;
while(str[i] != 0)
usart_send(str[i]);
}
*/


/* CDS  */
/*
int main (void)
{
Data_ADC2 = Read_Adc_Data(1) / 50;		// read analog port 0
_delay_ms(1000);

lcd_clear();
ScreenBuffer_clear();
lcd_string(0,0,"ADC1 CDS");
GLCD_Circle(35,55,Data_ADC2);
lcd_xy(1,0);
GLCD_4DigitDecimal(Data_ADC2);
}
*/
