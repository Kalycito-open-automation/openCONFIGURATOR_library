/**
 *****************************************************************************
 \file		Declarations.h

 \brief		This file contains the defines, enumerations, structure and function declarations that are exposed to the GUI developer who is interested in integrating openCONFIGURATOR into their tool
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

 *****************************************************************************/

#ifndef declarations_h
#define declarations_h

/*****************************************************************************/
/* Includes */

#include <string.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include "Exports.h"
#include "Error.h"

//using namespace std;

/*****************************************************************************/
/* Constants */

//const int OFFSET_INTEGER_8 = 0;
//const int OFFSET_UNSIGNED_INTEGER_8 = 0;

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
typedef enum
{
	DEFTYPE = 5,	/**< 5: Denotes a static data type definition */
	DEFSTRUCT = 6,	/**< 6: Defines a record type */
	VAR = 7,		/**< 7: Denotes a single value */
	ARRAY = 8,		/**< 8: A multiple data field object where each data field is a simple variable of the SAME basic data type */
	RECORD = 9		/**< 9: A multiple data field object where the data fields may be any combination of simple variables */
} ObjectType;

/** An enum PDOMapping.
 * This enum holds the list of PDO mapping type for the Object and SubObject
 */
DLLEXPORT typedef enum
{
		NO,			/**< Indicates the object must not be mapped into a Process Data Objects */
		DEFAULT,	/**< Indicates the object is part of the default mapping */
		OPTIONAL,	/**< Indicates the object can be mapped into both Receive and Transmit Process Data Objects */
		RPDO,		/**< Indicates the object shall be mapped into a Receive Process Data Objects */
		TPDO		/**< Indicates the object shall be mapped into a Transmit Process Data Objects */
} PDOMapping;

/** This enum holds the list of PDO mapping type for the Object and SubObject */
typedef PDOMapping *pdoMapping;

/** An enum NodeType.
 * This enum holds the list of available type of Nodes
 */
typedef enum
{
	MN = 0,		/**< To indicate an Managing Node */
	CN = 1		/**< To indicate an Controlled Node or Slave */
} NodeType;

/** An enum PDOType.
 * This enum holds the list of available type of Process Data Objects
 */
typedef enum
{
	PDO_TPDO = 1,	/**< 1: Indicates the Transmit Process Data Objects */
	PDO_RPDO = 2	/**< 2: Indicates the Receive Process Data Objects */
} PDOType;

/** An enum FeatureType.
 * This enum holds the types of network management features for a Node
 */
typedef enum
{
	GENERAL_FEATURES = 0,	/**< General features */
	MN_FEATURES = 1,		/**< Managing node features */
	CN_FEATURES = 2			/**< Controlled node features */
} FeatureType;

/** An enum ParameterAccess.
 * This enum holds the list of accessTypes that defines which operations valid for the parameter
 */
typedef enum
{
	constant,			/**< Const access */
	ro,					/**< Read only */
	wr,					/**< Write only */
	rw,					/**< Read and write access */
	readWriteInput,		/**< Read and write access, but represents process input data */
	readWriteOutput,	/**< Read and write, access, but represents process output data */
	noAccess			/**< access denied */
} ParameterAccess;

/** An enum IEC_Datatype.
 * This enum holds the list of available IEC datatypes
 */
typedef enum IEC_Datatype
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
} IEC_Datatype;

/** 
 * A struct to represent DataTypeList under the Application layer tag
 */
