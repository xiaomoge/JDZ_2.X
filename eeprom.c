/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */
#include "eeprom.h"
eeprom_data eepromData;
#ifdef eeprom_new
#include "neweeprom.h"
 void Get_Sys_Set(void){
     Get_Sys_Sets();
 }
void Put_Sys_Set(void){
    Put_Sys_Sets();
}
#else
#include "xc.h"
#include "eeprom.h"
//#include"eeprom(20190601).h"

#include "isl12026.h"
#include "utility.h"
#include "delay.h"
#include  "i2c.h"

#define Uint16 	unsigned int
#define Uchar	unsigned char
#define Uint32  unsigned long int
#define WP		LATCbits.LATC3//WP is the Write Protect pin
#define SCL		LATBbits.LATB4                             
#define SDA		LATAbits.LATA8                             
#define DELAY 	I2C_delay_ms(100)

void Change_Two_Byte(unsigned int a);

//#if old 

//#endif
//unsigned int neutral_resistor;
//unsigned int pre_ac;
//unsigned int pre_dc;
//unsigned int time_delay;
//unsigned int comm_address;

//----------------------------------------------
unsigned char call_leak_page;
unsigned char last_leak_page;
unsigned char total_leak_page;
unsigned char leak_page;

extern unsigned char leak_trouble_type;
//extern unsigned char leak_branch_No;

extern float DCActionCompareValue;
extern float ACActionCompareValue;
//---------------------log---------------------------------
unsigned char leak_branch_No_log;
float U0r_log,IRr_log,RLr_log;

unsigned char leak_trouble_type_log;

extern unsigned char year,month,day,hour,minute,second;

unsigned char year_log1,month_log1,day_log1,hour_log1,minute_log1,second_log1;
unsigned char year_log2,month_log2,day_log2,hour_log2,minute_log2,second_log2;
//----------------------------------------------
Uchar temp_short[2];
Uchar send_set[16];
//Uchar send_set[32] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x0a,0x0b,0x0c,0x0d,0x0e,0x0f};
//Uchar recv_data[16];
//----------------------------------------------
/*int failmemory[40];
unsigned int WriteTable[8]={0x11,0x22,0x33,0x44,0x55,0x66,0x77,0x88};
unsigned int ReadTable[8];
unsigned int ControlByteW=0xa0; //控制字：写准备（器件地址000）
unsigned int ControlByteR=0xa1; //控制字：读准备（器件地址000）
unsigned int AddressH=0x00;     //数据地址高7位
unsigned int AddressL=0x10;     //数据地址低8位*/
unsigned long int i = 0;

unsigned char ControlByteW=0xa0;//Control Byte
unsigned char ControlByteR=0xa1;//Control Byte
//unsigned char HighAdd;			//High Address byte
unsigned char LowAdd, HighAdd;	//Low and High Address byte
unsigned char Data;				//Data Byte
unsigned char Length;			//Length of Bytes to Read
unsigned char PageString[64];	//Array to hold page data to/from EPROM
//----------------------------------------------

