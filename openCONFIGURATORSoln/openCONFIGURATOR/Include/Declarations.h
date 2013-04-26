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

#define MN_NODEID 240
#define BROADCAST_NODEID 0
//TODO: to be removed.Const's are not used.
const int OFFSET_INTEGER_8 = 0;
const int OFFSET_UNSIGNED_INTEGER_8 = 0;

/*****************************************************************************/
/* Defines */

#define OFFSET_FIRST_INDEX_IN_MN_OBD	4

#define OFFSET_NUM_ENTRIES_MN 0
#define OFFSET_NUM_ENTRIES_CN 7

#define OFFSET_INDEX	0
#define OFFSET_SUBINDEX 2
#define OFFSET_SIZE		3
#define OFFSET_DATA		7
#define	INDEX_SIZE		4
#define	SUBINDEX_SIZE	2
#define NODE_ID			3
#define ERR_STRING_LEN	100
#define OFFSET_FIRST_INDEX_IN_1F22 11
#define MULTIPL_CYCLE_ASSIGN_OBJECT "1F9B"
#define MNCN_POLLRESPONSE_TIMEOUT_OBJECT "1F92"
#define PI_VAR_COUNT 4000
#define PRES_DEFAULT_PAYLOAD 36
#define PREQ_DEFAULT_PAYLOAD 36
#define MAX_CN_CROSS_TRAFFIC_STN 3

/*****************************************************************************/
/* Typedefs */

typedef unsigned char UINT8;
typedef unsigned short UINT16;
typedef unsigned int UINT32;
typedef signed int INT32;
typedef long int LONG;
typedef unsigned long int ULONG;

/******************************************************************************
 Enumerations
 *****************************************************************************/

typedef enum
{
	DEFTYPE = 5, DEFSTRUCT = 6, VAR = 7, ARRAY = 8, RECORD = 9
} ObjectType;

DLLEXPORT typedef enum
{
		NO, DEFAULT, OPTIONAL, RPDO, TPDO
} PDOMapping;

typedef PDOMapping *pdoMapping;

typedef enum
{
	MN = 0, CN = 1
} NodeType;

typedef enum
{
	PDO_TPDO = 1, PDO_RPDO = 2
} PDOType;

typedef enum
{
	GENERAL_FEATURES = 0, MN_FEATURES = 1, CN_FEATURES = 2
} FeatureType;

typedef enum
{
	constant, ro, /* read access only (default value)*/
	wr, /* write access only */
	rw, /* both read and write access*/
	readWriteInput, /*both read and write access, but represents process input data*/
	readWriteOutput, /*both read and write, access, but represents process output data*/
	noAccess /*access denied				*/
} ParameterAccess;

typedef enum IEC_Datatype
{
	BITSTRING = 0,
	BOOL,
	BYTE,
	_CHAR,
	DWORD,
	LWORD,
	SINT,
	INT,
	DINT,
	LINT,
	USINT,
	UINT,
	UDINT,
	ULINT,
	REAL,
	LREAL,
	STRING,
	WSTRING
} IEC_Datatype;

struct DataType
{
		char* dataTypeName;
		char* dataTypeValue;
		INT32 dataSize;
		IEC_Datatype iecDataType;
		char* GetName()
		{
			return (dataTypeName);
		}
		//TODO: Review. setName not called
		void SetName(char* tempDataTypeName)
		{
			dataTypeName = new char[strlen(tempDataTypeName) + 1];
			strcpy(dataTypeName, tempDataTypeName);
		}
		void Initialize()
		{
			dataTypeName = NULL;
			dataTypeValue = NULL;
			dataSize = 0;
		}
};

