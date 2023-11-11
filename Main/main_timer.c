/* 1. Timer/Counter0 Normal Mode
Page 92, https://ww1.microchip.com/downloads/en/DeviceDoc/doc2467.pdf
TIFR:  OCF2|TOV2|ICF1|OCF1B|TOV1|OCF0|TOV0
TCCR0: FOC0|WGM00|COM01|CON00|WGM01|CS02|CS01|CS00
*/

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
#include "_init.h"
#include "_interrupt.h"
#include "_port.h"
#include "_timer2.h"
#include "_uart.h"
#include "_glcd.h"

/* Timer Registers
	TCNT0
	OCR0
	TIFR:  OCF0 TOV0
	TIMSK: OCIE0 TOIE0
	TCCR0: WGM00 WGM01 CS02 CS01 CS00
*/
/*
void main_timer (void)
{
	// Graphics
	init_devices();
	lcd_string(0,0,"12345 Hong Jeong");
	
	// application
	DDRB = 1<<5;
	PORTB &= ~(1<<5);  //pb5=0

	while (1)
	{
		// delay
		for (int i=0; i < 100; i++){
			TCNT0 = 0x00;					// 0~255
			TCCR0 |= (1<<CS02) | (1<< CS01) | (1<< CS00); // clock/1024
			while(!(TIFR&(1<<TOV0)));	// wait
			TCCR0 = 0;						// stop
			TIFR |= (1<<TOV0);				// reset flag
			lcd_xy(4,0); GLCD_4DigitDecimal(i);
		}
		// application
		PORTB ^= (1<<5);	
	}
}
*/

/* 2. Timer/Counter0 CTC Mode
*/
void main_timer (void)
{
	// Graphics
	init_devices();
	lcd_string(0,0,"12345 Hong Jeong");
	
	// application
	DDRB = 1<<5;
	PORTB &= ~(1<<5);  //PB5=0
	
	while (1)
	{	
		// delay
		for (int i=0; i < 100; i++){
			TCCR0 = (1<<WGM01) | (1<<CS02) | (1<< CS01) | (1<< CS00); // CTC and Clock/1024
			OCR0 = 255;						// 0~255
			while(!(TIFR&(1<<OCF0))){}	// wait
			TCCR0 = 0;						// stop
			TIFR |= (1<<OCF0);				// reset flag
			lcd_xy(4,0); GLCD_4DigitDecimal(i);
		}
		// application
		PORTB ^= (1<<5);
	}
}

/* 3. Timer/Counter0 CTC Mode Application
*/
//int main (void)
//{
	//// Graphics
	//init_devices();
	//lcd_string(0,0,"12345 Hong Jeong");
		//
	//// application
	//DDRB = 1<<5;
	//PORTB &= ~(1<<5);  //PB5=0
	//
	//while(1){
		//for (int offset=255; offset >0; offset -= 10)
		//{
			//// delay
			//for (int i=0; i < 100; i++){
				//TCCR0 = (1<<WGM01) | (1<<CS02) | (1<< CS01) | (1<< CS00); // CTC and Clock/1024
				//OCR0 = offset;						// 0~255
				//while((TIFR&(1<<OCF0))==0){}	// wait
				//TCCR0 = 0;						// stop
				//TIFR = (1<<OCF0);				// reset flag
				//lcd_xy(4,0); GLCD_4DigitDecimal(i);
			//}
			//// application
			//PORTB ^= (1<<5);
		//}
	//}
	//return 0;
//}

/* 4. Timer/Counter0 Fast PWM mode
*/
//int main (void)
//{
	//// Graphics
	//init_devices();
	//lcd_string(0,0,"12345 Hong Jeong");
	//
	//// application
	//DDRB = 1<<4; //OC0 PWM output
	//
	//while(1){
		//for (int offset=255; offset >0; offset -= 10)
		//{
			//// delay
			//for (int i=0; i < 100; i++){
				//TCCR0 = (1<<WGM01) | (1<<WGM00) | (1<<COM01) | (1<<COM00) | (1<<CS02) | (1<< CS01) | (1<< CS00); // fast PWM, OC0, and Clock/1024
				//OCR0 = offset;						// 0~255 input signal
				//while((TIFR&(1<<OCF0))==0){}	// wait
				//TCCR0 = 0;						// stop
				//TIFR = (1<<OCF0);				// reset flag
			//}
			//// display result
			//lcd_xy(4,0); GLCD_4DigitDecimal(offset);
			//
		//}
	//}
	//return 0;
//}


