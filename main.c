/*********************************************************************
 *     MCU:dsPIC33EP128GP504 
 *     OSC:7.3728MHz
 *********************************************************************/
#include "main.h"
#include "math.h"
#include "system.h"
#include "eeprom.h" 
#include "keyboard.h"
#include "oled.h"
#include "welcome.h"
#include "menu_action.h"
#include "menu_main.h"
#include "menu_running.h"
#include "utility.h"
#include "adc.h"

#include  "i2c.h"

#define oled_test
//------------------------------------------------------------------------------

//#define oldeeprom_test
unsigned char leak_branch_No_save;
unsigned char leak_trouble_type_save;
unsigned char leak_trouble_type;
//------------------------------------------------------------------------------
extern unsigned char run_flag;
//extern unsigned int key_value;
extern key_type  keyValue;
extern eeprom_data eepromData;
extern unsigned char menu_no;

//extern unsigned char algorithm;
//extern unsigned char action;
//extern unsigned char baud_rate;
//extern unsigned int neutral_resistor;
//extern unsigned int pre_ac;
//extern unsigned int pre_dc;
//extern unsigned int time_delay;
//extern unsigned int comm_address;

extern unsigned char menu_class;

unsigned int Result;
//------------------------------------------------------------------------------
unsigned char samplingTimes;
unsigned char flagSamplingType = 1;//0-启动周期采样检测，1-单点采样检测
unsigned char flag_detection = 0;
unsigned char flagProcess = 0;
unsigned char flagLock = 0;
unsigned char flagBreak = 0;

float DCActionCompareValue = 0;
float ACActionCompareValue = 0;

//unsigned char ACSubdivisionType;

//float Base=1.65;
float Init_Base=1.65;
float VREF=3.3;

int Result_Vl[40];
int Result_Vdc[40];
int Result_Vac[40];

unsigned char flag_Vdc=0;
unsigned char flag_Vac=0;

float resultVdc = 0;
float resultVac = 0;
float resultVl = 0;

float preResultVdc = 0;
float preResultVac = 0;

float testResultVdc = 0;
float testResultVac = 0;
float testI1;
float testI2;
//------------------------------------------------------------------------------

void Delay_1ms(unsigned int t)//t = 1000���1s
{
    unsigned int i,j;
    for(i = 0;i < t;i ++)
        for(j = 0;j < 2200;j ++);
}
//��������
void SystemColck(void)
{
    //产生Fosc=140MHz,FCY=140MIP/2=70MIPS
    //fosc= fin*(div+2)/((pre+2)*2(lost+1))
    /*111 = FCY divided by 128
110 = FCY divided by 64
101 = FCY divided by 32
100 = FCY divided by 16
011 = FCY divided by 8 (default)
010 = FCY divided by 4
001 = FCY divided by 2
000 = FCY divided by 1
bit 11 DOZEN: Doze Mode Enab*/

// 
    //fcy=fosc/2->all peripherals
    CLKDIVbits.PLLPRE = 0;//N1=2分频,  
                          //7.3728MHz/2 = 3.6864MHz

    PLLFBDbits.PLLDIV = 76;//M=76      
                           //3.6864*76 = 280.1664MHz
    
    CLKDIVbits.PLLPOST = 0;//N2=2 
                           //280.1664/2 = 140.0832MHz
  
    while (OSCCONbits.COSC!= 0b011)
    while (OSCCONbits.LOCK!= 1) {};//PLL
//    CLKDIVbits.ROI =0;
//    CLKDIVbits.DOZEN =0;
//    CLKDIVbits.DOZE =0b001;//default is 8
//    CLKDIVbits.DOZEN =1;//peripheral divided by 2->70M/2=35M
}

void InitSystem(void)
{
    ANSELB = 0x0000;//数字引脚
    TRISB &= ~0xfc00;//RB10-RB15(~1111 1100 0000 0000‬)���
   
    TRISBbits.TRISB3 = 0;
    TRISBbits.TRISB13 = 0;
    TRISBbits.TRISB14 = 0;
    TRISBbits.TRISB15 = 0;
    
    LATBbits.LATB3 = 1;  	//继电器动作置高电位1(1不动作，0动作)
	LATBbits.LATB13 = 1;  	//继电器动作置高电位1(1不动作，0动作) 
    LATBbits.LATB14 = 1;  	//继电器动作置高电位1(1不动作，0动作)
	LATBbits.LATB15 = 1;  	//继电器动作置高电位1(1不动作，0动作) 
    
    TRISAbits.TRISA8 = 0;
    TRISBbits.TRISB4 = 0;
    TRISCbits.TRISC3 = 0;
                          
    LATAbits.LATA8 = 0;  
    LATBbits.LATB4 = 0; 
    
    TRISCbits.TRISC3 = 0;
    LATCbits.LATC3 = 0;
    
    TRISCbits.TRISC1 = 0;
    LATCbits.LATC1 = 0;     //复位指示灯  
    
    //IOCON1 = 0x0000;//GPIO 模块控制 PWMxH PWMxL引脚��
}

