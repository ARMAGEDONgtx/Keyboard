/*!
	@author Michal Wróblewski
	@file functions.h
	@date 22-12-2017
	@brief DEKLARACJE funckji	
*/
#ifndef _FUNCTIONS_H__
#define _FUNCTIONS_H__


void delay(int cycles);  //!< delay w cyklach 
void delay_ms(int ms);  //!< delay w ms
void delay_us(int us);  //!< delay w us

void UART_obsluga(void); //!< przerwanie od UART'a
void UART_wyslij(char c); //!< wysylanie znaku po UART'cie
void UART_wyslij_str(char* str, int ile); //!< wysylanie string'a po UART'cie
void UART_send_cfg(); //!< wysylanie konfiguracji czestotliwosci po UART'cie
void ext_int_0(void); //!< przerwanie od external interrupt 0
void ext_int_1(void); //!< przerwanie od external interrupt 1
void timer1_obsluga(void); //!< przerwanie od timera 1
void adc_obsluga(void); //!< przerwanie od ADC
int character_to_freq(char button); //!< zamiana chara na odpowaidajaca mu czestotliwosc
void send_record(char a); //!< wysyalnie danych dot. piosenki - nie zaimplementowane

void klawiatura_znak(void); //!< obsluga klawiatury
void klaw_init(void); //!< inicjalizacja klawiatury
void interrupt_3(void); //!< przerwanie timer 0
void timer0_interrupt(unsigned int us);	//!< funckja dodatkowa do przewania timer'a 0
void tone(char button, unsigned int  wypelnienie); //!< zmiana PWM'a - generowanie tonu

#endif