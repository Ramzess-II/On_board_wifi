

#ifndef MAIN_H_
#define MAIN_H_

#define F_CPU 16000000UL
#include <avr/io.h>
#include <util/delay.h>
#include <avr/interrupt.h>
#include <stdio.h>
#include <stdlib.h>
#include <avr/pgmspace.h>
#include "Uart.h"
#include "ADS1115.h"
#include "EEprom.h"
#include <avr/wdt.h>



//------------------------------------------------------------------------------
// дефайним порты
//------------------------------------------------------------------------------
#define INIT_PIN      ~ PINB & 0x01     // проверка на 0
#define UART_PIN        PINB & 0x10     // проверка на 1
#define LED_ON          PORTB |= 0x20
#define LED_OFF         PORTB &= ~0x20


//------------------------------------------------------------------------------
// инициализируем переменные
//------------------------------------------------------------------------------
struct bool_date {
     uint32_t go:1;
     uint32_t count_adc:3;
     // uint32_t rx0_done:1;
};

struct analog_data {          // 
     int32_t massa1;
     int32_t massa2;
     int32_t massa3;
     int32_t massa4;
     int32_t adc_kalib1;
     int32_t adc_kalib2;
     int32_t adc_kalib3;
     int32_t adc_kalib4;
     uint16_t adc1_clear;
     uint16_t adc2_clear;
     uint16_t adc3_clear;
     uint16_t adc4_clear;
     uint16_t zero_adc1;
     uint16_t zero_adc2;
     uint16_t zero_adc3;
     uint16_t zero_adc4;
};

struct setting_s {
     uint8_t amount_adc;
     uint8_t filtr;
     uint8_t disreta;
     int32_t number_kk_4;
     int32_t tara;
	 int32_t zero_ax;
     double kal_koef1;
     double kal_koef2;
     double kal_koef3;
     double kal_koef4;
     double kal_koef4_1;
     double kal_koef4_2;
     double kal_koef4_3;
     double kal_koef4_4;
     double kal_koef4_5;
};

struct cell_s {
	int32_t cell0;
	int32_t cell1;
	int32_t cell2;
	int32_t cell3;
	int32_t cell4;
	int32_t cell5;
	int32_t cell6;
	int32_t cell7;
	int32_t cell8;
	int32_t cell9;
	};
//------------------------------------------------------------------------------
// создаем прототипы функций
//------------------------------------------------------------------------------
void init_atmega (void) ;
void read_ALL_EEprom (void);
void uart_data_comand (uint8_t code);
void massa_send_uart (uint8_t number, int32_t massa );
void adc_to_massa (uint8_t number, uint8_t filtr, int16_t adc);
int32_t diskret (int32_t data, uint8_t dskrt);
void send_8bit (uint8_t comand, uint8_t data);
void send_32bit (uint8_t command, uint32_t data);


#endif /* MAIN_H_ */