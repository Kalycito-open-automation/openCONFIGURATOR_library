/**
 *****************************************************************************
 \file		Enums.h

 \brief		This file contains the enum declarations for the openCONFIGURATOR library
 *****************************************************************************
 */

/*

 © Kalycito Infotech Private Limited

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
#ifndef ENUMS_H_
#define ENUMS_H_

#include "Exports.h"

#include <string>

/*****************************************************************************/
/* Macro definition */

/** Defines the Managing Node id. */
#define MN_NODEID			240
/** Defines the broadcast Node id. */
#define BROADCAST_NODEID	0

/** Defines the maximum number of ProcessImage variable that is contributed by a controlled Node. */
#define PI_VAR_COUNT		4000

/** Defines the Poll response default payload value. */
#define PRES_DEFAULT_PAYLOAD	36
/** Defines the Poll request default payload value. */
#define PREQ_DEFAULT_PAYLOAD	36

/** Defines the length of the string for which the number of entries value for an MN in the CDC. */
#define OFFSET_NUM_ENTRIES_MN		0
/** Defines the length of the string for which the number of entries value for an CN in the CDC. */
#define OFFSET_NUM_ENTRIES_CN		7
/** Defines the remaining length of the index id to be packed as a hexadecimal value. */
#define OFFSET_INDEX				0
/** Defines the remaining length of the subindex id to be packed as a hexadecimal value. */
#define OFFSET_SUBINDEX				2
/** Defines the offset size. */
#define OFFSET_SIZE					3
/** Defines the offset data size. */
#define OFFSET_DATA					7

/** Defines the maximum length of the index id. */
#define	INDEX_SIZE					4
/** Defines the maximum length of the subindex id. */
#define	SUBINDEX_SIZE				2
/** Defines the maximum length of the Node id. */
#define NODE_ID						3

/** Defines the maximum length of the error string. */
#define ERR_STRING_LEN				100

/** Defines the length of the index id to be packed as a hexadecimal value. */
#define OFFSET_FIRST_INDEX_IN_MN_OBD	4

/** Defines the total no of characters for the value in 1F22 index. */
#define OFFSET_FIRST_INDEX_IN_1F22		11
/** Defines the maximum number of cross traffic stations allowed for a controlled node. */
#define MAX_CN_CROSS_TRAFFIC_STN		3

/** Defines the index id for the NMT_MultiplCycleAssign_AU8 object.  */
#define MULTIPL_CYCLE_ASSIGN_OBJECT			"1F9B"
/** Defines the index id for the NMT_MNCNPResTimeout_AU32 object. */
#define MNCN_POLLRESPONSE_TIMEOUT_OBJECT	"1F92"

#if defined(WIN32) || defined(_WIN32)
#define PATH_SEPARATOR "\\"
#else
#define PATH_SEPARATOR "/"
#endif

/*****************************************************************************/
/* Typedefs */

/** unsigned char as UINT8 */
typedef unsigned char UINT8;
/** unsigned short as UINT16 */
typedef unsigned short UINT16;
/** unsigned int as UINT32 */
typedef unsigned int UINT32;
/** signed int as UINT32 */
typedef signed int INT32;
/** long int as LONG */
typedef long int LONG;
/** unsigned long int as ULONG */
typedef unsigned long int ULONG;


/******************************************************************************
 Enumerations
 *****************************************************************************/
/** @addtogroup odapi
*  @{
*/
/** 
* ObjectType is used to denote what kind of object is at that particular index within the Object Dictionary
*/
enum ObjectType
{
	INVALID = 0,	/**< 0: Invalid object type. */
	DEFTYPE = 5,	/**< 5: Denotes a static data type definition. */
	DEFSTRUCT = 6,	/**< 6: Defines a record type. */
	VAR = 7,		/**< 7: Denotes a single value. */
	ARRAY = 8,		/**< 8: A multiple data field object where each data field is a simple variable of the SAME basic data type. */
	RECORD = 9		/**< 9: A multiple data field object where the data fields may be any combination of simple variables. */
};
/** @} */

/** An enum PDOMapping.
 * This enum holds the list of PDO mapping type for the Object and SubObject
 */
enum PDOMapping
{
		NOT_DEFINED = -1, /**< Indicates the object has a default value set to not defined */
		NO,			/**< Indicates the object must not be mapped into a Process Data Objects */
		DEFAULT,	/**< Indicates the object is part of the default mapping */
		OPTIONAL,	/**< Indicates the object can be mapped into both Receive and Transmit Process Data Objects */
		RPDO,		/**< Indicates the object shall be mapped into a Receive Process Data Objects */
		TPDO		/**< Indicates the object shall be mapped into a Transmit Process Data Objects */
};