void InitKeyboard(void)
{
    TRISCbits.TRISC6 = 1;//RC6按键输入
    TRISCbits.TRISC7 = 1;//RC7按键输入
    TRISCbits.TRISC8 = 1;//RC8按键输入
    TRISCbits.TRISC9 = 1;//RC9按键输入
    
    CNENCbits.CNIEC6 = 1;
    CNENCbits.CNIEC7 = 1;
    CNENCbits.CNIEC8 = 1;
    CNENCbits.CNIEC9 = 1;
    
    IFS1bits.CNIF = 0; 			         // Reset CN interrupt flag
	IEC1bits.CNIE = 1; 			         // Enable CN interrupt
    
    //*(volatile char *)(&TRISC) = 0x0F;   // Keyboard on CN16-CN23, output CN23-20,input CN19-16 
    //*(volatile char *)(&CNENC) = 0x0F;   // CN19-16 interrupt enable
}

void InitTMR1(void)                        //定时器模式
{
    /*
    T1CONbits.TON = 0;                      //禁止16位Timer1
    T1CONbits.TCS = 0;                      //内部时钟Fosc/2
    T1CONbits.TGATE = 0;                    //门控时间累加位 禁止
    T1CONbits.TCKPS = 0x00;                 //输入时钟预分频比1:1
    T1CONbits.TSIDL = 0;                    //空闲模式继续工作
    T1CONbits.TSYNC = 0;                    //不同步外部时钟（不起作用，始终是同步内部时钟）
    */
    
    T1CON = 0x0000;
    
    TMR1 = 0x0000;                          //初始值
    PR1 = 35000;                            //周期值
    
    IPC0bits.T1IP = 0x01;                   // 优先级1
    IFS0bits.T1IF = 0;                      // Timer1中断标志状态位 清零
    IEC0bits.T1IE = 1;                      // Timer1中断允许位 允许
    //T1CONbits.TON = 1;                    //启动使能16位Timer1
}

void InitTMR2(void)//定时器模式
{
    T2CON = 0x0000;
    /*T2CONbits.TON = 0;                    //禁止16位Timer2
    T2CONbits.T32 = 1;                      //TMR2类构成32位定时器
    T2CONbits.TGATE = 0;                    //门控时间累加位 禁止
    T2CONbits.TCKPS = 0x00;                 //输入时钟预分频比1:1*/
    
    IPC1bits.T2IP = 0x01;                   //优先级1
    
    TMR2 = 0x0000;                          //数值低位字                           
    PR2 = 35000;                            //lsw(低位字)
    //FCY=70M
    //20ms/40=500us,(65536*0.0005)/(65536*0.00000005)=10000(0x2710)
    //0.0005/0.0000000142857142857143=35000.035000035(0x‭88b8‬)
    
    IFS0bits.T2IF = 0;                      //Timer2中断标志状态位 清零
    IEC0bits.T2IE = 0;                      //Timer2中断允许位 禁止
}



