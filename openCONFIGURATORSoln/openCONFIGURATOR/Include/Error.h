/**
 *****************************************************************************************************
 \file			Error.h

 \brief			Handles the definitions of error messages for the Application
 *****************************************************************************************************
 */

/*

 (c) Kalycito Infotech Private Limited

 License:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of Kalycito Infotech Private Limited nor the names of 
 its contributors may be used to endorse or promote products derived
 from this software without prior written permission. For written
 permission, please contact info@kalycito.com.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 Severability Clause:

 If a provision of this License is or becomes illegal, invalid or
 unenforceable in any jurisdiction, that shall not affect:
 1. the validity or enforceability in that jurisdiction of any other
 provision of this License; or
 2. the validity or enforceability in other jurisdictions of that or
 any other provision of this License.

 ****************************************************************************/

#ifndef error_h
#define error_h

/****************************************************************************************************/
/* Includes */

#include "Exports.h"

/*************************************************************************************************/
/* Enumerations */

typedef enum
{
//        OCFM_ERR_FAIL = -1,
	OCFM_ERR_SUCCESS = 0,
	OCFM_ERR_FILE_NOT_PRESENT, /*1*/
	OCFM_ERR_FILE_CANNOT_OPEN, /*2*/
	OCFM_ERR_INVALID_NODEID, /*3*/
	OCFM_ERR_INVALID_NODEPOS,/*4*/
	OCFM_ERR_INVALID_NODETYPE,/*5*/
	OCFM_ERR_INVALID_INDEXID,/*6*/
	OCFM_ERR_INVALID_INDEXPOS,/*7*/
	OCFM_ERR_INVALID_SUBINDEXID,/*8*/
	OCFM_ERR_INVALID_SUBINDEXPOS,/*9*/
	OCFM_ERR_INVALID_ATTRIBUTETYPE,/*10*/
	OCFM_ERR_NO_NODES_FOUND,/*11*/
	OCFM_ERR_NO_INDEX_FOUND,/*12*/
	OCFM_ERR_NO_SUBINDEXS_FOUND,/*13*/
	OCFM_ERR_NODEID_NOT_FOUND,/*14*/
	OCFM_ERR_INDEXID_NOT_FOUND,/*15*/
	OCFM_ERR_SUBINDEXID_NOT_FOUND,/*16*/
	OCFM_ERR_NODE_ALREADY_EXISTS,/*17*/
	OCFM_ERR_INDEX_ALREADY_EXISTS,/*18*/
	OCFM_ERR_SUBINDEX_ALREADY_EXISTS,/*19*/
	OCFM_ERR_INVALID_VALUE,/*20*/
	OCFM_ERR_INVALID_NAME,/*21*/
	OCFM_ERR_XML_FILE_CORRUPTED,/*22*/
	OCFM_ERR_CANNOT_OPEN_FILE,/*23*/
	OCFM_ERR_PARSE_XML,/*24*/
	OCFM_ERR_MODULE_INDEX_NOT_FOUND,/*25*/
	OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND,/*26*/
	OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND,/*27*/
	OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND,/*28*/
	OCFM_ERR_NO_CN_NODES_FOUND,/*29*/
	OCFM_ERR_DATATYPE_NOT_FOUND,/*30*/
	OCFM_ERR_VALUE_NOT_WITHIN_RANGE,/*31*/
	OCFM_ERR_MN_NODE_DOESNT_EXIST,/*32*/
	OCFM_ERR_CREATE_XML_WRITER_FAILED,/*33*/
	OCFM_ERR_XML_WRITER_START_ELT_FAILED,/*34*/
	OCFM_ERR_XML_WRITER_END_ELT_FAILED,/*35*/
	OCFM_ERR_XML_START_DOC_FAILED,/*36*/
	OCFM_ERR_XML_END_DOC_FAILED,/*37*/
	OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH,/*38*/
	OCFM_ERR_INVALID_PJTXML,/*39*/
	OCFM_ERR_PROJECT_SETTINGS,/*40*/
	OCFM_ERR_INVALID_DATATYPE_FOR_PDO,/*41*/
	OCFM_ERR_XAP_FILE_NOT_WRITTEN,/*42*/
	OCFM_ERR_MAX_PI_SIZE,/*43*/
	OCFM_ERR_UNKNOWN,/*44*/
	OCFM_ERR_MULTIPLEX_ASSIGN_ERROR,/*45*/
	OCFM_ERR_INVALID_UPPERLOWER_LIMITS, /*46*/
	OCFM_ERR_LOW_CNPRESTIMEOUT, /*47*/
	OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN, /*48*/
	OCFM_ERR_EXCESS_CHANNEL,/*49*/
	OCFM_ERR_INVALID_TXT_FOR_CDC, /*50*/
	OCFM_ERR_MEMORY_ALLOCATION_ERROR, /*51*/
	OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS, /*52*/
	OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND, /*53*/
	OCFM_ERR_INVALID_PARAMETER, /*54*/
	OCFM_ERR_INVALID_SIZE_MAPPED, /*55*/
	OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO /*56*/
} ConfiguratorErrors;

/************************************************************************************************/
/* Structures */

DLLEXPORT typedef struct ocfmRetCode
{
		ConfiguratorErrors code;/* Error code from ConfiguratorErrors*/
		char* errorString; /* String Describes the error */
} ocfmRetCode;

#endif // Error_h
