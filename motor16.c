#include<avr/io.h>
#include<util/delay.h>
#define s1 (!(PIND&(1<<6)))
#define s2 (!(PIND&(1<<7)))
#define s3 (!(PINE&(1<<6)))
#define s4 (!(PINE&(1<<7)))
#include<LCD_mega16.h>
#include<avr/interrupt.h>
#include<avr/eeprom.h>
#include<uart.h>

volatile unsigned char degree;	
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
PORTD^=(1<<1);
PORTB^=(1<<6);
PORTB^=(1<<4);
seconds++;
degree_set_new=eeprom_read_byte(25);
lcd_gotoxy2(0);
lcd_num(degree_set_new);
/*degree_set_new=120-read_val;
deg_cw=degree_set_new*5;
deg_ccw=degree_set_new*10;*/



/*if(seconds==1)     // Put seconds=240 for 4 minute delay.
{
//PORTC^=0xff;
for(degree=0;degree<5;degree++)
{
PORTC=0xff;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
seconds=0;
clk++;
ccw--;
if(clk==deg_cw)
{
PORTA=0x00;
PORTC=0xaa;
PORTG=0x00;
PORTG|=(1<<0);
PORTG&=~(1<<2);
degree=0;
clk=0;
}
if(ccw==0)
{
PORTA=0xff;
PORTG=0x00;
PORTG|=(1<<2);
PORTG&=~(1<<0);
degree=0;
ccw=deg_ccw;
}



}


}
*/
}

void align_ccw(void)
{
unsigned char degree_ccw;
PORTG=0x00;

PORTD&=~(1<<4);

PORTB|=(1<<6);
PORTB&=~(1<<4);


/*for(degree_ccw=0;degree_ccw<50;degree_ccw++)
{
PORTC=0xff;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
}*/

}

void align_cw(void)
{
unsigned char degree_cw;
PORTG=0x00;
PORTD|=(1<<4);

PORTB|=(1<<4);
PORTB&=~(1<<6);
/*for(degree_cw=0;degree_cw<50;degree_cw++)
{
PORTC=0xff;
_delay_us(555);
PORTG|=(1<<4);
_delay_us(5);
PORTG&=~(1<<4);
}*/

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
DDRD|=(1<<1)|(1<<6)|(1<<4);
DDRB|=(1<<6)|(1<<4);
PORTD|=(1<<4);
//PORTG&=~(1<<0);
DDRC=0xff;
//TCCR1A=0x00;
uart_init();
lcd_init();
//lcd_char('a');
DDRD&=~((1<<6)|(1<<7));
DDRE&=~((1<<6)|(1<<7));
PORTD|=(1<<6)|(1<<7);
PORTE|=(1<<6)|(1<<7);
eeprom_write_byte(24,46);

while(1)
{
if(s1 && ((read_val=eeprom_read_byte(25))!=0))
{
_delay_ms(300);
align_ccw();
degree_set++;
eeprom_write_byte(25,degree_set);
test=eeprom_read_byte(25);
//uart_str("Degree\t");
lcd_num(test);
lcd_cmd(0x02);

}
if(s2 && (((read_val=eeprom_read_byte(25))!=10)))
{
_delay_ms(300);
align_cw();
if(degree_set!=1)
degree_set--;

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


