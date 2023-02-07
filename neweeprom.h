#ifndef	NEWEEPROM_H
#define NEWEEPROM_H
//#include "syschip.h"
//#define  u8 unsigned char
//#define  u32 unsigned int
#define Uint16 	unsigned int
#define Uchar	unsigned char
//#define Uint32  unsigned long int
//
//typedef struct{
//  unsigned char algorithm;
//  unsigned char action;
//  unsigned char baud_rate;
//  unsigned int neutral_resistor;
//  unsigned int pre_ac;
//  unsigned int pre_dc;
//  unsigned int time_delay;
//  unsigned int comm_address;   
//    
//}eeprom_data;
//
//extern  eeprom_data eepromData;
//eepr// <editor-fold defaultstate="collapsed" desc="comment">
// </editor-fold>
//om_data eepromData_set;
void Get_Sys_Sets(void);
void Put_Sys_Sets(void);
void I2C_delay_ms(int x);
void I2C_delay_s(int x);
void I2C_init(void);
void I2C_start(void);
void I2C_stop(void);
void I2C_readmode(void);
void I2C_writemode(void);
Uint16 I2C_ackRead(void);
void I2C_ackWrite(void);
void I2C_ackNo(void);
void I2C_writedata(Uchar ch);
Uchar I2C_readdata(void);
void I2C_writeByte(Uchar addrH,Uchar addrL,Uchar data);
Uchar I2C_readByte(Uchar addrH,Uchar addrL);
void I2C_writePage(Uchar addrH,Uchar addrL,Uchar* src,Uint16 length);
void I2C_readPage(Uchar addrH,Uchar addrL,Uchar* recv,Uint16 length);

#endif