/** An enum NodeType.
 * This enum holds the list of available type of Nodes
 */
enum NodeType
{
	MN = 0,		/**< To indicate an Managing Node */
	CN = 1		/**< To indicate an Controlled Node or Slave */
};

/** An enum PDOType.
 * This enum holds the list of available type of Process Data Objects
 */
enum PDOType
{
	PDO_INVALID = 0, /**< Indicates the object is not a Process Data Object */
	PDO_TPDO = 1,	/**< 1: Indicates the Transmit Process Data Objects */
	PDO_RPDO = 2	/**< 2: Indicates the Receive Process Data Objects */
};

/** An enum FeatureType.
 * This enum holds the types of network management features for a Node
 */
enum FeatureType
{
	GENERAL_FEATURES = 0,	/**< General features */
	MN_FEATURES = 1,		/**< Managing node features */
	CN_FEATURES = 2			/**< Controlled node features */
};

/** An enum ParameterAccess.
 * This enum holds the list of accessTypes that defines which operations valid for the parameter
 */
enum ParameterAccess
{
	constant,			/**< Const access */
	ro,					/**< Read only */
	wr,					/**< Write only */
	rw,					/**< Read and write access */
	readWriteInput,		/**< Read and write access, but represents process input data */
	readWriteOutput,	/**< Read and write, access, but represents process output data */
	noAccess			/**< access denied */
};

/** An enum IEC_Datatype.
 * This enum holds the list of available IEC datatypes
 */
enum IEC_Datatype
{
	BITSTRING = 0,	/**< Bit string (1 bit) */
	BOOL,			/**< Bool (1 bit) */
	BYTE,			/**< Byte (8 bit) */
	_CHAR,			/**< Char (8 bit) */
	DWORD,			/**< Dword (32 bit) */
	LWORD,			/**< Lword (64 bit) */
	SINT,			/**< Signed short integer (1 byte) */
	INT,			/**< Signed integer (2 byte) */
	DINT,			/**< Double integer (4 byte) */
	LINT,			/**< Long integer (8 byte) */
	USINT,			/**< Unsigned short integer (1 byte) */
	UINT,			/**< Unsigned integer (2 byte) */
	UDINT,			/**< Unsigned double integer (4 byte) */
	ULINT,			/**< Unsigned long integer (8 byte) */
	REAL,			/**< REAL (4 byte) */
	LREAL,			/**< LREAL (8 byte) */
	STRING,			/**< STRING */
	WSTRING			/**< WSTRING to hold multi byte strings */
};

/** @addtogroup odapi
*  @{
*/
/** 
* List of available attributes for the index or subindex.
*/
enum AttributeType
{
	NAME = 0,		/**< Name attribute. */
	OBJECTTYPE,		/**< Object Type attribute. */
	DATATYPE,		/**< Data Type attribute. */
	ACCESSTYPE,		/**< Access Type attribute. */
	DEFAULTVALUE,	/**< Default value attribute. */
	ACTUALVALUE,	/**< Actual value attribute. */
	PDOMAPPING,		/**< PDO Mapping attribute. */
	LOWLIMIT,		/**< Low limit attribute. */
	HIGHLIMIT,		/**< High limit attribute. */
	FLAGIFINCDC,	/**< Include in CDC attribute <b>(DEPRECATED)</b>. */
};
/** @} */

/** An enum DateTime.
 * This enum holds the date & time attribute
 */
enum DateTime
{
	DATE = 0,	/**< Date attribute */
	TIME		/**< Time attribute */
};

/** An enum AutoGenerate.
 * This enum lists of generate modes available for the generation of the MN object dictionary.
 */
//enum AutoGenerate
//{
//	NO_AG = 0,	/**< Specifies the option for no auto generate mode */
//	YES_AG = 1	/**< Specifies the option for auto generate mode */
//};

/** An enum AutoSave.
 * This enum lists of save modes available for the values & objects in the project.
 */
//enum AutoSave
//{
//	YES_AS = 0,		/**< Saves the configuration automatically */
//	PROMPT_AS,		/**< Prompts the user to for saving the configuration */
//	DISCARD_AS		/**< Discards the configuration */
//};

/** An enum ViewMode.
 * This enum lists of view modes available for the objects in the project.
 */
//enum ViewMode
//{
//	SIMPLE = 0,	/**< Simple tree mode */
//	EXPERT		/**<  Advanced tree mode */
//};

/** An enum ProjectSettings.
 * This enum lists of project settings attributes available for the project.
 */
