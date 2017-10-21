#include <avr/io.h>
#include <avr/interrupt.h>
#include "board/board.h"
#include "modules/display.h"

float inj_width_high=0;
float inj_width_low=0;
float inj_sum_high=0;
float inj_sum_1s=0;
float width_speed_impulse=0;
int count_speed_impulse=0;
float fuel_rate_in_1s=0;

//Измерение длительности открытия форсунки
void measure_injector_impulse()
{
	TCNT1=0;
	//ждем пока пройдет 1
	while (get_pb5_in()==1) 
	{
		//Если долго считает, то нет импульса
		//if (TCNT1>50000) return;
	}
	inj_width_high=TCNT1*0.008;
	TCNT1=0;
	 //ждем пока пройдет 0
	while (get_pb5_in()==0)
	{
		if (TCNT1>20000) return;
	}
	inj_width_low=TCNT1*0.008;
}

void measured_inj_imp_in1s()
{
	int no_impulse_counter=0;
	inj_width_high = 0;
	inj_width_low = 0;
	inj_sum_high=0;
	inj_sum_1s=0;
	while (inj_sum_1s < 1000)
	{
		measure_injector_impulse();
		//Если долго нет импульса, то выходим
		if (inj_width_high==0) no_impulse_counter++;
		if (no_impulse_counter>20) return;
		inj_sum_1s = inj_sum_1s + inj_width_high + inj_width_low;
		inj_sum_high = inj_sum_high + inj_width_high;
	}
	fuel_rate_in_1s=inj_sum_high/1000*130/60;
	//Время открытия в мс, делим на 1000, получаем в сек
	//130 производительность в мл/мин, делим на 60, получаем в мл/сек
}

//Измерение длительности импулься скорости
void measure_speed_impulse()
{
	while (get_pb4_in()==1) {}//ждем 0, на случай, если импульс не сначала
	TCNT1=0;
	while (get_pb4_in()==0) {}//ждем 1, ждем пока пройдет 0
	while (get_pb4_in()==1) {}//ждем 0
	width_speed_impulse = TCNT1*0.008;	
}

void show_data_on_display()
{
	string_clear();
	string_add_float(inj_width_high,1);
	string_add_string(";");
	string_add_float(inj_sum_high,1);
	string_add_string(";");
	string_add_float(fuel_rate_in_1s,1);
	string_move_to_new();

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

	//предделитель на 64, 125кГц, 0.008млс
	TCCR1B &= ~(1 << CS12);
	TCCR1B |=  (1 << CS11);
	TCCR1B |=  (1 << CS10);
	while (1)
	{
		//measure_injector_impulse();
		measured_inj_imp_in1s();
		//measure_speed_impulse();
		show_data_on_display();
	}
}