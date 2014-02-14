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
#include "Exception.h"
#include "Enums.h"
#include "UtilityClasses.h"

class ProjectConfiguration;

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
ocfmRetCode ImportXML(const char *fileName, INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used for parsing the XDC/XDD file and re-loading the objects for the given Node ID and Type. This API will not delete and create the node. Only the objects for the node are recreated

 \param[in]		fileName		Char pointer to the path of the configuration file [XDD or XDC] to be re-imported for a Node
 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode ReImportXML(const char* fileName, INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall write the process image variables to the XAP files [.h and .xml] in the location specified
 Note: This API is recommended to be called after the GenerateCDC API.

 \param[in]		xapFilePath		Char pointer to the path to which the generated xap contents will be written

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode GenerateXAP(const char* xapFilePath);

/**
 \brief		This API shall be used to generate the CDC for the project. This API writes the compiled CDC into the mnobd.txt file. The generated mnobd.txt is converted into binary [mnobd.cdc] file using txt2cdc [elf].

 \param[in]		cdcPath			Char pointer to the path to which the generated CDC contents will be written into.

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode GenerateCDC(const char* cdcPath, const ProjectConfiguration& projectConfiguration);

/**
 \brief		This API shall be used to generate the process image variables in a C# .NET usable format
 Note: This API is recommended to be called after the GenerateCDC API.

 \param[in]		netFilePath		Char pointer to the path to which the generated NET contents will be written

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode GenerateNET(const char* netFilePath);

/**
 \brief		This API shall be used to create a node [MN/CN] with the given node ID, node type and node name.

 \param[in]		nodeId			Node Id for the node to be created
 \param[in]		nodeType		Node type of the node - MN or CN
 \param[in]		nodeName		Name of the node to be created

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode CreateNode(INT32 nodeId, NodeType nodeType, const char* nodeName);

/**
 \brief		This API shall be used to delete a node with the given respective node ID and node type. Deleting a node will also delete the object dictionary for the corresponding node.

 \param[in]		nodeId			Node Id of the node to be deleted
 \param[in]		nodeType		Node type of the node - MN or CN

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode DeleteNode(INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used to delete the object dictionary of a node. This API will not delete the node

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode DeleteNodeObjDict(INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used to delete an Index of a node given the respective IndexID, the node ID and node type

 \param[in]		nodeId			Node Id of a node
 \param[in]		nodeType		Node type of the node
 \param[in]		indexId			The ID of the Index to be deleted

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode DeleteIndex(INT32 nodeId, NodeType nodeType, const char* indexId);

/**
 \brief		This API shall be used to delete a SubIndex in an Index of a node given the respective SubIndexID, IndexID, the node ID and node type

 \param[in]		nodeId			Node Id of a node
 \param[in]		nodeType		Node type of the node
 \param[in]		indexId			The ID of the Index which holds the SubIndex
 \param[in]		subIndexID		The ID of the SubIndex to be deleted

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode DeleteSubIndex(INT32 nodeId, NodeType nodeType, const char* indexId, const char* subIndexID);

/**
 \brief		This API shall be used to add an Index to a node

 \param[in]		nodeId			Integer to hold the Node ID of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		indexId			Character pointer to the Index ID of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode AddIndex(INT32 nodeId, NodeType nodeType, const char* indexId);

/**
 \brief		This API shall be used to add a particular SubIndex to an Index of a node given the respective SubIndexID, IndexID, the node ID and node type

 \param[in]		nodeId			Node Id of a node
 \param[in]		nodeType		Node type of the node
 \param[in]		indexId			The ID of the Index which holds the SubIndex
 \param[in]		subIndexId		The ID of the SubIndex to be added

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode AddSubIndex(INT32 nodeId, NodeType nodeType, const char* indexId, const char* subIndexId);

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
ocfmRetCode SetBasicIndexAttributes(INT32 nodeId, NodeType nodeType, const char* indexId,const char* indexValue, const char* indexName, bool includeInCDC);

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
ocfmRetCode SetBasicSubIndexAttributes(INT32 nodeId, NodeType nodeType, const char* indexId, const char* sidxId, const char* indexValue, const char* indexName, bool includeInCDC);

/**
 \brief		This API shall be used to check for the presence of a Node of given node ID and type in the collection list. If the node is present, the nodePos and nodeExist parameters are updated to reflect this in the calling function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		nodePos			Interger pointer to the Node position
 \param[in]		nodeExist		Boolean to indicate presence of the Node

 \return		ocfmRetCode		ConfiguratorErrors
 */
 ocfmRetCode IfNodeExists(INT32 nodeId, NodeType nodeType, UINT32 *nodePos, bool& nodeExist);

/**
 \brief		This API shall be used to check for the presence of Index of given node ID and type in the collection list. If the Index is present, the indexId and idxPos parameters are updated to reflect this in the calling function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		indexId			Character pointer to the IndexID
 \param[out]	idxPos			Integer pointer to the IndexPos

 \return		ocfmRetCode		ConfiguratorErrors
 \deprecated	Use IndexCollection::ContainsIndex(const UINT32 index) of the respective Node to query for Index-existence.
 */
ocfmRetCode IfIndexExists(INT32 nodeId, NodeType nodeType, const char* indexId, INT32 *idxPos);

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
ocfmRetCode IfSubIndexExists(INT32 nodeId, NodeType nodeType, const char* idxId, const char* sidxId, INT32* sidxPos, INT32* idxPos);

/**
 \brief		This API shall be used to get the attributes of an Index

 \param[in]		nodeId					Integer variable to hold the Node Id of a node
 \param[in]		nodeType				Enum to hold the Node type of the node
 \param[in]		indexId					Character pointer to the IndexID
 \param[in]		attributeType			Enum to hold the Attribute type of the index
 \param[out]	outAttributeValue		Character pointer to the AttributeValue

 \return		ocfmRetCode				ConfiguratorErrors
 */
ocfmRetCode GetIndexAttributes(INT32 nodeId, NodeType nodeType, const char* indexId, AttributeType attributeType, char* outAttributeValue);

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
ocfmRetCode GetSubIndexAttributes(INT32 nodeId, NodeType nodeType, const char* indexId, const char* sidxId, AttributeType attributeType, char* outAttributeValue);

/**
 \brief		This API shall be used to get the number of controlled nodes under a MN

 \param[in]		nodeId			Integer to hold the node id
 \param[out]	outNodeCount	Integer pointer to NodeCount

 \return		ocfmRetCode		ConfiguratorErrors
 \deprecated	Use NodeCollection::GetNumberOfNodes() instead.
 */
 ocfmRetCode GetNodeCount(UINT32* outNodeCount);

/**
 \brief		This API shall be used to get the number of Indexes under a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[out]	outIndexCount		Integer pointer to the IndexCount

 \return		ocfmRetCode			ConfiguratorErrors
 */
 ocfmRetCode GetIndexCount(UINT32 nodeId, NodeType nodeType, UINT32* outIndexCount);

/**
 \brief		This API shall be used to get the number of Sub Indexes under an Index

 \param[in]		nodeId				Integer to hold the Node ID of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[in]		indexId				Character pointer to the Index ID
 \param[out]	outSubIndexCount	Integer pointer to the IndexCount

 \return		ocfmRetCode			ConfiguratorErrors
 */
ocfmRetCode GetSubIndexCount(INT32 nodeId, NodeType nodeType, const char* indexId, UINT32* outSubIndexCount);

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
 ocfmRetCode GetNodeAttributesbyNodePos(UINT32 nodePos, INT32* outNodeId, char* outNodeName, StationType* outStationType, char* outForcedCycle, bool* outIsForcedCycle);

/**
 \brief		This API shall be used to get the Index ID of an Index, given the position of the Index under a node and the Node ID
 Note: The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node
 \param[in]		indexPos		Integer to hold the index position
 \param[out]	outIndexId		Character pointer to the IndexID

 \return		ocfmRetCode		ConfiguratorErrors
 */
 ocfmRetCode GetIndexIDbyIndexPos(INT32 nodeId, NodeType nodeType, INT32 indexPos, char* outIndexId);

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
ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 nodeId, NodeType nodeType, const char* indexId, INT32 subIndexPos, char* outSubIndexID);

/**
 \brief		This API shall be used to get the Index ID of an Index given the position of the Index under a node and the position of the node
 Note: The IfNodeExists() API should be called and iNodePos is passed as a parameter to this function
 The IfIndexExists() API should be called and iIndexPos is passed as a parameter to this function

 \param[in]		nodePos			Integer to hold the position of the node
 \param[in]		indexPos		Integer to hold the position of the Index
 \param[out]	outIndexID		Character pointer to the IndexID

 \return		ocfmRetCode		ConfiguratorErrors
 */
 ocfmRetCode GetIndexIDbyPositions(INT32 nodePos, INT32 indexPos, char* outIndexID);

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
 ocfmRetCode GetSubIndexIDbyPositions(INT32 nodePos, INT32 indexPos, INT32 subIndexPos, char* outSubIndexID);

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
 ocfmRetCode GetIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos, AttributeType attributeType, char* outAttributeValue);

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
 ocfmRetCode GetSubIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos, INT32 subIndexPos, AttributeType attributeType, char* outAttributeValue);

