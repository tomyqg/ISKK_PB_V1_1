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

	//Profibus_Prm
	sProfibus_Prm.Alfa          = 60;
	sProfibus_Prm.TimeSoftStart = 3000;
	sProfibus_Prm.fuse_Imax     = 10;
	sProfibus_Prm.fuse_Iroz     = 10;
	sProfibus_Prm.fuse_Iznam    = 10;
	sProfibus_Prm.fuse_Umax     = 780;
	sProfibus_Prm.fuse_Umin     = 320;
	//Profibus_Dane
	SSK_To_ISKK.AIN0_Hi=0;	SSK_To_ISKK.AIN0_Lo=0;
	SSK_To_ISKK.AIN1_Hi=0;	SSK_To_ISKK.AIN1_Lo=0;
	SSK_To_ISKK.AIN2_Hi=0;	SSK_To_ISKK.AIN2_Lo=0;
	SSK_To_ISKK.AIN3_Hi=0;	SSK_To_ISKK.AIN3_Lo=0;
	SSK_To_ISKK.Flaga_Hi=0; SSK_To_ISKK.Flaga_Lo=0;
	SSK_To_ISKK.IL1_Hi=0; SSK_To_ISKK.IL1_Lo=0;
	SSK_To_ISKK.IL2_Hi=0; SSK_To_ISKK.IL2_Lo=0;
	SSK_To_ISKK.IL3_Hi=0; SSK_To_ISKK.IL3_Lo=0;

	SSK_To_ISKK.MyProfibusAddress=0;
	SSK_To_ISKK.State_Input_Hi=0; 	SSK_To_ISKK.State_Input_Lo=0;
	SSK_To_ISKK.Temperatura_Hi=0; SSK_To_ISKK.Temperatura_Lo=0;
	SSK_To_ISKK.UL1_Hi=0; SSK_To_ISKK.UL1_Lo=0;
	SSK_To_ISKK.UL2_Hi=0; SSK_To_ISKK.UL2_Lo=0;
	SSK_To_ISKK.UL3_Hi=0; SSK_To_ISKK.UL3_Lo=0;

}