int main(void)
{
    double Vl=0;
    double Vac=0;
	float Vdc=0;
//	float Temp2, Temp3;
    
    //unsigned int i = 0,j = 0;
    SystemColck();
    InitSystem();
    InitKeyboard();
    InitTMR1();
    InitTMR2();
    InitADC();
    menu_init();
    //OLED
    OLED_Init();
    OLED_Clear();
    
    //EEPROM
    		//Initialise the I2C(2) Peripheral
//    ConfigI2CEEPROM();
//    Delay_1ms(3000);
//    Delay_1ms(3000);
//    Delay_1ms(3000);
    
    //从EEPROM读取设定值
#ifdef oldeeprom_test
    InitI2C();	
    while (1){
            Welcome_Page();
      Get_Sys_Set();
          Delay_1ms(3000);
    Delay_1ms(3000);
    Delay_1ms(3000);
    }
#elseif neweeprom_test
        while (1){
            Welcome_Page();
      Get_Sys_Set();
          Delay_1ms(3000);
    Delay_1ms(3000);
    Delay_1ms(3000);
    }
    
#endif
    Welcome_Page();
      Get_Sys_Set();
//      while(1);
    Delay_1ms(3000);
//    Delay_1ms(3000);
//    Delay_1ms(3000);
//    OLED_Clear();//清屏
    
//    Menu_Running();//running page
    
    if(eepromData.algorithm == 0 || eepromData.algorithm == 1 || eepromData.algorithm == 2)
	{
		samplingTimes=0;				//清采样次数	
		TMR2 = 0;                       //定时器2清零
		PR2 = 35000;
		IEC0bits.T2IE = 1;              //使能中断
		T2CONbits.TON = 1;              //开定时器2
	
		flagSamplingType = 0;			//初始化时不判断
		//Vac = 0;
		while(1)
		{
			if(samplingTimes==40)
				break;                  //AD转换次数达到40，则终止
		}
		IEC0bits.T2IE = 0;              //禁止中断
		Init_Base = Calculate_Base();
		flagSamplingType = 1;
	}
    
    while(1)
    {
#ifndef oled_test
        
        flagProcess = 0;
        while(run_flag==1)
        {   
            if(flagProcess == 0)
            {
                RealData_Sampling();
            }
            else if(flagProcess == 1)
            {
                RealData_Lock();
            }
            else if(flagProcess == 2)
            {
                RealData_Break();
            }
            
            //------------------------------------------------------------------
            if(eepromData.algorithm == 0)//直流
            {
                while(flagSamplingType==1)
                {
                    //CLRWDT();//-----------------------------------------------
                    flag_detection=0;

                    samplingTimes=0;	    //清采样次数	
                    TMR2 = 0;				//定时器2清零
                    PR2 = 35000;
                    IEC0bits.T2IE = 1; 		//使能中断
                    T2CONbits.TON = 1; 		//开定时器2

                    Vdc = 0;
                    Vac = 0;
                    Vl = 0;
                    while(1)
                    {	
                        if(flag_detection==1)                       //检测到启动信号时
                            break;				

                        if(samplingTimes>=40 && flag_detection==0)	//没有检测到启动信号时
                            break;
                    }
                    IEC0bits.T2IE = 0; 	//禁止中断

                    if(flag_detection==0)			//不启动检测的情况
                    {
                        Vdc = Calculate_Udc();
                        Vac = Calculate_Uac();
                        Vl  = Calculate_Ul();
                        resultVdc = Vdc;
                        resultVac = Vac;
                        resultVl  = Vl;
                        samplingTimes = 0;
                        
                        if(flagProcess == 0)
                        {
                            RealData_Sampling();
                        }
                        else if(flagProcess == 1)
                        {
                            RealData_Lock();
                        }
                        else if(flagProcess == 2)
                        {
                            RealData_Break();
                        }
                    }
                    else{
                        flagSamplingType=0;
                    }
                    
                    Get_Keyboard_Running();
                    if(keyValue == Enter)//5)
                    {
                        keyValue = None;//0;
                        run_flag = 0;
                        //menu_class = 1;
                        OLED_Clear();	//清屏
                        Menu_Main(menu_no);
                        
                        flagSamplingType=0;
                    }

                    if(keyValue == Escape)//6)
                    {
                       keyValue = None;//0;
                
                       OLED_Clear();//清屏
                       Menu_Running();
                       
                       flagBreak = 0;
                       flagLock = 0;
                       flagProcess = 0;  
                       flagSamplingType = 1;
                    }
                }

                //CLRWDT();//---------------------------------------------------
                //flagSamplingType=0;//调试用
                
                samplingTimes=0;	//清采样次数	
                TMR2 = 0;			//定时器2清零
                PR2 = 35000;
                IEC0bits.T2IE = 1; 	//使能中断
                T2CONbits.TON = 1; 	//开定时器2

                Vdc = 0;
                Vac = 0;
                Vl = 0;
                while(1)
                {
                    if(samplingTimes==40)	
                        break;		//AD转换次数达到40，则终止
                }
                IEC0bits.T2IE = 0; 	//禁止中断

                Vdc = Calculate_Udc();
                Vac = Calculate_Uac();
                Vl  = Calculate_Ul();
                resultVdc = Vdc;
                resultVac = Vac;
                resultVl  = Vl;

                if(flagLock == 0 && Vdc >= (DCActionCompareValue/2) && (float)Vac >= (ACActionCompareValue/10))
                {
                    //if((float)Vac >= (ACActionCompareValue/10))
                    //{
                        LATBbits.LATB14 = 0;        //警报继电器动作	

                         if(flagBreak == 0)
                        {
                            //RealStatus();               //显示闭锁
                            flagLock = 1;
                            flagProcess = 1;

                            OLED_Clear();//清屏
                            Alarm_Lock();
                            RealData_Lock();
                        }
                    //}
                }
                
                if(flagLock == 1 && Vdc < (DCActionCompareValue/2))
                {
                    LATBbits.LATB14 = 1;        //警报继电器动作	
                    
                     if(flagBreak == 0)
                    {
                        //RealStatus();               //显示闭锁
                        flagLock = 0;
                        flagProcess = 0;
                    
                        OLED_Clear();//清屏
                        Menu_Running();
                        RealData_Sampling();
                    }
                }
                
                if(Vdc >= DCActionCompareValue)
                {	
                    if(eepromData.time_delay != 0)
                    {
                        TMR1 = 0; 					//定时器1计数寄存器TMR1＝0
                        if(eepromData.time_delay == 0)	{
                            PR1 = 0x1EA4;			//延时0s，暂时给定为100ms(0x1EA4)
                        } else {
                            PR1 = eepromData.time_delay;		//time_delay                          
                        }

                        IEC0bits.T1IE = 0; 			//使能中断
                        T1CONbits.TON = 1; 			//开定时器1
                        while(IFS0bits.T1IF != 1)
                        {
                            //CLRWDT();//----------------------------------------		

                            samplingTimes=0;		//清采样次数	
                            TMR2 = 0;				//定时器2清零
                            PR2 = 35000;
                            IEC0bits.T2IE = 1; 		//使能中断
                            T2CONbits.TON = 1; 		//开定时器2

                            Vdc = 0;
                            Vac = 0;
                            Vl = 0;
                            while(1)
                            {
                                if(samplingTimes==40){	
                                    break;		//A/D转换次数达到40，则终止
                                }
                            }
                            IEC0bits.T2IE = 0; 	//禁止中断

                            Vdc = Calculate_Udc();
                            Vac = Calculate_Uac();
                            Vl  = Calculate_Ul();        
                            if(Vdc < DCActionCompareValue)
                            {
                                flagBreak = 1;
                                break;
                            }
                        }
                        T1CONbits.TON = 0; 			//关定时器1

                        if(flagBreak == 0 && Vdc >= DCActionCompareValue && (float)Vac >= (ACActionCompareValue/5))
                        {
                            //if((float)Vac >= (ACActionCompareValue/5))
                            //{
                                if(eepromData.action == 1){
                                    LATBbits.LATB15 = 0;    //设备跳闸驱动继电器
                                }

                                flagBreak = 1;
                                flagProcess = 2;

                                OLED_Clear();//清屏
                                Alarm_Break();
                                RealData_Break();
                            //}
                        }
                        
                        if(flagBreak == 1 && Vdc < DCActionCompareValue)
                        {
                            if(eepromData.action == 1){
                                LATBbits.LATB15 = 1;    //设备跳闸驱动继电器
                            }
                        }
                    }	
                    else
                    {
                        LATBbits.LATB15 = 0;    //设备跳闸驱动继电器
                    }
                }
                else if(Vdc < (DCActionCompareValue/2)){
                    flagSamplingType = 1;		//回到检查是否需要启动检测
                }
            }
            
            //------------------------------------------------------------------
            if(eepromData.algorithm == 1)//交流
            {
                while(flagSamplingType==1)
                {
                    //CLRWDT();//-----------------------------------------------
                    flag_detection=0;

                    samplingTimes=0;	    //清采样次数	
                    TMR2 = 0;				//定时器2清零
                    PR2 = 35000;
                    IEC0bits.T2IE = 1; 		//使能中断
                    T2CONbits.TON = 1; 		//开定时器2

                    Vdc = 0;
                    Vac = 0;
                    Vl = 0;
                    while(1)
                    {	
                        if(flag_detection==1)                       //检测到启动信号时
                            break;				

                        if(samplingTimes>=40 && flag_detection==0)	//没有检测到启动信号时
                            break;
                    }
                    IEC0bits.T2IE = 0; 	//禁止中断

                    if(flag_detection==0)			//不启动检测的情况
                    {
                        Vdc = Calculate_Udc();
                        Vac = Calculate_Uac();
                        Vl  = Calculate_Ul();
                        resultVdc = Vdc;
                        resultVac = Vac;
                        resultVl  = Vl;
                        samplingTimes = 0;
                        
                        if(flagProcess == 0)
                        {
                            RealData_Sampling();
                        }
                        else if(flagProcess == 1)
                        {
                            RealData_Lock();
                        }
                        else if(flagProcess == 2)
                        {
                            RealData_Break();
                        }
                    }
                    else {
                        flagSamplingType=0;
                    }
                    
                    Get_Keyboard_Running();
                    if(keyValue == Enter)//5)
                    {
                        keyValue = None;//0;
                        run_flag = 0;
                        //menu_class = 1;
                        OLED_Clear();	//清屏
                        Menu_Main(menu_no);
                        
                        flagSamplingType=0;
                    }

                    if(keyValue == Escape)//6)
                    {
                       keyValue = None;//0;
                
                       OLED_Clear();//清屏
                       Menu_Running();
                       
                       flagBreak = 0;
                       flagLock = 0;
                       flagProcess = 0;  
                       flagSamplingType = 1;
                    }
                }

                //CLRWDT();//---------------------------------------------------
                
                samplingTimes=0;	//清采样次数	
                TMR2 = 0;			//定时器2清零
                PR2 = 35000;
                IEC0bits.T2IE = 1; 	//使能中断
                T2CONbits.TON = 1; 	//开定时器2

                Vdc = 0;
                Vac = 0;
                Vl = 0;
                while(1)
                {
                    if(samplingTimes==40)	
                        break;		//AD转换次数达到40，则终止
                }
                IEC0bits.T2IE = 0; 	//禁止中断

                Vdc = Calculate_Udc();
                Vac = Calculate_Uac();
                Vl  = Calculate_Ul();
                resultVdc = Vdc;
                resultVac = Vac;
                resultVl  = Vl;
                
                if(flagLock == 0 && (float)Vac >= (ACActionCompareValue/2))
                {	
                    LATBbits.LATB14 = 0;            //警报继电器动作	
                   
                    if(flagBreak == 0)
                    {
                        //RealStatus();                   //显示闭锁
                        flagLock = 1;
                        flagProcess = 1;
                    
                        OLED_Clear();//清屏
                        Alarm_Lock();
                        RealData_Lock();
                    }
                }

                if(flagLock == 1 && (float)Vac < (ACActionCompareValue/2))
                {	
                    LATBbits.LATB14 = 1;            //警报继电器
                   
                    if(flagBreak == 0)
                    {
                         //RealStatus();                   //显示闭锁
                        flagLock = 0;
                        flagProcess = 0;
                        
                        OLED_Clear();//清屏
                        Menu_Running();
                        RealData_Sampling();
                    }
                }
                
                if((float)Vac >= ACActionCompareValue)
                {	
                    if(eepromData.time_delay != 0)
                    {
                        TMR1 = 0; 					//定时器1计数寄存器TMR1＝0
                        if(eepromData.time_delay == 0)	{
                            PR1 = 0x1EA4;			//延时0s，暂时给定为100ms(0x1EA4)
                        } else {
                            PR1 =eepromData.time_delay;		//time_delay                          
                        }

                        IEC0bits.T1IE = 0; 			//使能中断
                        T1CONbits.TON = 1; 			//开定时器1
                        while(IFS0bits.T1IF != 1)
                        {
                            //CLRWDT();//----------------------------------------		

                            samplingTimes=0;		//清采样次数	
                            TMR2 = 0;				//定时器2清零
                            PR2 = 35000;
                            IEC0bits.T2IE = 1; 		//使能中断
                            T2CONbits.TON = 1; 		//开定时器2

                            Vdc = 0;
                            Vac = 0;
                            Vl  = 0;
                            while(1)
                            {
                                if(samplingTimes==40){	
                                    break;		//A/D转换次数达到40，则终止
                                }
                            }
                            IEC0bits.T2IE = 0; 	//禁止中断

                            Vdc = Calculate_Udc();
                            Vac = Calculate_Uac();
                            Vl  = Calculate_Ul();  
                            
                            if((float)Vac < ACActionCompareValue)
                            {
                                flagBreak = 1;
                                break;
                            }
                        }
                        T1CONbits.TON = 0; 			//关定时器1

                        if(flagBreak == 0 && (float)Vac >= ACActionCompareValue)
                        {
                            if(eepromData.action == 1){
                                LATBbits.LATB15 = 0;    //设备跳闸驱动继电器
                            }

                            flagBreak = 1;
                            flagProcess = 2;

                            OLED_Clear();//清屏
                            Alarm_Break();
                            RealData_Break();
                        }

                        if(flagBreak == 1 && (float)Vac < ACActionCompareValue)
                        {
                            if(eepromData.action == 1){
                                LATBbits.LATB15 = 1;    //设备跳闸驱动继电器
                            }

                            /*flagBreak = 0;
                            flagProcess = 1;

                            OLED_Clear();//清屏
                            Alarm_Lock();
                            RealData_Lock();*/
                        }
                    }	
                    else
                    {
                        LATBbits.LATB15 = 0;    //设备跳闸驱动继电器
                    }
                }
                else if((float)Vac < (ACActionCompareValue/2)){
                    flagSamplingType = 1;//回到检查是否需要启动检测
                }
            }
            //------------------------------------------------------------------
            
            //------------------------------------------------------------------
            if(eepromData.algorithm == 2)//交直
            {
                while(flagSamplingType==1)
                {
                    //CLRWDT();//-----------------------------------------------
                    flag_detection=0;

                    samplingTimes=0;	    //清采样次数	
                    TMR2 = 0;				//定时器2清零
                    PR2 = 35000;
                    IEC0bits.T2IE = 1; 		//使能中断
                    T2CONbits.TON = 1; 		//开定时器2

                    Vdc = 0;
                    Vac = 0;
                    Vl = 0;
                    while(1)
                    {	
                        if(flag_detection==1)                       //检测到启动信号时
                            break;				

                        if(samplingTimes>=40 && flag_detection==0)	//没有检测到启动信号时
                            break;
                    }
                    IEC0bits.T2IE = 0; 	//禁止中断

                    if(flag_detection==0)			//不启动检测的情况
                    {
                        Vdc = Calculate_Udc();
                        Vac = Calculate_Uac();
                        Vl  = Calculate_Ul();
                        resultVdc = Vdc;
                        resultVac = Vac;
                        resultVl  = Vl;
                        samplingTimes = 0;
                        
                        if(flagProcess == 0)
                        {
                            RealData_Sampling();
                        }
                        else if(flagProcess == 1)
                        {
                            RealData_Lock();
                        }
                        else if(flagProcess == 2)
                        {
                            RealData_Break();
                        }
                    }
                    else {
                        flagSamplingType=0;
                    }
                    
                    Get_Keyboard_Running();
                    if(keyValue == Enter)//5)
                    {
                        keyValue = None;//0;
                        run_flag = 0;
                        //menu_class = 1;
                        OLED_Clear();	//清屏
                        Menu_Main(menu_no);
                
                        flagSamplingType=0;
                    }

                    if(keyValue == Escape)//6)
                    {
                        keyValue = None;//0;
                
                       OLED_Clear();//清屏
                       Menu_Running();
                       
                       flagBreak = 0;
                       flagLock = 0;
                       flagProcess = 0;  
                       flagSamplingType = 1;
                    }
                }

                //CLRWDT();//---------------------------------------------------
                //flagSamplingType=0;//调试用
                
                samplingTimes=0;	//清采样次数	
                TMR2 = 0;			//定时器2清零
                PR2 = 35000;
                IEC0bits.T2IE = 1; 	//使能中断
                T2CONbits.TON = 1; 	//开定时器2

                Vdc = 0;
                Vac = 0;
                Vl = 0;
                while(1)
                {
                    if(samplingTimes==40)	
                        break;		//AD转换次数达到40，则终止
                }
                IEC0bits.T2IE = 0; 	//禁止中断

                Vdc = Calculate_Udc();
                Vac = Calculate_Uac();
                Vl  = Calculate_Ul();
                resultVdc = Vdc;
                resultVac = Vac;
                resultVl  = Vl;

                if(flagLock == 0)
                {
                    if(Vdc >= (DCActionCompareValue/2) || (float)Vac >= (ACActionCompareValue/2))
                    {
                        if((float)Vac >= (ACActionCompareValue/10))
                        {
                            LATBbits.LATB14 = 0;        //警报继电器动作

                            if(flagBreak == 0)
                            {
                                //RealStatus();               //显示闭锁
                                flagLock = 1;
                                flagProcess = 1;

                                OLED_Clear();//清屏
                                Alarm_Lock();
                                RealData_Lock();
                            }
                        }
                    }
                }
                
                if(flagLock == 1)
                {
                    if(Vdc < (DCActionCompareValue/2) && (float)Vac < (ACActionCompareValue/2))
                    {
                         LATBbits.LATB14 = 1;            //警报继电器
                   
                        if(flagBreak == 0)
                        {
                             //RealStatus();                   //显示闭锁
                            flagLock = 0;
                            flagProcess = 0;

                            OLED_Clear();//清屏
                            Menu_Running();
                            RealData_Sampling();
                        }
                    }
                }
                
                if(Vdc >= DCActionCompareValue || (float)Vac >= ACActionCompareValue)
                {	
                    if(eepromData.time_delay != 0)
                    {
                        TMR1 = 0; 					//定时器1计数寄存器TMR1＝0
                        if(eepromData.time_delay == 0)	{
                            PR1 = 0x1EA4;			//延时0s，暂时给定为100ms(0x1EA4)
                        } else {
                            PR1 = eepromData.time_delay;		//time_delay                          
                        }

                        IEC0bits.T1IE = 0; 			//使能中断
                        T1CONbits.TON = 1; 			//开定时器1
                        while(IFS0bits.T1IF != 1)
                        {
                            //CLRWDT();//----------------------------------------		

                            samplingTimes=0;		//清采样次数	
                            TMR2 = 0;				//定时器2清零
                            PR2 = 35000;
                            IEC0bits.T2IE = 1; 		//使能中断
                            T2CONbits.TON = 1; 		//开定时器2

                            Vdc = 0;
                            Vac = 0;
                            Vl  = 0;
                            while(1)
                            {
                                if(samplingTimes==40){	
                                    break;		//A/D转换次数达到40，则终止
                                }
                            }
                            IEC0bits.T2IE = 0; 	//禁止中断

                            Vdc = Calculate_Udc();
                            Vac = Calculate_Uac();
                            Vl  = Calculate_Ul();        
                            if(Vdc < DCActionCompareValue && (float)Vac < ACActionCompareValue)
                            {
                                flagBreak = 1;
                                break;
                            }
                        }
                        T1CONbits.TON = 0; 			//关定时器1

                        if(flagBreak == 0)
                        {
                            if(Vdc >= DCActionCompareValue || (float)Vac >= ACActionCompareValue)
                            {
                                if((float)Vac >= (ACActionCompareValue/5))
                                {
                                    if(eepromData.action == 1){
                                        LATBbits.LATB15 = 0;    //设备跳闸驱动继电器
                                    }

                                    flagBreak = 1;
                                    flagProcess = 2;

                                    OLED_Clear();//清屏
                                    Alarm_Break();
                                    RealData_Break();
                                }
                            }
                        }
                        
                        if(flagBreak == 1)
                        {
                            if(Vdc < DCActionCompareValue && (float)Vac < ACActionCompareValue)
                            {
                                if(eepromData.action == 1){
                                    LATBbits.LATB15 = 1;    //设备跳闸驱动继电器
                                }
                            }
                        }
                    }	
                    else
                    {
                        if(Vdc >= DCActionCompareValue || (float)Vac >= ACActionCompareValue)
                        {
                            if(eepromData.action == 1){
                                LATBbits.LATB15 = 0;    //设备跳闸驱动继电器
                            }
                        }
                    }
                }
                else if(Vdc < (DCActionCompareValue/2) && (float)Vac < (ACActionCompareValue/2)){
                    flagSamplingType = 1;						//回到检查是否需要启动检测
                }
            }
            //------------------------------------------------------------------
            
            Get_Keyboard_Running();
            if(keyValue == Enter)//5)
            {
                keyValue = None;//0;
                run_flag = 0;
                
                OLED_Clear();	//清屏
                Menu_Main(menu_no);
                
            }
            
            if(keyValue == Escape)//6)
            {
                keyValue = None;//0;
                
                OLED_Clear();//清屏
                Menu_Running();
                
                flagBreak = 0;
                flagLock = 0;
                flagProcess = 0;  
                flagSamplingType = 1;
            }
        }
#endif
        
//        Get_Keyboard();
        excute_KeyFunction();
        
    }
}

