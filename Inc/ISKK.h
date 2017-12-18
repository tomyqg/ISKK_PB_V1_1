/*
 * ISKK.h
 *
 *  Created on: 12.09.2017
 *      Author: mikolajp
 */
#include "stdint.h"
#include "stdbool.h"

#ifndef ISKK_H_
#define ISKK_H_

#define kControl	   10
#define kState_Input   11
#define kFlaga		   12
#define kAINx          13
#define kILx		   14
#define kULx           15
#define kTemperatura   16
#define kAddress_ADR   17
#define kTimeSoftStart 18
#define kAlfa 	       19
#define kfuse_Umax 	   20
#define kfuse_Umin 	   21
#define kfuse_Iznam    22
#define kfuse_Imax 	   23
#define kfuse_Iroz 	   24




#define k_Frame_1Byte  0x04
#define k_Frame_2Byte  0x05
#define k_Frame_3Byte  0x06
#define k_Frame_4Byte  0x07

struct Profibus_Data{
	uint8_t MyProfibusAddress;
	uint8_t Control;
	uint8_t State_Input_Hi; //Wejscia
	uint8_t State_Input_Lo; //Wejscia
	uint8_t IL1_Hi;
	uint8_t IL1_Lo;
	uint8_t IL2_Hi;
	uint8_t IL2_Lo;
	uint8_t IL3_Hi;
	uint8_t IL3_Lo;
	uint8_t UL1_Hi;
	uint8_t UL1_Lo;
	uint8_t UL2_Hi;
	uint8_t UL2_Lo;
	uint8_t UL3_Hi;
	uint8_t UL3_Lo;
	uint8_t Temperatura_Hi;
	uint8_t Temperatura_Lo;
	uint8_t AIN0_Hi;
	uint8_t AIN0_Lo;
	uint8_t AIN1_Hi;
	uint8_t AIN1_Lo;
	uint8_t AIN2_Hi;
	uint8_t AIN2_Lo;
	uint8_t AIN3_Hi;
	uint8_t AIN3_Lo;
	uint8_t Flaga_Hi;
	uint8_t Flaga_Lo;

}SSK_To_ISKK;

struct Profibus_Prm
{
	/*
	 *  Struktura przechowuj¹ca parametry Profibus
	 */
	uint16_t TimeSoftStart;
	uint8_t Alfa;
	uint16_t fuse_Umax;
	uint16_t fuse_Umin;
	uint8_t fuse_Iznam;
	uint8_t fuse_Imax;
	uint8_t fuse_Iroz;

	uint16_t SSK_TimeSoftStart;
	uint8_t SSK_Alfa;
	uint16_t SSK_fuse_Umax;
	uint16_t SSK_fuse_Umin;
	uint8_t SSK_fuse_Iznam;
	uint8_t SSK_fuse_Imax;
	uint8_t SSK_fuse_Iroz;
} sProfibus_Prm;

struct UART{
	bool    Flaga_DMA_Rx;
	uint8_t RECESIEVE_BUFFOR;
	uint8_t RECESIEVE_BUFFOR_INDEX;
	uint8_t RECESIEVE_BUFFOR_TAB[60];
	uint8_t TRANSMIT_BUFFOR_TAB[20];
}UART1;

void UART_SSK2ISKK(void);
void ISKK_Init(void);

#endif /* ISKK_H_ */
