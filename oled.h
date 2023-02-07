#ifndef OLED_H
#define	OLED_H
//#include"P33ep32mc202.h"
//#include "P33ep128gp504.h"
#include "syschip.h"


#define  u8 unsigned char
#define  u32 unsigned int
#define OLED_CMD  0	//д����
#define OLED_DATA 1	//д����
#define OLED_MODE 0

//#define OLED_CS LATBbits.LATB4 //Ƭѡ
#define OLED_CS LATBbits.LATB8 //Ƭѡ

#define OLED_RST LATAbits.LATA4//��λ

//#define OLED_DC LATBbits.LATB9//����/�������
#define OLED_DC LATBbits.LATB7//����/�������

//#define OLED_SCL LATBbits.LATB7//ʱ�� D0��SCLK?
#define OLED_SCL LATCbits.LATC5//ʱ�� D0��SCLK?

//#define OLED_SDIN LATBbits.LATB8//D1��MOSI�� ����
#define OLED_SDIN LATCbits.LATC4//D1��MOSI�� ����


#define OLED_CS_Clr()  OLED_CS=0
#define OLED_CS_Set()  OLED_CS=1

#define OLED_RST_Clr() OLED_RST=0
#define OLED_RST_Set() OLED_RST=1

#define OLED_DC_Clr() OLED_DC=0
#define OLED_DC_Set() OLED_DC=1

#define OLED_SCLK_Clr() OLED_SCL=0
#define OLED_SCLK_Set() OLED_SCL=1

#define OLED_SDIN_Clr() OLED_SDIN=0
#define OLED_SDIN_Set() OLED_SDIN=1;
#define SIZE 16
#define XLevelL		0x02
#define XLevelH		0x10
#define Max_Column	128
#define Max_Row		64
#define	Brightness	0xFF
#define X_WIDTH 	128
#define Y_WIDTH 	64

void OLED_WR_Byte(u8 dat,u8 cmd);
void OLED_Set_Pos(unsigned char x, unsigned char y);
void OLED_Display_On(void);
void OLED_Display_Off(void);
void OLED_Clear(void);
void disn(unsigned char X,unsigned char Y,unsigned char n);
void disn_hanzi(unsigned char X,unsigned char Y,unsigned char n);
void disn2(unsigned char X,unsigned char Y,unsigned char n);
void disn_hanzi2(unsigned char X,unsigned char Y,unsigned char n);
void picture(void);
void OLED_Init(void);

void OLED_WR_Byte2(u8 dat,u8 cmd);


#endif	/* OLED_H */

