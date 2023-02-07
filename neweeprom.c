/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */
#include "neweeprom.h"
//#include "xc.h"
#include "eeprom.h"
#include "isl12026.h"
#include "utility.h"
#include "delay.h"
#include "i2c.h"
#include "string.h"
//#define Uint16 	unsigned int
//#define Uchar	unsigned char
 extern eeprom_data eepromData;
#define dataLen 14
#define Uint32  unsigned long int
#define WP		LATCbits.LATC3//WP is the Write Protect pin
#define SCL		LATBbits.LATB4                             
#define SDA		LATAbits.LATA8                             
#define DELAY 	I2C_delay_ms(72)
#define DELAY1ms 	I2C_delay_ms(2)
//void Change_Two_Byte(unsigned int a);

void ackTimeOut(void);
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
//Uchar temp_short[2];
//Uchar send_set[32];

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

//void ackTimeOut(void){
//    while(1){
//    I2C_delay_ms(2);
//    }
//}
//
Uchar dataTransfer[dataLen];
void Get_Sys_Sets(void)
{
    //njknn/
//	double temp5,temp6,temp0;
    
    I2C_init();
    memset(dataTransfer,0,dataLen);
//	I2C_readPage(0x02,0xE0,dataTransfer,dataLen);
    I2C_readPage(0x02,0x20+dataLen,dataTransfer,dataLen);
    eepromData.algorithm = dataTransfer[0];
    eepromData.action = dataTransfer[1];
	eepromData.baud_rate = dataTransfer[2];
    
//	eepromData.neutral_resistor = recv_data[4]*256 + recv_data[5];
    eepromData.neutral_resistor = dataTransfer[4];
    eepromData.neutral_resistor = (eepromData.neutral_resistor<<8)|dataTransfer[5];
    
//	eepromData.pre_ac = recv_data[6]*256 + recv_data[7];
    eepromData.pre_ac = dataTransfer[6];//*256 + recv_data[7];
    eepromData.pre_ac = ( eepromData.pre_ac<<8)|dataTransfer[7];
    
//	eepromData.pre_dc = recv_data[8]*256 + recv_data[9];
	eepromData.pre_dc = dataTransfer[8];//*256 + recv_data[9];
	eepromData.pre_dc = (eepromData.pre_dc<<8)| dataTransfer[9];  
    
//	eepromData.time_delay = recv_data[10]*256 + recv_data[11];
 	eepromData.time_delay = dataTransfer[10];//*256 + recv_data[11];
    eepromData.time_delay = (eepromData.time_delay<<8)| dataTransfer[11];
    
//    eepromData.comm_address = recv_data[12]*256 + recv_data[13];
    eepromData.comm_address = dataTransfer[12];//*256 + recv_data[13];
    eepromData.comm_address = (eepromData.comm_address<<8)| dataTransfer[13];
    /*algorithm=1;
    action=1;
    comm_address=1234;
    baud_rate=0;
    
	neutral_resistor=6000;
    pre_ac=100;
    pre_dc=50;
    time_delay=1000;
    
    DCActionCompareValue = ((float)pre_dc)/10.0;
    ACActionCompareValue = (float)pre_ac;*/
        //times
    if(dataTransfer[0] == 0xff){
//        dataTransfer[0]=0;
        
        eepromData.algorithm = 1;//jiaoliu
        eepromData.action = 2;//tiaozha 
        eepromData.baud_rate =3;
        eepromData.neutral_resistor=4;
        eepromData.pre_ac = 5;
        eepromData.pre_dc = 6;
        eepromData.time_delay = 7;
        eepromData.comm_address = 8;
//        Put_Sys_Sets();
    } 
//    else{
//    }
    
}

