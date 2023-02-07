/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "eeprom.h" 
#include "stdbool.h"
void System_Page_Data(void);
void Move_System_Position(unsigned char no,unsigned char mode);

extern int calculation(int data, int bitNo, unsigned char flag_cursor);
extern void float2char(unsigned int g, unsigned int l, float number, char *str);

extern void disn(unsigned char X,unsigned char Y,unsigned char n);
extern void disn_hanzi(unsigned char X,unsigned char Y,unsigned char n);
extern void disn2(unsigned char X,unsigned char Y,unsigned char n);
extern void disn_hanzi2(unsigned char X,unsigned char Y,unsigned char n);

extern float DCActionCompareValue;
extern float ACActionCompareValue;

extern eeprom_data eepromData;
//extern unsigned char algorithm;
//extern unsigned char action;
//extern unsigned char baud_rate;
//extern unsigned int neutral_resistor;
//extern unsigned int pre_ac;
//extern unsigned int pre_dc;
//extern unsigned int time_delay;
//extern unsigned int comm_address;
//---------------------------------------------
//extern 
eeprom_data eepromData_set;
//unsigned char algorithm_set;
//unsigned char action_set;
//unsigned char baud_rate_set;
//unsigned int neutral_resistor_set;
//unsigned int pre_ac_set;
//unsigned int pre_dc_set;
//unsigned int time_delay_set;
//unsigned int comm_address_set;

void Load_Sys_Setting(void)
{
	eepromData_set.algorithm = eepromData.algorithm;
	eepromData_set.action = eepromData.action;
	eepromData_set.baud_rate = eepromData.baud_rate;
	eepromData_set.neutral_resistor = eepromData.neutral_resistor;
	eepromData_set.pre_ac =eepromData.pre_ac;
	eepromData_set.pre_dc = eepromData.pre_dc;
	eepromData_set.time_delay = eepromData.time_delay;
	eepromData_set.comm_address = eepromData.comm_address;
	eepromData_set.comm_address = eepromData.comm_address;
}

void Save_Sys_Setting(void)
{
	eepromData.algorithm = eepromData_set.algorithm;
	eepromData.action = eepromData_set.action ;
	eepromData.baud_rate = eepromData_set.baud_rate;
	eepromData.neutral_resistor = eepromData_set.neutral_resistor;
	eepromData.pre_ac = eepromData_set.pre_ac;
	eepromData.pre_dc = eepromData_set.pre_dc;
	eepromData.time_delay = eepromData_set.time_delay;
	eepromData.comm_address = eepromData_set.comm_address;
//	eepromData.comm_address = eepromData_set.comm_address;
    
    DCActionCompareValue = ((float)eepromData.pre_dc)/10.0;
    ACActionCompareValue = (float)eepromData.pre_ac;
}

void Menu_Setting(void)
{
    //交流
    disn_hanzi(0,0,37);
    disn_hanzi(2,0,17);
    
    //直流
    disn_hanzi(8,0,36);
    disn_hanzi(10,0,17);
        
    //延时
    disn_hanzi(0,1,28);
    disn_hanzi(2,1,22);  

    //内阻
    disn_hanzi(8,1,29);
    disn_hanzi(10,1,30); 
    
    //算法
    disn_hanzi(0,2,38);
    disn_hanzi(2,2,39);
    
    //动作
    disn_hanzi(8,2,9);
    disn_hanzi(10,2,54);  
    
    //BTL
    disn(0,3,34);
    disn(1,3,52);
    disn(2,3,44);
    
    //地址
    disn_hanzi(8,3,55);
    disn_hanzi(10,3,56);  
    
    System_Page_Data();
}

