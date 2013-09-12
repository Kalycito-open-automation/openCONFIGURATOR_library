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

/** An enum ObjectType.
 * ObjectType is used to denote what kind of object is at that particular index within the Object Dictionary
 */
enum ObjectType
{
	DEFTYPE = 5,	/**< 5: Denotes a static data type definition */
	DEFSTRUCT = 6,	/**< 6: Defines a record type */
	VAR = 7,		/**< 7: Denotes a single value */
	ARRAY = 8,		/**< 8: A multiple data field object where each data field is a simple variable of the SAME basic data type */
	RECORD = 9		/**< 9: A multiple data field object where the data fields may be any combination of simple variables */
};

/** An enum PDOMapping.
 * This enum holds the list of PDO mapping type for the Object and SubObject
 */
DLLEXPORT enum PDOMapping
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

/** An enum AttributeType.
 * This enum holds the list of attributes for the object or subobject can hold
 */
enum AttributeType
{
	NAME = 0,		/**< Name attribute */
	OBJECTTYPE,		/**< Object Type attribute */
	DATATYPE,		/**< Data Type attribute */
	ACCESSTYPE,		/**< Access Type attribute */
	DEFAULTVALUE,	/**< Default value attribute */
	ACTUALVALUE,	/**< Actual value attribute */
	PDOMAPPING,		/**< PDO Mapping attribute */
	LOWLIMIT,		/**< Low limit attribute */
	HIGHLIMIT,		/**< High limit attribute */
	FLAGIFINCDC,	/**< Include in CDC attribute */
};

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
enum AutoGenerate
{
	NO_AG = 0,	/**< Specifies the option for no auto generate mode */
	YES_AG = 1	/**< Specifies the option for auto generate mode */
};

/** An enum AutoSave.
 * This enum lists of save modes available for the values & objects in the project.
 */
enum AutoSave
{
	YES_AS = 0,		/**< Saves the configuration automatically */
	PROMPT_AS,		/**< Prompts the user to for saving the configuration */
	DISCARD_AS		/**< Discards the configuration */
};

/** An enum ViewMode.
 * This enum lists of view modes available for the objects in the project.
 */
enum ViewMode
{
	SIMPLE = 0,	/**< Simple tree mode */
	EXPERT		/**<  Advanced tree mode */
};

/** An enum ProjectSettings.
 * This enum lists of project settings attributes available for the project.
 */
enum ProjectSettings
{
	AUTOGENERATE = 0,	/**< specifies the autogenerate attriubte */
	AUTOSAVE			/**< Specifies the autosave attribute */
};

/** An enum StationType.
 * This enum holds the list of available Controlled Node's station types
 */
enum StationType
{
	NORMAL = 0,		/**< Normal station */
	MULTIPLEXED,	/**< The node is multiplexed access */
	CHAINED			/**< Chained station */
};

#endif // ENUMS_H_