float Calculate_Udc(void)
{
	float voltage_dc=0;
	float Temp10=0;
	unsigned char i;//unsigned char i,j,k;
    /*int Data_Buf;
    
    for(i=0;i<40;i++)	
	{
		for(j=0;j<39-i;j++)
		{
			if(Result_Vdc[j]>Result_Vdc[j+1])
			{
				Data_Buf=Result_Vdc[j];
				Result_Vdc[j]=Result_Vdc[j+1];
				Result_Vdc[j+1]=Data_Buf;
			}
		}
	}
    
     for(k=1;k<(40-2);k++) 
    {
		Temp10 += (float)Result_Vdc[k];
    }
    Temp10 = Temp10/38;
    voltage_dc = VREF*(Temp10/4095);
     
    for(k=5;k<(40-10);k++)
    {
		Temp10 += (float)Result_Vdc[k];
    }
    Temp10 = Temp10/30;
    voltage_dc = VREF*(Temp10/4095);
    voltage_dc = 5.0*voltage_dc;//5倍关系还原
    voltage_dc = 2.0*voltage_dc;//对照直流输出关系进行计算(一次电流（电压）10MA -> 二次电压 5V)
    return voltage_dc;*/
    
    for(i=0; i<40; i++)
	{
		//Temp10 = (float)Result_Vdc[i];
        //voltage_dc = 10*(Temp10/4095) + voltage_dc;		//对照直流输出关系进行计算
        voltage_dc = voltage_dc + (float)Result_Vdc[i];
	}
    Temp10 = voltage_dc/40;
    voltage_dc = VREF*(Temp10/4095);
    voltage_dc = 5.0*voltage_dc;//5倍关系还原
    voltage_dc = 2.0*voltage_dc;//对照直流输出关系进行计算(一次电流（电压）10MA -> 二次电压 5V)
	return voltage_dc;
}

