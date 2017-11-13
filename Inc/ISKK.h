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

#define kReset 		   0x10
#define kKierunek 	   0x11
#define kTimeSoftStart 0x12
#define kAlfa 		   0x13
#define kStart 		   0x14
#define kfuse_Umax 	   0x15
#define kfuse_Umin 	   0x16
#define kfuse_Iznam    0x17
#define kfuse_Imax 	   0x18
#define kfuse_Iroz 	   0x19
#define kState_Input   0x20
#define kState_Output  0x21
#define kILx		   0x22
#define kULx           0x23
#define kTemperatura   0x24
#define kAIN0          0x25
#define kAIN1          0x26
#define kAIN2          0x27
#define kAIN3          0x28
#define kFlaga		   0x29
#define kAddress_ADR   0x30

#define k_Frame_1Byte  0x04
#define k_Frame_2Byte  0x05
#define k_Frame_3Byte  0x06
#define k_Frame_4Byte  0x07

struct Profibus{
	uint8_t State_Address_ADR_Hi;
	uint8_t State_Address_ADR_Lo;
	uint8_t Reset;
	uint8_t Kierunek;
	uint16_t TimeSoftStart;
	uint8_t Alfa;
	uint8_t Start;
	uint16_t fuse_Umax;
	uint16_t fuse_Umin;
	uint8_t fuse_Iznam;
	uint8_t fuse_Imax;
	uint8_t fuse_Iroz;
	uint8_t State_Input_Hi; //Wejscia
	uint8_t State_Input_Lo; //Wejscia
	uint8_t State_Output; //Wyjscia
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

}Profibus_Module1;

struct UART{
	bool    Flaga_DMA_Rx;
	uint8_t RECESIEVE_BUFFOR;
	uint8_t RECESIEVE_BUFFOR_INDEX;
	uint8_t RECESIEVE_BUFFOR_TAB[50];
	uint8_t TRANSMIT_BUFFOR_TAB[20];
}UART1;

void Main_UART(void);
void ISKK_Init(void);

#endif /* ISKK_H_ */
