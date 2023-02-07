/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "menu_action.h"
#include "utility.h"
#include "oled.h"
//extern void disn(unsigned char X,unsigned char Y,unsigned char n);

extern float resultVdc;
extern float resultVac;

void Alarm_Lock(void)
{
    //漏电闭锁
    disn_hanzi(4,0,13);
    disn_hanzi(6,0,6);
    disn_hanzi(8,0,52);
    disn_hanzi(10,0,53);
    
    //直流�����
    disn_hanzi(0,1,36);
    disn_hanzi(2,1,17);
    
    //交流
    disn_hanzi(8,1,37);
    disn_hanzi(10,1,17);
    
    //电阻
    disn_hanzi(0,2,6);
    disn_hanzi(2,2,30);
    
    //时间
    disn_hanzi(8,2,22);
    disn_hanzi(10,2,23);
}

void Alarm_Break(void)
{
    //漏电跳闸
    disn_hanzi(4,0,13);
    disn_hanzi(6,0,6);
    disn_hanzi(8,0,26);
    disn_hanzi(10,0,27);
    
    //直流�����
    disn_hanzi(0,1,36);
    disn_hanzi(2,1,17);
    
    //交流
    disn_hanzi(8,1,37);
    disn_hanzi(10,1,17);
    
    //电阻
    disn_hanzi(0,2,6);
    disn_hanzi(2,2,30);
}