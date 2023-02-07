#include "keyboard.h"
#include "syschip.h"
//-----------------------------------------------------------------
volatile unsigned int delay_timing = 0;  
  
/*记录按键按下时间 时间小于1.5S，按键为短按；时间大于1.5S，按键为长按*/  
volatile unsigned int key1_timing = 0;  
volatile unsigned int key2_timing = 0;  
  
/*记录两次短按之间的时间间隔*/  
volatile unsigned int key1_doublepress_timing = 0;  
volatile unsigned int key2_doublepress_timing = 0; 
  
/*定义按键键值结构体*/  
//Key_Value k1_value; 
volatile unsigned char k1_long_press = 0;
volatile unsigned char k1_short_press = 0;
volatile unsigned char k1_double_press = 0;

volatile unsigned char k2_long_press = 0;
volatile unsigned char k2_short_press = 0;
volatile unsigned char k2_double_press = 0;
  
/*K1按键短按标志位*/  
volatile unsigned char k1_shortpress_happen = 0; 
volatile unsigned char k2_shortpress_happen = 0;  
  
///*K1按键长按标志位*/  
//bool k1_longpress_happen = 0;  
  
/*K1按键双击标志位*/  
volatile unsigned char k1_doublepress_happen = 0; 
volatile unsigned char k2_doublepress_happen = 0; 
  
/*获取键值标志*/  
volatile unsigned char get_keyvalue1_flag = 0;  
volatile unsigned char get_keyvalue2_flag = 0;

extern unsigned int key_value;
 
/*获得键值*/  
void Get_Key1_Value(void)  
{  
	if(PORTCbits.RC8 == 0)//当按键按下
	{  
		if(k1_shortpress_happen==0)  
		{  
			k1_shortpress_happen = 1;      //开始一次按键键值扫描  
			key1_timing = 0;               //按键按下计时变量清0，开始计时，此值每1ms加1，sysclk中断函数中实现自加  
		}  
		else if(k1_shortpress_happen==1)  
		{  
			if(key1_timing > 100)         	//按键按下时间超过1.5S，长按事件发生，置k1_value.long_press为1  
			{  
				//k1_value.long_press = 1;  
				k1_long_press = 1;
				k1_shortpress_happen = 0;    //按键短按标志位置0  
			}  
		}  
	}  
             
	if(PORTCbits.RC8 == 1)//当按键抬起 	
	{  
		if(k1_shortpress_happen==1)      //按键抬起后，k1_shortpress_happen等于1，说明按键为短按，不是长按  
		{  
			k1_shortpress_happen = 0;   
			if(k1_doublepress_happen==0)  
			{  
				k1_doublepress_happen = 1;    		//按键双击标志位置1，等待确认按键是否为双击  
				key1_doublepress_timing = 0;  		//开始计时，同样1ms自增加1  
			}  
			else  
			{  
				if(key1_doublepress_timing < 12)  	//第一次短按发生后，在500ms时间内，发生第二次短按，完成一次双击，跟新按键键值  
				{  
					k1_doublepress_happen = 0;  
					//k1_value.double_press = 1;  
					k1_double_press = 1; 
				}  
			}  
		}  
		else if(k1_doublepress_happen == 1)         //第一次短按后，等待500ms,如未再发生短按，跟新按键短按键值  
		{  
			if(key1_doublepress_timing > 12)  
			{  
				k1_doublepress_happen = 0;  
				//k1_value.short_press = 1;  
				k1_short_press = 1; 
			}  
		} 
		
		if(k1_long_press == 1)
		{
			k1_long_press = 0; 
			
			get_keyvalue1_flag = 0; 
            T1CONbits.TON = 0;//关闭16位Timer1
			key1_timing = 0; 
			key1_doublepress_timing = 0;
		}
	}  
}  
  
