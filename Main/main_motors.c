/* Stepper Motor */

#define F_CPU 16000000UL
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
//#include "_init.h"
//#include "_interrupt.h"
#include "_port.h"
#include "_timer2.h"
//#include "_uart.h"
#include "_glcd.h"
#define BAUD 9600

/* Step motors */
/*
int main_motors ()
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
	
	return 0;
}
*/


/* PWM Motors */
/* Timer Registers
	OCR0
	TCCR0: WGM00 WGM01 CS02 CS01 CS00
*/
/*
void  main_motors(void)
{
	DDRB |= (1<<4);	//OC0 as output
	TCCR0 = (1<<COM01)|(1<<WGM01)|(1<<WGM00)|(1<<CS2); // Fast PWM, non-inverted, prescale=64
	
	while(1)
	{
		for(unsigned char k = 0; k < 255; k++)
		{
			OCR0 = k;
			_delay_ms(10);
		}
	}
}
*/