#include "adc.h"
#include "syschip.h" 
#include "eeprom.h"
#define FCY  20000000	 				// xtal = 10Mhz; 10MHz /PLLx8 -> 20 MIPS
extern void __delay32(unsigned long cycles);
#if !defined(FCY)
extern void __delay_ms(unsigned long);
extern void __delay_us(unsigned long);
#else
#define __delay_ms(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000ULL)); }
#define __delay_us(d) \
  { __delay32( (unsigned long) (((unsigned long long) d)*(FCY)/1000000ULL)); }
#endif


//extern unsigned char algorithm;
extern eeprom_data eepromData;
extern unsigned char flagSamplingType;
extern unsigned char samplingTimes;
extern unsigned char flag_detection;

extern float DCActionCompareValue;
extern float ACActionCompareValue;

//extern unsigned char ACSubdivisionType;

extern float Base;
extern float Init_Base;
extern float VREF;

extern int Result_Vl[40];
extern int Result_Vdc[40];
extern int Result_Vac[40];

extern unsigned char flag_Vdc;
extern unsigned char flag_Vac;
//-----------------------------------------
//-----------------------------------------
unsigned int SampleValue_Udc;
unsigned int SampleValue_Uac;
unsigned int SampleValue_Ul;

void InitADC(void)
{
    TRISAbits.TRISA0 = 1;//AN0/RA0作为输入
	TRISAbits.TRISA1 = 1;//AN1/RA1作为输入
	TRISBbits.TRISB0 = 1;//AN2/RB0作为输入
    
    ANSELAbits.ANSA0 = 0;//Disable AN0/RA0 is analog
    ANSELAbits.ANSA1 = 0;//Disable AN1/RA1 is analog
    ANSELBbits.ANSB0 = 0;//Disable AN2/RB0 is analog
     
    AD1CON1 = 0x0000;
    AD1CON1bits.AD12B = 1;//1 = 12 位1 通道ADC 工作
    AD1CON1bits.FORM = 0b00;//00 = 整数（DOUT = 0000 dddd dddd dddd）
    
    //AD1CON1bits.ASAM = 1;//最后一次转换结束后立即开始采样。 SAMP 位自动置 1。
    //AD1CON1bits.SSRC = 7;//内部计数器结束采样并开始转换（自动转换）
    
    AD1CON2 = 0x0000;
    AD1CON2bits.VCFG = 0b000;//AVDD AVSS参考电源
    AD1CON2bits.SMPI = 0b0000;//当ADDMAEN = 0 时：
                              //x0000 = 每完成1 次采样/ 转换操作后产生中断
                              //当ADDMAEN = 1 时：
                              //00000 = 每完成1 次采样/ 转换操作后将DMA 地址递增1
    
    AD1CON3 = 0x0000;
    AD1CON3bits.ADCS = 0xff;//ADC 转换时钟选择位
                            //TP • (ADCS<7:0> + 1) = TP •256 = TAD
    AD1CON4 = 0x0000;
    AD1CON4bits.DMABL = 0b000;//000 = 为每个模拟输入分配1 字的缓冲区
    
    AD1CSSL = 0x0000;//不对输入引脚扫描
   
    AD1CON1bits.ADON = 1;//使能AD模块
}

/************************************************************************************************************/
//AD转换
void GetResult_Udc(void)
{   
    ANSELAbits.ANSA0 = 1;
    ANSELAbits.ANSA1 = 0;
    ANSELBbits.ANSB0 = 0;
    
	AD1CON1bits.SAMP = 1; 			// start sampling ...
	//DelayMS(5); 					// for 100 mS
	//DelayUS(100);
	//DelayUS(200);
	__delay_us(100);
	AD1CON1bits.SAMP = 0; 			// start Converting
	while (!AD1CON1bits.DONE); 		// conversion done?
	SampleValue_Udc = ADC1BUF0; 	// yes then get ADC value
}

