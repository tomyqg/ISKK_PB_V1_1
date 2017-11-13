/*
 * ISKK.c
 *
 *  Created on: 12.09.2017
 *      Author: mikolajp
 */

#include "ISKK.h"
#include "stm32f1xx_hal.h"
#include "usart.h"

void ISKK_Init(void)
{
	//kiedy nadaje ustaw stan wysoki
	HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);//INT_IN

	UART1.Flaga_DMA_Rx=false;
	UART1.RECESIEVE_BUFFOR=0;
	UART1.RECESIEVE_BUFFOR_INDEX=0;

	//Profibus
	Profibus_Module1.AIN0_Hi=0;	Profibus_Module1.AIN0_Lo=0;
	Profibus_Module1.AIN1_Hi=0;	Profibus_Module1.AIN1_Lo=0;
	Profibus_Module1.AIN2_Hi=0;	Profibus_Module1.AIN2_Lo=0;
	Profibus_Module1.AIN3_Hi=0;	Profibus_Module1.AIN3_Lo=0;
	Profibus_Module1.Alfa=0;
	Profibus_Module1.Flaga_Hi=0; Profibus_Module1.Flaga_Lo=0;
	Profibus_Module1.IL1_Hi=0; Profibus_Module1.IL1_Lo=0;
	Profibus_Module1.IL2_Hi=0; Profibus_Module1.IL2_Lo=0;
	Profibus_Module1.IL3_Hi=0; Profibus_Module1.IL3_Lo=0;
	Profibus_Module1.Kierunek=0;
	Profibus_Module1.Reset=0;
	Profibus_Module1.Start=0;
	Profibus_Module1.State_Address_ADR_Hi=0; 	Profibus_Module1.State_Address_ADR_Lo=0;
	Profibus_Module1.State_Input_Hi=0; 	Profibus_Module1.State_Input_Lo=0;
	Profibus_Module1.State_Output=0;
	Profibus_Module1.Temperatura_Hi=0; Profibus_Module1.Temperatura_Lo=0;
	Profibus_Module1.TimeSoftStart=0;
	Profibus_Module1.UL1_Hi=0; Profibus_Module1.UL1_Lo=0;
	Profibus_Module1.UL2_Hi=0; Profibus_Module1.UL2_Lo=0;
	Profibus_Module1.UL3_Hi=0; Profibus_Module1.UL3_Lo=0;
	Profibus_Module1.fuse_Imax=0;
	Profibus_Module1.fuse_Iroz=0;
	Profibus_Module1.fuse_Iznam=0;
	Profibus_Module1.fuse_Umax=0;
	Profibus_Module1.fuse_Umin=0;
}

void Main_UART(void) {
	if (UART1.Flaga_DMA_Rx == true) { //Jezeli odebrano znak
		UART1.RECESIEVE_BUFFOR_TAB[UART1.RECESIEVE_BUFFOR_INDEX] = UART1.RECESIEVE_BUFFOR;
		UART1.RECESIEVE_BUFFOR_INDEX++;
		UART1.Flaga_DMA_Rx = false;

		if (UART1.RECESIEVE_BUFFOR_TAB[0] == UART1.RECESIEVE_BUFFOR_INDEX) { //Jezeli odebrano cala ramke
			if (UART1.RECESIEVE_BUFFOR_TAB[1] == 0xFF) { //Zapis

				if (UART1.RECESIEVE_BUFFOR_TAB[2] == kState_Input) {
					Profibus_Module1.State_Input_Hi = UART1.RECESIEVE_BUFFOR_TAB[3];
					Profibus_Module1.State_Input_Lo = UART1.RECESIEVE_BUFFOR_TAB[4];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[5] == kState_Output) {
					Profibus_Module1.State_Output = UART1.RECESIEVE_BUFFOR_TAB[6];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[7] == kTemperatura) {
					Profibus_Module1.Temperatura_Hi = UART1.RECESIEVE_BUFFOR_TAB[8];
					Profibus_Module1.Temperatura_Lo = UART1.RECESIEVE_BUFFOR_TAB[9];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[10] == kAIN0) {
					Profibus_Module1.AIN0_Hi = UART1.RECESIEVE_BUFFOR_TAB[11];
					Profibus_Module1.AIN0_Lo = UART1.RECESIEVE_BUFFOR_TAB[12];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[13] == kAIN1) {
					Profibus_Module1.AIN1_Hi = UART1.RECESIEVE_BUFFOR_TAB[14];
					Profibus_Module1.AIN1_Lo = UART1.RECESIEVE_BUFFOR_TAB[15];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[16] == kAIN2) {
					Profibus_Module1.AIN2_Hi = UART1.RECESIEVE_BUFFOR_TAB[17];
					Profibus_Module1.AIN2_Lo = UART1.RECESIEVE_BUFFOR_TAB[18];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[19] == kAIN3) {
					Profibus_Module1.AIN3_Hi = UART1.RECESIEVE_BUFFOR_TAB[20];
					Profibus_Module1.AIN3_Lo = UART1.RECESIEVE_BUFFOR_TAB[21];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[22] == kILx) {
					Profibus_Module1.IL1_Hi = UART1.RECESIEVE_BUFFOR_TAB[23];
					Profibus_Module1.IL1_Lo = UART1.RECESIEVE_BUFFOR_TAB[24];
					Profibus_Module1.IL2_Hi = UART1.RECESIEVE_BUFFOR_TAB[25];
					Profibus_Module1.IL2_Lo = UART1.RECESIEVE_BUFFOR_TAB[26];
					Profibus_Module1.IL3_Hi = UART1.RECESIEVE_BUFFOR_TAB[27];
					Profibus_Module1.IL3_Lo = UART1.RECESIEVE_BUFFOR_TAB[28];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[29] == kULx) {
					Profibus_Module1.UL1_Hi = UART1.RECESIEVE_BUFFOR_TAB[30];
					Profibus_Module1.UL1_Lo = UART1.RECESIEVE_BUFFOR_TAB[31];
					Profibus_Module1.UL2_Hi = UART1.RECESIEVE_BUFFOR_TAB[32];
					Profibus_Module1.UL2_Lo = UART1.RECESIEVE_BUFFOR_TAB[33];
					Profibus_Module1.UL3_Hi = UART1.RECESIEVE_BUFFOR_TAB[34];
					Profibus_Module1.UL3_Lo = UART1.RECESIEVE_BUFFOR_TAB[35];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[36] == kFlaga){
					Profibus_Module1.Flaga_Hi = UART1.RECESIEVE_BUFFOR_TAB[37];
					Profibus_Module1.Flaga_Lo = UART1.RECESIEVE_BUFFOR_TAB[38];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[39] == kAddress_ADR){
					Profibus_Module1.State_Address_ADR_Hi = UART1.RECESIEVE_BUFFOR_TAB[40];
					Profibus_Module1.State_Address_ADR_Lo = UART1.RECESIEVE_BUFFOR_TAB[41];
				}
			}
		}
	}
}