struct DataType
{
		char* dataTypeName;			/**< DataType of the interface variable or structure component */
		char* dataTypeValue;		/**< Value for the dataType */
		INT32 dataSize;				/**< DataSize for the dataType */
		IEC_Datatype iecDataType;	/**< To represent the equivalent IEC datatype */
		/**
		\brief		This function is used to get the dataType
		 
		\return		char*
		*/
		char* GetName()
		{
			return (dataTypeName);
		}
		/**
		\brief		This function is used to set the dataType	
		\param[in]	tempDataTypeName	Character pointer to the dataType to be set
		\return		void
		*/
		//TODO: Review. setName not called
		void SetName(char* tempDataTypeName)
		{
			dataTypeName = new char[strlen(tempDataTypeName) + 1];
			strcpy(dataTypeName, tempDataTypeName);
		}
		/**
		\brief		This function is used to initialise the members to a default value.
		\return		void
		*/
		void Initialize()
		{
			dataTypeName = NULL;
			dataTypeValue = NULL;
			dataSize = 0;
		}
};

/** 
 * A struct to represent the common members under the Application process tag
 */
typedef struct AppProcessCommon
{
		char* name;					/**< Name of the interface variable or structure component */
		char* uniqueId;				/**< Unique Id of the interface variable or structure component */
		char* dataType;				/**< Nested datatype of the interface variable or structure component */
		char* dataTypeUniqueIDRef;	/**< Datatype Unique Id reference of the interface variable or structure component */
		/**
		\brief		This function is used to get the uniqueId of the interface variable or structure component
		\return		char*	uniqueId of the interface variable or structure component
		*/
		char* GetUniqueID()
		{
			return (uniqueId);
		}

		/**
		\brief		This function is used to set the uniqueId of the interface variable or structure component
		\param[in]	varUniqueID		Character pointer to the uniqueId to be set
		\return		void
		*/
		void SetUniqueID(char* varUniqueID)
		{
			uniqueId = new char[strlen(varUniqueID) + 1];
			strcpy(uniqueId, varUniqueID);
		}

		/**
		\brief		This function is used to get the name of the interface variable or structure component	
		\return		char*	name of the interface variable or structure component
		*/
		char* GetName()
		{
			return (name);
		}

		/**
		\brief		This function is used to set the name of the interface variable or structure component
		\param[in]	attrName	Character pointer to the name to be set
		\return		void
		*/
		void SetName(char* attrName)
		{
			name = new char[strlen(attrName) + 1];
			strcpy(name, attrName);
		}

		/**
		\brief		This function is used to get the dataType of the interface variable or structure component
		\return		char*	dataType of the interface variable or structure component
		*/
		char* GetDataType()
		{
			return (dataType);
		}

		/**
		\brief		This function is used to set the dataType of the interface variable or structure component
		\param[in]	dtStr	Character pointer to the dataType to be set
		\return		void
		*/
		void SetDataType(char* dtStr)
		{
			dataType = new char[strlen(dtStr) + 1];
			strcpy(dataType, dtStr);
		}

		/**
		\brief		This function is used to get the uniqueIdReference of the interface variable or structure component
		\return		char*	dataTypeUniqueIdReference of the interface variable or structure component
		*/
		char* GetDtUniqueRefId()
		{
			return (dataTypeUniqueIDRef);
		}

		/**
		\brief		This function is used to set the uniqueIdReference of the interface variable or structure component
		\param[in]	uniqueRefID		Character pointer to the uniqueId Reference to be set
		\return		void	
		*/
		void SetDtUniqueRefId(char* uniqueRefID)
		{
			dataTypeUniqueIDRef = new char[strlen(uniqueRefID) + 1];
			strcpy(dataTypeUniqueIDRef, uniqueRefID);
		}

		/**
		\brief		This function is used to initialise the members to a default value.
		\return		void
		*/
		void Initialize()
		{
			name = NULL;
			uniqueId = NULL;
			dataType = NULL;
			dataTypeUniqueIDRef = NULL;
		}
} appProcessCommon;

/** 
 * A struct to represent the interface variable or structure component and their attributes
 */
struct varDeclaration
{
		AppProcessCommon* namIdDtAttr;		/**< To hold the common attributes of the interface variable or structure component */
		char size[5];						/**< To hold the size of the interface variable or structure component */
		char* initialValue; 				/**< To hold the initial value of the interface variable or structure component */
		char* structUniqueId;				/**< To hold the unique id of the struct component */

