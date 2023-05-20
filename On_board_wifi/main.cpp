#include "main.h"

//--------------переменные из других файлов---------------//
extern double in_float;
extern uint32_t in_data;
//--------------переменные из этого файла-----------------//
struct bool_date boolean;
struct analog_data adc_data;
struct setting_s setting;
struct cell_s cell_struct;
uint8_t count_adc_num = 1;
uint8_t cod_command;


char send_adc_data [20];



ISR(INT0_vect){
	boolean.go = true;
}


int main(void)
{
	
	init_atmega();
	I2C_Init();
	init_adc();
	init_UART();
	if (INIT_PIN) {init_EEprom(); LED_ON; _delay_ms(100); LED_OFF;  _delay_ms(100); LED_ON; _delay_ms(100); LED_OFF;}
	// если замкнуть ножку  D8 на землю то сбросить ЕЕпром
	if (UART_PIN) { LED_ON; UCSR0C = 0x06;}
	
	read_ALL_EEprom();
	
	for(;;)
	{
		wdt_reset();
		cod_command = parsing_dat();
		uart_data_comand(cod_command);
		
		if (boolean.go){           // если поднялся флаг принятых данных
			boolean.go = false;   // сбрасываем его
			switch (count_adc_num){  // в зависимости от того что опрашивали сохраняем данные
				case 1:
				adc_data.adc1_clear = get_data();      // получаем чистый код АЦП, чтоб можно было его использовать для кода нуля
				adc_to_massa(count_adc_num, setting.filtr, adc_data.adc1_clear);   // и дальше отправляем на преобразование
				count_adc_num = 2;
				start_convertion(count_adc_num);       // запускаем преобразование заново
				break;
				case 2:
				adc_data.adc2_clear = get_data();
				adc_to_massa(count_adc_num, setting.filtr, adc_data.adc2_clear);
				count_adc_num = 3;
				start_convertion(count_adc_num);       // запускаем преобразование заново
				break;
				case 3:
				adc_data.adc3_clear = get_data();
				adc_to_massa(count_adc_num, setting.filtr, adc_data.adc3_clear);
				count_adc_num = 4;
				start_convertion(count_adc_num);       // запускаем преобразование заново
				break;
				case 4:
				adc_data.adc4_clear = get_data();
				adc_to_massa(count_adc_num, setting.filtr, adc_data.adc4_clear);
				count_adc_num = 1;
				start_convertion(count_adc_num);       // запускаем преобразование заново
				break;
			}
		}
	}
}

