
#include "ADS1115.h"



void I2C_Init(void)  // инициализаци€ »2—
{
     TWBR = 0x20; //скорость передачи (при 8 м√ц получаетс€ 100 к√ц)
     TWSR = 0x01;
}

void I2C_StartCondition(void)  // формируем  старт
{
     TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
     while(!(TWCR&(1<<TWINT)));//подождем пока установитс€ TWIN //‘лаг TWINT установитс€ в следующих ситуаци€х
}

void I2C_StopCondition(void)     // формируем стоп
{
     TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2C_SendByte(unsigned char c)     // отправить байт
{
     TWDR = c;//запишем байт в регистр данных
     TWCR = (1<<TWINT)|(1<<TWEN);//включим передачу байта
     while (!(TWCR & (1<<TWINT)));//подождем пока установитс€ TWIN
}

unsigned char EE_ReadByte(void)    // прочитать байт, и сказать ок
{
     TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);//включим прием данных
     while(!(TWCR & (1<<TWINT)));//подождем пока установитс€ TWIN
     return TWDR;
}

unsigned char EE_ReadLastByte(void)  // прочитать последний байт и не говорить ок
{
     TWCR = (1<<TWINT)|(1<<TWEN);//включим прием данных
     while(!(TWCR&(1<<TWINT)));//подождем пока установитс€ TWIN
     return TWDR;
}

void init_adc (void) {  // инициализаци€ ј÷ѕ
     
     I2C_StartCondition(); // формируем старт
     I2C_SendByte(0x90); // адресс устройства
     I2C_SendByte(0x03); // адрес регистра
     I2C_SendByte(0xFF); // регистры 16 битные значит две послыки сначала старший
     I2C_SendByte(0xFF); // потом младший байт
     I2C_StopCondition();
     
     I2C_StartCondition();
     I2C_SendByte(0x90); // 90 запись 91 чтение
     I2C_SendByte(0x02);
     I2C_SendByte(0x00);
     I2C_SendByte(0x00);
     I2C_StopCondition();
     
     I2C_StartCondition();
     I2C_SendByte(0x90); // 90 запись 91 чтение
     I2C_SendByte(0x01);
     I2C_SendByte(0xC1); // канал 1 выбираем + одиночное преобразование
     I2C_SendByte(0xA8); // 250 герц + активный высокий уровень
     I2C_StopCondition();
}

uint16_t get_data (void){  // чтение данных из ј÷ѕ
     static uint16_t dat1;
   //  static uint8_t mirror;
     I2C_StartCondition();
     I2C_SendByte(0x90); // отправили адрес
     I2C_SendByte(0x00); // записали регистр из которго читаем
     I2C_StopCondition(); // стоп
     I2C_StartCondition(); // повторный старт
     I2C_SendByte(0x91);  // и теперь чтение
     
     dat1 = EE_ReadByte();  // читаем старшую часть
     dat1 = dat1 << 8;
     dat1 |= EE_ReadLastByte();  // потом младшую
     I2C_StopCondition();
     return dat1;
}

void start_convertion (uint8_t number)
{
     I2C_StartCondition();
     I2C_SendByte(0x90); // 90 запись 91 чтение
     I2C_SendByte(0x01);
     switch(number){
          case 1: I2C_SendByte(0xC1); // канал 1 выбираем + одиночное преобразование
          break;
          case 2: I2C_SendByte(0xD1); // канал 2 выбираем + одиночное преобразование
          break;
          case 3: I2C_SendByte(0xE1); // канал 3 выбираем + одиночное преобразование
          break;
          case 4: I2C_SendByte(0xF1); // канал 4 выбираем + одиночное преобразование
          break;
     }
     I2C_SendByte(0xA8); // 250 герц
     I2C_StopCondition();
}