#include "Uart.h"

//--------------переменные из этого файла-----------------//
volatile uint8_t tim_uart0, uart0_rx_caunter, uart0_tx_caunter,leng;
bool rx0_done;
uint32_t in_data;
double in_float;

char uart0_rx_buf [20];
char uart0_tx_buf [20];                       // буффер на передачу


void init_UART(void)                                  //Инициализация модуля USART
{
	UBRR0 = 103;//B19200;
	UCSR0B = (1<<RXEN0)|( 1<<TXEN0);                 //Включаем прием и передачу по USART
	UCSR0B |= (1<<RXCIE0);                          //Разрешаем прерывание при приеме
	UCSR0C = (1<<UCSZ01)|(1<<UPM01)|(1<<UCSZ00);               //паритет 1, 8 бит  ???
}

ISR (TIMER0_OVF_vect)
{
	if (tim_uart0 < 250) {tim_uart0 ++;}
	if (tim_uart0 == 249) { uart0_rx_caunter = 0; rx0_done = true; }
}

//----------функции инициализации отправки данных----------//
bool uart0_send_string ( char *send, uint8_t len)                // сюда передаем строку в конце которой 0x01
{
	if (uart0_tx_caunter == 0)                     // если счетчик на нуле можем обновить буфер, если нет то пропускаем
	{
		for (int i = 0; i < len; i ++)             // забиваем буффер отправки символами из переданной строки
		{
			uart0_tx_buf[i] = send[i];
			// if (uart0_tx_buf[i] == 0x01) break;   // пока не встретим символ 0x01. но его мы тоже заносим
		}
		UDR0 = uart0_tx_buf[uart0_tx_caunter];    // отправляем первый байт
		uart0_tx_caunter ++;                      // увеличиваем счетчик
		UCSR0B |= ( 1<<UDRIE0);                   // включаем прерывание по опустрошению буффера передачи
		leng = len;
		return true;
		} else{
		return false;
	}
}

//----------обработчики прерываний на передачу одинаковые----------//
ISR (USART_UDRE_vect)                                      // прерывание по опустошению буффера передачи
{
	if(uart0_tx_caunter < leng)
	//if(uart0_tx_buf[uart0_tx_caunter] != 0x01)              // пока символ в буффере не равен 0x01
	{
		UDR0 = uart0_tx_buf[uart0_tx_caunter];            // продолжаем копировать из буффера байты в юарт
		uart0_tx_caunter ++;                              // ну и естественно передвигаемся по массиву
	}
	else                                                   // если дошли до символа 0x01
	{
		uart0_tx_caunter = 0;                             // сбрасываем счетчик
		UCSR0B  &= ~ ( 1<<UDRIE0);                        // вырубаем прерывания по опустошению буфера
	}
}

ISR (USART_RX_vect)
{
	tim_uart0 = 0;                                                        // взводим таймер, на 150 (при таком раскладе это чуть больше чем длится прием одного байта)
	uart0_rx_buf[uart0_rx_caunter] = UDR0;                                // сохраняем принятый байт
	uart0_rx_caunter ++; if(uart0_rx_caunter > 18) uart0_rx_caunter = 0;  // если нулевой элемент 0 то продолжаем, если счетчик перевалил за 8 то сбросим в ноль
}

uint8_t parsing_dat (void)       //парсинг входящих данных
{
	static uint8_t out;
	if (rx0_done){              // если поднят флаг входящих данных
		rx0_done = false;      // сбросим его
		if(uart0_rx_buf[0] == 0x02 && uart0_rx_buf[6] == 0x03){  // проверяем первый и последний символ чтоб понять 32 бита или 8
			switch (uart0_rx_buf[1]){     // потом по первому символу смотрим что же за команда нам пришла
				case NEW_K_K1:
				in_float = transmit_32_double (collect_massiv(uart0_rx_buf));  // если это флоат то сначала складываем в массив данные а потом транспортируем в флоат
				clear_buf(); // очищаем буффер
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
//------------обновление Nextion--------//						
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
		
		if(uart0_rx_buf[0] == 0x02  && uart0_rx_buf[2] == 0x03 && uart0_rx_buf[6] == 0x00)     // если у нас короткое сообщение
		{
			out = uart0_rx_buf[1];
			clear_buf(); // очистить буфер
			return out;
		}
		clear_buf(); // очистить буфер
		return 0;   // вернуть ноль
	}
	return 0;
}

void copy_clear (void) {                    // чтоб 100 раз не писать одно и тоже
	in_data = collect_massiv(uart0_rx_buf); // если это вес, транспортируем его в 32битную переменную
	clear_buf();
}

void clear_buf (void){  // самая простая очистка буффера
	for (uint8_t i = 0; i < 18; i ++){
		uart0_rx_buf [i] = 0x00;
	}
}

uint32_t collect_massiv (char *massiv){       // собрать 32 битное число из входящих данных
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

double transmit_32_double (uint32_t dat){   // преобразовать 32 битное число в дабл
	static union                          // так же само, создаем объеденение, и обращаемся к одним и тем же данным по разному
	{
		double in_float;
		uint32_t out_float;
	}floatchik;
	floatchik.out_float = dat;
	return floatchik.in_float;                                         // вернули итог типа флоат
}

uint32_t transmit_double_32 (double dat){   // преобразовать дабл в 32 битное число
	static union                           // так же само, создаем объеденение, и обращаемся к одним и тем же данным по разному
	{
		double in_float;
		uint32_t out_float;
	}floatchik;
	floatchik.in_float = dat;
	return floatchik.out_float;                                 // вернули итог типа 32bit
}

uint8_t calcCRC(char *buf, uint8_t len) {                     // расчет срс, взял из дини
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