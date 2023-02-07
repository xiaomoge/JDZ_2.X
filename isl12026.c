#include "isl12026.h"
#include "utility.h"

#define I2C_SDA_DIR1 LATAbits.LATA8=0//RA8
#define I2C_SDA_DIR0 LATAbits.LATA8=1
#define I2C_SCL_DIR1 LATBbits.LATB4=0//RB4
#define I2C_SCL_DIR0 LATBbits.LATB4=1

#define I2C_SDA_OUT LATAbits.LATA8
#define I2C_SDA_IN  LATAbits.LATA8
#define I2C_SCL_OUT LATBbits.LATB4
#define I2C_SCL_IN  LATBbits.LATB4

//------------------------------------------------------------------------------
unsigned char year=99,month=99,day=99,hour=99,minute=99,second=99;

void init_rtc(void)
{
	I2C_SDA_DIR0;
	I2C_SCL_DIR0;
	Set_Delay(200);
	I2C_SCL_OUT = 0;
	Set_Delay(200);
	I2C_SDA_OUT = 1;
	Set_Delay(200);
}

void Set_Rtc(unsigned int date,unsigned int year,unsigned int month,unsigned int day,unsigned int hour,unsigned int minute,unsigned int second)
{
	unsigned int i;
	//写WEL
	Write_WEL();
	//写RWEL
	Write_RWEL();
	//写开始位
	Write_Start_bit();
	//写SLAVE BYTE
	Write_Rtc(0xde);	
	//读ACK应答��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写WORD ADDRESS 1
	Write_Rtc(0);	
	//读ACK应答�
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写WORD ADDRESS 0
	Write_Rtc(0x30);	
	//读ACK应答��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写second
	Write_Rtc(Change10ToBCD(second));
	//读ACK应答�
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写minute
	Write_Rtc(Change10ToBCD(minute));
	//读ACK应答���
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写hour
	Write_Rtc(Change10ToBCD(hour)|0x80);
	//读ACK应答��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写day
	Write_Rtc(Change10ToBCD(day));
	//读ACK应答��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写month
	Write_Rtc(Change10ToBCD(month));
	//读ACK应答��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写year
	Write_Rtc(Change10ToBCD(year));
	//读ACK应答���
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写date
	Write_Rtc(Change10ToBCD(date));//��,������д0
	//读ACK应答�
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//写停止位
	Write_Stop_bit();
    Reset_Status();
}

void Set_Control(unsigned int address,unsigned int data)
{
	unsigned int i;
	/*写WEL*/
	Write_WEL();
	/*写RWEL*/
	Write_RWEL();
	/*写开始位*/
	Write_Start_bit();
	/*写SLAVE BYTE*/
	Write_Rtc(0xde);	
	/*读ACK应答*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*写WORD ADDRESS 1*/
	Write_Rtc(0);	
	/*读ACK应答*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*写WORD ADDRESS 0*/
	Write_Rtc(address);	
	/*读ACK应答*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*写DATE*/
	Write_Rtc(data);
	/*读ACK应答*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*写停止位*/
	Write_Stop_bit();
	//Reset_Status();
}


void Set_Delay(unsigned int Delay_Num)
{
	unsigned int i;
	for(i = 0; i <= Delay_Num; i++)
	{
		asm(" nop");
	}
}

void Write_Start_bit(void)
{
	I2C_SDA_DIR0;
	I2C_SDA_OUT = 1;
	Set_Delay(200);
	I2C_SCL_OUT = 1;
	Set_Delay(200);
	I2C_SDA_OUT = 0;
	Set_Delay(200);
	I2C_SCL_OUT = 0;
	Set_Delay(200);
}

void Write_Stop_bit(void)
{
	I2C_SDA_DIR0;
	I2C_SDA_OUT = 0;
	Set_Delay(200);
	I2C_SCL_OUT = 1;
	Set_Delay(200);
	I2C_SDA_OUT = 1;
	Set_Delay(200);
	I2C_SCL_OUT = 0;
	Set_Delay(200);
}