double Calculate_Uac(void)
{
	int i,j;
	double temp=0,Squ=0,temp2;
	float Base = 0;
	float Temp;	

	for(samplingTimes=0; samplingTimes<40; samplingTimes++)
	{
		Temp = (float)Result_Vac[samplingTimes];
		Base = Temp + Base;
	}
	Base = Base/40;
	i = (int)Base;
	if((Base-(float)i)<0.5)
		Base = (int)Base;
	else
		Base = (int)Base + 1;
	
	for(j=0; j<40; j++)
	{
		//TempTemp[j] = Result_Vac[j];			//调试用
		Result_Vac[j] = Result_Vac[j] - Base;
	}
	
	for(j=0; j<40; j++)
	{
		Squ = (double)Result_Vac[j];
		//Squ = 1000*(Squ/4095);//变比改为500-2圈,1000-1圈
        Squ = VREF*(Squ/4095);
		//Squ = Squ/DivisionValue;
		temp2 = Squ * Squ;
		temp = temp + temp2;		
	}
	temp = temp/40;
	temp = sqrt(temp);
	
    temp2 = 5.0*temp;   //5倍关系还原
    temp  = 100*temp2;  //对照交流输出关系进行计算(一次电流（电压）500ma->二次电压5V)
	return temp;
}

