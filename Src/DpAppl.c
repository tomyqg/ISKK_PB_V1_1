/**************************  Filename: DpAppl.c  *****************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0      Einsteinstraﬂe 6        */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      91074 Herzogenaurach    */
/* 0000  000  0   0 000   0 0     00000 0 0000       Germany                 */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Function:      Demo for PROFICHIP Extension Board AT89C5132               */
/*                This example simulates simple profibus device              */
/*                with 2 byte of input and 2 byte of output data.            */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Hardware requirements: ProfiChip Evaluation Board AT89C5132  (PA006101)   */
/*                        ProfiChip Evaluation Board VPC3+/C    (PA006103)   */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/* memory:  0000H ... 7FFFH: RAM                                             */
/*          8000H ... 8FFFH: VPC3+                                           */
/*          9000H ... 9FFFH: Reserved                                        */
/*          A000H ... AFFFH: FPGA                                            */
/*          B000H ... BFFFH: RTC                                             */
/*          C000H ... CFFFH: LCD                                             */
/*          D000H ... DFFFH: I/O Port 0                                      */
/*          E000H ... EFFFH: I/O Port 1                                      */
/*          F000H ... FFFFH: I/O Port 2                                      */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       eMail: support@profichip.com                     */
/*                          Phone: ++49-9132-744-2150                        */
/*                          Fax. : ++49-9132-744-29-2150                     */
/*                                                                           */
/*****************************************************************************/


/*****************************************************************************/
/* contents:

  - function prototypes
  - data structures
  - internal functions

*/
/*****************************************************************************/
/* include hierarchy */

#include <string.h>
#include "platform.h"
#include "DpAppl.h"
#include "stm32f1xx_hal.h"
#include "usart.h"
#include "fsmc.h"
#include "ISKK.h"

/*---------------------------------------------------------------------------*/
/* defines, structures                                                       */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* global user data definitions                                              */
/*---------------------------------------------------------------------------*/
VPC3_STRUC_PTR             pVpc3;               /**< Pointer to VPC3+ structure. */
VPC3_STRUC_PTR             pVpc3Channel1;       /**< Pointer to VPC3+ structure channel 1. */

VPC3_ADR                   Vpc3AsicAddress;     /**< Global VPC3 address. */

VPC3_SYSTEM_STRUC_PTR      pDpSystem;           /**< Pointer to global profibus system structure. */
VPC3_SYSTEM_STRUC          sDpSystemChannel1;   /**< Global profibus system structure. */

VPC3_STRUC_ERRCB           sVpc3Error;          /**< Error structure. */
DP_APPL_STRUC              sDpAppl;             /**< User application structure. */

/*---------------------------------------------------------------------------*/
/* defines, structures and variables for our demo application                */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/

/*--------------------------------------------------------------------------*/
/* function: DpAppl_TestApplEvent                                           */
/*--------------------------------------------------------------------------*/
/*!
  \brief Check if the internal PROFIBUS event is set and clear the event.

  \param[in] eDpApplEv PROFIBUS event @see eDpApplEv_Flags

  \retval VPC3_TRUE Event was set
  \retval VPC3_FALSE Event was not set.
*/
static uint8_t DpAppl_TestApplEvent( eDpApplEv_Flags eDpApplEv )
{
   if( sDpAppl.eDpApplEvent & eDpApplEv )
   {
      sDpAppl.eDpApplEvent &= ~eDpApplEv;
      return VPC3_TRUE;
   }//if( sDpAppl.eDpApplEvent & eDpApplEv )

   return VPC3_FALSE;
}//static uint8_t DpAppl_TestApplEvent( eDpApplEv_Flags eDpApplEv )

/*--------------------------------------------------------------------------*/
/* function: DpAppl_SetApplEvent                                            */
/*--------------------------------------------------------------------------*/
/*!
  \brief Set PROFIBUS event.

  \param[in] eDpApplEv PROFIBUS event @see eDpApplEv_Flags
*/
static void DpAppl_SetApplEvent( eDpApplEv_Flags eDpApplEv )
{
   sDpAppl.eDpApplEvent |= eDpApplEv;
}//static void DpAppl_SetApplEvent( eDpApplEv_Flags eDpApplEv )

