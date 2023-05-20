
#ifndef UART_H_
#define UART_H_

#include <avr/io.h>
#include <avr/interrupt.h>
#define B19200  51

void init_UART(void);
bool uart0_send_string ( char *send, uint8_t len);
uint8_t parsing_dat (void);
uint8_t calcCRC(char *buf, uint8_t len);
double transmit_32_double (uint32_t dat);
uint32_t transmit_double_32 (double dat);
uint32_t collect_massiv (char *massiv);
void clear_buf (void);
void copy_clear (void);

enum
{
	ADC1_OUT = 0x01,
	ADC2_OUT,
	ADC3_OUT,
	ADC4_OUT,
	ZERO_ALL,
	NEW_K_K1,
	NEW_K_K2,
	NEW_K_K3,
	NEW_K_K4,
	NEW_MASSA1,
	NEW_MASSA2,
	NEW_MASSA3,
	NEW_MASSA4,
	NEW_DISKRT,
	SEND_DISKRT,
	NEW_FILTR,
	SEND_FILTR,
	SEND_K_K1,
	SEND_K_K2,
	SEND_K_K3,
	SEND_K_K4,
	ZERO_1,
	ZERO_2,
	ZERO_3,
	ZERO_4,
	SEND_TARA,
	NEW_TARA,
	NEW_NUMBER_KK,
	SEND_NUMBER_KK,
	//------------обновление Nextion--------//
	NEW_ZERO_AX,
	SEND_ZERO_AX,
	NEW_CELL0,
	SEND_CELL0,
	NEW_CELL1,
	SEND_CELL1,
	NEW_CELL2,
	SEND_CELL2,
	NEW_CELL3,
	SEND_CELL3,
	NEW_CELL4,
	SEND_CELL4,
	NEW_CELL5,
	SEND_CELL5,
	NEW_CELL6,
	SEND_CELL6,
	NEW_CELL7,
	SEND_CELL7,
	NEW_CELL8,
	SEND_CELL8,
	NEW_CELL9,
	SEND_CELL9,
};

#endif /* UART_H_ */


// код начала фрейма 0x02, код команды, (данные или пустота) код конца фрейма  0x03, 0x0D
// два типа посылки, или просто команда или команда + данные (всегда 32бита)
// 0x01 - adc1
// 0x02 - adc2
// 0x03 - adc3
// 0x04 - adc4
// 0x05 - zero
// 0x06 - k.k1
// 0x07 - k.k2
// 0x08 - k.k3
// 0x09 - k.k4
// 0x0A - massak1
// 0x0B - massak2
// 0x0C - massak3
// 0x0D - massak4
// 0x0≈ - дискрет установить
// 0x0F - дискрет отправить
// 0x10 - фильтр установить
// 0x11 - фильтр отправить
// 0x12 - коэф1 отправить
// 0x13 - коэф2 отправить
// 0x14 - коэф3 отправить
// 0x15 - коэф4 отправить
// 0x16 - zero1
// 0x17 - zero2
// 0x18 - zero3
// 0x19 - zero4
//------------обновление 2.0--------//
// 0x1A - тара отправить
// 0х1¬ - тара записать
// 0х1— - установить номер коэф (32бита посылка)
// 0х1D - отправить номер коэф
//------------обновление Nextion--------//
// 0х1E - установить вес нулевой оси(32бита посылка)
// 0х1F - отправить вес нулевой оси
// 0х20 - доп €чейка 0 записать
// 0х21 - доп €чейка 0 отправить
// 0х22 - доп €чейка 1 записать
// 0х23 - доп €чейка 1 отправить
// 0х24 - доп €чейка 2 записать
// 0х25 - доп €чейка 2 отправить
// 0х26 - доп €чейка 3 записать
// 0х27 - доп €чейка 3 отправить
// 0х28 - доп €чейка 4 записать
// 0х29 - доп €чейка 4 отправить
// 0х2A - доп €чейка 5 записать
// 0х2B - доп €чейка 5 отправить
// 0х2C - доп €чейка 6 записать
// 0х2D - доп €чейка 6 отправить
// 0х2E - доп €чейка 7 записать
// 0х2F - доп €чейка 7 отправить
// 0х30 - доп €чейка 8 записать
// 0х31 - доп €чейка 8 отправить
// 0х32 - доп €чейка 9 записать
// 0х33 - доп €чейка 9 отправить