void GetResult_Ul(void)
{   	
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 1;
    ANSELBbits.ANSB0 = 0;
    
	AD1CON1bits.SAMP = 1; 			// start sampling ...
                                    //SAMP： ADC 采样使能位
                                    //1 = ADC 采样/ 保持放大器正在采样
                                    //0 = ADC 采样/ 保持放大器在保持采样结果
                                    //如果ASAM = 0，由软件写入1 开始采样。如果ASAM = 1，该位由硬件自动置1。如果SSRC<2:0>
                                    //= 000，由软件写入0 结束采样并启动转换。如果SSRC<2:0> ≠ 000，由硬件自动清零来结束采样并启动
                                    //转换。
    
	//DelayMS(1); 					// for 100 mS
	//DelayUS(100);
	//DelayUS(200);
	//__delay_us(100);
	__delay_us(100);
	AD1CON1bits.SAMP = 0; 			// start Converting
	while (!AD1CON1bits.DONE); 		// conversion done?
                                    //DONE：ADC 转换状态位(3)
                                    //1 = ADC 转换完成
                                    //0 = ADC 转换尚未开始或在进行中
                                    //当A/D 转换完成时，由硬件自动置1。可由软件写入0 来清零DONE 状态位（不允许由软件写入
                                    //1）。清零该位不会影响进行中的任何操作。在新的转换开始时由硬件自动清零。
    
	SampleValue_Ul = ADC1BUF0;     // yes then get ADC value
}

void GetResult_Uac(void)
{   
    ANSELAbits.ANSA0 = 0;
    ANSELAbits.ANSA1 = 0;
    ANSELBbits.ANSB0 = 1;
    
	AD1CON1bits.SAMP = 1; 			// start sampling ...
	//DelayMS(5); 					// for 100 mS
	//DelayUS(100);
	//DelayUS(200);
	__delay_us(100);
	AD1CON1bits.SAMP = 0; 			// start Converting
	while (!AD1CON1bits.DONE); 		// conversion done?
	SampleValue_Uac = ADC1BUF0; 	// yes then get ADC value
}

/**********************************************************
 * 
 * 
 **************************************************/