typedef struct AppProcessCommon
{
		char* name;
		char* uniqueId;
		char* dataType;
		char* dataTypeUniqueIDRef;

		char* GetUniqueID()
		{
			return (uniqueId);
		}

		void SetUniqueID(char* varUniqueID)
		{
			uniqueId = new char[strlen(varUniqueID) + 1];
			strcpy(uniqueId, varUniqueID);
		}

		char* GetName()
		{
			return (name);
		}

		void SetName(char* attrName)
		{
			name = new char[strlen(attrName) + 1];
			strcpy(name, attrName);
		}

		char* GetDataType()
		{
			return (dataType);
		}

		void SetDataType(char* dtStr)
		{
			dataType = new char[strlen(dtStr) + 1];
			strcpy(dataType, dtStr);
		}

		char* GetDtUniqueRefId()
		{
			return (dataTypeUniqueIDRef);
		}

		void SetDtUniqueRefId(char* uniqueRefID)
		{
			dataTypeUniqueIDRef = new char[strlen(uniqueRefID) + 1];
			strcpy(dataTypeUniqueIDRef, uniqueRefID);
		}
		void Initialize()
		{
			name = NULL;
			uniqueId = NULL;
			dataType = NULL;
			dataTypeUniqueIDRef = NULL;
		}
} appProcessCommon;

struct varDeclaration
{
		AppProcessCommon* namIdDtAttr;
		char size[5];
		char* initialValue; //TODO: unused
		char* structUniqueId;
		void Initialize()
		{
			namIdDtAttr = new AppProcessCommon;
			namIdDtAttr->Initialize();
			strcpy(size, "");
			initialValue = NULL;
			structUniqueId = NULL;
		}
};

typedef struct Parameter
{
		AppProcessCommon nameIdDtAttr;
		char* dataType;	//TODO: unused
		INT32 structIndex; //TODO: unused
		char* accessStr;
		INT32 paraIndex;
		void Initialize()
		{
			nameIdDtAttr.Initialize();
			dataType = NULL;
			accessStr = NULL;
		}
} Parameter;

typedef struct Feature
{
		char* name;
		char* value;
		FeatureType featureType;
		void Initialize()
		{
			name = NULL;
			value = NULL;
		}
} Feature;

typedef struct MNPdoVariable
{
		char* indexId;
		char* subIndexId;
		INT32 offsetVal; //TODO: unused
		char* value;
		INT32 dataSize;
		PDOType pdoType;
		void Initialize()
		{
			indexId = NULL;
			subIndexId = NULL;
			offsetVal = 0;
			value = NULL;
			dataSize = 0;
		}
} MNPdoVariable;

typedef enum
{
	NAME = 0,	// Name of the Attribute
	OBJECTTYPE,		// Object Type of the Attribute
	DATATYPE,		// Data Type of the Attribute
	ACCESSTYPE,		// Access Type of the Attribute
	DEFAULTVALUE,	// Default value of the Attribute
	ACTUALVALUE,	// Actual value of the Attribute
	PDOMAPPING,		// PDO Mapping of the Attribute
	LOWLIMIT,
	HIGHLIMIT,
	FLAGIFINCDC,
} AttributeType;

typedef enum
{
	FALSE = 0, TRUE
} Flag;

typedef enum
{
	DATE = 0, TIME
} DateTime;

typedef enum
{
	NO_AG = 0, YES_AG = 1
} AutoGenerate;

typedef enum
{
	YES_AS = 0, PROMPT_AS, DISCARD_AS
} AutoSave;
typedef enum
{
	SIMPLE = 0, EXPERT

} ViewMode;

typedef enum
{
	AUTOGENERATE = 0, AUTOSAVE
} ProjectSettings;
typedef enum
{
	NORMAL = 0, MULTIPLEXED, CHAINED
} StationType;
/******************************************************************************
 * Function Declarations
 *****************************************************************************/
/*****************************************************************************/
/**
 \brief		This API shall be usd to parse the XDC/XDD file and load the objects for given Node ID and Type parameters

 \param		fileName		Char pointer to the path of the configuration file[XDD or XDC] to be imported
 \param		nodeId			Integer to hold the Node ID of a node
 \param		nodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode ImportXML(
		char *fileName, INT32 nodeId, NodeType nodeType);
/*****************************************************************************/
/**
 \brief		This API shall be used for parsing the XDC/XDD file and re-loading the objects for the given Node ID and Type. This API will not delete and create the node. Only the objects for the node are recreated

 \param		fileName		Char pointer to the path of the configuration file [XDD or XDC] to be re-imported for a Node
 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode ReImportXML(
		char* fileName, INT32 nodeId, NodeType nodeType);
/*****************************************************************************************/
/**
 \brief		This API shall write the process image variables to the XAP files [.h and .xml] in the location specified
 Note: This API is recommended to be called after the GenerateCDC API.

 \param		xapFilePath		Char pointer to the path to which the generated xap contents will be written

 \return	ocfmRetCode		ConfiguratorErrors
 */
