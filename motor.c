#include<avr/io.h>
#include<util/delay.h>
#define s1 (!(PIND&(1<<6)))
#define s2 (!(PIND&(1<<7)))
#define s3 (!(PINE&(1<<6)))
#define s4 (!(PINE&(1<<7)))
#include<LCD_mega128.h>
#include<avr/interrupt.h>     // G0=1 CCW   G0=0 CW
#include<avr/eeprom.h>
#include<uart.h>
#include<avr/pgmspace.h>

volatile unsigned char degree;
volatile unsigned char degree_set_new;
volatile unsigned char seconds=1;
volatile unsigned char iteration_ccw;
volatile unsigned char iteration_cw;

ISR(TIMER1_COMPA_vect)
{
sei();
unsigned char deg_cw;
unsigned char deg_ccw;
unsigned char read_val;
unsigned char variance;
PORTC^=0xff;
/*degree_set_new=eeprom_read_byte(25);
read_val=degree_set_new;
lcd_gotoxy2(0);
lcd_num(degree_set_new);*/
PORTG|=(1<<0);
if(seconds==1)     // Put seconds=240 for 4 minute delay.
{
for(degree=0;degree<200;degree++)
{
PORTG|=(1<<4);
_delay_us(555);
PORTG&=~(1<<4);
_delay_us(5);
}

}



}


void motor_execute(void)
{
	unsigned char degree_exec;
	degree_exec=eeprom_read_byte(25);
	
	for(degree=0;degree<200;degree++)
	{
	PORTG|=(1<<4);
	_delay_us(555);
	PORTG&=~(1<<4);
	_delay_us(5);
	}
	while(degree_exec<=120)
	{
	_delay_ms(24000);
	degree_exec++;
		if(degree_exec==120)
		{
		//chage motor_dir
		//degree_set=0;
		}
	}

}



void align_ccw(void)
{
PORTG=0x00;
PORTG&=~(1<<0);
PORTC=0x01;
unsigned char degree_ccw;
for(degree_ccw=0;degree_ccw<50;degree_ccw++)
{
PORTC=0xff;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
}
degree_set_new--;
eeprom_write_byte(25,degree_set_new);
}

void align_cw(void)
{

PORTG=0x00;
PORTG|=(1<<0);
unsigned char degree_cw;
PORTC=0x02;
for(degree_cw=0;degree_cw<50;degree_cw++)
{
PORTC=0xff;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
}
degree_set_new++;
eeprom_write_byte(25,degree_set_new);
}

void timer_init()
{
TCCR1B|=(1<<WGM12)|(1<<CS10)|(1<<CS12);
OCR1A=15625;
TIMSK|=(1<<OCIE1A);
sei();
}


void main()
{
DDRG|=(1<<3)|(1<<4)|(1<<2)|(1<<0);
PORTG|=(1<<2);
PORTG&=~(1<<0);
DDRC=0xff;
//TCCR1A=0x00;
uart_init();
lcd_init();
//lcd_char('a');
DDRD&=~((1<<6)|(1<<7));
DDRE&=~((1<<6)|(1<<7));
PORTD|=(1<<6)|(1<<7);
PORTE|=(1<<6)|(1<<7);
DDRC=0xff;
DDRG|=(1<<0)|(1<<4);
PORTG&=~(1<<0);

while(1)
{
if(s1)
{
cli();
align_cw();

}
if(s2)
{
cli();
align_ccw();
}
if(s4)
{
timer_init();
}


}


}
