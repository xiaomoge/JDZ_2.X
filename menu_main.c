/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "menu_main.h"

extern void disn_hanzi(unsigned char X,unsigned char Y,unsigned char n);
extern void disn2(unsigned char X,unsigned char Y,unsigned char n);
extern void disn_hanzi2(unsigned char X,unsigned char Y,unsigned char n);

void Menu_Main(unsigned char no)
{
    //主菜单
    disn_hanzi(5,0,43);
    disn_hanzi(7,0,44);
    disn_hanzi(9,0,45);
        
    //设置
    if(no == 0)
    {
        disn_hanzi2(2,1,41);
        disn_hanzi2(4,1,42);
    }
    else
    {
        disn_hanzi(2,1,41);
        disn_hanzi(4,1,42);
    }
    
    //时间
    if(no == 1)
    {
        disn_hanzi2(10,1,22);
        disn_hanzi2(12,1,23);
    }
    else
    {
        disn_hanzi(10,1,22);
        disn_hanzi(12,1,23);
    }
    
    //故障
    if(no == 2)
    {
        disn_hanzi2(2,2,24);
        disn_hanzi2(4,2,25);
    }
    else
    {
        disn_hanzi(2,2,24);
        disn_hanzi(4,2,25);
    }
    
    //清除
    if(no == 3)
    {
        disn_hanzi2(10,2,18);
        disn_hanzi2(12,2,19);
    }
    else
    {
        disn_hanzi(10,2,18);
        disn_hanzi(12,2,19);
    }
    
    //试验
    if(no == 4)
    {
        disn_hanzi2(2,3,20);
        disn_hanzi2(4,3,21);
    }
    else
    {
        disn_hanzi(2,3,20);
        disn_hanzi(4,3,21);
    }
    
    //校零
    if(no == 5)
    {
        disn_hanzi2(10,3,40);
        disn_hanzi2(12,3,31);
    }
    else
    {
        disn_hanzi(10,3,40);
        disn_hanzi(12,3,31);
    }
}

void Select_Main_Menu(unsigned char no)//0-5
{
    if(no==0)
        Move_Position(5, 0);
    else
        Move_Position(no-1, 0);
    
    Move_Position(no, 1);
}

void Move_Position(unsigned char no, unsigned char mode)
{
    switch(no)
    {
        case 0://设置
            if(mode==0)
            {
                disn_hanzi(2,1,41);
                disn_hanzi(4,1,42);
            }
            else
            {
                disn_hanzi2(2,1,41);
                disn_hanzi2(4,1,42);
            }
            break;
        case 1://时间
            if(mode==0)
            {
                disn_hanzi(10,1,22);
                disn_hanzi(12,1,23);
            }
            else
            {
                disn_hanzi2(10,1,22);
                disn_hanzi2(12,1,23);
            }
            break;
        case 2://故障
            if(mode==0)
            {
                disn_hanzi(2,2,24);
                disn_hanzi(4,2,25);
            }
            else
            {
                disn_hanzi2(2,2,24);
                disn_hanzi2(4,2,25);
            }
            break;
        case 3://清除
            if(mode==0)
            {
                disn_hanzi(10,2,18);
                disn_hanzi(12,2,19);
            }
            else
            {
                disn_hanzi2(10,2,18);
                disn_hanzi2(12,2,19);
            }
            break;
        case 4://试验
            if(mode==0)
            {
                disn_hanzi(2,3,20);
                disn_hanzi(4,3,21);
            }
            else
            {
                disn_hanzi2(2,3,20);
                disn_hanzi2(4,3,21);
            }
            break;
        case 5://校零
            if(mode==0)
            {
                disn_hanzi(10,3,40);
                disn_hanzi(12,3,31);
            }
            else
            {
                disn_hanzi2(10,3,40);
                disn_hanzi2(12,3,31);
            }
            break;
        default:
            break;
    }
}