double Calculate_Ul(void)
{
	int i,j;
	double temp=0,Squ=0,temp2;
	float Base = 0;
	float Temp;	

	for(samplingTimes=0; samplingTimes<40; samplingTimes++)
	{
		Temp = (float)Result_Vl[samplingTimes];
		Base = Temp + Base;
	}
	Base = Base/40;
	i = (int)Base;
	if((Base-(float)i)<0.5)
		Base = (int)Base;
	else
		Base = (int)Base + 1;
	
	for(j=0; j<40; j++)
	{
		//TempTemp[j] = Result_Vac[j];			//调试用
		Result_Vl[j] = Result_Vl[j] - Base;
	}
	
	for(j=0; j<40; j++)
	{
		Squ = (double)Result_Vl[j];
		//Squ = 1000*(Squ/4095);//变比改为500-2圈,1000-1圈
        Squ = VREF*(Squ/4095);
		//Squ = Squ/DivisionValue;
		temp2 = Squ * Squ;
		temp = temp + temp2;		
	}
	temp = temp/40;
	temp = sqrt(temp);
	//Vac = Vac * 0.97;
	//temp = 500*(temp/4095);

    temp2 = temp/2.0;//缩小2倍关系
    temp = temp2*1000;//对照交流输出关系进行计算
    
	return temp;
}

