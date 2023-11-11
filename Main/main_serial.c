/* Serial communication*/
/* http://ebook.pldworld.com/_Semiconductors/Atmel/Databook%20CDROM/Atmel/acrobat/doc1451.pdf */

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

/* Atmega128 board: PD2,3:RXD1,TXD1 */
/* 	UART Registers:
	UCSRA: RXC   TXC   UDRE	
	UCSRB: RXCIE TXCIE UDRIE RXEN TXEN UCSZ2 
	UCSRC: UCSZ1 UCSZ0 
*/	
/* SimulIDE UART1 is not working */


/* Serial communications UART1 for Atmega128 board: Polling */

void main_serial(void)										
{
	unsigned char data;
	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;		
	UCSR1B = (1<<RXEN1)|(1<<TXEN1);		// receiver and transmitter
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);	// 8-bit
	sei();								//never forget
		
	DDRB = 0xFF;
	PORTB = 0xAA;	
	while (1){
		while (!(UCSR1A & (1<<RXC1)));	// receiver polling
		data = UDR1;
		PORTB = ~PORTB;
		_delay_ms(10);
		
		while(!(UCSR1A & (1<<UDRE1)));	// transmitter polling
		UDR1 = data;
		_delay_ms(10);
		PORTB = ~PORTB;
	}	
}


/* Serial communications UART0 for SimulIDE: Polling */
/*
void main_serial(void)
{
	unsigned char data;
	// UART1: 9600 8N1
	UBRR0H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR0L = F_CPU/16/BAUD-1;
	UCSR0B = (1<<RXEN0)|(1<<TXEN0);		// receiver and transmitter
	UCSR0C = (1<<UCSZ01)|(1<<UCSZ00);	// 8-bit
	sei();								//never forget
	
	DDRB = 0xFF;
	PORTB = 0xAA;
	while (1){
		while (!(UCSR0A & (1<<RXC0)));	// receiver polling
		data = UDR0;
		_delay_ms(1000);
		
		while(!(UCSR0A & (1<<UDRE0)));	// transmitter polling
		UDR0 = data + 1;
		_delay_ms(1000);
	}
}
*/

/* Serial communication: Interrupt */
/* Atmega128 board: PD2,3:RXD1,TXD1 */
/* 	UART Registers:
	UCSRA: RXC   TXC   UDRE	
	UCSRB: RXCIE TXCIE UDRIE RXEN TXEN UCSZ2 
	UCSRC: UCSZ1 UCSZ0 
*/	
/*
int main_serial (void)
{
	DDRB = 0xFF;
	PORTB = 0xAA;

	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B |= (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1); // receiver/ transmit interrupt
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);			// character size 8
	sei();
	
	while(1);
	return 0;
}

		

SIGNAL(USART1_RX_vect)					// Serial ISR
{
	unsigned char data;
	data = UDR1;
	if ((data >= 0x41)&&(data <=0x5A)) data = data + 0x20;
	else if ((data >= 0x61) && (data <= 0x7A)) data = data - 0x20;
	UDR1 = data;			
}
*/


/* Lab by Interrupt  */
/*
Receive two numbers,from Data Visualizer, multiply them, and display like "3 x 4 = 12" on GLCD"
*/

/* SimulIDE Arduino Uno Test */
/*
#define BAUD_RATE 9600
void usart_init (void) 
{ 
	UCSR0B = (1<<TXEN0); 
	UCSR0C = (1<< UCSZ01)|(1<<UCSZ00); 
	UBRR0L = F_CPU/16/BAUD_RATE-1; //UBRR0L = 103; 
	
}
void usart_send (char ch) 
{ 
	while (! (UCSR0A & (1<<UDRE0))); //wait until UDR0 is empty 
	UDR0 = ch; //transmit ch 
}
void usart_sendStr(char str[]) { 
	unsigned char i = 0;
	while(str[i]) { 
		usart_send(str[i]); 
		i++; 
	} 
}
	
int main_serial (void) { 
	usart_init(); //initialize the USART 
	while(1) { 
		usart_sendStr("Hello World!\n\r"); 
		_delay_ms(1000); 
	}
return 0; 
}
*/



/* Lab: External/Timer Interrupt + Serial communication with Interrupt */
/*
unsigned char ch, ch_num = 0;
char buf[100];
char *y;
int main_serial (void)
{
	init_devices();								// initialize LCD
	lcd_clear();
	lcd_string(0,0, "12345 Hong Jeong");
	lcd_xy(5,5);
	GLCD_1DigitDecimal(0,1);
	
	DDRB = 0xFF;
	PORTB = 0xAA;

	// INT0 interrupt
		EICRA = 1<<ISC01;		//INT0 falling
		EICRB = 0x00;
		EIMSK = 1<<INT0;	//INT0 enabled

	// UART1: 9600 8N1
	UBRR1H = (F_CPU/16/BAUD-1)>>8;		// UBRR
	UBRR1L = F_CPU/16/BAUD-1;
	UCSR1B = (1<<RXCIE1)|(1<<RXEN1)|(1<<TXEN1); // receiver/ transmit interrupt
	UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);			// character size 8
	sei();
	
	while(1);
	return 0;
}

//SIGNAL (INT0_vect){
	//nop();
//}
SIGNAL(USART1_RX_vect)					// Serial ISR
{
	ch = UDR1;
	if (ch != '='){
		buf[ch_num] = ch;
		ch_num++;
	}
	else {		
		buf[ch_num] = 0x00;
		itoa(atoi(buf), buf, 10);
		while (*buf != 0x00){
			UDR1 = *buf;
			buf++;
			_delay_ms(10);
		}
		ch_num = 0;
	}
	
}
*/


