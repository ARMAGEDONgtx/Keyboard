/*!
	@author Michal Wróblewski
	@file functions.c
	@date 22-12-2017
	@brief Biblioteka funkcji
	
*/

#include "int_lib.h"
#include "dio.h"
#include "main.h"

/*!
opoznienie na podstawie liczby cykli maszynowych
*/
void delay(int cycles)
{
	int i = 0;
	for(i; i<cycles;i++)
	{
		//doing nothing
	}
}

/*! 
opoznienie na podstawie czasu
przez obliczenia tracimy jakies 1.988ms, na if'ie kolejne bysmy tracili
*/
void delay_ms(int ms)
{
	uint32_t cycles = ((float)ms - 2.0 - 0.022)/0.016 + 0.5;
	delay(cycles);
}

/*!
Opoznienie w mikrosekundach , bazuje na poporzednich funkcjach delay
*/
void delay_us(int us)
{
	uint32_t cycles = ((float)us/1000.0 - 2.0 - 0.022)/0.016 + 0.5;
	delay(cycles);
}

/*!
Funckja do wysylania pojednycznego chara po UART'cie
czekamy az TI bedzie jedynka, to oznacza tramsjie
potem zerujemy TI
*/
void UART_wyslij(char c)
{
	SBUF = c;
	while(!TI){}
	TI = 0;

}		
		
/*!
Funckja bazujaca na UART_wyslij, tylko wysylamy cala
tablice char'ow, konieczne jest podanie rozmiaru
*/
void UART_wyslij_str(char* str, int ile)
{
	int i;
	for(i = 0 ; i < ile; i++)
	{
		UART_wyslij(str[i]);
	}
}	

/*!
Przerwanie od UARTA, odczytuje dane z UARTA az do otrzymania '!'
wtedy wyciagam dane z buffora i przepisuje do tablicy z vartosciami,
kazda wartosc jest przypisana do przyciusku klawiatury,
po otryzmaniu '!' wystawiam zdanie 'reply' -> patrz inne funckje
*/
void UART_obsluga(void)interrupt 4
{
	char odbior;
	if(RI)
	{
	  UART_buff[UART_counter] = SBUF;
		odbior = UART_buff[UART_counter];
		RI = 0;
		UART_counter++;
	}
	if(odbior == '!' || UART_counter >= 50)
	{
		sscanf(UART_buff, "%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d,%d",
		&keypad_values[0], &keypad_values[1],&keypad_values[2],&keypad_values[3],&keypad_values[4],
		&keypad_values[5],&keypad_values[6],&keypad_values[7],&keypad_values[8],&keypad_values[9],
		&keypad_values[10],&keypad_values[11]);
		UART_counter =0;	
		reply = 1;
	}
}

/*!
Wysylanie aktualnej konfiguarcji jako potwierdzenia (tymczasowo),
wyciagamy wartosci z tablicy 'keypad_values' i przesylamy po UARCIE
wartosc bufara jest nadmiarowa
*/
char tmp[50];
void UART_send_cfg()
{
   
	sprintf(tmp, "%d %d %d %d %d %d %d %d %d %d %d %d",
	keypad_values[0], keypad_values[1],keypad_values[2],keypad_values[3],keypad_values[4],
	keypad_values[5],keypad_values[6],keypad_values[7],keypad_values[8],keypad_values[9],
	keypad_values[10],keypad_values[11]);
	
	UART_wyslij_str(tmp, 50);
	
}


/*!
wysylanie danych o aktualnie wcisnietym przycisku,
poczatki implemetnacji piosenek - nie dokoczono
*/
void send_record(char a)
{
	//unsigned int czas;
	//czas = TH1 << 8;
	//czas += TL1;
	//TH1 = 0x0;
	//TL1 = 0x0;
	//sprintf(tmp, "!%d %d", character_to_freq(a), czas);
	//UART_wyslij_str(tmp, 15);
	sprintf(tmp, "!%d", character_to_freq(a));
	UART_wyslij_str(tmp, 5);
	
	
}

/*!
obsluga timera 1
wysylanie danych o aktualnie wcisnietym przycisku,
poczatki implemetnacji piosenek - nie dokoczono
*/
void timer1_obsluga(void) interrupt 3
{
	send_record(character);
	T1_ustaw_czas_ms(5000);
}