/*--------------------------------------------------------------------------*/
/* function: DpAppl_CheckEvIoOut                                            */
/*--------------------------------------------------------------------------*/
/*!
  \brief Handling of the PROFIBUS output data ( master to slave ).

  The VPC3+ has received a DataExchange message and has made the new output data
  available in the N-buffer. In the case of Power_On or Leave_Master, the
  VPC3+ clears the content of the buffer, and generates this event also.
*/
static void DpAppl_CheckEvIoOut( void )
{
VPC3_UNSIGNED8_PTR  pToOutputBuffer;   /**< Pointer to output buffer. */
uint8_t             bOutputState;      /**< State of output data. */

   if( DpAppl_TestApplEvent( eDpApplEv_IoOut ) )
   {
      pToOutputBuffer = VPC3_GetDoutBufPtr( &bOutputState );
      if( pToOutputBuffer )
      {
         CopyFromVpc3_( &sDpAppl.abDpOutputData[0], pToOutputBuffer, pDpSystem->bOutputDataLength );

			if(SSK_To_ISKK.Control!=sDpAppl.abDpOutputData[0]){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
				SSK_To_ISKK.Control=sDpAppl.abDpOutputData[0];
				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_1Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kControl;
				UART1.TRANSMIT_BUFFOR_TAB[3]=SSK_To_ISKK.Control;
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,4,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}
			if(sProfibus_Prm.TimeSoftStart!=sProfibus_Prm.SSK_TimeSoftStart){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_2Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kTimeSoftStart;
				UART1.TRANSMIT_BUFFOR_TAB[3]=(sProfibus_Prm.TimeSoftStart>>8);
				UART1.TRANSMIT_BUFFOR_TAB[4]=(sProfibus_Prm.TimeSoftStart & 0xFF);
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,5,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}
			if(sProfibus_Prm.Alfa!=sProfibus_Prm.SSK_Alfa){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_1Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kAlfa;
				UART1.TRANSMIT_BUFFOR_TAB[3]=sProfibus_Prm.Alfa;
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,4,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}

			if(sProfibus_Prm.fuse_Umax!=sProfibus_Prm.SSK_fuse_Umax){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);

				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_2Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kfuse_Umax;
				UART1.TRANSMIT_BUFFOR_TAB[3]=(sProfibus_Prm.fuse_Umax>>8);
				UART1.TRANSMIT_BUFFOR_TAB[4]=(sProfibus_Prm.fuse_Umax & 0xFF);
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,5,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}
			if(sProfibus_Prm.fuse_Umin!=sProfibus_Prm.fuse_Umin){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_2Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kfuse_Umin;
				UART1.TRANSMIT_BUFFOR_TAB[3]=(sProfibus_Prm.fuse_Umin>>8);
				UART1.TRANSMIT_BUFFOR_TAB[4]=(sProfibus_Prm.fuse_Umin & 0xFF);
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,5,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}
			if(sProfibus_Prm.fuse_Iznam!=sProfibus_Prm.fuse_Iznam){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_1Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kfuse_Iznam;
				UART1.TRANSMIT_BUFFOR_TAB[3]=sProfibus_Prm.fuse_Iznam;
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,4,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}
			if(sProfibus_Prm.fuse_Imax!=sProfibus_Prm.fuse_Imax){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_1Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kfuse_Imax;
				UART1.TRANSMIT_BUFFOR_TAB[3]=sProfibus_Prm.fuse_Imax;
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,4,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}
			if(sProfibus_Prm.fuse_Iroz!=sProfibus_Prm.fuse_Iroz){
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_SET);
				UART1.TRANSMIT_BUFFOR_TAB[0]=k_Frame_1Byte;
				UART1.TRANSMIT_BUFFOR_TAB[1]=0xFF;
				UART1.TRANSMIT_BUFFOR_TAB[2]=kfuse_Iroz;
				UART1.TRANSMIT_BUFFOR_TAB[3]=sProfibus_Prm.fuse_Iroz;
				HAL_UART_Transmit(&huart1,UART1.TRANSMIT_BUFFOR_TAB,4,100);
				HAL_GPIO_WritePin(GPIOA,GPIO_PIN_12,GPIO_PIN_RESET);
			}

         /** @todo Write the output data to the output modules. */

      }//if( pToOutputBuffer )
   }//if( DpAppl_TestApplEvent( eDpApplEv_IoOut ) )
}//static void DpAppl_CheckEvIoOut( void )