/**
 \brief		This API shall be used to load the predefined object dictionary xml and loads the objects that the tool will refer to when adding/editing an Index and/or a SubIndex

 \param[in]		xmlFilePath		Char pointer to the path of the od.xml file

 \return	void
 */
void LoadObjectDictionary(const char* xmlFilePath);

/**
 \brief		This API shall be used to save all the objects [Index/SubIndex] under the node into a xml file along with all the attributes of the objects

 \param[in]		fileName		Char pointer to the path of the file [XDD or XDC]
 \param[in]		nodeId			Integer to hold the Node Id of a node
 \param[in]		nodeType		Enum to hold the Node type of the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
 ocfmRetCode SaveNode(const char* fileName, INT32 nodeId, NodeType nodeType);

/**
 \brief		This API shall be used to save the project

 \param[in]		projectPath		Character pointer to the Project Path
 \param[in]		projectName		Character pointer to the Project name

 \return		ocfmRetCode		ConfiguratorErrors
 */
ocfmRetCode SaveProject(const char* projectPath, const char* projectName);

/**
 \brief		This API shall be used to open a previously saved openCONFIGURATOR project

 \param[in]		projectPath			Character pointer to the project path
 \param[in]		projectFileName		Character pointer to the file name of the project xml

 \return		ocfmRetCode			ConfiguratorErrors
 */