void uart_data_comand (uint8_t code){          // после парсинга попадаем сюда
	switch(code){                             // в зависимости от кода выполняем действия
		case 0: break;
		case ADC1_OUT:
		massa_send_uart(1,adc_data.massa1);  // отвечаем массой
		break;
		case ADC2_OUT:
		massa_send_uart(2,adc_data.massa2);
		break;
		case ADC3_OUT:
		massa_send_uart(3,adc_data.massa3);
		break;
		case ADC4_OUT:
		massa_send_uart(4,adc_data.massa4);
		break;
		case ZERO_ALL:  // обнуляемся без сохранения в память
		break;
		case NEW_K_K1:  // изменяем коэф для первого ацп
		setting.kal_koef1 = in_float;
		EEPROM_write_float(EE_K_KOEF1,setting.kal_koef1);
		break;
		case NEW_K_K2:
		setting.kal_koef2 = in_float;
		EEPROM_write_float(EE_K_KOEF2,setting.kal_koef2);
		break;
		case NEW_K_K3:
		setting.kal_koef3 = in_float;
		EEPROM_write_float(EE_K_KOEF3,setting.kal_koef3);
		break;
		case NEW_K_K4: // тут добавляем нововведение, теперь записываем тот коєф, который был выбран
		setting.kal_koef4 = in_float;
		switch (setting.number_kk_4) {
			case 1:
			setting.kal_koef4_1 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_1,setting.kal_koef4);
			break;
			case 2:
			setting.kal_koef4_2 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_2,setting.kal_koef4);
			break;
			case 3:
			setting.kal_koef4_3 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_3,setting.kal_koef4);
			break;
			case 4:
			setting.kal_koef4_4 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_4,setting.kal_koef4);
			break;
			case 5:
			setting.kal_koef4_5 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_5,setting.kal_koef4);
			break;
		}
		break;
		case NEW_MASSA1: // расчитываем коэф по новой массе
		setting.kal_koef1 = (double) in_data / (double) labs(adc_data.adc_kalib1); // для калибровки нужны значения АЦП без коэфициента adc_data.adc_kalib1
		EEPROM_write_float(EE_K_KOEF1,setting.kal_koef1);
		break;
		case NEW_MASSA2:
		setting.kal_koef2 = (double) in_data / (double) labs(adc_data.adc_kalib2);
		EEPROM_write_float(EE_K_KOEF2,setting.kal_koef2);
		break;
		case NEW_MASSA3:
		setting.kal_koef3 = (double) in_data / (double) labs(adc_data.adc_kalib3);
		EEPROM_write_float(EE_K_KOEF3,setting.kal_koef3);
		break;
		case NEW_MASSA4:
		setting.kal_koef4 = (double) in_data / (double) labs(adc_data.adc_kalib4);
		switch (setting.number_kk_4) {
			case 1:
			setting.kal_koef4_1 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_1,setting.kal_koef4);
			break;
			case 2:
			setting.kal_koef4_2 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_2,setting.kal_koef4);
			break;
			case 3:
			setting.kal_koef4_3 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_3,setting.kal_koef4);
			break;
			case 4:
			setting.kal_koef4_4 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_4,setting.kal_koef4);
			break;
			case 5:
			setting.kal_koef4_5 = setting.kal_koef4;
			EEPROM_write_float(EE_K_KOEF4_5,setting.kal_koef4);
			break;
		}
		break;
		case NEW_DISKRT:
		setting.disreta = in_data;
		EEPROM_write(EE_DISKRET,setting.disreta);
		break;
		case SEND_DISKRT:
		send_8bit (SEND_DISKRT, setting.disreta);
		break;
		case NEW_FILTR:
		setting.filtr = in_data;
		EEPROM_write(EE_FILTR,setting.filtr);
		break;
		case SEND_FILTR:
		send_8bit (SEND_FILTR, setting.filtr);
		break;
		case SEND_K_K1:
		send_32bit(SEND_K_K1, transmit_double_32(setting.kal_koef1));
		break;
		case SEND_K_K2:
		send_32bit(SEND_K_K2, transmit_double_32(setting.kal_koef2));
		break;
		case SEND_K_K3:
		send_32bit(SEND_K_K3, transmit_double_32(setting.kal_koef3));
		break;
		case SEND_K_K4:
		send_32bit(SEND_K_K4, transmit_double_32(setting.kal_koef4));
		break;
		case ZERO_1:
		adc_data.zero_adc1 = adc_data.adc1_clear;
		EEPROM_write_16t (EE_ZERO_ADC1,adc_data.zero_adc1);
		break;
		case ZERO_2:
		adc_data.zero_adc2 = adc_data.adc2_clear;
		EEPROM_write_16t (EE_ZERO_ADC2,adc_data.zero_adc2);
		break;
		case ZERO_3:
		adc_data.zero_adc3 = adc_data.adc3_clear;
		EEPROM_write_16t (EE_ZERO_ADC3,adc_data.zero_adc3);
		break;
		case ZERO_4:
		adc_data.zero_adc4 = adc_data.adc4_clear;
		EEPROM_write_16t (EE_ZERO_ADC4,adc_data.zero_adc4);
		break;
		case SEND_TARA:
		send_32bit(SEND_TARA, setting.tara);
		break;
		case NEW_TARA:
		setting.tara = in_data;
		EEPROM_write_32t(EE_TARA, setting.tara);
		break;
		case NEW_NUMBER_KK:
		setting.number_kk_4 = in_data;
		switch (setting.number_kk_4){
			case 1:
			setting.kal_koef4 =  setting.kal_koef4_1;
			break;
			case 2:
			setting.kal_koef4 =  setting.kal_koef4_2;
			break;
			case 3:
			setting.kal_koef4 =  setting.kal_koef4_3;
			break;
			case 4:
			setting.kal_koef4 =  setting.kal_koef4_4;
			break;
			case 5:
			setting.kal_koef4 =  setting.kal_koef4_5;
			break;
		}
		EEPROM_write(EE_NUMBER_KK4,setting.number_kk_4);
		break;
		case SEND_NUMBER_KK:
		send_32bit (SEND_NUMBER_KK,setting.number_kk_4);
		break;
		//------------обновление Nextion--------//
		case NEW_ZERO_AX:
		setting.zero_ax = in_data;
		EEPROM_write_32t(EE_ZERO_AX, setting.zero_ax);
		break;
		case SEND_ZERO_AX:
		send_32bit (SEND_ZERO_AX,setting.zero_ax);
		break;
		
		case NEW_CELL0:
		cell_struct.cell0 = in_data;
		EEPROM_write_32t(EE_CELL0, cell_struct.cell0);
		break;
		case SEND_CELL0:
		send_32bit (SEND_CELL0, cell_struct.cell0);
		break;
		case NEW_CELL1:
		cell_struct.cell1 = in_data;
		EEPROM_write_32t(EE_CELL1, cell_struct.cell1);
		break;
		case SEND_CELL1:
		send_32bit (SEND_CELL1, cell_struct.cell1);
		break;
		case NEW_CELL2:
		cell_struct.cell2 = in_data;
		EEPROM_write_32t(EE_CELL2, cell_struct.cell2);
		break;
		case SEND_CELL2:
		send_32bit (SEND_CELL2, cell_struct.cell2);
		break;
		case NEW_CELL3:
		cell_struct.cell3 = in_data;
		EEPROM_write_32t(EE_CELL3, cell_struct.cell3);
		break;
		case SEND_CELL3:
		send_32bit (SEND_CELL3, cell_struct.cell3);
		break;
		case NEW_CELL4:
		cell_struct.cell4 = in_data;
		EEPROM_write_32t(EE_CELL4, cell_struct.cell4);
		break;
		case SEND_CELL4:
		send_32bit (SEND_CELL4, cell_struct.cell4);
		break;
		case NEW_CELL5:
		cell_struct.cell5 = in_data;
		EEPROM_write_32t(EE_CELL5, cell_struct.cell5);
		break;
		case SEND_CELL5:
		send_32bit (SEND_CELL5, cell_struct.cell5);
		break;
		case NEW_CELL6:
		cell_struct.cell6 = in_data;
		EEPROM_write_32t(EE_CELL6, cell_struct.cell6);
		break;
		case SEND_CELL6:
		send_32bit (SEND_CELL6, cell_struct.cell6);
		break;
		case NEW_CELL7:
		cell_struct.cell7 = in_data;
		EEPROM_write_32t(EE_CELL7, cell_struct.cell7);
		break;
		case SEND_CELL7:
		send_32bit (SEND_CELL7, cell_struct.cell7);
		break;
		case NEW_CELL8:
		cell_struct.cell8 = in_data;
		EEPROM_write_32t(EE_CELL8, cell_struct.cell8);
		break;
		case SEND_CELL8:
		send_32bit (SEND_CELL8, cell_struct.cell8);
		break;
		case NEW_CELL9:
		cell_struct.cell9 = in_data;
		EEPROM_write_32t(EE_CELL9, cell_struct.cell9);
		break;
		case SEND_CELL9:
		send_32bit (SEND_CELL9, cell_struct.cell9);
		break;
		
	}
	cod_command = 0;         // сбрасываем в ноль
	in_data = 0;
	in_float = 0;
}

