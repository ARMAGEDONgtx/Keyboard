/*!
	@author Kamil Hoffmann
	@file T1.h
	@date 22-12-2017
	@brief macra pomocnicze do timer'ow

*/

#ifndef T1_H
#define T1_H

#include "aduc831.h"

typedef unsigned short int int16;

//Ustawienia trybów bitowych
#define T1_13					(TMOD&=~(0x01<<4) | TMOD&=~(0x01<<5)) 	//!< Ustawia tryb 13-bitowy
#define T1_16					(TMOD&=~(0x01<<5)) | (TMOD|=0x01<<4)	//!< Ustawia tryb 16-bitowy
#define T1_8_O 				(TMOD|=0x01<<5) 												//!< Ustawia tryb 8-bitowy z przeladowaniem
#define T1_8_NO 			(TMOD|=0x01<<4 | TMOD|=0x01<<5) 				//!< Ustawia tryb 8-bitowy bez przeladowania


#define T0_13					(TMOD&=~(0x01<<0) | TMOD&=~(0x01<<1)) 	//!< Ustawia tryb 13-bitowy
#define T0_16					(TMOD&=~(0x01<<0)) | (TMOD|=0x01<<1)	//!< Ustawia tryb 16-bitowy
#define T0_8_O 				(TMOD|=0x01<<1) 												//!< Ustawia tryb 8-bitowy z przeladowaniem
#define T0_8_NO 			(TMOD|=0x01<<0 | TMOD|=0x01<<1) 				//!< Ustawia tryb 8-bitowy bez przeladowania

//Ustawienia rodzajów pracy
#define T1_COUNTER		(TMOD|=0x01<<6)													//!< Ustawia tryb pracy licznika
#define T1_TIMER			(TMOD&=~(0x01<<6))											//!< Ustawia tryb pracy czasomierza

#define T0_COUNTER		(TMOD|=0x01<<2)													//!< Ustawia tryb pracy licznika
#define T0_TIMER			(TMOD&=~(0x01<<2))											//!< Ustawia tryb pracy czasomierza

//Ustawienia bramki
#define T1_GATEON 		(TMOD|=(0x01<<7))												//!< Wlaczenie bramki
#define T1_GATEOFF 		(TMOD&=~(0x01<<7)) 											//!< Wylaczenie bramki

#define T0_GATEON 		(TMOD|=(0x01<<3))												//!< Wlaczenie bramki
#define T0_GATEOFF 		(TMOD&=~(0x01<<3)) 											//!< Wylaczenie bramki
//Uruchomienie timera
#define T1_RUN 				(TCON|=(0x01<<6))												//!< start timera
#define T1_STOP				(TCON&=~(0x01<<6))												//!< stop timera
#define T0_RUN 				(TCON|=(0x01<<4))												//!< start timera
#define T0_STOP				(TCON&=~(0x01<<4))												//!< stop timera

#endif