/******************************************************************************************/DLLEXPORT ocfmRetCode GenerateXAP(
		char* xapFilePath);
/*****************************************************************************************/
/**
 \brief		This API shall be used to generate the CDC for the project. This API writes the compiled CDC into the mnobd.txt file. The generated mnobd.txt is converted into binary [mnobd.cdc] file using txt2cdc [elf].

 \param		cdcPath			Char pointer to the path to which the generated CDC contents will be written into.

 \return	ocfmRetCode		ConfiguratorErrors
 */
/******************************************************************************************/DLLEXPORT ocfmRetCode GenerateCDC(
		char* cdcPath);
/*****************************************************************************************/
/**
 \brief		This API shall be used to generate the process image variables in a C# .NET usable format
 Note: This API is recommended to be called after the GenerateCDC API.

 \param		netFilePath		Char pointer to the path to which the generated NET contents will be written

 \return	ocfmRetCode		ConfiguratorErrors
 */
/******************************************************************************************/DLLEXPORT ocfmRetCode GenerateNET(
		char* netFilePath);
/*****************************************************************************/
/**
 \brief		This API shall be used to create a node [MN/CN] with the given node ID, node type and node name.

 \param		nodeId			Node Id for the node to be created
 \param		nodeType		Node type of the node - MN or CN
 \param		nodeName		Name of the node to be created

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode CreateNode(
		INT32 nodeId, NodeType nodeType, char* nodeName);
/*****************************************************************************/
/**
 \brief		This API shall be used to delete a node with the given respective node ID and node type. Deleting a node will also delete the object dictionary for the corresponding node.

 \param		nodeId			Node Id of the node to be deleted
 \param		nodeType		Node type of the node - MN or CN

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode DeleteNode(
		INT32 nodeId, NodeType nodeType);
/*****************************************************************************/
/**
 \brief		This API shall be used to delete the object dictionary of a node. This API will not delete the node

 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode DeleteNodeObjDict(
		INT32 nodeId, NodeType nodeType);
/*****************************************************************************/
/**
 \brief		This API shall be used to delete an Index of a node given the respective IndexID, the node ID and node type

 \param		nodeId			Node Id of a node
 \param		nodeType		Node type of the node
 \param		indexId			The ID of the Index to be deleted

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode DeleteIndex(
		INT32 nodeId, NodeType nodeType, char* indexId);
/*****************************************************************************/
/**
 \brief		This API shall be used to delete a SubIndex in an Index of a node given the respective SubIndexID, IndexID, the node ID and node type

 \param		nodeId			Node Id of a node
 \param		nodeType		Node type of the node
 \param		indexId			The ID of the Index which holds the SubIndex
 \param		subIndexID		The ID of the SubIndex to be deleted

 \return	ocfmRetCode
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode DeleteSubIndex(
		INT32 nodeId, NodeType nodeType, char* indexId, char* subIndexID);
/*****************************************************************************/
/**
 \brief		This API shall be used to add an Index to a node

 \param		nodeId			Integer to hold the Node ID of a node
 \param		nodeType		Enum to hold the Node type of the node
 \param		indexId			Character pointer to the Index ID of the node

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode AddIndex(
		INT32 nodeId, NodeType nodeType, char* indexId);
/*****************************************************************************/
/**
 \brief		This API shall be used to add a particular SubIndex to an Index of a node given the respective SubIndexID, IndexID, the node ID and node type

 \param		nodeId			Node Id of a node
 \param		nodeType		Node type of the node
 \param		indexId			The ID of the Index which holds the SubIndex
 \param		subIndexId		The ID of the SubIndex to be added

 \return	ocfmRetCode
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode AddSubIndex(
		INT32 nodeId, NodeType nodeType, char* indexId, char* subIndexId);
/*****************************************************************************/
/**
 \brief		This API shall be used to set the attributes [IndexValue, IndexName & flagIfInCdc] of an index for a node

 \param		nodeId				Integer to hold the Node Id of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		indexId				Character pointer to the IndexID
 \param		indexValue			Character Pointer to the IndexValue
 \param		indexName			Character Pointer to the IndexName
 \param		includeInCDC		Enum of type Flag to indicate if this index is included in CDC generation

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode SetBasicIndexAttributes(
		INT32 nodeId, NodeType nodeType, char* indexId, char* indexValue,
		char* indexName, Flag includeInCDC);
/*****************************************************************************/
/**
 \brief		This API shall be used to set the attributes [IndexValue, IndexName & flagIfInCdc] of a sub index for a node

 \param		nodeId				Integer to hold the Node Id of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		indexId				Character pointer to the IndexID
 \param		sidxId				Character pointer to the SubIndexID
 \param		indexValue			Character Pointer to the IndexValue
 \param		indexName			Character Pointer to the IndexName
 \param		includeInCDC		Enum of type Flag to indicate if this Sub Index is included in CDC generation

 \return	ocfmRetCode				ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode SetBasicSubIndexAttributes(
		INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId,
		char* indexValue, char* indexName, Flag includeInCDC);
/*****************************************************************************/
/**
 \brief		This API shall be used to check for the presence of a Node of given node ID and type in the collection list. If the node is present, the nodePos and nodeExist parameters are updated to reflect this in the calling function

 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node
 \param		nodePos			Interger pointer to the Node position
 \param		nodeExist		Boolean to indicate presence of the Node

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode IfNodeExists(
		INT32 nodeId, NodeType nodeType, INT32 *nodePos, bool& nodeExist);
/*****************************************************************************/
/**
 \brief		This API shall be used to check for the presence of Index of given node ID and type in the collection list. If the Index is present, the indexId and idxPos parameters are updated to reflect this in the calling function

 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node
 \param		indexId			Character pointer to the IndexID
 \param		idxPos			Integer pointer to the IndexPos

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode IfIndexExists(
		INT32 nodeId, NodeType nodeType, char* indexId, INT32 *idxPos);
/*****************************************************************************/
/**
 \brief		This API shall be used to check for the presence of SubIndex in Index of given node ID and type in the collection list. If the SubIndex is present, the sidxPos and idxPos parameters are updated to reflect this in the calling function

 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node
 \param		idxId			Character pointer to the IndexID
 \param		sidxId			Character pointer to the SubIndexID
 \param		sidxPos			Integer pointer to the SubIndexPos
 \param		idxPos			Integer pointer to the IndexPos

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode IfSubIndexExists(
		INT32 nodeId, NodeType nodeType, char* idxId, char* sidxId,
		INT32* sidxPos, INT32* idxPos);
/*****************************************************************************************/
/**
 \brief		This API shall be used to get the attributes of an Index

 \param		nodeId					Integer variable to hold the Node Id of a node
 \param		nodeType				Enum to hold the Node type of the node
 \param		indexId					Character pointer to the IndexID
 \param		attributeType			Enum to hold the Attribute type of the index
 \param		outAttributeValue		Character pointer to the AttributeValue

 \return	ocfmRetCode				ConfiguratorErrors
 */