/*--------------------------------------------------------------------------*/
/* function: DpAppl_ReadInputData                                           */
/*--------------------------------------------------------------------------*/
/*!
  \brief Handling of the PROFIBUS input data ( slave --> master ).
*/
static void DpAppl_ReadInputData( void )
{
   /** @todo Read cyclically the input module. */
	//sDpAppl.abDpInputData[0]=Profibus_Module1.Reset;
	//sDpAppl.abDpInputData[1]=Profibus_Module1.Kierunek;
	//sDpAppl.abDpInputData[2]=(Profibus_Module1.TimeSoftStart>>8);
	//sDpAppl.abDpInputData[3]=(Profibus_Module1.TimeSoftStart & 0xFF);
	//sDpAppl.abDpInputData[4]=Profibus_Module1.Alfa;
	//sDpAppl.abDpInputData[5]=Profibus_Module1.Start;
	//sDpAppl.abDpInputData[6]=(Profibus_Module1.fuse_Umax>>8);
	//sDpAppl.abDpInputData[7]=(Profibus_Module1.fuse_Umax & 0xFF);
	//sDpAppl.abDpInputData[8]=(Profibus_Module1.fuse_Umin>>8);
	//sDpAppl.abDpInputData[9]=(Profibus_Module1.fuse_Umin & 0xFF);
	//sDpAppl.abDpInputData[10]=Profibus_Module1.fuse_Iznam;
	//sDpAppl.abDpInputData[11]=Profibus_Module1.fuse_Imax;
	//sDpAppl.abDpInputData[12]=Profibus_Module1.fuse_Iroz;
	//sDpAppl.abDpInputData[13]=Profibus_Module1.State_Input_Hi;
	//sDpAppl.abDpInputData[14]=Profibus_Module1.State_Input_Lo;
	//sDpAppl.abDpInputData[15]=Profibus_Module1.State_Output;
	//sDpAppl.abDpInputData[16]=Profibus_Module1.Temperatura_Hi;
	//sDpAppl.abDpInputData[17]=Profibus_Module1.Temperatura_Lo;
	//sDpAppl.abDpInputData[18]=Profibus_Module1.AIN0_Hi;
	//sDpAppl.abDpInputData[19]=Profibus_Module1.AIN0_Lo;
	//sDpAppl.abDpInputData[20]=Profibus_Module1.AIN1_Hi;
	//sDpAppl.abDpInputData[21]=Profibus_Module1.AIN1_Lo;
	//sDpAppl.abDpInputData[22]=Profibus_Module1.AIN2_Hi;
	//sDpAppl.abDpInputData[23]=Profibus_Module1.AIN2_Lo;
	//sDpAppl.abDpInputData[24]=Profibus_Module1.AIN3_Hi;
	//sDpAppl.abDpInputData[25]=Profibus_Module1.AIN3_Lo;
	//sDpAppl.abDpInputData[26]=Profibus_Module1.IL1_Hi;
	//sDpAppl.abDpInputData[27]=Profibus_Module1.IL1_Lo;
	//sDpAppl.abDpInputData[28]=Profibus_Module1.IL2_Hi;
	//sDpAppl.abDpInputData[29]=Profibus_Module1.IL2_Lo;
	//sDpAppl.abDpInputData[30]=Profibus_Module1.IL3_Hi;
	//sDpAppl.abDpInputData[31]=Profibus_Module1.IL3_Lo;
	//sDpAppl.abDpInputData[32]=Profibus_Module1.UL1_Hi;
	//sDpAppl.abDpInputData[33]=Profibus_Module1.UL1_Lo;
	//sDpAppl.abDpInputData[34]=Profibus_Module1.UL2_Hi;
	//sDpAppl.abDpInputData[35]=Profibus_Module1.UL2_Lo;
	//sDpAppl.abDpInputData[36]=Profibus_Module1.UL3_Hi;
	//sDpAppl.abDpInputData[37]=Profibus_Module1.UL3_Lo;
	//sDpAppl.abDpInputData[38]=Profibus_Module1.Flaga_Hi;
	//sDpAppl.abDpInputData[39]=Profibus_Module1.Flaga_Lo;

	//Dane do PLC
	sDpAppl.abDpInputData[0] =SSK_To_ISKK.State_Input_Hi;
	sDpAppl.abDpInputData[1] =SSK_To_ISKK.State_Input_Lo;
	sDpAppl.abDpInputData[2] =SSK_To_ISKK.Flaga_Hi;
	sDpAppl.abDpInputData[3] =SSK_To_ISKK.Flaga_Lo;
	sDpAppl.abDpInputData[4] =SSK_To_ISKK.AIN0_Hi;
	sDpAppl.abDpInputData[5] =SSK_To_ISKK.AIN0_Lo;
	sDpAppl.abDpInputData[6] =SSK_To_ISKK.AIN1_Hi;
	sDpAppl.abDpInputData[7] =SSK_To_ISKK.AIN1_Lo;
	sDpAppl.abDpInputData[8] =SSK_To_ISKK.AIN2_Hi;
	sDpAppl.abDpInputData[9]=SSK_To_ISKK.AIN2_Lo;
	sDpAppl.abDpInputData[10]=SSK_To_ISKK.AIN3_Hi;
	sDpAppl.abDpInputData[11]=SSK_To_ISKK.AIN3_Lo;
	sDpAppl.abDpInputData[12]=SSK_To_ISKK.IL1_Hi;
	sDpAppl.abDpInputData[13]=SSK_To_ISKK.IL1_Lo;
	sDpAppl.abDpInputData[14]=SSK_To_ISKK.IL2_Hi;
	sDpAppl.abDpInputData[15]=SSK_To_ISKK.IL2_Lo;
	sDpAppl.abDpInputData[16]=SSK_To_ISKK.IL3_Hi;
	sDpAppl.abDpInputData[17]=SSK_To_ISKK.IL3_Lo;
	sDpAppl.abDpInputData[18]=SSK_To_ISKK.UL1_Hi;
	sDpAppl.abDpInputData[19]=SSK_To_ISKK.UL1_Lo;
	sDpAppl.abDpInputData[20]=SSK_To_ISKK.UL2_Hi;
	sDpAppl.abDpInputData[21]=SSK_To_ISKK.UL2_Lo;
	sDpAppl.abDpInputData[22]=SSK_To_ISKK.UL3_Hi;
	sDpAppl.abDpInputData[23]=SSK_To_ISKK.UL3_Lo;
	sDpAppl.abDpInputData[24]=SSK_To_ISKK.Temperatura_Hi;
	sDpAppl.abDpInputData[25]=SSK_To_ISKK.Temperatura_Lo;

   VPC3_InputDataUpdate( &sDpAppl.abDpInputData[0] );
}//static void DpAppl_ReadInputData( void )

