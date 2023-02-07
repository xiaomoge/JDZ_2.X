/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "welcome.h"
#include "oled.h"


void Welcome_Page(void)
{
    //JDZ-2
    disn(5,0,42);disn(6,0,36);disn(7,0,58);disn(8,0,23);disn(9,0,2);
    
    //IT漏电断路器��·��
    disn(2,1,41);disn(3,1,52);disn_hanzi(4,1,13);disn_hanzi(6,1,6);disn_hanzi(8,1,14);disn_hanzi(10,1,15);disn_hanzi(12,1,16);
    
    //溧阳市福沃特���
    disn_hanzi(2,2,0);disn_hanzi(4,2,1);disn_hanzi(6,2,2);disn_hanzi(8,2,3);disn_hanzi(10,2,4);disn_hanzi(12,2,5);
    
    //电力自动化公司���˾
    disn_hanzi(1,3,6);disn_hanzi(3,3,7);disn_hanzi(5,3,8);disn_hanzi(7,3,9);disn_hanzi(9,3,10);disn_hanzi(11,3,11);disn_hanzi(13,3,12);
}