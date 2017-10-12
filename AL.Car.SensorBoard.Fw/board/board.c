#include "board.h"

void var_delay_ms(int ms)
{
	for (int i=0; i<ms; i++)_delay_ms(1.0);
}

void adc_init_channel_0()
{
	adc_init(0, ADC_ADJUST_RIGHT, ADC_REFS_INTERNAL_1_1,  ADC_PRESCALER_32);
}

void button_1_enable()
{
	setbit(DDRC,6,0);
	setbit(PORTC,6,1);
}

byte button_1_is_pressed()
{
	if (getbit(PINC,6)) {return 1;}
	return 0;
}

void button_2_enable()
{
	setbit(DDRC,7,0);
	setbit(PORTC,7,1);
}

byte button_2_is_pressed()
{
	if (getbit(PINC,7)) {return 1;}
	return 0;
}

void pb3_in_enable()
{
	setbit(DDRB,3,0);
	setbit(PORTB,3,1);	
}

byte get_pb3_in()
{
	if (getbit(PINB,3)) {return 0;}
	return 1;
}

void set_board_green_led (byte state)
{
	setbit(DDRB,2,1);
	setbit(PORTB,2,state);	
}