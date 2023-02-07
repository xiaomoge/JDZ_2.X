#include "math.h"
#include "keyboard.h"
#include "oled.h"
#include "welcome.h"
#include "menu_main.h"
#include "menu_setting.h"
#include "menu_running.h"
#include "menu_trial.h"
#include "menu_adjust.h"
#include "menu_time.h"
#include "menu_log.h"
#include "time1.h"
#include "syschip.h"
#include "eeprom.h" 
#include "stdbool.h"

#define new_keyFunction

extern void Delay_1ms(unsigned int t);

unsigned char run_flag = 1;			//运行菜单和系统菜单切换标志位
unsigned char menu_class = 1;		//菜单层级
//unsigned int key_value;
unsigned char menu_no = 0;			
unsigned char system_no = 0;
unsigned char system_cursor = 0;
unsigned char setting_system_flag = 0;

//volatile unsigned char get_keyvalue1_flag;  
//volatile unsigned char get_keyvalue2_flag; 
//volatile unsigned char get_keyvalue3_flag;  
//volatile unsigned char get_keyvalue4_flag; 
bool keyPressedFlag = false;
bool saving_flag = true;
bool setting_going = false;
unsigned char time_cursor = 0;
//unsigned char is_save_flag = 0;
//unsigned char is_save_hor = 1;

extern float resultVdc;
extern float resultVac;

extern float preResultVdc;
extern float preResultVac;

extern float testResultVdc;
extern float testResultVac;
extern float testI1;
extern float testI2;

extern unsigned char samplingTimes;
extern unsigned char flagSamplingType;
//extern unsigned char algorithm;
extern eeprom_data eepromData;
unsigned int time1Times;

////////////////////////////////////////
/*
typedef enum{
          none_sub,
          setting_menu,
          timer_menu,
          fault_menu,
          clear_meanu,
          lab_menu,
          zero_calibration_menu
}sub_menu;

typedef enum{
            none_primary,
            primary_menu
}running_primary_menu ;


 
typedef enum{
            start_menu,
            running_menu,
            protect_menu   
}first_level_menu ;
typedef struct{
    first_level_menu firstLevel;
    running_primary_menu runningPrimaryMenu;
    sub_menu subMenu;    
    
}menu_level;
menu_level menuLevel;

*/

typedef enum{
          welcom_menu,
          running_menu,
          major_menu,
          setting_menu,
          timer_menu,
          fault_menu,
//          clear_meanu,
          trial_menu,
          zeroCalibration_menu,
          save_menu,
}normal_menu;

typedef enum{
         none,
         LeakageLockout_menu, 
         instantaneousLeakage_menu, 
         leakageFault_menu,
         leakageOccurrence_menu,
         leakageTripping_menu, 
    
}faultTriggle_menu;

    normal_menu normalMenu;
    faultTriggle_menu faultTriggleMenu;
 
key_type keyValue;
//------------------------------------------------------------------------------

