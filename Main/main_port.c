/* 2. Port Programming: Blinking */
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


int main_port(void){
	DDRB = 0xFF;
	PORTB = 0xAA;
	int flag = 0;
	init_devices();
	lcd_clear();
	lcd_string(0,0,"12345 Hong Jeong");
	while(1){
		PORTB = ~PORTB;
		_delay_ms(1000);
		lcd_clear();
		for (int i=0; i<8; i++){
			if(flag) {
				lcd_string(0,0,"12345 Hong Jeong");
				//GLCD_Circle(50,10*i+10,2);
				flag = 0;
			}
			else {
				lcd_string(0,0,"12345 Hong Jeong");
				GLCD_Circle(50,10*i+10,4);
				flag = 1;
			}
			//ScreenBuffer_clear();	
		}
		
	}
	return 0;
}


/* Port Programming: Rotation */
/*
int main_port(void){
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
