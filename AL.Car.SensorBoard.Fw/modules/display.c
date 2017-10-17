#include "display.h"

float	_climate_humidity_0 = 0;
float	_climate_temperature_0 = 0;

void show_message_on_display(char *message)
{
	uart_send_string(1,"\r\r\r"); //Полная очистка
	_delay_ms(100);
	string_clear();
	string_add_string(message);
	string_add_string("\r");
	uart_send_string(1,string_buffer);
}

void show_all_data_on_display(int _duration)
{
	static int index=0;
	if (index==0)
	{
		string_clear(); //Передает битовый ноль, которые не кодирует символы
		string_add_string(" V   To Ti Hi PS");
		string_move_to_new();
		string_add_string(" ");
		string_add_string(" ");
		string_add_float(_climate_temperature_0,0);
		string_add_string(" ");
		string_add_float(_climate_humidity_0,0);
		string_add_string(" ");
		string_move_to_new();
 		string_add_string("\r"); //Символ означает, что строка закончилась
 		uart_send_string(1,string_buffer);
	}
	index++;
	if (index > _duration) index=0;
}