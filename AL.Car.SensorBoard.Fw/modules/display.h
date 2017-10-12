#ifndef DISPALY_H_
#define DISPALY_H_

#include "..\board\board.h"
#include "..\refs\\bwl_uart.h"
#include "..\refs\bwl_strings.h"

void show_data_on_display(char *data);
void show_all_data_on_display(int _duration);
#endif /* DISPALY_H_ */