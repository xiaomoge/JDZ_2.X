/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "menu_running.h"
#include "utility.h"
#include "oled.h"
extern void disn(unsigned char X,unsigned char Y,unsigned char n);

extern float resultVdc;
extern float resultVac;
extern float resultVl;

void Running_Menu(void)
{
    //Ko
    disn(6,2,43);disn(7,2,2);
    
    //mA��·��
    disn(14,1,77);disn(15,1,33);
    
    //mA
    disn(14,2,77);disn(15,2,33);
    
    //V
    disn(14,3,54);
}

void RealStatus(void)
{
    //闭锁
    disn_hanzi(0,0,52);
    disn_hanzi(2,0,53);
}

void RealData_Sampling(void)
{
    char set_temp[5] ;//= "12345"
        
    //交流
    float2char(3, 1, resultVac, set_temp);
    disn(9,1,set_temp[0]-48);disn(10,1,set_temp[1]-48);disn(11,1,set_temp[2]-48);disn(12,1,24);disn(13,1,set_temp[4]-48);
    
    //直流
    float2char(2, 2, resultVdc, set_temp);
    disn(9,2,set_temp[0]-48);disn(10,2,set_temp[1]-48);disn(11,2,24);disn(12,2,set_temp[3]-48);disn(13,2,set_temp[4]-48);
    
    //线电压
    float2char(3, 1, resultVl, set_temp);
    disn(9,3,set_temp[0]-48);disn(10,3,set_temp[1]-48);disn(11,3,set_temp[2]-48);disn(12,3,24);disn(13,3,set_temp[4]-48);
    
	//float temp1,temp2,temp3;
	//double temp4,temp5,temp6;

	//unsigned char k;

	//times=0;			
	//IER |= M_INT1;
	//PieCtrl.PIEIER1.bit.INTx7=1;
	//EINT;   // Enable Global interrupt INTM
	//ERTM;	// Enable Global realtime interrupt DBGM
	//while(1)
	//{
	//	if(times>=128)
	//		break;		//AD×ª»»´ÎÊý´ïµ½128£¬ÔòÖÕÖ¹
	//}
	//PieCtrl.PIEIER1.bit.INTx7=0;  
}

void RealData_Lock(void)
{
    char set_temp[5] ;//= "12345"
    
    //直流
    float2char(1, 1, resultVdc, set_temp);
    disn(4,1,set_temp[0]-48);disn(5,1,24);disn(6,1,set_temp[2]-48);
    
    //交流
    float2char(3, 1, resultVac, set_temp);
    disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);
    
    //电阻
    
    //时间
    
}

void RealData_Break(void)
{
    char set_temp[5] ;//= "12345"
    
    //直流
    float2char(1, 1, resultVdc, set_temp);
    disn(4,1,set_temp[0]-48);disn(5,1,24);disn(6,1,set_temp[2]-48);
    
    //交流
    float2char(3, 1, resultVac, set_temp);
    disn(12,1,set_temp[0]-48);disn(13,1,set_temp[1]-48);disn(14,1,set_temp[2]-48);
    
    //电阻
}