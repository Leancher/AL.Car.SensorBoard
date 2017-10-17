#include <avr/io.h>
#include <avr/interrupt.h>
#include "board/board.h"
#include "modules/display.h"

float width_injector_impulse=0;
float width_speed_impulse=0;
int count_speed_impulse=0;
float fuel_rate=0;

//��������� ������������ �������� ��������
void measure_injector_impulse()
{
	while (get_pb5_in()==1) {}//���� 0, �� ������, ���� ������� �� �������
	while (get_pb5_in()==0) {}//���� 1, ���� ���� ������� 0
	TCNT1=0;
	while (get_pb5_in()==1) {}//���� 0
	width_injector_impulse=TCNT1*0.008;
}

//��������� ������������ �������� ��������
void measure_speed_impulse()
{
	while (get_pb4_in()==1) {}//���� 0, �� ������, ���� ������� �� �������
	TCNT1=0;
	while (get_pb4_in()==0) {}//���� 1, ���� ���� ������� 0
	while (get_pb4_in()==1) {}//���� 0
	width_speed_impulse = TCNT1*0.008;	
}

void measured_fuel_rate()
{
	fuel_rate=width_injector_impulse/1000*130/60;
	//����� �������� � ��, ����� �� 1000, �������� � ���
	//130 ������������������ � ��/���, ����� �� 60, �������� � ��/���
}

void show_data_on_display()
{
	string_clear();
	string_add_float(width_injector_impulse,1);
	string_add_string(";");
	string_add_float(fuel_rate,3);
	string_add_string("ml/s");
	string_move_to_new();
	string_add_float(width_speed_impulse,1);
	string_add_string(" ms");
	string_add_string("\r");
	uart_send_string(1,string_buffer);
	_delay_ms(1000);	
}

int main(void)
{
	uart_init_withdivider(1,UBRR_VALUE);
	_delay_ms(500);
	show_message_on_display("KOTMEGA");
	_delay_ms(1000);
	show_data_on_display();
	//pb4_in_enable();
	pb5_in_enable();

	//������������ �� 64, 125���, 0.008���
	TCCR1B &= ~(1 << CS12);
	TCCR1B |=  (1 << CS11);
	TCCR1B |=  (1 << CS10);
	while (1)
	{
		measure_injector_impulse();
		measured_fuel_rate();
		//measure_speed_impulse();
		show_data_on_display();
	}
}