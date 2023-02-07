#ifndef	MENU_SETTING_H
#define MENU_SETTING_H

void Move_System_Position(unsigned char no,unsigned char mode);
void Move_System_Cursor(unsigned char system_no, unsigned char system_cursor);
void System_Page_Data(void);
void Load_Sys_Setting(void);
void Save_Sys_Setting(void);
void Menu_Setting(void);
void Select_Setting_System(unsigned char no);

void Algorithm_Setting_Enter(void);
void Action_Setting_Enter(void);
void Baud_Rate_Setting_Enter(void);

void Judge_Is_Save_Page(void);
void Select_Save(unsigned char x);

void modify_system_setting(unsigned char system_no, unsigned char system_cursor, unsigned char plus_minus);


//void error_list(unsigned char listNu);
//
//void zero_setting(void);
#endif