/******************************************************************************************/DLLEXPORT ocfmRetCode GetIndexAttributes(
		INT32 nodeId, NodeType nodeType, char* indexId,
		AttributeType attributeType, char* outAttributeValue);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the attributes of a SubIndex

 \param		nodeId				Integer to hold the Node Id of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		indexId				Character pointer to the IndexID
 \param		sidxId				Character pointer to the SubIndexID
 \param		attributeType		Enum to hold the Attribute type of the index .
 \param		outAttributeValue	Character pointer to the AttributeValue

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetSubIndexAttributes(
		INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId,
		AttributeType attributeType, char* outAttributeValue);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the number of controlled nodes under a MN

 \param		nodeId			Integer to hold the node id
 \param		outNodeCount	Integer pointer to NodeCount

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetNodeCount(
		INT32 nodeId, INT32* outNodeCount);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the number of Indexes under a node

 \param		nodeId				Integer to hold the Node Id of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		outIndexCount		Integer pointer to the IndexCount

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetIndexCount(
		INT32 nodeId, NodeType nodeType, INT32* outIndexCount);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the number of Sub Indexes under an Index

 \param		nodeId				Integer to hold the Node ID of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		indexId				Character pointer to the Index ID
 \param		outSubIndexCount	Integer pointer to the IndexCount

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetSubIndexCount(
		INT32 nodeId, NodeType nodeType, char* indexId,
		INT32* outSubIndexCount);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the node attributes [NodeID, NodeName, Stationtype, Forcedcycle_value, Forcedcycle_flag] of a node given the position of the node under a MN
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function

 \param		nodePos				Integer to hold the position of the node
 \param		outNodeId			Integer pointer to the NodeID
 \param		outNodeName			Character pointer to the NodeName
 \param		outStationType		Character pointer to the station type
 \param		outForcedCycle		Character pointer to the forced cycle value
 \param		outIsForcedCycle	boolean pointer to the forced cycle flag

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetNodeAttributesbyNodePos(
		INT32 nodePos, INT32* outNodeId, char* outNodeName,
		StationType* outStationType, char* outForcedCycle,
		bool* outIsForcedCycle);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the Index ID of an Index, given the position of the Index under a node and the Node ID
 Note: The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node
 \param		indexPos		Integer to hold the index position
 \param		outIndexId		Character pointer to the IndexID

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetIndexIDbyIndexPos(
		INT32 nodeId, NodeType nodeType, INT32 indexPos, char* outIndexId);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the Sub Index ID of a Sub Index
 Note: The IfSubIndexExists() API should be called and iSubIndexPos is passed as a parameter to this function

 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node
 \param		indexId			Character pointer to the IndexID
 \param		subIndexPos		Integer variable to hold the position of SubIndex
 \param		outSubIndexID	Character pointer to the SubIndexID

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetSubIndexIDbySubIndexPos(
		INT32 nodeId, NodeType nodeType, char* indexId, INT32 subIndexPos,
		char* outSubIndexID);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the Index ID of an Index given the position of the Index under a node and the position of the node
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param		nodePos			Integer to hold the position of the node
 \param		indexPos		Integer to hold the position of the Index
 \param		outIndexID		Character pointer to the IndexID

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetIndexIDbyPositions(
		INT32 nodePos, INT32 indexPos, char* outIndexID);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the Sub Index ID of a Sub Index
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function
 The IfSubIndexExists() API should be called and iSubIndexPos is passed as a parameter to this function

 \param		nodePos			Integer to hold the position of the node
 \param		indexPos		Integer to hold the position of the Index
 \param		subIndexPos		Integer to hold the SubIndex position
 \param		outSubIndexID	Character pointer to the SubIndexID

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetSubIndexIDbyPositions(
		INT32 nodePos, INT32 indexPos, INT32 subIndexPos, char* outSubIndexID);