//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
//-------------------------------------------------------
void Put_Sys_Sets(void)
{
	//Uchar send_set[32];

	I2C_init();
	WP=0; 
    memset(dataTransfer,0,dataLen);
	delay_ms(1000);//需要加延时，否则可能会无法存储

	/////////////////////////////////////
	/*action_setting = 0;
	monitor_overcurrent = 0;
	neutral_resistor = 985;
	leak_degree = 3;
	monitor_system = 0;
	current_protect = 15;
	monitor_temp = 100;
	comm_address = 1;
	baud_rate = 1;
	password = 1111;*/
	/////////////////////////////////////

	dataTransfer[0] = eepromData.algorithm;
	dataTransfer[1] = eepromData.action;
	dataTransfer[2] = eepromData.baud_rate;
    dataTransfer[3] = 0;
    
//	Change_Two_Byte(eepromData.neutral_resistor);
    dataTransfer[5] = eepromData.neutral_resistor;
    dataTransfer[4] = eepromData.neutral_resistor>>8;
//	send_set[4]= temp_short[0];
//	send_set[5]= temp_short[1];	
	
//	Change_Two_Byte(eepromData.pre_ac);
	dataTransfer[7] = eepromData.pre_ac;//temp_short[0];
	dataTransfer[6] = eepromData.pre_ac>>8;//temp_short[1];

//	Change_Two_Byte(eepromData.pre_dc);
	dataTransfer[9] = eepromData.pre_dc;//temp_short[0];
	dataTransfer[8] =eepromData.pre_dc>>8;//temp_short[1];

//	Change_Two_Byte(eepromData.time_delay);
	dataTransfer[11] = eepromData.time_delay;//temp_short[0];
	dataTransfer[10] = eepromData.time_delay>>8;//temp_short[1];

//	Change_Two_Byte(eepromData.comm_address);
	dataTransfer[13] = eepromData.comm_address;//temp_short[0];
	dataTransfer[12] = eepromData.comm_address>>8;//temp_short[1];

//    send_set[14] = 0;
//	send_set[15] = 0;
//	send_set[16] = 0;
//	send_set[17] = 0;
//	send_set[18] = 0;
//	send_set[19] = 0;
//	send_set[20] = 0;
//	send_set[21] = 0;
//	send_set[22] = 0;
//	send_set[23] = 0;
//	send_set[24] = 0;
//	send_set[25] = 0;
//	send_set[26] = 0;
//	send_set[27] = 0;
//	send_set[28] = 0;
//	send_set[29] = 0;
//	send_set[30] = 0;
//	send_set[31] = 0;

//	I2C_writePage(0x02,0xE0,dataTransfer,dataLen);
	I2C_writePage(0x02,0x20+dataLen,dataTransfer,dataLen);
    DELAY;DELAY;DELAY;DELAY;
	WP=1;
}

//-----------------------------------------------------------
//-------------------------Leak Log--------------------------
//-----------------------------------------------------------
void Get_Leak_Page_Set(void)
{
	I2C_init();
	/*last_leak_page=0;
	total_leak_page=0;*/
	last_leak_page = I2C_readByte(0x02,0xA0);
	total_leak_page = I2C_readByte(0x02,0xA1);
}

void Put_Leak_Page_Set(void)
{
	I2C_init();

	WP=0;
	/*last_leak_page=0;
	total_leak_page=0;*/
	I2C_writeByte(0x02,0xA0,last_leak_page);
	I2C_writeByte(0x02,0xA1,total_leak_page);
	WP=1;
}

void Reset_Leak_Page(void)
{
	I2C_init();

	WP=0;
	last_leak_page=0;
	total_leak_page=0;
	I2C_writeByte(0x02,0xA0,last_leak_page);
	I2C_writeByte(0x02,0xA1,total_leak_page);
	WP=1;
}