/* Serial Communication with Buffer */
/* Atmega128 board: PD2,3:RXD1,TXD1 */
/* 	UART Registers:
	UCSRA: RXC   TXC   UDRE	
	UCSRB: RXCIE TXCIE UDRIE RXEN TXEN UCSZ2 
	UCSRC: UCSZ1 UCSZ0 
*/	
/* UART Buffer Defines */

//#define UART_RX_BUFFER_SIZE 128 /* 1,2,4,8,16,32,64,128 or 256 bytes */
//#define UART_RX_BUFFER_MASK ( UART_RX_BUFFER_SIZE - 1 )
//#if ( UART_RX_BUFFER_SIZE & UART_RX_BUFFER_MASK )
//#error RX buffer size is not a power of 2
//#endif
//#define UART_TX_BUFFER_SIZE 128 /* 1,2,4,8,16,32,64,128 or 256 bytes */
//#define UART_TX_BUFFER_MASK ( UART_TX_BUFFER_SIZE - 1 )
//#if ( UART_TX_BUFFER_SIZE & UART_TX_BUFFER_MASK )
//#error TX buffer size is not a power of 2
//#endif
///* Static Variables */
//static unsigned char UART_RxBuf[UART_RX_BUFFER_SIZE];
//static volatile unsigned char UART_RxHead;
//static volatile unsigned char UART_RxTail;
//static unsigned char UART_TxBuf[UART_TX_BUFFER_SIZE];
//static volatile unsigned char UART_TxHead;
//static volatile unsigned char UART_TxTail;
///* Prototypes */
//void InitUART( int baudrate );
//unsigned char ReceiveByte( void );
//void TransmitByte( unsigned char data );
///* main - a simple test program*/
//void main_serial( void )
//{
	//init_devices();
	//lcd_clear();
	 //DDRB = 0xFF;
	 //PORTB = 0x00;
	 //InitUART( 9600 ); /* set the baudrate to 9600 bps using a 16MHz crystal */
	 //sei(); /* enable interrupts => enable UART interrupts */
	 //
	 //while ( 1 ) /* forever */
	 //{
		//TransmitByte( ReceiveByte() ); /* echo the received character */
	 //}
//}
///* initialize UART */
//void InitUART( int baudrate )
//{
	 //unsigned char x;
	 //UBRR1H = (F_CPU/16/baudrate-1)>>8;		// UBRR
	 //UBRR1L = F_CPU/16/baudrate-1;
//
	 ///* enable UART receiver and transmitter, and
	 //receive interrupt */
	 //UCSR1B = (1<<RXCIE)|(1<<RXEN1)|(1<<TXEN1);		// receiver and transmitter
	 //UCSR1C = (1<<UCSZ11)|(1<<UCSZ10);	// 8-bit
	 //x = 0; /* flush receive buffer */
	 //UART_RxTail = x;
	 //UART_RxHead = x;
	 //UART_TxTail = x;
	 //UART_TxHead = x;
//}
///* interrupt handlers */
//SIGNAL (USART1_RX_vect)
//{
	//PORTB ^= (1<<0);
	//unsigned char data;
	//unsigned char tmphead;
	//data = UDR1; /* read the received data */
	///* calculate buffer index */
	//tmphead = ( UART_RxHead + 1 ) & UART_RX_BUFFER_MASK;
	//UART_RxHead = tmphead; /* store new index */
	//if ( tmphead == UART_RxTail )
	//{
		///* ERROR! Receive buffer overflow */
	//}
//
	//UART_RxBuf[tmphead] = data; /* store received data in buffer */
	//lcd_xy(3,0);
	//lcd_char(data);
//}
  //
//SIGNAL (USART1_UDRE_vect)
//{
	//unsigned char tmptail;
	///* check if all data is transmitted */
	//if ( UART_TxHead != UART_TxTail )
	//{
		///* calculate buffer index */
		//tmptail = ( UART_TxTail + 1 ) & UART_TX_BUFFER_MASK;
		//UART_TxTail = tmptail; /* store new index */
		//UDR1 = UART_TxBuf[tmptail]; /* start transmmition */
	//}
	//else
	//{
		//UCSR1B &= ~(1<<UDRIE); /* disable UDRE interrupt */
	//}
//}
///* Read and write functions */
//unsigned char ReceiveByte( void )
//{
	//unsigned char tmptail;
	//while ( UART_RxHead == UART_RxTail ); /* wait for incoming data */
	//tmptail = ( UART_RxTail + 1 ) & UART_RX_BUFFER_MASK;/* calculate buffer index */
	//UART_RxTail = tmptail; /* store new index */
	//return UART_RxBuf[tmptail]; /* return data */
//}
//void TransmitByte( unsigned char data )
//{
	//unsigned char tmphead;
	///* calculate buffer index */
	//tmphead = ( UART_TxHead + 1 ) & UART_TX_BUFFER_MASK; /* wait for free space in buffer */
	//while ( tmphead == UART_TxTail );
	//UART_TxBuf[tmphead] = data; /* store data in buffer */
	//UART_TxHead = tmphead; /* store new index */
	//UCSR1B |= (1<<UDRIE); /* enable UDRE interrupt */
//}
//unsigned char DataInReceiveBuffer( void )
//{
	//return ( UART_RxHead != UART_RxTail ); /* return 0 (FALSE) if the receive buffer is empty */
//}
