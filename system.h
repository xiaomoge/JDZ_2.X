#ifndef SYSTEM_H
#define	SYSTEM_H
//#include "P33ep32mc202.h"
//#include "P33ep128gp504.h"
#include "syschip.h"
//����λ
// FICD
#pragma config ICS = PGD2               // ICDͨ��ͨ��ѡ��λ��PGEC1��PGED1ͨѶ��
#pragma config JTAGEN = OFF             // ��ֹJTAG
// FPOR
#pragma config ALTI2C1 = OFF            // ����I2C1���ţ�I2C1ӳ�䵽SDA1 / SCL1���ţ�
#pragma config ALTI2C2 = OFF            // ����I2C2���ţ�I2C2ӳ�䵽SDA2 / SCL2���ţ�
#pragma config WDTWIN = WIN25           // ���Ź�����ѡ��λ��WDT����ΪWDT���ڵ�25����
// FWDT
#pragma config WDTPOST = PS32768        // ���Ź���ʱ�����Ƶ�ȣ�1��32,768��
#pragma config WDTPRE = PR128           // ���Ź���ʱ��Ԥ��Ƶ��λ��1��128��
#pragma config PLLKEN = ON              // PLL����ʹ��λ��PLLԴ��ʱ���л����ȴ�PLL�����ź���Ч����
#pragma config WINDIS = OFF             // ���Ź���ʱ������ʹ��λ���Ǵ���ģʽ�µĿ��Ź���ʱ����
#pragma config FWDTEN = OFF              // ���Ź���ʱ��ʹ��λ�����Ź���ʱ��ʼ��ʹ�ܣ�

// FOSC
#pragma config POSCMD = XT            // ������ģʽѡ��λ�����������ã�
#pragma config OSCIOFNC = OFF           // OSC2���Ź���λ��OSC2Ϊʱ�������
#pragma config IOL1WAY = ON             // ��������ѡ�����ã�������һ���������ã�
#pragma config FCKSM = CSDCMD           // ʱ���л�ģʽλ��ʱ���л��͹��ϰ�ȫʱ�Ӽ������������ã�

// FOSCSEL
#pragma config FNOSC = PRIPLL          // �� PLL ģ��������� ��XT + PLL�� HS + PLL �� EC + PLL��
//#pragma config PWMLOCK = ON             // PWM����ʹ��λ��ĳЩPWM�Ĵ���ֻ���ڰ�������֮��д�룩
#pragma config IESO = ON                // ˫����������ʹ��λ��ʹ��FRC����������Ȼ���л����û�ѡ�������Դ��

// FGS
#pragma config GWRP = OFF               // ͨ�ö�д����λ��ͨ�öο�д��
#pragma config GCP = OFF                // ͨ�öδ��뱣��λ��ͨ�öδ��뱣���ѽ��ã�
#endif


