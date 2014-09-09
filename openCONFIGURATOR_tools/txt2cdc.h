/**
********************************************************************************
\file   txt2cdc.h

\brief  This header file contains macros and variables of types used in the
        txt2cdc.c

\copyright (c) 2014, Kalycito Infotech Private Limited
All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
    * Redistributions of source code must retain the above copyright
      notice, this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright
      notice, this list of conditions and the following disclaimer in the
      documentation and/or other materials provided with the distribution.
    * Neither the name of the copyright holders nor the
      names of its contributors may be used to endorse or promote products
      derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#ifndef _TXT2CDC_H_
#define _TXT2CDC_H_

#define INT8    char
#define INT16   short int
#define INT32   int
#define UINT8   unsigned char
#define UINT16  unsigned short int
#define UINT32  unsigned int

/**
\brief Return status value

This enumeration specifies the status value of the required condition.
*/
typedef enum
{
    kError                          = -1,       ///< General error
    kTxt2CdcSuccess_g,                          ///< Conversion success
    kTextFileFormattingError_g                  ///< Text format Error
} tResult;

//Offset Variables
const UINT32 kFirstInd1F22_g        = 11;
const UINT32 kFirstIndMnObd_g       = 4;
const UINT32 kEntryMn_g             = 0;
const UINT32 kEntryCn_g             = 7;
const UINT32 kIndex_g               = 0;
const UINT32 kSubIndex_g            = 2;
const UINT32 kDataSize_g            = 3;
const UINT32 kData_g                = 7;

//Buffer Variables
const UINT32 kBufSize_g             = 10;

//------------------------------------------------------------------------------
// Function prototypes
//------------------------------------------------------------------------------
INT8 asciiToHex(INT8);
INT32 reverseData(UINT8*, UINT8*, UINT32);

#ifdef DEBUG
#define DEBUG_INFO(...)     printf(__VA_ARGS__)
#else
#define DEBUG_INFO(...)     //
#endif

#endif // _TXT2CDC_H_
