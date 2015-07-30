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
volatile unsigned char flag;

unsigned char clockwise;
	unsigned char counterwise;
ISR(INT4_vect) //CCW
{  
    flag=0;
	_delay_ms(500);
	align_ccw();
    flag=1;

}

ISR(INT5_vect) // CW
{   flag=0;
	_delay_ms(500);
	align_cw();
    flag=1;
}
ISR(INT6_vect) // Setting degree_exec=0
{
	_delay_ms(500);
	PORTC=0xf2;
	eeprom_write_byte(25,1);
	lcd_gotoxy2(5);
	lcd_string("  ");
	lcd_gotoxy2(5);
	lcd_num(eeprom_read_byte(25));

	}

ISR(INT7_vect)
	{
	lcd_gotoxy2(10);
	lcd_string("  ");
	lcd_gotoxy2(10);
	lcd_num(eeprom_read_byte(25));
	

	}
void motor_execute(void)
{cli();
	unsigned char degree_exec_cw;
	unsigned char degree_exec;
	unsigned char degree_exec_ccw;
	
	unsigned char variance;
	
	degree_set_new=eeprom_read_byte(25);
	/*lcd_gotoxy1(9);
	lcd_string("  ");
	lcd_gotoxy1(6);
	lcd_string("EX=");
	lcd_num(degree_set_new);
	degree_exec=10-degree_set_new;
	PORTC=0xaf;*/
    for(degree_exec_cw=degree_set_new;degree_exec_cw<13;) // replace 12 for 120deg
    	{
        PORTC=0x33;
        motor_dir_cw();
        for(degree=0;degree<50;degree++)
            {cli();
            PORTG|=(1<<4);
            _delay_us(555);
            PORTG&=~(1<<4);
            _delay_us(5);
            
            }
            degree_exec_cw++;
            eeprom_write_byte(25,degree_exec_cw);
             
             //sei();
            _delay_ms(4);
           
            if (flag==1)
            {
            
            degree_exec_cw=eeprom_read_byte(25);
            lcd_gotoxy2(13);
            lcd_num(degree_exec_cw);
            _delay_ms(300);
            PORTC=0x01;
            }
            
        //_delay_ms(240000);
        lcd_gotoxy1(10);
        lcd_string("   ");
        lcd_gotoxy1(10);
        lcd_num(eeprom_read_byte(25));
        }
     /*   if(degree_exec_cw==13)
        {
        PORTC=0x77;
        _delay_ms(500);
        motor_dir_ccw();
        eeprom_write_byte(25,0);
        }*/
        
        
    
   
    for(degree_exec_ccw=13;degree_exec_ccw>1;)
    {   PORTC=0x77;
        //cli();
        motor_dir_ccw();
        for(degree=0;degree<50;degree++)
        {
            PORTG|=(1<<4);
            _delay_us(555);
            PORTG&=~(1<<4);
            _delay_us(5);
        }
        degree_exec_ccw--;
        eeprom_write_byte(25,degree_exec_ccw);
        lcd_gotoxy1(10);
        lcd_string("   ");
        lcd_gotoxy1(10);
        lcd_num(eeprom_read_byte(25));
        //sei();
    }


sei();
}

void motor_execute_test(void)
{
	
	for(degree=0;degree<200;degree++)
	{
	PORTG|=(1<<4);
	_delay_us(555);
	PORTG&=~(1<<4);
	_delay_us(5);
	}
	
}


void motor_dir_cw(void)
{
	//PORTG=0x00;
	_delay_ms(500);
	PORTG&=~(1<<0);
}
void motor_dir_ccw(void)
{
	//PORTG=0x00;
	_delay_ms(500);
	PORTG|=(1<<0);
}

void align_cw(void)
{
	PORTG=0x00;
	clockwise=1;
	counterwise=0;
	PORTG&=~(1<<0);
	PORTC=0x01;

	unsigned char degree;
	degree_set_new=eeprom_read_byte(25);
    if(degree_set_new<12)      // keeping 120 in mind
	{
		for(degree=0;degree<50;degree++)
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
	lcd_gotoxy2(5);
	lcd_string("    ");
	lcd_gotoxy2(0);
	lcd_string("CW=");
	lcd_num(eeprom_read_byte(25));

}

void align_ccw(void)
	{
		clockwise=0;
	counterwise=1;
	PORTG=0x00;
	PORTG|=(1<<0);

	unsigned char degree;
	PORTC=0x02;
	degree_set_new=eeprom_read_byte(25);
    if(degree_set_new>1)     
	{	

		for(degree=0;degree<50;degree++)
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
	lcd_gotoxy1(5);
	lcd_string("    ");
	lcd_gotoxy1(0);
	lcd_string("CCW=");
	lcd_num(degree_set_new);
	}


void ext_timer_init()
{
EICRB=0x00;
EIMSK|=(1<<4)|(1<<5)|(1<<6)|(1<<7);
}


void main()
{

DDRC=0xff;
DDRE&=~((1<<4)|(1<<5)|(1<<6)); // External Interrupt Enable
PORTE|=(1<<4)|(1<<5)|(1<<6);
lcd_init();
DDRC=0xff;
DDRG|=(1<<0)|(1<<4);
DDRD&=~(1<<6);
PORTD=(1<<6);
PORTG&=~(1<<0);
sei();
ext_timer_init();
unsigned char data;
motor_execute();

while(1)
{
/*
//
data=eeprom_read_byte(25);
lcd_gotoxy2(9);
lcd_num(eeprom_read_byte(25));
*/



}


}
