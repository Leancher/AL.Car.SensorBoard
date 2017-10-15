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

float width_injector_impulse=0;
float width_speed_impulse=0;
int count_speed_impulse=0;

void measure_width_impulse()
{
	//предделитель на 64, 125к√ц, 0.008млс
	TCCR1B &= ~(1 << CS12);
	TCCR1B |=  (1 << CS11);
	TCCR1B |=  (1 << CS10);

	//»змерение длительности открыти€ форсунки
	while (get_pb5_in()==1) {}//ждем 0, на случай, если импульс не сначала
	while (get_pb5_in()==0) {}//ждем 1, ждем пока пройдет 0
	TCNT1=0;
	while (get_pb5_in()==1) {}//ждем 0
	width_injector_impulse=TCNT1*0.008;

	width_speed_impulse=0;

	while (get_pb4_in()==1) {}//ждем 0, на случай, если импульс не сначала
	TCNT1=0;
	while (get_pb4_in()==0) {}//ждем 1, ждем пока пройдет 0
	while (get_pb4_in()==1) {}//ждем 0
	width_speed_impulse = TCNT1*0.008;
}

void show_data_on_display()
{
	string_clear();
	string_add_float(width_injector_impulse,1);
	string_move_to_new();
	string_add_int(count_speed_impulse);
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
	//pb4_in_enable();
	pb5_in_enable();
	//предделитель на 64, 125к√ц, 0.008млс
	TCCR1B |= (0 << CS12)|(1 << CS11)|(1 << CS10);
	while (1)
	{
		measure_width_impulse();
		show_data_on_display();
	}
}