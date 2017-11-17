/************************  Filename: platform.h  *****************************/
/* ========================================================================= */
/*                                                                           */
/* 0000  000   000  00000 0  000  0   0 0 0000                               */
/* 0   0 0  0 0   0 0     0 0   0 0   0 0 0   0      Einsteinstraße 6        */
/* 0   0 0  0 0   0 0     0 0     0   0 0 0   0      91074 Herzogenaurach    */
/* 0000  000  0   0 000   0 0     00000 0 0000       Germany                 */
/* 0     00   0   0 0     0 0     0   0 0 0                                  */
/* 0     0 0  0   0 0     0 0   0 0   0 0 0          Phone: ++499132744200   */
/* 0     0  0  000  0     0  000  0   0 0 0    GmbH  Fax:   ++4991327442164  */
/*                                                                           */
/* ========================================================================= */
/*                                                                           */
/* Description: User defines ( microcontroller, data types, ... ).           */
/*                                                                           */
/* ------------------------------------------------------------------------- */
/*                                                                           */
/* Technical support:       eMail: support@profichip.com                     */
/*                          Phone: ++49-9132-744-2150                        */
/*                          Fax. : ++49-9132-744-29-2150                     */
/*                                                                           */
/*****************************************************************************/
#include "stdint.h"
#include "stdbool.h"

/*****************************************************************************/
/* contents:

    - header include hierarchy for system environment

*/
/*****************************************************************************/
/* reinclude protection */

/*! \file
     \brief Header file for hardware / compiler configuration.

*/

#ifndef PLATFORM_CUST_H
#define PLATFORM_CUST_H

//#include "stdint.h"
/*****************************************************************************/
/* header include hierarchy for system environment */

/*---------------------------------------------------------------------------*/
/* VPC3+C/S interface mode                                                   */
/*---------------------------------------------------------------------------*/

/*!
   \def VPC3_SERIAL_MODE

   \brief Activate / Deactivate VPC3+S serial mode.

   Use following values:
   - 1 - Activate VPC3+S serial mode.
   - 0 - Deactivate VPC3+S serial mode - VPC3+C, VPC3+S, MPI12x works in parallel mode.
*/

#define VPC3_SERIAL_MODE 0

   /*!
      \def MOTOROLA_MODE

      \brief Activate / Deactivate VPC3+ motorola mode.

      Use following values:
      - 0 - Activate INTEL mode - the VPC3+ configuration pin MOT/XINT is set to parallel interface INTEL format.
      - 1 - Activate MOTOROLA mode - the VPC3+ configuration pin MOT/XINT is set to parallel interface MOTOROLA format.
   */

   #define MOTOROLA_MODE 0

   /*!
      \def BUSINTERFACE_xy_BIT

      \brief Define businterface between microcontroller and VPC3+C.

       8 bit microcontroller:
      16 bit microcontroller with possibility to set external businterface to 8bit:
      32 bit microcontroller with possibility to set external businterface to 8bit:

      #define BUSINTERFACE_8_BIT  1
      #define BUSINTERFACE_16_BIT 0
      #define BUSINTERFACE_32_BIT 0

      The following modes are not tested!!!!
      16 bit microcontroller:

      #define BUSINTERFACE_8_BIT  0
      #define BUSINTERFACE_16_BIT 1
      #define BUSINTERFACE_32_BIT 0

      32 bit microcontroller:

      #define BUSINTERFACE_8_BIT  0
      #define BUSINTERFACE_16_BIT 0
      #define BUSINTERFACE_32_BIT 1
   */

   #define BUSINTERFACE_8_BIT  1
   #define BUSINTERFACE_16_BIT 0
   #define BUSINTERFACE_32_BIT 0

   #if BUSINTERFACE_8_BIT
      #define VPC3_EXTENSION           // Fix, do not edit!!!
   #else
      #define VPC3_EXTENSION     .bLo  // Fix, do not edit!!!
   #endif//#if BUSINTERFACE_8_BIT

   #define DP_INTERRUPT_MASK_8BIT 1    // Standard mode for reading/acknowledging profibus indications in dp_isr.c


#define _PACKED_                       /*!< Feed a keyword for packing structures.*/

/*---------------------------------------------------------------------------*/
/* byte ordering                                                             */
/*---------------------------------------------------------------------------*/
// sign the byte order of a word in your device
// possible: LITTLE_ENDIAN   lower address  - low byte
//                           higher address - high byte
//           BIG_ENDIAN      lower address  - high byte
//                           higher address - low byte

#define LITTLE_ENDIAN 1
#define BIG_ENDIAN    0

/*---------------------------------------------------------------------------*/
/* include hierarchy, data types                                             */
/*---------------------------------------------------------------------------*/

#define TRUE                     1
#define FALSE                    !(TRUE)

//#define BOOL                     unsigned char           /*!<  1 bit  basic type */
//#define uint8_t                  unsigned char           /*!<  8 bits basic type */
//#define uint16_t                 unsigned short          /*!< 16 bits basic type */
//#define uint32_t                 unsigned long           /*!< 32 bits basic type */

#define PTR_ATTR                                         /*!< Attribute of the pointers to the asic. ( compiler-,microcontroller specific: xdata, near, far, huge ... ) */
#define VPC3_PTR                 PTR_ATTR *              /*!< Pointer attribut */
#define VPC3_ADR                 uint32_t                /*!< Attribute of the asic address. ( uint16_t, uint32_t ) */
#define VPC3_UNSIGNED8_PTR       uint8_t PTR_ATTR *      /*!< Pointer of byte to VPC3+ */
#define VPC3_NULL_PTR            (void VPC3_PTR)0
#define VPC3_TRUE                TRUE
#define VPC3_FALSE               FALSE


#define MEM_PTR_ATTR                                     /*!< Attribute of the internal variable pointer. ( compiler-,microcontroller specific: xdata, near, far, huge ... ) */
#define MEM_PTR                  MEM_PTR_ATTR *          /*!< Pointer attribut of local memory */
#define MEM_UNSIGNED8_PTR        uint8_t MEM_PTR_ATTR *  /*!< Pointer of byte to local memory */

#define ROMCONST__               const                 /*!< Attribute of const variables. ( compiler-,microcontroller specific: code, const, rom ) */

#include "DpCfg.h"                   // default configuration
#include "dpl_list.h"               // double pointered list, makros
#include "dp_if.h"                  // vpc3 structure, makros
#include "dp_inc.h"                 // vpc3 structure, makros


/*---------------------------------------------------------------------*/
/* XRAM area                                                           */
/*---------------------------------------------------------------------*/
  #define VPC3_ASIC_ADDRESS        ((unsigned char *)0x60000000)       /*!< VPC3+ address. */



/*****************************************************************************/
/* reinclude-protection */


#else
    #pragma message "The header PLATFORM_CUST.H is included twice or more !"
#endif


/*****************************************************************************/
/*  Copyright (C) profichip GmbH 2009. Confidential.                         */
/*****************************************************************************/

