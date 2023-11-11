/* Joystick */
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

unsigned int mData_ADC3 = 0;
unsigned int mData_ADC4 = 0;
	
int main_joystick (void)
{
	
	mData_ADC3 = Read_Adc_Data(3) / 14;			// read analog port
	mData_ADC4 = Read_Adc_Data(4) / 8;			// read analog port
	_delay_ms(3000);			
	lcd_clear();				
	ScreenBuffer_clear();			
	lcd_string(0,0,"ADC3,ADC4 JOYSTICK");		// output joystick position
	GLCD_Line(32,0,31,127);   GLCD_Line(0,63,63,63); // line output horizontal vertical
	GLCD_Circle(mData_ADC3,mData_ADC4,5);			// output position
	
	return 0;
}
	