		/**
		\brief		This function is used to initialise the members to a default value.
		\return		void
		*/
		void Initialize()
		{
			namIdDtAttr = new AppProcessCommon;
			namIdDtAttr->Initialize();
			strcpy(size, "");
			initialValue = NULL;
			structUniqueId = NULL;
		}
};

/** 
 * A struct to represent the parameter list under the Application process tag
 */
typedef struct Parameter
{
		AppProcessCommon nameIdDtAttr;		/**< To hold the common attributes for a parameter */
		INT32 size;							/**< To hold the nested size of the parameter */
		char* accessStr;					/**< To hold the access for the parameter: */
		//char* dataType;
		//INT32 structIndex;
		//INT32 paraIndex;

		/**
		\brief		This function is used to initialise the members to a default value.
		\return		void
		*/
		void Initialize()
		{
			nameIdDtAttr.Initialize();
			accessStr = NULL;
			size = 0;
		}
} Parameter;

/** 
 * A struct to represent members of the Network management features
 */
typedef struct Feature
{
		char* name;					/**< Name of the feature */
		char* value;				/**< Value of the feature */
		FeatureType featureType;	/**< Type of the feature */

		/**
		\brief		This function is used to initialise the members to a default value.
		\return		void
		*/
		void Initialize()
		{
			name = NULL;
			value = NULL;
		}
} Feature;

/** 
 * A struct to represent the process data objects for the MN, which is used in generating the MN PDO index & subindex.
 */
typedef struct MNPdoVariable
{
		char* indexId;			/**< Holds the IndexId of the Index to be created */
		char* subIndexId;		/**< Holds the IndexId of the SubIndex to be created */
		INT32 offsetVal; 		/**< Holds the Offset value of the PDO */
		char* value;			/**< Holds the mapping configuration */	
		INT32 dataSize;			/**< To hold the size of the PDO data */
		PDOType pdoType;		/**< To hold the type of the PDO */

		/**
		\brief		This function is used to initialise the members to a default value.
		\return		void
		*/
		void Initialize()
		{
			indexId = NULL;
			subIndexId = NULL;
			offsetVal = 0;
			value = NULL;
			dataSize = 0;
		}
} MNPdoVariable;

/** An enum AttributeType.
 * This enum holds the list of attributes for the object or subobject can hold
 */
typedef enum
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
} AttributeType;

/** An enum Flag.
 * This enum holds the list of boolean flags
 */
typedef enum
{
	FALSE = 0,	/**< False */
	TRUE		/**< True */
} Flag;

/** An enum DateTime.
 * This enum holds the date & time attribute
 */
typedef enum
{
	DATE = 0,	/**< Date attribute */
	TIME		/**< Time attribute */
} DateTime;

/** An enum AutoGenerate.
 * This enum lists of generate modes available for the generation of the MN object dictionary.
 */
typedef enum
{
	NO_AG = 0,	/**< Specifies the option for no auto generate mode */
	YES_AG = 1	/**< Specifies the option for auto generate mode */
} AutoGenerate;

/** An enum AutoSave.
 * This enum lists of save modes available for the values & objects in the project.
 */
typedef enum
{
	YES_AS = 0,		/**< Saves the configuration automatically */
	PROMPT_AS,		/**< Prompts the user to for saving the configuration */
	DISCARD_AS		/**< Discards the configuration */
} AutoSave;

/** An enum ViewMode.
 * This enum lists of view modes available for the objects in the project.
 */
typedef enum
{
	SIMPLE = 0,	/**< Simple tree mode */
	EXPERT		/**<  Advanced tree mode */
} ViewMode;

/** An enum ProjectSettings.
 * This enum lists of project settings attributes available for the project.
 */
typedef enum
{
	AUTOGENERATE = 0,	/**< specifies the autogenerate attriubte */
	AUTOSAVE			/**< Specifies the autosave attribute */
} ProjectSettings;

