#include "Uart.h"

//--------------���������� �� ����� �����-----------------//
volatile uint8_t tim_uart0, uart0_rx_caunter, uart0_tx_caunter,leng;
bool rx0_done;
uint32_t in_data;
double in_float;

char uart0_rx_buf [20];
char uart0_tx_buf [20];                       // ������ �� ��������


void init_UART(void)                                  //������������� ������ USART
{
	UBRR0 = 103;//B19200;
	UCSR0B = (1<<RXEN0)|( 1<<TXEN0);                 //�������� ����� � �������� �� USART
	UCSR0B |= (1<<RXCIE0);                          //��������� ���������� ��� ������
	UCSR0C = (1<<UCSZ01)|(1<<UPM01)|(1<<UCSZ00);               //������� 1, 8 ���  ???
}

ISR (TIMER0_OVF_vect)
{
	if (tim_uart0 < 250) {tim_uart0 ++;}
	if (tim_uart0 == 249) { uart0_rx_caunter = 0; rx0_done = true; }
}

//----------������� ������������� �������� ������----------//
bool uart0_send_string ( char *send, uint8_t len)                // ���� �������� ������ � ����� ������� 0x01
{
	if (uart0_tx_caunter == 0)                     // ���� ������� �� ���� ����� �������� �����, ���� ��� �� ����������
	{
		for (int i = 0; i < len; i ++)             // �������� ������ �������� ��������� �� ���������� ������
		{
			uart0_tx_buf[i] = send[i];
			// if (uart0_tx_buf[i] == 0x01) break;   // ���� �� �������� ������ 0x01. �� ��� �� ���� �������
		}
		UDR0 = uart0_tx_buf[uart0_tx_caunter];    // ���������� ������ ����
		uart0_tx_caunter ++;                      // ����������� �������
		UCSR0B |= ( 1<<UDRIE0);                   // �������� ���������� �� ������������ ������� ��������
		leng = len;
		return true;
		} else{
		return false;
	}
}

//----------����������� ���������� �� �������� ����������----------//
ISR (USART_UDRE_vect)                                      // ���������� �� ����������� ������� ��������
{
	if(uart0_tx_caunter < leng)
	//if(uart0_tx_buf[uart0_tx_caunter] != 0x01)              // ���� ������ � ������� �� ����� 0x01
	{
		UDR0 = uart0_tx_buf[uart0_tx_caunter];            // ���������� ���������� �� ������� ����� � ����
		uart0_tx_caunter ++;                              // �� � ����������� ������������� �� �������
	}
	else                                                   // ���� ����� �� ������� 0x01
	{
		uart0_tx_caunter = 0;                             // ���������� �������
		UCSR0B  &= ~ ( 1<<UDRIE0);                        // �������� ���������� �� ����������� ������
	}
}

ISR (USART_RX_vect)
{
	tim_uart0 = 0;                                                        // ������� ������, �� 150 (��� ����� �������� ��� ���� ������ ��� ������ ����� ������ �����)
	uart0_rx_buf[uart0_rx_caunter] = UDR0;                                // ��������� �������� ����
	uart0_rx_caunter ++; if(uart0_rx_caunter > 18) uart0_rx_caunter = 0;  // ���� ������� ������� 0 �� ����������, ���� ������� ��������� �� 8 �� ������� � ����
}