//void __attribute__((__interrupt__, auto_psv)) _INT1Interrupt(void)
void __attribute__ ((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    Delay_1ms(50);//按键消抖
    //IFS1bits.INT1IF = 0;//清中断标志
    //PORTB =~ PORTB;
  
    if(PORTCbits.RC6 == 0 || PORTCbits.RC7 == 0 || PORTCbits.RC8 == 0 || PORTCbits.RC9 == 0)
    {
        Delay_1ms(50);//按键消抖 
      
        if(PORTCbits.RC6 == 0)//Enter
        {
            keyValue = Enter;//5;  
            keyPressedFlag = true;
        }
        else if(PORTCbits.RC7 == 0)
        {
            keyValue = Escape;// 6; 
            keyPressedFlag = true;
        }
        else if(PORTCbits.RC8 == 0)
        {
            keyValue = Plus;//2; 
            keyPressedFlag = true;
        }
        else if(PORTCbits.RC9 == 0)
        {
            keyValue = Shift;//1;
            keyPressedFlag = true;
        }
    }
    
    //Key_Func();
    
    IFS1bits.CNIF = 0; // Clear CN interrupt flag	
}

void Get_Keyboard_Running(void)
{
    //Move
//    if(get_keyvalue1_flag == 1)   
//        get_keyvalue1_flag = 0;
//
//    //Plus
//    if(get_keyvalue2_flag == 1)  
//        get_keyvalue2_flag = 0;
//
//    //Enter
//    if(get_keyvalue3_flag == 1)
//        get_keyvalue3_flag = 0;
//
//    //Escape
//    if(get_keyvalue4_flag == 1)
//        get_keyvalue4_flag = 0;
    if(keyPressedFlag == true)keyPressedFlag = false;
}
 
 
void majorMenu_running(void){
    
     Menu_Main(menu_no);
     
}
void saveMenu_running(void){
            Judge_Is_Save_Page();
            Select_Save(saving_flag); 
}
 void saveMenu(void){
    if(keyValue == Shift) {
        saving_flag =  !saving_flag;
        Select_Save(saving_flag);
    }
    
    if(keyValue == Enter)//5)//enter按键按下
    {
            OLED_Clear();
            Save_Sys_Setting();//保存设置值
            Put_Sys_Set(); //保存系统设置       
            Menu_Main(menu_no);             //装置设置菜单
            normalMenu = major_menu;
        
    }
//            is_save_flag = 0;
//
//            if(menu_no == 0)//系统设置
//            {
//                if(is_save_hor==0)
//                {
//                    Save_Sys_Setting();//保存设置值
//                    Put_Sys_Set(); //保存系统设置
//                }
////                
//                system_no=0;
//                
//            }
//            
//            if(menu_no == 1)//时间设置
//            {
//                if(is_save_hor==0)
//                {
//                    /*if(time_setting_flag==1)
//                    {
//                        time_setting_flag=0;
//                        //周不用写0
//                        Set_Rtc(0,year_set,month_set,day_set,hour_set,minute_set,second_set);
//                    }*/
////                }
//                
////                time_cursor=0;
////            }
//            
//            OLED_Clear();//清屏
//            menu_class = 1;					//置层级2标志位
//            keyValue = None;//0;					//清除键值
//            Menu_Main(menu_no);             //装置设置菜单
//            normalMenu = major_menu;
//        }
        	
        if(keyValue == Escape)//6)	//按下系统键返回值层级2
        {
            OLED_Clear();//清屏 
            Menu_Main(menu_no);             //装置设置菜单
            normalMenu = major_menu;
        }

}
void menu_init(void){
    normalMenu = welcom_menu;
    faultTriggleMenu = none;
}
void welcomMenu(void){
   if(keyValue == Enter){
//            keyValue = None;//0;	//清除键值
            OLED_Clear();	//清屏
            Running_Menu(); // 
            normalMenu = running_menu;
      }
 }
void runningMenu(void){
    
    if(keyValue == Enter){
            normalMenu = major_menu;
//            keyValue = None;//0;	//清除键值
            OLED_Clear();	//清屏
            majorMenu_running(); //运行菜单
    }
}

 void MajorMenu(void){
     
        if(keyValue == Shift){                
                menu_no = (menu_no<5)?(menu_no++):0;//1: // shift button                    
                Select_Main_Menu(menu_no);//设置，时间，故障，清除，试验，校零
          }        

        if(keyValue == Enter)//5)	//当Enter键按下时,进入下一级菜单 
        { 
            OLED_Clear();	//清屏
            switch(menu_no)
            {
                case 0:
                    normalMenu = setting_menu;
                    Load_Sys_Setting(); //加载设置值
                    Menu_Setting();     //设置界面
                    Move_System_Position(0,1);
                    break;
                case 1:
                    //时间界面
                    normalMenu = timer_menu;
                    Setting_Time_Page();
                    break;
                case 2:
                    //故障界面
                    normalMenu = fault_menu;
                    Menu_Log();   
                    break;
                case 3:
                    //清除界面
                    normalMenu = major_menu;
                    Clear_Success();
                    break;
                case 4:
                    //试验界面
                    normalMenu = trial_menu;
                    Menu_Trial();
                    break;
                case 5:
                    //校零操作
                    normalMenu = zeroCalibration_menu;
                    Menu_Adjust();
                    break;
                default:
                    break;
            }			
        }

        if(keyValue == Escape)//6)	//当系统键按下时,返回至运行 escape /cancel
        {
            OLED_Clear();	//清屏
            Running_Menu(); //运行菜单
            normalMenu = running_menu;
            
        }

}

 void bitChange(unsigned char cursorNo){
     
       switch(keyValue){
        
            case Shift:///1:
                (system_cursor<cursorNo)?(system_cursor++):0;
                Move_System_Cursor(system_no, system_cursor);
                break;
            case Plus://2:
                modify_system_setting(system_no, system_cursor, 1);
                break;
            case Enter:setting_going = !setting_going;Select_Setting_System(system_no);system_cursor=0;
                break;
//           case Escape:Move_System_Position(system_no, 1);	break;
            default:
                break;
        }
//        if(keyValue == Enter)//5)	//enter按键按下
//        {
//            ------------------------------------------------------------
//            ---再次按下，需要保存，退出menu_class = 3;
//            ------------------------------------------------------------
//            keyValue = None;//0;					//清除键值
//            menu_class = 2;					//置层级2标志位
//            setting_system_flag = 0;		//清标志位
//
//            system_cursor = 0;
//            Move_System_Position(system_no, 1);	
//        }
//
//        if(keyValue == Escape)//6)//按下系统键返回置层级2
//        {
//            menu_class = 2;					//置层级2标志位
//            keyValue = None;//0;					//清除键值
//            setting_system_flag = 0;		//清标志位
//
//            system_cursor = 0;
//            Move_System_Position(system_no, 1);
//        }
 }
 void settingMenu(void){   
     
     if(setting_going){
         
         switch(system_no){
             case 0:bitChange(3); break;
             case 1:bitChange(2);break;
             case 2:bitChange(4);break;
             case 3:bitChange(4);break;
             case 7:bitChange(4);break;
             default:break;             
         }
     }else{
         
       if(keyValue == Shift){
                  (system_no<7)?(system_no++):0;
                   Select_Setting_System(system_no);
       }

      if(keyValue == Enter)//5)	//当enter键按下时，进入下一界面
        {        
            switch(system_no)
            {
                case 0:setting_going = !setting_going;Move_System_Cursor(system_no, 0);break;
                case 1:setting_going = !setting_going;Move_System_Cursor(system_no, 0);break;
                case 2:setting_going = !setting_going;Move_System_Cursor(system_no, 0);break;
                case 3:setting_going = !setting_going;Move_System_Cursor(system_no, 0);break;
                case 4://算法
                    Algorithm_Setting_Enter();break;                    
                case 5://动作
                    Action_Setting_Enter();break;                    
                case 6://BTL
                    Baud_Rate_Setting_Enter();break;                    
                 case 7:setting_going = !setting_going;Move_System_Cursor(system_no, 0);break;
                default:
                    Move_System_Cursor(system_no, 0);
                    break;
            }
        }
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
            OLED_Clear();//清屏
            system_no = 0;
            system_cursor= 0;
            normalMenu = save_menu;
            saveMenu_running();
        }
     }
      
 }
 void timerMenu(void){
          switch(keyValue){
            case Shift: time_cursor = (time_cursor<9)?(time_cursor++):0;
//
                          Move_Time_Cursor(time_cursor);
                          break;
            case Plus:    modify_time_setting(time_cursor, 1);
                          break;
//            case Enter: OLED_Clear();break;
            case Escape:  OLED_Clear(); normalMenu = save_menu; saveMenu_running();
                          break;
            default:      break;
                
        }
//        keyValue = None;        
//        -------------进入退出保存界面-------------
//        if(keyValue == Escape || keyValue == Enter)//6)//
//        { 
//            OLED_Clear();//清屏
//            
////            menu_class = 4;		//置层级标志位
////            keyValue = None;//0;		//清除键值
////            
//            //system_no=0;
//            //Menu_Main(menu_no);
//            
//            //setting_flag = 0;   //清标志位
//            //password_auth_flag = 0;//清标志位
//
////            is_save_flag = 1;
////            is_save_hor = 0;
////            Judge_Is_Save_Page();
////            Select_Save(is_save_hor);
//        }
 }
 void faultMenu(void){
      if(keyValue == Enter)// 5)	//当enter键按下时，进入下一界面
        {
//            keyValue = None;
//            OLED_Clear();
//            menu_class = 5;
//            error_list(0);
        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
          OLED_Clear(); normalMenu = major_menu; majorMenu_running();
        }
//     keyValue = None;
 }
 void trialMenu(void){
        preResultVdc = resultVdc;
        preResultVac = resultVac;
        
        if(keyValue == Enter)//5)	//当enter键按下时，进入下一界面
        {
//            dealTrial();
//            TrialData();
        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
//            OLED_Clear();//清屏
            //delay_ms(200);

//            menu_class = 1;		//置层级标志位
//            keyValue = None;//0;		//清除键值
            OLED_Clear(); normalMenu = major_menu; majorMenu_running();
//            Menu_Main(menu_no);
        }
        keyValue = None;
 }
 void zeroCalibrationMenu(void){
     
      if(keyValue == Enter)//5)	//当enter键按下时，进入下一界面
        {
//            keyValue = None;
//            menu_class = 5;
//            zero_setting();
        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
//            OLED_Clear();//清屏
//
//            menu_class = 1;		//置层级标志位
//            keyValue = None;//0;		//清除键值
//            keyValue = None;
//            Menu_Main(menu_no);
             OLED_Clear(); normalMenu = major_menu; majorMenu_running();
        }
//      keyValue = None;
 }
