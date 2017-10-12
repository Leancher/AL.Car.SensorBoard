/*
 * AL.Car.SensorBoard.Fw
 *
 * Created: 28.07.2016 8:44:04
 * Author : Andrew
 */ 

#include <avr/io.h>
#include <avr/interrupt.h>
#include "board/board.h"
#include "modules/display.h"

float width_impulse=0;

byte get_in()
{
	return (getbit(PIND,6));
}

void count_width_impulse_1()
{
	while (get_in()!=0) {}//ждем 0
	while (get_in()==0) {}//ждем 1
	TCNT1=0;
	while (get_in()!=0) {}//ждем 0
	width_impulse=TCNT1*0.008;

	string_clear();
	string_add_float(width_impulse,4);
	string_add_string("\r");
	uart_send_string(1,string_buffer);
	_delay_ms(1000);
}

int main(void)
{
	uart_init_withdivider(1,UBRR_VALUE);
	_delay_ms(1000);
	show_data_on_display("\r\r\r");
	show_data_on_display("KOTMEGA");
	//предделитель на 64, 125к√ц, 0.008млс
	TCCR1B |= (0 << CS12)|(1 << CS11)|(1 << CS10);
	while (1)
	{
		count_width_impulse_1();
	}
}


// void count_width_impulse_2()
// {
// 	level=get_pb3_in();
// 	if (level==0) is_low_level=1;
// 	if (is_low_level==0) return;
// 	
// 	if (level==1)
// 	{
// 		width_impulse++;
// 		if (wait_next_period==0)
// 		{
// 			period++;
// 			wait_next_period=1;
// 		}
// 	}
// 	if (level==0) wait_next_period=0;
// 	if (period==5)
// 	{
// 		width_impulse=width_impulse;
// 		width_impulse=width_impulse/5;
// 		width_impulse=width_impulse;
// 		string_clear();
// 		string_add_int(width_impulse);
// 		string_add_string("\r"); //—имвол означает, что строка закончилась
// 		uart_send_string(1,string_buffer);
// 		width_impulse=0;
// 		_delay_ms(1000);
// 		is_low_level=0;
// 		period=0;
// 	}
// 	_delay_us(73.5);
// }