void init_atmega (void) {
	//--инициализируем порты-- //
	DDRB = 0x29;
	PORTB = 0x09;

	//--инициализируем прерывание -- INT1//
	EICRA = 0x03;            // EICRA = 0x03 прерывание по нарастанию. EICRA = 0x02 прерывание по спаду. EICRA = 0x01 по любому изменению
	EIMSK = 0x01;	          // разрешить прерывание INT1
	//--инициализируем таймер 0//
	TCCR0B = 0x01;          // делитель 0 на таймер 0       62 KГерц
	TIMSK0 = 0x01;          // включить прерывания по переполнению
	
	sei ();
	wdt_enable(WDTO_1S);
}

void read_ALL_EEprom (void)                                   // считываем значение из еепром при старте для всех переменных
{
	setting.amount_adc          = EEPROM_read(EE_AMOUNT_ADC);
	setting.filtr               = EEPROM_read(EE_FILTR);
	setting.disreta             = EEPROM_read(EE_DISKRET);
	setting.number_kk_4         = EEPROM_read_32t(EE_NUMBER_KK4);
	setting.kal_koef1           = EEPROM_read_float(EE_K_KOEF1);
	setting.kal_koef2           = EEPROM_read_float(EE_K_KOEF2);
	setting.kal_koef3           = EEPROM_read_float(EE_K_KOEF3);
	setting.kal_koef4           = EEPROM_read_float(EE_K_KOEF4);
	setting.kal_koef4_1         = EEPROM_read_float(EE_K_KOEF4_1);      // нужны ли эти переменные 4шт???
	setting.kal_koef4_2         = EEPROM_read_float(EE_K_KOEF4_2);
	setting.kal_koef4_3         = EEPROM_read_float(EE_K_KOEF4_3);
	setting.kal_koef4_4         = EEPROM_read_float(EE_K_KOEF4_4);
	setting.kal_koef4_5         = EEPROM_read_float(EE_K_KOEF4_5);
	setting.tara                = EEPROM_read_32t(EE_TARA);
	setting.zero_ax             = EEPROM_read_32t(EE_ZERO_AX);
	adc_data.zero_adc1          = EEPROM_read_16t (EE_ZERO_ADC1);
	adc_data.zero_adc2          = EEPROM_read_16t (EE_ZERO_ADC2);
	adc_data.zero_adc3          = EEPROM_read_16t (EE_ZERO_ADC3);
	adc_data.zero_adc4          = EEPROM_read_16t (EE_ZERO_ADC4);
	//------------обновление Nextion--------//
	cell_struct.cell0           = EEPROM_read_32t(EE_CELL0);
	cell_struct.cell1           = EEPROM_read_32t(EE_CELL1);
	cell_struct.cell2           = EEPROM_read_32t(EE_CELL2);
	cell_struct.cell3           = EEPROM_read_32t(EE_CELL3);
	cell_struct.cell4           = EEPROM_read_32t(EE_CELL4);
	cell_struct.cell5           = EEPROM_read_32t(EE_CELL5);
	cell_struct.cell6           = EEPROM_read_32t(EE_CELL6);
	cell_struct.cell7           = EEPROM_read_32t(EE_CELL7);
	cell_struct.cell8           = EEPROM_read_32t(EE_CELL8);
	cell_struct.cell9           = EEPROM_read_32t(EE_CELL9);

	
	switch (setting.number_kk_4){                                      // после чтения сразу установим нужный параметр
		case 1:
		setting.kal_koef4 = setting.kal_koef4_1;
		break;
		case 2:
		setting.kal_koef4 = setting.kal_koef4_2;
		break;
		case 3:
		setting.kal_koef4 = setting.kal_koef4_3;
		break;
		case 4:
		setting.kal_koef4 = setting.kal_koef4_4;
		break;
		case 5:
		setting.kal_koef4 = setting.kal_koef4_5;
		break;
	}
}