/*****************************************************************************************/
/**
 \brief		This API shall be used to get the attributes of an Index of a node given the Node and Index position
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param		nodePos				Integer to hold the position of the node
 \param		indexPos			Integer to hold the position of the Index
 \param		attributeType		Enum to hold the Attribute type of the index .
 \param		outAttributeValue	Character pointer to the AttributeValue

 \return	ocfmRetCode			ConfiguratorErrors
 */
/******************************************************************************************/DLLEXPORT ocfmRetCode GetIndexAttributesbyPositions(
		INT32 nodePos, INT32 indexPos, AttributeType attributeType,
		char* outAttributeValue);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the attributes of an Sub Index of a Node given the Node, Index, Sub Index position
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function
 The IfSubIndexExists() API should be called and iSubIndexPos is passed as a parameter to this function

 \param		nodePos				Integer to hold the position of the node
 \param		indexPos			Integer to hold the position of the Index
 \param		subIndexPos			Integer to hold the position of the SubIndex
 \param		attributeType		Enum to hold the Attribute type of the index
 \param		outAttributeValue	Character pointer to the AttributeValue

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetSubIndexAttributesbyPositions(
		INT32 nodePos, INT32 indexPos, INT32 subIndexPos,
		AttributeType attributeType, char* outAttributeValue);
