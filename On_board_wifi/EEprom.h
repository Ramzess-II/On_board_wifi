
#ifndef EEPROM_H_
#define EEPROM_H_

#include <avr/io.h>
#include <avr/interrupt.h>


void EEPROM_write(uint16_t uiAddress, uint8_t ucData);
uint8_t EEPROM_read(uint16_t uiAddress);
uint32_t EEPROM_read_32t (uint16_t uiAddress);
void EEPROM_write_32t (uint16_t uiAddress, uint32_t ucData) ;
void EEPROM_write_float (uint16_t uiAddress, double ucData) ;
double EEPROM_read_float (uint16_t uiAddress) ;
uint16_t EEPROM_read_16t (uint16_t uiAddress);
void EEPROM_write_16t (uint16_t uiAddress, uint16_t ucData);
void init_EEprom (void);





// тут храним адреса еепром памяти для всех нужных переменных
enum {
     EE_AMOUNT_ADC      = 0x00,  // 8
     EE_FILTR           = 0x01,  // 8
     EE_K_KOEF1         = 0x02,  // 32
     EE_K_KOEF2         = 0x06,  // 32
     EE_K_KOEF3         = 0x0A,  // 32
     EE_K_KOEF4         = 0x0E,  // 32
     EE_ZERO_ADC1       = 0x12,  // 16
     EE_ZERO_ADC2       = 0x14,  // 16
     EE_ZERO_ADC3       = 0x16,  // 16
     EE_ZERO_ADC4       = 0x18,  // 16
     EE_DISKRET         = 0x20,  // 8
     EE_K_KOEF4_1       = 0x21,  // 32
     EE_K_KOEF4_2       = 0x25,  // 32
     EE_K_KOEF4_3       = 0x29,  // 32
     EE_K_KOEF4_4       = 0x2D,  // 32
     EE_TARA            = 0x31,  // 32
     EE_NUMBER_KK4      = 0x35,  // 32
     EE_K_KOEF4_5       = 0x39,  // 32
	 EE_ZERO_AX         = 0x3D,  // 32
//------------обновление Nextion--------//
	 EE_CELL0           = 0x41,  // 32
	 EE_CELL1           = 0x45,  // 32
	 EE_CELL2           = 0x49,  // 32
	 EE_CELL3           = 0x4D,  // 32
	 EE_CELL4           = 0x51,  // 32
	 EE_CELL5           = 0x55,  // 32
	 EE_CELL6           = 0x59,  // 32
	 EE_CELL7           = 0x5D,  // 32
	 EE_CELL8           = 0x61,  // 32
	 EE_CELL9           = 0x65,  // 32
	 
};




#endif /* EEPROM_H_ */