void adc_to_massa (uint8_t number, uint8_t filtr, int16_t adc){      // функция преобразования кода АЦП в вес
	static int32_t adc_unsignet;
	static uint8_t count1,count2,count3,count4;                      // счетчики перемещения по фильтру
	static int32_t filtr_adc1[42],filtr_adc2[42],filtr_adc3[42],filtr_adc4[42];    // сами ячейки фильтра
	static int32_t adc_filtr;                                       // для хранения суммы фильтра
	
	switch (number) {                                                // сюда передаем номер канала с которым работаем
		case 1:
		adc = adc - adc_data.zero_adc1;                            // сначала отнимаем код нуля
		filtr_adc1[count1] = adc;                                  // записываем в ячейку памяти
		count1 ++;                                                 // увеличиваем значение ячейки в которую пишем
		if (count1 >= filtr) {count1 = 0;}                         // если она равняется фильтру начинаем писать в 0 ячейку
		for (uint8_t i = 0; i < filtr; i ++){                      // складываем все ячейки в одно число
			adc_filtr += filtr_adc1[i];
		}
		adc_unsignet = adc_filtr / filtr;                         // делим на значение фильтра и записываем результат
		adc_data.adc_kalib1 = adc_unsignet;                        // для калибровки нужны значения без коэфициента
		adc_data.massa1 = ((adc_unsignet * setting.kal_koef1)+0.5);        // потом умножаем на калибровочный коэф
		adc_filtr = 0;
		break;
		
		case 2:
		adc = adc - adc_data.zero_adc2;
		filtr_adc2[count1] = adc;
		count2 ++;
		if (count2 >= filtr) {count2 = 0;}
		for (uint8_t i = 0; i < filtr; i ++){
			adc_filtr += filtr_adc2[i];
		}
		adc_unsignet = adc_filtr / filtr;
		adc_data.adc_kalib2 = adc_unsignet;
		adc_data.massa2 = ((adc_unsignet * setting.kal_koef2)+0.5);
		adc_filtr = 0;
		break;
		
		case 3:
		adc = adc - adc_data.zero_adc3;
		filtr_adc3[count1] = adc;
		count3 ++;
		if (count3 >= filtr) {count3 = 0;}
		for (uint8_t i = 0; i < filtr; i ++){
			adc_filtr += filtr_adc3[i];
		}
		adc_unsignet = adc_filtr / filtr;
		adc_data.adc_kalib3 = adc_unsignet;
		adc_data.massa3 = ((adc_unsignet * setting.kal_koef3)+0.5);
		adc_filtr = 0;
		break;
		
		case 4:
		adc = adc - adc_data.zero_adc4;
		filtr_adc4[count1] = adc;
		count4 ++;
		if (count4 >= filtr) {count4 = 0;}
		for (uint8_t i = 0; i < filtr; i ++){
			adc_filtr += filtr_adc4[i];
		}
		adc_unsignet = adc_filtr / filtr;
		adc_data.adc_kalib4 = adc_unsignet;
		adc_data.massa4 = ((adc_unsignet * setting.kal_koef4)+0.5);
		adc_filtr = 0;
		break;
	}
}