/** An enum StationType.
 * This enum holds the list of available Controlled Node's station types
 */
typedef enum
{
	NORMAL = 0,		/**< Normal station */
	MULTIPLEXED,	/**< The node is multiplexed access */
	CHAINED			/**< Chained station */
} StationType;

/******************************************************************************
 * Function Declarations
 *****************************************************************************/

/**
 \brief		This API shall be usd to parse the XDC/XDD file and load the objects for given Node ID and Type parameters

 \param[in]		fileName		Char pointer to the path of the configuration file[XDD or XDC] to be imported
 \param[in]		nodeId			Integer to hold the Node ID of a node
 \param[in]		nodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode ImportXML(char *fileName, INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used for parsing the XDC/XDD file and re-loading the objects for the given Node ID and Type. This API will not delete and create the node. Only the objects for the node are recreated

 \param[in]		fileName		Char pointer to the path of the configuration file [XDD or XDC] to be re-imported for a Node
 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode ReImportXML(char* fileName, INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall write the process image variables to the XAP files [.h and .xml] in the location specified
 Note: This API is recommended to be called after the GenerateCDC API.

 \param[in]		xapFilePath		Char pointer to the path to which the generated xap contents will be written

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GenerateXAP(char* xapFilePath);

/**
 \brief		This API shall be used to generate the CDC for the project. This API writes the compiled CDC into the mnobd.txt file. The generated mnobd.txt is converted into binary [mnobd.cdc] file using txt2cdc [elf].

 \param[in]		cdcPath			Char pointer to the path to which the generated CDC contents will be written into.

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GenerateCDC(char* cdcPath);

/**
 \brief		This API shall be used to generate the process image variables in a C# .NET usable format
 Note: This API is recommended to be called after the GenerateCDC API.

 \param[in]		netFilePath		Char pointer to the path to which the generated NET contents will be written

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GenerateNET(char* netFilePath);

/**
 \brief		This API shall be used to create a node [MN/CN] with the given node ID, node type and node name.

 \param[in]		nodeId			Node Id for the node to be created
 \param[in]		nodeType		Node type of the node - MN or CN
 \param[in]		nodeName		Name of the node to be created

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode CreateNode(INT32 nodeId, NodeType nodeType, char* nodeName);

/**
 \brief		This API shall be used to delete a node with the given respective node ID and node type. Deleting a node will also delete the object dictionary for the corresponding node.

 \param[in]		nodeId			Node Id of the node to be deleted
 \param[in]		nodeType		Node type of the node - MN or CN

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode DeleteNode(INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used to delete the object dictionary of a node. This API will not delete the node

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode DeleteNodeObjDict(INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used to delete an Index of a node given the respective IndexID, the node ID and node type

 \param[in]		nodeId			Node Id of a node
 \param[in]		nodeType		Node type of the node
 \param[in]		indexId			The ID of the Index to be deleted

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode DeleteIndex(INT32 nodeId, NodeType nodeType, char* indexId);

/**
 \brief		This API shall be used to delete a SubIndex in an Index of a node given the respective SubIndexID, IndexID, the node ID and node type

 \param[in]		nodeId			Node Id of a node
 \param[in]		nodeType		Node type of the node
 \param[in]		indexId			The ID of the Index which holds the SubIndex
 \param[in]		subIndexID		The ID of the SubIndex to be deleted

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode DeleteSubIndex(INT32 nodeId, NodeType nodeType, char* indexId, char* subIndexID);

/**
 \brief		This API shall be used to add an Index to a node

 \param[in]		nodeId			Integer to hold the Node ID of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		indexId			Character pointer to the Index ID of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode AddIndex(INT32 nodeId, NodeType nodeType, char* indexId);

/**
 \brief		This API shall be used to add a particular SubIndex to an Index of a node given the respective SubIndexID, IndexID, the node ID and node type

 \param[in]		nodeId			Node Id of a node
 \param[in]		nodeType		Node type of the node
 \param[in]		indexId			The ID of the Index which holds the SubIndex
 \param[in]		subIndexId		The ID of the SubIndex to be added

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode AddSubIndex(INT32 nodeId, NodeType nodeType, char* indexId, char* subIndexId);

/**
 \brief		This API shall be used to set the attributes [IndexValue, IndexName & flagIfInCdc] of an index for a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		indexId				Character pointer to the IndexID
 \param[in]		indexValue			Character Pointer to the IndexValue
 \param[in]		indexName			Character Pointer to the IndexName
 \param[in]		includeInCDC		Enum of type Flag to indicate if this index is included in CDC generation

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode SetBasicIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* indexValue, char* indexName, Flag includeInCDC);

/**
 \brief		This API shall be used to set the attributes [IndexValue, IndexName & flagIfInCdc] of a sub index for a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		indexId				Character pointer to the IndexID
 \param[in]		sidxId				Character pointer to the SubIndexID
 \param[in]		indexValue			Character Pointer to the IndexValue
 \param[in]		indexName			Character Pointer to the IndexName
 \param[in]		includeInCDC		Enum of type Flag to indicate if this Sub Index is included in CDC generation

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode SetBasicSubIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId, char* indexValue, char* indexName, Flag includeInCDC);

/**
 \brief		This API shall be used to check for the presence of a Node of given node ID and type in the collection list. If the node is present, the nodePos and nodeExist parameters are updated to reflect this in the calling function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		nodePos			Interger pointer to the Node position
 \param[in]		nodeExist		Boolean to indicate presence of the Node

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode IfNodeExists(INT32 nodeId, NodeType nodeType, INT32 *nodePos, bool& nodeExist);

/**
 \brief		This API shall be used to check for the presence of Index of given node ID and type in the collection list. If the Index is present, the indexId and idxPos parameters are updated to reflect this in the calling function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		indexId			Character pointer to the IndexID
 \param[out]	idxPos			Integer pointer to the IndexPos

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode IfIndexExists(INT32 nodeId, NodeType nodeType, char* indexId, INT32 *idxPos);

/**
 \brief		This API shall be used to check for the presence of SubIndex in Index of given node ID and type in the collection list. If the SubIndex is present, the sidxPos and idxPos parameters are updated to reflect this in the calling function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		idxId			Character pointer to the IndexID
 \param[in]		sidxId			Character pointer to the SubIndexID
 \param[out]	sidxPos			Integer pointer to the SubIndexPos
 \param[out]	idxPos			Integer pointer to the IndexPos

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode IfSubIndexExists(INT32 nodeId, NodeType nodeType, char* idxId, char* sidxId, INT32* sidxPos, INT32* idxPos);

/**
 \brief		This API shall be used to get the attributes of an Index

 \param[in]		nodeId					Integer variable to hold the Node Id of a node
 \param[in]		nodeType				Enum to hold the Node type of the node
 \param[in]		indexId					Character pointer to the IndexID
 \param[in]		attributeType			Enum to hold the Attribute type of the index
 \param[out]	outAttributeValue		Character pointer to the AttributeValue

 \return		ocfmRetCode				ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, AttributeType attributeType, char* outAttributeValue);

/**
 \brief		This API shall be used to get the attributes of a SubIndex

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		indexId				Character pointer to the IndexID
 \param[in]		sidxId				Character pointer to the SubIndexID
 \param[in]		attributeType		Enum to hold the Attribute type of the index .
 \param[out]	outAttributeValue	Character pointer to the AttributeValue

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetSubIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId, AttributeType attributeType, char* outAttributeValue);

/**
 \brief		This API shall be used to get the number of controlled nodes under a MN

 \param[in]		nodeId			Integer to hold the node id
 \param[out]	outNodeCount	Integer pointer to NodeCount

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetNodeCount(INT32 nodeId, INT32* outNodeCount);

/**
 \brief		This API shall be used to get the number of Indexes under a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[out]	outIndexCount		Integer pointer to the IndexCount

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetIndexCount(INT32 nodeId, NodeType nodeType, INT32* outIndexCount);

/**
 \brief		This API shall be used to get the number of Sub Indexes under an Index

 \param[in]		nodeId				Integer to hold the Node ID of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		indexId				Character pointer to the Index ID
 \param[out]	outSubIndexCount	Integer pointer to the IndexCount

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetSubIndexCount(INT32 nodeId, NodeType nodeType, char* indexId, INT32* outSubIndexCount);

/**
 \brief		This API shall be used to get the node attributes [NodeID, NodeName, Stationtype, Forcedcycle_value, Forcedcycle_flag] of a node given the position of the node under a MN
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function

 \param[in]			nodePos				Integer to hold the position of the node
 \param[out]		outNodeId			Integer pointer to the NodeID
 \param[out]		outNodeName			Character pointer to the NodeName
 \param[out]		outStationType		Character pointer to the station type
 \param[out]		outForcedCycle		Character pointer to the forced cycle value
 \param[out]		outIsForcedCycle	boolean pointer to the forced cycle flag

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetNodeAttributesbyNodePos(INT32 nodePos, INT32* outNodeId, char* outNodeName, StationType* outStationType, char* outForcedCycle, bool* outIsForcedCycle);

/**
 \brief		This API shall be used to get the Index ID of an Index, given the position of the Index under a node and the Node ID
 Note: The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		indexPos		Integer to hold the index position
 \param[out]	outIndexId		Character pointer to the IndexID

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetIndexIDbyIndexPos(INT32 nodeId, NodeType nodeType, INT32 indexPos, char* outIndexId);

/**
 \brief		This API shall be used to get the Sub Index ID of a Sub Index
 Note: The IfSubIndexExists() API should be called and iSubIndexPos is passed as a parameter to this function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		indexId			Character pointer to the IndexID
 \param[in]		subIndexPos		Integer variable to hold the position of SubIndex
 \param[out]	outSubIndexID	Character pointer to the SubIndexID

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 nodeId, NodeType nodeType, char* indexId, INT32 subIndexPos, char* outSubIndexID);

/**
 \brief		This API shall be used to get the Index ID of an Index given the position of the Index under a node and the position of the node
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param[in]		nodePos			Integer to hold the position of the node
 \param[in]		indexPos		Integer to hold the position of the Index
 \param[out]	outIndexID		Character pointer to the IndexID

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetIndexIDbyPositions(INT32 nodePos, INT32 indexPos, char* outIndexID);

/**
 \brief		This API shall be used to get the Sub Index ID of a Sub Index
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function
 The IfSubIndexExists() API should be called and iSubIndexPos is passed as a parameter to this function

 \param[in]		nodePos			Integer to hold the position of the node
 \param[in]		indexPos		Integer to hold the position of the Index
 \param[in]		subIndexPos		Integer to hold the SubIndex position
 \param[out]	outSubIndexID	Character pointer to the SubIndexID

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetSubIndexIDbyPositions(INT32 nodePos, INT32 indexPos, INT32 subIndexPos, char* outSubIndexID);

/**
 \brief		This API shall be used to get the attributes of an Index of a node given the Node and Index position
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param[in]		nodePos				Integer to hold the position of the node
 \param[in]		indexPos			Integer to hold the position of the Index
 \param[in]		attributeType		Enum to hold the Attribute type of the index .
 \param[out]	outAttributeValue	Character pointer to the AttributeValue

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos, AttributeType attributeType, char* outAttributeValue);

/**
 \brief		This API shall be used to get the attributes of an Sub Index of a Node given the Node, Index, Sub Index position
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function
 The IfSubIndexExists() API should be called and iSubIndexPos is passed as a parameter to this function

 \param[in]		nodePos				Integer to hold the position of the node
 \param[in]		indexPos			Integer to hold the position of the Index
 \param[in]		subIndexPos			Integer to hold the position of the SubIndex
 \param[in]		attributeType		Enum to hold the Attribute type of the index
 \param[out]	outAttributeValue	Character pointer to the AttributeValue

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetSubIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos, INT32 subIndexPos, AttributeType attributeType, char* outAttributeValue);

/**
 \brief		This API shall be used to load the predefined object dictionary xml and loads the objects that the tool will refer to when adding/editing an Index and/or a SubIndex

 \param[in]		xmlFilePath		Char pointer to the path of the od.xml file

 \return	void
 */
