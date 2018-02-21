/*!
	@author Michal Wróblewski
	@file main.c
	@date 17-10-2017
	@brief glowny plik
*/


#include "main.h"

/*!
UWAGA, PARAMETRY KOMUNIKACJI:
-BAUDRATE 19200
-wykorzystywany port: COM8(polaczony z COM9 -> qt)
-8bitów, 1 bit stopu, parity:none (patrz plik init)
*/

//!Zmienne globalne wykorzystywane do komunikacji UART


int UART_counter; //!< licznik danych UART'a
char UART_buff[50]; //!< tablica danych od UART'a
int keypad_values[12]; //!< wartosci czestotliwosci odpowiadajace klaiwszom
int reply = 0; //!< zmianna odpowiedzialna za zadanie odpowiedzi konfiguracji
char previous = 'a'; //!< zmienna do piosenek - nie zaimplementowano
int stan0 = 1; //!< stan od set/reset zmiennej 'record' - piosenki nie zaimplementowano
int stan1 = 1; //!< stan od set/reset zmiennej 'allow_playing' - piosenki nie zaimplementowano
int record =0; //!< zmienna zezwalajaca na 'nagrywanie' 
int allow_playing = 0; //!< zmienna zezwalajaca na odtwarzanie

/*!
\brief int main(void)


glowna funkcja programu,
inicjalizacja calej konfiguracji,
set/reset stanow,
obsluga klawiatury,
generowanie dzwiekow
*/
int main(void)
{
	SetBit(CFG831, 0); //XRAM

	//wlaczenie przerwan
	ES = 1; //przerwanie UART
	EA = 1; //przerwanie globalne
	EADC = 1; //adc przerwanie
	ET0=1; //timer0
	ET1 = 1; //timer1

	
	//baudrate 19200 przy uzyciu timera 2
	SCON = 0x50;
	T3CON = 0x84;
	T3FD = 0x08;
		
	
	
	//seting timer 0 and timer 1
	//! mode 1 
	T0_16;
	T1_16;
	// initing timer 
	timer0_interrupt(2000);
	
	//! turning on timer 0
	T0_RUN; 
	klaw_init();
	
	
	ADCCON1 = 0xA4; //Konfigruracja ADC: MD1=1; EXT_REF=0; CKx=0x10; AQx=0x01; T2C=0; EXC=0;
	ADCCON2 = (ADCCON2 & 0x0F) | 0x03; //Wybor kanalu trzeciego jako kanal na ktorym bedzie mierzone napiecie
	
	
		while(1)
		{			
			//odpowiedz po UARCIE, przeyslamy konfiguarcje przyciskow
			if(reply)
			{
				UART_send_cfg();
				reply = 0;
			}
			
			//odczyt klawiatury
			klawiatura_znak();
			
			//cos kliknieta, liczymy czas ile bylo klikniete timerem 1
			/*
			if(previous == 'a' && character != 'a' && record)
			{
				//send_record(previous);				
			}
			
			//puszczono przycisk konczymy liczenie
			if(character == 'a' && previous != 'a' && record)
			{
				//send_record(character);		
			}
			*/
			
			
			//USTAWAWIANIE ZEZWOLENIA NA NAGRYWANIE
			if(stan0 == 8 && KEY_DOWN(PR1))
			{
				if(record)
				{
					record = 0;
					LedOff(LED_D5);
					//T1_STOP;
				}
				else
				{
					record = 1;
					LedOn(LED_D5);
					//T1_ustaw_czas_ms(1000);
					//T1_RUN;			
			}
			}

			//USTAWAWIANIE ZEZWOLENIA NA ODTWARZANIE
			if(stan1 == 16 && KEY_DOWN(PR2))
			{
				if(allow_playing)
				{
					allow_playing = 0;
					LedOff(LED_D9);
				}
				else
				{
					allow_playing = 1;
					LedOn(LED_D9);
				}			
			}
			
			stan0 = KEY_UP(PR1);
			stan1 = KEY_UP(PR2);
						
			//zmiana PWM'a - odpowdnik zmiant tonu
			tone(character,potencjometr);
		}
}


