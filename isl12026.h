#ifndef ISL12026_H
#define	ISL12026_H

//#include "P33ep128gp504.h"
#include "syschip.h"
void Set_Rtc(unsigned int date,unsigned int year,unsigned int month,unsigned int day,unsigned int hour,unsigned int minute,unsigned int second);
void Set_Delay(unsigned int Delay_Num);
void Write_Rtc(unsigned int Rtc_Var);
unsigned int Read_Rtc_ACK(void);
void Write_Rtc_ACK(void);
void Write_Start_bit(void);
void Write_Stop_bit(void);
unsigned int Read_Rtc_Data(unsigned int slave_address1,unsigned int slave_address0,unsigned int address1,unsigned int address0);
void Page_Read_Eeprom(unsigned int address1,unsigned int address0,unsigned int length);
void Write_WEL(void);
void Write_RWEL(void);
void Reset_Status(void);
void Byte_Write(unsigned int Slave_Address,unsigned int address1,unsigned int address0,unsigned int data);
void Byte_Write_Eeprom(unsigned int address1,unsigned address0,unsigned int data);
void Page_Write_Eeprom(unsigned int address1,unsigned int address0,unsigned int *data,unsigned int length);
void Delay_10ms(void);
//unsigned char Change10ToBCD(unsigned char data);
//unsigned char ChangeBCDTo10(unsigned char data);

#endif