void Write_WEL(void)
{
	unsigned int i;
	I2C_SDA_DIR0;
	//д��ʼλ
	Write_Start_bit();
	//дSLAVE BYTE
	Write_Rtc(0xde);	
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//дWORD ADDRESS 1
	Write_Rtc(0);	
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//дWORD ADDRESS 0
	Write_Rtc(0x3f);	
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//дDATE
	Write_Rtc(0x02);
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//дֹͣλ
	Write_Stop_bit();
	//��λ
	//Reset_Status();
}

void Write_RWEL(void)
{
	unsigned int i;
	I2C_SDA_DIR0;
	/*д��ʼλ*/
	Write_Start_bit();
	/*дSLAVE BYTE*/
	Write_Rtc(0xde);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 1*/
	Write_Rtc(0);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 0*/
	Write_Rtc(0x3f);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дDATE*/
	Write_Rtc(0x06);
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дֹͣλ*/
	Write_Stop_bit();
}

void Reset_Status(void)
{
	unsigned int i;
	I2C_SDA_DIR0;
	/*д��ʼλ*/
	Write_Start_bit();
	/*дSLAVE BYTE*/
	Write_Rtc(0xde);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 1*/
	Write_Rtc(0);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 0*/
	Write_Rtc(0x3f);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дDATE*/
	Write_Rtc(0x0);
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дֹͣλ*/
	Write_Stop_bit();
}

void Write_Rtc(unsigned int Rtc_Var)
{
	int i;
	I2C_SDA_DIR0;
	for(i = 7; i >= 0; i--)
	{
		I2C_SDA_OUT = (Rtc_Var >> i) & 1;
		Set_Delay(200);
		I2C_SCL_OUT = 1;
		Set_Delay(200);
		I2C_SCL_OUT = 0;
		Set_Delay(200);
	}
}

unsigned int Read_Rtc_ACK(void)
{
	unsigned int i;
	I2C_SDA_DIR1;
	Set_Delay(200);
	I2C_SCL_OUT = 1;
	Set_Delay(200);
	i = I2C_SDA_IN;
	Set_Delay(200);
	I2C_SCL_OUT = 0;
	Set_Delay(200);
	return(i);
}

void Write_Rtc_ACK(void)
{
	I2C_SDA_DIR0;
	I2C_SDA_OUT = 0;
	Set_Delay(200);
	I2C_SCL_OUT = 1;
	Set_Delay(200);
	I2C_SCL_OUT = 0;
	Set_Delay(200);
}

unsigned int Read_Rtc_Data(unsigned int slave_address1,unsigned int slave_address0,unsigned int address1,unsigned int address0)
{
	unsigned int i,j,var1[8];
	/*д��ʼλ*/
	Write_Start_bit();
	/*дSLAVE BYTE*/
	Write_Rtc(slave_address1);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 1*/
	Write_Rtc(address1);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 0*/
	Write_Rtc(address0);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*д��ʼλ*/
	Write_Start_bit();
	/*дSLAVE BYTE*/
	Write_Rtc(slave_address0);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*��DATA*/
	I2C_SDA_DIR1;
	j = 0;
	for(i = 0; i < 8; i++)
	{
		I2C_SCL_OUT = 1;
		Set_Delay(200);
		var1[i] = I2C_SDA_IN;
		j = j | (var1[i] << (7 - i));
		Set_Delay(200);
		I2C_SCL_OUT = 0;
		Set_Delay(200);
	}
	/*дֹͣλ*/
	Write_Stop_bit();
	return(j);
}

/*void Page_Read_Eeprom(unsigned int address1,unsigned int address0,unsigned int length)
{
	unsigned int i,j,k,var1[8];
	//д��ʼλ
	Write_Start_bit();
	//дSLAVE BYTE
	Write_Rtc(0xae);	
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//дWORD ADDRESS 1
	Write_Rtc(address1);	
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//дWORD ADDRESS 0
	Write_Rtc(address0);	
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	//д��ʼλ
	Write_Start_bit();
	//дSLAVE BYTE
	Write_Rtc(0xaf);	
	//��ACKӦ��
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	I2C_SDA_DIR1;
	k = 0;
	for(i = 0; i < 8; i++)
	{
		I2C_SCL_OUT = 1;
		Set_Delay(200);
		var1[i] = I2C_SDA_IN;
		k = k | (var1[i] << (7 - i));
		Set_Delay(200);
		I2C_SCL_OUT = 0;
		Set_Delay(200);
	}
	RTC_ReadData[0] = k;
	//��DATA
	for(j = 1; j < length; j++)
	{
		//дACKӦ��
		Write_Rtc_ACK();
		I2C_SDA_DIR1;
		k = 0;
		for(i = 0; i < 8; i++)
		{
			I2C_SCL_OUT = 1;
			Set_Delay(200);
			var1[i] = I2C_SDA_IN;
			k = k | (var1[i] << (7 - i));
			Set_Delay(200);
			I2C_SCL_OUT = 0;
			Set_Delay(200);
		}
		RTC_ReadData[j] = k;
	}
	//дֹͣλ
	Write_Stop_bit();
}*/

