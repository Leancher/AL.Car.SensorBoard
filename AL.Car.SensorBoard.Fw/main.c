#include <avr/io.h>
#include <avr/interrupt.h>
#include "board/board.h"
#include "modules/display.h"

#define INJ_AMOUNT 3
//130 производительность в мл/мин, делим на 60, получаем в мл/сек
//умножаем на кол-во форсунок
#define INJ_CAPACITY 130 / 60 * INJ_AMOUNT

#define SPEED_CORRECTION 12
#define LENGTH_WHEEL 0.025*13*2*3.14//2 m
//0.025м в дюйме
//13 радиус колеса в дюймах
//2*3.14 - пи*2*радиус - длина окружности (колеса)

float inj_width_high=0;
float inj_width_low=0;
float inj_sum_high=0;
float inj_sum_1s=0;
float fuel_rate_ml_s=0;
float fuel_rate_l_h=0;
float fuel_rate_l_100km=0;

float spd_width_high=0;
float spd_width_low=0;
float spd_sum_high=0;
float spd_sum_1s=0;
float speed=0;
int spd_count_imp_1s=0;

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

void measured_inj_imp_1s()
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
	//Расход в мл/с, время открытия в мс, делим на 1000, получаем в сек
	fuel_rate_ml_s=inj_sum_high/1000*INJ_CAPACITY;
	//Если скорость меньше 1 (холостой ход), считаем л/ч
	fuel_rate_l_h=fuel_rate_ml_s*3.6;
	//Расход в л/100км
	//Делим путь на скорость, получаем время в часах, умножаем на 3600, получаем в секундах 
	//Умножаем на расход в мл/с, делим на 1000 для результата в литрах
	//3600 делим сразу на 1000 получаем 3.6
	fuel_rate_l_100km = 100/speed*fuel_rate_ml_s*3.6;	
}

//Измерение длительности импулься скорости
void measure_speed_impulse()
{
	TCNT1=0;
	//ждем пока пройдет 1
	while (get_pb4_in()==1)
	{
		//Если долго считает, то нет импульса
		//if (TCNT1>50000) return;
	}
	spd_width_high=TCNT1*0.008;
	TCNT1=0;
	//ждем пока пройдет 0
	while (get_pb4_in()==0)
	{
		if (TCNT1>20000) return;
	}
	spd_width_low=TCNT1*0.008;
}

void measured_spd_imp_1s()
{
	int no_impulse_counter=0;
	spd_width_high = 0;
	spd_width_low = 0;
	spd_sum_high=0;
	spd_sum_1s=0;
	spd_count_imp_1s=0;
	while (spd_sum_1s < 1000)
	{
		measure_speed_impulse();
		spd_sum_1s = spd_sum_1s + spd_width_high + spd_width_low;
		spd_count_imp_1s++;
		//Если долго нет импульса, то выходим
		if (spd_width_high==0) no_impulse_counter++;
		if (no_impulse_counter>10)
		{
			spd_count_imp_1s=0;
			return;
		}
	}
	//spd_count_imp_1s=spd_count_imp_1s/SPEED_CORRECTION;
	if (spd_count_imp_1s<1)
	{
		speed=1;
		return;
	}
	speed= spd_count_imp_1s * LENGTH_WHEEL *3.6;//3.6 км/ч в 1 м/с
}

void show_data_on_display()
{
	string_clear();
	string_add_float(inj_width_high,1);
	string_add_string(";");
	string_add_float(inj_sum_high,2);
	string_add_string(";");
	string_add_int(spd_count_imp_1s);
	string_add_string("kmh ");
	string_move_to_new();
	if (speed>1)
	{
		string_add_float(fuel_rate_l_100km,1);
		string_add_string(" l/100km");		
	}
	else
	{
		string_add_float(fuel_rate_l_h,2);
		string_add_string(" l/h");		
	}
	string_add_string("\r");
	uart_send_string(1,string_buffer);
	_delay_ms(200);	
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
		measured_spd_imp_1s();
		measured_inj_imp_1s();	
		show_data_on_display();
	}
}