/*按键事件处理函数 根据键值 进行相应的事件处理*/  
void Key1_Func(void)  
{  
	int temp = 0;

	//if(k1_value.short_press == 1)  
	if(k1_short_press == 1) 
	{  
        //k1_value.short_press = 0;  
        k1_short_press = 0;

        //------------------------
        key_value=1;

        get_keyvalue1_flag = 0;  
        T1CONbits.TON = 0;//关闭16位Timer1
        key1_timing = 0; 
        k1_shortpress_happen = 0;
	}  
		
	//if(k1_value.long_press == 1)  
	if(k1_long_press == 1)
	{  
        //k1_value.long_press = 0;  
        //long_press = 0; 
        if((key1_timing%5)==0)
        {		
            //------------------------
            key_value=2;
        }
	}  
		
	//if(k1_value.double_press == 1) 
    if(k1_double_press == 1) 	
	{  
        //k1_value.double_press = 0;  
        k1_double_press = 0;

        {

        }

        get_keyvalue1_flag = 0; 
        T1CONbits.TON = 0;//关闭16位Timer1	
        key1_timing = 0; 
        key1_doublepress_timing = 0;
        k1_doublepress_happen = 0;
	}  
}  

//------------------------------------------------------------------------------
void Get_Key2_Value(void)  
{  
	if(PORTCbits.RC9 == 0)//当按键按下
	{  
		if(k2_shortpress_happen==0)  
		{  
			k2_shortpress_happen = 1;      //开始一次按键键值扫描  
			key2_timing = 0;               //按键按下计时变量清0，开始计时，此值每1ms加1，sysclk中断函数中实现自加  
		}  
		else if(k2_shortpress_happen==1)  
		{  
			if(key2_timing > 100)         	//按键按下时间超过1.5S，长按事件发生，置k1_value.long_press为1  
			{  
				//k2_value.long_press = 1;  
				k2_long_press = 1;
				k2_shortpress_happen = 0;    //按键短按标志位置0  
			}  
		}  
	}  
             
	if(PORTCbits.RC9 == 1)//当按键抬起 	
	{  
		if(k2_shortpress_happen==1)      //按键抬起后，k1_shortpress_happen等于1，说明按键为短按，不是长按  
		{  
			k2_shortpress_happen = 0;   
			if(k2_doublepress_happen==0)  
			{  
				k2_doublepress_happen = 1;    		//按键双击标志位置1，等待确认按键是否为双击  
				key2_doublepress_timing = 0;  		//开始计时，同样1ms自增加1  
			}  
			else  
			{  
				if(key2_doublepress_timing < 12)  	//第一次短按发生后，在500ms时间内，发生第二次短按，完成一次双击，跟新按键键值  
				{  
					k2_doublepress_happen = 0;  
					//k2_value.double_press = 1;  
					k2_double_press = 1; 
				}  
			}  
		}  
		else if(k2_doublepress_happen == 1)         //第一次短按后，等待500ms,如未再发生短按，跟新按键短按键值  
		{  
			if(key2_doublepress_timing > 12)  
			{  
				k2_doublepress_happen = 0;  
				//k1_value.short_press = 1;  
				k2_short_press = 1; 
			}  
		} 
		
        if(k2_long_press == 1)
        {
            k2_long_press = 0; 

            get_keyvalue2_flag = 0; 
            T1CONbits.TON = 0;//关闭16位Timer1
            key2_timing = 0; 
            key2_doublepress_timing = 0;
        }
	}  
}  
  
/*按键事件处理函数 根据键值 进行相应的事件处理*/  
void Key2_Func(void)  
{  
	int temp = 0;

	//if(k2_value.short_press == 1)  
	if(k2_short_press == 1) 
	{  
        //k2_value.short_press = 0;  
        k2_short_press = 0;

        //------------------------
        key_value=3;

        get_keyvalue2_flag = 0;  
        T1CONbits.TON = 0;//关闭16位Timer1
        key2_timing = 0; 
        k2_shortpress_happen = 0;
	}  
		
	//if(k2_value.long_press == 1)  
	if(k2_long_press == 1)
	{  
        //k2_value.long_press = 0;  
        //long_press = 0; 
        if((key2_timing%5)==0)
        {
            //------------------------
            key_value=4;
        }
	}  
		
	//if(k2_value.double_press == 1) 
    if(k2_double_press == 1) 	
	{  
        //k2_value.double_press = 0;  
        k2_double_press = 0;

        {

        }

        get_keyvalue2_flag = 0; 
        T1CONbits.TON = 0;//关闭16位Timer1		
        key2_timing = 0; 
        key2_doublepress_timing = 0;
        k2_doublepress_happen = 0;
	}  
}  