/*--------------------------------------------------------------------------*/
/* function: DpAppl_ApplicationReady                                        */
/*--------------------------------------------------------------------------*/
/*!
  \brief ApplicatioReady

  This function is called after the PROFIBUS configuration data has been acknowledged
  positive by the user. The slave is now in DataExchange but the static diagnostic bit is set.
  The user can do here own additional initialization and should read here the input data. The
  slave delete now the static diagnostic bit and the master will send DataExchange telegrams.
*/
static void DpAppl_ApplicationReady( void )
{
   #if DPV1_IM_SUPP
      DpIm_ClearImIndex( 0x03 );
   #endif//#if DPV1_IM_SUPP

   /** @todo Make here your own initialization. */

   //read input data
   DpAppl_ReadInputData();

   //reset Diag.Stat
   if( DpDiag_ResetStatDiag() )
   {
      VPC3_ClrDpState( eDpStateCfgOkStatDiag );
      VPC3_SetDpState( eDpStateApplReady );
   }//if( DpDiag_ResetStatDiag() )
}//static void DpAppl_ApplicationReady( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_ProfibusInit                                             */
/*---------------------------------------------------------------------------*/
/*!
  \brief Initializing of PROFIBUS slave communication.
*/
void DpAppl_ProfibusInit( void )
{
DP_ERROR_CODE       bError;

   /*-----------------------------------------------------------------------*/
   /* init application data                                                 */
   /*-----------------------------------------------------------------------*/
   memset( &sDpAppl, 0, sizeof( sDpAppl ) );

   /*-----------------------------------------------------------------------*/
   /* initialize VPC3                                                       */
   /*-----------------------------------------------------------------------*/
      pVpc3Channel1 = (VPC3_STRUC_PTR)VPC3_ASIC_ADDRESS;
      Vpc3AsicAddress = (VPC3_ADR)VPC3_ASIC_ADDRESS;
      pVpc3 = pVpc3Channel1;
      pDpSystem = &sDpSystemChannel1;


   /*-----------------------------------------------------------------------*/
   /* initialize global system structure                                    */
   /*-----------------------------------------------------------------------*/
   memset( pDpSystem, 0, sizeof( VPC3_SYSTEM_STRUC ));
   pDpSystem->eDpState = eDpStateInit;

   DpPrm_Init();
   DpCfg_Init();
   DpDiag_Init();

   DpAppl_ClrResetVPC3Channel1();

   bError = VPC3_MemoryTest();

   if( DP_OK == bError )
   {
         bError = VPC3_Initialization( SSK_To_ISKK.MyProfibusAddress/*DP_ADDR*/, IDENT_NR, (psCFG)&sDpAppl.sCfgData );                  // address of slave

      if( DP_OK == bError )
      {
         DpAppl_EnableInterruptVPC3Channel1();

         //todo: before startup the VPC3+, make here your own initialzations

         VPC3_Start();
      }//if( DP_OK == bError )
      else
      {
         sVpc3Error.bErrorCode = bError;
         DpAppl_FatalError( _DP_USER, __LINE__, &sVpc3Error );
      }//else of if( DP_OK == bError )
   }//if( DP_OK == bError )
   else
   {
      sVpc3Error.bErrorCode = bError;
      DpAppl_FatalError( _DP_USER, __LINE__, &sVpc3Error );
   }//else of if( DP_OK == bError )
}//void DpAppl_ProfibusInit( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_ProfibusMain                                                    */
/*---------------------------------------------------------------------------*/
/*!
  \brief The application program has to call this function cyclically so that the PROFIBUS DP slave services can be processed.
*/
void DpAppl_ProfibusMain( void )
{

uint8_t bDpState;

   /*-------------------------------------------------------------------*/
   /* trigger watchdogs                                                 */
   /*-------------------------------------------------------------------*/
   // toggle user watchdog
   VPC3_RESET_USER_WD();   // toggle user watchdog

   /*-------------------------------------------------------------------*/
   /* internal state machine                                            */
   /*-------------------------------------------------------------------*/
   if( VPC3_GetDpState( eDpStateInit ) )
   {
      // clear data
      memset( &sDpAppl.abDpOutputData , 0, DOUT_BUFSIZE );
      memset( &sDpAppl.abDpInputData,   0, DIN_BUFSIZE );

      VPC3_ClrDpState( eDpStateInit );
      VPC3_SetDpState( eDpStateRun );
   }//if( VPC3_GetDpState( eDpStateInit ) )

   /*-------------------------------------------------------------------*/
   /* VPC3+ DP-state                                                    */
   /*-------------------------------------------------------------------*/
      bDpState = VPC3_GET_DP_STATE();

   switch( bDpState )
   {
      case WAIT_PRM:
      {
    	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
    	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);

         break;
      }//case WAIT_PRM:

      case WAIT_CFG:
      {
    	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_SET);
    	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_RESET);

         break;
      }//case WAIT_CFG:

      case DATA_EX:
      {
    	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_14,GPIO_PIN_RESET);
    	  HAL_GPIO_WritePin(GPIOB,GPIO_PIN_15,GPIO_PIN_SET);

         if(( VPC3_GetDpState( eDpStateApplReady)) && ( VPC3_GetDpState( eDpStateRun )))
         {
            /*---------------------------------------------------------------*/
            /* profibus input ( slave to master )                            */
            /*---------------------------------------------------------------*/
            DpAppl_ReadInputData();
         }//if(    ( pDpSystem->bApplicationReady == VPC3_TRUE ) ...

         break;
      }//case DATA_EX:

      case DP_ERROR:
      default:
      {
         sVpc3Error.bErrorCode = VPC3_GET_DP_STATE();
         DpAppl_FatalError( _DP_USER, __LINE__, &sVpc3Error );
         break;
      }//case DP_ERROR:
   }//switch( bDpState )

   /*------------------------------------------------------------------------*/
   /* profibus output ( master to slave )                                    */
   /*------------------------------------------------------------------------*/
   DpAppl_CheckEvIoOut();

   /*------------------------------------------------------------------------*/
   /* handle here profibus interrupt events                                  */
   /*------------------------------------------------------------------------*/
   if( VPC3_GetDpState( eDpStateCfgOkStatDiag ) )
   {
      DpAppl_ApplicationReady();
   }//if( VPC3_GetDpState( eDpStateCfgOkStatDiag ) )
}//void DpAppl_ProfibusMain( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_FatalError                                               */
/*---------------------------------------------------------------------------*/
void DpAppl_FatalError( DP_ERROR_FILE bFile, uint16_t wLine, VPC3_ERRCB_PTR sVpc3Error )
{
      while(1)
      {
      }//while(1)

}//void DpAppl_FatalError( DP_ERROR_FILE bFile, uint16_t wLine, VPC3_ERRCB_PTR sVpc3Error )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_MacReset                                                 */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the
   VPC3+ has entered the offline mode (by setting the Go_Offline bit in Moderegister 1).