void excute_KeyFunction(void)
{    
  if(keyPressedFlag == false) return;
     keyPressedFlag = false;
          
     switch(normalMenu){
         
         case  welcom_menu:   welcomMenu(); break;
         case running_menu:   runningMenu();break;
         case   major_menu:   MajorMenu(); break;
         case setting_menu:   settingMenu();break;
         case   timer_menu:   timerMenu();break;
         case   fault_menu:   faultMenu();break;
         case   trial_menu:   trialMenu();break;
         case zeroCalibration_menu:zeroCalibrationMenu();break;
         case    save_menu:   saveMenu();break;
         default: break;         
         
     }
     keyValue = None;
}
void nonon(void){
    if(menu_class == 1)		//菜单层级为1
    {
        switch(keyValue)
        {
            case Shift://1: // shift button
                if(menu_no<5)
                    menu_no++;
                else if(menu_no==5)
                    menu_no=0;
                    
                Select_Main_Menu(menu_no);//设置，时间，故障，清除，试验，校零
                break;
            default:
                break;
        }

        if(keyValue == Enter)//5)	//当Enter键按下时,进入下一级菜单 
        {
            keyValue = None;//0;	//清除键值
            menu_class = 2;	//置菜单层级标志
            OLED_Clear();	//清屏
            switch(menu_no)
            {
                case 0:
                    Load_Sys_Setting(); //加载设置值
                    Menu_Setting();     //设置界面
                    Move_System_Position(0,1);
                    break;
                case 1:
                    //时间界面
                    Setting_Time_Page();
                    break;
                case 2:
                    //故障界面
                    Menu_Log();   
                    break;
                case 3:
                    //清除界面
                    Clear_Success();
                    break;
                case 4:
                    //试验界面
                    Menu_Trial();
                    break;
                case 5:
                    //校零操作
                    Menu_Adjust();
                    break;
                default:
                    break;
            }			
        }

        if(keyValue == Escape)//6)	//当系统键按下时,返回至运行 escape /cancel
        {
            keyValue = None;//0;	//清除键值
            OLED_Clear();	//清屏
            Running_Menu(); //运行菜单
            
            run_flag = 1;	//置运行菜单标志    
        }
    }
    
    if(menu_class == 2 && menu_no == 0)//设置子菜单
    {
        switch(keyValue)
        {
            case Shift://1:
                if(system_no<7)
                    system_no++;
                else if(system_no==7)
                    system_no=0;
                Select_Setting_System(system_no);
                break;
            default:
                break;
        }

        if(keyValue == Enter)//5)	//当enter键按下时，进入下一界面
        {
            keyValue = None;// 0;	//清除键值
           
            switch(system_no)
            {
                case 4://算法
                    Algorithm_Setting_Enter();
                    break;
                case 5://动作
                    Action_Setting_Enter();
                    break;
                case 6://BTL
                    Baud_Rate_Setting_Enter();
                    break;
                default:
                    menu_class = 3;	//置菜单层级标志
                    Move_System_Cursor(system_no, 0);
                    break;
            }
        }

        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
            OLED_Clear();//清屏
            
            menu_class = 4;		//置层级标志位
            keyValue = None;//0;		//清除键值
            
            //system_no=0;
            //Menu_Main(menu_no);
            
            //setting_flag = 0;   //清标志位
            //password_auth_flag = 0;//清标志位

//            is_save_flag = 1;
//            is_save_hor = 0;
//            Judge_Is_Save_Page();
//            Select_Save(is_save_hor);
        }
    }
    
    if(menu_class == 2 && menu_no == 1)//时间子菜单
    {
        switch(keyValue)
        {
            case Shift://1:
                if(time_cursor<9)
                    time_cursor++;
                else
                    time_cursor=0;
                Move_Time_Cursor(time_cursor);
                break;
            case Plus://2:
                modify_time_setting(time_cursor, 1);
                break;
            default:
                break;
        }
        
//        if(keyValue == Enter)//5)	//当enter键按下时，进入下一界面
//        {
//            
//        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape || keyValue == Enter)//6)//
        { 
            OLED_Clear();//清屏
            
            menu_class = 4;		//置层级标志位
            keyValue = None;//0;		//清除键值
            
            //system_no=0;
            //Menu_Main(menu_no);
            
            //setting_flag = 0;   //清标志位
            //password_auth_flag = 0;//清标志位

//            is_save_flag = 1;
//            is_save_hor = 0;
//            Judge_Is_Save_Page();
//            Select_Save(is_save_hor);
        }
    }
    ////////////////////////////////////////////////////////
    if(menu_class == 2 && menu_no == 2)//故障子菜单
    {
        if(keyValue == Enter)// 5)	//当enter键按下时，进入下一界面
        {
//            keyValue = None;
//            OLED_Clear();
//            menu_class = 5;
//            error_list(0);
        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
            OLED_Clear();//清屏

            menu_class = 1;		//置层级标志位
            keyValue = None;//0;		//清除键值
            
            Menu_Main(menu_no);
        }
    }
    
    if(menu_class == 2 && menu_no == 3)//清除子菜单
    {
        if(keyValue == Enter)//5)
        {
            OLED_Clear();//清屏

            menu_class = 1;		//置层级标志位
            keyValue = None;//0;		//清除键值
            
            Menu_Main(menu_no);
        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)
        {
            OLED_Clear();//清屏

            menu_class = 1;		//置层级标志位
            keyValue = None;// 0;		//清除键值
            
            Menu_Main(menu_no);
        }
    }
    
    if(menu_class == 2 && menu_no == 4)//试验子菜单
    {
        preResultVdc = resultVdc;
        preResultVac = resultVac;
        
        if(keyValue == Enter)//5)	//当enter键按下时，进入下一界面
        {
            dealTrial();
            TrialData();
        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
            OLED_Clear();//清屏
            //delay_ms(200);

            menu_class = 1;		//置层级标志位
            keyValue = None;//0;		//清除键值
            
            Menu_Main(menu_no);
        }
    }
    //////////////////////////////////////////////////////////////
    if(menu_class == 2 && menu_no == 5)//校零子菜单
    {
        if(keyValue == Enter)//5)	//当enter键按下时，进入下一界面
        {
//            keyValue = None;
//            menu_class = 5;
//            zero_setting();
        }
        
        //-------------进入退出保存界面-------------
        if(keyValue == Escape)//6)//
        {
            OLED_Clear();//清屏

            menu_class = 1;		//置层级标志位
            keyValue = None;//0;		//清除键值
            
            Menu_Main(menu_no);
        }
    }
    
    if(menu_class == 3 && system_no == 0)//交流
    {	
        switch(keyValue)
        {
            case Shift:///1:
                if(system_cursor<2)
                    system_cursor++;
                else
                    system_cursor=0;
                Move_System_Cursor(system_no, system_cursor);
                break;
            case Plus://2:
                modify_system_setting(system_no, system_cursor, 1);
                break;
            default:
                break;
        }

        if(keyValue == Enter)//5)	//enter按键按下
        {
            //------------------------------------------------------------
            //---再次按下，需要保存，退出menu_class = 3;
            //------------------------------------------------------------
            keyValue = None;//0;					//清除键值
            menu_class = 2;					//置层级2标志位
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);	
        }

        if(keyValue == Escape)//6)//按下系统键返回置层级2
        {
            menu_class = 2;					//置层级2标志位
            keyValue = None;//0;					//清除键值
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);
        }
    }
    
    if(menu_class == 3 && system_no == 1)//直流
    {
        switch(keyValue)
        {
            case Shift://1:
                if(system_cursor==0)
                    system_cursor=1;
                else
                    system_cursor=0;
                Move_System_Cursor(system_no, system_cursor);
                break;
            case Plus://2:
                modify_system_setting(system_no, system_cursor, 1);
                break;
            default:
                break;
        }

        if(keyValue == Enter)//5)	//enter按键按下
        {
            //------------------------------------------------------------
            //---再次按下，需要保存，退出menu_class = 3;
            //------------------------------------------------------------
            keyValue = None;//0;					//清除键值
            menu_class = 2;					//置层级2标志位
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);	
        }

        if(keyValue == Escape)//6)//按下系统键返回置层级2
        {
            menu_class = 2;					//置层级2标志位
            keyValue = None;//0;					//清除键值
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);
        }
    }
    
    if(menu_class == 3 && system_no == 2)//延时
    {
         switch(keyValue)
        {
             case Shift://1:
                if(system_cursor<3)
                    system_cursor++;
                else
                    system_cursor=0;
                Move_System_Cursor(system_no, system_cursor);
                break;
             case Plus://2:
                modify_system_setting(system_no, system_cursor, 1);
                break;
            default:
                break;
        }

        if(keyValue == Enter)//5)	//enter按键按下
        {
            //------------------------------------------------------------
            //---再次按下，需要保存，退出menu_class = 3;
            //------------------------------------------------------------
            keyValue = None;//0;					//清除键值
            menu_class = 2;					//置层级2标志位
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);	
        }

        if(keyValue == Escape)//6)//按下系统键返回置层级2
        {
            menu_class = 2;					//置层级2标志位
            keyValue = None;//0;					//清除键值
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);
        }
    }
    
    if(menu_class == 3 && system_no == 3)//内阻
    {
         switch(keyValue)
        {
             case Shift://1:
                if(system_cursor<3)
                    system_cursor++;
                else
                    system_cursor=0;
                Move_System_Cursor(system_no, system_cursor);
                break;
             case Plus://2:
                modify_system_setting(system_no, system_cursor, 1);
                break;
            default:
                break;
        }

        if(keyValue == Enter)//5)	//enter按键按下
        {
            //------------------------------------------------------------
            //---再次按下，需要保存，退出menu_class = 3;
            //------------------------------------------------------------
            keyValue = None;//0;					//清除键值
            menu_class = 2;					//置层级2标志位
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);	
        }

        if(keyValue == Escape)//6)//按下系统键返回置层级2
        {
            menu_class = 2;					//置层级2标志位
            keyValue = None;//0;					//清除键值
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);
        }
    }
   
    if(menu_class == 3 && system_no == 7)//地址
    {
         switch(keyValue)
        {
             case Shift://1:
                if(system_cursor<3)
                    system_cursor++;
                else
                    system_cursor=0;
                Move_System_Cursor(system_no, system_cursor);
                break;
             case Plus://2:
                modify_system_setting(system_no, system_cursor, 1);
                break;
            default:
                break;
        }

        if(keyValue == Enter)//5)	//enter按键按下
        {
            //------------------------------------------------------------
            //---再次按下，需要保存，退出menu_class = 3;
            //------------------------------------------------------------
            keyValue = None;//0;					//清除键值
            menu_class = 2;					//置层级2标志位
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);	
        }

        if(keyValue == Escape)//6)//按下系统键返回置层级2
        {
            menu_class = 2;					//置层级2标志位
            keyValue =None;//0;					//清除键值
            setting_system_flag = 0;		//清标志位

            system_cursor = 0;
            Move_System_Position(system_no, 1);
        }
    }
    
    //-------------------------装置设置保存-------------------------