ocfmRetCode OpenProject(const char* projectPath, const char* projectFileName);

/**
 \brief		This API shall be used to free the memory occupied by the shared library objects for the current project & settings. After calling this API, the project configurations & setting are no longer valid

 \return	ocfmRetCode		ConfiguratorErrors
 */
 ocfmRetCode FreeProjectMemory();

/**
 \brief		This API shall be used to get the Project settings information [auto save, auto generate] for the active project

 \param[out]	autoGenStatus				Enum to hold the Auto generation option of the project.
 \param[out]	autoSaveStatus				Enum to hold the Auto save option of the project.
 \param[out]	viewMode					Enum to hold the view mode option of the project.
 \param[out]	isExpertViewAlreadySet		Boolean pointer to hold the view type option of the project.

 \return		ocfmRetCode					ConfiguratorErrors
 */
//ocfmRetCode GetProjectSettings(AutoGenerate *autoGenStatus, AutoSave *autoSaveStatus, ViewMode *viewMode, bool* isExpertViewAlreadySet);

/**
 \brief		This API shall be used to set the Project settings information [auto save, auto generate] for the active project

 \param[in]		autoGenStatus				Enum to hold the Auto generation option of the project.
 \param[in]		autoSaveStatus				Enum to hold the Auto save option of the project.
 \param[in]		viewMode					Enum to hold the view mode option of the project.
 \param[in]		isExpertViewAlreadySet		Boolean to hold the view type option of the project.

 \return		ocfmRetCode					ConfiguratorErrors
 */
 //ocfmRetCode SetProjectSettings(AutoGenerate autoGenStatus, AutoSave autoSaveStatus, ViewMode viewMode, bool isExpertViewAlreadySet);

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
 ocfmRetCode SetAllIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* actualValue, char* indexName, char* accessType, char* dataTypeName, char* pdoMappingVal, char* defaultValue, char* highLimitVal, char* lowLimitVal, char* objectType, bool includeInCDC);

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
 ocfmRetCode SetAllSubIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId, char* actualValue, char* indexName, char* accessType, char* dataTypeName, char* pdoMappingVal, char* defaultValue, char* highLimitVal, char* lowLimitVal, char* objectType, bool includeInCDC);

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
 ocfmRetCode UpdateNodeParams(INT32 currentNodeId, INT32 newNodeID, NodeType nodeType, char* nodeName, StationType stationType, char* forcedCycleVal, bool isForcedCycle, char* presTimeoutVal);

/**
 \brief		This API shall be used to get all the datatypes available for a node

 \param[in]		nodeId				Integer to hold the Node Id of a node
 \param[in]		nodeType			Enum to hold the Node type of the node
 \param[out]	outDataTypes		Character pointer to all the datatypes available for the specifed node

 \return		ocfmRetCode			ConfiguratorErrors
 */
 ocfmRetCode GetNodeDataTypes(INT32 nodeId, NodeType nodeType, char* outDataTypes);

/**
 \brief		This API shall be used to create a node [MN/CN] and to set the Index/SubIndexes from the XDD specified to the specified node ID, node type and node name

 \param[in]		nodeId			Node Id to be created
 \param[in]		nodeType		Node type of the node - MN or CN
 \param[in]		nodeName		Name of the node to be created
 \param[in]		importXmlFile	Absolute path of the xml(xdd/xdc) file to be imported for the node

 \return		ocfmRetCode		ConfiguratorErrors
 */
 ocfmRetCode NewProjectNode(INT32 nodeId, NodeType nodeType, const char* nodeName, const char* importXmlFile);

/**
 \brief		This function shall be used to return the data size of the given standard datatype

 \param[in]		dataTypeVal     Character pointer to the datatype string

 \return		INT32			Returns the data size for the given datatype
 */
UINT32 GetDataSize(const string dataTypeVal);

/**
 \brief		Library function for validating imported XDDs/XDCs against the current schema

 \param		fileName     File to be validated

 \return	ocfmRetCode	 Error code
*/
ocfmRetCode ValidateXDDFile(const char *fileName);

#ifndef __GNUC__
#pragma warning( disable: 4251 )
#endif
#endif // declarations_h