DLLEXPORT void LoadObjectDictionary(char* xmlFilePath);

/**
 \brief		This API shall be used to save all the objects [Index/SubIndex] under the node into a xml file along with all the attributes of the objects

 \param[in]		fileName		Char pointer to the path of the file [XDD or XDC]
 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode SaveNode(const char* fileName, INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used to save the project

 \param[in]		projectPath		Character pointer to the Project Path
 \param[in]		projectName		Character pointer to the Project name

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode SaveProject(char* projectPath, char* projectName);

/**
 \brief		This API shall be used to open a previously saved openCONFIGURATOR project

 \param[in]		projectPath			Character pointer to the project path
 \param[in]		projectFileName		Character pointer to the file name of the project xml

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode OpenProject(char* projectPath, char* projectFileName);

/**
 \brief		This API shall be used to free the memory occupied by the shared library objects for the current project & settings. After calling this API, the project configurations & setting are no longer valid

 \return	ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode FreeProjectMemory();

/**
 \brief		This API shall be used to get the Project settings information [auto save, auto generate] for the active project

 \param[out]	autoGenStatus				Enum to hold the Auto generation option of the project.
 \param[out]	autoSaveStatus				Enum to hold the Auto save option of the project.
 \param[out]	viewMode					Enum to hold the view mode option of the project.
 \param[out]	isExpertViewAlreadySet		Boolean pointer to hold the view type option of the project.

 \return		ocfmRetCode					ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetProjectSettings(AutoGenerate *autoGenStatus, AutoSave *autoSaveStatus, ViewMode *viewMode, bool* isExpertViewAlreadySet);

/**
 \brief		This API shall be used to set the Project settings information [auto save, auto generate] for the active project

 \param[in]		autoGenStatus				Enum to hold the Auto generation option of the project.
 \param[in]		autoSaveStatus				Enum to hold the Auto save option of the project.
 \param[in]		viewMode					Enum to hold the view mode option of the project.
 \param[in]		isExpertViewAlreadySet		Boolean to hold the view type option of the project.

 \return		ocfmRetCode					ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode SetProjectSettings(AutoGenerate autoGenStatus, AutoSave autoSaveStatus, ViewMode viewMode, bool isExpertViewAlreadySet);

/**
 \brief		This API shall auto generate the MN's object dictionary

 \return	ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GenerateMNOBD();

/**
 \brief		This API shall be used to set all the attributes of an Index for a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		indexId				Character pointer to the IndexID
 \param[in]		actualValue			Character pointer to the ActualValue
 \param[in]		indexName			Character pointer to the IndexName
 \param[in]		accessType			Character pointer to the Access type
 \param[in]		dataTypeName		Character pointer to the Datatype
 \param[in]		pdoMappingVal		Character pointer to the PDO Mapping
 \param[in]		defaultValue		Character pointer to the Default value
 \param[in]		highLimitVal		Character pointer to the Highlimit
 \param[in]		lowLimitVal			Character pointer to the Lowlimit
 \param[in]		objectType			Character pointer to the Object type
 \param[in]		includeInCDC		Enum of type Flag to indicate if this Index is included in CDC generation

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode SetAllIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* actualValue, char* indexName, char* accessType, char* dataTypeName, char* pdoMappingVal, char* defaultValue, char* highLimitVal, char* lowLimitVal, char* objectType, Flag includeInCDC);

/**
 \brief		This API shall be used to set all the attributes of a Sub Index for a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		indexId				Character pointer to the IndexID
 \param[in]		sidxId				Character pointer to the SubIndexID
 \param[in]		actualValue			Character pointer to the ActualValue
 \param[in]		indexName			Character pointer to the IndexName
 \param[in]		accessType			Character pointer to the Access type
 \param[in]		dataTypeName		Character pointer to the Datatype
 \param[in]		pdoMappingVal		Character pointer to the PDO Mapping
 \param[in]		defaultValue		Character pointer to the Default value
 \param[in]		highLimitVal		Character pointer to the Highlimit
 \param[in]		lowLimitVal			Character pointer to the Lowlimit
 \param[in]		objectType			Character pointer to the Object type
 \param[in]		includeInCDC		Enum of type Flag to indicate if this Sub Index is included in CDC generation

 \return		ocfmRetCode			ConfigurationErrors
 */