#ifndef new_keyFunction
    if(menu_class == 4 && is_save_flag == 1)//层级为4
    {
        switch(keyValue)
        {
            case Shift://1:
                if(is_save_hor==0)
                    is_save_hor=1;
                else
                    is_save_hor=0;
                Select_Save(is_save_hor);
                break;
            default:
                break;
        }
        
        if(keyValue == Enter)//5)//enter按键按下
        {
            is_save_flag = 0;

            if(menu_no == 0)//系统设置
            {
                if(is_save_hor==0)
                {
                    Save_Sys_Setting();//保存设置值
                    Put_Sys_Set(); //保存系统设置
                }
                
                system_no=0;
            }
            
            if(menu_no == 1)//时间设置
            {
                if(is_save_hor==0)
                {
                    /*if(time_setting_flag==1)
                    {
                        time_setting_flag=0;
                        //周不用写0
                        Set_Rtc(0,year_set,month_set,day_set,hour_set,minute_set,second_set);
                    }*/
                }
                
                time_cursor=0;
            }
            
            OLED_Clear();//清屏
            menu_class = 1;					//置层级2标志位
            keyValue = None;//0;					//清除键值
            Menu_Main(menu_no);             //装置设置菜单
        }
        	
        if(keyValue == Escape)//6)	//按下系统键返回值层级2
        {
//            is_save_flag = 0;
            
            if(menu_no == 0)//系统设置
                system_no=0;
            
            if(menu_no == 1)//时间设置
                time_cursor=0;

            OLED_Clear();//清屏
            menu_class = 1;					//置层级2标志位
            keyValue = None;//0;					//清除键值
            Menu_Main(menu_no);             //装置设置菜单
        }
    }
