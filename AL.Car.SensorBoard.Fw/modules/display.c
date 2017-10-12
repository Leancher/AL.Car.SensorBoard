#include "display.h"

float	_climate_humidity_0 = 0;
float	_climate_temperature_0 = 0;

void show_data_on_display(char *data)
{
	//uart_send_string(1,"\r\r\r"); //������ �������
	 
	string_clear();
	string_add_string(data);
	string_add_string("\r"); //������ ��������, ��� ������ �����������
	uart_send_string(1,string_buffer);
	_delay_ms(1000);
}

void show_all_data_on_display(int _duration)
{
	static int index=0;
	if (index==0)
	{
		string_clear(); //�������� ������� ����, ������� �� �������� �������
		string_add_string(" V   To Ti Hi PS");
		string_move_to_new();
		string_add_string(" ");
		string_add_string(" ");
		string_add_float(_climate_temperature_0,0);
		string_add_string(" ");
		string_add_float(_climate_humidity_0,0);
		string_add_string(" ");
		string_move_to_new();
 		string_add_string("\r"); //������ ��������, ��� ������ �����������
 		uart_send_string(1,string_buffer);
	}
	index++;
	if (index > _duration) index=0;
}