/*
 * File:   page.c
 * Author: wet_chair
 *
 * Created on May 13, 2018, 10:58 AM
 */


#include "xc.h"

void Welcome_Page(void)
{
    //JDZ-2
    disn(5,0,42);
    disn(6,0,36);
    disn(7,0,58);
    disn(8,0,23);
    disn(9,0,2);
    
    //IT©���·��
    disn(2,1,41);
    disn(3,1,52);
    disn_hanzi(4,1,13);
    disn_hanzi(6,1,6);
    disn_hanzi(8,1,14);
    disn_hanzi(10,1,15);
    disn_hanzi(12,1,16);
    
    //�����и�����
    disn_hanzi(2,2,0);
    disn_hanzi(4,2,1);
    disn_hanzi(6,2,2);
    disn_hanzi(8,2,3);
    disn_hanzi(10,2,4);
    disn_hanzi(12,2,5);
    
    //�����Զ�����˾
    disn_hanzi(1,3,6);
    disn_hanzi(3,3,7);
    disn_hanzi(5,3,8);
    disn_hanzi(7,3,9);
    disn_hanzi(9,3,10);
    disn_hanzi(11,3,11);
    disn_hanzi(13,3,12);
}

void Menu_Page(void)
{
    //���� -----ֱ��
    disn_hanzi(0,0,37);
    disn_hanzi(2,0,17);
    
    disn_hanzi(8,0,36);
    disn_hanzi(10,0,17);
    
    //��ʱ -----�㷨ֱ��
    disn_hanzi(0,1,28);
    disn_hanzi(2,1,22);
    
    disn_hanzi(8,1,38);
    disn_hanzi(10,1,39);
    disn_hanzi(12,1,36);
    disn_hanzi(14,1,17);
    
    //���� -----ʱ��---����
    disn_hanzi(0,2,29);
    disn_hanzi(2,2,30);
    
    disn_hanzi(8,2,22);
    disn_hanzi(10,2,23);
    disn_hanzi(12,2,24);
    disn_hanzi(14,2,25);
    
    //��� -----����---У��
    disn_hanzi(0,3,18);
    disn_hanzi(2,3,19);
    
    disn_hanzi(8,3,20);
    disn_hanzi(10,3,21);
    disn_hanzi(12,3,40);
    disn_hanzi(14,3,31);
}

void Trial_Page(void)
{
    //1���� -----2����
    
    //ֱ��  -----��ϵ
    
    //�ݷ�       ����
    
    //�������
    
}

void Adjust_Page(void)
{
    //�������
    disn_hanzi(0,0,31);
    disn_hanzi(2,0,32);
    disn_hanzi(4,0,6);
    disn_hanzi(6,0,17);
   
    //ֱ������
    disn_hanzi(0,1,36);
    disn_hanzi(2,1,17);
    disn_hanzi(4,1,6);
    disn_hanzi(6,1,17);
    
    //ϵͳ��ѹ
    disn_hanzi(0,2,34);
    disn_hanzi(2,2,35);
    disn_hanzi(4,2,6);
    disn_hanzi(6,2,33);
}

void Action_Lock_Page(void)
{
    //©�����
    
    //ֱ��   ����
    
    //����  ʱ��
}

void Action_Break_Page(void)
{
    //©����բ
            
    //ֱ��  ����
    
    //����
}