int32_t diskret (int32_t data, uint8_t dskrt)        // функция дискрета, взята с впшки
{
	static uint32_t temp;
	if (dskrt == 1) return (data);
	temp = (data / dskrt) * dskrt;
	if ((data % dskrt) >= dskrt/2) temp += dskrt;
	return (temp);
}

void massa_send_uart (uint8_t number, int32_t massa ){   // передаем в юарт массу как она есть со знаком
	massa = diskret(massa, setting.disreta);            // приводим к нужному дискрету
	send_adc_data [0] = 0x02;                           // и по протоколу отправляем
	send_adc_data [1] = number ;
	send_adc_data [2] = massa;
	send_adc_data [3] = massa >> 8;
	send_adc_data [4] = massa >> 16;
	send_adc_data [5] = massa >> 24;
	//send_adc_data [6] = calcCRC (send_adc_data, 6);  //  6 потому что мы не используем в расчетах 6 байт
	send_adc_data [6] = 0x03;
	uart0_send_string (send_adc_data, 7);    // 7 потому что 7 тоже нужно передать
	for (int i = 0; i < 10; i ++){
		send_adc_data [i] = 0x00;
	}
}

void send_32bit (uint8_t command, uint32_t data){
	send_adc_data [0] = 0x02;                           // и по протоколу отправляем
	send_adc_data [1] = command;
	send_adc_data [2] = data;
	send_adc_data [3] = data >> 8;
	send_adc_data [4] = data >> 16;
	send_adc_data [5] = data >> 24;
	// send_adc_data [6] = calcCRC (send_adc_data, 6);  //  6 потому что мы не используем в расчетах 6 байт
	send_adc_data [6] = 0x03;
	uart0_send_string (send_adc_data, 7);            // 7 потому что 7 тоже нужно передать
	for (int i = 0; i < 10; i ++){
		send_adc_data [i] = 0x00;
	}
}

void send_8bit (uint8_t comand, uint8_t data){
	send_adc_data [0] = 0x02;                           // и по протоколу отправляем
	send_adc_data [1] = comand ;
	send_adc_data [2] = data;
	//send_adc_data [3] = calcCRC (send_adc_data, 3);  //  3 потому что мы не используем в расчетах 3 байт
	send_adc_data [3] = 0x03;
	uart0_send_string (send_adc_data, 4);    // 4 потому что 4 тоже нужно передать
	for (int i = 0; i < 10; i ++){
		send_adc_data [i] = 0x00;
	}
}
