
#ifndef ADS1115_H_
#define ADS1115_H_

#include <avr/io.h>


void I2C_Init(void);
void I2C_StartCondition(void);
void I2C_StopCondition(void);
void I2C_SendByte(unsigned char c);
void init_adc (void);
uint16_t get_data (void);
unsigned char EE_ReadLastByte(void);
unsigned char EE_ReadByte(void);
void start_convertion (uint8_t number);


#endif 