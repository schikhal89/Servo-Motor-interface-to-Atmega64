#include<avr/io.h>
#include<util/delay.h>
#include<LCD_mega128.h>
  void main()
  {
  int degree,i,count;
  DDRG|=(1<<4)|(1<<0)|(1<<2);
  PORTG|=(1<<0);
  lcd_init();
  lcd_string("dfd");
  
  //PORTG&=~(1<<2);
  //while(1)
	{
	PORTC=0xa0;
  for(i = 0; i < 180; i++) // this is to give 1800 steps at 1rpm
{
_delay_us(330);
PORTG|=(1<<4);
   
_delay_us(5);
PORTG&=~(1<<4);

}
  //degree=0;
  }
	}
	
 