uint8_t parsing_dat (void)       //������� �������� ������
{
	static uint8_t out;
	if (rx0_done){              // ���� ������ ���� �������� ������
		rx0_done = false;      // ������� ���
		if(uart0_rx_buf[0] == 0x02 && uart0_rx_buf[6] == 0x03){  // ��������� ������ � ��������� ������ ���� ������ 32 ���� ��� 8
			switch (uart0_rx_buf[1]){     // ����� �� ������� ������� ������� ��� �� �� ������� ��� ������
				case NEW_K_K1:
				in_float = transmit_32_double (collect_massiv(uart0_rx_buf));  // ���� ��� ����� �� ������� ���������� � ������ ������ � ����� �������������� � �����
				clear_buf(); // ������� ������
				return NEW_K_K1;
				break;
				case NEW_K_K2:
				in_float = transmit_32_double (collect_massiv(uart0_rx_buf));
				clear_buf();
				return NEW_K_K2;
				break;
				case NEW_K_K3:
				in_float = transmit_32_double (collect_massiv(uart0_rx_buf));
				clear_buf();
				return NEW_K_K3;
				break;
				case NEW_K_K4:
				in_float = transmit_32_double (collect_massiv(uart0_rx_buf));
				clear_buf();
				return NEW_K_K4;
				break;
				case NEW_MASSA1:
				copy_clear();
				return NEW_MASSA1;
				break;
				case NEW_MASSA2:
				copy_clear();
				return NEW_MASSA2;
				break;
				case NEW_MASSA3:
				copy_clear();
				return NEW_MASSA3;
				break;
				case NEW_MASSA4:
				copy_clear();
				return NEW_MASSA4;
				break;
				case NEW_DISKRT:
				copy_clear();
				return NEW_DISKRT;
				break;
				case NEW_FILTR:
				copy_clear();
				return NEW_FILTR;
				break;
				case NEW_TARA:
				copy_clear();
				return NEW_TARA;
				break;		
				case NEW_NUMBER_KK:
				copy_clear();
				return NEW_NUMBER_KK;
				break;
//------------���������� Nextion--------//						
				case NEW_ZERO_AX:
				copy_clear();
				return NEW_ZERO_AX;
				break;
				case NEW_CELL0:
				copy_clear();
				return NEW_CELL0;
				break;
				case NEW_CELL1:
				copy_clear();
				return NEW_CELL1;
				break;
				case NEW_CELL2:
				copy_clear();
				return NEW_CELL2;
				break;
				case NEW_CELL3:
				copy_clear();
				return NEW_CELL3;
				break;
				case NEW_CELL4:
				copy_clear();
				return NEW_CELL4;
				break;
				case NEW_CELL5:
				copy_clear();
				return NEW_CELL5;
				break;
				case NEW_CELL6:
				copy_clear();
				return NEW_CELL6;
				break;
				case NEW_CELL7:
				copy_clear();
				return NEW_CELL7;
				break;
				case NEW_CELL8:
				copy_clear();
				return NEW_CELL8;
				break;
				case NEW_CELL9:
				copy_clear();
				return NEW_CELL9;
				break;
			}
		}
		
		if(uart0_rx_buf[0] == 0x02  && uart0_rx_buf[2] == 0x03 && uart0_rx_buf[6] == 0x00)     // ���� � ��� �������� ���������
		{
			out = uart0_rx_buf[1];
			clear_buf(); // �������� �����
			return out;
		}
		clear_buf(); // �������� �����
		return 0;   // ������� ����
	}
	return 0;
}

void copy_clear (void) {                    // ���� 100 ��� �� ������ ���� � ����
	in_data = collect_massiv(uart0_rx_buf); // ���� ��� ���, �������������� ��� � 32������ ����������
	clear_buf();
}

void clear_buf (void){  // ����� ������� ������� �������
	for (uint8_t i = 0; i < 18; i ++){
		uart0_rx_buf [i] = 0x00;
	}
}

uint32_t collect_massiv (char *massiv){       // ������� 32 ������ ����� �� �������� ������
	uint32_t dat;
	dat = massiv [5];
	dat = dat << 8;
	dat |= massiv [4];
	dat = dat << 8;
	dat |= massiv [3];
	dat = dat << 8;
	dat |= massiv [2];
	return dat;
}

double transmit_32_double (uint32_t dat){   // ������������� 32 ������ ����� � ����
	static union                          // ��� �� ����, ������� �����������, � ���������� � ����� � ��� �� ������ �� �������
	{
		double in_float;
		uint32_t out_float;
	}floatchik;
	floatchik.out_float = dat;
	return floatchik.in_float;                                         // ������� ���� ���� �����
}

uint32_t transmit_double_32 (double dat){   // ������������� ���� � 32 ������ �����
	static union                           // ��� �� ����, ������� �����������, � ���������� � ����� � ��� �� ������ �� �������
	{
		double in_float;
		uint32_t out_float;
	}floatchik;
	floatchik.in_float = dat;
	return floatchik.out_float;                                 // ������� ���� ���� 32bit
}

uint8_t calcCRC(char *buf, uint8_t len) {                     // ������ ���, ���� �� ����
	uint8_t r = 0;
	for (uint32_t i = 0; i < len; i++) {
		r -= buf[i];
	}
	r &= 0x7F;
	if (r < 0x21) {
		r += 0x21;
	}
	return r;
}