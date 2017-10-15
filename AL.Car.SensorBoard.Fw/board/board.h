#ifndef BOARD_H
#define BOARD_H

#define F_CPU 8000000UL
#define BAUD 9600

#define UART_DISPLAY 1
#define ADC_VOLT_MULTIPLIER_MV		(68+2.2)/2.2 * 1.1

#include <avr/io.h>
#include <util/delay.h>
#include <avr/wdt.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <util/setbaud.h>
#include "../refs/bwl_adc.h"

#define getbit(port, bit)		((port) &   (1 << (bit)))
#define setbit(port,bit,val)	{if ((val)) {(port)|= (1 << (bit));} else {(port) &= ~(1 << (bit));}}

typedef unsigned char byte;
void var_delay_ms(int ms);

void adc_init_channel_0();
void button_1_enable();
byte button_1_is_pressed();
void button_2_enable();
byte button_2_is_pressed();

void pb3_in_enable();
byte get_pb3_in();
void pb4_in_enable();
byte get_pb4_in();
void pb5_in_enable();
byte get_pb5_in();
void set_board_green_led (byte state);

#endif /* BOARD_H_ */