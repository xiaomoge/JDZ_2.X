#ifndef	EEPROM_H
#define EEPROM_H
//#include "P33ep128gp504.h"

typedef struct{
  unsigned char algorithm;
  unsigned char action;
  unsigned char baud_rate;
  unsigned int neutral_resistor;
  unsigned int pre_ac;
  unsigned int pre_dc;
  unsigned int time_delay;
  unsigned int comm_address;   
    
}eeprom_data;

#define eeprom_new



#ifdef eeprom_new

void Get_Sys_Set(void);
void Put_Sys_Set(void);

#else

#include "syschip.h"
//#define  u8 unsigned char
//#define  u32 unsigned int
#define Uint16 	unsigned int
#define Uchar	unsigned char
#define Uint32  unsigned long int

/*extern int failmemory[40];
extern unsigned int WriteTable[8];
extern unsigned int ReadTable[8];
extern unsigned int ControlByteW;
extern unsigned int ControlByteR;
extern unsigned int AddressH;
extern unsigned int AddressL;
extern unsigned long int i;*/

//void ConfigI2CEEPROM();
//void PageWrite();
//void PageRead();

void Get_Sys_Set(void);
void Put_Sys_Set(void);
#define old 0
//#ifndef old

extern eeprom_data eepromData;
//#endif
#endif


#endif