#endif
//    //error list
//    if(menu_class ==5 && menu_no == 2){
//        if(keyValue == Enter){
//            keyValue =None;
////            error_list(1);
//        }
//        if(keyValue== Escape){
//            menu_class=2;
//            keyValue = None;
//            OLED_Clear();
//            Menu_Main(menu_no);
//            Select_Main_Menu(menu_no);
//        }
//        
//    }
//    //zero_update
//    if(menu_class == 5 && menu_no == 5 ){
//        if(keyValue == Enter){
//            keyValue =None;
//            zero_setting();
//        }
//        if(keyValue== Escape){//save
//            menu_class=2;
//            keyValue = None;
//            OLED_Clear();
//            Menu_Main(menu_no);
//            Select_Main_Menu(menu_no);
//        }
//    }
}

void dealTrial(void)
{
    unsigned char savedAlgorithm = 0;
    double Vl=0;
    double Vac=0;
	float Vdc=0;
    
    double temp1000,temp1001,temp1002,temp1003;
    
    savedAlgorithm = eepromData.algorithm;
    eepromData.algorithm = 2;//交直
    
    LATBbits.LATB13 = 0;     //打开试验继电器J2
            
    time1Times=0;           //清采样次数	
    TMR1 = 0;				//定时器清零
    PR1 = 35000;
    IEC0bits.T1IE = 1; 		//使能中断
    T1CONbits.TON = 1; 		//开定时器

    while(1)
    {
        if(time1Times>=500){	
            break;
        }
    }
    IEC0bits.T1IE = 0; 	//禁止中断

    //------------------
    //采集数据
    samplingTimes=0;	//清采样次数	
    TMR2 = 0;			//定时器2清零
    PR2 = 35000;
    IEC0bits.T2IE = 1; 	//使能中断
    T2CONbits.TON = 1; 	//开定时器2

    Vdc = 0;
    Vac = 0;
    Vl = 0;
    flagSamplingType = 0;
    while(1)
    {
        if(samplingTimes==40)	
            break;		//AD转换次数达到40，则终止
    }
    IEC0bits.T2IE = 0; 	//禁止中断

    Vdc = Calculate_Udc();
    Vac = Calculate_Uac();
    Vl  = Calculate_Ul();
    testResultVac = Vac;
    flagSamplingType = 1;
    //------------------

    temp1001 = 3*Vl/Vac;
    temp1001 = temp1001*temp1001;
    temp1002 = 3*3500;
    temp1002 = temp1002*temp1002;
    temp1003 = temp1002 - temp1001;
    temp1000 = sqrt(temp1003);
    testI1 = (float)temp1000;
            
    //------------------
    LATBbits.LATB3 = 0;    //打开试验继电器J1
    time1Times=0;           //清采样次数	
    TMR1 = 0;				//定时器清零
    PR1 = 35000;
    IEC0bits.T1IE = 1; 		//使能中断
    T1CONbits.TON = 1; 		//开定时器
    while(1)
    {
        if(time1Times>=150)//30ms
        {
            LATBbits.LATB13 = 1; //释放试验继电器J2
        }

        if(time1Times>=500)//100ms
        {	
            break;
        }
    }
    IEC0bits.T1IE = 0; 	//禁止中断

     //------------------
    //采集数据
    samplingTimes=0;	//清采样次数	
    TMR2 = 0;			//定时器2清零
    PR2 = 35000;
    IEC0bits.T2IE = 1; 	//使能中断
    T2CONbits.TON = 1; 	//开定时器2

    Vdc = 0;
    Vac = 0;
    Vl = 0;
    flagSamplingType = 0;
    while(1)
    {
        if(samplingTimes==40)	
            break;		//AD转换次数达到40，则终止
    }
    IEC0bits.T2IE = 0; 	//禁止中断

    Vdc = Calculate_Udc();
    Vac = Calculate_Uac();
    Vl  = Calculate_Ul();
    testResultVdc = Vdc;
    flagSamplingType = 1;
    //------------------

    LATBbits.LATB3 = 1;    //释放试验继电器J1
    
    eepromData.algorithm = savedAlgorithm;
}