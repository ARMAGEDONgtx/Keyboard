/*!
	@author Michal Wróblewski
	@file LCD.c
	@date 03-11-2017
	@brief AduC831 LAB04
*/
//#define _COMP_LCD__

#ifndef _COMP_LCD__

#ifndef _LCD_C__
#define _LCD_C__
#include "aduc831.h"
#include "functions.h"
#include <stdarg.h>
#include <stdio.h>
#define LCDRS MCO  
#define LCDRW MDO
#define LCDDANE P0 
sbit LCDZgoda = P2^0;


//! bardziej ogolna funkcja do wysylania danych
void LCD_wyslij(char dane, bit RS, bit RW)
{
	LCDZgoda = 1;
	LCDRS = RS;
	LCDRW = RW;
	LCDDANE = dane;
	LCDZgoda = 0;
	delay(50);
	LCDZgoda = 1;
	delay(500);
}

//! COMMANDS***********************************************************
/*! Display clear - (RS = 0, R/W = 0, dane = 00000001) -instrukcja ta powoduje wyczyszczenie wyswietlacza poprzez wypelnienie go spacjami,
ustawienie trybu zapisu danych od pozycji w lewym górnym rogu wyswietlacza oraz wylaczenie trybu przesuwania okna*/
void Display_clear(void)
{
	LCD_wyslij(0x01,0,0);
	delay_us(1540);
}
/*! Display/cursor home - (RS - 0, R/W = 0, dane = 0000001x) -
instrukcja powoduje ustawienie kursora na pozycji pierwszego znaku w pierwszej linii*/
void Cursor_home(void)
{
	LCD_wyslij(0x02,0,0);
	delay_us(1540);
}
/*! Entry mode set - (RS = 0; R/W = 0, dane = 000001IS) - okreslenie trybu pracy kursora/okna wyswietlacza :
- dla S = 1 po zapisaniu znaku do wyswietlacza kursor nie zmienia polozenia, natomiast przesuwa sie cala zawartosc wyswietlacza
- dla S = 0 po zapisaniu znaku do wyswietlacza kursor zmienia polozenie, a przesuwanie okna jest wylaczone
- dla I = 1 kursor lub okno wyswietlacza przesuwa sie w prawo (inkrementacja adresu znaku)
- dla I = 0 kursor lub okno wyswietlacza przesuwa sie w lewo (dekrementacja adresu znaku)*/
void Entry_mode(int I, int S )
{
	LCD_wyslij(0x04+0x2*I+0x1*S,0,0);
}
/*! Display ON/OFF - (RS = 0, R/W = 0, dane = 00001DCB)
- dla D = 1 - wlaczenie wyswietlacza - dla D = 0 - wylaczenie wyswietlacza
- dla C = 1 - wlaczenie kursora - dla C = 0 - wylaczenie kursora
- dla B = 1 - wlaczenie migania kursora - dla B = 0 - wylaczenie migania kursora*/
void Display_ON_OFF(int D, int C,int B)
{
	LCD_wyslij(0x08+0x4*D+0x2*C+0x1*B, 0,0);
}
/* !Display cursor shift - (RS = 0, R/W = 0, dane = 0001SRxx)
- dla S = 1 - przesuwana jest zawartosc okna - dla S = 0 - przesuwany jest kursor
- dla R = 1 - kierunek przesuwu w prawo - dla R = 0 - kierunek przesuwu w lewo*/
void Display_Cursor_shift(int S, int R)
{
	LCDZgoda = 1;
	LCDRS = 0;
	LCDRW = 0;
	LCDDANE=0x10+0x8*S+0x4*R;
	LCDZgoda = 0;
	delay_us(37);
}
/*! Function set (RS= 0,  R/W = 0, dane = 001DNFxx)
- dla D = 1 - interfejs 8-bitowy - dla D = 0 - interfejs 4-bitowy
- dla N = 1 - wyswietlacz dwuwierszowy - dla N = 0 - wyswietlacz jednowierszowy
- dla F = 1 - matryca znaków 5*10 punktów - dla F = 0 - matryca znaków 5*7punktów*/
void Function_set(int D, int N,int F)
{
	LCD_wyslij(0x20+0x10*D+0x8*N+0x4*F,0,0);
}
/*! CG RAM set - (RS= 0, RW = 0, dane = 01AAALLL) - ustawia adres pamieci generatora znaków.
AAA - 3-bitowy adres znaku, LLL - 3-bitowy numer linii skladajacej sie na graficzne odwzorowanie znaku.*/
void CG_RAM_set(int ADRES)
{
	LCDZgoda = 1;
	LCDRS = 0;
	LCDRW = 0;
	LCDDANE=0x40+ADRES;
	LCDZgoda = 0;
	delay_us(37);
}
/*! DD RAM set - (RS = 0, R/W = 0, dane = 1AAAAAAA) - ustawia adres pamieci wyswietlacza,
pod który nastapi zapis (badz odczyt) danych operacja Data write lub Data read.*/
void DD_RAM_set(int ADRES)
{
	LCDZgoda = 1;
	LCDRS = 0;
	LCDRW = 0;
	LCDDANE=0x80+ADRES;
	LCDZgoda = 0;
	delay_us(37);
}
void LCD_GoTo(unsigned char x, unsigned char y)
{
	LCD_wyslij(0x80 | (x + (0x40 * y)), 0 ,0);
}

/*! Data write - (RS = 1, R/W = 0, dane = zapisywany bajt danych) - zapis danych do pamieci wyswietlacza,
badz pamieci CG RAM (jesli poprzednio wydano komende CG RAM set)*/
void Data_Write(int DANA)
{
	LCDZgoda = 1;
	LCDRS = 1;
	LCDRW = 0;
	LCDDANE = DANA;
	LCDZgoda = 0;
	delay_us(37);
}
/*! Data read - (RS = 1, R/W= 1, dane = odczytywany bajt danych) - odczyt danych z pamieci wyswietlacza,
badz pamieci CG RAM (jesli poprzednio wydano komende CG RAM set)*/
char Data_Read()
{
	LCDZgoda = 1;
	LCDRS = 1;
	LCDRW = 1;
	LCDZgoda = 0;
	delay_us(37);
	return (LCDDANE);
}

//! inicjalizacja wyswietlacza
void LCD_inicjalizacja()
{
	I2CM = 1;
	MDE = 1;
	
	delay_ms(20);
	LCD_wyslij(0x30,0,0);
	delay_ms(5);
	LCD_wyslij(0x30,0,0);
	delay_ms(1);
	LCD_wyslij(0x30,0,0);
	//!ustawiania wyswieltacza
	Function_set(1,1,0);
	Display_ON_OFF(0,0,0); //!disp off
	Display_clear(); //! disp clear
	delay_ms(1);
	Entry_mode(1,0); //! disp entry mode
	Display_ON_OFF(1,0,0);
}


void LCD_czekaj(int n)
{
	
}

//! wysylanie pojedzynczego znaku
void LCD_znak(char znak)
{
	LCD_wyslij(znak,1,0);
}

void LCD_wyslij_str(char *str)
{
	while(*str != '\0')
	{
		LCD_znak(*str);
		str++;
	}
}

void Printf_LCD (const char *format, ...)
{
  xdata char buffer[20];
  va_list args;
  va_start (args, format);
  vsprintf (buffer,format, args);
  LCD_wyslij_str(buffer);
  va_end (args);
}



#endif

#endif