DLLEXPORT ocfmRetCode SetAllSubIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId, char* actualValue, char* indexName, char* accessType, char* dataTypeName, char* pdoMappingVal, char* defaultValue, char* highLimitVal, char* lowLimitVal, char* objectType, Flag includeInCDC);

/**
 \brief		This API shall be used to get the specific network management feature value for a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		featureType			Enum to hold the Feature type of the node
 \param[in]		featureName			Character pointer to the Feature name
 \param[out]	outFeatureValue		Character pointer to get the feature value

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetFeatureValue(INT32 nodeId, NodeType nodeType, FeatureType featureType, char* featureName, char* outFeatureValue);

/**
 \brief		This API shall be used to update all the attributes of a node including the Node ID

 \param[in]		currentNodeId			Integer to hold the current node id of a node
 \param[in]		newNodeID				Integer to hold the new node id to be set to the node
 \param[in]		nodeType				Enum to hold the node type of the node
 \param[in]		nodeName				Character pointer to the Node Name
 \param[in]		stationType				Enum to hold the station type of the node
 \param[in]		forcedCycleVal			Character pointer to the ForcedCycle value
 \param[in]		isForcedCycle			Boolean to hold the Forced Cycle Flag of the node
 \param[in]		presTimeoutVal			Character pointer to the Poll Response Timeout value for the node

 \return		ocfmRetCode				ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode UpdateNodeParams(INT32 currentNodeId, INT32 newNodeID, NodeType nodeType, char* nodeName, StationType stationType, char* forcedCycleVal, bool isForcedCycle, char* presTimeoutVal);

/**
 \brief		This API shall be used to get all the datatypes available for a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[out]	outDataTypes		Character pointer to all the datatypes available for the specifed node

 \return		ocfmRetCode			ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode GetNodeDataTypes(INT32 nodeId, NodeType nodeType, char* outDataTypes);

/**
 \brief		This API shall be used to create a node [MN/CN] and to set the Index/SubIndexes from the XDD specified to the specified node ID, node type and node name

 \param[in]		nodeId			Node Id to be created
 \param[in]		nodeType		Node type of the node - MN or CN
 \param[in]		nodeName		Name of the node to be created
 \param[in]		importXmlFile	Absolute path of the xml(xdd/xdc) file to be imported for the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
DLLEXPORT ocfmRetCode NewProjectNode(INT32 nodeId, NodeType nodeType, char* nodeName, char* importXmlFile);

/**
 \brief		This function shall be used to return the data size of the given standard datatype

 \param[in]		dataTypeVal     Character pointer to the datatype string

 \return		INT32			Returns the data size for the given datatype
 */
DLLEXPORT INT32 GetDataSize(char* dataTypeVal);

#ifndef __GNUC__
#pragma warning( disable: 4251 )
#endif
#endif // declarations_h
