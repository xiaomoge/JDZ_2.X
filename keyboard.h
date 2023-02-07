#ifndef KEYBOARD_H
#define	KEYBOARD_H

//#include "P33ep128gp504.h"

#include "main.h"
void Get_Keyboard_Running(void);
//void Get_Keyboard(void);
void excute_KeyFunction(void);
void menu_init(void);
void dealTrial(void);
typedef enum{
            None,
            Plus,
            Shift,
            Enter,        
            Escape   
}key_type;

extern key_type  keyValue;
#endif