//enum ProjectSettings
//{
//	AUTOGENERATE = 0,	/**< specifies the autogenerate attriubte */
//	AUTOSAVE			/**< Specifies the autosave attribute */
//};

/** An enum StationType.
 * This enum holds the list of available Controlled Node's station types
 */
enum StationType
{
	NORMAL = 0,		/**< Normal station */
	MULTIPLEXED,	/**< The node is multiplexed access */
	CHAINED			/**< Chained station */
};

DLLEXPORT enum ConfiguratorError
{
	//        OCFM_ERR_FAIL = -1,
	OCFM_ERR_SUCCESS = 0,	/**< 0 */
	OCFM_ERR_FILE_NOT_PRESENT, /**< 1 */
	OCFM_ERR_FILE_CANNOT_OPEN, /**< 2 */
	OCFM_ERR_INVALID_NODEID, /**< 3 */
	OCFM_ERR_INVALID_NODEPOS, /**< 4 */
	OCFM_ERR_INVALID_NODETYPE, /**< 5 */
	OCFM_ERR_INVALID_INDEXID, /**< 6 */
	OCFM_ERR_INVALID_INDEXPOS, /**< 7 */
	OCFM_ERR_INVALID_SUBINDEXID, /**< 8 */
	OCFM_ERR_INVALID_SUBINDEXPOS, /**< 9 */
	OCFM_ERR_INVALID_ATTRIBUTETYPE, /**< 10 */
	OCFM_ERR_NO_NODES_FOUND, /**< 11 */
	OCFM_ERR_NO_INDEX_FOUND, /**< 12 */
	OCFM_ERR_NO_SUBINDEXS_FOUND, /**< 13 */
	OCFM_ERR_NODEID_NOT_FOUND, /**< 14 */
	OCFM_ERR_INDEXID_NOT_FOUND, /**< 15 */
	OCFM_ERR_SUBINDEXID_NOT_FOUND, /**< 16 */
	OCFM_ERR_NODE_ALREADY_EXISTS, /**< 17 */
	OCFM_ERR_INDEX_ALREADY_EXISTS, /**< 18 */
	OCFM_ERR_SUBINDEX_ALREADY_EXISTS, /**< 19 */
	OCFM_ERR_INVALID_VALUE, /**< 20*/
	OCFM_ERR_INVALID_NAME, /**< 21*/
	OCFM_ERR_XML_FILE_CORRUPTED, /**< 22 */
	OCFM_ERR_CANNOT_OPEN_FILE, /**< 23 */
	OCFM_ERR_PARSE_XML, /**< 24*/
	OCFM_ERR_MODULE_INDEX_NOT_FOUND, /**< 25 */
	OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND, /**< 26 */
	OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND, /**< 27 */
	OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND, /**< 28 */
	OCFM_ERR_NO_CN_NODES_FOUND, /**< 29 */
	OCFM_ERR_DATATYPE_NOT_FOUND, /**< 30 */
	OCFM_ERR_VALUE_NOT_WITHIN_RANGE, /**< 31 */
	OCFM_ERR_MN_NODE_DOESNT_EXIST, /**< 32 */
	OCFM_ERR_CREATE_XML_WRITER_FAILED, /**< 33 */
	OCFM_ERR_XML_WRITER_START_ELT_FAILED, /**< 34 */
	OCFM_ERR_XML_WRITER_END_ELT_FAILED, /**< 35 */
	OCFM_ERR_XML_START_DOC_FAILED, /**< 36 */
	OCFM_ERR_XML_END_DOC_FAILED, /**< 37 */
	OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH, /**< 38 */
	OCFM_ERR_INVALID_PJTXML, /**< 39 */
	OCFM_ERR_PROJECT_SETTINGS, /**< 40 */
	OCFM_ERR_INVALID_DATATYPE_FOR_PDO, /**< 41 */
	OCFM_ERR_XAP_FILE_NOT_WRITTEN, /**< 42 */
	OCFM_ERR_MAX_PI_SIZE, /**< 43 */
	OCFM_ERR_UNKNOWN, /**< 44 */
	OCFM_ERR_MULTIPLEX_ASSIGN_ERROR, /**< 45 */
	OCFM_ERR_INVALID_UPPERLOWER_LIMITS, /**< 46 */
	OCFM_ERR_LOW_CNPRESTIMEOUT, /**< 47 */
	OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN, /**< 48 */
	OCFM_ERR_EXCESS_CHANNEL, /**< 49 */
	OCFM_ERR_INVALID_TXT_FOR_CDC, /**< 50 */
	OCFM_ERR_MEMORY_ALLOCATION_ERROR, /**< 51 */
	OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS, /**< 52 */
	OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND, /**< 53 */
	OCFM_ERR_INVALID_PARAMETER, /**< 54 */
	OCFM_ERR_INVALID_SIZE_MAPPED, /**< 55 */
	OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO, /**< 56 */
	OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PDO, /**< 57 */
	OCFM_ERR_INVALID_PDO_OFFSET, /**< 58 */
	COMPATIBILITY_INFO_PRE_130_PDOMAPPING, /**< 59 */
	/* NOTE: New error codes if any only append to this. Do not change the order/values. Undefined behaviour of the TCL/TK GUI*/
	OCFM_ERR_SCHEMA_VALIDATION_FAILED, /**< 60*/
	OCFM_ERR_XDD_SCHEMA_NOT_FOUND, /**< 61*/
	OCFM_ERR_XDD_SCHEMA_NOT_VALID, /**< 62*/
	OCFM_ERR_XDD_SCHEMA_PARSER_CONTEXT_ERROR, /**< 63*/
	OCFM_ERR_XDD_SCHEMA_VALIDATION_CONTEXT_ERROR, /**< 64*/
	OCFM_ERR_XML_PARSING_ERROR, /**< 65*/
	OCFM_ERR_CANNOT_CREATE_DIR, /**< 66*/
	OCFM_ERR_NO_PROJECT_LOADED, /**< 67*/	
	OCFM_ERR_CHAINED_AND_MULTIPLEXED,
	OCFM_ERR_INSUFFICIENT_MAPPING_OBJECTS,
	OCFM_ERR_CHANNEL_PAYLOAD_LIMIT_EXCEEDED,
	OCFM_ERR_CHANNEL_OBJECT_LIMIT_EXCEEDED,
	OCFM_ERR_MULTIPLEXING_NOT_SUPPORTED,
	OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PARAMETER
};

