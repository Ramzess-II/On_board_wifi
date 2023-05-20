#include "EEprom.h"

void EEPROM_write(uint16_t uiAddress, uint8_t ucData)      // функция записи в еепром 8 битного числа  по заданному адресу
{
     cli();
     while(EECR & (1<<EEPE)){}       //ждем освобождения флага окончания последней операцией с памятью
     EEAR = uiAddress;               //Устанавливаем адрес
     EEDR = ucData;                  //Пищем данные в регистр
     EECR |= (1<<EEMPE);             //Разрешаем запись
     EECR |= (1<<EEPE);              //Пишем байт в память
     sei();
}

uint8_t EEPROM_read(uint16_t uiAddress)                     // функция чтения 8 битного числа из еепром
{
     cli();
     while(EECR & (1<<EEPE)){}                              //ждем освобождения флага окончания последней операцией с памятью
     EEAR = uiAddress;                                      //Устанавливаем адрес
     EECR |= (1<<EERE);                                     //Запускаем операцию считывания из памяти в регистр данных
     sei();
     return EEDR; //Возвращаем результат
}

uint32_t EEPROM_read_32t (uint16_t uiAddress)                // функция чтения 32битного числа из еепром по заданному дресу
{
     static uint32_t receive_data;                            // создали переменную
     receive_data = EEPROM_read (uiAddress+0x03);             // считали в нее старшый байт
     receive_data = receive_data << 8;                        // сдвинули освободив место под младший
     receive_data |= EEPROM_read (uiAddress+0x02);            // считали второй байт и присвоили без изменения других битов
     receive_data = receive_data << 8;                        // сдвинули
     receive_data |= EEPROM_read (uiAddress+0x01);
     receive_data = receive_data << 8;
     receive_data |= EEPROM_read (uiAddress);
     return receive_data;                                     // вернули итог
}

void EEPROM_write_32t (uint16_t uiAddress, uint32_t ucData)   // запись 32 битного числа
{
     EEPROM_write (uiAddress,ucData);                          // в обратном порядке складываем в память
     EEPROM_write (uiAddress + 0x01 ,ucData >> 8);
     EEPROM_write (uiAddress + 0x02 ,ucData >> 16);
     EEPROM_write (uiAddress + 0x03 ,ucData >> 24);
}

void EEPROM_write_float (uint16_t uiAddress, double ucData)        // запись float.
{
     static union                                                  // создаем объеденение
     {
          double in_float;                                          // два числа которые начинаются с одного адресса но имеют разный тип
          uint32_t out_float;                                       // мы флоат записываем как флоат, а потом при записи в еепром уже пользуемся 32 бит переменной
     }floatchik;
     floatchik.in_float = ucData;
     EEPROM_write (uiAddress,floatchik.out_float);                          // в обратном порядке складываем в память
     EEPROM_write (uiAddress + 0x01 ,floatchik.out_float >> 8);
     EEPROM_write (uiAddress + 0x02 ,floatchik.out_float >> 16);
     EEPROM_write (uiAddress + 0x03 ,floatchik.out_float >> 24);
}

double EEPROM_read_float (uint16_t uiAddress)                       // вычитываем float
{
     static union                                                   // так же само, создаем объеденение, и обращаемся к одним и тем же данным по разному
     {
          double in_float;
          uint32_t out_float;
     }floatchik;
     floatchik.out_float = EEPROM_read (uiAddress+0x03);                // считали в нее старшый байт
     floatchik.out_float = floatchik.out_float << 8;                    // сдвинули освободив место под младший
     floatchik.out_float |= EEPROM_read (uiAddress+0x02);               // считали второй байт и присвоили без изменения других битов
     floatchik.out_float = floatchik.out_float << 8;                    // сдвинули
     floatchik.out_float |= EEPROM_read (uiAddress+0x01);
     floatchik.out_float = floatchik.out_float << 8;
     floatchik.out_float |= EEPROM_read (uiAddress);
     return floatchik.in_float;                                         // вернули итог типа флоат
}

uint16_t EEPROM_read_16t (uint16_t uiAddress)                 // тоже самое только для 16 бит
{
     static uint16_t receive_data;
     receive_data = EEPROM_read (uiAddress+0x01);
     receive_data = receive_data << 8;
     receive_data |= EEPROM_read (uiAddress);
     return receive_data;
}

void EEPROM_write_16t (uint16_t uiAddress, uint16_t ucData)
{
     EEPROM_write (uiAddress,ucData);
     EEPROM_write (uiAddress + 0x01 ,ucData >> 8);
}


void init_EEprom (void)                                       // инициализация еепром, забиваем стандартные значения
{
     EEPROM_write(EE_AMOUNT_ADC,4);
     EEPROM_write(EE_FILTR,5);
     EEPROM_write(EE_DISKRET,20);
      //----------metrology----------//
     EEPROM_write_float(EE_K_KOEF1,1.000);
     EEPROM_write_float(EE_K_KOEF2,1.000);
     EEPROM_write_float(EE_K_KOEF3,1.000);
     EEPROM_write_float(EE_K_KOEF4,1.000);  
     EEPROM_write_float(EE_K_KOEF4_1,1.000);
     EEPROM_write_float(EE_K_KOEF4_2,1.000);
     EEPROM_write_float(EE_K_KOEF4_3,1.000);
     EEPROM_write_float(EE_K_KOEF4_4,1.000);
     EEPROM_write_float(EE_K_KOEF4_5,1.000);
     EEPROM_write_32t(EE_TARA, 5000);
     EEPROM_write_16t (EE_ZERO_ADC1,10);
     EEPROM_write_16t (EE_ZERO_ADC2,10);
     EEPROM_write_16t (EE_ZERO_ADC3,10);
     EEPROM_write_16t (EE_ZERO_ADC4,10);
     EEPROM_write_32t(EE_NUMBER_KK4,1);
	 EEPROM_write_32t(EE_ZERO_AX,0);
//------------обновление Nextion--------//
     EEPROM_write_32t(EE_CELL0,0);
	 EEPROM_write_32t(EE_CELL1,0);
	 EEPROM_write_32t(EE_CELL2,0);
	 EEPROM_write_32t(EE_CELL3,0);
	 EEPROM_write_32t(EE_CELL4,0);
	 EEPROM_write_32t(EE_CELL5,0);
	 EEPROM_write_32t(EE_CELL6,0);
	 EEPROM_write_32t(EE_CELL7,0);
	 EEPROM_write_32t(EE_CELL8,0);
	 EEPROM_write_32t(EE_CELL9,0);
     
}



