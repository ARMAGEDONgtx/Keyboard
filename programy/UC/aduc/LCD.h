/*!
	@author Michal Wróblewski
	@file LCD.h
	@date 03-11-2017
	@brief AduC831 LAB04
*/


#include "aduc831.h"
#include <stdio.h>
#include <stdarg.h>
#ifndef _LCD_H__
#define _LCD_H__


void LCD_inicjalizacja();
void LCD_wyslij(char dane, bit RS, bit RW);
void LCD_czekaj(int n);
void LCD_znak(char znak);
void Display_clear(void);
void Cursor_home(void);
void Entry_mode(int ID, int S );
void Display_ON_OFF(int D, int C,int B);
void Display_Cursor_shift(int SC, int RL);
void Function_set(int DL, int N,int F);
void CG_RAM_set(int ADRES);
void DD_RAM_set(int ADRES);
void Data_Write(int DANA);
char Data_Read();
void LCD_wyslij_str(char *str);
void Printf_LCD (const char *format, ...);
void LCD_GoTo(unsigned char x, unsigned char y);

#endif