void Byte_Write(unsigned int Slave_Address,unsigned int address1,unsigned int address0,unsigned int data)
{
	unsigned int i;
	/*дWEL*/
	Write_WEL();
	/*дRWEL*/
	Write_RWEL();
	/*д��ʼλ*/
	Write_Start_bit();
	/*дSLAVE BYTE*/
	Write_Rtc(Slave_Address);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 1*/
	Write_Rtc(address1);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 0*/
	Write_Rtc(address0);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дDATE*/
	Write_Rtc(data);
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дֹͣλ*/
	Write_Stop_bit();
}

void Byte_Write_Eeprom(unsigned int address1,unsigned address0,unsigned int data)
{
	/*дWEL*/
	Write_WEL();
	/*дRWEL*/
	Write_RWEL();
	/*дEEPROM*/
	Byte_Write(0xae,address1,address0,data);
	/*��ʱ10MS*/
	Delay_10ms();
	/*��STATUS*/
	Reset_Status();
}

void Page_Write_Eeprom(unsigned int address1,unsigned int address0,unsigned int *data,unsigned int length)
{
	unsigned int i,j;
	/*дWEL*/
	Write_WEL();
	/*дRWEL*/
	Write_RWEL();
		/*д��ʼλ*/
	Write_Start_bit();
	/*дSLAVE BYTE*/
	Write_Rtc(0xae);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 1*/
	Write_Rtc(address1);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	/*дWORD ADDRESS 0*/
	Write_Rtc(address0);	
	/*��ACKӦ��*/
	i = Read_Rtc_ACK();
	if(i == 1)
	{
		while(1);
	}
	for(j = 0; j < length; j++)
	{
		/*дDATE*/
		Write_Rtc(*(data+j));
		/*��ACKӦ��*/
		i = Read_Rtc_ACK();
	}
	/*дֹͣλ*/
	Write_Stop_bit();
	/*��ʱ10MS*/
	Delay_10ms();
	/*��STATUS*/
	Reset_Status();
}

void Delay_10ms(void)
{
	unsigned int i;
	for(i = 0; i < 10000; i++)
	{
		Set_Delay(200);
	}
}

void Get_Rtc(void)
{
	year = ChangeBCDTo10(Read_Rtc_Data(0xde,0xdf,0,0x35));
	month = ChangeBCDTo10(Read_Rtc_Data(0xde,0xdf,0,0x34));
	day = ChangeBCDTo10(Read_Rtc_Data(0xde,0xdf,0,0x33));
	hour = ChangeBCDTo10(Read_Rtc_Data(0xde,0xdf,0,0x32)&0x7F);
	minute = ChangeBCDTo10(Read_Rtc_Data(0xde,0xdf,0,0x31));
	second = ChangeBCDTo10(Read_Rtc_Data(0xde,0xdf,0,0x30));
}

/*void Put_Leak_Log_Time(unsigned char log_save_times)
{
	unsigned int page;

	Get_Rtc();

	RTC_WriteData[7]=20;
	RTC_WriteData[6]=0;
	RTC_WriteData[5]=year;
	RTC_WriteData[4]=month;
	RTC_WriteData[3]=day;
	RTC_WriteData[2]=hour;
	RTC_WriteData[1]=minute;
	RTC_WriteData[0]=second;

	if(log_save_times==1)
		page = 8;
	else
		page = 8;
	Page_Write_Eeprom(0,0,&RTC_WriteData[0],8);
}*/