/*****************************************************************************/
/**
 \brief		This API shall be used to load the predefined object dictionary xml and loads the objects that the tool will refer to when adding/editing an Index and/or a SubIndex

 \param		xmlFilePath		Char pointer to the path of the od.xml file

 \return	void
 */
/*****************************************************************************/DLLEXPORT void LoadObjectDictionary(
		char* xmlFilePath);
/*****************************************************************************/
/**
 \brief		This API shall be used to save all the objects [Index/SubIndex] under the node into a xml file along with all the attributes of the objects

 \param		fileName		Char pointer to the path of the file [XDD or XDC]
 \param		nodeId			Integer to hold the Node Id of a node
 \param		nodeType		Enum to hold the Node type of the node

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode SaveNode(
		const char* fileName, INT32 nodeId, NodeType nodeType);
/*****************************************************************************/
/**
 \brief		This API shall be used to save the project

 \param		projectPath		Character pointer to the Project Path
 \param		projectName		Character pointer to the Project name

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode SaveProject(
		char* projectPath, char* projectName);
/*****************************************************************************/
/**
 \brief		This API shall be used to open a previously saved openCONFIGURATOR project

 \param		projectPath			Character pointer to the project path
 \param		projectFileName		Character pointer to the file name of the project xml

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode OpenProject(
		char* projectPath, char* projectFileName);
/*****************************************************************************/
/**
 \brief		This API shall be used to free the memory occupied by the shared library objects for the current project & settings. After calling this API, the project configurations & setting are no longer valid

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode FreeProjectMemory();
/*****************************************************************************/
/**
 \brief		This API shall be used to get the Project settings information [auto save, auto generate] for the active project

 \param		autoGenStatus				Enum to hold the Auto generation option of the project.
 \param		autoSaveStatus				Enum to hold the Auto save option of the project.
 \param		viewMode					Enum to hold the view mode option of the project.
 \param		isExpertViewAlreadySet		Boolean pointer to hold the view type option of the project.

 \return	ocfmRetCode					ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetProjectSettings(
		AutoGenerate *autoGenStatus, AutoSave *autoSaveStatus,
		ViewMode *viewMode, bool* isExpertViewAlreadySet);
/*****************************************************************************/
/**
 \brief		This API shall be used to set the Project settings information [auto save, auto generate] for the active project

 \param		autoGenStatus				Enum to hold the Auto generation option of the project.
 \param		autoSaveStatus				Enum to hold the Auto save option of the project.
 \param		viewMode					Enum to hold the view mode option of the project.
 \param		isExpertViewAlreadySet		Boolean to hold the view type option of the project.

 \return	ocfmRetCode					ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode SetProjectSettings(
		AutoGenerate autoGenStatus, AutoSave autoSaveStatus, ViewMode viewMode,
		bool isExpertViewAlreadySet);
/*****************************************************************************/
/**
 \brief		This API shall auto generate the MN's object dictionary

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GenerateMNOBD();
/*****************************************************************************/
/**
 \brief		This API shall be used to set all the attributes of an Index for a node

 \param		nodeId				Integer to hold the Node Id of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		indexId				Character pointer to the IndexID
 \param		actualValue			Character pointer to the ActualValue
 \param		indexName			Character pointer to the IndexName
 \param		accessType			Character pointer to the Access type
 \param		dataTypeName		Character pointer to the Datatype
 \param		pdoMappingVal		Character pointer to the PDO Mapping
 \param		defaultValue		Character pointer to the Default value
 \param		highLimitVal		Character pointer to the Highlimit
 \param		lowLimitVal			Character pointer to the Lowlimit
 \param		objectType			Character pointer to the Object type
 \param		includeInCDC		Enum of type Flag to indicate if this Index is included in CDC generation

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode SetAllIndexAttributes(
		INT32 nodeId, NodeType nodeType, char* indexId, char* actualValue,
		char* indexName, char* accessType, char* dataTypeName,
		char* pdoMappingVal, char* defaultValue, char* highLimitVal,
		char* lowLimitVal, char* objectType, Flag includeInCDC);
/*****************************************************************************/
/**
 \brief		This API shall be used to set all the attributes of a Sub Index for a node

 \param		nodeId				Integer to hold the Node Id of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		indexId				Character pointer to the IndexID
 \param		sidxId				Character pointer to the SubIndexID
 \param		actualValue			Character pointer to the ActualValue
 \param		indexName			Character pointer to the IndexName
 \param		accessType			Character pointer to the Access type
 \param		dataTypeName		Character pointer to the Datatype
 \param		pdoMappingVal		Character pointer to the PDO Mapping
 \param		defaultValue		Character pointer to the Default value
 \param		highLimitVal		Character pointer to the Highlimit
 \param		lowLimitVal			Character pointer to the Lowlimit
 \param		objectType			Character pointer to the Object type
 \param		includeInCDC		Enum of type Flag to indicate if this Sub Index is included in CDC generation

 \return	ocfmRetCode			ConfigurationErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode SetAllSubIndexAttributes(
		INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId,
		char* actualValue, char* indexName, char* accessType,
		char* dataTypeName, char* pdoMappingVal, char* defaultValue,
		char* highLimitVal, char* lowLimitVal, char* objectType,
		Flag includeInCDC);
/*****************************************************************************/
/**
 \brief		This API shall be used to get the specific network management feature value for a node

 \param		nodeId				Integer to hold the Node Id of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		featureType			Enum to hold the Feature type of the node
 \param		featureName			Character pointer to the Feature name
 \param		outFeatureValue		Character pointer to get the feature value

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetFeatureValue(
		INT32 nodeId, NodeType nodeType, FeatureType featureType,
		char* featureName, char* outFeatureValue);
/*****************************************************************************/
/**
 \brief		This API shall be used to update all the attributes of a node including the Node ID

 \param		currentNodeId			Integer to hold the current node id of a node
 \param		newNodeID				Integer to hold the new node id to be set to the node
 \param		nodeType				Enum to hold the node type of the node
 \param		nodeName				Character pointer to the Node Name
 \param		stationType				Enum to hold the station type of the node
 \param		forcedCycleVal			Character pointer to the ForcedCycle value
 \param		isForcedCycle			Boolean to hold the Forced Cycle Flag of the node
 \param		presTimeoutVal			Character pointer to the Poll Response Timeout value for the node

 \return	ocfmRetCode				ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode UpdateNodeParams(
		INT32 currentNodeId, INT32 newNodeID, NodeType nodeType, char* nodeName,
		StationType stationType, char* forcedCycleVal, bool isForcedCycle,
		char* presTimeoutVal);
/*****************************************************************************/
/**
 \brief		This API shall be used to get all the datatypes available for a node

 \param		nodeId				Integer to hold the Node ID of a node
 \param		nodeType			Enum to hold the Node type of the node
 \param		outDataTypes		Character pointer to all the datatypes available for the specifed node

 \return	ocfmRetCode			ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode GetNodeDataTypes(
		INT32 nodeId, NodeType nodeType, char* outDataTypes);
/*****************************************************************************/
/**
 \brief		This API shall be used to create a node [MN/CN] and to set the Index/SubIndexes from the XDD specified to the specified node ID, node type and node name

 \param		nodeId			Node Id to be created
 \param		nodeType		Node type of the node - MN or CN
 \param		nodeName		Name of the node to be created
 \param		importXmlFile	Absolute path of the xml(xdd/xdc) file to be imported for the node

 \return	ocfmRetCode		ConfiguratorErrors
 */
/*****************************************************************************/DLLEXPORT ocfmRetCode NewProjectNode(
		INT32 nodeId, NodeType nodeType, char* nodeName, char* importXmlFile);
/*****************************************************************************/
/**
 \brief		This function shall be used to return the data size of the given datatype

 \param		dataTypeVal     Character pointer to the datatype string

 \return	INT32
 */
/*****************************************************************************/DLLEXPORT INT32 GetDataSize(
		char* dataTypeVal);

#ifndef __GNUC__
#pragma warning( disable: 4251 )
#endif
#endif // declarations_h
