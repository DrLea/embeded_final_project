/* Graphics: basics */
#define F_CPU 16000000UL
#include <avr/io.h>				//<avr/portpins.h> <avr/iom128.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <stdbool.h>
#include <math.h>


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

void tank(int x, int y){
	GLCD_Dot(y-4,x+2); GLCD_Dot(y-4,x+3); GLCD_Dot(y-4,x+4);
	GLCD_Dot(y-3,x+2); GLCD_Dot(y-3,x+3); GLCD_Dot(y-3,x+4); GLCD_Dot(y-3,x+5); GLCD_Dot(y-3,x+6);
	GLCD_Dot(y-2,x+2); GLCD_Dot(y-2,x+3); GLCD_Dot(y-2,x+4);
	GLCD_Dot(y-1,x); GLCD_Dot(y-1,x+1); GLCD_Dot(y-1,x+2); GLCD_Dot(y-1,x+3); GLCD_Dot(y-1,x+4); GLCD_Dot(y-1,x+5);
	GLCD_Dot(y,x); GLCD_Dot(y,x+1); GLCD_Dot(y,x+2); GLCD_Dot(y,x+3); GLCD_Dot(y,x+4); GLCD_Dot(y,x+5);
}

void miss(int x, int y){
	GLCD_Dot(y, x+1); GLCD_Dot(y, x+2);
	GLCD_Dot(y+1, x); GLCD_Dot(y+1, x+2);
	GLCD_Dot(y+2, x+1);
}


/* Graphics: Characters  */
/**/
char Dis_Scr_IO_ON1[]={"O"};
char Dis_Scr[]={"#$%&'()*+,-./0123456"};
int main_graphics(void)
{
	init_devices();								// initialize LCD
	//BillieJeanBassLine();					// sound
	//NokiaTone();
	//playNokiaRingtone();
	signed int y = 32-0.25;
	signed int x = 64-1.25;
	int newY = 0;
	int newX = 0;
	int speed = 15;
	unsigned char reload = 2;
	unsigned int counter = 0;
	unsigned int miss_counter = 0;
	unsigned int frame_counter = 0;
	bool shouted_flag = false;
	bool enemy_flag = true;
	int miss_array[6] = {0,0,0,0,0,0};
	unsigned char tail = 1;
	DDRD = 0x0;
	
	int x_temp = 5+rand()%120;
	int y_temp = 15+rand()%15;
	while(1){
		newY = Read_Adc_Data(3) / 16 - 31;			// read analog port
		newX = 127 - Read_Adc_Data(4) / 8 - 64;			// read analog port
		y = y + newY/speed;
		x = x + newX/speed;
		if (x<5)
			x = 5;
		else if (x>123)
			x = 123;
		if (y<5)
			y = 5;
		else if (y>59)
			y = 59;
		_delay_ms(100);
		lcd_clear();
		ScreenBuffer_clear();
		
		if (frame_counter%3==0){
			if (x_temp>5 && rand()%2 == 1)
				x_temp -=2;
			else if (x_temp<120)
				x_temp +=2;
			if (y_temp>15 && rand()%2 == 1)
				y_temp -=2;
			else if (y_temp<58)
				y_temp +=2;
		}
		tank(x_temp, y_temp);
		for (int i = 0; i<6;i+=2){
			miss(miss_array[i], miss_array[i+1]);
		}

		GLCD_Dot(5,5);
		GLCD_3DigitDecimal(reload);
		GLCD_Line(10,2,10,80); // line output horizontal vertical
		GLCD_Circle(y,x,5);
		GLCD_Dot(y-4,x);
		GLCD_Dot(5,25);
		GLCD_3DigitDecimal(counter);
		GLCD_Dot(5,45);
		GLCD_3DigitDecimal(miss_counter);
		
		if (!(PIND & 0x10) && !reload){
			reload = 10;
			lcd_string(0,0, "Boom!!!");
			if (abs(x-x_temp)<=5 && abs(y-y_temp)<=5){
				shouted_flag = true;
				counter += 1;
				Boom();
			}
			else if (abs(x-40)<=5 && abs(y-5)<=5){
				counter = 0;
				playNokiaRingtone();
				
			}
			else if (abs(x-60)<=5 && abs(y-5)<=5){
				miss_counter = 0;
				playNokiaRingtone();
			}
			else{
				miss_array[tail-1] = x;
				miss_array[tail] = y;
				tail = (tail + 2) % 6;
				miss_counter+=1;
				SError();
				}
		}
		
		if (counter%5==0 && shouted_flag){
			shouted_flag = false;
			NokiaTone();
		}
		
		if (counter%7==0 && shouted_flag){
			shouted_flag = false;
			BillieJeanBassLine();
		}
		
		if (reload>0){
			reload -= 1;
			if (reload == 5 && shouted_flag){
				x_temp = 5+rand()%120;
				y_temp = 15+rand()%59;
				tank(x_temp, y_temp);
				shouted_flag = false;
			}
		}
		
			
		frame_counter+=1;	
	}	
	return 0;
}
/**/


/* Graphics: Random circles 	*/
/*
int main_graphics (void)
{
	// random numbers and seed.
	uint16_t u_rand_x = 0;
	uint16_t u_rand_y = 0;
	uint16_t u_rand_r = 0;
	time_t t;
	init_devices();
	lcd_clear();
	srand((unsigned) time(&t));
	while(1){
		u_rand_x = rand()%64;
		u_rand_y = rand()%128;
		u_rand_r = rand()%10;
		GLCD_Circle(u_rand_x, u_rand_y, u_rand_r);
		_delay_ms(100);
	}
}
*/
	
/* 	Graphics: Random movement 	*/
/*
int main_graphics (void)
{
	//random numbers and seed.
	uint16_t u_rand_x = 0, old_x = 0;
	uint16_t u_rand_y = 0, old_y = 0;
	time_t t;
		
	init_devices();
	lcd_clear();
	srand((unsigned) time(&t));
	while(1){
		S_Star();
		u_rand_x = rand()%64;
		u_rand_y = rand()%128;
		GLCD_Line(old_x, old_y, u_rand_x, u_rand_y);
		old_x = u_rand_x; old_y = u_rand_y;
		_delay_ms(100);
	}
}
*/