void System_Page_Data(void)
{
    char set_temp[5];
    
    //交流
    float2char(3, 0, (float)eepromData_set.pre_ac, set_temp);
    disn(4,0,set_temp[0]-48);disn(5,0,set_temp[1]-48);disn(6,0,set_temp[2]-48);
    
    //直流
    float2char(2, 0, (float)eepromData_set.pre_dc, set_temp);
    disn(12,0,set_temp[0]-48);disn(13,0,24);disn(14,0,set_temp[1]-48);
    
    //延时
    float2char(4, 0, (float)eepromData_set.time_delay, set_temp);
    disn(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
    
    //内阻
    float2char(4, 0, (float)eepromData_set.neutral_resistor, set_temp);
    disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
    
	//动作设置
	switch(eepromData_set.action)
	{
		case 0:
			//报警
			disn_hanzi(12,2,57);
            disn_hanzi(14,2,58);  
			break;
		case 1:  
            //跳闸
			disn_hanzi(12,2,26);
            disn_hanzi(14,2,27);
			break;
		default:
			break;
	}

	//算法
	switch(eepromData_set.algorithm)
	{
		case 0: 
			//直流
            disn_hanzi(4,2,36);
            disn_hanzi(6,2,17);
			break;
		case 1:
			//交流
            disn_hanzi(4,2,37);
            disn_hanzi(6,2,17);
			break;
		case 2:
			//交直
            disn_hanzi(4,2,37);
            disn_hanzi(6,2,36);
			break;
		case 3:
			//自动
            disn_hanzi(4,2,8);
            disn_hanzi(6,2,9);
			break;
		default:
			break;
	}

	//波特率设置
	switch(eepromData_set.baud_rate)
	{
		case 0:
			//1200
            disn(4,3,1);disn(5,3,2);disn(6,3,0);disn(7,3,0);
			break;
		case 1:
			//2400
            disn(4,3,2);disn(5,3,4);disn(6,3,0);disn(7,3,0);
			break;
		case 2:
			//4800
            disn(4,3,4);disn(5,3,8);disn(6,3,0);disn(7,3,0);
			break;
		case 3:
			//9600
            disn(4,3,9);disn(5,3,6);disn(6,3,0);disn(7,3,0);
			break;
		default:
			break;
	}
    
    //通讯地址
    float2char(4, 0, (float)eepromData_set.comm_address, set_temp);
    disn(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
}

void Select_Setting_System(unsigned char no)
{
    if(no==0)
        Move_System_Position(7, 0);
    else
        Move_System_Position(no-1, 0);
    
    Move_System_Position(no, 1);
}

void Move_System_Position(unsigned char no,unsigned char mode)
{
	char set_temp[5] ;//= "12345"

    switch(no)
    {
        case 0://交流
            if(mode==1)
            {
                float2char(3, 0, (float)eepromData_set.pre_ac, set_temp);
                disn2(4,0,set_temp[0]-48);disn2(5,0,set_temp[1]-48);disn2(6,0,set_temp[2]-48);
            }
            else
            {
                float2char(3, 0, (float)eepromData_set.pre_ac, set_temp);
                disn(4,0,set_temp[0]-48);disn(5,0,set_temp[1]-48);disn(6,0,set_temp[2]-48);
            }
            break;
        case 1://直流
            if(mode==1)
            {
                float2char(2, 0, (float)eepromData_set.pre_dc, set_temp);
                disn2(12,0,set_temp[0]-48);disn2(13,0,24);disn2(14,0,set_temp[1]-48);
            }
            else
            {
                float2char(2, 0, (float)eepromData_set.pre_dc, set_temp);
                disn(12,0,set_temp[0]-48);disn(13,0,24);disn(14,0,set_temp[1]-48);
            }
            break;
        case 2://延时
            if(mode==1)
            {
                float2char(4, 0, (float)eepromData_set.time_delay, set_temp);
                disn2(4,1,set_temp[0]-48);disn2(5,1,set_temp[1]-48);disn2(6,1,set_temp[2]-48);disn2(7,1,set_temp[3]-48);
            }
            else
            {
                float2char(4, 0, (float)eepromData_set.time_delay, set_temp);
                disn(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
            }
            break;
        case 3://内阻
            if(mode==1)
            {
                float2char(4, 0, (float)eepromData_set.neutral_resistor, set_temp);
                disn2(12,1,set_temp[0]-48);disn2(13,1,set_temp[1]-48);disn2(14,1,set_temp[2]-48);disn2(15,1,set_temp[3]-48);
            }
            else
            {
                float2char(4, 0, (float)eepromData_set.neutral_resistor, set_temp);
                disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
            }
            break;
        case 4://算法
            if(eepromData_set.algorithm==0)
            {
                //直流
                if(mode==1)
                {
                   disn_hanzi2(4,2,36);
                   disn_hanzi2(6,2,17);
                }
                else
                {
                   disn_hanzi(4,2,36);
                   disn_hanzi(6,2,17);
                }
            }
            else if(eepromData_set.algorithm==1)
            {
                //交流
                if(mode==1)
                {
                    disn_hanzi2(4,2,37);
                    disn_hanzi2(6,2,17);
                }
                else
                {
                    disn_hanzi(4,2,37);
                    disn_hanzi(6,2,17);
                }
            }
            else if(eepromData_set.algorithm==2)
            {
                //交直
                 if(mode==1)
                {
                    disn_hanzi2(4,2,37);
                    disn_hanzi2(6,2,36);
                }
                else
                {
                    disn_hanzi(4,2,37);
                    disn_hanzi(6,2,36);
                }
            }
            else if(eepromData_set.algorithm==3)
            {
                //自动
                 if(mode==1)
                {
                    disn_hanzi2(4,2,8);
                    disn_hanzi2(6,2,9);
                }
                else
                {
                    disn_hanzi(4,2,8);
                    disn_hanzi(6,2,9);
                }          
            }
            break;
        case 5://动作
            if(eepromData_set.action==0)
            {
                //报警
                if(mode==1)
                {
                    disn_hanzi2(12,2,57);
                    disn_hanzi2(14,2,58);
                }
                else
                {
                    disn_hanzi(12,2,57);
                    disn_hanzi(14,2,58);
                }
            }
            else if(eepromData_set.action==1)
            {
                //跳闸
                if(mode==1)
                {
                    disn_hanzi2(12,2,26);
                    disn_hanzi2(14,2,27);  
                }
                else
                {
                    disn_hanzi(12,2,26);
                    disn_hanzi(14,2,27);   
                }
            }
            break;
        case 6://波特率
            if(eepromData_set.baud_rate==0)
            {
                if(mode==1)
                {
                    disn2(4,3,1);disn2(5,3,2);disn2(6,3,0);disn2(7,3,0);//1200
                }
                else
                {
                    disn(4,3,1);disn(5,3,2);disn(6,3,0);disn(7,3,0);//1200
                }
            }
            else if(eepromData_set.baud_rate==1)
            {
                if(mode==1)
                {
                    disn2(4,3,2);disn2(5,3,4);disn2(6,3,0);disn2(7,3,0);//2400
                }
                else
                {
                    disn(4,3,2);disn(5,3,4);disn(6,3,0);disn(7,3,0);//2400
                }
            }
            else if(eepromData_set.baud_rate==2)
            {
                if(mode==1)
                {
                    disn2(4,3,4);disn2(5,3,8);disn2(6,3,0);disn2(7,3,0);//4800
                }
                else
                {
                    disn(4,3,4);disn(5,3,8);disn(6,3,0);disn(7,3,0);//4800
                }
            }
            else if(eepromData_set.baud_rate==3)
            {
                if(mode==1)
                {
                    disn2(4,3,9);disn2(5,3,6);disn2(6,3,0);disn2(7,3,0);//9600
                }
                else
                {
                    disn(4,3,9);disn(5,3,6);disn(6,3,0);disn(7,3,0);//9600
                }
            }
            break;
        case 7:
            //通讯地址
            if(mode==1)
            {
                float2char(4, 0, (float)eepromData_set.comm_address, set_temp);
                disn2(12,3,set_temp[0]-48);disn2(13,3,set_temp[1]-48);disn2(14,3,set_temp[2]-48);disn2(15,3,set_temp[3]-48);
            }
            else
            {
                float2char(4, 0, (float)eepromData_set.comm_address, set_temp);
                disn(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
            }
            break;
        default:
            break;
    }
}

void modify_system_setting(unsigned char system_no, unsigned char system_cursor, unsigned char plus_minus)
{
	char set_temp[5];
	
	if(system_no==0)//交流
	{
		eepromData_set.pre_ac = calculation(eepromData_set.pre_ac,system_cursor+1,plus_minus);//0-减,1-加

		float2char(3, 0, eepromData_set.pre_ac, set_temp);
        if(system_cursor==0)
        {
            disn(4,0,set_temp[0]-48);disn(5,0,set_temp[1]-48);disn2(6,0,set_temp[2]-48);
        }
        else if(system_cursor==1)
        {
            disn(4,0,set_temp[0]-48);disn2(5,0,set_temp[1]-48);disn(6,0,set_temp[2]-48);
        }
        else if(system_cursor==2)
        {
            disn2(4,0,set_temp[0]-48);disn(5,0,set_temp[1]-48);disn(6,0,set_temp[2]-48);
        }
	}
	
    if(system_no==1)//直流
	{
		eepromData_set.pre_dc = calculation(eepromData_set.pre_dc,system_cursor+1,plus_minus);//0-减,1-加
        float2char(2, 0, (float)eepromData_set.pre_dc, set_temp);
        if(system_cursor==0)
        {
             disn(12,0,set_temp[0]-48);disn(13,0,24);disn2(14,0,set_temp[1]-48);
        }
        else if(system_cursor==1)
        {
             disn2(12,0,set_temp[0]-48);disn(13,0,24);disn(14,0,set_temp[1]-48);
        }
	}
    
	if(system_no==2)//延时
	{
		eepromData_set.time_delay = calculation(eepromData_set.time_delay,system_cursor+1,plus_minus);//0-减,1-加
        float2char(4, 0, (float)eepromData_set.time_delay, set_temp);
        if(system_cursor==0)
        {
            disn(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn2(7,1,set_temp[3]-48);
        }
        else if(system_cursor==1)
        {
            disn(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn2(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
        }
        else if(system_cursor==2)
        {
            disn(4,1,set_temp[0]-48);disn2(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
        }
        else if(system_cursor==3)
        {
            disn2(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
        }
	}
	if(system_no==3)//内阻
	{
		eepromData_set.neutral_resistor = calculation(eepromData_set.neutral_resistor,system_cursor+1,plus_minus);//0-减,1-加
        float2char(4, 0, (float)eepromData_set.neutral_resistor, set_temp);
        if(system_cursor==0)
        {
            disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn2(15,1,set_temp[3]-48);
        }
        else if(system_cursor==1)
        {
            disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn2(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
        }
        else if(system_cursor==2)
        {
            disn(12,1,set_temp[0]-48);disn2(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
        }
        else if(system_cursor==3)
        {
            disn2(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
        }
	}
	if(system_no==7)//地址
	{
		eepromData_set.comm_address = calculation(eepromData_set.comm_address,system_cursor+1,plus_minus);//0-减,1-加
        float2char(4, 0, (float)eepromData_set.comm_address, set_temp);
        if(system_cursor==0)
        {
            disn(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn2(15,3,set_temp[3]-48);
        }
        else if(system_cursor==1)
        {
            disn(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn2(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
        }
        else if(system_cursor==2)
        {
            disn(12,3,set_temp[0]-48);disn2(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
        }
        else if(system_cursor==3)
        {
            disn2(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
        }
	}
}

void Move_System_Cursor(unsigned char system_no, unsigned char system_cursor)
{
	char set_temp[5] ;//= "12345"

    switch(system_no)
    {
        case 0://交流
            float2char(3, 0, (float)eepromData_set.pre_ac, set_temp);
            if(system_cursor==0)
            {
                disn(4,0,set_temp[0]-48);disn(5,0,set_temp[1]-48);disn2(6,0,set_temp[2]-48);
            }
            else if(system_cursor==1)
            {
                disn(4,0,set_temp[0]-48);disn2(5,0,set_temp[1]-48);disn(6,0,set_temp[2]-48);
            }
            else if(system_cursor==2)
            {
                disn2(4,0,set_temp[0]-48);disn(5,0,set_temp[1]-48);disn(6,0,set_temp[2]-48);
            }
            break;
        case 1://直流
            float2char(2, 0, (float)eepromData_set.pre_dc, set_temp);
            if(system_cursor==0)
            {
                 disn(12,0,set_temp[0]-48);disn(13,0,24);disn2(14,0,set_temp[1]-48);
            }
            else if(system_cursor==1)
            {
                 disn2(12,0,set_temp[0]-48);disn(13,0,24);disn(14,0,set_temp[1]-48);
            }
            break;
        case 2://延时
            float2char(4, 0, (float)eepromData_set.time_delay, set_temp);
            if(system_cursor==0)
            {
                disn(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn2(7,1,set_temp[3]-48);
            }
            else if(system_cursor==1)
            {
                disn(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn2(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
            }
            else if(system_cursor==2)
            {
                disn(4,1,set_temp[0]-48);disn2(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
            }
            else if(system_cursor==3)
            {
                disn2(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn(6,1,set_temp[2]-48);disn(7,1,set_temp[3]-48);
            }
            break;
        case 3://内阻
            float2char(4, 0, (float)eepromData_set.neutral_resistor, set_temp);
            if(system_cursor==0)
            {
                disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn2(15,1,set_temp[3]-48);
            }
            else if(system_cursor==1)
            {
                disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn2(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
            }
            else if(system_cursor==2)
            {
                disn(12,1,set_temp[0]-48);disn2(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
            }
            else if(system_cursor==3)
            {
                disn2(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);disn(15,1,set_temp[3]-48);
            }
            break;
        case 7://通讯地址
            float2char(4, 0, (float)eepromData_set.comm_address, set_temp);
            if(system_cursor==0)
            {
                disn(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn2(15,3,set_temp[3]-48);
            }
            else if(system_cursor==1)
            {
                disn(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn2(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
            }
            else if(system_cursor==2)
            {
                disn(12,3,set_temp[0]-48);disn2(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
            }
            else if(system_cursor==3)
            {
                disn2(12,3,set_temp[0]-48);disn(13,3,set_temp[1]-48);disn(14,3,set_temp[2]-48);disn(15,3,set_temp[3]-48);
            }
            break;
        default:
            break;
    }
}

void Algorithm_Setting_Enter(void)
{
    if(eepromData_set.algorithm<3)
        eepromData_set.algorithm++;
    else
        eepromData_set.algorithm=0;

    switch(eepromData_set.algorithm)
    {
        case 0:
            eepromData_set.algorithm = 0;
            disn_hanzi2(4,2,36);//直流
            disn_hanzi2(6,2,17);
            break;
        case 1:
            eepromData_set.algorithm = 1;
            disn_hanzi2(4,2,37);//交流
            disn_hanzi2(6,2,17);
            break;
        case 2:
            eepromData_set.algorithm = 2;
            disn_hanzi2(4,2,37);//交直
            disn_hanzi2(6,2,36);
            break;
        case 3:
            eepromData_set.algorithm = 3;
            disn_hanzi2(4,2,8);//自动
            disn_hanzi2(6,2,9);
            break;
        default:
            break;
    }
}

void Action_Setting_Enter(void)
{
    if(eepromData_set.action==0)
        eepromData_set.action=1;
    else
        eepromData_set.action=0;
    
    switch(eepromData_set.action)
    {
        case 0:
            eepromData_set.action = 0;
            disn_hanzi2(12,2,57);//报警
            disn_hanzi2(14,2,58); 
            break;
        case 1:
            eepromData_set.action = 1;
            disn_hanzi2(12,2,26);//跳闸
            disn_hanzi2(14,2,27);
            break;
        default:
            break;
    }
}

void Baud_Rate_Setting_Enter(void)
{ 
    if(eepromData_set.baud_rate<3)
        eepromData_set.baud_rate++;
    else
        eepromData_set.baud_rate=0;

    switch(eepromData_set.baud_rate)
    {
        case 0:
            eepromData_set.baud_rate = 0;
            disn2(4,3,1);disn2(5,3,2);disn2(6,3,0);disn2(7,3,0);//1200
            break;
        case 1:
            eepromData_set.baud_rate = 1;
            disn2(4,3,2);disn2(5,3,4);disn2(6,3,0);disn2(7,3,0);//2400
            break;
        case 2:
            eepromData_set.baud_rate = 2;
            disn2(4,3,4);disn2(5,3,8);disn2(6,3,0);disn2(7,3,0);//4800
            break;
        case 3:
            eepromData_set.baud_rate = 3;
            disn2(4,3,9);disn2(5,3,6);disn2(6,3,0);disn2(7,3,0);//9600
            break;
        default:
            break;
    }
}

void system_setting_3(void)
{
    /*
	menu_class = 3;	//置菜单层级3
	setting_system_flag = 1;//系统设置标志位

	//打开光标

	//关闭当前反白
	Move_System_Position(system_setting_ver, system_setting_hor, 1);

	system_cursor = 1;
	Move_System_Cursor(system_setting_hor, system_setting_ver, 1);
    */
}

void Judge_Is_Save_Page(void)
{    
    disn_hanzi(6,0,41);
    disn_hanzi(8,0,42);

    //"-----------------------"
    disn(0,1,23);disn(1,1,23);disn(2,1,23);disn(3,1,23);disn(4,1,23);disn(5,1,23);disn(6,1,23);disn(7,1,23);
    disn(8,1,23);disn(9,1,23);disn(10,1,23);disn(11,1,23);disn(12,1,23);disn(13,1,23);disn(14,1,23);disn(15,1,23);

	//保存
	disn_hanzi(2,2,66);disn_hanzi(4,2,67);

	//不保存
	disn_hanzi(8,2,65);disn_hanzi(10,2,66);disn_hanzi(12,2,67);
}

void Select_Save(bool x)
{
  
	if(!x)
	{
		//保存
		disn_hanzi(2,2,66);disn_hanzi(4,2,67);
		//不保存
		disn_hanzi2(8,2,65);disn_hanzi2(10,2,66);disn_hanzi2(12,2,67);
	}else {
        		//保存
		disn_hanzi2(2,2,66);disn_hanzi2(4,2,67);
		//不保存
		disn_hanzi(8,2,65);disn_hanzi(10,2,66);disn_hanzi(12,2,67);
    }
}
//void error_list(unsigned char listNu){
//    listNu=0;
//    disn_hanzi2(4,0,61); disn_hanzi2(6,0,62);//编号 
//    disn_hanzi2(12,0,63); disn_hanzi2(14,0,64);//总数
//    
//    disn_hanzi2(4,1,59); disn_hanzi2(6,1,60);//类型    
//    disn_hanzi2(12,1,37); disn_hanzi2(14,1,17);//交流
//    
//    disn_hanzi2(4,2,36); disn_hanzi2(6,2,17);//直流
//    disn_hanzi2(12,2,6); disn_hanzi2(14,2,30);//电阻
//    
//    disn_hanzi2(4,3,22); disn_hanzi2(6,3,23);//时间
////    disn_hanzi2(12,2,6); disn_hanzi2(14,2,30);//电阻
//        
//}
//
//void zero_setting(void){
//    
//    disn_hanzi2(4,0,31); disn_hanzi2(6,0,32);//零序电流
//    disn_hanzi2(8,0,6); disn_hanzi2(10,0,17);
//    disn_hanzi2(11,0,26);
//
//    disn_hanzi2(4,2,36); disn_hanzi2(6,2,17);////直流电流
//    disn_hanzi2(8,2,6); disn_hanzi2(10,2,17);
//    disn_hanzi2(11,2,26);
//    
//    disn_hanzi2(4,4,34); disn_hanzi2(6,4,35);// //系统电压
//    disn_hanzi2(8,4,6); disn_hanzi2(10,4,33);
//    disn_hanzi2(11,4,26);    
//    
//   
//    
//}