// Timer2 interrupt every 500uS 
//void __attribute__((interrupt, no_auto_psv)) _T2Interrupt(void)
void __attribute__((__interrupt__, no_auto_psv)) _T2Interrupt(void)  
{
	/********************************************
	********************************************/
	float Temp;

	TMR2 = 0;
	IFS0bits.T2IF = 0;			// Clear Timer2 interrupt flag

    //线电压采集
    SampleValue_Ul = 0;
    AD1CHS0 = 0x0001;
    GetResult_Ul();
    Result_Vl[samplingTimes] = SampleValue_Ul;
    
    //---------------算法一(每一点判断)-----------------------------
 	if(eepromData.algorithm == 0 && flagSamplingType == 1)
	{   
        //直流采集
		SampleValue_Udc = 0;
		AD1CHS0 = 0x0000;
		GetResult_Udc();
        
		Temp = VREF*((float)SampleValue_Udc/4095);
        Temp = 5.0*Temp;//5倍关系还原
        Temp = 2.0*Temp;//对照直流输出关系进行计算(一次电流（电压）10MA -> 二次电压 5V)
		if(Temp>= (DCActionCompareValue/2)){
			flag_detection = 1;			
        }
        
        //交流采集
		SampleValue_Uac = 0;
		AD1CHS0 = 0x0002;
		GetResult_Uac();

		Result_Vdc[samplingTimes] = SampleValue_Udc;
        Result_Vac[samplingTimes] = SampleValue_Uac;
	}

    //---------------算法一(40点计算)-----------------------------
	if(eepromData.algorithm == 0 && flagSamplingType == 0)
	{
        //直流采集
        SampleValue_Udc = 0;
		AD1CHS0 = 0x0000;
		GetResult_Udc();
        
        //交流采集
		SampleValue_Uac = 0;
		AD1CHS0 = 0x0002;
		GetResult_Uac();		
		
		Result_Vdc[samplingTimes] = SampleValue_Udc;
		Result_Vac[samplingTimes] = SampleValue_Uac;
	}
	
    //---------------算法二(每一点判断)-----------------------------
	if(eepromData.algorithm == 1 && flagSamplingType == 1)
	{
        //直流采集
        SampleValue_Udc = 0;
		AD1CHS0 = 0x0000;
		GetResult_Udc();
        
        //交流采集
		SampleValue_Uac = 0;
		AD1CHS0 = 0x0002;
		GetResult_Uac();
		
		Temp = VREF*((float)SampleValue_Uac/4095);		//对照交流输出关系进行计算
		if(Temp>=Init_Base){
			Temp = Temp-Init_Base;
        }
		else {
			Temp = Init_Base-Temp;
        }
		//Temp = 1000*(Temp/5);//变比改为500-2圈,1000-1圈
        Temp = 5.0*Temp;    //5倍关系还原
        Temp = 100*Temp;    //对照交流输出关系进行计算(一次电流（电压）500ma->二次电压5V)
		
		if(Temp>=(ACActionCompareValue/2)){
			flag_detection = 1;	
        }

        Result_Vdc[samplingTimes] = SampleValue_Udc;
		Result_Vac[samplingTimes] = SampleValue_Uac;
	}

    //---------------算法二(40点计算)-----------------------------
	if(eepromData.algorithm == 1 && flagSamplingType == 0)
	{
        //直流采集
        SampleValue_Udc = 0;
		AD1CHS0 = 0x0000;
		GetResult_Udc();
        
        //交流采集
		SampleValue_Uac = 0;
		AD1CHS0 = 0x0002;
		GetResult_Uac();		
		
		Result_Vdc[samplingTimes] = SampleValue_Udc;
		Result_Vac[samplingTimes] = SampleValue_Uac; 
	}
	
    //---------------算法三(每一点判断)-----------------------------
	if(eepromData.algorithm == 2 && flagSamplingType == 1)
	{	
		//交流信号的处理
		SampleValue_Uac = 0;
        AD1CHS0 = 0x0002;
		GetResult_Uac();

        Temp = VREF*((float)SampleValue_Uac/4095);		//对照交流输出关系进行计算
		if(Temp>=Init_Base){
			Temp = Temp-Init_Base;
        }
		else {
			Temp = Init_Base-Temp;
        }
		//Temp = 1000*(Temp/5);//变比改为500-2圈,1000-1圈
        Temp = 5.0*Temp;    //5倍关系还原
        Temp = 100*Temp;    //对照交流输出关系进行计算(一次电流（电压）500ma->二次电压5V)
        
		if(Temp>= (ACActionCompareValue/2)){
			flag_detection = 1;
        }
        
		////////////////////////////////////////////////////////////////////////	
		
		SampleValue_Udc = 0;		//直流信号的处理
		AD1CHS0 = 0x0000;
		GetResult_Udc();

        Temp = VREF*((float)SampleValue_Udc/4095);
        Temp = 5.0*Temp;//5倍关系还原
        Temp = 2.0*Temp;//对照直流输出关系进行计算(一次电流（电压）10MA -> 二次电压 5V)
		if(Temp>= (DCActionCompareValue/2)){
			flag_detection = 1;			
        }

        Result_Vac[samplingTimes] = SampleValue_Uac;
		Result_Vdc[samplingTimes] = SampleValue_Udc;		
	}

    //---------------算法三(40点计算)-----------------------------
	if(eepromData.algorithm == 2 && flagSamplingType == 0)
	{	
        SampleValue_Uac = 0;
        AD1CHS0 = 0x0002;
        GetResult_Uac();
	
		////////////////////////////////////////////////////////////////////////	
		
		SampleValue_Udc = 0;		//直流信号的处理
		AD1CHS0 = 0x0000;
		GetResult_Udc();	

        Result_Vac[samplingTimes] = SampleValue_Uac;
		Result_Vdc[samplingTimes] = SampleValue_Udc;		
	}

    //LATBbits.LATB3=~LATBbits.LATB3; 
	samplingTimes++;	
}