*/
#if( DP_TIMESTAMP == 0 )
void DpAppl_MacReset( void )
{
   //print_string("\r\nDpAppl_MacReset");
} /* void DpAppl_MacReset( void ) */
#endif /* #if( DP_TIMESTAMP == 0 ) */

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrGoLeaveDataExchange                                   */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the
   DP-Statemachine has entered the DataEx-mode or has exited it.
   With the function VPC3_GET_DP_STATE() you can find out the state of VPC3+.
   \param[in] bDpState - state of PROFIBUS connection (WAIT_PRM,WAIT_CFG,DATA_EX)
*/
void DpAppl_IsrGoLeaveDataExchange( uint8_t bDpState )
{
   if( bDpState != DATA_EX )
   {
      VPC3_ClrDpState( eDpStateApplReady | eDpStateRun );
      VPC3_SetDpState( eDpStateInit );
   }//if( bDpState != DATA_EX )
}//void DpAppl_IsrGoLeaveDataExchange( uint8_t bDpState )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrDxOut                                                 */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has received a DataExchange message and has made the new output data
   available in the N-buffer. In the case of Power_On or Leave_Master, the
   VPC3+ clears the content of the buffer, and generates this event also.
*/
void DpAppl_IsrDxOut( void )
{
   DpAppl_SetApplEvent( eDpApplEv_IoOut );
}//void DpAppl_IsrDxOut( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrNewWdDpTimeout                                        */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the
   watchdog timer expired in the WD mode DP_Control.
   The communication between master and slave is time controlled, every time you're
   disconnecting the PROFIBUS master or you're disconnecting the PROFIBUS cable you'll
   get this event.
