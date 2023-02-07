#include "utility.h"
#include "math.h"

static char table[]={'0', '1', '2', '3', '4', '5', '6', '7', '8', '9'};

/*******************************************************************************
* Float转换成数字
* 有效位-8位
********************************************************************************/
void float2char(unsigned int g, unsigned int l, float number, char *str)
{
    unsigned int i;
    int temp = number/1;
    float t2 = 0.0;
    for (i = 1; i<=g; i++)
    {
        if (temp==0)
            str[g-i] = '0';
        else
            str[g-i] = table[temp%10];
        temp = temp/10;
    }
	if(l!=0)
	{
	    *(str+g) = '.';
	    temp = 0;
	    t2 = number;
	    for(i=1; i<=l; i++)
	    {
	        temp = t2*10;
	        str[g+i] = table[temp%10];
	        t2 = t2*10;
	    }
		*(str+g+l+1) = '\0';
	}
	else
	{
		*(str+g+l) = '\0';
	}
}

float char2float(char *str)
{
	int flag=0;   
    int count = 0;  
    int ret = 1;  
    float sum = 0;

	while(*str != '\0')  
    {    
    	if(*str == '-')  
    	{  
    		ret = -1;  
    		str++;
    	}  
       	if(*str == '.')  
       	{  
           	flag = 1 ;  
           	str ++;  
       	}  
       	if(*str >= '0' && *str <= '9')  
       	{  
         	sum = sum*10 + (*str - '0');  
           
         	if(flag == 1)  
         	{  
           		count ++;  
         	}  
       	}  
       	str++;
    } 
    sum = sum /pow(10,count);  
    return ret*sum;	 
}

unsigned char Change10ToBCD(unsigned char data)//10->BCD�
{
    unsigned char high,low,result;

    high=data/10;
    low=data%10;
    result=(high<<4)|low;
    return result;
}

unsigned char ChangeBCDTo10(unsigned char data)//BCD->10�
{
    unsigned char temp,result;

    result=data&0x0F;
    temp=data>>4;
    result+=temp*10;
    return result;
}