void Get_Leak_Log(void)
{
	Uchar recv_data[32];
	unsigned char i;
	char set_outputI[6];

	unsigned char addrH, addrL;
	unsigned char temp;

	temp = call_leak_page/8;
	addrH = 0x06 + temp;
	temp = call_leak_page%8;
	addrL = 0x00 + (temp-1)*0x20;

	I2C_init();
	//----------------------------------------------
	//I2C_readPage(0x06,0x00,recv_data,32);
	I2C_readPage(addrH,addrL,recv_data,32);

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
	for(i=0;i<6;i++){
		set_outputI[i] = recv_data[i+18];
    }
	RLr_log = char2float(set_outputI);

	//报告编号
	leak_page = recv_data[24];

	//总报告数
	//total_leak_page = recv_data[25];
	total_leak_page = I2C_readByte(0x02,0xA1);

	//漏电类型
	leak_trouble_type_log = recv_data[26];
}

/*
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
		I2C_init();
		WP=0;
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
		for(i=0;i<6;i++){
			send_set[i+18] = set_outputI[i];
        }

		//报告编号
		leak_page=last_leak_page;
		send_set[24] = leak_page;

		//总报告数---所有页面共用
		send_set[25] = total_leak_page;

		//漏电类型
		//leak_trouble_type=1;
		send_set[26] = leak_trouble_type;

		//漏电支路
		//leak_branch_No=1;
		//send_set[27] = leak_branch_No;

		//I2C_writePage(0x06,0x00,send_set,32);
		I2C_writePage(addrH,addrL,send_set,32);

		WP=1;
	}
}
*/
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

	I2C_init();
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

	WP=1;
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

	I2C_init();
	//----------------------------------------------
	//I2C_readPage(addrH,addrL,recv_data,32);

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
	year_log2=I2C_readByte(addrH,addrL+13);
}

//-----------------------------------------------------------
//-----------------------------------------------------------
//void Change_Two_Byte(unsigned int a)
//{
//	temp_short[1] = (unsigned int)a;
//	a>>=8;
//	temp_short[0] = (unsigned int)a;
//}

//-----------------------------------------------------------
//-----------------------------------------------------------
void I2C_init(void)
{
//	SCL=0;
//	DELAY;
//	SDA=1;
//	DELAY;
//    SDA=0;
}

void I2C_start(void)
{
	SDA=1;
//	DELAY;
	SCL=1;
	DELAY;
	SDA=0;
	DELAY;
	SCL=0;
	DELAY;
}

void I2C_stop(void)
{
	SDA=0;
    SCL=0;
	DELAY;
	SCL=1;
	DELAY;
	SDA=1;
	DELAY;
//	SCL=0;
//	DELAY;
}

void I2C_readmode(void)
{
	//GpioMuxRegs.GPADIR.bit.GPIOA15=0;//输入input
    TRISAbits.TRISA8 = 1;
}

void I2C_writemode(void)
{
    //GpioMuxRegs.GPADIR.bit.GPIOA15=1;//输出output 
    TRISAbits.TRISA8 = 0;
}

Uint16 I2C_ackRead(void)
{
	Uint16 ack;
//    SCL=1;
	I2C_readmode();
//	DELAY;
	SCL=1;
	DELAY;
	ack=SDA;
//	DELAY;
	SCL=0;
	DELAY;
	I2C_writemode();
 
	return ack;	
}

void I2C_ackWrite(void)
{
	SDA=0;
//	DELAY;
	SCL=1;
	DELAY;
	SCL=0;
	DELAY;
}

void I2C_ackNo(void)
{
	SDA=1;
//	DELAY1ms;
	SCL=1;
	DELAY;
	SCL=0;
	DELAY;
}

void I2C_writedata(Uchar ch)
{
	Uchar i;
	for(i=0;i<8;i++)
	{
      SDA = ((ch<<i)&0x80)?1:0;
      asm("NOP");asm("NOP");asm("NOP");asm("NOP");
//		DELAY;
      
		SCL=1;
//        	 if((ch<<i)&0x80)
//	    	SDA=1;   /*判断发送位 一位一位地发送*/
//        else 
//        	SDA=0; 
        
		DELAY;
		SCL=0;
		DELAY;		
	}
//    SDA=0;
}

