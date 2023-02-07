/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "menu_trial.h"
#include "utility.h"

extern void disn(unsigned char X,unsigned char Y,unsigned char n);
extern void disn_hanzi(unsigned char X,unsigned char Y,unsigned char n);

extern float resultVdc;
extern float resultVac;
extern float resultVl;
extern float testResultVdc;
extern float testI1;
extern float testI2;

void Menu_Trial(void)
{
    //试验数据
    disn_hanzi(4,0,20);disn_hanzi(6,0,21);disn_hanzi(8,0,64);disn_hanzi(10,0,71);

    //I系
    disn(0,1,41);disn_hanzi(2,1,34);
    //I支
    disn(8,1,41);disn_hanzi(10,1,72);
    
    //直流
    disn_hanzi(0,2,36);disn_hanzi(2,2,17);
    //内阻
    disn_hanzi(8,2,29);disn_hanzi(10,2,30);
    
    //结果正常
    disn_hanzi(0,3,48);disn_hanzi(2,3,49);disn_hanzi(4,3,50);disn_hanzi(6,3,51);
}

void TrialData(void)
{
    char set_temp[5] ;//= "12345"
    
    //I系
    float2char(2, 1, testI1, set_temp);
    disn(4,1,set_temp[0]-48);disn(5,1,set_temp[1]-48);disn(6,1,24);disn(7,1,set_temp[2]-48);
    //I支
    //disn(12,1,0);disn(13,1,0);disn(14,1,0);
    
    //直流
    float2char(1, 2, testResultVdc, set_temp);
    disn(4,2,set_temp[0]-48);disn(6,2,24);disn(5,2,set_temp[1]-48);disn(7,2,set_temp[3]-48);//disn(8,2,set_temp[4]-48);
    //内阻
    //disn(12,2,0);disn(13,2,0);disn(14,2,0);
    
    //正常
}