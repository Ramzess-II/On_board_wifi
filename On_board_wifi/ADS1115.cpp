
#include "ADS1115.h"



void I2C_Init(void)  // ������������� �2�
{
     TWBR = 0x20; //�������� �������� (��� 8 ��� ���������� 100 ���)
     TWSR = 0x01;
}

void I2C_StartCondition(void)  // ���������  �����
{
     TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);
     while(!(TWCR&(1<<TWINT)));//�������� ���� ����������� TWIN //���� TWINT ����������� � ��������� ���������
}

void I2C_StopCondition(void)     // ��������� ����
{
     TWCR = (1<<TWINT)|(1<<TWSTO)|(1<<TWEN);
}

void I2C_SendByte(unsigned char c)     // ��������� ����
{
     TWDR = c;//������� ���� � ������� ������
     TWCR = (1<<TWINT)|(1<<TWEN);//������� �������� �����
     while (!(TWCR & (1<<TWINT)));//�������� ���� ����������� TWIN
}

unsigned char EE_ReadByte(void)    // ��������� ����, � ������� ��
{
     TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWEA);//������� ����� ������
     while(!(TWCR & (1<<TWINT)));//�������� ���� ����������� TWIN
     return TWDR;
}

unsigned char EE_ReadLastByte(void)  // ��������� ��������� ���� � �� �������� ��
{
     TWCR = (1<<TWINT)|(1<<TWEN);//������� ����� ������
     while(!(TWCR&(1<<TWINT)));//�������� ���� ����������� TWIN
     return TWDR;
}

void init_adc (void) {  // ������������� ���
     
     I2C_StartCondition(); // ��������� �����
     I2C_SendByte(0x90); // ������ ����������
     I2C_SendByte(0x03); // ����� ��������
     I2C_SendByte(0xFF); // �������� 16 ������ ������ ��� ������� ������� �������
     I2C_SendByte(0xFF); // ����� ������� ����
     I2C_StopCondition();
     
     I2C_StartCondition();
     I2C_SendByte(0x90); // 90 ������ 91 ������
     I2C_SendByte(0x02);
     I2C_SendByte(0x00);
     I2C_SendByte(0x00);
     I2C_StopCondition();
     
     I2C_StartCondition();
     I2C_SendByte(0x90); // 90 ������ 91 ������
     I2C_SendByte(0x01);
     I2C_SendByte(0xC1); // ����� 1 �������� + ��������� ��������������
     I2C_SendByte(0xA8); // 250 ���� + �������� ������� �������
     I2C_StopCondition();
}

uint16_t get_data (void){  // ������ ������ �� ���
     static uint16_t dat1;
   //  static uint8_t mirror;
     I2C_StartCondition();
     I2C_SendByte(0x90); // ��������� �����
     I2C_SendByte(0x00); // �������� ������� �� ������� ������
     I2C_StopCondition(); // ����
     I2C_StartCondition(); // ��������� �����
     I2C_SendByte(0x91);  // � ������ ������
     
     dat1 = EE_ReadByte();  // ������ ������� �����
     dat1 = dat1 << 8;
     dat1 |= EE_ReadLastByte();  // ����� �������
     I2C_StopCondition();
     return dat1;
}

void start_convertion (uint8_t number)
{
     I2C_StartCondition();
     I2C_SendByte(0x90); // 90 ������ 91 ������
     I2C_SendByte(0x01);
     switch(number){
          case 1: I2C_SendByte(0xC1); // ����� 1 �������� + ��������� ��������������
          break;
          case 2: I2C_SendByte(0xD1); // ����� 2 �������� + ��������� ��������������
          break;
          case 3: I2C_SendByte(0xE1); // ����� 3 �������� + ��������� ��������������
          break;
          case 4: I2C_SendByte(0xF1); // ����� 4 �������� + ��������� ��������������
          break;
     }
     I2C_SendByte(0xA8); // 250 ����
     I2C_StopCondition();
}