/**********************************************************
***********************************************************
EEPROM分配情况:
bianhao     0x00,0x00;  0x00,0x20;  0x00,0x40
bianbi	    0x00,0x60;  0x00,0x80;  0x00,0xA0
shijian     0x00,0xC0;  0x00,0xE0;  0x01,0x00  
zhuangtai   0x01,0x20;  0x01,0x40;

modify      0x01,0x60;  0x01,0x80;  0x01,0xA0;  0x01,0xC0;  0x01,0xE0;  
			0x02,0x80;

metal_trial 0x02,0x00;  0x02,0x20;  0x02,0x40;  0x02,0x60;
Sys_Set     0x02,0xE0;
Adv_Set     0x02,0xC0;

Page_Set    0x02,0xA0;

trial_data_branch 0x03,0x00 ~ 0x03,0xE0
trial_data_branch 0x04,0x00 ~ 0x04,0xE0

leak_page(漏电分析用数据) 0x05,0x00 ~ 0x05,0x20

leak_page  0x06,0x00 ~ 0x06,0xE0   8page 
		   0x07,0x00 ~ 0x07,0xE0   
		   0x08,0x00 ~ 0x08,0xE0
		   0x09,0x00 ~ 0x09,0xE0
		   0x0A,0x00 ~ 0x0A,0xE0
		   0x0B,0x00 ~ 0x0B,0xE0
		   0x0C,0x00 ~ 0x0C,0xE0

system_page0x10,0x00 ~ 0x10,0xE0   8page
		   0x11,0x00 ~ 0x11,0xE0   
		   0x12,0x00 ~ 0x12,0xE0
		   0x13,0x00 ~ 0x13,0xE0
		   0x14,0x00 ~ 0x14,0xE0
		   0x15,0x00 ~ 0x15,0xE0
		   0x16,0x00 ~ 0x16,0xE0

overcurrent_page  	0x17,0x00 ~ 0x17,0xE0   8page
		   			0x18,0x00 ~ 0x18,0xE0   
		   			0x19,0x00 ~ 0x19,0xE0
		   			0x1A,0x00 ~ 0x1A,0xE0
		   			0x1B,0x00 ~ 0x1B,0xE0
		   			0x1C,0x00 ~ 0x1C,0xE0
		   			0x1D,0x00 ~ 0x1D,0xE0

leak_page  0x20,0x00 ~ 0x20,0xE0   8page
(rtc)	   0x21,0x00 ~ 0x21,0xE0   
		   0x22,0x00 ~ 0x22,0xE0
		   0x23,0x00 ~ 0x23,0xE0
		   0x24,0x00 ~ 0x24,0xE0
		   0x25,0x00 ~ 0x25,0xE0
		   0x26,0x00 ~ 0x26,0xE0

temp_page  0x27,0x00 ~ 0x27,0xE0   8page
    	   0x28,0x00 ~ 0x28,0xE0   
		   0x29,0x00 ~ 0x29,0xE0
		   0x2A,0x00 ~ 0x2A,0xE0
		   0x2B,0x00 ~ 0x2B,0xE0
		   0x2C,0x00 ~ 0x2C,0xE0
		   0x2D,0x00 ~ 0x2D,0xE0

***********************************************************
***********************************************************/

