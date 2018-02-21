/*!
	@author Kamil Hoffmann
	@file functions_sound.c
	@date 22-12-2017
	@brief Biblioteka funkcji
	
	
*/

#include "int_lib.h"
#include "dio.h"
#include "main.h"

//#define F_OSC 11058000

sbit W1 = P1^4; //!< sbit wiersz 1
sbit W2 = P1^5; //!< sbit wiersz 2
sbit W3 = P1^6; //!< sbit wiersz 3
sbit W4 = P1^7; //!< sbit wiersz 4

sbit K1 = P3^5; //!< sbit kolumna 1
sbit K2 = P3^6; //!< sbit kolumna 2
sbit K3 = P3^7; //!< sbit kolumna 3

/*!
incjalizacja klawiatury -
ustawienie odpowiednich wejsc,
zaincjalizowanie czestotliwosci wg. gammy
*/
void klaw_init(void)
{

	W1=0;
	W2=0;
	W3=0;
	W4=0;
	
	keypad_values[1] = 262; //c1
	keypad_values[2] = 277; //cis1
	keypad_values[3] = 294; //d1
	keypad_values[4] = 311; //dis1
	keypad_values[5] = 330; //e1
	keypad_values[6] = 349; //f1
	keypad_values[7] = 370; //fis1
	keypad_values[8] = 392; //g1
	keypad_values[9] = 415; //gis1
	keypad_values[10] = 440; //a1
	keypad_values[0] = 466; //ais1
	keypad_values[11] = 494; //b1
}


char character = 'a'; //!< poprzedni wcisniety przycisk - do piosenek - nie zaimplementowaeno 
/*!
odczytywanie klawiatury,
sprawdzanie czy nie zostalo cos klikniete
if'y zamiast while zeby nie nie czekac w funkcji jak przycisk bedzie wcisniety
*/
void klawiatura_znak(void)
{
character = 'a';

//do{
K1=0;
K2=1;
K3=1;

if(W1==0) character = '1';
if(W2==0) character = '4';
if(W3==0) character = '7';
if(W4==0) character = '*';
	
K1=1;
K2=0;
K3=1;

if(W1==0) character = '2';
if(W2==0) character = '5';
if(W3==0) character = '8';
if(W4==0) character = '0';

K1=1;
K2=1;
K3=0;
if(W1==0) character = '3';
if(W2==0) character = '6';
if(W3==0) character = '9';
if(W4==0) character = '#';
//} while(character == 'a')

}

/*!
funckja pomocnicza do timer'a 0
ustawienie rejestrow
*/
void timer0_interrupt(unsigned int us)
{
	unsigned char stl1;
	unsigned char sth1;
	unsigned int N;
	
	N = 65536-us;

	stl1=N;
	sth1=N>>8;
	
	TL0 = stl1;
	TH0 = sth1;
	
}




/*!
obsluga przerwania od timer'a 0
zadamy pojedynczej kowersji ADC co 2 sekundy
*/
void interrupt_3(void) interrupt 1
{
	SCONV = 1;
	
	timer0_interrupt(2000);
}

char uart_potencjo[5]	= 0; //!< tablica do UART'a
unsigned int prev_potencjo = 0; //!< poprzednia wartosc potencjometru
unsigned int potencjometr=0; //!< wartosc potencjometru
/*!
obsluga przerwania od ADC
przetwarzamy dane,
wyslamy wartosc potencjometru po UART'cie (jesli sie zmienila) - 
dodatek jako pozycja potencjometru, ktora aktualizuje sie w aplikacji okienkowej po stronie PC
*/
void adc_obsluga(void) interrupt 6
{
	
	unsigned int ADC_rejestr_dolny = 0;
	unsigned int ADC_rejestr_gorny = 0;	
	
	//Pobranie wartosci probki
	ADC_rejestr_dolny = ADCDATAL;
	ADC_rejestr_gorny = ADCDATAH & 0x0F;		
	
	prev_potencjo = potencjometr;
	//Odpowiednie przeksztalcenie wartosci z rejestrow do zmniennej typu int
	potencjometr = ADC_rejestr_gorny << 8;
	potencjometr += ADC_rejestr_dolny;
	
	
	if(prev_potencjo != potencjometr)
	{
		sprintf(uart_potencjo, "@%d", potencjometr);
		UART_wyslij_str(uart_potencjo,5);
	}
	

}

/*!
	funkcja konfigurujaca pwm
	dziwiek wejscia takie jak symbole na klawiaturze
	wyp w skali 2^16
	if button == 'a' turn off
*/
void tone(char buttom, unsigned int  wypelnienie)
{

	
#if bez_pot
	
#define F_ZAD 500
#define WYPELNIENIE 0.5

#define PWMa1 11058 //F_OSC / F_ZAD) // 
#define PWMa0 2765 //(WYPELNIENIE * PWM1) // 
	
		//dla mode 1
		CFG831 = CFG831&(~(7<<2)) | CFG831&(~(1<<6));

		PWMCON = 0x13;
		
		PWM1L = PWMa1;
		PWM1H = (PWMa1>>8);

		PWM0L = PWMa0;
		PWM0H = (PWMa0>>8);
	
	
#endif	

#if z_pot 

unsigned int f_zad=0;
unsigned int PWMa1r;  //F_OSC / F_ZAD 
unsigned int PWMa0r;   //WYPELNIENIE * PWM1)

if(buttom=='1') f_zad=keypad_values[1];
if(buttom=='2') f_zad=keypad_values[2];
if(buttom=='3') f_zad=keypad_values[3];
if(buttom=='4') f_zad=keypad_values[4];
if(buttom=='5') f_zad=keypad_values[5];
if(buttom=='6') f_zad=keypad_values[6];
if(buttom=='7') f_zad=keypad_values[7]; 
if(buttom=='8') f_zad=keypad_values[8];
if(buttom=='9') f_zad=keypad_values[9];
if(buttom=='*') f_zad=keypad_values[10];
if(buttom=='0') f_zad=keypad_values[0];
if(buttom=='#') f_zad=keypad_values[11];
if(buttom=='a') f_zad=0; //turned off

if(f_zad == 0)
{
	PWMCON = 0x00;
	return;
}

CFG831 = CFG831&(~(7<<2)) | CFG831&(~(1<<6));

PWMCON = 0x13;


PWMa1r=F_OSC/f_zad;
PWMa0r=PWMa1r*(float)wypelnienie/4096.0;

PWM1L = PWMa1r;
PWM1H = (PWMa1r>>8);

PWM0L = PWMa0r;
PWM0H = (PWMa0r>>8);
	

#endif

}

/*!
zamiana znaku odpowiadajacemu danemu przyciskowi
na przypisana wartosc czestotliwosci dla tego przycisku
*/
int character_to_freq(char button)
{
	switch(button)
	{
		case '1':
		{
			return keypad_values[1];
		}
		case '2':
		{
			return keypad_values[2];
		}
		case '3':
		{
			return keypad_values[3];
		}
			case '4':
		{
			return keypad_values[4];
		}
			case '5':
		{
			return keypad_values[5];
		}
			case '6':
		{
			return keypad_values[6];
		}
			case '7':
		{
			return keypad_values[7];
		}
			case '8':
		{
			return keypad_values[8];
		}
			case '9':
		{
			return keypad_values[9];
		}
			case '0':
		{
			return keypad_values[0];
		}
			case '*':
		{
			return keypad_values[10];
		}
			case '#':
		{
			return keypad_values[11];
		}
	}
	
	return 0;
	
}



