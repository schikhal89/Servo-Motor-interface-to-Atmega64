#include<avr/io.h>
#include<util/delay.h>
#define s1 (!(PIND&(1<<6)))
#define s2 (!(PIND&(1<<7)))     
#define s3 (!(PINA&(1<<5)))
#define s4 (!(PINE&(1<<7)))     //Pulse D5
#include<LCD_mega128.h>        //Dir:- D4=1 CCW   D4=0 CW
#include<avr/interrupt.h>     // G0=1 CCW   G0=0 CW
#include<avr/eeprom.h>
#include<uart.h>
#include<avr/pgmspace.h>

volatile unsigned char degree;
volatile unsigned char degree_set_new=1;
volatile unsigned char seconds=1;

unsigned char clockwise;
	unsigned char counterwise;
ISR(INT0_vect) //CCW
{  

	_delay_ms(500);
	align_ccw();

}

ISR(INT1_vect) // CW
{	_delay_ms(500);
	align_cw();
 }
ISR(INT2_vect) // Setting degree_exec=0
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
{//cli();
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
    for(;degree_set_new<6000;) //  6000 for 6000 degree motion....1
    	{
        PORTC=0x33;
        motor_dir_cw();
		
        for(degree=0;degree<5;degree++)  // Motor shall move for 1 degree in every 4.8 secs....2
            {
			cli();
            PORTD|=(1<<5);
            _delay_us(555);
            PORTD&=~(1<<5);
            _delay_us(5);
			
            }
            degree_set_new++;
            eeprom_write_byte(25,degree_set_new);
             
             sei();
            _delay_ms(4);
           
            
            
        _delay_ms(4800);       // Motor shall move for 1 degree in 4.8 secs
        lcd_gotoxy1(10);
        lcd_string("   ");
        lcd_gotoxy1(10);
        lcd_num(eeprom_read_byte(25));
        }
        
		/* Note:- The User expects 6000 degree motion, to get a 120 degree movement based on the gears installed
		on the motor. Hence in 1..the for loop is for 6000 degree motion, in 2...the for loop is for 1 degree movement
		per iteration which shall happen every 4.8 secs. Hence 6000*4.8secs=28800 secs which is 480 minutes or 8 Hours
		
		Summarising, the motor shall now move 6000 degrees in 8 hours with a speed of 1degree/4.8secs*/       
		
		
		
		
    PORTC=0x2f;
		_delay_ms(1000);

   
    for(degree_exec_ccw=600;degree_exec_ccw>1;)
    {   intr_disable();
		PORTC=0x77;
        cli();
        motor_dir_ccw();
        for(degree=0;degree<50;degree++)
        {
            PORTD|=(1<<5);
            _delay_us(555);
            PORTD&=~(1<<5);
            _delay_us(5);
        }
        degree_exec_ccw--;
        eeprom_write_byte(25,degree_exec_ccw);
        lcd_gotoxy1(10);
        lcd_string("   ");
        lcd_gotoxy1(10);
        lcd_num(eeprom_read_byte(25));
        
    }
sei();
ext_timer_init();
//sei();// use a variable for eeprom address and update it after every iteration to avoid break down of eeprom
}

void motor_execute_test(void)
{
	
	for(degree=0;degree<200;degree++)
	{
	PORTD|=(1<<5);
	_delay_us(555);
	PORTD&=~(1<<5);
	_delay_us(5);
	}
	
}


void motor_dir_cw(void)
{
	//PORTG=0x00;
	_delay_ms(500);
	PORTD&=~(1<<4);
}
void motor_dir_ccw(void)
{
	//PORTG=0x00;
	_delay_ms(500);
	PORTD|=(1<<4);
}

void align_cw(void)
{

	clockwise=1;
	counterwise=0;
	PORTD&=~(1<<4);
	PORTC=0x01;

	unsigned char degree;
	degree_set_new=eeprom_read_byte(25);
    if(degree_set_new<600)      // keeping 6000 in mind
	{
		for(degree=0;degree<50;degree++)
		{
		PORTC=0xff;
		_delay_us(555);
		PORTD|=(1<<5);
		_delay_us(5);
		PORTD&=~(1<<5);
		}
	degree_set_new++;
	eeprom_write_byte(25,degree_set_new);
	_delay_ms(300);
	}
	lcd_gotoxy2(5);
	lcd_string("    ");
	lcd_gotoxy2(0);
	lcd_string("CW=");
	lcd_num(eeprom_read_byte(25));

//main();

}

void align_ccw(void)
	{
		clockwise=0;
	counterwise=1;
	
	PORTD|=(1<<4);

	unsigned char degree;
	PORTC=0x02;
	degree_set_new=eeprom_read_byte(25);
    if(degree_set_new>1)     
	{	

		for(degree=0;degree<50;degree++)
		{
		PORTC=0xff;
		_delay_us(555);
		PORTD|=(1<<5);
		_delay_us(5);
		PORTD&=~(1<<5);
		}
	degree_set_new--;
	eeprom_write_byte(25,degree_set_new);
	_delay_ms(300);
	}
	lcd_gotoxy1(5);
	lcd_string("    ");
	lcd_gotoxy1(0);
	lcd_string("CCW=");
	lcd_num(degree_set_new);
//main();
	}


void ext_timer_init()
{
GICR|=(1<<INT0)|(1<<INT1);					// Enable INT0
MCUCR =0;
}
void intr_disable()
{
GICR=0x00;
}


void main()
{
//eeprom_write_byte(25,1)   // decide to set eeprom initial value as 1
DDRC=0xff;

DDRD&=~((1<<2)|(1<<3)); // External Interrupt Enable
DDRB&=~((1<<2)); // External Interrupt Enable
PORTD|=(1<<2)|(1<<3);
PORTB|=(1<<2);
lcd_init();
DDRC=0xff;
DDRD|=(1<<5)|(1<<4);
DDRD&=~((1<<6)|(1<<7));
PORTD|=(1<<6)|(1<<7);
sei();
int var=0;
ext_timer_init();
unsigned char data;
//motor_execute();

_delay_ms(300);    // Give 3 Sec
while(s3)
{_delay_ms(300);
while(1)
{
PORTB|=(1<<4);
	while(s1)//ccw
	{
	
	_delay_ms(300);
	uart_string("S1 Config");
	PORTD|=(1<<4);
		for(degree=0;degree<50;degree++)
		{
		_delay_us(555);
		PORTD|=(1<<5);
		_delay_us(5);
		PORTD&=~(1<<5);
		}
	var=0;
	}
	while(s2)//cw
	{
		
		_delay_ms(300);
		uart_string("S2 Config");
	PORTD&=~(1<<4);
		for(degree=0;degree<50;degree++)
		{
		_delay_us(555);
		PORTD|=(1<<5);
		_delay_us(5);
		PORTD&=~(1<<5);
		}
	
	var=0;
	}
	while(var<10)
	{
	_delay_ms(500);
	var++;

		uart_string("Waiting...");
		uart_num(var);
		if(s1||s2)
		break;
	}
	
	if(var==10)
	{
	//uart_string("var==10");
	eeprom_write_byte(25,1);
	
	break;
		}
	
	}

}
		//uart_string("Out main while");
	sei();
	PORTB&=~(1<<4);
	motor_execute();
while(1);
}



