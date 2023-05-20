
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
	//------------���������� Nextion--------//
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


// ��� ������ ������ 0x02, ��� �������, (������ ��� �������) ��� ����� ������  0x03, 0x0D
// ��� ���� �������, ��� ������ ������� ��� ������� + ������ (������ 32����)
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
// 0x0� - ������� ����������
// 0x0F - ������� ���������
// 0x10 - ������ ����������
// 0x11 - ������ ���������
// 0x12 - ����1 ���������
// 0x13 - ����2 ���������
// 0x14 - ����3 ���������
// 0x15 - ����4 ���������
// 0x16 - zero1
// 0x17 - zero2
// 0x18 - zero3
// 0x19 - zero4
//------------���������� 2.0--------//
// 0x1A - ���� ���������
// 0�1� - ���� ��������
// 0�1� - ���������� ����� ���� (32���� �������)
// 0�1D - ��������� ����� ����
//------------���������� Nextion--------//
// 0�1E - ���������� ��� ������� ���(32���� �������)
// 0�1F - ��������� ��� ������� ���
// 0�20 - ��� ������ 0 ��������
// 0�21 - ��� ������ 0 ���������
// 0�22 - ��� ������ 1 ��������
// 0�23 - ��� ������ 1 ���������
// 0�24 - ��� ������ 2 ��������
// 0�25 - ��� ������ 2 ���������
// 0�26 - ��� ������ 3 ��������
// 0�27 - ��� ������ 3 ���������
// 0�28 - ��� ������ 4 ��������
// 0�29 - ��� ������ 4 ���������
// 0�2A - ��� ������ 5 ��������
// 0�2B - ��� ������ 5 ���������
// 0�2C - ��� ������ 6 ��������
// 0�2D - ��� ������ 6 ���������
// 0�2E - ��� ������ 7 ��������
// 0�2F - ��� ������ 7 ���������
// 0�30 - ��� ������ 8 ��������
// 0�31 - ��� ������ 8 ���������
// 0�32 - ��� ������ 9 ��������
// 0�33 - ��� ������ 9 ���������