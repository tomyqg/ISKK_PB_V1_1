/***************************  Filename: DpCfg.c ******************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0                              */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      Einsteinstraﬂe 6        */
/* 0000  000  0   0 000   0 0     00000 0 0000       91074 Herzogenaurach    */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
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


*/
/*****************************************************************************/
/** @file
 * Copyright (C) by profichip GmbH   All Rights Reserved. Confidential
 *
 * @brief Handling of PROFIBUS-DP configuration telegram.
 *
 * @author Peter Fredehorst
 * @version $Rev$
 */

/* include hierarchy */

#include <string.h>
#include "platform.h"
#include "DpAppl.h"

/*---------------------------------------------------------------------------*/
/* defines, structures                                                       */
/*---------------------------------------------------------------------------*/
//default configuration data for startup
#define AllCfgDataLength ((uint8_t)0x04)
ROMCONST__ uint8_t AllCfgBytes[AllCfgDataLength] = { 0x3C,0x32,0x1F,0x1F};

//default configuration data for startup
#define DpApplCfgDataLength ((uint8_t)0x01)     /**< Length of configuration data. */
ROMCONST__ uint8_t DpApplDefCfg[DpApplCfgDataLength] = { 0x3C};  /**< Default configuration data. */

/*---------------------------------------------------------------------------*/
/* local user data definitions                                               */
/*---------------------------------------------------------------------------*/

/*---------------------------------------------------------------------------*/
/* function prototypes                                                       */
/*---------------------------------------------------------------------------*/
/*!
  \brief Init profibus configuration.
*/
void DpCfg_Init( void )
{
   //todo:
   //insert your real configuration data here
   sDpAppl.sCfgData.bLength = DpApplCfgDataLength; // length of configuration data
   memcpy( &sDpAppl.sCfgData.abData[0], &DpApplDefCfg[0], sDpAppl.sCfgData.bLength );
}//void DpCfg_Init( void )


/*---------------------------------------------------------------------------*/
/* function: DpCfg_FindCfgByte                                               */
/*---------------------------------------------------------------------------*/
/**
 * @brief Checking configuration data byte.
 * The byte must be defined in gsd-file and in AllCfgBytes[].
 *
 * @param[in] bCfg configuration data byte
 *
 * @return VPC3_TRUE The configuration byte is valid.
 * @return VPC3_FALSE The configuration byte is not valid.
 */
static uint8_t DpCfg_FindCfgByte( uint8_t bCfg )
{
uint8_t bRetValue;
uint8_t i;

   bRetValue = VPC3_FALSE;

   for( i = 0; i < AllCfgDataLength; i++ )
   {
      if( bCfg == AllCfgBytes[ i ] )
      {
         bRetValue = VPC3_TRUE;
         break;
      }//if( bCfg == AllCfgBytes[ i ] )
   }//for( i = 0; i < AllCfgDataLength; i++ )

   return bRetValue;
}//static uint8_t DpCfg_FindCfgByte( uint8_t bCfg )


/*---------------------------------------------------------------------------*/
/* function: DpCfg_ChkNewCfgData                                             */
/*---------------------------------------------------------------------------*/
/**
 * @brief Checking configuration data.
 * The function VPC3_Isr() or VPC3_Poll() calls this function if the VPC3+
 * has received a Check_Cfg message and has made the data available in the Cfg buffer.
 *
 * The user has to program the function for checking the received configuration data.
 *
 * @param[in] pbCfgData - address of check configuration data
 * @param[in] bCfgLength - length of configuration data
 *
 * @return see E_DP_CFG_ERROR @see E_DP_CFG_ERROR
 */
E_DP_CFG_ERROR DpCfg_ChkNewCfgData( MEM_UNSIGNED8_PTR pbCfgData, uint8_t bCfgLength )
{
	E_DP_CFG_ERROR eRetValue;
	uint8_t i;

	eRetValue = DP_CFG_UPDATE;

//max module in gsd-file: 20
		if( ( bCfgLength > 0 ) && ( bCfgLength <= 20 ) )
		{
			for( i = 0; i < bCfgLength; i++ )
			{
				if( DpCfg_FindCfgByte( *pbCfgData ) == VPC3_FALSE )
				{
					//cfg byte not defined
					eRetValue = DP_CFG_FAULT;
					break;
				} //if( DpCfg_FindCfgByte( *pbCfgData ) == VPC3_FALSE )

				pbCfgData++;
			} //for( i = 0; i < bCfgLength; i++ )
		} //if( ( bCfgLength > 0 ) && ( bCfgLength <= 20 ) )
		else
		{
			eRetValue = DP_CFG_FAULT;
		} //else of if( ( bCfgLength > 0 ) && ( bCfgLength <= 20 ) )

		if( ( eRetValue == DP_CFG_OK ) || ( eRetValue == DP_CFG_UPDATE ) )
		{
			eRetValue = DpDiag_SetCfgOk( eRetValue );
			if( eRetValue != DP_CFG_FAULT )
			{
				VPC3_SetDpState( eDpStateCfgOkStatDiag );
			}
		} //if( ( eRetValue == DP_CFG_OK ) || ( eRetValue == DP_CFG_UPDATE ) )

		DpDiag_AlarmInit();

		/*eRetValue = DP_CFG_OK;

		 if( bCfgLength == sDpAppl.sCfgData.bLength )
		 {
		 for( i = 0; i < bCfgLength; i++ )
		 {
		 if( sDpAppl.sCfgData.abData[ i ] != *pbCfgData )
		 {
		 eRetValue = DP_CFG_FAULT;
		 }//if( sDpAppl.sCfgData.abData[ i ] != *pbCfgData )

		 pbCfgData++;
		 }//for( i = 0; i < bCfgLength; i++ )
		 }//if( bCfgLength == sDpAppl.sCfgData.bLength )
		 else
		 {
		 eRetValue = DP_CFG_FAULT;
		 }//else of if( bCfgLength == sDpAppl.sCfgData.bLength )

		 if( ( eRetValue == DP_CFG_OK ) || ( eRetValue == DP_CFG_UPDATE ) )
		 {
		 eRetValue = DpDiag_SetCfgOk( eRetValue );
		 if( eRetValue != DP_CFG_FAULT )
		 {
		 VPC3_SetDpState( eDpStateCfgOkStatDiag );
		 }
		 }//if( ( eRetValue == DP_CFG_OK ) || ( eRetValue == DP_CFG_UPDATE ) )

		 DpDiag_AlarmInit();*/

		return eRetValue;
	} //E_DP_CFG_ERROR DpCfg_ChkNewCfgData( MEM_UNSIGNED8_PTR pbCfgData, uint8_t bCfgLength )

/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