/*******************************************************************************
* Float转换成数字
* 有效位-8位
********************************************************************************/
/*void float2char(unsigned int g, unsigned int l, float number, char *str)
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
}*/

/*******************************************************************************
 * eepromData_set.pre_ac = calculation(eepromData_set.pre_ac,system_cursor+1,plus_minus);//0-减,1-加
********************************************************************************/
int calculation(int data, int bitNo, unsigned char flag_cursor)
{
	int calcResult=0;
	int calcTemp=1;
	int temp[4];
	int j;

	temp[3] = data/1000;
	temp[2] = data%1000/100;
	temp[1] = data%1000%100/10;
	temp[0] = data%1000%100%10;

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

//    for(j=0;j<4;j++){
//        calcResult |= temp[3-j]<<j*4;
//    }
//	for(j=0;j<4;j++)
//	{
//		if(j==0)
//			calcTemp = 1;
//		else
//			calcTemp = calcTemp*10;
//			
		calcResult = temp[3]*1000 +temp[2]*100 +temp[1]*10 + temp[0]*1  ;
//	}

	return calcResult;
}

float Calculate_Base(void)
{
	int i;
	float Base = 0;
	float Temp;

	for(samplingTimes=0; samplingTimes<40; samplingTimes++)
	{
		Temp = (float)Result_Vac[samplingTimes];
		Base = Temp + Base;
	}
	Base = Base/40;
	i = (int)Base;
	if((Base-(float)i)<0.5)
		Base = (int)Base;
	else
		Base = (int)Base + 1;

	Base = VREF*(Base/4095);
	return Base;
}

 /*
    disn(4,0,55);disn(5,0,69);disn(6,0,76);disn(7,0,67);disn(8,0,79);disn(9,0,77);disn(10,0,69);//Welcome
    disn(0,1,68);disn(1,1,83);disn(2,1,48);disn(3,1,41);disn(4,1,35);disn(5,1,3);disn(6,1,3);disn(7,1,37);disn(8,1,48);//dsPIC33EP
    disn(10,1,51);disn(11,1,89);disn(12,1,83);disn(13,1,84);disn(14,1,69);disn(15,1,77);
    disn(0,2,38);disn(1,2,79);disn(2,2,83);disn(3,2,67);disn(4,2,26);//Fosc:
    disn(5,2,7);disn(6,2,0);disn(7,2,45);disn(8,2,41);disn(9,2,48);disn(10,2,51);//70MIPS
    disn(0,3,36);disn(1,3,65);disn(2,3,84);disn(3,3,69);disn(4,3,26);disn(5,3,2);
    disn(6,3,0);disn(7,3,1);disn(8,3,5);disn(9,3,24);disn(10,3,1);disn(11,3,2);disn(12,3,24);disn(13,3,0);disn(14,3,2);
    while(1)
    {
        i ++;
        j = i;
        j <<= 10;
        PORTB = j;//0
        Delay_1ms(1000);
        disn(6,3,0);disn(7,3,1);disn(8,3,5);disn(9,3,24);disn(10,3,1);disn(11,3,2);disn(12,3,24);disn(13,3,i%100/10);disn(14,3,i%10);
    }
    */

/*void Interrupt_Int1_Init(void)
{
    //RPINR0bits.INT1R = 55;//INT1输入连接到RPI55
    //TRISCbits.TRISC7 = 1;//RC7按键输入
    //CNPUCbits.CNPUC7 = 1;//RC7弱上拉使能
    //CNPDCbits.CNPDC7 = 0;//RC7弱下拉禁止 
    
    RPINR0bits.INT1R = 54;//INT1输入连接到RPI54
    TRISCbits.TRISC6 = 1;//RC6按键输入
    
    INTCON1bits.NSTDIS = 0;//中断嵌套禁止
    IPC5bits.INT1IP = 7;//中断最高优先级
    IFS1bits.INT1IF = 0;//外中断标志清零
    IEC1bits.INT1IE = 1;//允许INT1外中断
    INTCON2bits.INT1EP = 1;//下降沿中断
    
    CNPUCbits.CNPUC6 = 1;//RC6弱上拉使能
    CNPDCbits.CNPDC6 = 0;//RC6弱下拉禁止
    
    INTCON2bits.GIE = 1;//全局中断允许
}*/