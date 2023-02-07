/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"

extern void disn(unsigned char X,unsigned char Y,unsigned char n);
extern void disn_hanzi(unsigned char X,unsigned char Y,unsigned char n);

void Menu_Adjust(void)
{
    //零序电流�������
    disn_hanzi(0,0,31);
    disn_hanzi(2,0,32);
    disn_hanzi(4,0,6);
    disn_hanzi(6,0,17);
   
    //直流电流ֱ������
    disn_hanzi(0,1,36);
    disn_hanzi(2,1,17);
    disn_hanzi(4,1,6);
    disn_hanzi(6,1,17);
    
    //系统电压
    disn_hanzi(0,2,34);
    disn_hanzi(2,2,35);
    disn_hanzi(4,2,6);
    disn_hanzi(6,2,33);
}