void UART_SSK2ISKK(void) { //Od SSK do ISSK
	if (UART1.Flaga_DMA_Rx == true) { //Jezeli odebrano znak
		UART1.RECESIEVE_BUFFOR_TAB[UART1.RECESIEVE_BUFFOR_INDEX] = UART1.RECESIEVE_BUFFOR;
		UART1.RECESIEVE_BUFFOR_INDEX++;
		UART1.Flaga_DMA_Rx = false;
		//if(UART1.RECESIEVE_BUFFOR_INDEX == 1 && UART1.RECESIEVE_BUFFOR_TAB[1]!=0xFF) UART1.RECESIEVE_BUFFOR_INDEX=0;

		if ((UART1.RECESIEVE_BUFFOR_TAB[0] == UART1.RECESIEVE_BUFFOR_INDEX) && UART1.RECESIEVE_BUFFOR_TAB[1]==0xFF) { //Jezeli odebrano cala ramke
			if (UART1.RECESIEVE_BUFFOR_TAB[1] == 0xFF) { //Zapis

				if (UART1.RECESIEVE_BUFFOR_TAB[2] == kControl) {
					SSK_To_ISKK.Control = UART1.RECESIEVE_BUFFOR_TAB[3];
				}

				if (UART1.RECESIEVE_BUFFOR_TAB[4] == kState_Input) {
					SSK_To_ISKK.State_Input_Hi = UART1.RECESIEVE_BUFFOR_TAB[5];
					SSK_To_ISKK.State_Input_Lo = UART1.RECESIEVE_BUFFOR_TAB[6];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[7] == kFlaga) {
					SSK_To_ISKK.Flaga_Hi = UART1.RECESIEVE_BUFFOR_TAB[8];
					SSK_To_ISKK.Flaga_Lo = UART1.RECESIEVE_BUFFOR_TAB[9];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[10] == kAINx) {
					SSK_To_ISKK.AIN0_Hi = UART1.RECESIEVE_BUFFOR_TAB[11];
					SSK_To_ISKK.AIN0_Lo = UART1.RECESIEVE_BUFFOR_TAB[12];
					SSK_To_ISKK.AIN1_Hi = UART1.RECESIEVE_BUFFOR_TAB[13];
					SSK_To_ISKK.AIN1_Lo = UART1.RECESIEVE_BUFFOR_TAB[14];
					SSK_To_ISKK.AIN2_Hi = UART1.RECESIEVE_BUFFOR_TAB[15];
					SSK_To_ISKK.AIN2_Lo = UART1.RECESIEVE_BUFFOR_TAB[16];
					SSK_To_ISKK.AIN3_Hi = UART1.RECESIEVE_BUFFOR_TAB[17];
					SSK_To_ISKK.AIN3_Lo = UART1.RECESIEVE_BUFFOR_TAB[18];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[19] == kILx) {
					SSK_To_ISKK.IL1_Hi = UART1.RECESIEVE_BUFFOR_TAB[20];
					SSK_To_ISKK.IL1_Lo = UART1.RECESIEVE_BUFFOR_TAB[21];
					SSK_To_ISKK.IL2_Hi = UART1.RECESIEVE_BUFFOR_TAB[22];
					SSK_To_ISKK.IL2_Lo = UART1.RECESIEVE_BUFFOR_TAB[23];
					SSK_To_ISKK.IL3_Hi = UART1.RECESIEVE_BUFFOR_TAB[24];
					SSK_To_ISKK.IL3_Lo = UART1.RECESIEVE_BUFFOR_TAB[25];
				}
				if (UART1.RECESIEVE_BUFFOR_TAB[26] == kULx) {
					SSK_To_ISKK.UL1_Hi = UART1.RECESIEVE_BUFFOR_TAB[27];
					SSK_To_ISKK.UL1_Lo = UART1.RECESIEVE_BUFFOR_TAB[28];
					SSK_To_ISKK.UL2_Hi = UART1.RECESIEVE_BUFFOR_TAB[29];
					SSK_To_ISKK.UL2_Lo = UART1.RECESIEVE_BUFFOR_TAB[30];
					SSK_To_ISKK.UL3_Hi = UART1.RECESIEVE_BUFFOR_TAB[31];
					SSK_To_ISKK.UL3_Lo = UART1.RECESIEVE_BUFFOR_TAB[32];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[33] == kTemperatura){
					SSK_To_ISKK.Temperatura_Hi = UART1.RECESIEVE_BUFFOR_TAB[34];
					SSK_To_ISKK.Temperatura_Lo = UART1.RECESIEVE_BUFFOR_TAB[35];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[36] == kAddress_ADR){
					SSK_To_ISKK.MyProfibusAddress = UART1.RECESIEVE_BUFFOR_TAB[37];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[38] == kTimeSoftStart){
					sProfibus_Prm.SSK_TimeSoftStart = ((UART1.RECESIEVE_BUFFOR_TAB[39])<<8)|UART1.RECESIEVE_BUFFOR_TAB[40];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[41] == kAlfa){
					sProfibus_Prm.SSK_Alfa          = UART1.RECESIEVE_BUFFOR_TAB[42];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[43] == kfuse_Umax){
					sProfibus_Prm.SSK_fuse_Umax     = ((UART1.RECESIEVE_BUFFOR_TAB[44])<<8)|UART1.RECESIEVE_BUFFOR_TAB[45];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[46] == kfuse_Umin){
					sProfibus_Prm.SSK_fuse_Umin     = ((UART1.RECESIEVE_BUFFOR_TAB[47])<<8)|UART1.RECESIEVE_BUFFOR_TAB[48];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[49] == kfuse_Iznam){
					sProfibus_Prm.SSK_fuse_Iznam    = ((UART1.RECESIEVE_BUFFOR_TAB[50])<<8)|UART1.RECESIEVE_BUFFOR_TAB[51];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[52] == kfuse_Imax){
					sProfibus_Prm.SSK_fuse_Imax     = ((UART1.RECESIEVE_BUFFOR_TAB[53])<<8)|UART1.RECESIEVE_BUFFOR_TAB[54];
				}
				if(UART1.RECESIEVE_BUFFOR_TAB[55] == kfuse_Iroz){
					sProfibus_Prm.SSK_fuse_Iroz     = ((UART1.RECESIEVE_BUFFOR_TAB[56])<<8)|UART1.RECESIEVE_BUFFOR_TAB[57];
				}
			}else{
				UART1.RECESIEVE_BUFFOR_INDEX=0;
			}
		}
	}
}

