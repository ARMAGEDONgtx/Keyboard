/*!
	@author Michal Wróblewski
	@file dio.h
	@date 17-10-2017
	@brief input/output macros
*/
#ifndef _DIO_H__
#define _DIO_H__

#include "aduc831.h"
#include "int_lib.h"
/*! Reg/Bit Operations****************************************************/

/*! SetBit(byte,b) MAKR DO SETOWANIA*/
#define SetBit(byte,b) byte|=0x01<<b

/*! ClearBit(byte, b) MAKR DO Kasowania */
#define ClearBit(byte,b) byte&=~(0x01<<b)

/*! ToggleBit(byte,b) MAKR DO TOGGLOWANIA*/
#define ToggleBit(byte,b) byte^=0x01<<b

/*! CheckBit(byte,b) MAKRO DO SPRAWDZANIA BITU*/
#define CheckBit(byte,b) (byte&(0x01<<b))


#define LedPort P2  //!< definy do makro instrukcji
#define LED_D5 7  //!< definy do makro instrukcji 
#define LED_D9 6  //!< definy do makro instrukcji
#define LED_D11 5  //!< definy do makro instrukcji
#define LED_D12 4  //!< definy do makro instrukcji
#define KeyPort P3  //!< definy do makro instrukcji
#define PR1 3  //!< definy do makro instrukcji
#define PR2 4 
/*! INPUT OP*/

#define LedOn(Dioda) ClearBit(LedPort,Dioda)  //!< makro wlaczajace diode
#define LedOff(Dioda) SetBit(LedPort,Dioda)  //!< makro wylaczajace diode


#define KEY_DOWN(Przycisk) !CheckBit(KeyPort,Przycisk) //!< makro sprawdzajace czy przycisk jest wcisniety
#define KEY_UP(Przycisk) CheckBit(KeyPort,Przycisk) //!< makro sprawdzajace czy przycisk jest wcisniety


/*!
wejscie - musimy ustawic w stan wysoki i odczytywac piny
wyjscie - musimy ustawic stan niski - 0 , wysoki - 1
******************************************************************************/

/*timers operations***************************************************************/

#define F_OSC 11058000.0 //! stala czestotliwosc kwarcu
#define T_dzielnik 12 //! stala dzielnik zegara
#define T_rozdzielczosc 16 //! stala rozdzielczosc timera

//! zaleznosci ogólne
#define Tx_tau(dzielnik) (float)((float)dzielnik/F_OSC)
//! zaleznosci ogolne cd.
#define Tx_N(czas_ms,dzielnik) (unsigned int)((float)czas_ms/Tx_tau(dzielnik)/1000.0)


//! ustawianie timera
#define T_rejestr_wartosc_poczatkowa(czas_ms) \
((0x000001ul<<T_rozdzielczosc)-Tx_N(czas_ms,T_dzielnik))

//! ustawianie timera
#define T1_ustaw_czas_ms(czas_ms) \
TL1=T_rejestr_wartosc_poczatkowa(czas_ms);\
TH1=T_rejestr_wartosc_poczatkowa(czas_ms)>>8
//**********************************************************************************
#endif