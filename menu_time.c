/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */

#include "xc.h"
#include "menu_time.h"
#include "oled.h"

extern void float2char(unsigned int g, unsigned int l, float number, char *str);
extern int calculation(int data, int bitNo, unsigned char flag_cursor);

unsigned char year_set=99,month_set=99,day_set=99,hour_set=99,minute_set=99,second_set=99;
extern unsigned char year,month,day,hour,minute,second;

extern unsigned char time_cursor;

void Setting_Time_Page(void)
{
    char set_temp[5];
    
    //时间
	//Display_English_16(110,6,"2013-11-18-10-20-00");
	//Get_Rtc();
	year_set = year;
	month_set = month;
	day_set = day;
	hour_set = hour;
	minute_set = minute;
	second_set = second;

    disn(0,1,2);disn(1,1,0);//"20"
	
	float2char(2, 0, year_set, set_temp);
    disn2(2,1,set_temp[0]-48);disn(3,1,set_temp[1]-48);

    disn(4,1,23);//"-"

	float2char(2, 0, month_set, set_temp);
	disn(5,1,set_temp[0]-48);disn(6,1,set_temp[1]-48);

	disn(7,1,23);//"-"

	float2char(2, 0, day_set, set_temp);
	disn(8,1,set_temp[0]-48);disn(9,1,set_temp[1]-48);

	disn(10,1,23);//"-"

	float2char(2, 0, hour_set, set_temp);
	disn(11,1,set_temp[0]-48);disn(12,1,set_temp[1]-48);

	disn(13,1,23);//"-"
	
	float2char(2, 0, minute_set, set_temp);
	disn(14,1,set_temp[0]-48);disn(15,1,set_temp[1]-48);

	//disn(5,2,23);//"-"
	
	//float2char(2, 0, second_set, set_temp);
	//disn(6,2,set_temp[0]-48);disn(7,2,set_temp[1]-48);
}

void Move_Time_Cursor(unsigned char time_cursor)
{
    char set_temp[5] ;//= "12345"
    
    switch(time_cursor)
    {
        case 0:
            float2char(2, 0, (float)minute_set, set_temp);
            disn(14,1,set_temp[0]-48);disn(15,1,set_temp[1]-48);
            
            float2char(2, 0, (float)year_set, set_temp);
            disn2(2,1,set_temp[0]-48);disn(3,1,set_temp[1]-48);
            break;
        case 1:
            float2char(2, 0, (float)year_set, set_temp);
            disn(2,1,set_temp[0]-48);disn2(3,1,set_temp[1]-48);
            break;
        case 2:
            float2char(2, 0, (float)year_set, set_temp);
            disn(2,1,set_temp[0]-48);disn(3,1,set_temp[1]-48);
            
            float2char(2, 0, (float)month_set, set_temp);
            disn2(5,1,set_temp[0]-48);disn(6,1,set_temp[1]-48);
            break;
        case 3:
            float2char(2, 0, (float)month_set, set_temp);
            disn(5,1,set_temp[0]-48);disn2(6,1,set_temp[1]-48);
            break;
        case 4:
            float2char(2, 0, (float)month_set, set_temp);
            disn(5,1,set_temp[0]-48);disn(6,1,set_temp[1]-48);
            
            float2char(2, 0, (float)day_set, set_temp);
            disn2(8,1,set_temp[0]-48);disn(9,1,set_temp[1]-48);
            break;
        case 5:
            float2char(2, 0, (float)day_set, set_temp);
            disn(8,1,set_temp[0]-48);disn2(9,1,set_temp[1]-48);
            break;
        case 6:
            float2char(2, 0, (float)day_set, set_temp);
            disn(8,1,set_temp[0]-48);disn(9,1,set_temp[1]-48);
            
            float2char(2, 0, (float)hour_set, set_temp);
            disn2(11,1,set_temp[0]-48);disn(12,1,set_temp[1]-48);
            break;
        case 7:
            float2char(2, 0, (float)hour_set, set_temp);
            disn(11,1,set_temp[0]-48);disn2(12,1,set_temp[1]-48);
            break;
        case 8:
            float2char(2, 0, (float)hour_set, set_temp);
            disn(11,1,set_temp[0]-48);disn(12,1,set_temp[1]-48);
            
            float2char(2, 0, (float)minute_set, set_temp);
            disn2(14,1,set_temp[0]-48);disn(15,1,set_temp[1]-48);
            break;
        case 9:
            float2char(2, 0, (float)minute_set, set_temp);
            disn(14,1,set_temp[0]-48);disn2(15,1,set_temp[1]-48);
            break;
        default:
            break;
    }
}

