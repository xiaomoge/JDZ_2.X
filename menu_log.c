/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "oled.h"

void Menu_Log(void)
{
    //编号
    disn_hanzi(0,0,61);disn_hanzi(2,0,62);
    //总数
    disn_hanzi(9,0,63);disn_hanzi(11,0,64);
    
    //类型
    disn_hanzi(0,1,59);disn_hanzi(2,1,60); 
    //直流
    disn_hanzi(9,1,36);disn_hanzi(11,1,17);
    
    //交流
    disn_hanzi(0,2,37);disn_hanzi(2,2,17);
    //电阻
    disn_hanzi(9,2,6);disn_hanzi(11,2,30); 
    
    //时间
    disn_hanzi(0,3,22);disn_hanzi(2,3,23);
}

void Clear_Success(void)
{
	disn_hanzi(3,2,68);//报告已清除
	disn_hanzi(5,2,69);
	disn_hanzi(7,2,70);
	disn_hanzi(9,2,18);
    disn_hanzi(11,2,19);
}