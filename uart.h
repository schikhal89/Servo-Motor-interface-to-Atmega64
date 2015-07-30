

void uart_init()
{
UBRRL=103;
UCSRA=(1<<3)|(1<<4);
}

void uart_tx(unsigned char data)
{
while(!(UCSRB&(1<<UDRE)));
UDR=data;
}

unsigned char uart_rx(void)
{
while(!(UCSRB&(1<<RXC)));
return UDR;
}


uart_string(unsigned char *data)
{
while((*data) != '\0')	/* print char from str pointer and point to next location */
				uart_tx((*data++));


}
void uart_num(int num)
{
unsigned char data[10];
sprintf(data,"%d",num);
uart_string(data);
}

  