void modify_time_setting(unsigned char system_cursor, unsigned char plus_minus)
{
    char set_temp[5];

    switch(time_cursor)
    {
        case 0:
            year_set = calculation(year_set,time_cursor+2,plus_minus);//0-减,1-加
            float2char(2, 0, (float)year_set, set_temp);
            disn2(2,1,set_temp[0]-48);disn(3,1,set_temp[1]-48);
            break;
        case 1:
            year_set = calculation(year_set,time_cursor,plus_minus);//0-减,1-加
            float2char(2, 0, (float)year_set, set_temp);
            disn(2,1,set_temp[0]-48);disn2(3,1,set_temp[1]-48);
            break;
        case 2:
            month_set = calculation(month_set,time_cursor+2-2,plus_minus);//0-减,1-加
            float2char(2, 0, (float)month_set, set_temp);
            disn2(5,1,set_temp[0]-48);disn(6,1,set_temp[1]-48);
            break;
        case 3:
            month_set = calculation(month_set,time_cursor-2,plus_minus);//0-减,1-加
            float2char(2, 0, (float)month_set, set_temp);
            disn(5,1,set_temp[0]-48);disn2(6,1,set_temp[1]-48);
            break;
        case 4:
            day_set = calculation(day_set,time_cursor+2-4,plus_minus);//0-减,1-加
            float2char(2, 0, (float)day_set, set_temp);
            disn2(8,1,set_temp[0]-48);disn(9,1,set_temp[1]-48);
            break;
        case 5:
            day_set = calculation(day_set,time_cursor-4,plus_minus);//0-减,1-加
            float2char(2, 0, (float)day_set, set_temp);
            disn(8,1,set_temp[0]-48);disn2(9,1,set_temp[1]-48);
            break;
        case 6:
            hour_set = calculation(hour_set,time_cursor+2-6,plus_minus);//0-减,1-加
            float2char(2, 0, (float)hour_set, set_temp);
            disn2(11,1,set_temp[0]-48);disn(12,1,set_temp[1]-48);
            break;
        case 7:
            hour_set = calculation(hour_set,time_cursor-6,plus_minus);//0-减,1-加
            float2char(2, 0, (float)hour_set, set_temp);
            disn(11,1,set_temp[0]-48);disn2(12,1,set_temp[1]-48);
            break;
        case 8:
            minute_set = calculation(minute_set,time_cursor+2-8,plus_minus);//0-减,1-加
            float2char(2, 0, (float)minute_set, set_temp);
            disn2(14,1,set_temp[0]-48);disn(15,1,set_temp[1]-48);
            break;
        case 9:
            minute_set = calculation(minute_set,time_cursor-8,plus_minus);//0-减,1-加
            float2char(2, 0, (float)minute_set, set_temp);
            disn(14,1,set_temp[0]-48);disn2(15,1,set_temp[1]-48);
            break;
        default:
            break;
    }
}

/*int calculation_time(int data, int bitNo, unsigned char flag_cursor)
{
	int calcResult=0;
	int calcTemp=1;
	int temp[2];
	int j;

	temp[1] = data/10;
	temp[0] = data%10;

	if(flag_cursor == 1)
	{
		if(temp[bitNo-1]==9)
			temp[bitNo-1]=0;
		else
			temp[bitNo-1]=temp[bitNo-1]+1;	
	}
	else
	{
		if(temp[bitNo-1]==0)
			temp[bitNo-1]=9;
		else
			temp[bitNo-1]=temp[bitNo-1]-1;
	}

	for(j=0;j<2;j++)
	{
		if(j==0)
			calcTemp = 1;
		else
			calcTemp = calcTemp*10;
			
		calcResult = (temp[j]*calcTemp) + calcResult;
	}

	return calcResult;
}*/
