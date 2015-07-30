#include<avr/io.h>
#include<util/delay.h>

void main()
{
unsigned char delay;
DDRG|=(1<<4)|(1<<0);   //G1 Pulse G0 DIR
PORTG|=(1<<0);
for(delay=0;delay<18000;delay++)
{
PORTG|=(1<<4);
_delay_us(555);
PORTG&=~(1<<4);
_delay_us(5);
}



}
/*

			for(variance>=0;variance<10;variance++)   //variance<120 for 120 degree motion
	{motor_dir_cw();
	PORTC=0x0f;
	for(degree=0;degree<50;degree++)
	{
	PORTG|=(1<<4);
	_delay_us(555);
	PORTG&=~(1<<4);
	_delay_us(5);
	}

	
	}
	
	clockwise=0;
	counterwise=1;

	}
	//if(counterwise==1)
	{
	for(variance<=10;variance>0;variance--)
	{motor_dir_ccw();
	PORTC=0xf0;
	for(degree=0;degree<200;degree++)
	{
	PORTG|=(1<<4);
	_delay_us(555);
	PORTG&=~(1<<4);
	_delay_us(5);
	}
	
	}
	clockwise=1;
	counterwise=0;
	
}	

	







for(variance=degree_set_new;variance<=10;variance++)
{
for(degree=0;degree<200;degree++)   //degree<5 for 1 degree resolution
{
PORTC^=0xfd;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
seconds=0;
lcd_gotoxy1(8);
lcd_num(variance);
lcd_gotoxy2(8);
lcd_num(degree_set_new);
if(variance==10)
{
_delay_ms(1000);
PORTG=0x00;
PORTG|=(1<<0);
//variance=degree_set_new;
}
if(variance==degree_set_new)
{
_delay_ms(1000);
PORTG=0x00;
PORTG&=~(1<<0);
}


}

}
/*include"boot1.h"

/*volatile unsigned char degree;
//volatile unsigned char degree_p[2];	
volatile unsigned int degree_set;	
volatile unsigned int test;	
volatile unsigned int degree_set_new;	
volatile unsigned char seconds;	
volatile unsigned int clk=0;	
volatile unsigned int ccw=1200;	

ISR(TIMER1_COMPA_vect)
{
unsigned char deg_cw;
unsigned char deg_ccw;
unsigned char read_val;
unsigned char variance;
PORTC^=0xff;
seconds++;
//degree_set_new=pgm_read_byte(&degree_p);//eeprom_read_byte(25);
degree_set_new=eeprom_read_byte(25);
read_val=degree_set_new;
lcd_gotoxy2(0);
lcd_num(degree_set_new);
degree_set_new=10-read_val;    //120 for 120 degree correction
//deg_cw=degree_set_new;
//deg_ccw=degree_set_new*10;
lcd_gotoxy1(4);
lcd_num(degree_set_new);


if(seconds==1)     // Put seconds=240 for 4 minute delay.
{
for(variance=degree_set_new;variance<=10;variance++)
{
for(degree=0;degree<200;degree++)
{
PORTC^=0xfd;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
lcd_gotoxy2(10);
lcd_num(variance);
}

}




}



//PORTC^=0xff;
}


*/

/*

void align_ccw(void)
{
unsigned char degree_ccw;
PORTG=0x00;

PORTG&=~(1<<0);
PORTC=0x01;

for(degree_ccw=0;degree_ccw<50;degree_ccw++)
{
PORTC=0xff;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
}

}

void align_cw(void)
{
unsigned char degree_cw;
PORTG=0x00;
PORTG|=(1<<0);

PORTC=0x02;
for(degree_cw=0;degree_cw<50;degree_cw++)
{
PORTC=0xff;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
}

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
unsigned char read_val;
DDRA|=(1<<3);
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
//eeprom_write_byte(24,46);

while(1)
{
if(s1 && ((read_val=eeprom_read_byte(25))>=0))
{
_delay_ms(300);
align_ccw();
degree_set++;
//degree_p[0]=degree_set;
//boot_program_page(0x100,degree_p);
eeprom_write_byte(25,degree_set);
test=eeprom_read_byte(25);
//uart_str("Degree\t");
lcd_num(test);
lcd_cmd(0x02);

}
if(s2 && (((read_val=eeprom_read_byte(25)<=10))))
{
_delay_ms(300);
align_cw();
//if(degree_set!=1)
degree_set--;
//degree_p[0]=degree_set;
//boot_program_page(0x100,degree_p);
eeprom_write_byte(25,degree_set);
test=eeprom_read_byte(25);

//uart_str("Degree\t");
lcd_num(test);

lcd_cmd(0x02);
}


if(s4)
{
timer_init();
}

if(s3)
{
eeprom_write_byte(25,0);
}



}
}


*/