/*!
	@author Michal Wróblewski
	@file main.h
	@date 22-12-2017
	@brief deklaracje zmiennych i bibliotek 
*/
#ifndef _MAIN_H__
#define _MAIN_H__


//! biblioteki
#include <stdio.h>
#include <string.h>

//! nasze biblioteki 

#include "dio.h"
#include "functions.h"
#include "code_master.h"
#include "T1.h"


extern int UART_counter; //!< deklaracja 
extern char UART_buff[50]; //!< deklaracja 
extern int keypad_values[12]; //!< deklaracja 
extern int reply; //!< deklaracja 
extern int record; //!< deklaracja 
extern int allow_playing; //!< deklaracja 
extern char character; //!< deklaracja 
extern unsigned int potencjometr; //!< deklaracja 

#endif