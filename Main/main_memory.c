/*  Accessing Flash Memory */
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

/*
#include <avr/pgmspace.h>
const unsigned char PROGMEM lookup[] = "0123456789abcdefzhijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ!@#$%^&*()_-<>,.~`+";
int main_memory(void)
{
	unsigned char a;
	DDRB = 0xFF;
	
	init_devices();
	while (1){
		for (int i=0; i < 128; i++){
			a = pgm_read_byte(&lookup[i]);
			PORTB = ~(a - 0x30);
			_delay_ms(1000);
			lcd_clear();							// clear LCD
			ScreenBuffer_clear();					// clear screen buffer
			lcd_string(0,0,"12345 Hong Jeong");
			lcd_string(2,0, "EEPROM Data");
			lcd_xy(4,0);
			lcd_char(a);
		}
	}
	return 0;
}
*/

/* Accessing EEPROM */
#include <avr/eeprom.h>

unsigned char EEMEM *lookup_p;
unsigned char lookup[]="012345";
unsigned char result;

int main_memory(void)
{
	DDRD = 0x00;
	init_devices();
	//S_Start();
	S_Start();
	while (1){
		if((PIND&(1<<0)) == 0){  //if PD0 is LOW
			//eeprom_write_byte(lookup_p,lookup[4]);
			//In_EEPROM_Write(*lookup_p,lookup[5]);
			In_EEPROM_Write(100,lookup[5]);
		}
		else {
			//result = eeprom_read_byte(lookup_p); //read from EEPROM
			//result = In_EEPROM_Read(*lookup_p);
			result = In_EEPROM_Read(100);
			_delay_ms(1000);
			lcd_clear();							// clear LCD
			ScreenBuffer_clear();					// clear screen buffer
			lcd_string(0,0,"12345 Hong Jeong");
			lcd_string(2,0,"EEPROM Data");
			lcd_xy(4,0);
			lcd_char(result);
		}
	}

	return 0;
}