Uchar I2C_readdata()
{
	Uchar i; 
	Uchar data=0; 
	I2C_readmode();
//	DELAY;  
	for(i=0;i<8;i++)
	{
		
//		DELAY;
		SCL=1;
		DELAY;
//		if(SDA==1){
//			data=data+1; /*读数据位,接收的数据位放入data?*/
//        }
        data |= SDA?1:0;
//		DELAY;
        data=data<<1;
		SCL=0;
		DELAY;	
	}
	I2C_writemode(); //置数据线
   	return (data);
}

void I2C_writeByte(Uchar addrH,Uchar addrL,Uchar data)
{
	Uint16 j;

	I2C_start();
	I2C_writedata(0xA0);
	if(I2C_ackRead())
	{
//		while(1);
	}		
	I2C_writedata(addrH);
	if(I2C_ackRead())
	{
//		while(1);
	}	
	I2C_writedata(addrL);
	if(I2C_ackRead())
	{
//		while(1);
	}	
	I2C_writedata(data);
	if(I2C_ackRead())
	{
//		while(1);
	}	
	I2C_stop();

	for(j=0;j<30000;j++);//等待写入完成
}

Uchar I2C_readByte(Uchar addrH,Uchar addrL)
{
	Uchar data=0;
	I2C_start();
	I2C_writedata(0xA0);
	if(I2C_ackRead())
	{
//		while(1);
	}
	I2C_writedata(addrH);
	if(I2C_ackRead())
	{
//		while(1);
	}
	I2C_writedata(addrL);
	if(I2C_ackRead())
	{
//		while(1);
	}
	I2C_start();
	I2C_writedata(0xA1);
	if(I2C_ackRead())
	{
//		while(1);
	}
	data=I2C_readdata();
	I2C_ackNo();
	DELAY;
	I2C_stop();
	return data;
}

void I2C_writePage(Uchar addrH,Uchar addrL,Uchar* src,Uint16 length)
{
	Uint16 i;//,j;
	I2C_start();
	I2C_writedata(0xA0);
    I2C_ackRead();
//	if(I2C_ackRead())
//	{
//		while(1);
//	}		
	I2C_writedata(addrH);
    I2C_ackRead();
//	if(I2C_ackRead())
//	{
//		while(1);
//	}	
	I2C_writedata(addrL);
    I2C_ackRead();
//	if(I2C_ackRead())
//	{
//		while(1);
//	}
	for(i=0;i<length;i++)
	{
		I2C_writedata(src[i]);
        I2C_ackRead();
//		if(I2C_ackRead())
//		{
//			while(1);
//		}	
	}
	I2C_stop();//发送停止条件启动内部写周期
    DELAY;DELAY;
//	for(j=0;j<30000;j++);//等写
}

void I2C_readPage(Uchar addrH,Uchar addrL,Uchar* recv,Uint16 length)
{//0x02,0xE0,recv_data,32);
	Uint16 i;
	I2C_start();
	I2C_writedata(0xA0);
    I2C_ackRead();
//	if(I2C_ackRead())
//	{
//		while(1);
//	}
	I2C_writedata(addrH);
    I2C_ackRead();
//	if(I2C_ackRead())
//	{
//		while(1);
//	}
	I2C_writedata(addrL);
    I2C_ackRead();
//	if(I2C_ackRead())
//	{
//		while(1);
//	}
	I2C_start();
	I2C_writedata(0xA1);
    I2C_ackRead();
//	if(I2C_ackRead())
//	{
//		while(1);
//	}
	for(i=0;i<length;i++)
	{
		recv[i]=I2C_readdata();
		I2C_ackWrite();		
	}
//	recv[length-1]=I2C_readdata();
//	I2C_ackNo();
	I2C_stop();
}

void I2C_delay_ms(int x)
{
	unsigned int i;
	for(i = 0; i <= x; i++)
		asm("NOP");
}

void I2C_delay_s(int x)
{
	Uint16 i;
	for(i=0;i<x;i++)
		DELAY;
}