/** @addtogroup imexport @{ */
/************************************************************************
 \brief List of available process image representations.
 \author Christoph Rücker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/
DLLEXPORT enum OutputLanguage {
	C, /**< ANSI C*/
	CSHARP, /**< Microsoft C Sharp*/
	/*JAVA,*/
	XML /**< Generic XML representation (needed for ANSI C)*/
};
/** @} */

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace ObjectDictionary
		{
			namespace AccessType
			{
				/** @addtogroup xddapi @{ */
				/************************************************************************
				\brief	Accesstype for objects/subobjects of the ObjectDictionary.

						See EPSG DS 301, v1.1.0, v1.2.0
				\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
				************************************************************************/
				enum AccessType 
				{
					INVALID,
					RW,
					WO,
					RO,
					CONST,
					COND,
					RWS,/**< From v1.2.0 upwards! */
					WOS	/**< From v1.2.0 upwards! */
				};
				/** @} */
			}

			AccessType::AccessType GetAccessType(const std::string& accessType);
			::ObjectType GetObjectType(const std::string& objectType);
			
			namespace PlkDataType
			{
				/** @addtogroup xddapi @{ */
				/************************************************************************
				\brief	Powerlink data types

						See EPSG DS 301, v1.1.0, v1.2.0
				\author Christoph Rücker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
				************************************************************************/
				enum PlkDataType
				{
					BOOLEAN = 0x0001,
					INTEGER8 = 0x0002,
					INTEGER16 = 0x0003,
					INTEGER32 = 0x0004,
					UNSIGNED8 = 0x0005,
					UNSIGNED16 = 0x0006,
					UNSIGNED32 = 0x0007,
					REAL32 = 0x0008,
					VISIBLE_STRING = 0x0009,
					OCTET_STRING = 0x000A,
					UNICODE_STRING = 0x000B,
					TIME_OF_DAY = 0x000C,
					TIME_DIFF = 0x000D,
					Domain = 0x000F,
					INTEGER24 = 0x0010,
					REAL64 = 0x0011,
					INTEGER40 = 0x0012,
					INTEGER48 = 0x0013,
					INTEGER56 = 0x0014,
					INTEGER64 = 0x0015,
					UNSIGNED24 = 0x0016,
					UNSIGNED40 = 0x0018,
					UNSIGNED48 = 0x0019,
					UNSIGNED56 = 0x001A,
					UNSIGNED64 = 0x001B,
					MAC_ADDRESS = 0x0401,
					IP_ADDRESS = 0x0402,
					NETTIME = 0x0403
				};
				/** @} */
			}
		}
	}
}

#endif // ENUMS_H_