*/
void DpAppl_IsrNewWdDpTimeout( void )
{
    //not used in our application
}//void DpAppl_IsrNewWdDpTimeout( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrClockSynchronisation                                  */
/*---------------------------------------------------------------------------*/
#if DP_TIMESTAMP
   void DpAppl_IsrClockSynchronisation( void )
   {
      //not used in our application
   }//void DpAppl_IsrClockSynchronisation( void )
#endif//#if DP_TIMESTAMP

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrBaudrateDetect                                        */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has exited the Baud_Search mode and has found a baudrate.
   With the macro VPC3_GET_BAUDRATE() you can detect the baudrate.
*/
void DpAppl_IsrBaudrateDetect( void )
{
   //not used in our application
}//void DpAppl_IsrBaudrateDetect( void )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrNewGlobalControlCommand                               */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has received a Global_Control message. The GC_Command_Byte can be read out
   with the macro VPC3_GET_GC_COMMAND().
   \param[in] bGcCommand - global control command @see VPC3_GET_GC_COMMAND()
*/
void DpAppl_IsrNewGlobalControlCommand( uint8_t bGcCommand )
{
   //not used in our application
   //bGcCommand = bGcCommand;   //avoid compiler warning
}//void DpAppl_IsrNewGlobalControlCommand( uint8_t bGcCommand )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrNewSetSlaveAddress                                    */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
   has received a Set_Slave_Address message and made the data available in the SSA
   buffer.
  \param[in] psSsa - pointer to set slave address structure
*/
void DpAppl_IsrNewSetSlaveAddress( MEM_STRUC_SSA_BLOCK_PTR psSsa )
{
    //not used in our application
    //psSsa = psSsa; //avoid compiler warning
/*
   //store the new address and the bit bNoAddressChanged for the next startup
   print_string("\r\nNewAddr: ");
   print_hexbyte(psSsa->bTsAddr);
   print_hexbyte(psSsa->bNoAddressChanged);
   print_hexbyte(psSsa->bIdentHigh);
   print_hexbyte(psSsa->bIdentLow);
*/
}//void DpAppl_IsrNewSetSlaveAddress( MEM_STRUC_SSA_BLOCK_PTR psSsa )

/*---------------------------------------------------------------------------*/
/* function: DpAppl_IsrTimerClock (10ms)                                     */
/*---------------------------------------------------------------------------*/
/*!
   \brief The function VPC3_Isr() calls this function if the time base
   of the User_Timer_Clock has expired (1/10ms).

   \attention Use this function only interrupt driven!
*/
void DpAppl_IsrTimerClock( void )
{
}//void DpAppl_IsrTimerClock( void )

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

