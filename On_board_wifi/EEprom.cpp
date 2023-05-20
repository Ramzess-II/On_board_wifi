#include "EEprom.h"

void EEPROM_write(uint16_t uiAddress, uint8_t ucData)      // ������� ������ � ������ 8 ������� �����  �� ��������� ������
{
     cli();
     while(EECR & (1<<EEPE)){}       //���� ������������ ����� ��������� ��������� ��������� � �������
     EEAR = uiAddress;               //������������� �����
     EEDR = ucData;                  //����� ������ � �������
     EECR |= (1<<EEMPE);             //��������� ������
     EECR |= (1<<EEPE);              //����� ���� � ������
     sei();
}

uint8_t EEPROM_read(uint16_t uiAddress)                     // ������� ������ 8 ������� ����� �� ������
{
     cli();
     while(EECR & (1<<EEPE)){}                              //���� ������������ ����� ��������� ��������� ��������� � �������
     EEAR = uiAddress;                                      //������������� �����
     EECR |= (1<<EERE);                                     //��������� �������� ���������� �� ������ � ������� ������
     sei();
     return EEDR; //���������� ���������
}

uint32_t EEPROM_read_32t (uint16_t uiAddress)                // ������� ������ 32������� ����� �� ������ �� ��������� �����
{
     static uint32_t receive_data;                            // ������� ����������
     receive_data = EEPROM_read (uiAddress+0x03);             // ������� � ��� ������� ����
     receive_data = receive_data << 8;                        // �������� ��������� ����� ��� �������
     receive_data |= EEPROM_read (uiAddress+0x02);            // ������� ������ ���� � ��������� ��� ��������� ������ �����
     receive_data = receive_data << 8;                        // ��������
     receive_data |= EEPROM_read (uiAddress+0x01);
     receive_data = receive_data << 8;
     receive_data |= EEPROM_read (uiAddress);
     return receive_data;                                     // ������� ����
}

void EEPROM_write_32t (uint16_t uiAddress, uint32_t ucData)   // ������ 32 ������� �����
{
     EEPROM_write (uiAddress,ucData);                          // � �������� ������� ���������� � ������
     EEPROM_write (uiAddress + 0x01 ,ucData >> 8);
     EEPROM_write (uiAddress + 0x02 ,ucData >> 16);
     EEPROM_write (uiAddress + 0x03 ,ucData >> 24);
}

void EEPROM_write_float (uint16_t uiAddress, double ucData)        // ������ float.
{
     static union                                                  // ������� �����������
     {
          double in_float;                                          // ��� ����� ������� ���������� � ������ ������� �� ����� ������ ���
          uint32_t out_float;                                       // �� ����� ���������� ��� �����, � ����� ��� ������ � ������ ��� ���������� 32 ��� ����������
     }floatchik;
     floatchik.in_float = ucData;
     EEPROM_write (uiAddress,floatchik.out_float);                          // � �������� ������� ���������� � ������
     EEPROM_write (uiAddress + 0x01 ,floatchik.out_float >> 8);
     EEPROM_write (uiAddress + 0x02 ,floatchik.out_float >> 16);
     EEPROM_write (uiAddress + 0x03 ,floatchik.out_float >> 24);
}

double EEPROM_read_float (uint16_t uiAddress)                       // ���������� float
{
     static union                                                   // ��� �� ����, ������� �����������, � ���������� � ����� � ��� �� ������ �� �������
     {
          double in_float;
          uint32_t out_float;
     }floatchik;
     floatchik.out_float = EEPROM_read (uiAddress+0x03);                // ������� � ��� ������� ����
     floatchik.out_float = floatchik.out_float << 8;                    // �������� ��������� ����� ��� �������
     floatchik.out_float |= EEPROM_read (uiAddress+0x02);               // ������� ������ ���� � ��������� ��� ��������� ������ �����
     floatchik.out_float = floatchik.out_float << 8;                    // ��������
     floatchik.out_float |= EEPROM_read (uiAddress+0x01);
     floatchik.out_float = floatchik.out_float << 8;
     floatchik.out_float |= EEPROM_read (uiAddress);
     return floatchik.in_float;                                         // ������� ���� ���� �����
}

uint16_t EEPROM_read_16t (uint16_t uiAddress)                 // ���� ����� ������ ��� 16 ���
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


void init_EEprom (void)                                       // ������������� ������, �������� ����������� ��������
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
//------------���������� Nextion--------//
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