/**********************************************************
***********************************************************/
#ifdef old
void Get_Sys_Set(void)
{
    Uchar recv_data[16];
	double temp0;
   
    //ControlByte = 0xA1;
//    LowAdd = 0x00;
    LowAdd=0x7f;	
	HighAdd = 0x00;
    //0xa1 
//    HDByteReadI2C(ControlByteR, HighAdd, LowAdd, recv_data, 16);
//    HDSequentialReadI2C(ControlByteR, HighAdd, LowAdd, recv_data, 16);
//    HDByteReadI2C(ControlByteR, HighAdd, LowAdd, recv_data, 16);
    for(unsigned i=0; i < 16; i++){
       HDByteReadRandomI2C(ControlByteR, HighAdd++, &recv_data[i]); 
    }
//I2C_start();
    //times
    if(recv_data[14] == 0xff){
        recv_data[14]=0;
        
        eepromData.algorithm = 1;//jiaoliu
        eepromData.action = 1;//tiaozha 
        eepromData.baud_rate =3;
        eepromData.neutral_resistor=1000;
        eepromData.pre_ac = 0;
        eepromData.pre_dc = 0;
        eepromData.time_delay = 1000;
        eepromData.comm_address = 1;
        
    }else{
     recv_data[14]=recv_data[14]+1;//power on times +1
     
    eepromData.algorithm = recv_data[0];
    eepromData.action = recv_data[1];
	eepromData.baud_rate = recv_data[2];
    
	temp0 = ((float)recv_data[4])*256 + (float)recv_data[5];
    eepromData.neutral_resistor = (unsigned int)temp0;
    
	temp0 = ((float)recv_data[6])*256 + (float)recv_data[7];
    eepromData.pre_ac = (unsigned int)temp0;
    
	temp0 = ((float)recv_data[8])*256 + (float)recv_data[9];
    eepromData.pre_dc = (unsigned int)temp0;
    
	temp0 = ((float)recv_data[10])*256 + (float)recv_data[11];
    eepromData.time_delay = (unsigned int)temp0;
    
    temp0 = ((float)recv_data[12])*256 + (float)recv_data[13];
    eepromData.comm_address = (unsigned int)temp0;
    
    /*if(Data1 == 0xff){
        pre_ac=999;
    }
    else{
        pre_ac=111;
    }*/
    
    /*algorithm=1;
    action=1;
    comm_address=1234;
    baud_rate=0;
    
	neutral_resistor=6000;
    pre_ac=100;
    pre_dc=50;
    time_delay=1000;*/
    
    DCActionCompareValue = ((float)eepromData.pre_dc)/10.0;
    ACActionCompareValue = (float)eepromData.pre_ac;
    
    }
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void Put_Sys_Set(void)
{
	WP=0;

	delay_ms(1000);//需要加延时，否则可能会无法存储

	/////////////////////////////////////
	/*algorithm=1;
    action=1;
    comm_address=4321;
    baud_rate=1;
	neutral_resistor=6000;
    pre_ac=100;
    pre_dc=50;
    time_delay=1000;*/
	/////////////////////////////////////

	send_set[0] = eepromData.algorithm;
	send_set[1] = eepromData.action;
	send_set[2] = eepromData.baud_rate;
    send_set[3] = 0;
    
	Change_Two_Byte(eepromData.neutral_resistor);
	send_set[4]= temp_short[0];
	send_set[5]= temp_short[1];	
	
	Change_Two_Byte(eepromData.pre_ac);
	send_set[6] = temp_short[0];
	send_set[7] = temp_short[1];

	Change_Two_Byte(eepromData.pre_dc);
	send_set[8] = temp_short[0];
	send_set[9] = temp_short[1];

	Change_Two_Byte(eepromData.time_delay);
	send_set[10] = temp_short[0];
	send_set[11] = temp_short[1];

	Change_Two_Byte(eepromData.comm_address);
	send_set[12] = temp_short[0];
	send_set[13] = temp_short[1];

    send_set[14] = 0;
	send_set[15] = 0;
    
    //ControlByte = 0xA0;
	LowAdd = 0x10;	
	HighAdd = 0x00;
	//Data = 0xAA;
    //Data = 10;
	//Length = 0x01;
    
//    HDPageWriteI2C(ControlByteW, HighAdd, LowAdd, send_set);
//        for(unsigned i=0; i < 16; i++){
//         HDByteWriteRandomI2C(ControlByteW, HighAdd++,  send_set[i]); 
//    }
    //PageWrite();//写入1字节数据
    for(i=0; i<650000; i++); //页写操作后的等待
    for(i=0; i<650000; i++); //页写操作后的等待
    for(i=0; i<650000; i++); //页写操作后的等待
    
	WP=1;
}
#endif
//-----------------------------------------------------------
//-------------------------Leak Log--------------------------
//-----------------------------------------------------------
void Get_Leak_Page_Set(void)
{
	/*I2C_init();
	last_leak_page = I2C_readByte(0x02,0xA0);
	total_leak_page = I2C_readByte(0x02,0xA1);*/
}

void Put_Leak_Page_Set(void)
{
	/*I2C_init();

	WP=0;
	I2C_writeByte(0x02,0xA0,last_leak_page);
	I2C_writeByte(0x02,0xA1,total_leak_page);
	WP=1;*/
}

void Reset_Leak_Page(void)
{
	/*I2C_init();

	WP=0;
	last_leak_page=0;
	total_leak_page=0;
	I2C_writeByte(0x02,0xA0,last_leak_page);
	I2C_writeByte(0x02,0xA1,total_leak_page);
	WP=1;*/
}

void Get_Leak_Log(void)
{
	Uchar recv_data[16];
	unsigned char i;
	char set_outputI[6];

	unsigned char addrH, addrL;
	unsigned char temp;

	temp = call_leak_page/8;
	addrH = 0x06 + temp;
	temp = call_leak_page%8;
	addrL = 0x00 + (temp-1)*0x20;

	//I2C_init();
	//----------------------------------------------
	//I2C_readPage(addrH,addrL,recv_data,32);

	//IRr
	for(i=0;i<6;i++){
        set_outputI[i] = recv_data[i];
    }
	IRr_log = char2float(set_outputI);

	//CLr
	for(i=0;i<6;i++){
		set_outputI[i] = recv_data[i+6];
    }
	//CLr_log = char2float(set_outputI);

	//U0r
	for(i=0;i<6;i++){
		set_outputI[i] = recv_data[i+12];
    }
	U0r_log = char2float(set_outputI);

	//RLr
	//for(i=0;i<6;i++){
	//	set_outputI[i] = recv_data[i+18];
    //}
	//RLr_log = char2float(set_outputI);

	//报告编号
	//leak_page = recv_data[24];

	//总报告数
	//total_leak_page = I2C_readByte(0x02,0xA1);

	//漏电类型
	//leak_trouble_type_log = recv_data[26];
}

void Put_Leak_Log(unsigned char saveNo)
{
	char set_outputI[6];
	unsigned char i;

	unsigned char addrH, addrL;
	unsigned char temp;

	if(saveNo==1)
	{
		if(last_leak_page<50)
			last_leak_page++;
		else
			last_leak_page=1;
	}
	
	if(total_leak_page<50)
		total_leak_page=last_leak_page;
	else
		total_leak_page=50;

	temp = last_leak_page/8;
	addrH = 0x06 + temp;
	temp = last_leak_page%8;
	addrL = 0x00 + (temp-1)*0x20;

	if(saveNo==1)
		Put_Leak_Page_Set();//保存最终页数和总页数

	if(saveNo==2)
	{
		//I2C_init();
		//WP=0;
		delay_ms(1000);//需要加延时，否则可能会无法存储

		//IRr--电阻电流
		//IRr = 99.9;
		//float2char(4,1,IRr,set_outputI);
		for(i=0;i<6;i++){
			send_set[i] = set_outputI[i];
        }

		//CLr--漏电程度
		//CLr = 9.99;
		//float2char(1,4,CLr,set_outputI);
		for(i=0;i<6;i++){
			send_set[i+6] = set_outputI[i];
        }

		//U0r--零序电压
		//U0r = 99.9;
		//float2char(4,1,U0r,set_outputI);
		for(i=0;i<6;i++){
			send_set[i+12] = set_outputI[i];
        }

		//RLr--漏电电阻
		//RLr = 999.9;
		//float2char(4,1,RLr,set_outputI);
		//for(i=0;i<6;i++){
		//	send_set[i+18] = set_outputI[i];
        //}

		//报告编号
		leak_page=last_leak_page;
		//send_set[24] = leak_page;

		//总报告数---所有页面共用
		//send_set[25] = total_leak_page;

		//漏电类型
		//leak_trouble_type=1;
		//send_set[26] = leak_trouble_type;

		//漏电支路
		//leak_branch_No=1;
		//send_set[27] = leak_branch_No;

		//I2C_writePage(addrH,addrL,send_set,32);

		WP=1;
	}
}

//-----------------------------------------------------------
//-----------------------------------------------------------
void Put_Leak_Log_Rtc(unsigned char saveNo)
{
	unsigned char addrH, addrL;
	unsigned char temp;

	temp = last_leak_page/8;
	addrH = 0x20 + temp;
	temp = last_leak_page%8;
	addrL = 0x00 + (temp-1)*0x20;

	//Put_Leak_Page_Set();//保存最终页数和总页数

	/*I2C_init();
	WP=0;
	delay_ms(1000);//需要加延时，否则可能会无法存储

	if(saveNo==1)
	{
		I2C_writeByte(addrH,addrL,second);
		I2C_writeByte(addrH,addrL+1,minute);
		I2C_writeByte(addrH,addrL+2,hour);
		I2C_writeByte(addrH,addrL+3,day);
		I2C_writeByte(addrH,addrL+4,month);
		I2C_writeByte(addrH,addrL+5,year);
		I2C_writeByte(addrH,addrL+6,0);
		I2C_writeByte(addrH,addrL+7,20);
	}
	else if(saveNo==2)
	{
		I2C_writeByte(addrH,addrL+8,second);
		I2C_writeByte(addrH,addrL+9,minute);
		I2C_writeByte(addrH,addrL+10,hour);
		I2C_writeByte(addrH,addrL+11,day);
		I2C_writeByte(addrH,addrL+12,month);
		I2C_writeByte(addrH,addrL+13,year);
		I2C_writeByte(addrH,addrL+14,0);
		I2C_writeByte(addrH,addrL+15,20);
	}

	WP=1;*/
}

void Get_Leak_Log_Rtc(void)
{
	//Uchar recv_data[32];
	unsigned char addrH, addrL;
	unsigned char temp;

	temp = call_leak_page/8;
	addrH = 0x20 + temp;
	temp = call_leak_page%8;
	addrL = 0x00 + (temp-1)*0x20;

	/*I2C_init();
	//----------------------------------------------

	second_log1=I2C_readByte(addrH,addrL);
	minute_log1=I2C_readByte(addrH,addrL+1);
	hour_log1=I2C_readByte(addrH,addrL+2);
	day_log1=I2C_readByte(addrH,addrL+3);
	month_log1=I2C_readByte(addrH,addrL+4);
	year_log1=I2C_readByte(addrH,addrL+5);
	
	second_log2=I2C_readByte(addrH,addrL+8);
	minute_log2=I2C_readByte(addrH,addrL+9);
	hour_log2=I2C_readByte(addrH,addrL+10);
	day_log2=I2C_readByte(addrH,addrL+11);
	month_log2=I2C_readByte(addrH,addrL+12);
	year_log2=I2C_readByte(addrH,addrL+13);*/
}

//-----------------------------------------------------------
//-----------------------------------------------------------
void Change_Two_Byte(unsigned int a)
{
	temp_short[1] = (unsigned char)a;
	a>>=8;
	temp_short[0] = (unsigned char)a;
}

/*
//I2CEEPROM配置子程序
void ConfigI2CEEPROM()
{
    //SRbits.IPL = 7;	//关闭所有中断
    I2C2CONbits.I2CEN = 1; //启动I2C模块
    I2C2BRG = 0x02f; //I2C波特率400 kHz
}

//向24LC256中写入8个字节的数据
void PageWrite()
{
    I2C2CONbits.SEN = 1; //发送启动位
    while(I2C2CONbits.SEN == 1); //等待启动操作完成
    I2C2TRN = ControlByteW; //发送控制字：写准备
    while(I2C2STATbits.TRSTAT == 1); //等待发送完成
    I2C2TRN = AddressH;
    while(I2C2STATbits.TRSTAT == 1); //等待发送完成
	I2C2TRN = AddressL;
    while(I2C2STATbits.TRSTAT == 1); //等待发送完成
    for(i=0; i<8; i++)
	{
		I2C2TRN = WriteTable[i];
        while(I2C2STATbits.TRSTAT == 1); //等待发送完成
    }
    I2C2CONbits.PEN = 1; //发送停止位
    while(I2C2CONbits.PEN == 1); //等待停止操作完成
}

//从24LC256中读出8个字节的数据
void PageRead()
{
    I2C2CONbits.SEN = 1; //发送启动位
    while(I2C2CONbits.SEN == 1); //等待启动操作完成
    I2C2TRN = ControlByteW; //发送控制字：写准备
    //while(I2C1STATbits.TRSTAT == 1); //等待发送完成
    while(I2C2STATbits.TRSTAT == 1); //等待发送完成
    I2C1TRN = AddressH;
    while(I2C2STATbits.TRSTAT == 1); //等待发送完成
	I2C2TRN = AddressL;
    while(I2C2STATbits.TRSTAT == 1); //等待发送完成
    I2C2CONbits.RSEN = 1; //发送重启动位
    while(I2C2CONbits.RSEN == 1); //等待重启动操作完成
    I2C2TRN = ControlByteR; //发送控制字：读准备
    while(I2C2STATbits.TRSTAT == 1); //等待发送完成
    for(i=0; i<8; i++)
	{
        I2C2CONbits.RCEN = 1; //接收数据使能
        while(I2C2STATbits.RBF == 0);
        ReadTable[i] = I2C2RCV;
        I2C2CONbits.ACKDT = 0;
        if (i==7)
        	I2C2CONbits.ACKDT = 1; //最后一字节则不发ACK
        I2C2CONbits.ACKEN = 1;
        while(I2C2CONbits.ACKEN == 1);
    }
    I2C2CONbits.PEN = 1; //发送停止位
    while(I2C2CONbits.PEN == 1); //等待停止操作完成
}
 * */

#endif