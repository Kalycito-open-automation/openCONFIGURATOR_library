/**
 ******************************************************************************
 \file		APIOperations.cpp

 \brief		This file contains the functions that are exposed as an API for handling Project settings, Node and Object operations
 ******************************************************************************
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

/****************************************************************************/
/* Includes */

#include <cassert>
#include <iostream>
#include <fstream>
#include <sstream>
#include <stdio.h>
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <sys/stat.h> 

#if defined(_WIN32) && defined(_MSC_VER)
#include <direct.h>
#include <time.h>
#else
#include <sys/stat.h>
#endif
#include "../Include/Exception.h"
#include "../Include/Internal.h"
#include "../Include/ObjectDictionary.h"
#include "../Include/ProcessImage.h"
#include "../Include/Declarations.h"
#include "../Include/Validation.h"
#include "../Include/APIOperations.h"
#include "../Include/NodeCollection.h"
#include "../Include/Index.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/LibraryConfiguration.h"
#include "../Include/Logging.h"
#include "../Include/Result.h"
#include "../Include/BoostShared.h"

using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::Library::Utilities;

/****************************************************************************/
/* Defines */

#define ENCODING "UTF-8"
#define CDC_BUFFER 5000
#define CDC_MN_BUFFER 200000
#define MAX_FILE_PATH_SIZE 500

/****************************************************************************/
/* Global Variables */

INT32 lastVarIndexGlobal = -1;
static bool cdtCompletedGlobal = false;
static bool objectDictLoadedGlobal = false;
static BuildTime buildTimeGlobal;
static Offsets size8INOffset;
static Offsets size16INOffset;
static Offsets size32INOffset;
static Offsets size64INOffset;
static Offsets size8OUTOffset;
static Offsets size16OUTOffset;
static Offsets size32OUTOffset;
static Offsets size64OUTOffset;

/** Represents the maximum payload for each channel. */ 
char abC_DLL_ISOCHR_MAX_PAYL[5] = "1490";
INT32 configDateGlobal; //global used in xdcoperations
INT32 configTimeGlobal; //global used in xdcoperations
UINT32 cycleNumberGlobal; //global used in Utility

/*****************************************************************************/
/* Function Declarations */

/****************************************************************************/
/**
 \brief		Default attributes to an Index are set from the object dictionary

 \param		indexId				Character pointer to hold the IndexID
 \param		indexObj			Class pointer of Index to add the attributes
 \param		dictIndexObj		Class pointer of Index form object dictionary

 \return	void
 */
/****************************************************************************/
static void SetDefaultIndexAttributes(char* indexId, Index* indexObj,
		Index* dictIndexObj);

/*****************************************************************************/
/**
 \brief		Default attributes to a SubIndex are set from the object dictionary

 \param		subIndexId		Character pointer to hold the IndexID
 \param		sidxObj			Class pointer of SubIndex to add the attributes
 \param		dictSidxObj		Class pointer of SubIndex from object dictionary

 \return	void
 */
/*****************************************************************************/
static void SetDefaultSubIndexAttributes(char* subIndexId, SubIndex* sidxObj,
		SubIndex* dictSidxObj);



//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

static void SetDefaultIndexAttributes(const char* indexId, Index* indexObj,
		Index* dictIndexObj)
{
	if ((NULL == dictIndexObj) || (NULL == indexObj))
	{
		LOG_FATAL() << "Parameters 'dictIndexObj' and 'indexObj' must not be NULL.";
		return;
	}

	if (NULL == indexId)
	{
		LOG_FATAL() << "Parameter 'indexId' must not be NULL.";
		return;
	}
	else
	{
		indexObj->SetIndexValue(indexId);
	}

	//The Value for all the attributes is retrieved from objDictCollection[ObjDict.xml]
	if (NULL != dictIndexObj->GetName())
	{
		indexObj->SetName((char*) dictIndexObj->GetName());
	}
	else
	{
		indexObj->SetName((char*) "");
	}

	if (NULL != dictIndexObj->GetObjectType())
	{
		indexObj->SetObjectType((char*) dictIndexObj->GetObjectType());
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		indexObj->SetObjectType((char*) "");
	}

	if (NULL != dictIndexObj->GetLowLimit())
	{
		indexObj->SetLowLimit((char*) dictIndexObj->GetLowLimit());
	}
	else
	{
		indexObj->SetLowLimit((char*) "");
	}

	if (NULL != dictIndexObj->GetHighLimit())
	{
		indexObj->SetHighLimit((char*) dictIndexObj->GetHighLimit());
	}
	else
	{
		indexObj->SetHighLimit((char*) "");
	}

	if (NULL != dictIndexObj->GetAccessType())
	{
		indexObj->SetAccessType((char*) dictIndexObj->GetAccessType());
	}
	else
	{
		indexObj->SetAccessType((char*) "");
	}

	if (NULL != dictIndexObj->GetPDOMapping())
	{
		char* tempStr = NULL;
		tempStr = new char[strlen(dictIndexObj->GetPDOMapping())
				+ STR_ALLOC_BUFFER];
		strcpy(tempStr, dictIndexObj->GetPDOMapping());
		indexObj->SetPDOMapping(tempStr);
		delete[] tempStr;
	}
	else
	{
		indexObj->SetPDOMapping((char*) "NOT_DEFINED");
	}

	if (NULL != dictIndexObj->GetDefaultValue())
	{
		indexObj->SetDefaultValue((char*) dictIndexObj->GetDefaultValue());
	}
	else
	{
		indexObj->SetDefaultValue((char*) "");
	}

	if (NULL != dictIndexObj->GetActualValue())
	{
		indexObj->SetActualValue((char*) dictIndexObj->GetActualValue());
	}
	else
	{
		indexObj->SetActualValue((char*) "");
	}

	if (NULL != dictIndexObj->GetDataType().GetName())
	{
		indexObj->SetDataTypeST(dictIndexObj->GetDataType());
	}
	else
	{
		indexObj->SetDataType((char*) "");
	}
}

static void SetDefaultSubIndexAttributes(const char* subIndexId, SubIndex* sidxObj,
		SubIndex* dictSidxObj)
{
	if ((NULL == dictSidxObj) || (NULL == sidxObj))
	{
		LOG_FATAL() << "Parameters 'dictSidxObj' and 'sidxObj'  must not be NULL.";
		return;
	}

	if (NULL == subIndexId)
	{
		LOG_FATAL() << "Parameter 'subIndexId' must not be NULL.";
		return;
	}
	else
	{
		// Setting the Index Value for the SUBindex--
		sidxObj->SetIndexValue(subIndexId);
	}

	//The Value for all the attributes is retrieved from objDictCollection[ObjDict.xml]
	if (NULL != dictSidxObj->GetName())
	{
		sidxObj->SetName((char*) dictSidxObj->GetName());
	}
	else
	{
		sidxObj->SetName((char*) "");
	}

	if (NULL != dictSidxObj->GetObjectType())
	{
		sidxObj->SetObjectType((char*) (dictSidxObj->GetObjectType()));
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		sidxObj->SetObjectType((char*) "");
	}

	if (NULL != dictSidxObj->GetLowLimit())
	{
		sidxObj->SetLowLimit((char*) dictSidxObj->GetLowLimit());
	}
	else
	{
		sidxObj->SetLowLimit((char*) "");
	}

	if (NULL != dictSidxObj->GetHighLimit())
	{
		sidxObj->SetHighLimit((char*) dictSidxObj->GetHighLimit());
	}
	else
	{
		sidxObj->SetHighLimit((char*) "");
	}

	if (NULL != dictSidxObj->GetAccessType())
	{
		sidxObj->SetAccessType((char*) dictSidxObj->GetAccessType());
	}
	else
	{
		sidxObj->SetAccessType((char*) "");
	}

	if (NULL != dictSidxObj->GetPDOMapping())
	{
		char* tempStr = NULL;
		tempStr = new char[strlen(dictSidxObj->GetPDOMapping())
				+ STR_ALLOC_BUFFER];
		strcpy(tempStr, dictSidxObj->GetPDOMapping());
		sidxObj->SetPDOMapping(tempStr);
		delete[] tempStr;
	}
	else
	{
		sidxObj->SetPDOMapping((char*) "NOT_DEFINED");
	}

	if (NULL != dictSidxObj->GetDefaultValue())
	{
		sidxObj->SetDefaultValue((char*) dictSidxObj->GetDefaultValue());
	}
	else
	{
		sidxObj->SetDefaultValue((char*) "");
	}

	if (NULL != dictSidxObj->GetActualValue())
	{
		sidxObj->SetActualValue((char*) dictSidxObj->GetActualValue());
	}
	else
	{
		sidxObj->SetActualValue((char*) "");
	}

	if (NULL != dictSidxObj->GetDataType().GetName())
	{
		sidxObj->SetDataTypeST(dictSidxObj->GetDataType());
	}
	else
	{
		sidxObj->SetDataType((char*) "");
	}
	return;
}

ocfmRetCode CreateNode(INT32 nodeId, NodeType nodeType, const char* nodeName)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		LOG_INFO() << "Creating new node with nodeId " << nodeId << " and name '" << nodeName << "'.";
		if (MN == nodeType)
		{
			if (!objectDictLoadedGlobal)
			{
				LoadObjectDictionary(LibraryConfiguration::GetObjectDictinaryFilePath().c_str());
				objectDictLoadedGlobal = true;
			}
			cycleNumberGlobal = 0;
		}
		else if (CN == nodeType)
		{
			UINT32 nodePos = 0;
			bool nodeExistFlag = false;
			errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos,
					nodeExistFlag);
			if ((errCodeObj.getErrorCode() == OCFM_ERR_SUCCESS)
					&& (nodeExistFlag == true))
			{
				boost::format formatter(kMsgExistingNode);
				formatter % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_NODE_ALREADY_EXISTS);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}
			else if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				LOG_WARN() << "IfNodeExists() failed with error code " << errCodeObj.getErrorCode() << ".";
			}
		}
		Node nodeObj;
		nodeObj.SetNodeId(nodeId);
		nodeObj.SetNodeType(nodeType);
		if (!nodeName)
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'nodeName'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		else
		{
			nodeObj.SetNodeName(nodeName);
		}

		NodeCollection* nodeCollObj = NULL;
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		nodeCollObj->AddNode(nodeObj);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}

	errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
	return errCodeObj;
}

// FIXME: The path to the XML-File to import is represented as char*, 1 byte wide.
// This will impose problems for paths that contain non-ASCII characters!
ocfmRetCode NewProjectNode(INT32 nodeId, NodeType nodeType, const char* nodeName,
		const char* importXmlFile)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		//Creates the node with Nodeid & nodeName
		if (!nodeName)
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'nodeName'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		errCodeObj = CreateNode(nodeId, nodeType, nodeName);
		if (OCFM_ERR_SUCCESS != errCodeObj.getErrorCode())
		{
			LOG_FATAL() << errCodeObj.getErrorString();
			throw errCodeObj;
		}
		//Import the xdd/xdc for the Node created
		if (!importXmlFile)
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'importXmlFile'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		errCodeObj = ValidateXDDFile(importXmlFile);
		if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << errCodeObj.getErrorString();
			throw errCodeObj;
		}
		errCodeObj = ImportXML(importXmlFile, nodeId, nodeType);
		if (OCFM_ERR_SUCCESS != errCodeObj.getErrorCode())
		{
			LOG_FATAL() << errCodeObj.getErrorString();
			throw errCodeObj;
		}

		errCodeObj.setErrorCode(OCFM_ERR_UNKNOWN);
		if (MN == nodeType)
		{
			//set the loss of SoC tolerance to 50 ms
			INT32 idxPos = 0;
			errCodeObj = IfIndexExists(nodeId, nodeType, (char *) "1C14",
					&idxPos);
			if (OCFM_ERR_SUCCESS == errCodeObj.getErrorCode())
			{
				Index* idxObj = NULL;

				idxObj = GetMNIndexValues((char*) "1C14");
				if ((NULL != idxObj))
				{
					if (NULL != idxObj->GetActualValue())
					{
						idxObj->SetActualValue((char*) "50000000");
					}
				}
			}
			else
			{
				//TODO: SOC index 1c14 is mandatory and if NotExists Throw ERR 1c14 not found.
				//Set SOC tolerance Fails
			}
		}

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
	return errCodeObj;
}

ocfmRetCode DeleteNode(INT32 nodeId, NodeType nodeType)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	UINT32 nodePos = 0;
	bool nodeExist = false;

	errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, nodeExist);

	if (!nodeExist)
	{
		/* Function didnt throw any exception but Node doesnt exist */
		if (errCodeObj.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			boost::format formatter(kMsgNonExistingNode);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NODEID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
		}
		return errCodeObj;
	}

	Node nodeObj;
	NodeCollection* nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	assert(nodeCollObj);
	nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);

	if (CN == nodeObj.GetNodeType())
	{
		INT32 indexPos = 0;
		INT32 subIndexPos = 0;
		errCodeObj.setErrorCode(OCFM_ERR_UNKNOWN);

		if (MULTIPLEXED == nodeObj.GetStationType())
		{
			//Deleted MN's 1F9B and Subindex = old node id
			if (NULL != nodeObj.GetForcedCycleValue())
			{
				CheckAndReAssignMultiplex(nodeObj.GetNodeId(),
						nodeObj.GetForcedCycleValue());
			}
		}

		char* sidxStr = new char[SUBINDEX_LEN];
		IntToAscii(nodeObj.GetNodeId(), sidxStr, 16);
		sidxStr = PadLeft(sidxStr, '0', 2);

		errCodeObj = IfSubIndexExists(MN_NODEID, MN,
				(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, sidxStr, &subIndexPos,
				&indexPos);
		if (errCodeObj.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			errCodeObj = SetSubIndexAttribute(MN_NODEID, MN,
					(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, sidxStr, ACTUALVALUE,
					(char*) "");
			if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				LOG_ERROR() << "Failed to set subIndex-attribute for index 0x1F9B.";
			}
		}

		errCodeObj = IfSubIndexExists(MN_NODEID, MN,
				(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, sidxStr, &subIndexPos,
				&indexPos);
		if (errCodeObj.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			errCodeObj = SetSubIndexAttribute(MN_NODEID, MN,
					(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, sidxStr,
					ACTUALVALUE, (char*) "");
			if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				LOG_ERROR() << "Failed to set subIndex-attribute for index 0x1F92.";
			}
		}
		delete[] sidxStr;

		//Delete the auto generated pdo indexes also.
		Node *nodeObjMN = NULL;
		IndexCollection *idxCollObjMN = NULL;
		nodeObjMN = nodeCollObj->GetNodePtr(MN, MN_NODEID);
		idxCollObjMN = nodeObjMN->GetIndexCollection();

		INT32 totalIndexMN = idxCollObjMN->GetNumberofIndexes();
		LOG_INFO() << "Deleting node " << nodeObjMN->GetNodeId() << ". " << totalIndexMN << " OD-Indices to delete.";

		for (INT32 idxLC = 0; idxLC < totalIndexMN; idxLC++)
		{
			Index *idxCommObjMN = NULL;
			char* subStr1 = new char[SUBINDEX_LEN];
			char* subStr2 = new char[SUBINDEX_LEN];
			bool idx14Present = false;
			bool idx18Present = false;

			idxCommObjMN = idxCollObjMN->GetIndexByPosition(idxLC);
			subStr1 = SubString(subStr1, idxCommObjMN->GetIndexValue(), 0, 2);
			subStr2 = SubString(subStr2, idxCommObjMN->GetIndexValue(), 2, 2);
			LOG_DEBUG() << "Processing OD-Index 0x" << idxCommObjMN->GetIndexValue();

			if ((0 == strcmp(subStr1, "14")))
			{
				idx14Present = true;
			}
			else if ((0 == strcmp(subStr1, "18")))
			{
				idx18Present = true;
			}
			delete[] subStr1;

			if ((idx14Present == true) || (idx18Present == true))
			{
				SubIndex *sidxObj = NULL;
				sidxObj = idxCommObjMN->GetSubIndexbyIndexValue((char*) "01");

				if (NULL != sidxObj)
				{
					INT32 nodeIdTemp;
					char* sidxActValue = new char[INDEX_LEN];
					strcpy(sidxActValue, sidxObj->GetActualValue());
					nodeIdTemp = GetDecimalValue(sidxActValue);

					if (nodeId == nodeIdTemp)
					{
						Index *idxTxObjMN = NULL;
						char *idxIdTxobjMN = new char[INDEX_LEN];
						if (idx14Present)
						{
							idxIdTxobjMN = strcpy(idxIdTxobjMN, "16");
							idxIdTxobjMN = strcat(idxIdTxobjMN, subStr2);

							idxTxObjMN = idxCollObjMN->GetIndexbyIndexValue(
									idxIdTxobjMN);
						}
						else if (idx18Present)
						{
							idxIdTxobjMN = strcpy(idxIdTxobjMN, "1A");
							idxIdTxobjMN = strcat(idxIdTxobjMN, subStr2);

							idxTxObjMN = idxCollObjMN->GetIndexbyIndexValue(
									idxIdTxobjMN);
						}
						else
						{
							//no others will enter
						}

						if (NULL != idxTxObjMN)
						{
							idxTxObjMN->DeleteSubIndexCollection();
							errCodeObj = AddSubIndex(MN_NODEID, MN,
									idxIdTxobjMN, (char*) "00");
							if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
							{
								LOG_ERROR() << "SubIndex 0x00 cannot be added.";
							}
							sidxObj->SetActualValue((char*) "00");
						}
						LOG_INFO() << "Deleted SubIndex collection.";
						delete[] idxIdTxobjMN;
					}
					delete[] sidxActValue;
				}
				else
				{
					//00th subindex in the communication index is not present
				}
			}

			delete[] subStr2;
		}
	}

	nodeCollObj->DeleteNode(nodePos);

	LOG_INFO() << "Deleted node " << nodeId << ".";

	return errCodeObj;
}

ocfmRetCode DeleteIndex(INT32 nodeId, NodeType nodeType, const char* indexId)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		INT32 idxPos = 0;
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &idxPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.getErrorCode())
		{
			LOG_FATAL() << errCodeObj.getErrorString();
			throw errCodeObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node* nodeObj = NULL;
		IndexCollection* indexCollObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID);
		//pobjIndexCollection = objNode.getIndexCollection();

		nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
		if (!nodeObj)
		{
			boost::format formatter(kMsgNonExistingNode);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NODEID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexCollObj = nodeObj->GetIndexCollection();
		if (indexCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		indexCollObj->DeleteIndex(idxPos);
		LOG_INFO() << "Deleted index 0x" << indexId << "from node " << nodeId << ".";
		errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode DeleteSubIndex(INT32 nodeId, NodeType nodeType, const char* indexId, const char* subIndexID)
{
	INT32 sidxPos = 0;
	INT32 idxPos = 0;
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		if ((indexId == NULL) || (subIndexID == NULL))
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'indexId', 'subIndexID'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, subIndexID,
				&sidxPos, &idxPos);
		if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << errCodeObj.getErrorString();
			throw errCodeObj;
		}

		NodeCollection* nodeCollObj = NULL;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* subIndexObj = NULL;
		Node nodeObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (indexCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(idxPos);
		if (!indexObj)
		{
			boost::format formatter(kMsgNonExistingIndex);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			return errCodeObj;
		}

		subIndexObj = indexObj->GetSubIndexByPosition(sidxPos);
		if (NULL != subIndexObj)
		{
			//delete the sub-index and then updated the 00th entry
			indexObj->DeleteSubIndex(sidxPos);
			UpdateNumberOfEnteriesSIdx(indexObj, nodeType);
			errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
		}
		else
		{
			boost::format formatter(kMsgNonExistingSubIndex);
			formatter 
				% HexToInt<UINT32>(string(indexId))
				% HexToInt<UINT32>(string(subIndexID))
				% nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
		}

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode AddSubIndex(INT32 nodeId, NodeType nodeType, const char* indexId,
		const char* subIndexId)
{
	INT32 sidxPos = 0;
	INT32 idxPos = 0;
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		if (indexId == NULL || subIndexId == NULL)
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'indexId', 'subIndexId'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, subIndexId,
				&sidxPos, &idxPos);
		if (OCFM_ERR_SUCCESS == errCodeObj.getErrorCode())
		{
			boost::format formatter(kMsgExistingSubIndex);
			formatter 
				% HexToInt<UINT32>(string(indexId))
				% HexToInt<UINT32>(string(subIndexId))
				% nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_SUBINDEX_ALREADY_EXISTS);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		if ((OCFM_ERR_NO_SUBINDEXS_FOUND == errCodeObj.getErrorCode())
				|| (OCFM_ERR_SUBINDEXID_NOT_FOUND == errCodeObj.getErrorCode()))
		{
			NodeCollection* nodeCollObj = NULL;
			//Node objNode;
			Node *nodeObj = NULL;
			IndexCollection* idxCollObj = NULL;
			Index* pobjIndex = NULL;

			//                        CIndex objIndex;

			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			assert(nodeCollObj);

			//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID);
			nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);

			//pobjIndexCollection = objNode.getIndexCollection();
			idxCollObj = nodeObj->GetIndexCollection();
			if (idxCollObj->GetNumberofIndexes() == 0)
			{
				boost::format formatter(kMsgObjectDictoryEmpty);
				formatter % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			pobjIndex = idxCollObj->GetIndexByPosition(idxPos);
			if (!pobjIndex)
			{
				boost::format formatter(kMsgNonExistingIndex);
				formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			/* Get the SubIndex from ObjectDictionary*/
			ObjectDictionary* dictCollObj = NULL;
			dictCollObj = ObjectDictionary::GetObjDictPtr();
			if (!dictCollObj)
			{
				boost::format formatter(kMsgNullArgument);
					formatter % "'dictCollObj'";
				errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			SubIndex* sidxObj = NULL;
			SubIndex* dictSidxObj = NULL;

			dictSidxObj = dictCollObj->GetObjectDictSubIndex(indexId,
					subIndexId);
			if (NULL != dictSidxObj)
			{
				sidxObj = new SubIndex;
				sidxObj->SetNodeID(nodeId);
				SetDefaultSubIndexAttributes(subIndexId, sidxObj, dictSidxObj);
				if (NULL != pobjIndex)
				{
					pobjIndex->AddSubIndex(*sidxObj);
				}
			}
			else if ((nodeType == MN)
					&& (true == CheckIfProcessImageIdx(indexId)))
			{
				sidxObj = new SubIndex;
				sidxObj->SetNodeID(nodeId);
				sidxObj->SetIndexValue(subIndexId);
				if (0 != strcmp(subIndexId, (const char*) "00"))
				{
					sidxObj->SetName((char*) "NumberOfEntries");
					sidxObj->SetDataType((char*) "UNSIGNED8");
					sidxObj->SetAccessType((char*) "rw");
				}
				if (NULL != pobjIndex)
				{
					pobjIndex->AddSubIndex(*sidxObj);
				}
			}
			else if (true == CheckIfManufactureSpecificObject(indexId))
			{
				sidxObj = new SubIndex;
				sidxObj->SetNodeID(nodeId);
				sidxObj->SetIndexValue(subIndexId);

				if (NULL != pobjIndex)
				{
					if ((ARRAY == pobjIndex->GetEObjectType())
							&& (0 != strcmp(subIndexId, (const char*) "00")))
					{
						//If objectType='ARRAY', all subobjects (except 0x00) have got the same dataType as the object
						sidxObj->SetDataTypeST(pobjIndex->GetDataType());
					}
					//all the subobjects is of type VAR
					sidxObj->SetPDOMapping((char*) "NOT_DEFINED");
					sidxObj->SetObjectType((char*) "VAR");
					sidxObj->SetFlagIfIncludedCdc(true);
					pobjIndex->AddSubIndex(*sidxObj);
				}
			}
			else
			{
				boost::format formatter(kMsgSubIndexInvalid);
				formatter 
					% HexToInt<UINT32>(string(indexId))
					% HexToInt<UINT32>(string(subIndexId))
					% nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXID);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}
			/* Update subindex "00"*/
			if ((NULL != sidxObj) && (NULL != sidxObj->GetIndexValue()))
			{
				if (0 != strcmp(sidxObj->GetIndexValue(), (const char*) "00"))
				{
					UpdateNumberOfEnteriesSIdx(pobjIndex, nodeType);
				}
			}

			//SetDefaultSubIndexAttributes(varSubIndexID, &pobjSubIndex);

			errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode AddSubobject(INT32 nodeId, NodeType nodeType, const char* indexId)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		if (!indexId)
		{
			boost::format formatter(kMsgNullArgument);
			formatter
				% "'indexId'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		LOG_INFO() << "Adding subIndex 0x0 to index 0x" << indexId << ".";
		errCodeObj = AddSubIndex(nodeId, nodeType, indexId, (char*) "00");
		if ((OCFM_ERR_SUCCESS == errCodeObj.getErrorCode())
				&& (true == CheckIfManufactureSpecificObject(indexId)))
		{
			NodeCollection* nodeCollObj = NULL;
			Node nodeObj;
			IndexCollection* indexCollObj = NULL;

			Index* indexObj = NULL;
			SubIndex* sidxObj = NULL;

			INT32 indexPos = 0;
			INT32 sidxPos = 0;

			errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId,
					(char*) "00", &sidxPos, &indexPos);
			if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				LOG_FATAL() << "IfSubIndexExists() returned '" << errCodeObj.getErrorCode() << "'.";
				boost::format formatter(kMsgSubIndexInvalid);
				formatter % HexToInt<UINT32>(string(indexId)) % 0 % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXID);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			assert(nodeCollObj);

			nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

			indexCollObj = nodeObj.GetIndexCollection();
			if (indexCollObj->GetNumberofIndexes() == 0)
			{
				boost::format formatter(kMsgObjectDictoryEmpty);
				formatter % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			indexObj = indexCollObj->GetIndexByPosition(indexPos);
			if (!indexObj)
			{
				boost::format formatter(kMsgNonExistingIndex);
				formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			sidxObj = indexObj->GetSubIndexByPosition(sidxPos);
			if (!sidxObj)
			{
				boost::format formatter(kMsgNonExistingSubIndex);
				formatter 
					% HexToInt<UINT32>(string(indexId)) 
					% 0 
					% nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			sidxObj->SetName((char*) "NumberOfEntries");
			sidxObj->SetObjectType((char*) "VAR");

			if(0 == strcmp(indexObj->GetObjectType(), "ARRAY"))
			{
				sidxObj->SetAccessType((char*) "rw");
			}
			else
			{
				sidxObj->SetAccessType((char*) "const");
			}
			sidxObj->SetPDOMapping((char*) "NO");
			sidxObj->SetDefaultValue((char*) "0x0");
			sidxObj->SetActualValue((char*) "0x0");
			sidxObj->SetHighLimit((char*) "0xFE");
			sidxObj->SetLowLimit((char*) "0x0");

			char* dtName = new char[15];
			strcpy(dtName, (const char*) "UNSIGNED8");
			if (true == (CheckIfDataTypeByNameExists(dtName, nodeId)))
			{
				sidxObj->SetDataTypeName(dtName, nodeId);
			}
			else
			{
				boost::format formatter(kMsgSimpleDatatypeNotFound);
				formatter 
					% dtName
					% indexObj->GetIndex()
					% sidxObj->GetIndexValue()
					% nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_DATATYPE_NOT_FOUND);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();

				delete[] dtName;
				throw errCodeObj;
			}
			delete[] dtName;
		}
		errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode AddIndex(INT32 nodeId, NodeType nodeType, const char* indexId)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);
	INT32 indexPos = 0;

	try
	{
		if (!indexId)
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'indexId'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (OCFM_ERR_SUCCESS == errCodeObj.getErrorCode())
		{
			boost::format formatter(kMsgExistingIndex);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INDEX_ALREADY_EXISTS);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		if ((OCFM_ERR_NO_INDEX_FOUND == errCodeObj.getErrorCode())
			|| (OCFM_ERR_INDEXID_NOT_FOUND == errCodeObj.getErrorCode()))
		{
			NodeCollection* nodeCollObj = NULL;
			Node nodeObj;
			IndexCollection* indexCollObj = NULL;
			ObjectDictionary* dictObj = NULL;
			Index* dictIndexObj = NULL;

			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			assert(nodeCollObj);
			nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
			indexCollObj = nodeObj.GetIndexCollection();
			if (indexCollObj->GetNumberofIndexes() == 0)
			{
				boost::format formatter(kMsgObjectDictoryEmpty);
				formatter % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			/* Get the Index from ObjectDictionary*/
			dictObj = ObjectDictionary::GetObjDictPtr();
			if (!dictObj)
			{
				boost::format formatter(kMsgNullArgument);
				formatter
					% "'dictObj'";
				errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}
			//Validate for TPDO channels for a CN
			if ((CN == nodeObj.GetNodeType())
					&& ((0 == strncmp(indexId, "1A", 2))
							|| (0 == strncmp(indexId, "1a", 2))))
			{
				INT32 tpdoCount = 0;
				INT32 rpdoCount = 0;
				nodeObj.getPDOIndexCollection(&rpdoCount, &tpdoCount);
				LOG_DEBUG() << "TPDO-Channels: " << tpdoCount << " RPDO-Channels: " << rpdoCount << ".";
				//Allowed to add TPDO only if the node has 0 TPDO's(1Axx)
				if (tpdoCount > 0)
				{
					boost::format formatter(kMsgTPDOChannelCountExceeded);
					formatter % nodeId % 1 % tpdoCount;
					errCodeObj.setErrorCode(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					errCodeObj.setErrorString(string("Cannot add index 0x") + indexId + ". " + formatter.str());
					LOG_FATAL() << formatter.str();
					throw errCodeObj;
				}
			}
			dictIndexObj = dictObj->GetObjectDictIndex(indexId);
			if (NULL != dictIndexObj)
			{
				Index indexObj;
				indexObj.SetNodeID(nodeId);
				SetDefaultIndexAttributes(indexId, &indexObj, dictIndexObj);
				for (INT32 idxLC = 0;
						idxLC < dictIndexObj->GetNumberofSubIndexes(); idxLC++)
				{
					SubIndex* objSIdx;
					objSIdx = dictIndexObj->GetSubIndexByPosition(idxLC);
					//objSIdx->SetNodeID(nodeId);
					if (NULL != objSIdx)
					{
						indexObj.AddSubIndex(*objSIdx);
					}
				}
				char indexSubStr[INDEX_LEN];
				SubString((char*) indexSubStr, (const char*) indexId, 2, 4);
				char* newIndexName = dictObj->GetIndexName((char*)indexSubStr, (char*) indexObj.GetName());
				if( newIndexName != NULL)
				{
					indexObj.SetName(newIndexName);
				}
				//updates the no of entries for the subindex added
				UpdateNumberOfEnteriesSIdx(&indexObj, nodeType);
				indexCollObj->AddIndex(indexObj);
				delete[] newIndexName;
			}
			else if ((MN == nodeType)
					&& (true == CheckIfProcessImageIdx(indexId)))
			{
				Index indexObj;
				indexObj.SetNodeID(nodeId);
				indexObj.SetIndexValue(indexId);
				indexCollObj->AddIndex(indexObj);
			}
			else if (true == CheckIfManufactureSpecificObject(indexId))
			{
				Index indexObj;
				indexObj.SetNodeID(nodeId);
				indexObj.SetIndexValue(indexId);
				indexObj.SetFlagIfIncludedCdc(true);
				indexCollObj->AddIndex(indexObj);
			}
			else
			{
				boost::format formatter(kMsgIndexInvalid);
				formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
				errCodeObj.setErrorCode(OCFM_ERR_INVALID_INDEXID);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}

			errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
		}
		else
		{
			// This Part of code is never expected to happen
			boost::format formatter(kMsgIndexInvalid);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_INDEXID);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode SetBasicIndexAttributes(INT32 nodeId, NodeType nodeType,
		const char* indexId, const char* indexValue, const char* indexName, bool includeInCDC)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		if ((indexId == NULL) || (indexValue == NULL) || (indexName == NULL))
		{
			boost::format formatter(kMsgNullArgument);
			formatter
				% "'indexId', 'indexValue', 'indexName'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		INT32 indexPos = 0;
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.getErrorCode())
		{
			LOG_FATAL() << errCodeObj.getErrorString();
			throw errCodeObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		//Index objIndex;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
		//objIndex.SetNodeID(nodeId);
		indexCollObj = nodeObj.GetIndexCollection();
		if (indexCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		if (!indexObj)
		{
			boost::format formatter(kMsgNonExistingIndex);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexObj->SetName(indexName);
		indexObj->SetFlagIfIncludedCdc(includeInCDC);

		if (NULL != indexValue)
		{
			if (true == indexObj->IsIndexValueValid(indexValue))
			{
				indexObj->SetActualValue(indexValue);
				errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
			}
			// FIXME: Dead code - IsIndexValueValid() always returns true
			//else
			//{
			//	errCodeObj.setErrorCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
			//	LOG_FATAL() << errCodeObj.getErrorString();
			//	throw errCodeObj;
			//}
		}

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode SetBasicSubIndexAttributes(INT32 nodeId, NodeType nodeType,
		const char* indexId, const char* sidxId, const char* indexValue, const char* indexName,
		bool includeInCDC)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		if ((indexId == NULL) || (sidxId == NULL) || (indexValue == NULL)
				|| (indexName == NULL))
		{
			boost::format formatter(kMsgNullArgument);
			formatter
			% "'indexId', 'sidxId', 'indexValue', 'indexName'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		INT32 indexPos = 0;
		INT32 subIndexPos = 0;
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&subIndexPos, &indexPos);
		if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << "IfSubIndexExists() returned '" << errCodeObj.getErrorCode() << "'.";
			boost::format formatter(kMsgSubIndexInvalid);
			formatter % HexToInt<UINT32>(string(indexId)) 
				% HexToInt<UINT32>(string(sidxId))
				% nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXID);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		if ((MN_NODEID == nodeId) && (MN == nodeType)
				&& (0 == strcmp(indexId, "1F92"))
				&& (0 != strcmp(sidxId, "00")))
		{
			if (false == ValidateCNPresTimeout(sidxId, indexValue))
			{
				boost::format formatter(kMsgLowCnPresTimeout);
				formatter % HexToInt<UINT32>(string(indexValue))
					% HexToInt<UINT32>(string(sidxId))
					% HexToInt<UINT32>(string(sidxId));
				errCodeObj.setErrorCode(OCFM_ERR_LOW_CNPRESTIMEOUT);
				errCodeObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw errCodeObj;
			}
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* sidxObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (indexCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		if (!indexObj)
		{
			boost::format formatter(kMsgNonExistingIndex);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		sidxObj = indexObj->GetSubIndexByPosition(subIndexPos);
		if (!sidxObj)
		{
			boost::format formatter(kMsgNonExistingSubIndex);
			formatter 
				% HexToInt<UINT32>(string(indexId)) 
				% HexToInt<UINT32>(string(sidxId)) 
				% nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		sidxObj->SetName(indexName);
		sidxObj->SetFlagIfIncludedCdc(includeInCDC);

		if (sidxObj->IsIndexValueValid(indexValue))
		{
			sidxObj->SetActualValue(indexValue);
			errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
		}
		// FIXME: Dead code - IsIndexValueValid() always returns true
		//else
		//{
		//	errCodeObj.setErrorCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
		//	LOG_FATAL() << errCodeObj.getErrorString();
		//	throw errCodeObj;
		//}

		if ((MN_NODEID == nodeId) && (MN == nodeType)
				&& (0 == strcmp(indexId, (const char*) "1F98"))
				&& (0 == strcmp(sidxId, (const char*) "07")))
		{
			errCodeObj = RecalculateMultiplex();
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode SetAllIndexAttributes(INT32 nodeId, NodeType nodeType,
		char* indexId, char* actualValue, char* indexName, char* accessType,
		char* dataTypeName, char* pdoMappingVal, char* defaultValue,
		char* highLimitVal, char* lowLimitVal, char* objectType,
		bool includeInCDC)
{
	ocfmRetCode errCodeObj;

	try
	{
		// || (NULL == varActualValue) || (NULL == varIndexName) || (NULL == varAccess) || (NULL == varDataTypeName) || (NULL == pdoMappingVal) || (NULL == vardefaultValue) || (NULL == varhighLimit) || (NULL == varlowLimit) || (NULL == varobjType)
		if (!indexId)
		{
			boost::format formatter(kMsgNullArgument);
			formatter
				% "'indexId'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		INT32 indexPos;
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.getErrorCode())
		{
			LOG_FATAL() << errCodeObj.getErrorString();
			throw errCodeObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (indexCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		if (!indexObj)
		{
			boost::format formatter(kMsgNonExistingIndex);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		/* Check if the value is valid*/
		if (NULL != indexName)
		{
			if (strcmp(indexName, "") != 0)
				indexObj->SetName(indexName);
		}

		if (NULL != accessType)
		{
			if (strcmp(accessType, "") != 0)
				indexObj->SetAccessType(accessType);
		}

		if (NULL != pdoMappingVal)
		{
			indexObj->SetPDOMapping(pdoMappingVal);
		}
		else
		{
			indexObj->SetPDOMapping((char*) "NOT_DEFINED");
		}

		if (NULL != defaultValue)
		{
			indexObj->SetDefaultValue(defaultValue);
		}
		ocfmRetCode errorLimitInfo;
		if ((NULL != highLimitVal) && (NULL != lowLimitVal))
		{
			errorLimitInfo = CheckUpperAndLowerLimits(lowLimitVal,
					highLimitVal);
			if (OCFM_ERR_SUCCESS == errorLimitInfo.getErrorCode())
			{				
				indexObj->SetHighLimit(highLimitVal);
				indexObj->SetLowLimit(lowLimitVal);
			}
			else
			{
				boost::format formatter(kMsgNodeIndexDescription);
				formatter % nodeId
					% indexObj->GetIndex();
				errorLimitInfo.setErrorString(formatter.str() + errorLimitInfo.getErrorString());
				LOG_FATAL() << errorLimitInfo.getErrorString();
				return errorLimitInfo;
			}
		}

		if (NULL != objectType)
		{
			indexObj->SetObjectType(objectType);
			if (indexObj->GetEObjectType() == ARRAY)
			{
				indexObj->UpdateArraySubObjects();
			}
		}

		indexObj->SetFlagIfIncludedCdc(includeInCDC);

		if ((ARRAY == indexObj->GetEObjectType())
				|| (RECORD == indexObj->GetEObjectType()))
		{
			//delete the subobject 00 and then add and updates the number of entries
			errCodeObj = DeleteSubIndex(nodeId, nodeType, indexId, (char*)"00");
			errCodeObj = AddSubobject(nodeId, nodeType, indexId);
			UpdateNumberOfEnteriesSIdx(indexObj, nodeType);
		}

		if (NULL != dataTypeName)
		{
			if (strcmp(dataTypeName, "") != 0)
			{
				if ((CheckIfDataTypeByNameExists(dataTypeName,
						indexObj->GetNodeID())) == true)
				{
					indexObj->SetDataTypeName(dataTypeName, nodeId);
					//DataType objDataType;
					//objDataType = pobjIndex->GetDataType();
					if (indexObj->GetEObjectType() == ARRAY)
					{
						indexObj->UpdateArraySubObjects();
					}
				}
				else
				{
					boost::format formatter(kMsgSimpleDatatypeNotFound);
					formatter
						% dataTypeName
						% indexObj->GetIndex()
						% "-"
						% nodeId;
					errCodeObj.setErrorCode(OCFM_ERR_DATATYPE_NOT_FOUND);
					errCodeObj.setErrorString(formatter.str());
					LOG_FATAL() << formatter.str();
					throw errCodeObj;
				}
			}
		}
		if (NULL != actualValue)
		{
			if (indexObj->IsIndexValueValid(actualValue))
			{
				indexObj->SetActualValue(actualValue);
				errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);

			}
			// FIXME: Dead code - IsIndexValueValid() always returns true
			//else
			//{
			//	errCodeObj.setErrorCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
			//	LOG_FATAL() << errCodeObj.getErrorString();
			//	throw errCodeObj;
			//}
		}

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode SetAllSubIndexAttributes(INT32 nodeId, NodeType nodeType,
		char* indexId, char* sidxId, char* actualValue, char* indexName,
		char* accessType, char* dataTypeName, char* pdoMappingVal,
		char* defaultValue, char* highLimitVal, char* lowLimitVal,
		char* objectType, bool includeInCDC)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		if ((indexId == NULL) || (sidxId == NULL))
		{
			boost::format formatter(kMsgNullArgument);
			formatter
				% "'indexId', 'sidxId'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		INT32 indexPos = 0;
		INT32 sidxPos = 0;
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&sidxPos, &indexPos);
		if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << "IfSubIndexExists() returned '" << errCodeObj.getErrorCode() << "'.";
			boost::format formatter(kMsgSubIndexInvalid);
			formatter % HexToInt<UINT32>(string(indexId)) 
				% HexToInt<UINT32>(string(sidxId))
				% nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXID);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* subIndexObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (indexCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		if (!indexObj)
		{
			boost::format formatter(kMsgNonExistingIndex);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		subIndexObj = indexObj->GetSubIndexByPosition(sidxPos);
		if (!subIndexObj)
		{
			boost::format formatter(kMsgNonExistingSubIndex);
			formatter 
				% HexToInt<UINT32>(string(indexId)) 
				% HexToInt<UINT32>(string(sidxId)) 
				% nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		if (NULL != indexName)
		{
			subIndexObj->SetName(indexName);
		}

		if (NULL != accessType)
		{
			subIndexObj->SetAccessType(accessType);
		}

		if (NULL != pdoMappingVal)
		{
			subIndexObj->SetPDOMapping(pdoMappingVal);
		}
		else
		{
			subIndexObj->SetPDOMapping((char*) "NOT_DEFINED");
		}

		if (NULL != defaultValue)
		{
			subIndexObj->SetDefaultValue(defaultValue);
		}
		ocfmRetCode errLimitInfo;
		if ((NULL != highLimitVal) && (NULL != lowLimitVal))
		{
			errLimitInfo = CheckUpperAndLowerLimits(lowLimitVal, highLimitVal);
			if (OCFM_ERR_SUCCESS == errLimitInfo.getErrorCode())
			{
				subIndexObj->SetHighLimit(highLimitVal);
				subIndexObj->SetLowLimit(lowLimitVal);
			}
			else
			{
				boost::format formatter(kMsgNodeSubIndexDescription);
				formatter % nodeId
					% indexObj->GetIndex()
					% subIndexObj->GetIndex();
				errLimitInfo.setErrorString(formatter.str() + errLimitInfo.getErrorString());
				LOG_FATAL() << errLimitInfo.getErrorString();
				return errLimitInfo;
			}
		}

		if (NULL != objectType)
		{
			subIndexObj->SetObjectType(objectType);
		}

		subIndexObj->SetFlagIfIncludedCdc(includeInCDC);

		if (NULL != actualValue)
		{
			if (subIndexObj->IsIndexValueValid(actualValue))
			{
				subIndexObj->SetActualValue(actualValue);
				errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
			}
			// FIXME: Dead code - IsIndexValueValid() always returns true
			//else
			//{
			//	errCodeObj.setErrorCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
			//	LOG_FATAL() << errCodeObj.getErrorString();
			//	throw errCodeObj;
			//}
		}
		if (NULL != dataTypeName)
		{
			if (0 != strcmp(dataTypeName, (const char*) ""))
			{
				if (true
						== (CheckIfDataTypeByNameExists(dataTypeName,
								subIndexObj->GetNodeID())))
				{
					subIndexObj->SetDataTypeName(dataTypeName, nodeId);
				}
				else
				{
					boost::format formatter(kMsgSimpleDatatypeNotFound);
					formatter
						% dataTypeName
						% indexObj->GetIndex()
						% subIndexObj->GetIndexValue()
						% nodeId;
					errCodeObj.setErrorCode(OCFM_ERR_DATATYPE_NOT_FOUND);
					errCodeObj.setErrorString(formatter.str());
					LOG_FATAL() << formatter.str();

					throw errCodeObj;
				}
			}
		}

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode SetSubIndexAttribute(INT32 nodeId, NodeType nodeType,const char* indexId,
		const char* sidxId, AttributeType attributeType, const char* attributeValue)
{
	ocfmRetCode errCodeObj(OCFM_ERR_UNKNOWN);

	try
	{
		if ((indexId == NULL) || (sidxId == NULL))
		{
			boost::format formatter(kMsgNullArgument);
			formatter
				% "'indexId', 'sidxId'";
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		INT32 sidxPos = 0;
		INT32 iIndexPos = 0;
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&sidxPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.getErrorCode())
		{
			return errCodeObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* sidxObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (indexCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(iIndexPos);
		if (!indexObj)
		{
			boost::format formatter(kMsgNonExistingIndex);
			formatter % HexToInt<UINT32>(string(indexId)) % nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		sidxObj = indexObj->GetSubIndexByPosition(sidxPos);
		if (!sidxObj)
		{
			boost::format formatter(kMsgNonExistingSubIndex);
			formatter 
				% HexToInt<UINT32>(string(indexId)) 
				% HexToInt<UINT32>(string(sidxId)) 
				% nodeId;
			errCodeObj.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}

		switch (attributeType)
		{
		case NAME:
			// No Operation
			break;
		case OBJECTTYPE:
			// No Operation
			break;
		case DATATYPE:
			// No Operation
			break;
		case ACCESSTYPE:
			// No Operation
			break;
		case DEFAULTVALUE:
			// No Operation
			break;
		case ACTUALVALUE:
			if (NULL != attributeValue)
				sidxObj->SetActualValue(attributeValue);
			else
				sidxObj->SetActualValue((char*) "");
			break;
		case PDOMAPPING:
			if (NULL != attributeValue)
				sidxObj->SetPDOMapping(attributeValue);
			else
				sidxObj->SetPDOMapping((char*) "");
			break;
		case LOWLIMIT:
			// No Operation
			break;
		case HIGHLIMIT:
			// No Operation
			break;
		case FLAGIFINCDC:
			// No Operation
			break;
		default:
		{
			boost::format formatter(kMsgUnsupportedAttributeType);
			formatter % ((int) attributeType);
			errCodeObj.setErrorCode(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			errCodeObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw errCodeObj;
		}
		}

		errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return errCodeObj;
}

ocfmRetCode CheckUpperAndLowerLimits(const char* lowLimitVal, const char* highLimitVal)
{
	ocfmRetCode errCodeObj;
	errCodeObj.setErrorCode(OCFM_ERR_SUCCESS);

	if ((NULL != lowLimitVal) && (NULL != highLimitVal))
	{
		if ((0 != strcmp(lowLimitVal, "")) && (0 != strcmp(highLimitVal, "")))
		{
			ULONG tempLowLimit;
			ULONG tempHighLimit;
			if (true == CheckIfHex((char*) lowLimitVal))
			{
				char *lowLimitTemp = new char[strlen(lowLimitVal)];
				SubString(lowLimitTemp, (const char*) lowLimitVal, 2, (strlen(lowLimitVal) - 2));
				tempLowLimit = HexToInt(lowLimitTemp);
				delete[] lowLimitTemp;
			}
			else
			{
				tempLowLimit = atoi(lowLimitVal);
			}

			if (true == CheckIfHex((char*) highLimitVal))
			{
				char *highLimitTemp = new char[strlen(lowLimitVal)];
				SubString(highLimitTemp, (const char*) highLimitVal, 2, (strlen(highLimitVal) - 2));
				tempHighLimit = HexToInt(highLimitTemp);
				delete[] highLimitTemp;
			}
			else
			{
				tempHighLimit = atoi(highLimitVal);
			}

			if (tempHighLimit >= tempLowLimit)
			{
				return errCodeObj;
			}
			else
			{				
				boost::format formatter(kMsgObjectLimitsInvalid);
				formatter % tempHighLimit
					% tempLowLimit;
				errCodeObj.setErrorCode(OCFM_ERR_INVALID_UPPERLOWER_LIMITS);
				errCodeObj.setErrorString(formatter.str());
				return errCodeObj;
			}
		}
	}
	return errCodeObj;
}

void EnableDisableMappingPDO(IndexCollection* indexCollObj, Index* indexObj,
		char* cdcBuffer, bool enablePDO)
{
	ocfmRetCode exceptionObj;

	//Get the Index Value
	if (!indexCollObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexCollObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	if (!indexObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	if (!cdcBuffer)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'cdcBuffer'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	strcpy(cdcBuffer, "");
	if (0 != indexObj->GetNumberofSubIndexes())
	{

		//bool resetValueAdded = false;
		SubIndex* sidxObj = NULL;
		sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
		if (NULL != sidxObj)
		{
			//if (NULL != sidxObj->GetActualValue())
			if ((NULL != sidxObj->GetActualValue())
					&& (0 != strcmp(sidxObj->GetActualValue(), ""))
					&& !(CheckIfValueZero((char*) sidxObj->GetActualValue())))
			{
				INT32 noOfSubIndexes = 0; //= pobjIndex->getNumberofSubIndexes();
				if (true == CheckIfHex((char*) sidxObj->GetActualValue()))
				{
					INT32 sidxActLen = strlen(sidxObj->GetActualValue());
					char *sidxActVal = new char[sidxActLen];
					SubString(sidxActVal, sidxObj->GetActualValue(), 2, (sidxActLen - 2));
					noOfSubIndexes = (INT32) HexToInt(sidxActVal);
					delete[] sidxActVal;
				}
				else
				{
					noOfSubIndexes = atoi(sidxObj->GetActualValue());
				}

				/* No need to reinitailize mapping pdo to zero again */
				if ((0 == noOfSubIndexes) && (true == enablePDO))
				{
					return;
				}

				if ((true == sidxObj->GetFlagIfIncludedCdc())
						&& ((true
								== ReactivateMappingPDO(indexCollObj, indexObj))
								|| (true == IsDefaultActualNotEqual(sidxObj))))
				{
					bool isStringDt = false;
					//Format: 1600    01   00000000   Act_value
					strcat(cdcBuffer, indexObj->GetIndexValue());
					//Place a tab
					strcat(cdcBuffer, "\t");

					strcat(cdcBuffer, sidxObj->GetIndexValue());
					strcat(cdcBuffer, "\t");
					//Add datatype
					DataType dtObj;
					dtObj = sidxObj->GetDataType();
					INT32 padLength = 0;
					char* dataSizeStr = new char[8 + STR_ALLOC_BUFFER];

					if (NULL != dtObj.dataTypeName)
					{
						if (true == CheckIfStringDatatypes(dtObj.dataTypeName))
						{
							INT32 len = strlen(sidxObj->GetActualValue());
							IntToAscii(len, dataSizeStr, 16);
							dataSizeStr = PadLeft(dataSizeStr, '0', 8);
							strcat(cdcBuffer, dataSizeStr);
							padLength = len * 2;
							isStringDt = true;
						}
						else
						{
							IntToAscii(dtObj.dataSize,dataSizeStr, 16);
							dataSizeStr = PadLeft(dataSizeStr, '0', 8);
							strcat(cdcBuffer, dataSizeStr);
							padLength = dtObj.dataSize * 2;
							isStringDt = false;
						}
					}
					else
					{
						//txt2cdc will not be success
						strcat(cdcBuffer, (const char*) "00000000");
					}
					delete[] dataSizeStr;
					strcat(cdcBuffer, "\t");

					if (false == enablePDO)
					{
						// Actual value checked for Empty check for 00'th subindex non-Zero value.
						if ((0 != strcmp(sidxObj->GetActualValue(), ""))
								&& !(CheckIfValueZero(
										(char*) sidxObj->GetActualValue())))
						{
							char actValue[20];
							strcpy(actValue, "0");
							strcat(cdcBuffer,
									PadLeft(actValue, '0', padLength));
						}
						else
						{
							//No need to enable/disable the mapping pdo if actual value set to zero or empty
						}
					}
					else
					{
						char actValue[64];
						actValue[0] = '\0';

						if (true == isStringDt)
						{
							strcpy(actValue, (char*) sidxObj->GetActualValue());
							strcpy(actValue,
									ConvertStringToHex((char*) actValue));
							strcat(cdcBuffer, actValue);
						}
						else
						{
							//non empty non-zero actual values are only written to cdc
							if ((0 != strcmp(sidxObj->GetActualValue(), ""))
									&& (!(CheckIfValueZero(
											(char*) sidxObj->GetActualValue()))))
							{
								if (true
										== CheckIfHex(
												(char*) sidxObj->GetActualValue()))
								{
									INT32 actValueLen = strlen(
											(char*) sidxObj->GetActualValue());
									strncpy(actValue,
											((char*) (sidxObj->GetActualValue()
													+ 2)), actValueLen - 2);
									actValue[actValueLen - 2] = '\0';

									strcat(cdcBuffer,
											PadLeft(actValue, '0', padLength));
								}
								else
								{
									strcpy(actValue,
											IntToAscii(
													atoi(
															sidxObj->GetActualValue()),
													actValue, 16));
									strcat(cdcBuffer,
											PadLeft(actValue, '0', padLength));
								}
							}
						}
					}
					strcat(cdcBuffer, "\n");
				}
			}
		}
		else
		{
			boost::format formatter(kMsgNonExistingSubIndex);
			formatter 
				% indexObj->GetIndex()
				% 0
				% indexObj->GetNodeID();
			exceptionObj.setErrorCode(OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
	}
	else
	{
		LOG_ERROR() << "Index 0x" << indexObj->GetIndexValue() << " has no subIndices.";
	}
}

void UpdateCNCycleTime(IndexCollection* indexCollObj, char* cycleTimeValue)
{
	ocfmRetCode exceptionObj;
	Index* indexObj = NULL;

	if (!indexCollObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexCollObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1006");
	if (NULL != indexObj)
	{
		if (NULL != cycleTimeValue)
		{
			cycleTimeValue = ConvertToUpper(cycleTimeValue);
			indexObj->SetActualValue(cycleTimeValue);
		}
		else
		{
			LOG_ERROR() << "Parameter 'cycleTimeValue' must not be NULL.";
		}
	}
	else
	{
		LOG_ERROR() << "Index 0x1006 not found in collection.";
	}

}

void UpdateCNSoCTolerance(IndexCollection* indexCollObj,
		char* socToleranceValue)
{
	ocfmRetCode exceptionObj;
	Index* indexObj = NULL;

	if (!indexCollObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexCollObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1C14");
	if (NULL != indexObj)
	{
		if (NULL != socToleranceValue)
		{
			indexObj->SetActualValue(socToleranceValue);
			indexObj->SetFlagIfIncludedCdc(true);
		}
		else
		{
			LOG_ERROR() << "Parameter 'socToleranceValue' must not be NULL.";
		}
	}
	else
	{
		LOG_ERROR() << "Index 0x1C14 not found in collection.";
	}

}

// FIXME: All Update* functions could be refactored into 1, taking the proper arguments.
void UpdateCNAsyncMTUsize(IndexCollection* indexCollObj, char* asyncMTUsize)
{
	ocfmRetCode exceptionObj;
	Index* indexObj = NULL;

	if (!indexCollObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexCollObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != indexObj)
	{
		SubIndex* subIndexObj = NULL;
		subIndexObj = indexObj->GetSubIndexbyIndexValue((char*) "08");
		if (NULL != subIndexObj)
		{
			if (NULL != asyncMTUsize)
			{
				subIndexObj->SetActualValue(asyncMTUsize);
			}
			else
			{
				LOG_ERROR() << "Parameter 'asyncMTUsize' must not be NULL.";
			}
		}
		else
		{
			LOG_ERROR() << "Subindex 0x1F98/0x8 not found in collection.";
		}
	}
	else
	{
		LOG_ERROR() << "Index 0x1F98 not found in collection.";
	}
}

void UpdateCNMultiPrescal(IndexCollection* indexCollObj, char* multiPrescalVal)
{
	ocfmRetCode exceptionObj;
	Index* indexObj = NULL;

	if (!indexCollObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexCollObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != indexObj)
	{
		SubIndex* sidxObj = NULL;
		sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "07");
		if (NULL != sidxObj)
		{
			if (NULL != multiPrescalVal)
			{
				sidxObj->SetActualValue(multiPrescalVal);
			}
			else
			{
				LOG_ERROR() << "Parameter 'multiPrescalVal' must not be NULL.";
			}
		}
		else
		{
			LOG_ERROR() << "Subindex 0x1F98/0x7 not found in collection.";
		}
	}
	else
	{
		LOG_ERROR() << "Index 0x1F98 not found in collection.";
	}
}

bool IsCNNodeAssignmentValid(Node* nodeObj)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	INT32 indexPos = 0;
	INT32 sidxPos = 0;
	INT32 nodeId = 0;
	bool copyNodeAssignmentVal = false;
	NodeType nodeType;

	if (!nodeObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();

	exceptionObj = IfIndexExists(nodeId, nodeType, (char*) "1F9B", &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		exceptionObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98",
				(char*) "07", &sidxPos, &indexPos);
		if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
		{
			char* multipleCycleCnt = new char[20];
			exceptionObj = GetSubIndexAttributes(nodeId, nodeType, (char*) "1F98",
					(char*) "07", ACTUALVALUE, multipleCycleCnt);
			if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
			{
				if ((NULL != multipleCycleCnt)
						&& (0 != strcmp(multipleCycleCnt, ""))
						&& !(CheckIfValueZero(multipleCycleCnt)))
				{
					copyNodeAssignmentVal = true;
				}
			}
			else
			{
				LOG_ERROR() << "Failed to get attribute 'actualValue' for subIndex 0x1F98/0x7 for node " << nodeId << ".";
			}
			delete[] multipleCycleCnt;
		}
		else
		{
			LOG_ERROR() << "Subindex 0x1F98/0x7 does not exist for node " << nodeId << ".";
		}
	}
	else
	{
		LOG_ERROR() << "Index 0x1F9B does not exist for node " << nodeId << ".";
	}

	exceptionObj = IfIndexExists(nodeId, nodeType, (char*) "1016", &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		copyNodeAssignmentVal = true;
	}
	else
	{
		LOG_ERROR() << "Index 0x1016 does not exist for node " << nodeId << ".";
	}

	exceptionObj = IfIndexExists(nodeId, nodeType, (char*) "1F8D", &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		copyNodeAssignmentVal = true;
	}
	else
	{
		LOG_ERROR() << "Index 0x1F8D does not exist for node " << nodeId << ".";
	}
	return copyNodeAssignmentVal;
}

void UpdateCNMultipleCycleAssign(Node* nodeObj)
{
	/*ocfmRetCode exceptionObj;
	IndexCollection* indexCollObjCN = NULL;
	Index* indexObjCN = NULL;
	Index* indexObjMN = NULL;
	SubIndex* sindexObjCN = NULL;
	SubIndex* sindexObjMN = NULL;

	if (!nodeObj)
	{
		string errorString("Parameter 'nodeObj' must not be NULL.");
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(errorString);
		LOG_FATAL() << errorString;
		throw exceptionObj;
	}

	indexCollObjCN = nodeObj->GetIndexCollection();

	if (indexCollObjCN->GetNumberofIndexes() == 0)
	{
		boost::format formatter(kMsgObjectDictoryEmpty);
		formatter % nodeObj->GetNodeId();
		exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObjCN = indexCollObjCN->GetIndexbyIndexValue((char*) "1F9B");
	if (NULL == indexObjCN)
	{
		return;
	}

	indexObjCN->SetFlagIfIncludedCdc(true);
	indexObjMN = GetMNIndexValues((char*) "1F9B");
	if (NULL == indexObjMN)
	{
		return;
	}

	for (INT32 sidxLC = 0; sidxLC < indexObjCN->GetNumberofSubIndexes();
			sidxLC++)
	{

		sindexObjCN = indexObjCN->GetSubIndexByPosition(sidxLC);
		if (NULL == sindexObjCN)
			continue;

		if (NULL != sindexObjCN->GetIndexValue())
		{
			sindexObjMN = indexObjMN->GetSubIndexbyIndexValue(
					(char*) sindexObjCN->GetIndexValue());
		}

		if (NULL == sindexObjMN)
			continue;

		if (NULL != sindexObjMN->GetActualValue())
		{
			sindexObjCN->SetActualValue((char*) sindexObjMN->GetActualValue());
		}
		else
		{
			sindexObjCN->SetActualValue((char*) "");
		}
		sindexObjCN->SetFlagIfIncludedCdc(true);
	}*/
	
	LOG_DEBUG() << "Updating multiplexed-cycle assignment (0x1F9B) on node " << nodeObj->GetNodeId() << " with MN-Configuration.";
	//Check if index 1F9B exists on MN (is optional)
	assert(NodeCollection::GetNodeColObjectPointer());
	Node& managingNode = NodeCollection::GetNodeColObjectPointer()->GetNodeRef(MN_NODEID);
	assert(managingNode.GetIndexCollection());
	if (!managingNode.GetIndexCollection()->ContainsIndex(0x1F9B, 0))
	{
		LOG_WARN() << "Multiplexed-cycle assignment cannot be updated because index 0x1F9B/0x0 is missing on MN.";
		return;
	}	
	//Check if index 1F9B exists on CN (is optional)
	assert(nodeObj->GetIndexCollection());
	if (!nodeObj->GetIndexCollection()->ContainsIndex(0x1F9B, 0))
	{
		LOG_WARN() << "Multiplexed-cycle assignment cannot be updated because index 0x1F9B/0x0 is missing on CN.";
		return;
	}
	Index& NMT_MultiplCycleAssign = managingNode.GetIndexCollection()->GetIndexRef(0x1F9B);
	for (int subIndex = 0; subIndex < NMT_MultiplCycleAssign.GetNumberofSubIndexes(); subIndex++)
	{
		SubIndex* subIndexPtr = NMT_MultiplCycleAssign.GetSubIndexByPosition(subIndex);
		if (subIndexPtr->GetActualValue())
		{
			LOG_DEBUG() << "Assigning multiplexed cycle " 
				<< subIndexPtr->GetActualValue()
				<< " to node " << subIndexPtr->GetIndexValue() << ".";
			CopyMNSubindexToCN(nodeObj, NMT_MultiplCycleAssign.GetIndexValue(), subIndexPtr->GetIndexValue());
		}
	}
}

void UpdateCNPresMNActLoad(Node* nodeObj)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	INT32 indexPos = 0;
	INT32 subIndexPos = 0;
	INT32 nodeId = 0;
	NodeType nodeType;

	NodeCollection* nodeCollObj = NULL;
	IndexCollection* indexCollObj = NULL;
	Index* indexObj = NULL;

	if (!nodeObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	assert(nodeCollObj);

	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();
	exceptionObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F8D",
			(char*) "F0", &subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
	{
		LOG_ERROR() << "IfSubIndexExists() for subIndex 0x1F8D/0xF0 and node " 
			<< nodeId 
			<< " failed with error code " 
			<< exceptionObj.getErrorCode() << ".";
		return;
	}

	indexCollObj = nodeObj->GetIndexCollection();
	if (indexCollObj->GetNumberofIndexes() == 0)
	{
		boost::format formatter(kMsgObjectDictoryEmpty);
		formatter % nodeId;
		exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1F8D");
	if (!indexObj)
	{
		boost::format formatter(kMsgNonExistingIndex);
		formatter % 0x1F8D % nodeId;
		exceptionObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	else
	{
		string subIndexId = "F0";
		SubIndex *sidxObj = indexObj->GetSubIndexbyIndexValue(subIndexId.c_str());
		if (!sidxObj)
		{
			boost::format formatter(kMsgNonExistingSubIndex);
			formatter 
				% indexObj->GetIndex()
				% HexToInt<UINT32>(subIndexId) 
				% nodeId;
			exceptionObj.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		else
		{
			if (CHAINED == nodeObj->GetStationType())
			{
				char conValue[5];
				char actValue[7];
				Node nodeObjMN;

				//memset(conValue, 0, 20 * sizeof(char));
				//memset(actValue, 0, 22 * sizeof(char));
				nodeObjMN = nodeCollObj->GetMNNode();

				if (PRES_DEFAULT_PAYLOAD > nodeObjMN.GetPResActPayloadValue())
				{
					IntToAscii(PRES_DEFAULT_PAYLOAD, conValue, 16);
				}
				else
				{
					IntToAscii(nodeObjMN.GetPResActPayloadValue(), conValue,
							16);
				}
				strcpy(actValue, (char*) "0x");
				strcat(actValue, conValue);
				sidxObj->SetActualValue(actValue);

				indexObj->SetFlagIfIncludedCdc(true);
				sidxObj->SetFlagIfIncludedCdc(true);
			}
			else
			{
				sidxObj->SetActualValue((char*) "");
				sidxObj->SetFlagIfIncludedCdc(false);
			}
		}
	}
}

void UpdatePreqActLoad(Node* nodeObj)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	IndexCollection* indexCollObj = NULL;
	Index* indexObj = NULL;

	INT32 indexPos = 0;
	INT32 subIndexPos = 0;
	INT32 nodeId = 0;
	NodeType nodeType;

	if (!nodeObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();
	exceptionObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98",
			(char*) "04", &subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
	{
		LOG_ERROR() << "IfSubIndexExists() for subIndex 0x1F98/0x04 and node " 
			<< nodeId 
			<< " failed with error code " 
			<< exceptionObj.getErrorCode() << ".";
		return;
	}

	indexCollObj = nodeObj->GetIndexCollection();
	if (indexCollObj->GetNumberofIndexes() == 0)
	{
		boost::format formatter(kMsgObjectDictoryEmpty);
		formatter % nodeId;
		exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != indexObj)
	{
		SubIndex* sidxObj = NULL;
		char* subIndexId = new char[SUBINDEX_LEN];
		strcpy(subIndexId, (char*) "04");
		sidxObj = indexObj->GetSubIndexbyIndexValue(subIndexId);
		if (NULL != sidxObj)
		{
			char conValue[20];
			memset(conValue, 0, 20 * sizeof(char));
			char actValue[22];
			memset(actValue, 0, 22 * sizeof(char));

			if (PREQ_DEFAULT_PAYLOAD > nodeObj->GetPReqActPayloadValue())
			{
				IntToAscii(PREQ_DEFAULT_PAYLOAD, conValue, 16);
			}
			else
			{
				IntToAscii(nodeObj->GetPReqActPayloadValue(), conValue, 16);
			}
			strcpy((char*) conValue, ConvertToUpper((char*) conValue));
			strcpy(actValue, (char*) "0x");
			strcat(actValue, conValue);

			sidxObj->SetActualValue(actValue);
			indexObj->SetFlagIfIncludedCdc(true);
			sidxObj->SetFlagIfIncludedCdc(true);

			//set the value in MN
			if (CN == nodeObj->GetNodeType())
			{
				Index* indexObjMN = NULL;

				IntToAscii(nodeObj->GetNodeId(), subIndexId, 16);
				subIndexId = PadLeft(subIndexId, '0', 2);

				exceptionObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F8B",
						subIndexId, &subIndexPos, &indexPos);
				if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
				{
					return;
				}
				indexObjMN = GetMNIndexValues((char*) "1F8B");
				if (NULL != indexObjMN)
				{
					SubIndex* sidxObjMN = NULL;
					sidxObjMN = indexObjMN->GetSubIndexbyIndexValue(subIndexId);
					if (NULL != sidxObjMN)
					{
						sidxObjMN->SetActualValue(actValue);
						indexObjMN->SetFlagIfIncludedCdc(true);
						sidxObjMN->SetFlagIfIncludedCdc(true);
					}

					//00'th sub index is set to FE(a max value) for make to reflect in cdc
					sidxObjMN = indexObjMN->GetSubIndexbyIndexValue(
							(char*) "00");
					if (NULL != sidxObjMN)
					{
						strcpy(actValue, (char*) "0xFE"); //to make display of 1f8b in cdc if act != def value
						sidxObjMN->SetActualValue(actValue);
					}
				}
			}
		}
		delete[] subIndexId;
	}

}

void UpdatePresActLoad(Node* nodeObj)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	Index* indexObj = NULL;
	IndexCollection* indexCollObj = NULL;
	INT32 indexPos = 0;
	INT32 subIndexPos = 0;
	INT32 nodeId = 0;
	char conValue[20];
	char actValue[22];
	char* subIndexId = NULL;
	NodeType nodeType;

	if (!nodeObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	memset(conValue, 0, 20 * sizeof(char));
	memset(actValue, 0, 22 * sizeof(char));
	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();
	subIndexId = new char[SUBINDEX_LEN];
	strcpy(subIndexId, (char*) "05");

	exceptionObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98", subIndexId,
			&subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
	{
		LOG_ERROR() << "IfSubIndexExists() for subIndex 0x1F98/0x05 and node " 
			<< nodeId 
			<< " failed with error code " 
			<< exceptionObj.getErrorCode() << ".";
		return;
	}

	indexCollObj = nodeObj->GetIndexCollection();
	if (indexCollObj->GetNumberofIndexes() == 0)
	{
		boost::format formatter(kMsgObjectDictoryEmpty);
		formatter % nodeId;
		exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1F98");
	if (NULL != indexObj)
	{
		SubIndex *subIndexObj = NULL;
		subIndexObj = indexObj->GetSubIndexbyIndexValue(subIndexId);
		if (NULL != subIndexObj)
		{
			if (PRES_DEFAULT_PAYLOAD > nodeObj->GetPResActPayloadValue())
			{
				IntToAscii(PRES_DEFAULT_PAYLOAD, conValue, 16);
			}
			else
			{
				IntToAscii(nodeObj->GetPResActPayloadValue(), conValue, 16);
			}
			strcpy((char*) conValue, ConvertToUpper((char*) conValue));

			strcpy(actValue, (char*) "0x");
			strcat(actValue, conValue);
			subIndexObj->SetActualValue(actValue);
			indexObj->SetFlagIfIncludedCdc(true);
			subIndexObj->SetFlagIfIncludedCdc(true);

			//set the value in MN
			if (CN == nodeObj->GetNodeType())
			{
				IntToAscii(nodeObj->GetNodeId(), subIndexId, 16);
				subIndexId = PadLeft(subIndexId, '0', 2);

				exceptionObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F8D",
						subIndexId, &subIndexPos, &indexPos);
				if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
				{
					LOG_ERROR() << "IfSubIndexExists() for subIndex 0x1F8D/0x" << subIndexId 
						<< " and node 240 "
						<< " failed with error code " << exceptionObj.getErrorCode() << ".";
					return;
				}
				indexObj = GetMNIndexValues((char *) "1F8D");
				if (NULL != indexObj)
				{
					subIndexObj = indexObj->GetSubIndexbyIndexValue(subIndexId);
					if (NULL != subIndexObj)
					{
						subIndexObj->SetActualValue(actValue);
						indexObj->SetFlagIfIncludedCdc(true);
						subIndexObj->SetFlagIfIncludedCdc(true);
					}

					//00'th sub index is set to FE(a max value) for make to reflect in cdc
					subIndexObj = indexObj->GetSubIndexbyIndexValue(
							(char*) "00");
					if (NULL != subIndexObj)
					{
						strcpy(actValue, (char*) "0xFE"); //to make display of 1f8d in cdc if act != def value
						subIndexObj->SetActualValue(actValue);
					}
				}
				else
				{
					LOG_ERROR() << "Index 0x1F8D does not exist for node 240.";
				}
			}
			else
			{
				LOG_ERROR() << "Processed node is not a CN.";
			}
		}
		else
		{
			LOG_ERROR() << "SubIndex 0x1F98/0x5 does not exist for node " << nodeId << ".";
		}
	}
	else
	{
		LOG_ERROR() << "Index 0x1F98 does not exist for node " << nodeId << ".";
	}
	delete[] subIndexId;
}

void UpdateCNVisibleNode(Node* nodeObj)
{
	ocfmRetCode exceptionObj;

	if (!nodeObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	IndexCollection* pdoIndexCollObj = NULL;
	INT32 tpdoCount = 0;
	INT32 rpdoCount = 0;
	pdoIndexCollObj = nodeObj->getPDOIndexCollection(&tpdoCount, &rpdoCount);

	if (NULL == pdoIndexCollObj)
	{
		LOG_FATAL() << "No PDO-Indices found for node " << nodeObj->GetNodeIndex() << ".";
		return;
	}

	Index* indexObj = NULL;
	Index* commIndexObj = NULL;
	SubIndex* sidxObj1 = NULL;
	SubIndex* sidxObj2 = NULL;
	INT32 crossTxStnCnt = 0;

	ResetAllSubIndexFlag(
			(nodeObj->GetIndexCollection())->GetIndexbyIndexValue(
					(char*) "1F81"));
	ResetAllSubIndexFlag(
			(nodeObj->GetIndexCollection())->GetIndexbyIndexValue(
					(char*) "1F8D"));

	for (INT32 idxLC = 0; idxLC < pdoIndexCollObj->GetNumberofIndexes();
			idxLC++)
	{
		indexObj = pdoIndexCollObj->GetIndexByPosition(idxLC);
		if ((NULL != indexObj)
				&& (0 == strncmp(indexObj->GetIndexValue(), "16", 2)))
		{
			sidxObj1 = indexObj->GetSubIndexbyIndexValue((char*) "00");

			if (NULL != sidxObj1)
			{
				if ((NULL != sidxObj1->GetActualValue())
						&& (0 != strcmp(sidxObj1->GetActualValue(), ""))
						&& !(CheckIfValueZero(
								(char*) sidxObj1->GetActualValue())))
				{
					char* commIdxId = NULL;
					char* indexId = new char[SUBINDEX_LEN];
					commIdxId = new char[INDEX_LEN + 1];
					indexId = SubString(indexId, indexObj->GetIndexValue(), 2, 2);
					strcpy(commIdxId, (char*) "14");
					strcat(commIdxId, indexId);
					delete[] indexId;

					commIndexObj = pdoIndexCollObj->GetIndexbyIndexValue(commIdxId);
					delete[] commIdxId;

					if (NULL != commIndexObj)
					{
						sidxObj2 = commIndexObj->GetSubIndexbyIndexValue(
								(char*) "01");
						if (NULL != sidxObj2)
						{
							if ((NULL != sidxObj2->GetActualValue())
									&& (0
											!= strcmp(
													sidxObj2->GetActualValue(),
													""))
									&& !(CheckIfValueZero(
											(char*) sidxObj2->GetActualValue())))
							{
								crossTxStnCnt++;
								//FIXME: Check whether there are enough receive objects for the cross traffic
								if (MAX_CN_CROSS_TRAFFIC_STN < crossTxStnCnt)
								{
									boost::format formatter(kMsgCrossTrafficStationLimitExceeded);
									formatter % nodeObj->GetNodeId()
										% crossTxStnCnt
										% MAX_CN_CROSS_TRAFFIC_STN;
									exceptionObj.setErrorCode(OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN);
									exceptionObj.setErrorString(formatter.str());
									LOG_FATAL() << formatter.str();
									throw exceptionObj;
								}

								char* mappedNodeId = new char[SUBINDEX_LEN];
								//copy the MN objects
								if (CheckIfHex((char*) sidxObj2->GetActualValue()))
								{
									mappedNodeId = SubString(mappedNodeId, sidxObj2->GetActualValue(), 2, 2);
								}
								else
								{
									strcpy(mappedNodeId, sidxObj2->GetActualValue());
								}

								mappedNodeId = PadLeft(mappedNodeId, '0', 2);

								if (true == IsCNNodeAssignmentValid(nodeObj))
								{
									if (true
											== CopyMNSubindexToCN(nodeObj,
													(char*) "1F81",
													mappedNodeId))
									{
										if (true
												== CopyMNSubindexToCN(nodeObj,
														(char*) "1F8D",
														mappedNodeId))
										{
											//CopyMNSubindexToCN 1F8D Success
										}
										else
										{
											LOG_ERROR() << "Failed copying index 0x1F8D/0x" << mappedNodeId << " to CN.";	
										}
									}
									else
									{
										LOG_ERROR() << "Failed copying index 0x1F81/0x" << mappedNodeId << " to CN.";
									}
								}
								delete[] mappedNodeId;
							}
						}
					}

				}
			}

		}
	}

}

bool CopyMNSubindexToCN(Node* nodeObj, const char* indexId, const char* subIndexId)
{
	bool sidxCopied = false;
	ocfmRetCode exceptionObj;
	IndexCollection* indexCollObj = NULL;
	Index* indexObjMN = NULL;
	Index* indexObjCN = NULL;

	if (!nodeObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	indexCollObj = nodeObj->GetIndexCollection();
	if (indexCollObj->GetNumberofIndexes() == 0)
	{
		boost::format formatter(kMsgObjectDictoryEmpty);
		formatter % nodeObj->GetNodeId();
		exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	if ((indexId == NULL) || (subIndexId == NULL))
	{
		boost::format formatter(kMsgNullArgument);
		formatter
		% "'indexId', 'subIndexId'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	indexObjCN = indexCollObj->GetIndexbyIndexValue(indexId);
	indexObjMN = GetMNIndexValues(indexId);
	if ((NULL != indexObjCN) && (NULL != indexObjMN))
	{
		SubIndex* sidxObjMN = NULL;
		SubIndex* sidxObjCN = NULL;
		sidxObjCN = indexObjCN->GetSubIndexbyIndexValue(subIndexId);
		sidxObjMN = indexObjMN->GetSubIndexbyIndexValue(subIndexId);

		if ((NULL == sidxObjCN) || (NULL == sidxObjMN))
		{
			LOG_FATAL() << "Parameters 'sidxObjCN' and 'sidxObjMN'  must not be NULL.";
		}
		else if (NULL != sidxObjMN->GetActualValue())
		{
			sidxObjCN->SetActualValue((char*) sidxObjMN->GetActualValue());
			sidxObjCN->SetFlagIfIncludedCdc(true);
			indexObjCN->SetFlagIfIncludedCdc(true);
			sidxCopied = true;
		}
		else
		{
			LOG_WARN() << "Attribute 'actualValue' not set for 0x" << indexObjMN->GetIndexValue() << "/0x" << sidxObjMN->GetIndexValue() << " on node 240.";
		}
	}
	else
	{
		LOG_WARN() << "Index 0x" << indexId << " does not exist on both node 240 and " << nodeObj->GetNodeId() << ".";
	}
	return sidxCopied;
}

void ResetAllSubIndexFlag(Index* indexObj)
{
	if (NULL == indexObj)
	{
		LOG_FATAL() << "Parameter 'indexObj' must not be NULL.";
		return;
	}

	for (INT32 sidxLC = 0; sidxLC < indexObj->GetNumberofSubIndexes(); sidxLC++)
	{
		SubIndex* sidxObj = NULL;
		sidxObj = indexObj->GetSubIndexByPosition(sidxLC);
		if (NULL != sidxObj)
		{
			sidxObj->SetFlagIfIncludedCdc(false);
		}
		else
		{
			LOG_ERROR() << "Subindex at position " << sidxLC << " is NULL.";
		}
	}
	indexObj->SetFlagIfIncludedCdc(false);
}

void ResetAllPdos(INT32 nodeId, NodeType nodeType)
{
	NodeCollection *nodeCollObj = NULL;
	Node *nodeObj = NULL;
	IndexCollection *indexCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
	indexCollObj = nodeObj->GetIndexCollection();

	for (INT32 idxLC = 0; idxLC < indexCollObj->GetNumberofIndexes(); idxLC++)
	{
		Index *indexObj = NULL;
		indexObj = indexCollObj->GetIndexByPosition(idxLC);
		if ((NULL == indexObj) || (NULL == indexObj->GetIndexValue()))
		{
			continue;
		}

		if (!CheckIfNotPDO((char*) indexObj->GetIndexValue()))
		{
			INT32 totalNoOfSidx = 0;
			totalNoOfSidx = indexObj->GetNumberofSubIndexes();

			char* idxIdStr = new char[SUBINDEX_LEN];
			idxIdStr = SubString(idxIdStr, indexObj->GetIndexValue(), 0, 2);
			if ((0 == strcmp(idxIdStr, "1A")) || (0 == strcmp(idxIdStr, "1a"))
					|| (0 == strcmp(idxIdStr, "16")))
			{
				for (INT32 sidxLoopCnt = 0; sidxLoopCnt < totalNoOfSidx;
						sidxLoopCnt++)
				{
					SubIndex *sidxObj = NULL;
					sidxObj = indexObj->GetSubIndexByPosition(sidxLoopCnt);
					if ((NULL == sidxObj) || (NULL == sidxObj->GetIndexValue()))
					{
						continue;
					}
					if ((0 == strcmp(sidxObj->GetIndexValue(), (char*) "00")))
					{
						sidxObj->SetActualValue((char*) "0x0");
					}
					else
					{
						sidxObj->SetActualValue((char*) "0x0000000000000000");
					}

				}
				//get 00 sidx set val 0
				//get all idx set actval 0x000..(16)
			}
			if ((0 == strcmp(idxIdStr, "14")) || (0 == strcmp(idxIdStr, "18")))
			{
				for (INT32 sidxLoopCnt = 0; sidxLoopCnt < totalNoOfSidx;
						sidxLoopCnt++)
				{
					SubIndex *sidxObj = NULL;
					sidxObj = indexObj->GetSubIndexByPosition(sidxLoopCnt);
					if ((NULL == sidxObj) || (NULL == sidxObj->GetIndexValue()))
					{
						continue;
					}
					if ((0 == strcmp(sidxObj->GetIndexValue(), (char*) "00")))
					{
						//Not to set the value for nr.of entries sidx
					}
					else
					{
						sidxObj->SetActualValue((char*) "0x0");
					}
				}
			}
			delete[] idxIdStr;
		}
	}
}

void GetIndexData(Index* indexObj, char* cdcBuffer)
{
	if ((indexObj == NULL) || (cdcBuffer == NULL))
	{
		boost::format formatter(kMsgNullArgument);
		formatter
		% "'indexObj', 'cdcBuffer'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	bool isStringDT = false;
	//Get the Index Value

	strcpy(cdcBuffer, "");
	if ((0 == indexObj->GetNumberofSubIndexes()))
	{
		if (NULL != indexObj->GetActualValue())
		{
			if (true == IsDefaultActualNotEqual(indexObj))
			{
				strcpy(cdcBuffer, indexObj->GetIndexValue());
				strcat(cdcBuffer, "\t");

				//There are no subindexes, So add "00"
				strcat(cdcBuffer, "00");
				strcat(cdcBuffer, "\t");

				DataType dtObj;
				INT32 padLen = 0;
				char* dataSizeStr = new char[8 + STR_ALLOC_BUFFER];

				dtObj = indexObj->GetDataType();
				if (NULL != dtObj.dataTypeName)
				{
					if (!CheckIfStringDatatypes(dtObj.dataTypeName))
					{
						dataSizeStr = IntToAscii(dtObj.dataSize, dataSizeStr,
								16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLen = dtObj.dataSize * 2;
						isStringDT = false;
					}
					else
					{
						INT32 actValLen = strlen(indexObj->GetActualValue());
						dataSizeStr = IntToAscii(actValLen, dataSizeStr, 16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLen = actValLen * 2;
						isStringDT = true;
					}
				}
				else
				{
					//txt2cdc will not be successful
					strcat(cdcBuffer, "00000000");
				}
				delete[] dataSizeStr;

				strcat(cdcBuffer, "\t");

				char actValue[64];
				actValue[0] = '\0';
				if (isStringDT)
				{
					strcpy(actValue, (char*) indexObj->GetActualValue());
					strcpy(actValue, ConvertStringToHex((char*) actValue));
					strcat(cdcBuffer, actValue);
				}
				else
				{
					if (CheckIfHex((char*) indexObj->GetActualValue()))
					{
						INT32 actValLen = strlen(
								(char*) indexObj->GetActualValue());
						strncpy(actValue, (indexObj->GetActualValue() + 2),
								actValLen - 2);
						actValue[actValLen - 2] = '\0';
						strcat(cdcBuffer, PadLeft(actValue, '0', padLen));
					}
					else
					{
						strcpy(actValue,
								IntToAscii(atoi(indexObj->GetActualValue()),
										actValue, 16));
						strcat(cdcBuffer, PadLeft(actValue, '0', padLen));
					}
				}
				strcat(cdcBuffer, "\n");
			}
			else
			{
				//Default&Actual values are same. So not written to CDC
			}
		}
		else
		{
			//Actual value not configured.
		}
	}
	else
	{
		// If Subobjects presents 
		INT32 noOfSubIndexes = 0;
		INT32 noOfTotalSubIndexes = 0;
		INT32 noOfValidSubIndexes = 0;
		bool idxAdded = false;
		bool resetValueAdded = false;
		bool noOfEnteriesAdded = false;
		bool mappingPDO = false;
		SubIndex* sidxObj = NULL;

		mappingPDO = CheckIfMappingPDO((char*) indexObj->GetIndexValue());
		noOfTotalSubIndexes = indexObj->GetNumberofSubIndexes();
		sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
		if (sidxObj != NULL)
		{
			if (sidxObj->GetActualValue() != NULL)
			{
				if (CheckIfHex((char*) sidxObj->GetActualValue()))
				{
					INT32 sidxLen = strlen(sidxObj->GetActualValue());
					char* sidxActVal = new char[sidxLen];
					SubString(sidxActVal, sidxObj->GetActualValue(), 2, (sidxLen - 2));
					noOfSubIndexes = (INT32) HexToInt(sidxActVal);
					delete[] sidxActVal;
				}
				else
				{
					noOfSubIndexes = atoi(sidxObj->GetActualValue());
				}

				if (0 == noOfSubIndexes)
				{
					return;
				}

				noOfSubIndexes = noOfSubIndexes + 1;
			}
			else
			{
				//If actual value is not configured. Take Default value. Anyhow default value will be NumberofSubindexes.
				noOfSubIndexes = noOfTotalSubIndexes;
			}
		}
		else
		{
			//00th subindex not present.
			noOfSubIndexes = noOfTotalSubIndexes;
		}

		if (mappingPDO)
		{
			if (NULL == sidxObj->GetActualValue())
			{
				noOfSubIndexes = noOfTotalSubIndexes + 1;
			}
		}
		LOG_DEBUG() << "No. of subIndices " << noOfSubIndexes << ".";
		for (INT32 sidxLC = 0; sidxLC < noOfSubIndexes; sidxLC++)
		{
			bool includeAccess = false;

			sidxObj = indexObj->GetSubIndexByPosition(sidxLC);
			LOG_DEBUG() << "Processing index 0x" << indexObj->GetIndexValue() << "/0x" << sidxObj->GetIndexValue() << ".";
			includeAccess = CheckAccessTypeForInclude(
					(char*) sidxObj->GetAccessType());

			if ((sidxObj->GetActualValue() != NULL)
					&& (sidxObj->GetFlagIfIncludedCdc() == true)
					&& ((true == includeAccess) || (true == mappingPDO))
					&& (true == IsDefaultActualNotEqual(sidxObj)))
			{
				noOfValidSubIndexes = noOfValidSubIndexes + 1;

				if (noOfValidSubIndexes == noOfSubIndexes)
				{
					sidxLC = noOfTotalSubIndexes - 1;
				}

				if (mappingPDO)
				{
					if (0 == strcmp((char*) sidxObj->GetIndexValue(), "00"))
					{
						continue;
					}
					if (0 == GetDecimalValue((char*) sidxObj->GetActualValue()))
					{
						if ((NULL == sidxObj->GetDefaultValue())
							|| (0 == GetDecimalValue((char*) sidxObj->GetDefaultValue())))
						{
							continue;
						}
						continue; //non-Zero actual values should NOT be added
					}
				}

				if (idxAdded)
				{
					strcat(cdcBuffer, indexObj->GetIndexValue());
				}
				else
				{
					strcpy(cdcBuffer, indexObj->GetIndexValue());
					idxAdded = true;
				}

				//Place a tab
				strcat(cdcBuffer, "\t");

				strcat(cdcBuffer, sidxObj->GetIndexValue());
				strcat(cdcBuffer, "\t");
				//Add datatype
				DataType dtObj;
				dtObj = sidxObj->GetDataType();
				INT32 padLength = 0;
				char* dataSizeStr = new char[8 + STR_ALLOC_BUFFER];

				if (NULL != dtObj.dataTypeName)
				{
					if (!CheckIfStringDatatypes(dtObj.dataTypeName))
					{
						dataSizeStr = IntToAscii(dtObj.dataSize, dataSizeStr,
								16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLength = dtObj.dataSize * 2;
						isStringDT = false;
					}
					else
					{
						INT32 actualValLen = strlen(sidxObj->GetActualValue());
						dataSizeStr = IntToAscii(actualValLen, dataSizeStr, 16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLength = actualValLen * 2;
						isStringDT = true;
					}
				}
				else
				{
					strcat(cdcBuffer, "00000000");
				}

				delete[] dataSizeStr;
				strcat(cdcBuffer, "\t");

				// Add the reset value for that Index,SubIndex
				if ((0 == strcmp(sidxObj->GetIndexValue(), "00")) && mappingPDO
						&& (false == resetValueAdded))
				{
					char actValue[20];
					strcpy(actValue, "0");
					strcat(cdcBuffer, PadLeft(actValue, '0', padLength));
					resetValueAdded = true;
				}
				else
				{
					char actValue[64];
					actValue[0] = '\0';
					if (isStringDT)
					{
						strcpy(actValue, (char*) sidxObj->GetActualValue());
						strcpy(actValue, ConvertStringToHex((char*) actValue));
						strcat(cdcBuffer, actValue);
					}
					else
					{
						if (CheckIfHex((char*) sidxObj->GetActualValue()))
						{
							INT32 len = strlen(
									(char*) sidxObj->GetActualValue());
							strncpy(actValue, (sidxObj->GetActualValue() + 2),
									len - 2);
							actValue[len - 2] = '\0';

							strcat(cdcBuffer,
									PadLeft(actValue, '0', padLength));
						}
						else
						{
							strcpy(actValue,
									IntToAscii(atoi(sidxObj->GetActualValue()),
											actValue, 16));
							strcat(cdcBuffer,
									PadLeft(actValue, '0', padLength));
						}
					}
				}

				strcat(cdcBuffer, "\n");
				if ((0 == sidxLC) && mappingPDO && (true == noOfEnteriesAdded))
				{
					sidxLC = noOfTotalSubIndexes - 1;
				}
				if ((sidxLC == noOfTotalSubIndexes - 1) && mappingPDO
						&& (false == noOfEnteriesAdded)
						&& (true == resetValueAdded))
				{
					sidxLC = -1;
					noOfEnteriesAdded = true;
				}
			}
		}

	}
}

void BRSpecificGetIndexData(Index* indexObj, char* cdcBuffer, INT32 nodeId)
{
	if ((indexObj == NULL) || (cdcBuffer == NULL))
	{
		boost::format formatter(kMsgNullArgument);
		formatter
		% "'indexObj', 'cdcBuffer'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	bool isStringDT = false;
	//Get the Index Value

	strcpy(cdcBuffer, "");
	if ((0 == indexObj->GetNumberofSubIndexes()))
	{
		if (NULL != indexObj->GetActualValue())
		{
			if (true == IsDefaultActualNotEqual(indexObj))
			{
				strcpy(cdcBuffer, indexObj->GetIndexValue());
				strcat(cdcBuffer, "\t");

				//There are no subindexes, So add "00"
				strcat(cdcBuffer, "00");
				strcat(cdcBuffer, "\t");

				DataType dtObj;
				INT32 padLen = 0;
				char* dataSizeStr = new char[8 + STR_ALLOC_BUFFER];

				dtObj = indexObj->GetDataType();
				if (NULL != dtObj.dataTypeName)
				{
					if (!CheckIfStringDatatypes(dtObj.dataTypeName))
					{
						dataSizeStr = IntToAscii(dtObj.dataSize, dataSizeStr,
								16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLen = dtObj.dataSize * 2;
						isStringDT = false;
					}
					else
					{
						INT32 actValLen = strlen(indexObj->GetActualValue());
						dataSizeStr = IntToAscii(actValLen, dataSizeStr, 16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLen = actValLen * 2;
						isStringDT = true;
					}
				}
				else
				{
					//txt2cdc will not be successful
					strcat(cdcBuffer, "00000000");
				}
				delete[] dataSizeStr;

				strcat(cdcBuffer, "\t");

				char actValue[64];
				actValue[0] = '\0';
				if (isStringDT)
				{
					strcpy(actValue, (char*) indexObj->GetActualValue());
					strcpy(actValue, ConvertStringToHex((char*) actValue));
					strcat(cdcBuffer, actValue);
				}
				else
				{
					if (CheckIfHex((char*) indexObj->GetActualValue()))
					{
						INT32 actValLen = strlen(
								(char*) indexObj->GetActualValue());
						strncpy(actValue, (indexObj->GetActualValue() + 2),
								actValLen - 2);
						actValue[actValLen - 2] = '\0';
						strcat(cdcBuffer, PadLeft(actValue, '0', padLen));
					}
					else
					{
						strcpy(actValue,
								IntToAscii(atoi(indexObj->GetActualValue()),
										actValue, 16));
						strcat(cdcBuffer, PadLeft(actValue, '0', padLen));
					}
				}
				strcat(cdcBuffer, "\n");
			}
			else
			{
				//Default&Actual values are same. So not written to CDC
			}
		}
		else
		{
			//Actual value not configured.
		}
	}
	else
	{
		// If Subobjects presents 
		INT32 noOfSubIndexes = 0;
		INT32 noOfTotalSubIndexes = 0;
		INT32 noOfValidSubIndexes = 0;
		bool idxAdded = false;
		bool resetValueAdded = false;
		bool noOfEnteriesAdded = false;
		bool mappingPDO = false;
		SubIndex* sidxObj = NULL;
		char tempNodeid[10];

		mappingPDO = CheckIfMappingPDO((char*) indexObj->GetIndexValue());
		noOfTotalSubIndexes = indexObj->GetNumberofSubIndexes();
		sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
		if (sidxObj != NULL)
		{
			if (sidxObj->GetActualValue() != NULL)
			{
				if (CheckIfHex((char*) sidxObj->GetActualValue()))
				{
					INT32 sidxActLen = strlen(sidxObj->GetActualValue());
					char* sidxActVal = new char[sidxActLen];
					SubString(sidxActVal, sidxObj->GetActualValue(), 2, (sidxActLen - 2));
					noOfSubIndexes = (INT32) HexToInt(sidxActVal);
					delete[] sidxActVal;
				}
				else
				{
					noOfSubIndexes = atoi(sidxObj->GetActualValue());
				}

				if (0 == noOfSubIndexes)
				{
					return;
				}

				noOfSubIndexes = noOfSubIndexes + 1;
			}
			else
			{
				//If actual value is not configured. Take Default value. Anyhow default value will be NumberofSubindexes.
				noOfSubIndexes = noOfTotalSubIndexes;
			}
		}
		else
		{
			//00th subindex not present.
			noOfSubIndexes = noOfTotalSubIndexes;
		}

		if (mappingPDO)
		{
			if (NULL == sidxObj->GetActualValue())
			{
				noOfSubIndexes = noOfTotalSubIndexes + 1;
			}
		}

		if ((0 == strcmp(indexObj->GetIndexValue(), (char*) "1F81")))
		{
			IntToAscii(nodeId, tempNodeid, 16);
			PadLeft(tempNodeid, '0', 2);
		}
		LOG_DEBUG() << "No. of subIndices: " << noOfTotalSubIndexes << ".";
		for (INT32 sidxLC = 0; sidxLC < noOfTotalSubIndexes; sidxLC++)
		{
			bool includeAccess = false;
			sidxObj = indexObj->GetSubIndexByPosition(sidxLC);
			LOG_DEBUG() << "Processing index 0x" << indexObj->GetIndexValue() << "/0x" << sidxObj->GetIndexValue() << ".";
			includeAccess = CheckAccessTypeForInclude(
					(char*) sidxObj->GetAccessType());

			if ((sidxObj->GetActualValue() != NULL)
					&& (sidxObj->GetFlagIfIncludedCdc() == true)
					&& ((true == includeAccess) || (true == mappingPDO))
					&& (true == IsDefaultActualNotEqual(sidxObj)))
			{
				if ((0 == strcmp(indexObj->GetIndexValue(), (char*) "1F81")))
				{
					if ((0
							== strcmp(sidxObj->GetIndexValue(),
									(const char*) "00"))
							|| (0
									== strcmp(sidxObj->GetIndexValue(),
											tempNodeid)))
					{
						continue;
					}
					}
				noOfValidSubIndexes = noOfValidSubIndexes + 1;

				if (noOfValidSubIndexes == noOfSubIndexes)
				{
					sidxLC = noOfTotalSubIndexes - 1;
				}

				if (mappingPDO)
				{
					if (0 == strcmp((char*) sidxObj->GetIndexValue(), "00"))
					{
						continue;
					}
					if (0 == GetDecimalValue((char*) sidxObj->GetActualValue()))
					{
						if ((NULL == sidxObj->GetDefaultValue())
								|| (0
										== GetDecimalValue(
												(char*) sidxObj->GetDefaultValue())))
						{
							continue;
						}
						continue; //non-Zero actual values should NOT be added
					}
				}

				if (idxAdded)
				{
					strcat(cdcBuffer, indexObj->GetIndexValue());
				}
				else
				{
					strcpy(cdcBuffer, indexObj->GetIndexValue());
					idxAdded = true;
				}

				//Place a tab
				strcat(cdcBuffer, "\t");

				strcat(cdcBuffer, sidxObj->GetIndexValue());
				strcat(cdcBuffer, "\t");
				//Add datatype
				DataType dtObj;
				dtObj = sidxObj->GetDataType();
				INT32 padLength = 0;
				char* dataSizeStr = new char[8 + STR_ALLOC_BUFFER];

				if (NULL != dtObj.dataTypeName)
				{
					if (!CheckIfStringDatatypes(dtObj.dataTypeName))
					{
						dataSizeStr = IntToAscii(dtObj.dataSize, dataSizeStr,
								16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLength = dtObj.dataSize * 2;
						isStringDT = false;
					}
					else
					{
						INT32 actualValLen = strlen(sidxObj->GetActualValue());
						dataSizeStr = IntToAscii(actualValLen, dataSizeStr, 16);
						dataSizeStr = PadLeft(dataSizeStr, '0', 8);
						strcat(cdcBuffer, dataSizeStr);
						padLength = actualValLen * 2;
						isStringDT = true;
					}
				}
				else
				{
					strcat(cdcBuffer, "00000000");
				}

				delete[] dataSizeStr;
				strcat(cdcBuffer, "\t");

				// Add the reset value for that Index,SubIndex
				if ((0 == strcmp(sidxObj->GetIndexValue(), "00")) && mappingPDO
						&& (false == resetValueAdded))
				{
					char actValue[20];
					strcpy(actValue, "0");
					strcat(cdcBuffer, PadLeft(actValue, '0', padLength));
					resetValueAdded = true;
				}
				else
				{
					char actValue[64];
					actValue[0] = '\0';
					if (isStringDT)
					{
						strcpy(actValue, (char*) sidxObj->GetActualValue());
						strcpy(actValue, ConvertStringToHex((char*) actValue));
						strcat(cdcBuffer, actValue);
					}
					else
					{
						if (CheckIfHex((char*) sidxObj->GetActualValue()))
						{
							INT32 actualValLen = strlen(
									(char*) sidxObj->GetActualValue());
							strncpy(actValue, (sidxObj->GetActualValue() + 2),
									actualValLen - 2);
							actValue[actualValLen - 2] = '\0';

							strcat(cdcBuffer,
									PadLeft(actValue, '0', padLength));
						}
						else
						{
							strcpy(actValue,
									IntToAscii(atoi(sidxObj->GetActualValue()),
											actValue, 16));
							strcat(cdcBuffer,
									PadLeft(actValue, '0', padLength));
						}
					}
				}

				strcat(cdcBuffer, "\n");
				if ((0 == sidxLC) && mappingPDO && (true == noOfEnteriesAdded))
				{
					sidxLC = noOfTotalSubIndexes - 1;
				}
				if ((sidxLC == noOfTotalSubIndexes - 1) && mappingPDO
						&& (false == noOfEnteriesAdded)
						&& (true == resetValueAdded))
				{
					sidxLC = -1;
					noOfEnteriesAdded = true;
				}
			}
		}

	}
}

void WriteCNsData(char* fileName)
{
	ocfmRetCode exceptionObj;
	NodeCollection* nodeCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	assert(nodeCollObj);

	INT32 len = 0;
	char* cdcBuffer2 = NULL;
	char* mainBuffer = NULL;
	char* tempStr = new char[50];
	INT32 cnCount = 0;
	FILE* fileptr = new FILE();
	Node nodeObj;
	IndexCollection* indexCollObj = NULL;

	for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes(); nodeLC++)
	{
		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);
		if (CN == nodeObj.GetNodeType())
		{
			if (NULL == (fileptr = fopen(fileName, "a+")))
			{
				//cout << "Problem" <<endl;
			}

			//AddOtherRequiredCNIndexes(objNode.getNodeId());
			indexCollObj = nodeObj.GetIndexCollection();
			if (indexCollObj->GetNumberofIndexes() == 0)
			{
				boost::format formatter(kMsgObjectDictoryEmpty);
				formatter % nodeObj.GetNodeId();
				exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
				exceptionObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				delete fileptr;
				throw exceptionObj;
			}

			char* commentStr = new char[50];
			tempStr = IntToAscii(cnCount + 1, tempStr, 10);
			cnCount++;

			strcpy(commentStr, "////Configuration Data for CN-");
			commentStr = strcat(commentStr, tempStr);
			commentStr = strcat(commentStr, "\n");
			len = strlen(commentStr);
			if ((fwrite(commentStr, sizeof(char), len, fileptr)) != 0)
			{
				fclose(fileptr);
			}

			delete[] commentStr;

			Index* indexObj = NULL;
			char* cdcBuffer3 = NULL;

			/*************WRITE MN'S 1006,1020 Indexes Values *******************************/

			// David Puffer: Indices 0x1006, 0x1F98/7-8 have to be distributed to all nodes in the network.
			// If there is no actualValue configured for these indices, CopyMNPropDefToAct() will copy default to actual.
			// This causes problems on project-load, therefore this logic was moved here.
			indexObj = GetMNIndexValues((char*) "1006");
			if (NULL != indexObj)
			{
				if (indexObj->GetActualValue() != NULL)
				{
					UpdateCNCycleTime(indexCollObj, (char*) indexObj->GetActualValue());
				}
				else if (indexObj->GetDefaultValue() != NULL)
				{
					UpdateCNCycleTime(indexCollObj, (char*) indexObj->GetDefaultValue());
				}
				else
				{
					LOG_ERROR() << "Neither attribute 'actualValue', nor 'defaultValue' exist for index 0x1006 on node 240.";
				}
			}
			else
			{
				LOG_ERROR() << "Index 0x1006 does not exist on node 240.";
			}

			indexObj = GetMNIndexValues((char*) "1C14");
			if (NULL != indexObj)
			{
				if (NULL != (char*) indexObj->GetActualValue())
				{
					UpdateCNSoCTolerance(indexCollObj,
							(char*) indexObj->GetActualValue());
				}
				else
				{
					LOG_ERROR() << "Attribute 'actualValue' not set for 0x" << indexObj->GetIndexValue() << "on node 240.";
				}
			}
			else
			{
				LOG_ERROR() << "Index 0x1C14 does not exist for node 240.";
			}

			indexObj = GetMNIndexValues((char*) "1F26");
			if (NULL != indexObj)
			{
				UpdatedCNDateORTime(indexObj, nodeObj.GetNodeId(), DATE);
			}
			else
			{
				LOG_ERROR() << "Index 0x1F26 does not exist for node 240.";
			}

			indexObj = GetMNIndexValues((char*) "1F27");
			if (NULL != indexObj)
			{
				UpdatedCNDateORTime(indexObj, nodeObj.GetNodeId(), TIME);
			}
			else
			{
				LOG_ERROR() << "Index 0x1F27 does not exist for node 240.";
			}

			indexObj = GetMNIndexValues((char*) "1F98");
			if (NULL != indexObj)
			{
				SubIndex* subIndexObj = GetMNSubIndexValues((char*) "1F98", (char*) "08");
				if (subIndexObj != NULL)
				{
					if (subIndexObj->GetActualValue() != NULL)
					{
						UpdateCNAsyncMTUsize(indexCollObj, (char*) subIndexObj->GetActualValue());
					}
					else if (subIndexObj->GetDefaultValue() != NULL)
					{
						UpdateCNAsyncMTUsize(indexCollObj, (char*) subIndexObj->GetDefaultValue());
					}
					else
					{
						LOG_ERROR() << "Neither attribute 'actualValue', nor 'defaultValue' exist for index 0x1F98/0x8 on node 240.";
					}
				}
				else
				{
					LOG_ERROR() << "SubIndex 0x1F98/0x8 does not exist on node 240.";
				}

				subIndexObj = GetMNSubIndexValues((char*) "1F98", (char*) "07");
				if (subIndexObj != NULL)
				{
					if (subIndexObj->GetActualValue() != NULL)
					{
						UpdateCNMultiPrescal(indexCollObj, (char*) subIndexObj->GetActualValue());
					}
					else if (subIndexObj->GetDefaultValue() != NULL)
					{
						UpdateCNMultiPrescal(indexCollObj, (char*) subIndexObj->GetDefaultValue());
					}
					else
					{
						LOG_ERROR() << "Neither attribute 'actualValue', nor 'defaultValue' exist for index 0x1F98/07 on node 240."; 
					}
				}
				else
				{
					LOG_ERROR() << "SubIndex 0x1F98/0x7 does not exist on node 240.";
				}


				if ((NULL != subIndexObj)
						&& (NULL != subIndexObj->GetActualValue()))
				{
					UpdateCNMultiPrescal(indexCollObj,
							(char*) subIndexObj->GetActualValue());
				}
				else
				{
					LOG_ERROR() << "Subindex 0x1F98/0x7 does not exist for node 240 or its attribute 'actualValue' is not set.";
				}
			}
			else
			{
				LOG_ERROR() << "Index 0x1F98 does not exist for node 240.";
			}

			indexObj = GetMNIndexValues((char*) "1F9B");
			if (NULL != indexObj)
			{
				UpdateCNMultipleCycleAssign(&nodeObj);
			}
			else
			{
				LOG_ERROR() << "Index 0x1F9B does not exist for node 240.";
			}

			UpdateCNVisibleNode(&nodeObj);
			UpdateCNPresMNActLoad(&nodeObj);

			cdcBuffer3 = new char[30000];
			cdcBuffer2 = new char[60000];
			strcpy(cdcBuffer2, "");
			strcpy(cdcBuffer3, "");

			char* noOfEnteries = new char[10];
			//workaround for B&R Bus Controller stack
			//NoOfenteries = _IntToAscii(getNodeTotalIndexSubIndex(objNode.getNodeId()), NoOfenteries, 16);
			noOfEnteries = IntToAscii(
					BRSpecificgetCNsTotalIndexSubIndex(nodeObj.GetNodeId()),
					noOfEnteries, 16);
			/*1 is not added for the size*/
			noOfEnteries = PadLeft(noOfEnteries, '0', 8);
			noOfEnteries = ConvertToUpper(noOfEnteries);
			strcpy(cdcBuffer3, noOfEnteries);
			strcat(cdcBuffer3, "\n");
			strcpy(cdcBuffer2, cdcBuffer3);
			delete[] noOfEnteries;

			////workaround for B&R Bus Controller stack
			//FormatCdc(objIndexCollection, Buffer4, fileptr, CN);
			BRSpecificFormatCdc(indexCollObj, cdcBuffer3, fileptr, CN,
					nodeObj.GetNodeId());
			strcat(cdcBuffer2, cdcBuffer3);
			delete[] cdcBuffer3;

			//Convert CN NodeID to Hex
			IntToAscii(nodeObj.GetNodeId(), tempStr, 16);
			char* tempStr2 = new char[50];
			strcpy(tempStr2, "1F22\t");
			tempStr = PadLeft(tempStr, '0', 2);

			///write CN-n NodeID  in the next to 1F22
			strcat(tempStr2, tempStr);
			strcat(tempStr2, "\t");

			//write the size of CN-n Buffer
			INT32 dataLenCN = GetCNDataLen(cdcBuffer2);

			//Convert length to Hex
			IntToAscii(dataLenCN, tempStr, 16);
			//printf("c%s",tempStr);

			tempStr = PadLeft(tempStr, '0', 8);
			strcat(tempStr2, tempStr);

			// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
			mainBuffer = new char[strlen(cdcBuffer2) + 50];
			strcpy(mainBuffer, tempStr2);
			strcat(mainBuffer, "\n");
			strcat(mainBuffer, cdcBuffer2);
			delete[] tempStr2;
			delete[] cdcBuffer2;

			//write all CNs data in the file
			dataLenCN = strlen(mainBuffer);
			if (NULL == (fileptr = fopen(fileName, "a+")))
			{
				//cout << "Problem" <<endl;
			}
			if (0 != (fwrite(mainBuffer, sizeof(char), dataLenCN, fileptr)))
			{
				fclose(fileptr);
			}
			delete[] mainBuffer;
		}

	}
	delete[] tempStr;
}

INT32 GetNodeTotalIndexSubIndex(INT32 nodeId)
{
	NodeCollection *nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node nodeObj;
	INT32 noOfCDCEntries = 0;
	IndexCollection *indexCollObj = NULL;

	if (MN_NODEID == nodeId)
	{
		/* Number of 1F81 enteries is twice the number of CN's*/
		noOfCDCEntries = 2 * nodeCollObj->GetCNNodesCount();
		/* include the number of CN's*/
		noOfCDCEntries = noOfCDCEntries + nodeCollObj->GetCNNodesCount();
	}

	nodeObj = nodeCollObj->GetNode(nodeId);

	indexCollObj = nodeObj.GetIndexCollection();

	for (INT32 indexLC = 0; indexLC < indexCollObj->GetNumberofIndexes();
			indexLC++)
	{

		Index *indexObj = NULL;
		indexObj = indexCollObj->GetIndexByPosition(indexLC);

		if ((indexObj->GetFlagIfIncludedCdc() == true)
			&& (true == CheckAccessTypeForInclude((char*) indexObj->GetAccessType())
			|| CheckIfMappingPDO((char*) indexObj->GetIndexValue())))
		{
			if (indexObj->GetNumberofSubIndexes() == 0)
			{
				if ((indexObj->GetActualValue() != NULL)
					&& (true == IsDefaultActualNotEqual(indexObj)))
				{
					LOG_DEBUG() << "Index 0x" << indexObj->GetIndexValue() 
						<< ": 'actualValue': " << indexObj->GetActualValue() 
						<< " 'defaultValue': " << indexObj->GetDefaultValue() << ".";
					noOfCDCEntries = noOfCDCEntries + 1;
				}
			}
			else
			{
				LOG_DEBUG() << "Processing index 0x" << indexObj->GetIndexValue() << ".";
				if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
				{
					SubIndex* sidxObj = NULL;
					sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");

					if ((NULL != sidxObj) && (NULL != sidxObj->GetActualValue())
						&& (0 != strcmp(sidxObj->GetActualValue(), "")))
					{
						if (true == ReactivateMappingPDO(indexCollObj, indexObj)
							|| (true == IsDefaultActualNotEqual(sidxObj)))
						{
							noOfCDCEntries = noOfCDCEntries + 1; /* to initalize 00 entry subindex */
						}
						if (CheckIfValueZero((char*) sidxObj->GetActualValue()))
						{
							continue;
						}
						if (true == ReactivateMappingPDO(indexCollObj, indexObj)
							|| (true == IsDefaultActualNotEqual(sidxObj)))
						{
							noOfCDCEntries = noOfCDCEntries + 1; /* to reinitalize 00 entry subindex */
						}
						INT32 noValidMappings = 0;
						if (CheckIfHex((char*) sidxObj->GetActualValue()))
						{
							INT32 sidxActLen = strlen(sidxObj->GetActualValue());
							char* sidxActVal = new char[sidxActLen];
							SubString(sidxActVal, sidxObj->GetActualValue(), 2, (sidxActLen - 2));
							noValidMappings = HexToInt(sidxActVal);
							delete[] sidxActVal;
						}
						else
						{
							noValidMappings = atoi(sidxObj->GetActualValue());
						}

						for (INT32 sidxLC = 1; sidxLC <= noValidMappings; sidxLC++)
						{
							SubIndex* sidxObjTemp = NULL;
							char *sidxId = new char[SUBINDEX_LEN];
							sidxId = IntToAscii(sidxLC, sidxId, 16);
							sidxId = PadLeft(sidxId, '0', 2);
							sidxObjTemp = indexObj->GetSubIndexbyIndexValue(sidxId);
							delete[] sidxId;
							if ((sidxObjTemp != NULL)
								&& (sidxObjTemp->GetActualValue() != NULL)
								&& (true == sidxObjTemp->GetFlagIfIncludedCdc())
								&& (true == IsDefaultActualNotEqual(sidxObjTemp)))
							{
								if (0 == GetDecimalValue((char*) sidxObjTemp->GetActualValue()))
								{
									if ((NULL == sidxObjTemp->GetDefaultValue())
										|| (0 == GetDecimalValue((char*) sidxObjTemp->GetDefaultValue())))
									{
										continue;
									}
								}
								noOfCDCEntries = noOfCDCEntries + 1;
							}
						}
					}
					continue;
				}

				if (MN_NODEID == nodeId)
				{
					if (0 == strcmp((char*) indexObj->GetIndexValue(), "1F81"))
					{
						SubIndex* sidxObj;
						sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "F0");
						if (NULL != sidxObj && sidxObj->GetActualValue() != NULL
							&& 0 != strcmp((char*) sidxObj->GetActualValue(), "")
							&& true == CheckAccessTypeForInclude((char*) sidxObj->GetAccessType())
							&& true == IsDefaultActualNotEqual(sidxObj))
						{
							noOfCDCEntries = noOfCDCEntries + 1;
						}
						continue;
					}
				}

				SubIndex* sidxObj = NULL;
				sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
				if ((NULL != sidxObj) && (NULL != sidxObj->GetActualValue())
					&& (0 != strcmp(sidxObj->GetActualValue(), "")))
				{
					if (CheckIfValueZero((char*) sidxObj->GetActualValue()))
					{
						continue;
					}
				}

				for (INT32 sidxLC = 0;
					sidxLC < indexObj->GetNumberofSubIndexes(); sidxLC++)
				{
					if ((indexObj->GetSubIndexByPosition(sidxLC)->GetActualValue() != NULL)
						&& (true == indexObj->GetSubIndexByPosition(sidxLC)->GetFlagIfIncludedCdc())
						&& (true == CheckAccessTypeForInclude((char*) indexObj->GetSubIndexByPosition(
										sidxLC)->GetAccessType()))
						&& (true == IsDefaultActualNotEqual(indexObj->GetSubIndexByPosition(sidxLC))))
					{
						noOfCDCEntries = noOfCDCEntries + 1;
					}
				}
			}
		}
	}
	return noOfCDCEntries;
}

INT32 BRSpecificgetCNsTotalIndexSubIndex(INT32 nodeId)
{
	NodeCollection* nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node nodeObj;
	INT32 noOfCDCEntries = 0;
	IndexCollection* indexCollObj = NULL;
	char tempNodeId[10];
	IntToAscii(nodeId, tempNodeId, 16);
	PadLeft(tempNodeId, '0', 2);

	nodeObj = nodeCollObj->GetNode(nodeId);
	indexCollObj = nodeObj.GetIndexCollection();
	for (INT32 indexLC = 0; indexLC < indexCollObj->GetNumberofIndexes();
			indexLC++)
	{

		Index* indexObj = NULL;
		indexObj = indexCollObj->GetIndexByPosition(indexLC);
		
		if (indexObj->GetFlagIfIncludedCdc() == true
			&& (true == CheckAccessTypeForInclude((char*) indexObj->GetAccessType())
			|| CheckIfMappingPDO((char*) indexObj->GetIndexValue())))
		{
			if (indexObj->GetNumberofSubIndexes() == 0)
			{
				if (indexObj->GetActualValue() != NULL
					&& true == IsDefaultActualNotEqual(indexObj))
				{
					noOfCDCEntries = noOfCDCEntries + 1;
				}
			}
			else
			{
				if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
				{
					
					SubIndex* sidxObj = NULL;
					sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");

					//actual value checked for non-Zero
					if ((NULL != sidxObj) && (NULL != sidxObj->GetActualValue())
						&& (0 != strcmp(sidxObj->GetActualValue(), ""))
						&& !(CheckIfValueZero((char*) sidxObj->GetActualValue())))
					{
						if (true == ReactivateMappingPDO(indexCollObj, indexObj)
							|| true == IsDefaultActualNotEqual(sidxObj))
						{
							noOfCDCEntries = noOfCDCEntries + 1; /* to initalize 00 entry subindex */
						}

						if (true == ReactivateMappingPDO(indexCollObj, indexObj)
							|| true == IsDefaultActualNotEqual(sidxObj))
						{
							noOfCDCEntries = noOfCDCEntries + 1; /* to reinitalize 00 entry subindex */
						}
						//Rule: In 1A00 and 1600 the mappings should be in order(Starting from 01-FE, No skipping of subindex allowed)

						INT32 noValidMappings = 0;
						if (CheckIfHex((char*) sidxObj->GetActualValue()))
						{
							INT32 sidxActLen = strlen(sidxObj->GetActualValue());
							char* sidxActVal = new char[sidxActLen];
							SubString(sidxActVal, sidxObj->GetActualValue(), 2, (sidxActLen - 2));
							noValidMappings = HexToInt(sidxActVal);
							delete[] sidxActVal;
						}
						else
						{
							noValidMappings = atoi(sidxObj->GetActualValue());
						}

						for (INT32 sidxLC = 1; sidxLC <= noValidMappings; sidxLC++)
						{
							SubIndex* sidxObjTemp = NULL;
							char *sidxId = new char[SUBINDEX_LEN];
							sidxId = IntToAscii(sidxLC, sidxId, 16);
							sidxId = PadLeft(sidxId, '0', 2);
							sidxObjTemp = indexObj->GetSubIndexbyIndexValue(sidxId);
							delete[] sidxId;
							if ((NULL != sidxObjTemp)
								&& (sidxObjTemp->GetActualValue() != NULL)
								&& (true == sidxObjTemp->GetFlagIfIncludedCdc())
								&& (true == IsDefaultActualNotEqual(sidxObjTemp)))
							{
								if (0 == GetDecimalValue((char*) sidxObjTemp->GetActualValue()))
								{
									if ((NULL == sidxObjTemp->GetDefaultValue())
										|| (0 == GetDecimalValue((char*) sidxObjTemp->GetDefaultValue())))
									{
										continue;
									}
								}
								noOfCDCEntries = noOfCDCEntries + 1;
							}
						}
					}
					continue;
				}

				SubIndex* sidxObj = NULL;
				sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
				if ((NULL != sidxObj) && (NULL != sidxObj->GetActualValue())
					&& (0 != strcmp(sidxObj->GetActualValue(), "")))
				{
					if (CheckIfValueZero((char*) sidxObj->GetActualValue()))
					{
						continue;
					}
				}

				for (INT32 sidxLC = 0;
					sidxLC < indexObj->GetNumberofSubIndexes(); sidxLC++)
				{
					if (indexObj->GetSubIndexByPosition(sidxLC)->GetActualValue() != NULL
						&& true == indexObj->GetSubIndexByPosition(sidxLC)->GetFlagIfIncludedCdc()
						&& true == CheckAccessTypeForInclude((char*) indexObj->GetSubIndexByPosition(sidxLC)->GetAccessType())
						&& true == IsDefaultActualNotEqual(indexObj->GetSubIndexByPosition(sidxLC)))
					{
						if (0 == strcmp(indexObj->GetIndexValue(), (char*) "1F81"))
						{
							if (0 == strcmp(indexObj->GetSubIndexByPosition(sidxLC)->GetIndexValue(),"00")
								|| 0 == strcmp(indexObj->GetSubIndexByPosition(sidxLC)->GetIndexValue(), tempNodeId))
							{
								continue;
							}
							}
						noOfCDCEntries = noOfCDCEntries + 1;
					}
				}

			}
		}
	}
	return noOfCDCEntries;
}

ocfmRetCode GenerateCDC(const char* cdcPath, const ProjectConfiguration& projectConfiguration)
{	
	LOG_INFO() << "Generating CDC.";
	Node nodeObjMN;
	IndexCollection* indexCollObj;
	char *Buffer1 = NULL;
	char *tempFileName = NULL;
	char *tempOutputFileName = NULL;

	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);
	ocfmRetCode storeExceptionObj;
	INT32 sidxPos = 0;
	INT32 indexPos = 0;

	try
	{
		//get the MN Node object from the NodeCollection
		NodeCollection* objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		assert(objNodeCollection);

		/******************************* Write MN's Indexes ******************************************************************/

		nodeObjMN = objNodeCollection->GetMNNode();
		if (objNodeCollection->GetNumberOfNodes() == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}
		if (nodeObjMN.IsNull())
		{
			exceptionObj.setErrorCode(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			exceptionObj.setErrorString(kMsgNoManagingNodeConfigured);
			LOG_FATAL() << kMsgNoManagingNodeConfigured;
			throw exceptionObj;
		}
		if (objNodeCollection->GetCNNodesCount() == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_CN_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoControlledNodesConfigured);
			LOG_FATAL() << kMsgNoControlledNodesConfigured;
			throw exceptionObj;
		}

		/* Check if autogeneration of MN's Object Dicitonary is set to true */

		//PjtSettings* pjtSettingsObj;
		//pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();

		//if (pjtSettingsObj->GetGenerateAttr() == YES_AG)
		if (projectConfiguration.GetGenerateMNOBD())
		{
			exceptionObj = CheckMutliplexAssigned();
			if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				return exceptionObj;
			}
			/*Generate MNOBD for Auto Generate On*/
			exceptionObj = GenerateMNOBD(true);
			if (OCFM_ERR_EXCESS_CHANNEL == exceptionObj.getErrorCode())
			{
				//Do not throw exception here as we need the process to complete
				storeExceptionObj.setErrorCode(exceptionObj.getErrorCode());
			}
			else if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
			{
				return exceptionObj;
			}
		}


		//if (YES_AG == pjtSettingsObj->GetGenerateAttr())
		if (projectConfiguration.GetGenerateMNOBD())
		{
			for (INT32 nodeLC = 0; nodeLC < objNodeCollection->GetNumberOfNodes();
				nodeLC++)
			{
				Node nodeObjCN;
				nodeObjCN = objNodeCollection->GetNodebyCollectionIndex(nodeLC);
				if (nodeObjCN.GetNodeType() == CN)
				{
					INT32 nodeId = nodeObjCN.GetNodeId();
					char* tempStr = new char[10];
					tempStr = IntToAscii(nodeId, tempStr, 16);
					tempStr = PadLeft(tempStr, '0', 2);

					char* nodeAssignmentBitsStr = GetNodeAssigmentBits(&nodeObjCN);
					char* tempStr2 = new char[strlen(nodeAssignmentBitsStr) + ALLOC_BUFFER + 2];
					sprintf(tempStr2, "0x%s", nodeAssignmentBitsStr);
					exceptionObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F81",
						tempStr, &sidxPos, &indexPos);
					if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
					{
						continue;
					}

					SetSIdxValue((char*) "1F81", tempStr, tempStr2,
						objNodeCollection->GetMNNode().GetIndexCollection(),
						MN_NODEID, MN, false);
					delete[] tempStr;
					delete[] tempStr2;
				}
			}
		}
		//nodeObjMN = objNodeCollection->GetMNNode();
		INT32 totalCNCount = 0;
		totalCNCount = objNodeCollection->GetCNNodesCount();
		UpdateMNNodeAssignmentIndex(&nodeObjMN, totalCNCount, (char*) "1F81",
				true);
		UpdateMNNodeAssignmentIndex(&nodeObjMN, totalCNCount, (char*) "1F92",
				false);
		UpdateMNNodeAssignmentIndex(&nodeObjMN, totalCNCount, (char*) "1F8D",
				true);
		//1c07,1c08,1f22,1f84,1f8e,1f8f to be added
		UpdateMNNodeAssignmentIndex(&nodeObjMN, totalCNCount, (char*) "1F8B",
				true);
		UpdateMNNodeAssignmentIndex(&nodeObjMN, totalCNCount, (char*) "1F26",
				false);
		UpdateMNNodeAssignmentIndex(&nodeObjMN, totalCNCount, (char*) "1F27",
				false);
		UpdateMNNodeAssignmentIndex(&nodeObjMN, totalCNCount, (char*) "1C09",
				true);
		//if (YES_AG == pjtSettingsObj->GetGenerateAttr())
		if (projectConfiguration.GetGenerateMNOBD())
		{
			CalculatePayload();
		}


		FILE* fileptr = new FILE();
		tempFileName = new char[strlen(cdcPath) + 5];
		sprintf(tempFileName, "%s.txt", cdcPath);

		if ((fileptr = fopen(tempFileName, "w+")) == NULL)
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % tempFileName;
			exceptionObj.setErrorCode(OCFM_ERR_CANNOT_OPEN_FILE);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}

		//Buffer1 = (char*)malloc(CDC_BUFFER);
		Buffer1 = new char[CDC_BUFFER];
		char* noOfEntries = new char[10];
		noOfEntries = IntToAscii(GetNodeTotalIndexSubIndex(MN_NODEID),
				noOfEntries, 16);
		noOfEntries = PadLeft(noOfEntries, '0', 8);
		strcpy(Buffer1, noOfEntries);
		strcat(Buffer1, "\n");
		UINT32 len;
		len = strlen(Buffer1);
		delete[] noOfEntries;

		/* Write number of enteries */
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
			LOG_FATAL() << "Error writing CDC-Buffer.";
		}
		delete[] Buffer1;

		Buffer1 = new char[(CDC_BUFFER * objNodeCollection->GetCNNodesCount())];
		memset(Buffer1, 0, (((CDC_BUFFER * objNodeCollection->GetCNNodesCount())) * sizeof(char)));

		GetAllNodeIdAssignment(Buffer1, false);
		strcat(Buffer1, "\n");
		//Write all 1F81 NodeId Assignment
		len = strlen(Buffer1);
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
			LOG_FATAL() << "Error writing CDC-Buffer.";
		}
		delete[] Buffer1;

		fclose(fileptr);

		if ((fileptr = fopen(tempFileName, "a+")) == NULL)
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % tempFileName;
			exceptionObj.setErrorCode(OCFM_ERR_CANNOT_OPEN_FILE);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		nodeObjMN = objNodeCollection->GetMNNode();
		indexCollObj = nodeObjMN.GetIndexCollection();
//cout<<"Write MN CDC"<<endl;

		//Get all the MN's Default Data in Buffer1
		Buffer1 = new char[(CDC_MN_BUFFER * objNodeCollection->GetCNNodesCount())];
		memset(Buffer1, 0, (((CDC_MN_BUFFER * objNodeCollection->GetCNNodesCount())) * sizeof(char)));
		FormatCdc(indexCollObj, Buffer1, fileptr, MN);

		len = strlen(Buffer1);
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
			LOG_FATAL() << "Error writing CDC-Buffer.";
		}
		delete[] Buffer1;
		fclose(fileptr);
//cout<<"Completed writing MN CDC. Starting CN CDC part"<<endl;
		/*************************Write CN's Data in Buffer2***************************************************/
		WriteCNsData((char*) tempFileName);
		//INT32 ret;

		//Write all the IF81 nodeId ReAssignment in Buffer1
		if (objNodeCollection->GetNumberOfNodes() != 0)
		{
			if ((fileptr = fopen(tempFileName, "a+")) == NULL)
			{
				boost::format formatter(kMsgFileReadFailed);
				formatter % tempFileName;
				exceptionObj.setErrorCode(OCFM_ERR_CANNOT_OPEN_FILE);
				exceptionObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw exceptionObj;
			}

			Buffer1 = new char[(CDC_BUFFER * objNodeCollection->GetCNNodesCount())];
			memset(Buffer1, 0, (((CDC_BUFFER * objNodeCollection->GetCNNodesCount())) * sizeof(char)));
			strcpy(Buffer1, "\n");
			GetAllNodeIdAssignment(Buffer1, true);
			len = strlen(Buffer1);
			if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
			{
				LOG_FATAL() << "Error writing CDC-Buffer.";
			}
			delete[] Buffer1;
			fclose(fileptr);
		}

		// Convert CDC txt file to Binary
		INT32 returnFromTxt2Cdc;
		tempOutputFileName = new char[strlen(cdcPath) + 5];
		sprintf(tempOutputFileName, "%s.cdc", cdcPath);

		string txt2cdcCommand(LibraryConfiguration::GetTxt2CdcFilePath());
		txt2cdcCommand.append(" \"" + string(tempFileName) + "\"");
		txt2cdcCommand.append(" \"" + string(tempOutputFileName) + "\"");
		returnFromTxt2Cdc = system(txt2cdcCommand.c_str());
		delete[] tempFileName;
		delete[] tempOutputFileName;

		if (OCFM_ERR_SUCCESS == returnFromTxt2Cdc)
		{
			exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
		}
		else
		{
			boost::format formatter(kMsgExternalSystemCallFailed);
			formatter % txt2cdcCommand % returnFromTxt2Cdc;
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_TXT_FOR_CDC);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		if (OCFM_ERR_EXCESS_CHANNEL == storeExceptionObj.getErrorCode())
		{
			throw storeExceptionObj;
		}

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

//TODO: only buffer is used not the fileptr. Should be removed in header & related functions
void FormatCdc(IndexCollection *objIndexCollection, char* Buffer1,
		FILE* fileptr, NodeType eNodeType)
{
	if (objIndexCollection == NULL || Buffer1 == NULL)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'objIndexCollection', 'Buffer1'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	char *tempBuffer1 = NULL;
	strcpy(Buffer1, "");
	// UINT32 len;
	INT32 noOfIndexes = objIndexCollection->GetNumberofIndexes();
	//disable mapping pdo
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index* indexObj;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == true)
		{
			if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
			{
				tempBuffer1 = new char[CDC_BUFFER];
				//len = strlen(Buffer1);      
				//GetIndexData(objIndex,Buffer1);
				EnableDisableMappingPDO(objIndexCollection, indexObj,
						tempBuffer1, false);
				strcat(Buffer1, tempBuffer1);
				delete[] tempBuffer1;
			}
		}
	}
	// write all objects except pdo
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index* indexObj;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);

		if (indexObj->GetFlagIfIncludedCdc() == true
				&& true
						== CheckAccessTypeForInclude(
								(char*) indexObj->GetAccessType())
				&& CheckIfNotPDO((char*) indexObj->GetIndexValue()))
		{
			if ((CN == eNodeType)
					|| (strcmp(indexObj->GetIndexValue(), "1F81") != 0
							&& MN == eNodeType))
			{
				tempBuffer1 = new char[15000];
				GetIndexData(indexObj, tempBuffer1);
				strcat(Buffer1, tempBuffer1);

				delete[] tempBuffer1;
			}
			else
			{
				SubIndex* sidxObj = indexObj->GetSubIndexbyIndexValue(
						(char*) "F0");
				if (NULL != sidxObj && true == sidxObj->GetFlagIfIncludedCdc()
						&& true
								== CheckAccessTypeForInclude(
										(char*) sidxObj->GetAccessType())
						&& NULL != sidxObj->GetActualValue()
						&& 0 != strcmp((char*) sidxObj->GetActualValue(), "")
						&& true == IsDefaultActualNotEqual(sidxObj))
				{
					tempBuffer1 = new char[CDC_BUFFER];
					strcpy(tempBuffer1, "1F81");
					strcat(tempBuffer1, "\t");
					strcat(tempBuffer1, "F0");
					strcat(tempBuffer1, "\t00000004\t");

					char actValue[20];
					actValue[0] = '\0';
					if (CheckIfHex((char*) sidxObj->GetActualValue()))
					{
						INT32 len = strlen((char*) sidxObj->GetActualValue());
						strncpy(actValue, (sidxObj->GetActualValue() + 2),
								len - 2);
						actValue[len - 2] = '\0';
						strcat(tempBuffer1, PadLeft(actValue, '0', 8));
					}
					else
					{
						strcpy(actValue,
								IntToAscii(atoi(sidxObj->GetActualValue()),
										actValue, 16));
						strcat(tempBuffer1, PadLeft(actValue, '0', 8));
					}

					strcat(tempBuffer1, "\n");
					// len = strlen(TempBuffer1);
					strcat(Buffer1, tempBuffer1);
					delete[] tempBuffer1;
				}
			}
		}
	}
	//Write the pdo configuration
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index* indexObj;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == true
				&& (true
						== CheckAccessTypeForInclude(
								(char*) indexObj->GetAccessType())
						|| CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
				&& !CheckIfNotPDO((char*) indexObj->GetIndexValue()))
		{
			tempBuffer1 = new char[3 * CDC_BUFFER];
			GetIndexData(indexObj, tempBuffer1);
			strcat(Buffer1, tempBuffer1);

			delete[] tempBuffer1;
		}
	}
	//reenable the pdos
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index* indexObj;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == true)
		{
			if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
			{
				tempBuffer1 = new char[CDC_BUFFER];
				EnableDisableMappingPDO(objIndexCollection, indexObj,
						tempBuffer1, true);
				strcat(Buffer1, tempBuffer1);
				delete[] tempBuffer1;
			}
		}
	}
}

//TODO: only buffer is used not the fileptr. should be removed in header & related functions
void BRSpecificFormatCdc(IndexCollection *objIndexCollection, char* Buffer1,
		FILE* fileptr, NodeType eNodeType, INT32 iNodeId)
{
	ocfmRetCode exceptionObj;
	if ((objIndexCollection == NULL) || ( Buffer1 == NULL))
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'objIndexCollection', 'Buffer1'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	char *tempBuffer1 = NULL;
	strcpy(Buffer1, "");
	// UINT32 len;
	INT32 noOfIndexes = objIndexCollection->GetNumberofIndexes();
	//disable mapping pdo
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index* indexObj = NULL;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);
		if (NULL == indexObj)
		{
			ostringstream errorString;
			errorString << "Encountered null-pointer in index-collection of node " << iNodeId << ".";
			LOG_FATAL() << errorString.str();			
			exceptionObj.setErrorCode(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			exceptionObj.setErrorString(errorString.str());
			throw exceptionObj;
		}
		if (indexObj->GetFlagIfIncludedCdc() == true)
		{
			if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being disabled.
				SubIndex* sidxObj = NULL;
				sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
				if (NULL == sidxObj)
				{
					ostringstream errorString;
					errorString << "Index 0x" << indexObj->GetIndexValue() << " on node " << iNodeId << " does not contain subIndex 0x0.";
					LOG_FATAL() << errorString.str();					
					exceptionObj.setErrorCode(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					exceptionObj.setErrorString(errorString.str());
					throw exceptionObj;
				}
				if (!CheckIfValueZero((char*) sidxObj->GetActualValue()))
				{
					tempBuffer1 = new char[CDC_BUFFER];
					//len = strlen(Buffer1);      
					EnableDisableMappingPDO(objIndexCollection, indexObj,
							tempBuffer1, false);
					strcat(Buffer1, tempBuffer1);
					delete[] tempBuffer1;
				}
			}
		}
	}
	// write all objects except pdo
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index* indexObj;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == true
				&& true
						== CheckAccessTypeForInclude(
								(char*) indexObj->GetAccessType())
				&& CheckIfNotPDO((char*) indexObj->GetIndexValue()))
		{
			if ((CN == eNodeType)
					|| (strcmp(indexObj->GetIndexValue(), "1F81") != 0
							&& MN == eNodeType))
			{
				tempBuffer1 = new char[15000];
				//commented the GetIndexData fn and BRSpecificGetIndexData is used
				//GetIndexData(objIndex,TempBuffer1);
				BRSpecificGetIndexData(indexObj, tempBuffer1, iNodeId);
				strcat(Buffer1, tempBuffer1);

				delete[] tempBuffer1;
			}
			else
			{
				SubIndex* sidxObj = indexObj->GetSubIndexbyIndexValue(
						(char*) "F0");
				if (NULL != sidxObj 
					&& true == sidxObj->GetFlagIfIncludedCdc()
					&& true == CheckAccessTypeForInclude((char*) sidxObj->GetAccessType())
					&& NULL != sidxObj->GetActualValue()
					&& 0 != strcmp((char*) sidxObj->GetActualValue(), "")
					&& true == IsDefaultActualNotEqual(sidxObj))
				{
					tempBuffer1 = new char[CDC_BUFFER];
					strcpy(tempBuffer1, "1F81");
					strcat(tempBuffer1, "\t");
					strcat(tempBuffer1, "F0");
					strcat(tempBuffer1, "\t00000004\t");

					char actValue[20];
					actValue[0] = '\0';
					if (CheckIfHex((char*) sidxObj->GetActualValue()))
					{
						INT32 len = strlen((char*) sidxObj->GetActualValue());
						strncpy(actValue, (sidxObj->GetActualValue() + 2),
								len - 2);
						actValue[len - 2] = '\0';
						strcat(tempBuffer1, PadLeft(actValue, '0', 8));
					}
					else
					{
						strcpy(actValue,
								IntToAscii(atoi(sidxObj->GetActualValue()),
										actValue, 16));
						strcat(tempBuffer1, PadLeft(actValue, '0', 8));
					}

					strcat(tempBuffer1, "\n");
					//  len = strlen(TempBuffer1);
					strcat(Buffer1, tempBuffer1);
					delete[] tempBuffer1;
				}
			}
		}
	}
	//Write the pdo configuration
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index *indexObj = NULL;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == true
				&& (true
						== CheckAccessTypeForInclude(
								(char*) indexObj->GetAccessType())
						|| CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
				&& !CheckIfNotPDO((char*) indexObj->GetIndexValue()))
		{
			tempBuffer1 = new char[3 * CDC_BUFFER];
			GetIndexData(indexObj, tempBuffer1);
			strcat(Buffer1, tempBuffer1);

			delete[] tempBuffer1;
		}
	}
	//reenable the pdos
	for (INT32 indexLC = 0; indexLC < noOfIndexes; indexLC++)
	{
		Index* indexObj;
		indexObj = objIndexCollection->GetIndexByPosition(indexLC);
		if (NULL == indexObj)
		{
			ostringstream errorString;
			errorString << "Encountered null-pointer in index-collection of node " << iNodeId << ".";
			LOG_FATAL() << errorString.str();
			exceptionObj.setErrorString(errorString.str());
			exceptionObj.setErrorCode(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw exceptionObj;
		}
		if (indexObj->GetFlagIfIncludedCdc() == true)
		{
			if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being enabled.
				SubIndex *sidxObj = NULL;
				sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
				if (NULL == sidxObj)
				{
					ostringstream errorString;
					errorString << "Index 0x" << indexObj->GetIndexValue() << " on node " << iNodeId << " does not contain subIndex 0x0.";
					LOG_FATAL() << errorString.str();					
					exceptionObj.setErrorCode(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					exceptionObj.setErrorString(errorString.str());
					throw exceptionObj;
				}
				if (!CheckIfValueZero((char*) sidxObj->GetActualValue()))
				{
					tempBuffer1 = new char[CDC_BUFFER];
					EnableDisableMappingPDO(objIndexCollection, indexObj,
							tempBuffer1, true);
					strcat(Buffer1, tempBuffer1);

					delete[] tempBuffer1;
				}
			}
		}
	}
}

INT32 ProcessCDT(ComplexDataType* cdtObj, ApplicationProcess* appProcessObj,
		Node* nodeObj, Parameter* parameterObj, PDOType pdoType,
		char* moduleName, char* moduleIndexId)
{
	ocfmRetCode exceptionObj;
	if (!cdtObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'cdtObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	if ((NULL == appProcessObj) || (NULL == nodeObj) || (NULL == parameterObj)
			|| (NULL == moduleName) || (NULL == moduleIndexId))
	{
		LOG_FATAL() << "Parameters 'appProcessObj', 'nodeObj', 'parameterObj', 'moduleName', 'moduleIndexId' must not be NULL.";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);		
		throw exceptionObj;
	}

	//TODO: No header definitions is for ProcessCDT
	INT32 startBitOffset = 0;
	INT32 offsetVal = 0;
	bool isNewBitStr = false;
	INT32 dataSize = 0;
	INT32 totalBytesMapped = 0;
	for (UINT32 varDeclLC = 0; varDeclLC < cdtObj->varDeclarationCollection.size(); varDeclLC++)
	{

		VarDeclaration varDeclObj;
		varDeclObj = cdtObj->varDeclarationCollection[varDeclLC];

		if (varDeclObj.namIdDtAttr->GetDtUniqueRefId() != NULL)
		{
			cdtObj = appProcessObj->GetCDTbyUniqueID(
					varDeclObj.namIdDtAttr->GetDtUniqueRefId());
			appProcessObj->UpdatePreviousCDTUId(varDeclObj.structUniqueId,
					cdtObj->cDtObjPosition);
			lastVarIndexGlobal = varDeclLC;
			ProcessCDT(cdtObj, appProcessObj, nodeObj, parameterObj, pdoType,
					moduleName, moduleIndexId);
		}
		if (!cdtCompletedGlobal)
		{
			// add rest of the contents
			ProcessImage piObj;
			piObj.Initialize();
			piObj.bitOffset = -1;
			piObj.byteOffset = 0;
			if (parameterObj->accessStr != NULL)
				strcpy(piObj.direction,
						GetParameterAccess(parameterObj->accessStr));

			if (pdoType == PDO_TPDO)
			{
				piObj.directionType = INPUT;
			}
			else if (pdoType == PDO_RPDO)
			{
				piObj.directionType = OUTPUT;
			}

			if (varDeclObj.size != NULL)
			{
				piObj.dataInfo.dataSize = atoi(varDeclObj.size);
			}
			else
			{
			}
			if (varDeclObj.namIdDtAttr->GetName() != NULL)
			{
				piObj.name = (char*) malloc(strlen(varDeclObj.namIdDtAttr->GetName()) + strlen(moduleName) + 6 + ALLOC_BUFFER);
				strcpy(piObj.name, GetPIName(nodeObj->GetNodeId()));
				strcat(piObj.name, moduleName);
				strcat(piObj.name, ".");
				strcat(piObj.name, varDeclObj.namIdDtAttr->GetName());

				piObj.moduleName = new char[strlen(moduleName)
						+ STR_ALLOC_BUFFER];
				strcpy(piObj.moduleName, moduleName);

				piObj.moduleIndex = new char[strlen(moduleIndexId)
						+ STR_ALLOC_BUFFER];
				strcpy(piObj.moduleIndex, moduleIndexId);

				piObj.varDeclName = new char[strlen(
						varDeclObj.namIdDtAttr->GetName()) + STR_ALLOC_BUFFER];
				strcpy(piObj.varDeclName, varDeclObj.namIdDtAttr->GetName());
				//TODO: delete new char (memory Issue)
			}

			piObj.nodeId = nodeObj->GetNodeId();

			if (varDeclObj.namIdDtAttr->GetDataType() != NULL)
			{
				piObj.dataInfo.dtName = new char[strlen(
						varDeclObj.namIdDtAttr->GetDataType())
						+ STR_ALLOC_BUFFER];
				strcpy(piObj.dataInfo.dtName,
						(const char*) varDeclObj.namIdDtAttr->GetDataType());
			}

			/* Set the IEC DT*/
			piObj.dataInfo = *(GetIECDT(varDeclObj.namIdDtAttr->GetDataType(),
					piObj.dataInfo.dataSize));

			/* Calculate Offset*/
			if (((piObj.dataInfo.iecDtVar != BITSTRING)
					&& (piObj.dataInfo.dataSize >= 8))
					|| ((piObj.dataInfo.iecDtVar == BITSTRING)
							&& (startBitOffset == 0 || startBitOffset == 8
									|| startBitOffset == 16
									|| startBitOffset == 32
									|| startBitOffset == 64)))
			{

				isNewBitStr = true;
				if (piObj.dataInfo.iecDtVar == BITSTRING)
				{
					startBitOffset = 0;
					dataSize = 0;
					for (UINT32 bitStrCount = varDeclLC;
							bitStrCount
									< cdtObj->varDeclarationCollection.size();
							bitStrCount++)
					{
						VarDeclaration varDeclBitStr;
						varDeclBitStr =
								cdtObj->varDeclarationCollection[bitStrCount];
						if (BITSTRING
								== ((*(GetIECDT(
										varDeclBitStr.namIdDtAttr->GetDataType(),
										piObj.dataInfo.dataSize))).iecDtVar))
						{
							dataSize += atoi(varDeclBitStr.size);
							if (8 == dataSize || 16 == dataSize
									|| 32 == dataSize || 64 == dataSize)
							{
								break;
							}
						}
						else
						{
							break;
						}
					}
				}
				else
				{
					dataSize = piObj.dataInfo.dataSize;

				}

				if (pdoType == PDO_RPDO)
				{
					offsetVal = ComputeOUTOffset(dataSize, pdoType);
				}
				else if (pdoType == PDO_TPDO)
				{
					offsetVal = ComputeINOffset(dataSize, pdoType);
				}
				}
			/* Set the Byte Offet*/
			piObj.byteOffset = offsetVal;
			if (isNewBitStr)
			{
				/* Total bytes Mapped */
				totalBytesMapped = totalBytesMapped + (dataSize / 8);
				LOG_DEBUG() << "Total bytes mapped: " << totalBytesMapped;
				if (totalBytesMapped > MAX_PI_SIZE)
				{
					boost::format formatter(kMsgMaxPiSizeExceeded);
					formatter 
						% totalBytesMapped
						% MAX_PI_SIZE;
					exceptionObj.setErrorCode(OCFM_ERR_MAX_PI_SIZE);
					exceptionObj.setErrorString(formatter.str());
					LOG_FATAL() << formatter.str();
					throw exceptionObj;
				}
				CreateMNPDOVar(offsetVal, dataSize, piObj.dataInfo.iecDtVar,
						pdoType, nodeObj);
			}

			if ((piObj.dataInfo.dataSize >= 8) && (startBitOffset != 0)
					&& (piObj.dataInfo.iecDtVar != BITSTRING))
			{
				startBitOffset = 0;
			}
			else if (piObj.dataInfo.iecDtVar == BITSTRING)
			{
				piObj.bitOffset = startBitOffset;
				startBitOffset = startBitOffset + piObj.dataInfo.dataSize;
				piObj.byteOffset = offsetVal;
				isNewBitStr = false;
			}
			nodeObj->AddProcessImage(piObj);

		}

	}

	if (cdtObj->prevUniqueId != NULL)
	{
		cdtObj = appProcessObj->GetCDTbyUniqueID(cdtObj->prevUniqueId);

		for (UINT32 iLoopCount = (lastVarIndexGlobal + 1);
				iLoopCount < cdtObj->varDeclarationCollection.size();
				iLoopCount++)
		{
			if (!cdtCompletedGlobal)
			{
				VarDeclaration objVarDecl;
				objVarDecl = cdtObj->varDeclarationCollection[iLoopCount];
				if (objVarDecl.namIdDtAttr->GetDtUniqueRefId() != NULL)
				{
					cdtObj = appProcessObj->GetCDTbyUniqueID(
							objVarDecl.namIdDtAttr->GetDtUniqueRefId());
					appProcessObj->UpdatePreviousCDTUId(
							objVarDecl.structUniqueId, cdtObj->cDtObjPosition);

					lastVarIndexGlobal = iLoopCount;
					ProcessCDT(cdtObj, appProcessObj, nodeObj, parameterObj,
							pdoType, moduleName, moduleIndexId);
				}
			}
		}
	}
	cdtCompletedGlobal = true;
	LOG_DEBUG() << "Total bytes mapped: " << totalBytesMapped << ", offset: " << offsetVal;
	//Returned current mapped size in bytes
	return totalBytesMapped;
}

//INT32 DecodeUniqueIDRef(char* uniqueidRefID, Node* nodeObj, PDOType pdoType, char* moduleName, char* moduleIndex)
INT32 DecodeUniqueIDRef(char* uniqueidRefId, Node* nodeObj, Index indexObj, SubIndex* sidxObj, Index* moduleIndexObj, SubIndex* moduleSidxObj)
{
	ocfmRetCode exceptionObj;
	ostringstream errorString;
//moduleSidxObj can be null for Var object types
if ((uniqueidRefId == NULL) || (nodeObj == NULL) || (sidxObj == NULL) || (moduleIndexObj == NULL))
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'uniqueidRefId', 'nodeObj', 'sidxObj', 'moduleIndexObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		return 0;
	}

	Parameter* parameterObj = NULL;
	ApplicationProcess* appProcessObj = NULL;
	ComplexDataType* cdtObj = NULL;
	INT32 totalBytesMapped = 0;
	INT32 iStartBitOffset =  0;
	INT32 iOffset = 0;
	//bool bIsNewBitStringVar = false;
	INT32 iDataSize = 0;
	//cout<<"DecodeUniqiueIDRef"<<endl;
	try
	{
		if (nodeObj->GetApplicationProcess() != NULL)
		{
			appProcessObj = nodeObj->GetApplicationProcess();
			if (appProcessObj->GetParameterCollection().size() != 0)
			{
				parameterObj = appProcessObj->GetParameterbyUniqueIDRef(uniqueidRefId);
				if (parameterObj == NULL)
				{
					boost::format formatter(kMsgParameterNotFound);
					if (moduleSidxObj == NULL)
					{
						formatter % uniqueidRefId 
							% nodeObj->GetNodeId()
							% moduleIndexObj->GetIndex()
							% "-";
					}
					else
					{
						formatter % uniqueidRefId 
							% nodeObj->GetNodeId()
							% moduleIndexObj->GetIndex()
							% moduleSidxObj->GetIndexValue();
					}

					exceptionObj.setErrorCode(OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND);
					exceptionObj.setErrorString(formatter.str());
					LOG_FATAL() << formatter.str();
					throw exceptionObj;
				}
				if (parameterObj->accessStr == NULL)
				{					
					boost::format formatter(kMsgAccessTypeForParameterInvalid);
					if (moduleSidxObj == NULL)
					{
						formatter % indexObj.GetIndex()
							% sidxObj->GetIndex()
							% uniqueidRefId 
							% moduleIndexObj->GetIndex()
							% "-"
							% nodeObj->GetNodeId()
							% parameterObj->accessStr;
					}
					else
					{
						formatter % indexObj.GetIndex()
							% sidxObj->GetIndex()
							% uniqueidRefId 
							% moduleIndexObj->GetIndex()
							% moduleSidxObj->GetIndexValue()
							% nodeObj->GetNodeId()
							% parameterObj->accessStr;
					}
					exceptionObj.setErrorCode(OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PARAMETER);
					exceptionObj.setErrorString(formatter.str());
					LOG_FATAL() << formatter.str();
					throw exceptionObj;
				}

				//Blocked thowing exception for objects with UniqueIdRef because the TCL/TK GUI has no support for UniqueIdRef.
/*
				//Checking for AccessType for a Parameter that is mapped to a PDO via an Object
				if(moduleSidxObj != NULL)
				{
					if(!IsValidAccessTypeForPdo(indexObj.GetPDOType(), (char*) moduleSidxObj->GetPDOMapping(), parameterObj->accessStr))
					{
						exceptionObj.setErrorCode(OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PDO);
						errorString <<"Parameter with accessType="<<parameterObj->accessStr<<" cannot be mapped to PDO\n In "<<nodeObj->GetNodeName()<<"("<<nodeObj->GetNodeId()<<"), "<<indexObj.GetIndexValue()<<"/"<<sidxObj->GetIndexValue()<<" mapped via the module "<<moduleIndexObj->GetName()<<"(0x"<<moduleIndexObj->GetIndexValue()<<")/"<<moduleSidxObj->GetName()<<"(0x"<<moduleSidxObj->GetIndexValue()<<")";
						exceptionObj.setErrorString(errorString.str());
						throw exceptionObj;
					}
				}
				else
				{
					if(!IsValidAccessTypeForPdo(indexObj.GetPDOType(), (char*) moduleIndexObj->GetPDOMapping(), parameterObj->accessStr))
					{
						exceptionObj.setErrorCode(OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PDO);
						errorString << "Parameter with accessType="<<parameterObj->accessStr<<" cannot be mapped to PDO\n In "<<nodeObj->GetNodeName()<<"("<<nodeObj->GetNodeId()<<"), "<<indexObj.GetIndexValue()<<"/"<<sidxObj->GetIndexValue()<<" mapped via the module "<<moduleIndexObj->GetName()<<"(0x"<<moduleIndexObj->GetIndexValue()<<")";
						exceptionObj.setErrorString(errorString.str());
						throw exceptionObj;
					}
				}
*/
				// Check if DataTypeUniqueIDref exists
				if((parameterObj->nameIdDtAttr->GetDtUniqueRefId() != NULL) && (strcmp(parameterObj->nameIdDtAttr->GetDtUniqueRefId(), "") != 0))
				{
					cdtObj = appProcessObj->GetCDTbyUniqueID(parameterObj->nameIdDtAttr->GetDtUniqueRefId());
					if (cdtObj == NULL)
					{
						boost::format formatter(kMsgStructDatatypeNotFound);
						formatter % parameterObj->nameIdDtAttr->GetDtUniqueRefId()
							% uniqueidRefId
							% nodeObj->GetNodeId();
						exceptionObj.setErrorCode(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
						exceptionObj.setErrorString(formatter.str());
						LOG_FATAL() << formatter.str();
						throw exceptionObj;
					}
					totalBytesMapped = ProcessCDT(cdtObj, appProcessObj, nodeObj, parameterObj, indexObj.GetPDOType(), (char*)moduleIndexObj->GetName(), (char*) moduleIndexObj->GetIndexValue());
					lastVarIndexGlobal = -1;
					cdtCompletedGlobal = false;
				}
				else if ((parameterObj->nameIdDtAttr->GetDataType() != NULL) && (strcmp(parameterObj->nameIdDtAttr->GetDataType(), "") != 0))
				{
					//cout<<"Dt: "<<parameterObj->nameIdDtAttr.dataType<<" ModName:"<<moduleIndexObj->GetName()<<" moduleIndex:"<<moduleIndexObj->GetIndexValue()<<endl;
					//pobjAppProc, nodeObj, parameterObj, pdoType, moduleName, moduleIndex
					ProcessImage objProcessImage;
					objProcessImage.Initialize();
					objProcessImage.bitOffset = -1;
					objProcessImage.byteOffset = 0;
					if(parameterObj->accessStr != NULL)
					{
						strcpy(objProcessImage.direction, GetParameterAccess(parameterObj->accessStr));
					}
					//cout<<"Access:"<<objProcessImage.direction<<endl;
					if(indexObj.GetPDOType() == PDO_TPDO)
					{
						objProcessImage.directionType = INPUT;
					}
					else if(indexObj.GetPDOType() == PDO_RPDO)
					{
						objProcessImage.directionType = OUTPUT;
					}
					objProcessImage.nodeId = nodeObj->GetNodeId();

					objProcessImage.name = new char[strlen(uniqueidRefId) + strlen(moduleIndexObj->GetName()) + 6 + ALLOC_BUFFER];
					strcpy(objProcessImage.name, GetPIName(nodeObj->GetNodeId()));
					strcat(objProcessImage.name, moduleIndexObj->GetName());
					strcat(objProcessImage.name, ".");
					strcat(objProcessImage.name, uniqueidRefId);
					
					objProcessImage.moduleName = new char[strlen(moduleIndexObj->GetName()) + ALLOC_BUFFER];
					strcpy(objProcessImage.moduleName, moduleIndexObj->GetName());
					
					objProcessImage.moduleIndex = new char[strlen(moduleIndexObj->GetIndexValue()) + ALLOC_BUFFER];
					strcpy(objProcessImage.moduleIndex, moduleIndexObj->GetIndexValue());

					objProcessImage.dataInfo = *(GetIECDT(parameterObj->nameIdDtAttr->GetDataType(), parameterObj->size));
					if( ((objProcessImage.dataInfo.iecDtVar != BITSTRING) && (objProcessImage.dataInfo.dataSize >= 8 )) 
						//|| ((objProcessImage.dataInfo.iecDtVar == BITSTRING) && (iStartBitOffset == 0 || iStartBitOffset == 8 || iStartBitOffset == 16 || iStartBitOffset == 32 || iStartBitOffset == 64)))
						)
					{
						//bIsNewBitStringVar =  true;
						//if(objProcessImage.dataInfo.iecDtVar == BITSTRING) 
						//{
							iStartBitOffset = 0;
							iDataSize =  0;
						//}
						//else
						//{
							iDataSize =  objProcessImage.dataInfo.dataSize;
						//}

						if(indexObj.GetPDOType() == PDO_RPDO)
						{
							iOffset =  ComputeOUTOffset(iDataSize, indexObj.GetPDOType());
						}
						else if(indexObj.GetPDOType() == PDO_TPDO)
						{
							iOffset =  ComputeINOffset(iDataSize, indexObj.GetPDOType());
						}
					}
					
					//cout<<"iOffset"<<iOffset<<endl;
					objProcessImage.byteOffset = iOffset;

					 totalBytesMapped = totalBytesMapped + (iDataSize / 8);
					//cout<<"Create MN pdo Var"<<" iOffset:"<<iOffset<<" iDataSize:"<<iDataSize<<endl;
					CreateMNPDOVar(iOffset, iDataSize, objProcessImage.dataInfo.iecDtVar, indexObj.GetPDOType(), nodeObj);
					if((objProcessImage.dataInfo.dataSize >= 8) && (iStartBitOffset!= 0 ) && (objProcessImage.dataInfo.iecDtVar != BITSTRING))
					{
						iStartBitOffset = 0;
					}
					else if(objProcessImage.dataInfo.iecDtVar == BITSTRING)
					{
						objProcessImage.bitOffset = iStartBitOffset;
						iStartBitOffset = iStartBitOffset + objProcessImage.dataInfo.dataSize;
						objProcessImage.byteOffset = iOffset;
						//bIsNewBitStringVar =  false;
					}

					nodeObj->AddProcessImage(objProcessImage);
					lastVarIndexGlobal = -1;
					cdtCompletedGlobal = false;
				}
				else
				{
					LOG_ERROR() << "DataTypeIdRef and data type do not exist for uniqueIdRef: " << uniqueidRefId << ".";
				}
			}
		}

	} catch (ocfmRetCode& ex)
	{
		LOG_FATAL() << ex.getErrorString();
		throw ex;
	}
	return totalBytesMapped;
}

ocfmRetCode ProcessPDONodes(bool isBuild)
{
	NodeCollection *nodeCollObj = NULL;
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);
	ostringstream errorString;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node *nodeObj = NULL;
	//CNode *pobjMNNode = NULL;
	INT32 totalBytesMapped = 0;
	INT32 totalChainedBytesMapped = 0;
	INT32 rpdoMappedNodeID = 0;

	IndexCollection *pdoIndexCollObj = NULL;
	IndexCollection *indexCollObj = NULL;
	/* Check RPDO Mapped objects*/
	INT32 nodesCount = 0;
	// bool bChangeOffset = false;
	nodesCount = nodeCollObj->GetCNNodesCount();

	LOG_INFO() << "Processing a total no. of " << nodesCount << " nodes.";
	try
	{
		if (nodesCount == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_CN_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoControlledNodesConfigured);
			LOG_FATAL() << kMsgNoControlledNodesConfigured;
			throw exceptionObj;
		}

		size8INOffset.currOffset = 0;
		size8INOffset.prevOffset = 0;
		size16INOffset.currOffset = 0;
		size16INOffset.prevOffset = 0;
		size32INOffset.currOffset = 0;
		size32INOffset.prevOffset = 0;
		size64INOffset.currOffset = 0;
		size64INOffset.prevOffset = 0;

		size8OUTOffset.currOffset = 0;
		size8OUTOffset.prevOffset = 0;
		size16OUTOffset.currOffset = 0;
		size16OUTOffset.prevOffset = 0;
		size32OUTOffset.currOffset = 0;
		size32OUTOffset.prevOffset = 0;
		size64OUTOffset.currOffset = 0;
		size64OUTOffset.prevOffset = 0;

		//INT32 *nodeIDbyStnArranged = NULL;
		//nodeIDbyStnArranged = ArrangeNodeIDbyStation();
		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{

			nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
			/* Process PDO Objects for CN*/

			if (nodeObj->GetNodeType() == CN)
			{
				if (!(nodeObj->HasPdoObjects()))
				{
					continue;
				}
				StationType stnType = nodeObj->GetStationType();

				/* Empty ProcessImage collection (Axxx objects)*/
				nodeObj->DeleteCollectionsForPI();

				INT32 countTPDO = 0;
				INT32 countRPDO = 0;
				pdoIndexCollObj = nodeObj->getPDOIndexCollection(&countRPDO,
						&countTPDO);

				if (pdoIndexCollObj == NULL)
				{
					LOG_ERROR() << "No PDO-Indices existent for node " << nodeObj->GetNodeId() << ".";
					continue;
				}
				//Validate the number of TPDO's for a CN
				if (countTPDO > 1)
				{		
					boost::format formatter(kMsgTPDOChannelCountExceeded);
					formatter % nodeObj->GetNodeId() % 1 % countTPDO;
					exceptionObj.setErrorCode(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					exceptionObj.setErrorString(formatter.str());
					LOG_FATAL() << formatter.str();
					throw exceptionObj;
				}

				indexCollObj = nodeObj->GetIndexCollection();

				nodeObj->SetPReqActPayloadValue(0);
				nodeObj->SetPResActPayloadValue(0);

				INT32 nodeMappedTotalBytes = 0;

				for (INT32 indexLC = 0;
						indexLC < pdoIndexCollObj->GetNumberofIndexes();
						indexLC++)
				{

					Index *indexObjB4Sort = NULL;
					Index indexObj;
					indexObjB4Sort = pdoIndexCollObj->GetIndexByPosition(indexLC);
					if (!(CheckIfMappingPDO(
							(char*) indexObjB4Sort->GetIndexValue())))
					{
						continue;
					}
					else
					{
						LOG_DEBUG() << "Processing PDO-Index 0x" << indexObjB4Sort->GetIndexValue();
					}
					nodeMappedTotalBytes = 0;
					// bChangeOffset = false;

					if (indexObjB4Sort->GetNumberofSubIndexes() > 0)
					{
/*
	Commented the function call to sort the subindex objects with respect to offset. It is redundant while fetching subindex by order (00-FE) and 00th sidx gives the number of valid subindex
*/
						/* Sort the pdo collection */
						//indexObj = GetPDOIndexByOffset(indexObjB4Sort);
						indexObj = *indexObjB4Sort;
						INT32 sidxCount = 1;

						// Initialised to Zero and the value will be taken from the Actual value or the default value in priority
						INT32 sidxTotalCount = 0;
						//check whether the channel is activated
						SubIndex *sidxObjB4Sort = NULL;
						sidxObjB4Sort = indexObjB4Sort->GetSubIndexbyIndexValue((char *) "00");
						if (NULL == sidxObjB4Sort)
						{				
							boost::format formatter(kMsgNonExistingSubIndex);
							formatter % indexObjB4Sort->GetIndex()
								% 0 
								% nodeObj->GetNodeId();
							exceptionObj.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
							exceptionObj.setErrorString(formatter.str());
							LOG_FATAL() << formatter.str();

							throw exceptionObj;
							//continue;
						}
						// Actual value checked for Null, Empty, non-zero
						bool actualValueValid = true;
						if ((sidxObjB4Sort->GetActualValue() != NULL)
								&& (0 != strcmp(sidxObjB4Sort->GetActualValue(), ""))
								&& !(CheckIfValueZero(
										(char*) sidxObjB4Sort->GetActualValue())))
						{
							//value is not zero the channel is activated
							sidxTotalCount = GetDecimalValue(
									(char*) sidxObjB4Sort->GetActualValue());
						}
						else
						{
							if (0 == strcmp(sidxObjB4Sort->GetActualValue(), ""))
							{
								//pdo channel is deactivated. Empty act value
								continue;
							}
							if (CheckIfValueZero(
									(char*) sidxObjB4Sort->GetActualValue()))
							{
								// PDO channel is deactivated
								// Zero is not set here,as it is intialised to Zero previously
								LOG_WARN() << "CheckIfValueZero default:: Not Processing - CN " << nodeObj->GetNodeId() << " Index - " << indexObj.GetIndexValue();
								continue;
							}
							else // If the Actual values is Null or Empty, Default value is set for Total SIdx for mapping
							{
								//No need to check for value null or empty. GetDecimalValue returns zero or particular value.
								sidxTotalCount = GetDecimalValue((char*) sidxObjB4Sort->GetDefaultValue());
								actualValueValid = false;
							}
						}

						//Check isiTotal value is valid
						if (sidxTotalCount >= (indexObjB4Sort->GetNumberofSubIndexes()))
						{
							boost::format formatter(kMsgAttributeValueInvalid);
							formatter % (actualValueValid ? sidxObjB4Sort->GetActualValue() : sidxObjB4Sort->GetDefaultValue())
								% (actualValueValid ? ACTUALVALUE : DEFAULTVALUE)
								% indexObjB4Sort->GetIndex()
								% sidxObjB4Sort->GetIndex()
								% nodeObj->GetNodeId()
								% "<= highest available subIndex";
							exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
							exceptionObj.setErrorString(formatter.str());
							LOG_FATAL() << formatter.str();
							throw exceptionObj;
						}

						bool pdoResult = false;
						pdoResult = CheckPdoCommParam(indexObj.GetPDOType(), isBuild, &indexObj, indexCollObj, nodeObj);
						if (pdoResult == false)
						{
							//Incorrect Target node id for a PDO(may be a cross Tx). So do not process.
							LOG_WARN() << "CheckPdoCommParam:: Not Processing - CN " << nodeObj->GetNodeId() << " Index - " << indexObj.GetIndexValue();
							continue;
						}

						while (sidxCount <= sidxTotalCount)
						{
							SubIndex *sidxObj = NULL;
							char *sidxId = new char[SUBINDEX_LEN];
							sidxId = IntToAscii(sidxCount, sidxId, 16);
							sidxId = PadLeft(sidxId, '0', 2);
							LOG_DEBUG() << "Position: "<< sidxCount << " SidxId: " << sidxId;
							//sidxObj = indexObj.GetSubIndex(sidxCount);
							sidxObj = indexObj.GetSubIndexbyIndexValue(sidxId);

							if (sidxObj == NULL)
							{	
								boost::format formatter(kMsgSubIndexInvalid);
								formatter % indexObj.GetIndex()
									% HexToInt<UINT32>(string(sidxId)) 
									% nodeObj->GetNodeId();
								exceptionObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXID);
								exceptionObj.setErrorString(formatter.str());
								LOG_FATAL() << formatter.str();
								delete [] sidxId;
								throw exceptionObj;
							}
							delete [] sidxId;
							LOG_DEBUG() << "pobjSubIdx->getIndexValue(): " << sidxObj->GetIndexValue();
							LOG_DEBUG() << "pobjSubIdx->getName(): "<< sidxObj->GetName();
							if ((NULL != sidxObj->GetActualValue())
									&& (0 != strcmp(sidxObj->GetActualValue(),"")))
							{

								const char* actualVal =
										sidxObj->GetActualValue();
								INT32 iLength = strlen(actualVal);
								LOG_DEBUG() << "Length of the value: " << iLength << " Actual Value: " << actualVal;
								//Actual pdo mapping value includes 16 hex characters of original payload mapping and two for "0x" prefix
								if (CheckIfValueZero(actualVal))
								{
									boost::format formatter(kMsgMappingInvalid);
									formatter % indexObjB4Sort->GetIndex()
										% sidxCount
										% nodeObj->GetNodeId();
									exceptionObj.setErrorCode(OCFM_ERR_MAPPING_INVALID);
									exceptionObj.setErrorString(formatter.str());
									LOG_FATAL() << formatter.str();
									throw exceptionObj;
								}
								if (iLength != (16 + 2))
								{
									boost::format formatter(kMsgAttributeValueInvalid);
									formatter % actualVal
										% ACTUALVALUE
										% indexObj.GetIndex()
										% sidxObj->GetIndex()
										% nodeObj->GetNodeId()
										% "0x<64-bit value>";
									exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
									exceptionObj.setErrorString(formatter.str());
									LOG_FATAL() << formatter.str();
									throw exceptionObj;
								}

								//Mapped length in bits
								INT32 mappedLength = 0;
								char* mappingLen = new char[INDEX_LEN];
								SubString(mappingLen, actualVal, 2, 4);
								mappedLength = (INT32) HexToInt(mappingLen);
								delete[] mappingLen;
								LOG_DEBUG() << "PIMappedLength [bits]: " << mappedLength;
								char* moduleIndex = new char[INDEX_LEN];
								moduleIndex = SubString(moduleIndex, actualVal, iLength - 4, 4);

								/* Get the SubIndex*/
								char* varSubIndex = new char[SUBINDEX_LEN];
								varSubIndex = SubString(varSubIndex, actualVal, iLength - 6, 2);
								LOG_DEBUG() << "PImoduleIndex:" << moduleIndex << " PImoduleSubIndex:" << varSubIndex;
								Index *moduleIndexObj = NULL;
								SubIndex *moduleSidxObj = NULL;
								char *uniqueidRefID = NULL;
								char *sidxName = NULL;
								char *accessStr = NULL;
								char *moduleName = NULL;
								DataType dtObj;
								dtObj.dataTypeName = NULL;
								bool objMapped = false;
								PDOType pdoType = indexObj.GetPDOType();

								moduleIndexObj = indexCollObj->GetIndexbyIndexValue(moduleIndex);

								if (moduleIndexObj == NULL)
								{						
									boost::format formatter(kMsgNonExistingMappedIndex);
									string strModuleIndex(moduleIndex);
									formatter % indexObj.GetIndex()
										% sidxObj->GetIndex() 
										% HexToInt<UINT32>(strModuleIndex) 
										% nodeObj->GetNodeId();
									exceptionObj.setErrorCode(OCFM_ERR_MODULE_INDEX_NOT_FOUND);
									exceptionObj.setErrorString(formatter.str());
									LOG_FATAL() << formatter.str();
									delete[] moduleIndex;
									throw exceptionObj;
								}
								delete[] moduleIndex;

								if (moduleIndexObj->GetName() != NULL)
								{

									moduleName = new char[strlen(
											moduleIndexObj->GetName())
											+ ALLOC_BUFFER];
									strcpy(moduleName,
											moduleIndexObj->GetName());
								}
								if (moduleIndexObj->GetNumberofSubIndexes() == 0
										&& (strcmp(varSubIndex, "00") == 0))
								{
									objMapped = true;

									//Check if the object has correct PDOmapping attribute
									if(!CheckForValidPDOMapping(pdoType, moduleIndexObj))
									{//Mapping-Object 0x%X/0x%s: (Sub)Index 0x%X/0x%s on node %d cannot be mapped. Mismatching PDOMapping ('%s' Mapping-Object, mapped object '%s').
										string pdoTypeString = pdoType == PDO_TPDO 
											? "TPDO" : "RPDO";
										boost::format formatter(kMsgMappingTypeForPdoInvalid);
										formatter 
											% indexObj.GetIndex()
											% sidxObj->GetIndex()
											% moduleIndexObj->GetIndex()
											% "-"
											% nodeObj->GetNodeId()
											% pdoTypeString
											% moduleIndexObj->GetPDOMapping();
										exceptionObj.setErrorCode(OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();

										//Blocked thowing exception for objects with UniqueIdRef because the TCL/TK GUI has no support for UniqueIdRef.
										if (moduleIndexObj->GetUniqueIDRef() == NULL)
										{
											delete[] moduleName;
											throw exceptionObj;
										}
									}

									if (moduleIndexObj->GetUniqueIDRef() != NULL)
									{
										uniqueidRefID =
												new char[strlen(
														moduleIndexObj->GetUniqueIDRef())
														+ ALLOC_BUFFER];
										strcpy(uniqueidRefID,
												moduleIndexObj->GetUniqueIDRef());
									}
									else
									{
										if (moduleIndexObj->GetAccessType() != NULL)
										{
											accessStr = new char[strlen(moduleIndexObj->GetAccessType()) + ALLOC_BUFFER];
											strcpy(accessStr, moduleIndexObj->GetAccessType());
											if (!IsValidAccessTypeForPdo(pdoType, (char*)moduleIndexObj->GetPDOMapping(), accessStr))
											{
												boost::format formatter(kMsgAccessTypeForPdoInvalid);
												formatter % indexObj.GetIndex()
													% sidxObj->GetIndex()
													% moduleIndexObj->GetIndex()
													% "-"
													% nodeObj->GetNodeId()
													% accessStr;
												exceptionObj.setErrorCode(OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PDO);
												exceptionObj.setErrorString(formatter.str());
												LOG_FATAL() << formatter.str();
												throw exceptionObj;
											}
										}
										else
										{
											//Error: Invalid Access Type for the object %s in Node name:%s id:%d
											LOG_WARN() << "AccessType empty for index: "<< moduleIndexObj->GetIndexValue();
										}
										dtObj = moduleIndexObj->GetDataType();
									}
								}
								else
								{
									moduleSidxObj =
											moduleIndexObj->GetSubIndexbyIndexValue(
													varSubIndex);
									if (moduleSidxObj == NULL)
									{									
										boost::format formatter(kMsgNonExistingMappedSubIndex);
										formatter % indexObj.GetIndex()
											% sidxObj->GetIndex()
											% moduleIndexObj->GetIndex()
											% moduleSidxObj->GetIndex()
											% nodeObj->GetNodeId();
										exceptionObj.setErrorCode(OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();
		
										if (moduleName != NULL)
										{
											delete[] moduleName;
										}
										throw exceptionObj;
									}
									else
									{
										//Check if the subobject has correct PDOmapping attribute
										if(!CheckForValidPDOMapping(pdoType, moduleIndexObj, moduleSidxObj))
										{
											string pdoTypeString = pdoType == PDO_TPDO 
												? "TPDO" : "RPDO";
											boost::format formatter(kMsgMappingTypeForPdoInvalid);
											formatter 
												% indexObj.GetIndex()
												% sidxObj->GetIndex()
												% moduleIndexObj->GetIndex()
												% moduleSidxObj->GetIndexValue()
												% nodeObj->GetNodeId()
												% pdoTypeString
												% moduleSidxObj->GetPDOMapping();
											exceptionObj.setErrorCode(OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO);
											exceptionObj.setErrorString(formatter.str());
											LOG_FATAL() << formatter.str();
											//Blocked thowing exception for objects with UniqueIdRef because the TCL/TK GUI has no support for UniqueIdRef.
											if (moduleSidxObj->GetUniqueIDRef() == NULL)
											{
												LOG_FATAL() << errorString.str();
												throw exceptionObj;
											}
										}

										if (moduleSidxObj->GetUniqueIDRef() != NULL)
										{
											uniqueidRefID =
													new char[strlen(
															moduleSidxObj->GetUniqueIDRef())
															+ ALLOC_BUFFER];
											strcpy(uniqueidRefID,
													moduleSidxObj->GetUniqueIDRef());
										}
										else
										{
											if (moduleSidxObj->GetName() != NULL)
											{
												sidxName = new char[strlen(moduleSidxObj->GetName()) + ALLOC_BUFFER];
												strcpy(sidxName, moduleSidxObj->GetName());
											}
											else
											{
												//Error: Name not configured for the SubObject %s/%s in Node name:%s id:%d
											}

											if (moduleSidxObj->GetAccessType() != NULL)
											{
												accessStr = new char[strlen(moduleSidxObj->GetAccessType()) + ALLOC_BUFFER];
												strcpy(accessStr, moduleSidxObj->GetAccessType());

												if(!IsValidAccessTypeForPdo(pdoType, (char*)moduleSidxObj->GetPDOMapping(), accessStr))
												{
													boost::format formatter(kMsgAccessTypeForPdoInvalid);
													formatter % indexObj.GetIndex()
														% sidxObj->GetIndex()
														% moduleIndexObj->GetIndex()
														% moduleSidxObj->GetIndexValue()
														% nodeObj->GetNodeId()
														% accessStr;
													exceptionObj.setErrorCode(OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PDO);
													exceptionObj.setErrorString(formatter.str());
													LOG_FATAL() << formatter.str();
													delete[] sidxName;
													throw exceptionObj;
												}
											}
											else
											{
												//Error: Invalid Access Type for the SubObject %s/%s in Node name:%s id:%d
												LOG_WARN() << "AccessType empty for index: "<< moduleIndexObj->GetIndexValue() << " subindex:" << moduleSidxObj->GetIndexValue();
											}
											dtObj = moduleSidxObj->GetDataType();
										}
									}
								}


								delete[] varSubIndex;

								if (uniqueidRefID != NULL)
								{
									INT32 actualMappedBytes = 0;
									actualMappedBytes = DecodeUniqueIDRef(uniqueidRefID, nodeObj, indexObj, sidxObj, moduleIndexObj, moduleSidxObj);
									LOG_DEBUG() << "iMappedLength:" << mappedLength << " actualMappedBytes:" << actualMappedBytes;
									if (mappedLength != (actualMappedBytes * 8))
									{										
										boost::format formatter(kMsgObjectSizeMappedInvalid);
										formatter % indexObj.GetIndex()
											% sidxObj->GetIndex() 
											% nodeObj->GetNodeId()
											% mappedLength
											% (actualMappedBytes * 8);
										exceptionObj.setErrorCode(OCFM_ERR_INVALID_SIZE_MAPPED);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();
										throw exceptionObj;
									}
								}
								else
								{
									ProcessImage piObj;
									piObj.Initialize();
									if (dtObj.GetName() == NULL)
									{
										boost::format formatter(kMsgNullArgument);
										formatter 
											% "'dtObj.GetName()'";
										exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();
										throw exceptionObj;
									}
									else if (!CheckAllowedDTForMapping(
										dtObj.GetName()))
									{
										boost::format formatter(kMsgPdoDatatypeInvalid);
										formatter 
											% nodeObj->GetNodeId()
											% indexObj.GetIndex()
											% sidxObj->GetIndexValue()
											% dtObj.GetName();
										exceptionObj.setErrorCode(OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();
										throw exceptionObj;
									}

									piObj.nodeId = nodeObj->GetNodeId();
									/* Name of the Process Image variable*/
									piObj.name = (char*) malloc(
											6 + ALLOC_BUFFER);
									strcpy(piObj.name,
											GetPIName(nodeObj->GetNodeId()));

									if (moduleName != NULL)
									{
										piObj.name =
												(char*) realloc(piObj.name,
														strlen(
																piObj.name)+ strlen(moduleName) + ALLOC_BUFFER);
										strcat(piObj.name, moduleName);

										piObj.moduleName = new char[strlen(
												moduleName) + STR_ALLOC_BUFFER];
										strcpy(piObj.moduleName, moduleName);
									}
									else
									{
										/*If the object name is empty, Ixxxx (xxxx is the index number in hex) */
										moduleName = new char[INDEX_LEN + 1
												+ ALLOC_BUFFER];
										strcpy(moduleName, "I");
										strcat(moduleName,
												indexObj.GetIndexValue());

										piObj.name =
												(char*) realloc(piObj.name,
														strlen(
																piObj.name)+ strlen(moduleName) + ALLOC_BUFFER);
										strcat(piObj.name, moduleName);

										piObj.moduleName = new char[strlen(
												moduleName) + STR_ALLOC_BUFFER];
										strcpy(piObj.moduleName, moduleName);
									}

									piObj.moduleIndex = new char[strlen(
											moduleIndexObj->GetIndexValue()) + STR_ALLOC_BUFFER];
									strcpy(piObj.moduleIndex, moduleIndexObj->GetIndexValue());

									if (objMapped)
									{
										piObj.varDeclName = new char[strlen(
												moduleName) + STR_ALLOC_BUFFER];
										strcpy(piObj.varDeclName, moduleName);
									}
									else
									{
										if (sidxName != NULL)
										{
											piObj.varDeclName =
													new char[strlen(sidxName)
															+ STR_ALLOC_BUFFER];
											strcpy(piObj.varDeclName, sidxName);

											piObj.name =
													(char*) realloc(piObj.name,
															(strlen(piObj.name)
																	+ strlen(
																			sidxName)
																	+ 1
																	+ ALLOC_BUFFER));
											strcat(piObj.name, ".");
											strcat(piObj.name, sidxName);
										}
										else
										{
											//TODO: pobjModulesubindex may be null. Check for null befor used.

											/*If the subobject name is empty, use Sxx (xx is the subindex number in hex) */
											sidxName = new char[SUBINDEX_LEN + 1
													+ ALLOC_BUFFER];

											strcpy(sidxName, "S");
											strcat(sidxName,
													moduleSidxObj->GetIndexValue());

											piObj.varDeclName =
													new char[strlen(sidxName)
															+ STR_ALLOC_BUFFER];
											strcpy(piObj.varDeclName, sidxName);

											piObj.name =
													(char*) realloc(piObj.name,
															(strlen(piObj.name)
																	+ strlen(
																			sidxName)
																	+ 1
																	+ ALLOC_BUFFER));
											strcat(piObj.name, ".");
											strcat(piObj.name, sidxName);

										}
									}
									LOG_DEBUG() << "DataSize: " << dtObj.dataSize * 8 << " MappedLength: " << mappedLength;
									if ((dtObj.dataSize * 8) != mappedLength)
									{										
										boost::format formatter(kMsgObjectSizeMappedInvalid);
										formatter % indexObj.GetIndex()
											% sidxObj->GetIndex() 
											% nodeObj->GetNodeId()
											% mappedLength
											% (dtObj.dataSize * 8);
										exceptionObj.setErrorCode(OCFM_ERR_INVALID_SIZE_MAPPED);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();
										delete[] sidxName;
										delete[] moduleName;
										delete[] accessStr;

										throw exceptionObj;
									}
									piObj.dataInfo.dataSize = dtObj.dataSize
											* 8;

									/* Total bytes Mapped */
									totalBytesMapped = totalBytesMapped
											+ dtObj.dataSize;
									if (totalBytesMapped > MAX_PI_SIZE)
									{
										boost::format formatter(kMsgMaxPiSizeExceeded);
										formatter
											% totalBytesMapped
											% MAX_PI_SIZE;
										exceptionObj.setErrorCode(OCFM_ERR_MAX_PI_SIZE);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();

										delete[] sidxName;
										delete[] accessStr;
										delete[] moduleName;
										throw exceptionObj;
									}
									/* Datatype in hex of the Process Image variable*/
									//TODO: dtObj.dataTypeName may be NULL
									piObj.dataInfo.dtName = new char[strlen(
											dtObj.dataTypeName)
											+ STR_ALLOC_BUFFER];
									strcpy(piObj.dataInfo.dtName,
											dtObj.dataTypeName);
									piObj.dataInfo.iecDtVar = dtObj.iecDataType;

									if (pdoType == PDO_TPDO)
									{
										piObj.byteOffset = ComputeINOffset(
												piObj.dataInfo.dataSize,
												pdoType);
										piObj.directionType = INPUT;
									}
									else if (pdoType == PDO_RPDO)
									{
										piObj.byteOffset = ComputeOUTOffset(
												piObj.dataInfo.dataSize,
												pdoType);
										piObj.directionType = OUTPUT;
									}

									piObj.bitOffset = 0;
									CreateMNPDOVar(piObj.byteOffset,
											piObj.dataInfo.dataSize,
											piObj.dataInfo.iecDtVar, pdoType,
											nodeObj);
									nodeObj->AddProcessImage(piObj);
									delete[] moduleName;
									delete[] sidxName;
								}
								
								LOG_INFO() << "Checking for MaxPayload: " << nodeMappedTotalBytes + mappedLength << " with 11920";
								if ((nodeMappedTotalBytes + mappedLength) > (atoi((const char*)abC_DLL_ISOCHR_MAX_PAYL) * 8))
								{
									boost::format formatter(kMsgChannelPayloadLimitExceeded);
									formatter 
										% nodeObj->GetNodeId()
										% indexObj.GetIndex()
										% (nodeMappedTotalBytes + mappedLength);
									exceptionObj.setErrorCode(OCFM_ERR_CHANNEL_PAYLOAD_LIMIT_EXCEEDED);
									exceptionObj.setErrorString(formatter.str());
									LOG_FATAL() << formatter.str();
									throw exceptionObj;
								}

								if ((true == isBuild)
										&& (strncmp(indexObj.GetIndexValue(),
												"16", 2) == 0)
										&& ((MN_NODEID == rpdoMappedNodeID)
												|| (BROADCAST_NODEID
														== rpdoMappedNodeID)))
								{
									char* modOffset = new char[strlen(actualVal) + 1];
									strcpy(modOffset, actualVal);
									char* offsetStr = new char[5];
									//memset(offsetStr, 0, 5 * sizeof(char));
									if (CHAINED == stnType)
									{
										offsetStr = IntToAscii(totalChainedBytesMapped, &(offsetStr[0]), 16);
									}
									else
									{
										offsetStr = IntToAscii(nodeMappedTotalBytes, &(offsetStr[0]), 16);
									}
									offsetStr = PadLeft(&(offsetStr[0]), '0', 4);

									for (INT32 offsetCopyCount = 0; offsetCopyCount <= 3; offsetCopyCount++)
									{
										modOffset[offsetCopyCount + 2 + 4] = offsetStr[offsetCopyCount];
									}

									if (CHAINED == stnType)
									{
										//Updating the total chained bits mapped.
										totalChainedBytesMapped = totalChainedBytesMapped + mappedLength;
										if ((totalChainedBytesMapped) > (atoi((const char*)abC_DLL_ISOCHR_MAX_PAYL) * 8))
										{							
											boost::format formatter(kMsgChannelPayloadLimitExceeded);
											formatter 
												% nodeObj->GetNodeId()
												% indexObj.GetIndex()
												% totalChainedBytesMapped;
											exceptionObj.setErrorCode(OCFM_ERR_CHANNEL_PAYLOAD_LIMIT_EXCEEDED);
											exceptionObj.setErrorString(formatter.str());
											LOG_FATAL() << formatter.str();
											delete[] modOffset;
											throw exceptionObj;
										}
									}

									//Set the correct offset for the RPDO
									indexCollObj->GetIndexbyIndexValue((char *) indexObjB4Sort->GetIndexValue())->GetSubIndexbyIndexValue((char *) sidxObj->GetIndexValue())->SetActualValue(modOffset);

									delete[] modOffset;
									delete[] offsetStr;
								}

								if (strncmp(indexObj.GetIndexValue(), "1A", 2) == 0)
								{
									INT32 mappedOffset = 0;
									char* mappingOffset = new char[INDEX_LEN];
									SubString(mappingOffset, actualVal, 6, 4);
									mappedOffset = HexToInt(mappingOffset);
									delete[] mappingOffset;
									LOG_INFO() << "Validating offset(decimal): " << "mappedOffset" << mappedOffset << " ChannelMappedTotalBits" << nodeMappedTotalBytes << " Sidx: " << sidxObj->GetIndexValue();
									if (mappedOffset != nodeMappedTotalBytes)
									{	//Mapping-Object 0x%X/0x%X on node %d: PDO offset invalid. Actual: %d bits, expected %d bits."
										boost::format formatter(kMsgPdoOffsetInvalid);
										formatter 
											% indexObj.GetIndex()
											% sidxObj->GetIndex()
											% nodeObj->GetNodeId()
											% mappedOffset
											% nodeMappedTotalBytes;
										exceptionObj.setErrorCode(OCFM_ERR_INVALID_PDO_OFFSET);
										exceptionObj.setErrorString(formatter.str());
										LOG_FATAL() << formatter.str();
										throw exceptionObj;
									}
								}

								//Updating the ChannelMapped Total bits.
								nodeMappedTotalBytes = nodeMappedTotalBytes + mappedLength;

								LOG_INFO() << "ChannelMappedTotalBytes:" << nodeMappedTotalBytes / 8 << " TotalChainedBytesMapped:" << totalChainedBytesMapped / 8;
							}
							else
							{
								boost::format formatter(kMsgAttributeValueInvalid);
								formatter % ""
									% ACTUALVALUE
									% indexObj.GetIndex()
									% sidxObj->GetIndex()
									% nodeObj->GetNodeId()
									% "non-empty";
								exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
								exceptionObj.setErrorString(formatter.str());
								LOG_FATAL() << formatter.str();
								throw exceptionObj;
							}
							sidxCount++;
						}
					}
				}
			}
			else
			{
				// pobjMNNode = pobjNode;
			}

		}
		//delete[] nodeIDbyStnArranged;
		//The PI variable name should be unique
		SetUniquePIVarName();
		//find the time of build
		SetBuildTime();

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

void CalculatePayload()
{

	NodeCollection *nodeCollObj = NULL;
	ostringstream errorString;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	/*Check RPDO Mapped objects*/

	if (nodeCollObj->GetCNNodesCount() == 0)
	{
		exit(0);
	}
	ocfmRetCode exceptionObj;
	Node *nodeObj = NULL;
	Node *nodeObjMN = NULL;
	INT32 totalChainedBytesMapped = 0;
	//INT32* nodeIdbyStnArranged = NULL;
	//nodeIdbyStnArranged = ArrangeNodeIDbyStation();
	for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes(); nodeLC++)
	{
		nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
		/* Process PDO Objects for CN*/

		if (nodeObj->GetNodeType() == MN)
		{
			nodeObjMN = nodeObj;
		}
		else
		{
			if (!(nodeObj->HasPdoObjects()))
			{
				continue;
			}

			StationType nodeStn = nodeObj->GetStationType();
			IndexCollection* pdoIdxCollObj = NULL;
			INT32 tpdoCount = 0;
			INT32 rpdoCount = 0;
			pdoIdxCollObj = nodeObj->getPDOIndexCollection(&rpdoCount,
					&tpdoCount);

			if (pdoIdxCollObj == NULL)
			{
				continue;
			}

			IndexCollection* indexCollObj = NULL;
			indexCollObj = nodeObj->GetIndexCollection();

			nodeObj->SetPReqActPayloadValue(0);
			nodeObj->SetPResActPayloadValue(0);

			for (INT32 idxLC = 0;
					idxLC < pdoIdxCollObj->GetNumberofIndexes(); idxLC++)
			{
				Index* indexObjB4Sort;
				Index indexObj;
				indexObjB4Sort = pdoIdxCollObj->GetIndexByPosition(idxLC);
				if (!(CheckIfMappingPDO(
						(char*) indexObjB4Sort->GetIndexValue())))
				{
					continue;
				}
				//	iNodeMappedTotalBytes = 0;

				if (indexObjB4Sort->GetNumberofSubIndexes() > 0)
				{
/*
Do not sort pdo subindexes by offset. The Sidx should start from 00 to FE. also the offset from 0000 for the subindex "0x01"
*/
					/* Sort the pdo collection */
					//indexObj = GetPDOIndexByOffset(indexObjB4Sort);
					indexObj = *indexObjB4Sort;
					INT32 sidxCount = 1; /* SubIndex processing should not include 00th subindex*/
					INT32 sidxTot = 0;

					SubIndex *sidxObj;
					sidxObj = indexObjB4Sort->GetSubIndexbyIndexValue(
							(char *) "00");
					if (NULL == sidxObj)
					{
						continue;
					}

					if ((sidxObj->GetActualValue() != NULL) // Actual value checked for Null
							&& (0
									!= strcmp(sidxObj->GetActualValue(),
											"")) // Actual value checked for Empty
							&& !(CheckIfValueZero(
									(char*) sidxObj->GetActualValue()))) // Actual value checked for non-zero
					{
						//value is not zero the channel is activated
						sidxTot = GetDecimalValue((char*) sidxObj->GetActualValue());
					}
					else
					{
						if (0 == strcmp(sidxObj->GetActualValue(), ""))
						{
							//pdo channel is deactivated. Empty act value
							continue;
						}
						if (CheckIfValueZero(
								(char*) sidxObj->GetActualValue()))
						{
							// PDO channel is deactivated
							// Zero is not set here,as it is intialised to Zero previously
							continue;
						}
						else // If the Actual values is Null or Empty, Default value is set for Total SIdx for mapping
						{
							//No need to check for value null or empty. GetDecimalValue returns zero or particular value.
							sidxTot = GetDecimalValue(
									(char*) sidxObj->GetDefaultValue());
						}
					}

					//Check isiTotal value is valid
					if (sidxTot
							>= (indexObjB4Sort->GetNumberofSubIndexes()))
					{		
						boost::format formatter(kMsgInsufficientMappingObjects);
						formatter % indexObjB4Sort->GetIndex()
							% nodeObj->GetNodeId() 
							% sidxTot 
							% (indexObjB4Sort->GetNumberofSubIndexes() - 1);
						exceptionObj.setErrorCode(OCFM_ERR_INSUFFICIENT_MAPPING_OBJECTS);
						exceptionObj.setErrorString(formatter.str());
						LOG_FATAL() << formatter.str();
						throw exceptionObj;
					}

					INT32 rpdoMappedNodeId = -1;
					if (strncmp(indexObj.GetIndexValue(), "16", 2) == 0)
					{
						Index *commIndexObj = NULL;
						char *indexId = new char[SUBINDEX_LEN];
						SubString(indexId, indexObj.GetIndexValue(), 2, 2);
						char *commIdxId = new char[INDEX_LEN + 1];
						strcpy(commIdxId, (char *) "14");
						strcat(commIdxId, indexId);
						commIndexObj = indexCollObj->GetIndexbyIndexValue(
								commIdxId);
						if (NULL != commIndexObj)
						{
							SubIndex *subIndexObj = NULL;
							subIndexObj =
									commIndexObj->GetSubIndexbyIndexValue(
											(char *) "01");
							if (NULL != subIndexObj)
							{
								rpdoMappedNodeId =
										GetDecimalValue(
												(char*) subIndexObj->GetActualValue());
							}
						}
						delete[] commIdxId;
						delete[] indexId;
					}

					while (sidxCount <= sidxTot)
					{

						SubIndex* subIndexObj = NULL;
						char *sidxId = new char[SUBINDEX_LEN];
						sidxId = IntToAscii(sidxCount, sidxId, 16);
						sidxId = PadLeft(sidxId, '0', 2);
						LOG_DEBUG() << " Position: " << sidxCount << " SidxId: "<< sidxId;
						//subIndexObj = indexObj.GetSubIndex(sidxCount);
						subIndexObj = indexObj.GetSubIndexbyIndexValue(sidxId);

						if (subIndexObj == NULL)
						{
							delete [] sidxId;
							continue;
						}
						delete [] sidxId;

						sidxCount++;

						if ((subIndexObj->GetActualValue() == NULL)
							|| (0 == strcmp(subIndexObj->GetActualValue(), ""))
							|| (CheckIfValueZero((char*) subIndexObj->GetActualValue())))
						{
							continue;
						}

						const char* actualValueStr = subIndexObj->GetActualValue();

						if ((strncmp(indexObj.GetIndexValue(), "16", 2) == 0)
							&& ((MN_NODEID == rpdoMappedNodeId)
							|| (BROADCAST_NODEID == rpdoMappedNodeId)))
						{
							//char* modOffsetVal = new char[strlen(actualValueStr) + 1];
							//strcpy(modOffsetVal, actualValueStr);
							//delete[] modOffsetVal;

							INT32 iLength = 0;
							INT32 iOffset = 0;

							char* lengthVal = new char[INDEX_LEN];
							lengthVal = SubString(lengthVal, actualValueStr, 2, 4);
							iLength = (INT32) HexToInt(lengthVal);
							delete[] lengthVal;

							char* offsetVal = new char[INDEX_LEN];
							offsetVal = SubString(offsetVal, actualValueStr, 6, 4);
							iOffset = (INT32) HexToInt(offsetVal);
							delete[] offsetVal;

							//	iNodeMappedTotalBytes = iOffset + iLength;

							if (CHAINED == nodeStn)
							{
								totalChainedBytesMapped = iOffset + iLength;
							}
							if (BROADCAST_NODEID == rpdoMappedNodeId)
							{
								nodeObj->SetPReqActPayloadValue(
										(iOffset + iLength) / 8);
							}
						}
						if (strncmp(indexObj.GetIndexValue(), "1A", 2) == 0)
						{
							//char* modOffset = new char[strlen(actualValueStr) + 1];
							//strcpy(modOffset, actualValueStr);
							//delete[] modOffset;

							INT32 len = 0;
							char* lengthStr = new char[INDEX_LEN];
							lengthStr = SubString(lengthStr, actualValueStr, 2, 4);
							len = (INT32) HexToInt(lengthStr);
							delete[] lengthStr;

							INT32 offsetVal = 0;
							char* varOffset = new char[INDEX_LEN];
							varOffset = SubString(varOffset, actualValueStr, 6, 4);
							offsetVal = (INT32) HexToInt(varOffset);
							delete[] varOffset;

							nodeObj->SetPResActPayloadValue(
									(offsetVal + len) / 8);
						}
					}
				}
			}
			UpdatePreqActLoad(nodeObj);
			UpdatePresActLoad(nodeObj);
		}
	}
	if ((NULL != nodeObjMN) && (true == IsPresMN()))
	{
		nodeObjMN->SetPResActPayloadValue(totalChainedBytesMapped / 8);
		UpdatePresActLoad(nodeObjMN);
	}
	//delete[] nodeIdbyStnArranged;
}

INT32 GetCNDataLen(const char* cdcBuffer)
{
	ocfmRetCode errCodeObj;
	INT32 count = 0;
	INT32 noOfChars = 0;
	INT32 position = 0;
 
	if (!cdcBuffer)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'cdcBuffer'";
		errCodeObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		errCodeObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw errCodeObj;
	}

	char readChar = cdcBuffer[position];

	while (readChar != '\0')
	{
		readChar = cdcBuffer[position];
		//Line with comment
		if (readChar == '/')
		{
			//traverse the comment
			while (readChar != '\n')
			{
				readChar = cdcBuffer[position];
				position++;
			}
		}
		else
		{
			//A - F , a - f , 0 - 9
			if ((readChar >= 65 && readChar <= 70) || (readChar >= 97 && readChar <= 102) || (readChar >= 48 && readChar <= 57))
			{
				noOfChars++;
			}
			position++;
			}
		}
		//For Byte Packing
	count = noOfChars / 2;
	return count;
}


Index* GetMNIndexValues(const char* indexId)
{
	NodeCollection* nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	IndexCollection* indexCollObj;
	Node *nodeObj;
//TODO: Review
	//CNode objNode;
	//objNode = pobjNodeCollection->getMNNode();
	//objIndexCol = objNode.getIndexCollection();
	nodeObj = nodeCollObj->GetNodePtr(MN, MN_NODEID);
	indexCollObj = nodeObj->GetIndexCollection();
	Index* indexObj = NULL;
	indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
	return indexObj;
}

SubIndex* GetMNSubIndexValues(char* indexId, char* subIndexId)
{
	NodeCollection* nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	IndexCollection* indexCollObj;
	Node nodeObj;

	nodeObj = nodeCollObj->GetMNNode();
	indexCollObj = nodeObj.GetIndexCollection();
	Index* indexObj = NULL;
	indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
	if (NULL == indexObj)
		return NULL;

	SubIndex* subIndexObj = NULL;
	subIndexObj = indexObj->GetSubIndexbyIndexValue(subIndexId);
	return subIndexObj;
}

void WriteXAPElements(ProcessImage piCollObj[], xmlTextWriterPtr& xmlWriter,
		INT32 varCount, PIDirectionType piType)
{

	if (varCount != 0)
	{
		INT32 highBitOffset = 0;
		INT32 highBitOffsetDatasize = 0;
		INT32 bytesWritten;
		/* Start an element named "Channel". Since thist is the first
		 * element, this will be the root element of the document. */
		bytesWritten = xmlTextWriterStartElement(xmlWriter,
				BAD_CAST "ProcessImage");
		if (bytesWritten < 0)
		{
			return;
		}
		bytesWritten = -1;
		if (piType == INPUT)
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "type", BAD_CAST "output");
		}
		else if (piType == OUTPUT)
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "type", BAD_CAST "input");
		}

		if (bytesWritten < 0)
		{
			return;
		}

		//find the size and write it to attribute
		for (INT32 varCountLC = 0; varCountLC < varCount; varCountLC++)
		{
			ProcessImage piObj;
			//piObj.Initialize();
			piObj = piCollObj[varCountLC];

			if (highBitOffset <= ((piObj.byteOffset * 8) + piObj.bitOffset))
			{
				//save the higher offset of process image and its datasize
				highBitOffset = (piObj.byteOffset * 8) + piObj.bitOffset; //multiply by 8 for byte to bit conversion
				highBitOffsetDatasize = piObj.dataInfo.dataSize;
			}
		}

		char *piSize = new char[20];
		piSize = IntToAscii((highBitOffset + highBitOffsetDatasize) / 8, piSize,
				10); //divide by 8 for bit to byte conversion
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "size",
				BAD_CAST piSize);
		delete[] piSize;
		if (bytesWritten < 0)
		{
			return;
		}

		for (INT32 varCountLC = 0; varCountLC < varCount; varCountLC++)
		{
			ProcessImage piObj;
			//piObj.Initialize();
			piObj = piCollObj[varCountLC];
			/* Start an element named "Channel". Since thist is the first
			 * element, this will be the root element of the document. */
			bytesWritten = xmlTextWriterStartElement(xmlWriter,
					BAD_CAST "Channel");
			if (bytesWritten < 0)
			{
				return;
			}

			stringstream moduleName;
			string moduleNr(piObj.moduleIndex);
			moduleName << "CN" << piObj.nodeId << ".M" << moduleNr.substr(2,4) << "." << piObj.moduleName << "." << piObj.varDeclName;
			/* Add an attribute with name "Name" and value to channel. */
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
				BAD_CAST "Name", BAD_CAST moduleName.str().c_str());

			if (bytesWritten < 0)
			{
				return;
			}
			/* Add an attribute with name "DataType" and value to channel */
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "dataType", BAD_CAST piObj.dataInfo.dtName);
			if (bytesWritten < 0)
			{
				return;
			}

			///* Add an attribute with name "dataSize" and value to channel */
			char * dataSize = new char[20];
			dataSize = IntToAscii(piObj.dataInfo.dataSize, dataSize, 10);

			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "dataSize", BAD_CAST dataSize);
			delete[] dataSize;
			if (bytesWritten < 0)
			{
				return;
			}

			///* Add an attribute with name "dataSize" and value to channel */
			char* byteOffset = new char[6];
			byteOffset = IntToAscii(piObj.byteOffset, byteOffset, 16);
			byteOffset = ConvertToHexformat(byteOffset, 4, 1);

			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "PIOffset", BAD_CAST byteOffset);
			delete[] byteOffset;
			if (bytesWritten < 0)
			{
				return;
			}

			if (piObj.bitOffset != -1)
			{
				char* bitOffset = new char[4];
				bitOffset = IntToAscii(piObj.bitOffset, bitOffset, 16);
				bitOffset = ConvertToHexformat(bitOffset, 2, 1);

				bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
						BAD_CAST "BitOffset", BAD_CAST bitOffset);
				delete[] bitOffset;
			}
			if (bytesWritten < 0)
			{
				return;
			}
			///* Close the element named Channel. */
			bytesWritten = xmlTextWriterEndElement(xmlWriter);
			if (bytesWritten < 0)
			{
				return;
			}
		}
		///* Close the element named ProcessImage. */
		bytesWritten = xmlTextWriterEndElement(xmlWriter);
		if (bytesWritten < 0)
		{
			return;
		}
	}
}

void StartXAPxml(xmlTextWriterPtr& xmlWriter, xmlDocPtr& xmlDocObj)
{
	INT32 bytesWritten;

	/* Create a new XmlWriter for DOM, with no compression. */
	xmlWriter = xmlNewTextWriterDoc(&xmlDocObj, 0);
	if (xmlWriter == NULL)
	{
		return;
	}

	/* Start the document with the xml default for the version,
	 * encoding UTF-8 and the default for the standalone
	 * declaration. */
	bytesWritten = xmlTextWriterStartDocument(xmlWriter, NULL, ENCODING,
			NULL);
	if (bytesWritten < 0)
	{
		return;
	}

	/* Write a comment as child of ORDER */
	char* commentStr = new char[strlen(BUILD_COMMENT) + BUILDTIME_BUF_LEN];
	strcpy(commentStr, BUILD_COMMENT);
	strcat(commentStr, GetBuildTime());
	bytesWritten = xmlTextWriterWriteComment(xmlWriter, BAD_CAST commentStr);
	delete[] commentStr;
	if (bytesWritten < 0)
	{
		return;
	}

	bytesWritten = xmlTextWriterStartElement(xmlWriter,
			BAD_CAST "ApplicationProcess");
	if (bytesWritten < 0)
	{
		return;
	}
}

void EndWritingXAP(xmlTextWriterPtr& xmlWriter, char* xmlFileName,
		xmlDocPtr& xmlDocObj)
{
	INT32 bytesWritten;

	// Close the element named ApplicationProcess.
	bytesWritten = xmlTextWriterEndElement(xmlWriter);
	if (bytesWritten < 0)
	{
		return;
	}
	bytesWritten = xmlTextWriterEndDocument(xmlWriter);
	if (bytesWritten < 0)
	{
		return;
	}

	xmlFreeTextWriter(xmlWriter);

	xmlSaveFormatFile((const char*) xmlFileName, xmlDocObj, 1);

	xmlFreeDoc(xmlDocObj);
}

ocfmRetCode GenerateXAP(const char* xapFilePath)
{
	NodeCollection* nodeCollObj = NULL;
	ocfmRetCode exceptionObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	assert(nodeCollObj);

	char* xapFileName = new char[strlen(xapFilePath) + 4 + ALLOC_BUFFER];
	ProcessImage* piInCollObj = NULL;
	ProcessImage* piOutCollObj = NULL;
	try
	{
		if (nodeCollObj->GetNumberOfNodes() == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}
		if (nodeCollObj->GetCNNodesCount() == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_CN_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoControlledNodesConfigured);
			LOG_FATAL() << kMsgNoControlledNodesConfigured;
			throw exceptionObj;
		}

		xmlTextWriterPtr xmlWriter = NULL;
		xmlDocPtr xmlDocObj = NULL;

		piInCollObj = new ProcessImage[(nodeCollObj->GetCNNodesCount() * PI_VAR_COUNT)];
		piOutCollObj = new ProcessImage[(nodeCollObj->GetCNNodesCount() * PI_VAR_COUNT)];
		GroupInOutPIVariables(piInCollObj, piOutCollObj);

		StartXAPxml(xmlWriter, xmlDocObj);
		if (inVarsGlobal != 0)
			WriteXAPElements(piInCollObj, xmlWriter, inVarsGlobal, INPUT);

		if (outVarsGlobal != 0)
			WriteXAPElements(piOutCollObj, xmlWriter, outVarsGlobal, OUTPUT);

		strcpy(xapFileName, xapFilePath);
		strcat(xapFileName, ".xml");
		EndWritingXAP(xmlWriter, xapFileName, xmlDocObj);

		/*Generate Header file */
		GenerateXAPHeaderFile(xapFilePath, piInCollObj, piOutCollObj,
				inVarsGlobal, outVarsGlobal);
		delete[] piInCollObj;
		delete[] piOutCollObj;
	} catch (ocfmRetCode& ex)
	{
		delete[] xapFileName;
		return ex;
	}
	delete[] xapFileName;
	return exceptionObj;
}

ocfmRetCode GenerateNET(const char* netFilePath)
{
	NodeCollection* nodeCollObj;
	ocfmRetCode exceptionObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	assert(nodeCollObj);
	ProcessImage* piInCollObj = NULL;
	ProcessImage* piOutCollObj = NULL;
	try
	{
		if (nodeCollObj->GetNumberOfNodes() == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}
		if (nodeCollObj->GetCNNodesCount() == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_CN_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoControlledNodesConfigured);
			LOG_FATAL() << kMsgNoControlledNodesConfigured;
			throw exceptionObj;
		}

		piInCollObj = new ProcessImage[(nodeCollObj->GetCNNodesCount() * PI_VAR_COUNT)];
		piOutCollObj = new ProcessImage[(nodeCollObj->GetCNNodesCount() * PI_VAR_COUNT)];
		GroupInOutPIVariables(piInCollObj, piOutCollObj);
		/*Generate Dot NET Header file */
		GenerateNETHeaderFile(netFilePath, piInCollObj, piOutCollObj,
				inVarsGlobal, outVarsGlobal);
		delete[] piInCollObj;
		delete[] piOutCollObj;
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

// FIXME: outAttributeValue should be refactored to be of type std::string&
ocfmRetCode GetIndexAttributes(INT32 nodeId, NodeType nodeType, const char* indexId,
		AttributeType attributeType, char* outAttributeValue)
{
	INT32 indexPos;
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	try
	{
		exceptionObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		//Index objIndex;
		Index* indexObj = NULL;

		//objIndex.SetNodeID(nodeObj.GetNodeId());
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		indexObj->SetNodeID(nodeObj.GetNodeId());

		DataType tempDTObj;

		switch (attributeType)
		{
		case NAME:
			if (indexObj->GetName() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetName());
			else
				strcpy(outAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (indexObj->GetObjectType() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetObjectType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DATATYPE:
			
			tempDTObj = indexObj->GetDataType();
			if (tempDTObj.dataTypeName != NULL)
				strcpy(outAttributeValue, tempDTObj.dataTypeName);
			else
				strcpy(outAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (indexObj->GetAccessType() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetAccessType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (indexObj->GetDefaultValue() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetDefaultValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (indexObj->GetActualValue() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetActualValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case PDOMAPPING:
			if (indexObj->GetPDOMapping() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetPDOMapping());
			else
				strcpy(outAttributeValue, "");
			break;
		case LOWLIMIT:
			if (indexObj->GetLowLimit() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetLowLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (indexObj->GetHighLimit() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetHighLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case FLAGIFINCDC:
			/* Flag if it should be included in cdc*/
			if (indexObj->GetFlagIfIncludedCdc() == true)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;
		default:
		{
			boost::format formatter(kMsgUnsupportedAttributeType);
			formatter % ((int) attributeType);
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		}
		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

ocfmRetCode GetIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos,
		AttributeType attributeType, char* outAttributeValue)
{
	ocfmRetCode exceptionObj;

	try
	{
		// Check for the Existance of the Node in the iNodePos
		Node nodeObj;
		NodeCollection *nodeCollObj;
		IndexCollection *indexCollObj;
		Index* indexObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();
		if (nodePos >= nodeCount)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}
		else if (nodeCount == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();
		if (indexCount == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeObj.GetNodeId();
			exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		else if (indexPos >= indexCount)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_INDEXPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		DataType tempDTObj;

		switch (attributeType)
		{
		case NAME:
			if (indexObj->GetName() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetName());
			else
				strcpy(outAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (indexObj->GetObjectType() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetObjectType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DATATYPE:
			
			tempDTObj = indexObj->GetDataType();
			if (tempDTObj.dataTypeName != NULL)
			{
				strcpy(outAttributeValue, tempDTObj.dataTypeName);
			}
			else
			{
				strcpy(outAttributeValue, "");
			}
			break;
		case ACCESSTYPE:
			if (indexObj->GetAccessType() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetAccessType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (indexObj->GetDefaultValue() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetDefaultValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (indexObj->GetActualValue() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetActualValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case PDOMAPPING:
			if (indexObj->GetPDOMapping() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetPDOMapping());
			else
				strcpy(outAttributeValue, "");
			break;
		case LOWLIMIT:
			if (indexObj->GetLowLimit() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetLowLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (indexObj->GetHighLimit() != NULL)
				strcpy(outAttributeValue, (char *) indexObj->GetHighLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case FLAGIFINCDC:
			/* Flag if it should be included in cdc*/
			if (indexObj->GetFlagIfIncludedCdc() == true)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;
		default:
		{
			boost::format formatter(kMsgUnsupportedAttributeType);
			formatter % ((int) attributeType);
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	return exceptionObj;
}

ocfmRetCode GetSubIndexAttributes(INT32 nodeId, NodeType nodeType,
		const char* indexId, const char* sidxId, AttributeType attributeType,
		char* outAttributeValue)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	try
	{
		INT32 sidxPos = -1;
		INT32 indexPos = -1;
		exceptionObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&sidxPos, &indexPos);
		if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << "IfSubIndexExists() returned '" << exceptionObj.getErrorCode() << "'.";
			boost::format formatter(kMsgSubIndexInvalid);
			formatter % HexToInt<UINT32>(string(indexId)) 
				% HexToInt<UINT32>(string(sidxId))
				% nodeId;
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXID);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}

		//Index objIndex;
		//objIndex.SetNodeID(nodeObj.GetNodeId());
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		Index* indexObj = NULL;
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		indexObj->SetNodeID(nodeObj.GetNodeId());

		SubIndex* subIndexObj = NULL;
		subIndexObj = indexObj->GetSubIndexByPosition(sidxPos);

		DataType tempDTObj;

		switch (attributeType)
		{
		case NAME:
			if (subIndexObj->GetName() != NULL)
				strcpy(outAttributeValue, (char *) subIndexObj->GetName());
			else
				strcpy(outAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (subIndexObj->GetObjectType() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetObjectType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DATATYPE:
			
			tempDTObj = subIndexObj->GetDataType();
			if (tempDTObj.dataTypeName != NULL)
				strcpy(outAttributeValue, tempDTObj.dataTypeName);
			else
				strcpy(outAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (subIndexObj->GetAccessType() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetAccessType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (subIndexObj->GetDefaultValue() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetDefaultValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (subIndexObj->GetActualValue() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetActualValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case PDOMAPPING:
			if (subIndexObj->GetPDOMapping() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetPDOMapping());
			else
				strcpy(outAttributeValue, (char*) "");
			break;
		case LOWLIMIT:
			if (subIndexObj->GetLowLimit() != NULL)
				strcpy(outAttributeValue, (char *) subIndexObj->GetLowLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (subIndexObj->GetHighLimit() != NULL)
				strcpy(outAttributeValue, (char *) subIndexObj->GetHighLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case FLAGIFINCDC:
			if (subIndexObj->GetFlagIfIncludedCdc() == true)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;
		default:
		{
			boost::format formatter(kMsgUnsupportedAttributeType);
			formatter % ((int) attributeType);
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		}
		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

ocfmRetCode GetSubIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos,
		INT32 subIndexPos, AttributeType attributeType, char* outAttributeValue)
{
	ocfmRetCode exceptionObj;
	try
	{
		// Check for the Existance of the Node in the iNodePos
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		Index* indexObj = NULL;


		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);

		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();
		if (nodePos >= nodeCount)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}
		else if (nodeCount == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();
		if (indexCount == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeObj.GetNodeId();
			exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		else if (indexPos >= indexCount)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_INDEXPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);

		INT32 subIndexCount = indexObj->GetNumberofSubIndexes();

		if (subIndexPos >= subIndexCount)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}
		else if (subIndexCount == 0)
		{
			boost::format formatter(kMsgIndexContainsNoSubIndices);
			formatter % indexObj->GetIndex() % nodeObj.GetNodeId();
			exceptionObj.setErrorCode(OCFM_ERR_NO_SUBINDEXS_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}

		SubIndex* subIndexObj = NULL;
		subIndexObj = indexObj->GetSubIndexByPosition(subIndexPos);

		DataType tempDTObj;

		switch (attributeType)
		{
		case NAME:
			if (subIndexObj->GetName() != NULL)
				strcpy(outAttributeValue, (char *) subIndexObj->GetName());
			else
				strcpy(outAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (subIndexObj->GetObjectType() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetObjectType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DATATYPE:
			
			tempDTObj = subIndexObj->GetDataType();
			if (tempDTObj.dataTypeName != NULL)
				strcpy(outAttributeValue, tempDTObj.dataTypeName);
			else
				strcpy(outAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (subIndexObj->GetAccessType() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetAccessType());
			else
				strcpy(outAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (subIndexObj->GetDefaultValue() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetDefaultValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (subIndexObj->GetActualValue() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetActualValue());
			else
				strcpy(outAttributeValue, "");
			break;
		case PDOMAPPING:
			if (subIndexObj->GetPDOMapping() != NULL)
				strcpy(outAttributeValue,
						(char *) subIndexObj->GetPDOMapping());
			else
				strcpy(outAttributeValue, "");
			break;
		case LOWLIMIT:
			if (subIndexObj->GetLowLimit() != NULL)
				strcpy(outAttributeValue, (char *) subIndexObj->GetLowLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (subIndexObj->GetHighLimit() != NULL)
				strcpy(outAttributeValue, (char *) subIndexObj->GetHighLimit());
			else
				strcpy(outAttributeValue, "");
			break;
		case FLAGIFINCDC:
			if (subIndexObj->GetFlagIfIncludedCdc() == true)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;

		default:
		{
			boost::format formatter(kMsgUnsupportedAttributeType);
			formatter % ((int) attributeType);
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	return exceptionObj;

}

//TODO: nodeId not used. to be removed in header also
ocfmRetCode GetNodeCount(UINT32* outNodeCount)
{
	NodeCollection *nodeCollObj = NULL;
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	try
	{
		if (!outNodeCount)
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'outNodeCount'";
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		if (nodeCollObj->GetNumberOfNodes() == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}

		*outNodeCount = nodeCollObj->GetNumberOfNodes();

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

//TODO: change the returning char* to delete varNodeIdStr
char* GetPIName(INT32 nodeID)
{
	char* nodeIdStr = NULL;
	char* nodeIdAsci = new char[4];

	nodeIdStr = new char[7];
	strcpy(nodeIdStr, "CN");
	nodeIdAsci = IntToAscii(nodeID, nodeIdAsci, 10);
	strcat(nodeIdStr, nodeIdAsci);
	strcat(nodeIdStr, ".");
	strcat(nodeIdStr, "\0");

	delete[] nodeIdAsci;
	return nodeIdStr;
}

char* GetParameterAccess(char* accessStr)
{
	if (!accessStr)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'accessStr'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	if (!strcmp(ConvertToUpper(accessStr), "READ"))
		return (char*) "ro\0";
	else if (!strcmp(ConvertToUpper(accessStr), "READWRITE"))
		return (char*) "rw\0";
	else
		return (char*) "xx";

}

ocfmRetCode GetIndexCount(UINT32 nodeId, NodeType nodeType, UINT32* outIndexCount)
{
	ocfmRetCode exceptionObj;

	if (!outIndexCount)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'outIndexCount'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	ocfmRetCode stErrorInfo(OCFM_ERR_UNKNOWN);

	try
	{
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		bool nodeExist = false;

		UINT32 nodePos;
		stErrorInfo = IfNodeExists(nodeId, nodeType, &nodePos, nodeExist);
		if (stErrorInfo.getErrorCode() != 0 && nodeExist != true)
		{
			boost::format formatter(kMsgNodeIdInvalid);
			formatter % nodeId;
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEID);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
		indexCollObj = nodeObj.GetIndexCollection();

		*outIndexCount = indexCollObj->GetNumberofIndexes();

		stErrorInfo.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return stErrorInfo;
}

ocfmRetCode GetSubIndexCount(INT32 nodeId, NodeType nodeType, const char* indexId, UINT32* outSubIndexCount)
{
	ocfmRetCode exceptionObj;

	if ((NULL == indexId) || (NULL == outSubIndexCount))
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexId', 'outSubIndexCount'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	try
	{
		INT32 indexPos;
		exceptionObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		//Index indexObj1;
		Index* indexObj = NULL;
		//indexObj1.SetNodeID(nodeObj.GetNodeId());
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		indexObj = indexCollObj->GetIndexByPosition(indexPos);
		indexObj->SetNodeID(nodeObj.GetNodeId());

		*outSubIndexCount = indexObj->GetNumberofSubIndexes();

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

void LoadObjectDictionary(const char* xmlFilePath)
{

	xmlTextReaderPtr xmlReader;

	ObjectDictionary* dictObj;
	dictObj = ObjectDictionary::GetObjDictPtr();
	/*
	 xmlTextReaderPtr xmlReaderForFile (const char * filename, const char * encoding, int options)
	 Returns: the new reader or NULL in case of error.
	 */

	xmlReader = xmlReaderForFile(xmlFilePath, NULL, 0);
	if (xmlReader != NULL)
	{
		INT32 retVal = 0;
		/*
		 int xmlTextReaderRead(xmlTextReaderPtr reader)
		 Return:   1 if the node was read successfully, 0 if there is no more nodes to read, or -1 in case of error
		 */
		retVal = xmlTextReaderRead(xmlReader);
		while (retVal == 1)
		{
			//const xmlChar* pxcName	= NULL;
			//const xmlChar* pxcValue	= NULL;

			//pxcName = xmlTextReaderConstName(pxReader);

			//pxcValue = xmlTextReaderConstValue(pxReader);
			dictObj->ProcessObjectDictionary(xmlReader);
			retVal = xmlTextReaderRead(xmlReader);
		}
		if (retVal != 0)
		{
			//continue with process
		}
	}
	xmlCleanupParser();
	/*
	 * this is to debug memory for regression tests
	 */
	xmlMemoryDump();
}

ocfmRetCode GetNodeAttributesbyNodePos(UINT32 nodePos, INT32* outNodeId,
		char* outNodeName, StationType* outStationType, char* outForcedCycle,
		bool* outIsForcedCycle)
{
	ocfmRetCode exceptionObj;

	if ((NULL == outNodeId) || (NULL == outNodeName) || (NULL == outStationType)
			|| (NULL == outForcedCycle) || (NULL == outIsForcedCycle))
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'outNodeId', 'outNodeName', 'outStationType', 'outForcedCycle', 'outIsForcedCycle'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	UINT32 nodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		exceptionObj = GetNodeCount(&nodeCount);

		if (nodePos > nodeCount)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}
		else if (nodeCount == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}

		Node nodeObj;
		NodeCollection *nodeCollObj;
		//IndexCollection *pobjIndexCollection;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);

		//pobjIndexCollection = objNode.getIndexCollection();

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		*outNodeId = nodeObj.GetNodeId();
		if (nodeObj.GetNodeName() != NULL)
		{
			strcpy(outNodeName, nodeObj.GetNodeName());
		}
		else
		{
			outNodeName = NULL;
		}

		if (nodeObj.GetForcedCycleValue() != NULL)
		{
			strcpy(outForcedCycle, nodeObj.GetForcedCycleValue());
		}
		else
		{
			outForcedCycle = NULL;
		}

		*outStationType = nodeObj.GetStationType();
		*outIsForcedCycle = nodeObj.GetForceCycleFlag();

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

ocfmRetCode GetIndexIDbyIndexPos(INT32 nodeId, NodeType nodeType,
		INT32 indexPos, char* outIndexId)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	Node nodeObj;
	NodeCollection *nodeCollObj;
	IndexCollection *indexCollObj;
	Index* indexObj;

	UINT32 nodePos;
	try
	{
		bool existFlag = false;
		exceptionObj = IfNodeExists(nodeId, nodeType, &nodePos, existFlag);
		if (exceptionObj.getErrorCode() != 0 && existFlag != true)
		{
			// Node Doesn't Exist
			boost::format formatter(kMsgNodeIdInvalid);
			formatter % nodeId;
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEID);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();
		if (indexCount == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeObj.GetNodeId();
			exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		else if (indexCount < indexPos)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_INDEXPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);

		if (indexObj->GetIndexValue() != NULL)
			strcpy(outIndexId, (char *) indexObj->GetIndexValue());

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

ocfmRetCode GetIndexIDbyPositions(INT32 nodePos, INT32 indexPos,
		char* outIndexID)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	Node nodeObj;
	NodeCollection *nodeCollObj;
	IndexCollection *indexCollObj;
	Index* indexObj;

	try
	{
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();

		if (nodeCount == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}
		else if (nodeCount < nodePos)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();

		if (indexCount == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeObj.GetNodeId();
			exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		else if (indexCount < indexPos)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_INDEXPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);

		if (indexObj->GetIndexValue() != NULL)
			strcpy(outIndexID, (char *) indexObj->GetIndexValue());

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;

}

ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 nodeId, NodeType nodeType,
		const char* indexId, INT32 subIndexPos, char* outSubIndexID)
{
	Node nodeObj;
	NodeCollection *nodeCollObj;
	IndexCollection *indexCollObj;
	//Index objIndex;
	Index* indexObj;
	ocfmRetCode stErrorInfo;

	try
	{
		INT32 indexPos;
		stErrorInfo = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (stErrorInfo.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() << stErrorInfo.getErrorString();
			throw stErrorInfo;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		indexObj = indexCollObj->GetIndexByPosition(indexPos);

		INT32 subIndexCount = indexObj->GetNumberofSubIndexes();
		if (subIndexCount == 0)
		{
			boost::format formatter(kMsgIndexContainsNoSubIndices);
			formatter % indexObj->GetIndex() % nodeObj.GetNodeId();
			stErrorInfo.setErrorCode(OCFM_ERR_NO_SUBINDEXS_FOUND);
			stErrorInfo.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw stErrorInfo;
		}
		else if (subIndexCount < subIndexPos)
		{
			stErrorInfo.setErrorCode(OCFM_ERR_INVALID_SUBINDEXPOS);
			LOG_FATAL() << stErrorInfo.getErrorString();
			throw stErrorInfo;
		}

		SubIndex *sidxObj = NULL;
		sidxObj = indexObj->GetSubIndexByPosition(subIndexPos);

		if (sidxObj->GetIndexValue() != NULL)
			strcpy(outSubIndexID, (char *) sidxObj->GetIndexValue());

		stErrorInfo.setErrorCode(OCFM_ERR_SUCCESS);

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return stErrorInfo;
}

ocfmRetCode GetSubIndexIDbyPositions(INT32 nodePos, INT32 indexPos,
		INT32 subIndexPos, char* outSubIndexID)
{
	Node nodeObj;
	NodeCollection *nodeCollObj = NULL;
	IndexCollection *indexCollObj = NULL;
	//CIndex objIndex;
	Index* indexObj = NULL;
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	try
	{
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();

		if (nodeCount == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoNodesConfigured);
			LOG_FATAL() << kMsgNoNodesConfigured;
			throw exceptionObj;
		}
		else if (nodeCount < nodePos)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 iTempIndexCount = indexCollObj->GetNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeObj.GetNodeId();
			exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		else if (iTempIndexCount < indexPos)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_INDEXPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndexByPosition(indexPos);

		INT32 subIndexCount = indexObj->GetNumberofSubIndexes();
		if (subIndexCount == 0)
		{
			boost::format formatter(kMsgIndexContainsNoSubIndices);
			formatter % indexObj->GetIndex() % nodeObj.GetNodeId();
			exceptionObj.setErrorCode(OCFM_ERR_NO_SUBINDEXS_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}
		else if (subIndexCount < subIndexPos)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_SUBINDEXPOS);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		SubIndex* sidxObj = NULL;
		sidxObj = indexObj->GetSubIndexByPosition(subIndexPos);

		if (sidxObj->GetIndexValue() != NULL)
			strcpy(outSubIndexID, (char *) sidxObj->GetIndexValue());

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

ocfmRetCode DeleteNodeObjDict(INT32 nodeId, NodeType nodeType)
{
	UINT32 nodePos;
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	try
	{
		
		bool existFlag = false;
		exceptionObj = IfNodeExists(nodeId, nodeType, &nodePos, existFlag);

		if (exceptionObj.getErrorCode() != 0 && existFlag != true)
		{
			boost::format formatter(kMsgNodeIdInvalid);
			formatter % nodeId;
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEID);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}
		//CNode objNode;
		Node* nodeObj = NULL;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		DataTypeCollection *dataTypeCollObj = NULL;
		Index indexObj;
		//SubIndex pobjSubIndex;
		NetworkManagement *nmtObj = NULL;
		ApplicationProcess *appProcessObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		assert(nodeCollObj);
		nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
		//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID); // (varNodeType, iNodeID);
		indexObj.SetNodeID(nodeObj->GetNodeId());
		//objSubIndex.SetNodeID(objNode->GetNodeId());
		dataTypeCollObj = nodeObj->GetDataTypeCollection();

		indexCollObj = nodeObj->GetIndexCollection();
		nmtObj = nodeObj->GetNetworkManagement();
		appProcessObj = nodeObj->GetApplicationProcess();
		// Delete IndexCollection

		Index* indexObj1 = NULL;
		INT32 noOfIndex = indexCollObj->GetNumberofIndexes();

		for (INT32 count = 0; count < noOfIndex; count++)
		{
			indexObj1 = indexCollObj->GetIndexByPosition(count);
			if (indexObj1->GetNumberofSubIndexes() > 0)
			{
				indexObj1->DeleteSubIndexCollection();
			}
		}

		indexCollObj->DeleteIndexCollection();
		// Delete DataTypeCollection
		dataTypeCollObj->DeleteDataTypeCollection();
		//Delete Network management collectionObj
		nmtObj->DeleteFeatureCollections();
		//Delete ComplexDataTypeCollection
		appProcessObj->DeleteParameterCollection();
		appProcessObj->DeleteComplexDataTypeCollection();
		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& exCatch)
	{
		return exCatch;
	}
	return exceptionObj;
}

ocfmRetCode SaveProject(const char* projectPath, const char* projectName)
{
	//Node nodeObj;
	//NodeCollection *nodeCollObj = NULL;
	ocfmRetCode exceptionObj;
//	int retVal;
//	char* tempPath;
//	tempPath = new char[(strlen(projectPath)) + (strlen(projectName))
//			+ strlen("cdc_xap") + ALLOC_BUFFER];
//
//	try
//	{
//		char* tempPjtName;
//		struct stat fileInfo;
//		INT32 retStatus;
//
//		tempPjtName = new char[300];
//		sprintf(tempPjtName, "%s%s/%s.oct", projectPath, projectName,
//				projectName);
//
//		retStatus = stat(tempPjtName, &fileInfo);
//		delete[] tempPjtName;
//		if (retStatus == 0)
//		{
//#if defined DEBUG
//			cout << "\n\n\nProject File Already exists\n" << endl;
//#endif
//		}
//		else
//		{
//#if defined(_WIN32) && defined(_MSC_VER)
//			{
//				sprintf(tempPath, "%s\\%s", projectPath, projectName);
//				retVal = _mkdir(tempPath);
//			}
//#else
//			{
//				sprintf(tempPath, "%s/%s", projectPath, projectName);
//				mkdir(tempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
//			}
//#endif
//		}
//
//#if defined(_WIN32) && defined(_MSC_VER)
//		{
//			sprintf(tempPath, "%s\\%s\\%s", projectPath, projectName,
//					"cdc_xap");
//			retVal = _mkdir(tempPath);
//		}
//#else
//		{
//			sprintf(tempPath, "%s/%s/%s", projectPath, projectName, "cdc_xap");
//			mkdir(tempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
//		}
//#endif
//
//		SaveProjectXML(projectPath, projectName);
//		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//		if (nodeCollObj == NULL)
//		{
//			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
//			throw exceptionObj;
//		}
//		if (nodeCollObj->GetNumberOfNodes() > 0)
//		{
//			for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
//					nodeLC++)
//			{
//				nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);
//				char* fullFileName;
//				fullFileName = new char[MAX_FILE_PATH_SIZE];
//#if defined(_WIN32) && defined(_MSC_VER)
//				{
//					sprintf(tempPath, "%s\\%s\\octx", projectPath, projectName);
//					struct stat tempFileInfo;
//
//					retStatus = stat(tempPath, &tempFileInfo);
//					if (retStatus == 0)
//					{
//
//#if defined DEBUG
//						cout << "Folder Already Exists\n" << endl;
//#endif
//					}
//					else
//					{
//						retVal = _mkdir(tempPath);
//					}
//					// Saves the nodes with their nodeId as the name
//					sprintf(fullFileName, "%s\\%d.octx", tempPath,
//							nodeObj.GetNodeId());
//				}
//#else
//				{
//					struct stat fileInfo;
//
//					sprintf(tempPath, "%s/%s/octx", projectPath, projectName);
//					retStatus = stat(tempPath, &fileInfo);
//					if(retStatus == 0)
//					{
//#if defined DEBUG
//						cout << "Folder Already Exists\n" << endl;
//#endif
//					}
//					else
//					{
//						mkdir(tempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
//					}
//					// Saves the nodes with their nodeId as the name
//					sprintf(fullFileName, "%s/%d.octx", tempPath, nodeObj.GetNodeId());
//				}
//#endif				
//				SaveNode(fullFileName, nodeObj.GetNodeId(),
//						nodeObj.GetNodeType());
//				delete[] fullFileName;
//
//			}
//			exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
//		}
//		else
//		{
//			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
//			throw exceptionObj;
//		}
//	} catch (ocfmRetCode& ex)
//	{
//		delete[] tempPath;
//		return ex;
//	}
//	delete[] tempPath;
	return exceptionObj;
}

void GetMNPDOSubIndex(MNPdoVariable mnPdoVarObj, INT32& prevSubIndex,
		Index* indexObj, char* indexId, INT32 prevSize)
{
	ocfmRetCode exceptionObj;

	if ((NULL == indexObj) || (NULL == indexId))
	{
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		LOG_FATAL() << "Parameter 'indexObj', 'indexId' must not be NULL.";
		throw exceptionObj;
	}

	SubIndex* subIndexObj = NULL;
	char* tempIndexId = new char[3];
	/* Set the MN's PDO subIndex*/
	prevSubIndex = prevSubIndex + 1;
	tempIndexId = IntToAscii(prevSubIndex, tempIndexId, 16);
	tempIndexId = PadLeft(tempIndexId, '0', 2);

	LOG_DEBUG() << " index " << tempIndexId;
	LOG_DEBUG() <<  "subindex " << prevSubIndex;
	if (CheckIfSubIndexExists(MN_NODEID, MN, indexId, tempIndexId))
	{
		subIndexObj = indexObj->GetSubIndexbyIndexValue(tempIndexId);
	}
	else
	{
		AddSubIndex(MN_NODEID, MN, indexId, tempIndexId);
		subIndexObj = indexObj->GetSubIndexbyIndexValue(tempIndexId);
	}
	delete[] tempIndexId;
	if (!subIndexObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'subIndexObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	/* Calculate the actual value of MN PDO */
	char* actValue = new char[20];
	SubString(actValue, (const char*) mnPdoVarObj.value, 0, 6);
	char* offsetVal = new char[5];

	offsetVal = IntToAscii(prevSize, offsetVal, 16);
	offsetVal = PadLeft(offsetVal, '0', 4);
	offsetVal = ConvertToUpper(offsetVal);
	strcat(actValue, offsetVal);
	delete[] offsetVal;

	/* Add reserve byte*/
	strcat(actValue, "00");

	strcat(actValue, mnPdoVarObj.subIndexId);
	strcat(actValue, mnPdoVarObj.indexId);

	subIndexObj->SetActualValue(actValue);
	subIndexObj->SetFlagIfIncludedCdc(true);
	delete[] actValue;

	AddPDOIndexsToMN(mnPdoVarObj.indexId, mnPdoVarObj.subIndexId,
			mnPdoVarObj.pdoType);
}

void SetSIdxValue(char* indexId, char* sidxId, char* value,
		IndexCollection *indexCollObj, INT32 nodeId, NodeType nodeType,
		bool setDefaultValue)
{
	//TODO: varValue (3rd parameter) to be null checked. Empty value sent in generateotherMNindexes function
	if ((NULL == indexId) || (NULL == sidxId) || (NULL == indexCollObj))
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexId', 'sidxId', 'indexCollObj'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	Index *indexObj = NULL;
	SubIndex* sidxObj = NULL;
	ocfmRetCode stRetInfo;

	indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
	if (CheckIfSubIndexExists(nodeId, nodeType, indexId, sidxId))
	{
		sidxObj = indexObj->GetSubIndexbyIndexValue(sidxId);
		if (setDefaultValue)
			sidxObj->SetActualValue((char*) sidxObj->GetDefaultValue());
		else
			sidxObj->SetActualValue(value);
		sidxObj->SetFlagIfIncludedCdc(true);
	}
	else
	{
		stRetInfo = AddSubIndex(nodeId, nodeType, indexId, sidxId);
		sidxObj = indexObj->GetSubIndexbyIndexValue(sidxId);
		sidxObj->SetFlagIfIncludedCdc(true);
		if (setDefaultValue)
			sidxObj->SetActualValue((char*) sidxObj->GetDefaultValue());
		else
		{
			sidxObj->SetActualValue(value);
		}
	}
}

void AddForEachSIdx(char *indexId, IndexCollection *indexCollObj, INT32 nodeId,
		char *value, bool setDefaultValue)
{
	NodeCollection *nodeCollObj = NULL;
	Node nodeObj;
	//Index *pobjIndex;
	char *sidxId = new char[3];
	char *indexNo = new char[3];
	char *hexIndexNo = new char[5];
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);
	//pobjIndex = pobjIdxCol->getIndexbyIndexValue(varIdx);
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	//TODO: Stack wasnt booting up with sub index 00 for 1C09, 1F26
	if ((strcmp("1C09", indexId) == 0) || (strcmp("1F26", indexId) == 0)
			|| (strcmp("1F8B", indexId) == 0) || (strcmp("1F8D", indexId) == 0)
			|| (strcmp("1F27", indexId) == 0) || (strcmp("1F84", indexId) == 0))
	{
		strcpy(sidxId, "00");
		exceptionObj = DeleteSubIndex(nodeId, MN, indexId, sidxId);
	}
	else
	{
		if (nodeCollObj->GetCNNodesCount() != 0)
		{
			strcpy(sidxId, "00");
			strcpy(hexIndexNo, "0x");

			indexNo = IntToAscii(nodeCollObj->GetCNNodesCount(), indexNo, 16);
			indexNo = PadLeft(indexNo, '0', 2);
			strcat(hexIndexNo, indexNo);
			SetSIdxValue(indexId, sidxId, hexIndexNo, indexCollObj, nodeId, MN,
					false);
		}
	}

	for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes(); nodeLC++)
	{
		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);

		if (nodeObj.GetNodeType() == CN)
		{
			sidxId = IntToAscii(nodeObj.GetNodeId(), sidxId, 16);
			sidxId = PadLeft(sidxId, '0', 2);

			if (strcmp("1F84", indexId) == 0)
			{
				Index* indexObj = NULL;
				//SubIndex objCNSIdx;
				IndexCollection* indexCollObjCN = NULL;
				char* tempIndexId = new char[INDEX_LEN];
				indexCollObjCN = nodeObj.GetIndexCollection();

				strcpy(tempIndexId, "1000");
				indexObj = indexCollObjCN->GetIndexbyIndexValue(tempIndexId);

				if (indexObj != NULL)
				{
					if (indexObj->GetActualValue() != NULL)
					{
						SetSIdxValue(indexId, sidxId,
								(char*) indexObj->GetActualValue(),
								indexCollObj, nodeId, MN, false);
					}
					else
					{
						SetSIdxValue(indexId, sidxId,
								(char*) indexObj->GetActualValue(),
								indexCollObj, nodeId, MN, true);
					}
				}
				delete[] tempIndexId;

			}
			else
			{
				SetSIdxValue(indexId, sidxId, value, indexCollObj, nodeId, MN,
						setDefaultValue);
			}

		}

	}
	delete[] hexIndexNo;
	delete[] indexNo;
	delete[] sidxId;
}

ocfmRetCode GenerateMNOBD(bool IsBuild)
{

	Node *nodeObjMN = NULL;
	NodeCollection *nodeCollObj = NULL;
	IndexCollection *indexCollObj = NULL;
	ostringstream errorString;

	char* indexIdMN = new char[INDEX_LEN];
	char* sidxId = new char[SUBINDEX_LEN];
	char* mappingSidxId = new char[SUBINDEX_LEN];

	ocfmRetCode exceptionObj;
	INT32 prevSubIndex = 0;
	INT32 outPrevSize = 0;
	INT32 chainOutPrevSubIndex = 0;
	INT32 chainOutPrevSize = 0;
	INT32 inPrevSubIndex = 0;
	INT32 inPrevSize = 0;
	INT32 indexPos = 0;
	INT32 rxChannelCount = 0;
	INT32 txChannelCount = 0;
	INT32 maxNoOfChannels = 0;

	try
	{
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObjMN = nodeCollObj->GetNodePtr(MN, MN_NODEID);
		if (nodeObjMN == NULL)
		{
			exceptionObj.setErrorCode(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			exceptionObj.setErrorString(kMsgNoManagingNodeConfigured);
			LOG_FATAL() << kMsgNoManagingNodeConfigured;
			throw exceptionObj;
		}
		else
		{
			/*Process PDO Nodes*/
			exceptionObj = ProcessPDONodes(IsBuild);
			LOG_INFO() << "PDO's in CNs processed.";
			if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				return exceptionObj;
			}

			//CNode objMNNode;
			//CNodeCollection *pobjNodeCollection;
			IndexCollection *pobjIndexCollection = NULL;
			//CIndex objIndex;

			//pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
			//objMNNode = pobjNodeCollection->getNode(MN, MN_NODEID);
			//pobjIndexCollection = objMNNode.getIndexCollection();

			NetworkManagement *nmtObj = NULL;
			nmtObj = nodeObjMN->GetNetworkManagement();
			maxNoOfChannels = nmtObj->GetMaxPDOCount();
			LOG_INFO() << "Max no. of TPDO-Channels = " << maxNoOfChannels;

			/* Delete the MN's old object dictionary*/
			pobjIndexCollection = nodeObjMN->GetIndexCollection();
			//DO not delete and try to reset PDO indexes.
			//			pobjIndexCollection->DeletePDOs();
			ResetAllPdos(nodeObjMN->GetNodeId(), nodeObjMN->GetNodeType());
			// Delete Process Image Objects
			pobjIndexCollection->DeletePIObjects();
			LOG_INFO() << "Deleted PI objects (Axxx indices) in MN.";
			// Autogenertate other indexs 
			AutogenerateOtherIndexs(nodeObjMN);
			/* Add other Indexes than PDO*/
			LOG_INFO() << "Autogenerated other indices in MN.";
		}

		bool isPresMnVal = false;
		isPresMnVal = IsPresMN();

		if (true == isPresMnVal)
		{
			LOG_INFO() << "PresMN: Setting txChannelCount to 1";
			txChannelCount = 1;
		}
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObjMN = nodeCollObj->GetNodePtr(MN, MN_NODEID);

//		INT32* arrangedNodeIDbyStation = NULL;
		Node nodeObj;
//Commented because the below code is not using the arranged node id..
//		arrangedNodeIDbyStation = ArrangeNodeIDbyStation();
		LOG_INFO() << "NodeID arranged by station.";
		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{
			nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);

			StationType currCNStation;
			if (nodeObj.GetNodeType() == CN)
			{

				char* versionNumber = new char[INDEX_LEN];
				versionNumber[0] = 0;
				currCNStation = nodeObj.GetStationType();

				nodeObjMN = nodeCollObj->GetNodePtr(MN, MN_NODEID);
				indexCollObj = nodeObjMN->GetIndexCollection();

				if (nodeObj.MNPDOOUTVarCollection.size() != 0)
				{
					if (CHAINED != currCNStation)
					{
						prevSubIndex = 0;
						outPrevSize = 0;
					}
					else
					{
						prevSubIndex = chainOutPrevSubIndex;
						outPrevSize = chainOutPrevSize;
					}
					/* Create PDO_TxCommParam_XXh_REC 1800 Index*/
					Index* indexObj;
					indexPos = 0;
					char* mappNodeID = new char[INDEX_LEN];
					strcpy(indexIdMN, "18");
					if (CHAINED != currCNStation)
					{
						sidxId = IntToAscii(txChannelCount, sidxId, 16);
						txChannelCount++;

						sidxId = PadLeft(sidxId, '0', 2);
						indexIdMN = strcat(indexIdMN, sidxId);
						exceptionObj = IfIndexExists(MN_NODEID, MN, indexIdMN, &indexPos);
						if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
						{
							exceptionObj = AddIndex(MN_NODEID, MN, indexIdMN);
						}

						//to write cn node id in 18XX/01
						mappNodeID = IntToAscii(nodeObj.GetNodeId(), mappNodeID, 10);
					}
					else
					{
						//1800 is used of PRes chained station
						strcpy(indexIdMN, (char *) "1800");
						strcpy(sidxId, (char *) "00");
						exceptionObj = IfIndexExists(MN_NODEID, MN, indexIdMN, &indexPos);
						if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
						{
							exceptionObj = AddIndex(MN_NODEID, MN, indexIdMN);
						}

						//to write 0 in 18XX/01 to indicate PRes MN
						strcpy(mappNodeID, (char *) "0x0");
					}
					/* set bFlag to true for 1800*/
					indexObj = indexCollObj->GetIndexbyIndexValue(indexIdMN);

					if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
					{
						//delete[] arrangedNodeIDbyStation;
						LOG_ERROR() << "Error: " << exceptionObj.getErrorCode();
						throw exceptionObj;
					}

					char* tempSidxId = new char[SUBINDEX_LEN];
					strcpy(tempSidxId, "01");
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN, tempSidxId, mappNodeID, (char*) "NodeID_U8", true);
					delete[] mappNodeID;

					GetSubIndexAttributes(nodeObj.GetNodeId(), CN, (char*) "1400", (char*) "02", ACTUALVALUE, versionNumber);
					if ((NULL == versionNumber)
							|| (strcmp(versionNumber, "") == 0))
					{
						strcpy(versionNumber, "0x0");
					}
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN, (char*) "02", versionNumber, (char*) "MappingVersion_U8", true);

					strcpy(indexIdMN, "1A");
					strcat(indexIdMN, sidxId);
					/* Set the MN's PDO Index*/
					indexPos = 0;
					exceptionObj = IfIndexExists(MN_NODEID, MN, indexIdMN, &indexPos);
					if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
					{
						exceptionObj = AddIndex(MN_NODEID, MN, indexIdMN);
					}

					indexObj->SetFlagIfIncludedCdc(true);

					if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
					{
						//delete[] arrangedNodeIDbyStation;
						LOG_ERROR() << "Error: " << exceptionObj.getErrorCode();
						throw exceptionObj;
					}
					UINT32 pdoOutLC = 0;
					for (pdoOutLC = 0; pdoOutLC < nodeObj.MNPDOOUTVarCollection.size(); pdoOutLC++)
					{
						MNPdoVariable mnPDOObj;
						mnPDOObj = nodeObj.MNPDOOUTVarCollection[pdoOutLC];
						indexObj = indexCollObj->GetIndexbyIndexValue(indexIdMN);
						if (prevSubIndex >= 252)
						{						
							boost::format formatter(kMsgChannelObjectLimitExceeded);
							formatter 
								% nodeObj.GetNodeId()
								% indexObj->GetIndex()
								% 255;
							exceptionObj.setErrorCode(OCFM_ERR_CHANNEL_OBJECT_LIMIT_EXCEEDED);
							exceptionObj.setErrorString(formatter.str());
							LOG_FATAL() << formatter.str();
							throw exceptionObj;
						}
	
						if((outPrevSize + mnPDOObj.dataSize) > (atoi((const char*) abC_DLL_ISOCHR_MAX_PAYL) * 8))
						{							
							boost::format formatter(kMsgIsochronousMaxPayloadExceeded);
							formatter 
								% nodeObj.GetNodeId()
								% "TPDO"
								% (outPrevSize + mnPDOObj.dataSize);
							exceptionObj.setErrorCode(OCFM_ERR_CHANNEL_PAYLOAD_LIMIT_EXCEEDED);
							exceptionObj.setErrorString(formatter.str());
							LOG_FATAL() << formatter.str();
							throw exceptionObj;
						}

						if (indexObj != NULL)
						{
							indexObj->SetFlagIfIncludedCdc(true);
							GetMNPDOSubIndex(mnPDOObj, prevSubIndex, indexObj, indexIdMN, outPrevSize);
							outPrevSize = outPrevSize + mnPDOObj.dataSize;
						}
					}
					SubIndex *sidxMNobj = NULL;
					sidxMNobj = indexObj->GetSubIndexbyIndexValue((char*) "00");
					if (NULL != sidxMNobj)
					{
						char *tempStr = new char[INDEX_LEN];
						tempStr = IntToAscii(prevSubIndex, tempStr, 10);
						LOG_INFO() << "Setting actualValue: " << tempStr << " in index: " <<indexObj->GetIndexValue();
						sidxMNobj->SetActualValue(tempStr);
						delete[] tempStr;
					}

					if (CHAINED == currCNStation)
					{
						chainOutPrevSubIndex = prevSubIndex;
						chainOutPrevSize = outPrevSize;
					}
				}

				if (nodeObj.MNPDOINVarCollection.size() != 0)
				{
					/* Create PDO_RxCommParam_XXh_REC 1400 Index*/
					Index* indexObjTemp;
					strcpy(indexIdMN, "14");
					sidxId = IntToAscii(rxChannelCount, sidxId, 16);
					rxChannelCount++;

					sidxId = PadLeft(sidxId, '0', 2);
					indexIdMN = strcat(indexIdMN, sidxId);
					exceptionObj = IfIndexExists(MN_NODEID, MN, indexIdMN, &indexPos);
					if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
					{
						exceptionObj = AddIndex(MN_NODEID, MN, indexIdMN);
						/* set bFlag to true for 1800*/
					}
					indexObjTemp = indexCollObj->GetIndexbyIndexValue(indexIdMN);
					if (indexObjTemp != NULL)
						indexObjTemp->SetFlagIfIncludedCdc(true);

					inPrevSubIndex = 0;
					inPrevSize = 0;
					if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
					{
						//delete[] arrangedNodeIDbyStation;
						LOG_FATAL() << exceptionObj.getErrorCode();
						throw exceptionObj;
					}

					mappingSidxId = IntToAscii((nodeObj.GetNodeId()), mappingSidxId, 10);
					char* tempSidxId = new char[SUBINDEX_LEN];
					strcpy(tempSidxId, "01");
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN, tempSidxId, mappingSidxId, (char*) "NodeID_U8", true);

					GetSubIndexAttributes(nodeObj.GetNodeId(), CN, (char*) "1800", (char*) "02", ACTUALVALUE, versionNumber);
					if ((NULL == versionNumber)
							|| (strcmp(versionNumber, "") == 0))
					{
						strcpy(versionNumber, "0x0");
					}
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN, (char*) "02", versionNumber, (char*) "MappingVersion_U8", true);
					delete[] tempSidxId;

					strcpy(indexIdMN, "16");
					strcat(indexIdMN, sidxId);
					/* Set the MN's PDO Index*/
					exceptionObj = IfIndexExists(MN_NODEID, MN, indexIdMN, &indexPos);
					if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
					{
						exceptionObj = AddIndex(MN_NODEID, MN, indexIdMN);
						if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
						{
							//delete[] arrangedNodeIDbyStation;
							LOG_FATAL() << exceptionObj.getErrorCode();
							throw exceptionObj;
						}
					}

					indexObjTemp = indexCollObj->GetIndexbyIndexValue(indexIdMN);
					UINT32 pdoInLC = 0;
					for (pdoInLC = 0; pdoInLC < nodeObj.MNPDOINVarCollection.size(); pdoInLC++)
					{
						MNPdoVariable mnPDOobj;
						mnPDOobj = nodeObj.MNPDOINVarCollection[pdoInLC];
						indexObjTemp = indexCollObj->GetIndexbyIndexValue(indexIdMN);
						indexObjTemp->SetFlagIfIncludedCdc(true);

						if (inPrevSubIndex >= 254)
						{	
							boost::format formatter(kMsgChannelObjectLimitExceeded);
							formatter 
								% nodeObj.GetNodeId()
								% indexObjTemp->GetIndex()
								% 255;
							exceptionObj.setErrorCode(OCFM_ERR_CHANNEL_OBJECT_LIMIT_EXCEEDED);
							exceptionObj.setErrorString(formatter.str());
							LOG_FATAL() << formatter.str();
							throw exceptionObj;
						}

						if ((inPrevSize + mnPDOobj.dataSize) > (atoi((const char*) abC_DLL_ISOCHR_MAX_PAYL) * 8))
						{							
							boost::format formatter(kMsgIsochronousMaxPayloadExceeded);
							formatter 
								% nodeObj.GetNodeId()
								% "RPDO"
								% (inPrevSize + mnPDOobj.dataSize);
							exceptionObj.setErrorCode(OCFM_ERR_CHANNEL_PAYLOAD_LIMIT_EXCEEDED);
							exceptionObj.setErrorString(formatter.str());
							LOG_FATAL() << formatter.str();
							throw exceptionObj;
						}

						GetMNPDOSubIndex(mnPDOobj, inPrevSubIndex, indexObjTemp, indexIdMN, inPrevSize);
						inPrevSize = inPrevSize + mnPDOobj.dataSize;
					}

					SubIndex *sidxObjtemp = NULL;
					sidxObjtemp = indexObjTemp->GetSubIndexbyIndexValue((char*) "00");
					if (NULL != sidxObjtemp)
					{
						char *temp = new char[INDEX_LEN];
						temp = IntToAscii(inPrevSubIndex, temp, 10);
						LOG_DEBUG() << "NumberOfEntries updated to: " << temp << " in index: " << indexObjTemp->GetIndexValue();
						//itoa(PdoInCount, temp, 16);
						sidxObjtemp->SetActualValue(temp);
						delete[] temp;
					}
				}
				delete[] versionNumber;
			}
		}
		SetPresMNNodeAssigmentBits();

		//delete[] arrangedNodeIDbyStation;
		SetFlagForRequiredMNIndexes(MN_NODEID);
	}

	catch (ocfmRetCode& exObj)
	{
		return exObj;
	}

	if (txChannelCount > maxNoOfChannels)
	{
		boost::format formatter(kMsgPDOTPDOChannelCountExceeded);
		formatter 
			% maxNoOfChannels
			% txChannelCount;
		exceptionObj.setErrorCode(OCFM_ERR_EXCESS_CHANNEL);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
	}
	else
	{
		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	}
	delete[] indexIdMN;
	return exceptionObj;
}

//TODO: No need of pdoType for size calculation. to be removed in header
INT32 ComputeOUTOffset(INT32 dataSize, PDOType pdoType)
{
	INT32 retOffset = 0;

	switch (dataSize)
	{

	/*Unsigned8, Int8*/
	case 8:
		size8OUTOffset.prevOffset = size8OUTOffset.currOffset;
		retOffset = size8OUTOffset.currOffset;
		size8OUTOffset.currOffset = size8OUTOffset.currOffset + 1;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (size16OUTOffset.currOffset >= size8OUTOffset.currOffset)
		{
		}
		else
		{
			size16OUTOffset.prevOffset = size16OUTOffset.currOffset;
			size16OUTOffset.currOffset = size16OUTOffset.currOffset + 2;
		}

		/* if greater no change*/
		if (size32OUTOffset.currOffset >= size8OUTOffset.currOffset)
		{
		}
		else
		{
			size32OUTOffset.prevOffset = size32OUTOffset.currOffset;
			size32OUTOffset.currOffset = size32OUTOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (size64OUTOffset.currOffset >= size8OUTOffset.currOffset)
		{
		}
		else
		{
			size64OUTOffset.prevOffset = size64OUTOffset.currOffset;
			size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
		}
		break;

		/*Unsigned16, Int16*/
	case 16:
		size16OUTOffset.prevOffset = size16OUTOffset.currOffset;
		retOffset = size16OUTOffset.currOffset;
		size16OUTOffset.currOffset = size16OUTOffset.currOffset + 2;

		/* Set other DataType Offsets*/

		/* if greater no change*/
		if (size8OUTOffset.currOffset >= size16OUTOffset.currOffset)
		{
		}
		else
		{
			size8OUTOffset.prevOffset = size8OUTOffset.currOffset;
			size8OUTOffset.currOffset = size16OUTOffset.currOffset;
		}

		/* if greater no change*/
		if (size32OUTOffset.currOffset >= size16OUTOffset.currOffset)
		{
			//
		}
		else
		{
			size32OUTOffset.prevOffset = size32OUTOffset.currOffset;
			size32OUTOffset.currOffset = size32OUTOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (size64OUTOffset.currOffset >= size16OUTOffset.currOffset)
		{
		}
		else
		{
			size64OUTOffset.prevOffset = size64OUTOffset.currOffset;
			size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
		}
		break;
		/*Unsigned32, Int32*/
	case 32:
		size32OUTOffset.prevOffset = size32OUTOffset.currOffset;
		retOffset = size32OUTOffset.currOffset;
		size32OUTOffset.currOffset = size32OUTOffset.currOffset + 4;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (size8OUTOffset.currOffset >= size32OUTOffset.currOffset)
		{
			//
		}
		else
		{
			size8OUTOffset.prevOffset = size8OUTOffset.currOffset;
			size8OUTOffset.currOffset = size32OUTOffset.currOffset;

		}

		/* if greater no change*/
		if (size16OUTOffset.currOffset >= size32OUTOffset.currOffset)
		{
		}
		else
		{
			size16OUTOffset.prevOffset = size16OUTOffset.currOffset;
			size16OUTOffset.currOffset = size32OUTOffset.currOffset;

		}
		/* if greater no change*/
		if (size64OUTOffset.currOffset >= size32OUTOffset.currOffset)
		{
		}
		else
		{
			size64OUTOffset.prevOffset = size64OUTOffset.currOffset;
			size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
		}
		/*Unsigned64, Int64*/
		break;
	case 64:
		size64OUTOffset.prevOffset = size64OUTOffset.currOffset;
		retOffset = size64OUTOffset.currOffset;
		size64OUTOffset.currOffset = size64OUTOffset.currOffset + 8;
		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (size8OUTOffset.currOffset >= size64OUTOffset.currOffset)
		{
			//
		}
		else
		{
			size8OUTOffset.prevOffset = size8OUTOffset.currOffset;
			size8OUTOffset.currOffset = size64OUTOffset.currOffset;
		}

		/* if greater no change*/
		if (size16OUTOffset.currOffset >= size64OUTOffset.currOffset)
		{
		}
		else
		{
			size16OUTOffset.prevOffset = size16OUTOffset.currOffset;
			size16OUTOffset.currOffset = size64OUTOffset.currOffset;
		}
		/* if greater no change*/
		if (size32OUTOffset.currOffset >= size64OUTOffset.currOffset)
		{
			//
		}
		else
		{
			size32OUTOffset.prevOffset = size32OUTOffset.currOffset;
			size32OUTOffset.currOffset = size64OUTOffset.currOffset;
		}
		break;
	default:
		LOG_ERROR() << "Undefined dataSize encountered: " << dataSize;
		break;
	}
	return retOffset;
}

//TODO: No need of pdoType for size calculation. to be removed in header
INT32 ComputeINOffset(INT32 dataSize, PDOType pdoType)
{
	INT32 retOffset = 0;

	switch (dataSize)
	{

	/*Unsigned8, Int8*/
	case 8:
		size8INOffset.prevOffset = size8INOffset.currOffset;
		retOffset = size8INOffset.currOffset;
		size8INOffset.currOffset = size8INOffset.currOffset + 1;
		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (size16INOffset.currOffset >= size8INOffset.currOffset)
		{
		}
		else
		{
			size16INOffset.prevOffset = size16INOffset.currOffset;
			size16INOffset.currOffset = size16INOffset.currOffset + 2;
		}

		/* if greater no change*/
		if (size32INOffset.currOffset >= size8INOffset.currOffset)
		{
		}
		else
		{

			size32INOffset.prevOffset = size32INOffset.currOffset;
			size32INOffset.currOffset = size32INOffset.currOffset + 4;

		}

		/* if greater no change*/
		if (size64INOffset.currOffset >= size8INOffset.currOffset)
		{
		}
		else
		{
			size64INOffset.prevOffset = size64INOffset.currOffset;
			size64INOffset.currOffset = size64INOffset.currOffset + 8;
		}
		break;

		/*Unsigned16, Int16*/
	case 16:
		size16INOffset.prevOffset = size16INOffset.currOffset;
		retOffset = size16INOffset.currOffset;
		size16INOffset.currOffset = size16INOffset.currOffset + 2;

		/* Set other DataType Offsets*/

		/* if greater no change*/
		if (size8INOffset.currOffset >= size16INOffset.currOffset)
		{
		}
		else
		{
			size8INOffset.prevOffset = size8INOffset.currOffset;
			size8INOffset.currOffset = size16INOffset.currOffset;

		}

		/* if greater no change*/
		if (size32INOffset.currOffset >= size16INOffset.currOffset)
		{
		}
		else
		{
			size32INOffset.prevOffset = size32INOffset.currOffset;
			size32INOffset.currOffset = size32INOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (size64INOffset.currOffset >= size16INOffset.currOffset)
		{
		}
		else
		{
			size64INOffset.prevOffset = size64INOffset.currOffset;
			size64INOffset.currOffset = size64INOffset.currOffset + 8;
		}
		break;
		/*Unsigned32, Int32*/
	case 32:
		size32INOffset.prevOffset = size32INOffset.currOffset;
		retOffset = size32INOffset.currOffset;
		size32INOffset.currOffset = size32INOffset.currOffset + 4;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (size8INOffset.currOffset >= size32INOffset.currOffset)
		{
		}
		else
		{
			size8INOffset.prevOffset = size8INOffset.currOffset;
			size8INOffset.currOffset = size32INOffset.currOffset;

		}

		/* if greater no change*/
		if (size16INOffset.currOffset >= size32INOffset.currOffset)
		{
		}
		else
		{
			size16INOffset.prevOffset = size16INOffset.currOffset;
			size16INOffset.currOffset = size32INOffset.currOffset;

		}
		/* if greater no change*/
		if (size64INOffset.currOffset >= size32INOffset.currOffset)
		{
		}
		else
		{
			size64INOffset.prevOffset = size64INOffset.currOffset;
			size64INOffset.currOffset = size64INOffset.currOffset + 8;
		}
		break;
		/*Unsigned64, Int64*/
	case 64:
		size64INOffset.prevOffset = size64INOffset.currOffset;
		retOffset = size64INOffset.currOffset;
		size64INOffset.currOffset = size64INOffset.currOffset + 8;
		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (size8INOffset.currOffset >= size64INOffset.currOffset)
		{
		}
		else
		{
			size8INOffset.prevOffset = size8INOffset.currOffset;
			size8INOffset.currOffset = size64INOffset.currOffset;
		}

		/* if greater no change*/
		if (size16INOffset.currOffset >= size64INOffset.currOffset)
		{
		}
		else
		{
			size16INOffset.prevOffset = size16INOffset.currOffset;
			size16INOffset.currOffset = size64INOffset.currOffset;
		}
		/* if greater no change*/
		if (size32INOffset.currOffset >= size64INOffset.currOffset)
		{
		}
		else
		{
			size32INOffset.prevOffset = size32INOffset.currOffset;
			size32INOffset.currOffset = size64INOffset.currOffset;
		}
		break;
	default:
		LOG_ERROR() << "Undefined dataSize encountered: " << dataSize;
		break;
	}
	return retOffset;
}

ocfmRetCode FreeProjectMemory()
{
	NodeCollection *nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	for(int i = 0; i<nodeCollObj->GetNumberOfNodes(); i++)
	{
		Node *nodeObj = NULL;
		nodeObj = nodeCollObj->GetNodebyColIndex(i);
		nodeObj->DeleteCollectionsForPI();
		nodeObj->DeleteCollectionsForNETPI();
		nodeObj->MNPDOINVarCollection.clear();
		nodeObj->MNPDOOUTVarCollection.clear();
		nodeObj->NETPIColl.clear();
		nodeObj->PICollection.clear();

		ApplicationProcess *applObj = NULL;
		applObj = nodeObj->GetApplicationProcess();
		applObj->DeleteComplexDataTypeCollection();
		applObj->DeleteParameterCollection();

		DataTypeCollection *dtCollObj = NULL;
		dtCollObj = nodeObj->GetDataTypeCollection();
		dtCollObj->DeleteDataTypeCollection();

		IndexCollection *idxCollObj = NULL;
		idxCollObj = nodeObj->GetIndexCollection();
		for(int j = 0; j<idxCollObj->GetNumberofIndexes(); j++)
		{
			Index *idxObj = NULL;
			idxObj = idxCollObj->GetIndexByPosition(j);
			for(int k = 0; k<idxObj->GetNumberofSubIndexes(); k++)
			{
				SubIndex *sidxObj = NULL;
				sidxObj = idxObj->GetSubIndexByPosition(k);
				sidxObj->DeleteAllMemberMemory();
			}
			idxObj->DeleteAllMemberMemory();
			idxObj->DeleteSubIndexCollection();
			//delete idxObj;
		}
		idxCollObj->DeleteIndexCollection();
		//delete idxCollObj;
		nodeCollObj->DeleteNode(i);
	}
	delete nodeCollObj;
	ocfmRetCode exceptionObj;
	exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	return exceptionObj;
}

//ocfmRetCode OpenProject(const char* projectPath, const char* projectFileName)
//{
//	NodeCollection *nodeCollObj;
//	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//	xmlTextReaderPtr xmlReader;
//	char *tempFileName = NULL;
//
//#if defined DEBUG
//	cout << "\nStrLen for FileName:"
//	<< (strlen(projectPath) + strlen(projectFileName) + 1) << endl;
//#endif
//
//	tempFileName =
//			new char[(strlen(projectPath) + strlen(projectFileName) + 5)];
//#if defined(_WIN32) && defined(_MSC_VER)
//	{
//		sprintf(tempFileName, "%s\\%s", projectPath, projectFileName);
//	}
//#else
//	{
//		sprintf(tempFileName, "%s/%s", projectPath, projectFileName);
//	}
//#endif
//
//	xmlReader = xmlReaderForFile(tempFileName, NULL, 0);
//	delete[] tempFileName;
//
//	ocfmRetCode exceptionObj;
//
//	try
//	{
//		if (xmlReader != NULL)
//		{
//			INT32 retVal;
//			retVal = xmlTextReaderRead(xmlReader);
//			while (retVal == 1)
//			{
//				ProcessProjectXML(xmlReader, projectPath);
//				retVal = xmlTextReaderRead(xmlReader);
//			}
//			if (retVal != 0)
//			{
//				LOG_FATAL() << "OCFM_ERR_PARSE_XML";
//				exceptionObj.setErrorCode(OCFM_ERR_PARSE_XML);
//				throw exceptionObj;
//			}
//		}
//		else
//		{
//			LOG_FATAL() << "OCFM_ERR_CANNOT_OPEN_FILE";
//			exceptionObj.setErrorCode(OCFM_ERR_CANNOT_OPEN_FILE);
//			throw exceptionObj;
//		}
//
//		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//
//		char* presTimeoutVal = new char[50];
//		presTimeoutVal[0] = 0;
//
//		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
//				nodeLC++)
//		{
//			Node nodeObj;
//			INT32 nodeId;
//			NodeType nodeType;
//			nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);
//
//			nodeType = nodeObj.GetNodeType();
//			nodeId = nodeObj.GetNodeId();
//			CopyPDODefToAct(nodeId, nodeType);
//			CopyMNPropDefToAct(nodeId, nodeType);
//
//			if ((nodeId != MN_NODEID) && (MN != nodeType))
//			{
//				char* value = NULL;
//				value = new char[SUBINDEX_LEN];
//				value = IntToAscii(nodeId, value, 16);
//				value = PadLeft(value, '0', 2);
//				GetSubIndexAttributes(MN_NODEID, MN, (char*) "1f92", value,
//						ACTUALVALUE, presTimeoutVal);
//				if (((NULL == presTimeoutVal)
//						|| (strcmp(presTimeoutVal, "") == 0))
//						|| (!(ValidateCNPresTimeout(value, presTimeoutVal))))
//				{
//					CalculateCNPollResponse(nodeId, nodeType);
//				}
//
//				delete[] value;
//			}
//		}
//		delete[] presTimeoutVal;
//	}
//	catch (ocfmRetCode& ex)
//	{
//		return ex;
//	}
//
//	PjtSettings* pjtSettingsObj;
//	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
//	//A compatibility workaround for the pdo mapping and accesstype errors.
//	if(1 == strcmp("1.3.0", pjtSettingsObj->GetPjtVersion()))
//	{
//		exceptionObj.setErrorCode(COMPATIBILITY_INFO_PRE_130_PDOMAPPING);
//		return exceptionObj;
//	}
//
//	exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
//	return exceptionObj;
//}

//ocfmRetCode ProcessProjectXML(xmlTextReaderPtr xmlReader, const char* projectPath)
//{
//	const xmlChar *xmlName;
//	ocfmRetCode exceptionObj;

//	xmlName = xmlTextReaderConstName(xmlReader);
//	if (xmlName == NULL)
//	{
//		LOG_WARN() << "Local variable 'xmlName' must not be NULL.";
//	}
//	//value = xmlTextReaderConstValue(pxReader);
//	try
//	{
//		if (xmlTextReaderNodeType(xmlReader) == 1)
//		{
//			// Check for openCONFIGURATOR Tag
//			if (strcmp(((char*) xmlName), "openCONFIGURATOR") == 0)
//			{
//				if (xmlTextReaderHasAttributes(xmlReader) == 1)
//				{
//					while (xmlTextReaderMoveToNextAttribute(xmlReader))
//					{
//						// Call Check Version number Fn
//						if (IfVersionNumberMatches(xmlReader) == false)
//						{
//							LOG_FATAL() << "Cannot open project. Version no. mismatch.";
//							exceptionObj.setErrorCode(OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH);
//							throw exceptionObj;
//						}
//					}
//				}
//			}
//			else if (strcmp(((char*) xmlName), "profile") == 0)
//			{
//				if (xmlTextReaderHasAttributes(xmlReader) == 1)
//				{
//					LOG_FATAL() << "Cannot open project. Invalid project file.";
//					exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//					throw exceptionObj;
//				}
//			}
//			else if (strcmp(((char*) xmlName), "Auto") == 0)
//			{
//				if (xmlTextReaderHasAttributes(xmlReader) == 1)
//				{
//					if (SetProjectSettingsAuto(xmlReader) == false)
//					{
//						LOG_FATAL() << "Cannot open project. Invalid project file.";
//						exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//						throw exceptionObj;
//					}
//				}
//				else
//				{
//					LOG_FATAL() << "Cannot open project. Invalid project file.";
//					exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//					throw exceptionObj;
//				}
//			}
//			else if (strcmp(((char*) xmlName), "Communication") == 0)
//			{
//				if (xmlTextReaderHasAttributes(xmlReader) == 1)
//				{
//					if (SetProjectSettingsCommunication(xmlReader) == false)
//					{
//						LOG_FATAL() << "Cannot open project. Invalid project file.";
//						exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//						throw exceptionObj;
//					}
//				}
//				else
//				{
//					LOG_FATAL() << "Cannot open project. Invalid project file.";
//					exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//					throw exceptionObj;
//				}
//			}
//			else if (strcmp(((char*) xmlName), "NodeCollection") == 0)
//			{
//				if (xmlTextReaderHasAttributes(xmlReader) == 1)
//				{
//					LOG_FATAL() << "Cannot open project. Invalid project file.";
//					exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//					throw exceptionObj;
//				}
//			}
//			else if (strcmp(((char*) xmlName), "Node") == 0)
//			{
//				if (xmlTextReaderHasAttributes(xmlReader) == 1)
//				{
//					if (GetandCreateNode(xmlReader, projectPath) == false)
//					{
//						LOG_FATAL() << "Cannot open project. Invalid project file.";
//						exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//						throw exceptionObj;
//					}
//					else
//					{
//						//ocfmRetCode stErrorInfo;
//						//stErrorInfo.code = OCFM_ERR_SUCCESS;
//						//return stErrorInfo;
//					}
//				}
//				else
//				{
//					LOG_FATAL() << "Cannot open project. Invalid project file.";
//					exceptionObj.setErrorCode(OCFM_ERR_INVALID_PJTXML);
//					throw exceptionObj;
//				}
//			}
//		}
//
//	} catch (ocfmRetCode& exObj)
//	{
//		return exObj;
//	}
//
//	exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
//	return exceptionObj;
//}

//bool SetProjectSettingsAuto(xmlTextReaderPtr xmlReader)
//{
//	PjtSettings* pjtSettingsObj;
//	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
//
//	while (xmlTextReaderMoveToNextAttribute(xmlReader))
//	{
//		const xmlChar* xmlName;
//		const xmlChar* xmlValue;
//		//Retrieve the pxcName and Value of an attribute	
//		xmlValue = xmlTextReaderConstValue(xmlReader);
//		xmlName = xmlTextReaderConstName(xmlReader);
//
//		if (xmlValue == NULL || xmlName == NULL)
//			return false;
//		if (strcmp(((char*) xmlName), "Generate") == 0)
//		{
//			if (strcmp(((char*) xmlValue), "YES") == 0)
//				pjtSettingsObj->SetGenerateAttr(YES_AG);
//			else if (strcmp(((char*) xmlValue), "NO") == 0)
//				pjtSettingsObj->SetGenerateAttr(NO_AG);
//			else
//			{
//				return false;
//			}
//		}
//		else if (strcmp(((char*) xmlName), "Save") == 0)
//		{
//			if (strcmp(((char*) xmlValue), "YES") == 0)
//				pjtSettingsObj->SetSaveAttr(YES_AS);
//			else if (strcmp(((char*) xmlValue), "PROMPT") == 0)
//				pjtSettingsObj->SetSaveAttr(PROMPT_AS);
//			else if (strcmp(((char*) xmlValue), "DISCARD") == 0)
//				pjtSettingsObj->SetSaveAttr(DISCARD_AS);
//			else
//			{
//				return false;
//			}
//		}
//		else if (strcmp(((char*) xmlName), "View") == 0)
//		{
//			if (strcmp(((char*) xmlValue), "General") == 0)
//				pjtSettingsObj->SetViewMode(SIMPLE);
//			else if (strcmp(((char*) xmlValue), "Advanced") == 0)
//				pjtSettingsObj->SetViewMode(EXPERT);
//			else
//			{
//				return false;
//			}
//		}
//		else if (strcmp(((char*) xmlName), "ExpertViewSelected") == 0)
//		{
//			if (strcmp(((char*) xmlValue), "true") == 0)
//				pjtSettingsObj->SetExpertViewSelectedFlag(true);
//			else if (strcmp(((char*) xmlValue), "false") == 0)
//				pjtSettingsObj->SetExpertViewSelectedFlag(false);
//			else
//			{
//				return false;
//			}
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return true;
//}

//bool SetProjectSettingsCommunication(xmlTextReaderPtr xmlReader)
//{
//
//	while (xmlTextReaderMoveToNextAttribute(xmlReader))
//	{
//		const xmlChar* xmlName;
//		const xmlChar* xmlValue;
//		//Retrieve the pxcName and Value of an attribute	
//		xmlValue = xmlTextReaderConstValue(xmlReader);
//		xmlName = xmlTextReaderConstName(xmlReader);
//
//		if (xmlValue == NULL || xmlName == NULL)
//			return false;
//		if (strcmp(((char*) xmlName), "IP") == 0)
//		{
//			if ((char*) xmlValue != NULL)
//			{
//				PjtSettings* pjtSettingsObj;
//				pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
//				pjtSettingsObj->SetIP((char*) xmlValue);
//			}
//			else
//			{
//				return false;
//			}
//
//		}
//		else
//		{
//			return false;
//		}
//	}
//	return true;
//}

//bool GetandCreateNode(xmlTextReaderPtr xmlReader, const char* projectPath)
//{
//
//	char* nodeName = NULL;
//	char* octFilePath = NULL;
//	char* fileName = NULL;
//	char* forceCycleValue = NULL;
//
//	INT32 nodeId; //can't be initialised. and if it is not present false is returned.
//	NodeType nodeType; //can't be initialised. and if it is not present false is returned.
//	bool forceCycleFlag = false;
//	StationType stationType;
//	ocfmRetCode exceptionObj;
//	exceptionObj.setErrorCode(OCFM_ERR_UNKNOWN);
//
//	stationType = NORMAL;
//
//	while (xmlTextReaderMoveToNextAttribute(xmlReader))
//	{
//		const xmlChar *xmlName = NULL;
//		const xmlChar *xmlValue = NULL;
//		//Retrieve the pxcName and Value of an attribute	
//		xmlValue = xmlTextReaderConstValue(xmlReader);
//		xmlName = xmlTextReaderConstName(xmlReader);
//
//		if (xmlValue == NULL || xmlName == NULL)
//		{
//			return false;
//		}
//
//		if (strcmp(((char*) xmlName), "name") == 0)
//		{
//			if ((char*) xmlValue != NULL)
//			{
//				nodeName = new char[strlen((char*) xmlValue) + ALLOC_BUFFER];
//				strcpy((char*) nodeName, (char*) xmlValue);
//			}
//			else
//			{
//				return false;
//			}
//
//		}
//		else if (strcmp(((char*) xmlName), "NodeId") == 0)
//		{
//			if ((char*) xmlValue != NULL)
//			{
//				nodeId = atoi((char*) xmlValue);
//			}
//			else
//			{
//				return false;
//			}
//
//		}
//		else if (strcmp(((char*) xmlName), "NodeType") == 0)
//		{
//			if (strcmp(((char*) xmlValue), "MN") == 0)
//			{
//				nodeType = MN;
//			}
//			else if (strcmp(((char*) xmlValue), "CN") == 0)
//			{
//				nodeType = CN;
//			}
//			else
//			{
//				return false;
//			}
//
//		}
//		else if (strcmp(((char*) xmlName), "xdc") == 0)
//		{
//			if ((char*) xmlValue != NULL)
//			{
//				octFilePath = new char[strlen((char*) xmlValue) + 1];
//				strcpy((char*) octFilePath, (char*) xmlValue);
//			}
//			else
//			{
//				octFilePath = NULL;
//			}
//		}
//		else if (strcmp(((char*) xmlName), "ForceCycleFlag") == 0)
//		{
//			if (strcmp(((char*) xmlValue), "true") == 0)
//			{
//				forceCycleFlag = true;
//			}
//			else if (strcmp(((char*) xmlValue), "false") == 0)
//			{
//				forceCycleFlag = false;
//			}
//			else
//			{
//				forceCycleFlag = false;
//			}
//		}
//		else if (strcmp(((char*) xmlName), "ForceCycle") == 0)
//		{
//			if ((char*) xmlValue != NULL && strcmp((char*) xmlValue, "") != 0)
//			{
//				forceCycleValue = new char[strlen((char*) xmlValue)
//						+ ALLOC_BUFFER];
//				strcpy((char*) forceCycleValue, (char*) xmlValue);
//			}
//
//		}
//		else if (strcmp(((char*) xmlName), "StationType") == 0)
//		{
//			if (strcmp(((char*) xmlValue), "Multiplexed") == 0)
//			{
//				stationType = MULTIPLEXED;
//			}
//			else if (strcmp(((char*) xmlValue), "Chained") == 0)
//			{
//				stationType = CHAINED;
//			}
//			else if (strcmp(((char*) xmlValue), "Normal") == 0)
//			{
//				stationType = NORMAL;
//			}
//		}
//	}
//	//varNodeName 
//	if (nodeType == 1)
//	{
//		exceptionObj = CreateNode(nodeId, CN, nodeName);
//	}
//	else if (nodeType == 0)
//	{
//		exceptionObj = CreateNode(nodeId, MN, nodeName);
//	}
//
//	if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
//	{
//		return false;
//	}
//
//	Node* nodeObj;
//	NodeCollection *nodeCollObj;
//	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//	nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
//	nodeObj->SetForceCycleFlag(forceCycleFlag);
//	fileName = new char[(strlen(projectPath) + strlen(octFilePath) + 5)];
//	sprintf(fileName, "%s/%s", projectPath, octFilePath);
//	delete[] octFilePath;
//	if (nodeType == 1)
//	{
//		exceptionObj = ParseFile(fileName, nodeId, CN);
//	}
//	else if (nodeType == 0)
//	{
//		exceptionObj = ParseFile(fileName, nodeId, MN);
//	}
//
//	if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
//	{
//		return false;
//	}
//
//	if (nodeType == 1 && forceCycleValue != NULL)
//	{
//		nodeObj->SetForcedCycle(forceCycleValue);
//	}
//
//	nodeObj->SetStationType(stationType);
//
//	//    copyDefToAct(iNodeID, varNodeType);
//	delete[] fileName;
//	delete[] nodeName;
//	return true;
//}

//bool SaveProjectXML(const char* projectPath, const char* projectName)
//{
//	PjtSettings* pjtSettingsObj;
//	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
//	ocfmRetCode exceptionObj;
//
//	xmlTextWriterPtr xmlWriter;
//	xmlDocPtr xmlDocObj;
//	INT32 bytesWritten;
//	char* fileName = NULL;
//
//	/* Create a new XmlWriter for DOM, with no compression. */
//	xmlWriter = xmlNewTextWriterDoc(&xmlDocObj, 0);
//	if (xmlWriter == NULL)
//	{
//		LOG_FATAL() << "Error creating XML-Writer.";
//		exceptionObj.setErrorCode(OCFM_ERR_CREATE_XML_WRITER_FAILED);
//		throw exceptionObj;
//	}
//	/* Start the document with the xml default for the version,
//	 * encoding UTF-8 and the default for the standalone
//	 * declaration. */
//	bytesWritten = xmlTextWriterStartDocument(xmlWriter, NULL, ENCODING,
//			NULL);
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error starting XML-Document.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_START_DOC_FAILED);
//		throw exceptionObj;
//	}
//	bytesWritten = xmlTextWriterWriteComment(xmlWriter,
//			BAD_CAST "This file was autogenerated by openCONFIGURATOR");
//	xmlTextWriterSetIndent(xmlWriter, 1);
//	// Start openCONFIGURATOR Tag		
//	bytesWritten = xmlTextWriterStartElement(xmlWriter,
//			BAD_CAST "openCONFIGURATOR");
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error starting XML-Element 'openCONFIGURATOR'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
//		throw exceptionObj;
//	}
//	bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Version",
//			BAD_CAST TOOL_VERSION);
//
//	xmlTextWriterSetIndent(xmlWriter, 1);
//	// Start profile Tag
//	bytesWritten = xmlTextWriterStartElement(xmlWriter, BAD_CAST "profile");
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error starting XML-Element 'profile'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
//		throw exceptionObj;
//	}
//
//	xmlTextWriterSetIndent(xmlWriter, 1);
//	// Start Auto Tag		
//	bytesWritten = xmlTextWriterStartElement(xmlWriter, BAD_CAST "Auto");
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error starting XML-Element 'Auto'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
//		throw exceptionObj;
//	}
//
////	if (pjtSettingsObj->GetGenerateAttr() == NO_AG)
//	if(ProjectConfiguration::GetInstance().GetGenerateMNOBD() == false)
//	{
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//				BAD_CAST "Generate", BAD_CAST "NO");
//	}
////	else if (pjtSettingsObj->GetGenerateAttr() == YES_AG)
//	else if(ProjectConfiguration::GetInstance().GetGenerateMNOBD() == true)
//	{
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//				BAD_CAST "Generate", BAD_CAST "YES");
//	}
//
//	if (pjtSettingsObj->GetSaveAttr() == YES_AS)
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Save",
//				BAD_CAST "YES");
//	else if (pjtSettingsObj->GetSaveAttr() == PROMPT_AS)
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Save",
//				BAD_CAST "PROMPT");
//	else if (pjtSettingsObj->GetSaveAttr() == DISCARD_AS)
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Save",
//				BAD_CAST "DISCARD");
//
//	if (pjtSettingsObj->GetViewMode() == SIMPLE)
//	{
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "View",
//				BAD_CAST "General");
//	}
//	else if (pjtSettingsObj->GetViewMode() == EXPERT)
//	{
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "View",
//				BAD_CAST "Advanced");
//	}
//
//	if (pjtSettingsObj->GetExpertViewSelectedFlag() == true)
//	{
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//				BAD_CAST "ExpertViewSelected", BAD_CAST "true");
//	}
//	else if (pjtSettingsObj->GetExpertViewSelectedFlag() == false)
//	{
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//				BAD_CAST "ExpertViewSelected", BAD_CAST "false");
//	}
//
//	// End Auto Tag
//	bytesWritten = xmlTextWriterEndElement(xmlWriter);
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error closing XML-Element 'Auto'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
//		throw exceptionObj;
//	}
//
//	xmlTextWriterSetIndent(xmlWriter, 1);
//	// Start Communication Tag		
//	bytesWritten = xmlTextWriterStartElement(xmlWriter,
//			BAD_CAST "Communication");
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error starting XML-Element 'Communication'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
//		throw exceptionObj;
//	}
//	bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "IP",
//			BAD_CAST "0.0.0.0");
//
//	// End Communication Tag
//	bytesWritten = xmlTextWriterEndElement(xmlWriter);
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error closing XML-Element 'Communication'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
//		throw exceptionObj;
//	}
//
//	// End profile Tag
//	bytesWritten = xmlTextWriterEndElement(xmlWriter);
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error closing XML-Element 'profile'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
//		throw exceptionObj;
//	}
//	xmlTextWriterSetIndent(xmlWriter, 1);
//	// Start NodeCollection Tag
//	bytesWritten = xmlTextWriterStartElement(xmlWriter,
//			BAD_CAST "NodeCollection");
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error starting XML-Element 'NodeCollection'.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
//		throw exceptionObj;
//	}
//	//Node nodeObj;
//	NodeCollection *nodeCollObj = NULL;
//	//Index indexObj;
//	//indexObj.SetNodeID(nodeObj.GetNodeId());
//	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//
//	for (INT32 nodeCount = 0; nodeCount < nodeCollObj->GetNumberOfNodes();
//			nodeCount++)
//	{
//		Node* nodeObj;
//
//		nodeObj = nodeCollObj->GetNodebyColIndex(nodeCount);
//
//		xmlTextWriterSetIndent(xmlWriter, 1);
//
//		// Start Node Tag		
//		bytesWritten = xmlTextWriterStartElement(xmlWriter, BAD_CAST "Node");
//
//		if (bytesWritten < 0)
//		{
//			LOG_FATAL() << "Error starting XML-Element 'Node'.";
//			exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
//			throw exceptionObj;
//		}
//
//		char* tempNodeName;
//		tempNodeName = new char[50];
//		strcpy(tempNodeName, (char*) nodeObj->GetNodeName());
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "name",
//				BAD_CAST tempNodeName);
//
//		char* tempNodeID;
//		tempNodeID = new char[20];
//
//		IntToAscii(nodeObj->GetNodeId(), tempNodeID, 0);
//
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "NodeId",
//				BAD_CAST tempNodeID);
//
//		NodeType tempNodeType;
//		tempNodeType = nodeObj->GetNodeType();
//
//		if (tempNodeType == 0)
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "NodeType", BAD_CAST "MN");
//		if (tempNodeType == 1)
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "NodeType", BAD_CAST "CN");
//
//		char* tempOctName = new char[50];
//
//		sprintf(tempOctName, "%s/%s.octx", "octx", tempNodeID);
//		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "xdc",
//				BAD_CAST tempOctName);
//		if (nodeObj->GetForceCycleFlag() == true)
//		{
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "ForceCycleFlag", BAD_CAST "true");
//		}
//		else if (nodeObj->GetForceCycleFlag() == false)
//		{
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "ForceCycleFlag", BAD_CAST "false");
//		}
//
//		if (nodeObj->GetForcedCycleValue() != NULL
//				&& strcmp(nodeObj->GetForcedCycleValue(), ""))
//		{
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "ForceCycle",
//					BAD_CAST nodeObj->GetForcedCycleValue());
//		}
//
//		if (nodeObj->GetStationType() == NORMAL)
//		{
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "StationType", BAD_CAST "Normal");
//		}
//		else if (nodeObj->GetStationType() == MULTIPLEXED)
//		{
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "StationType", BAD_CAST "Multiplexed");
//		}
//		else if (nodeObj->GetStationType() == CHAINED)
//		{
//			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
//					BAD_CAST "StationType", BAD_CAST "Chained");
//		}
//
//		// End Node Tag
//		bytesWritten = xmlTextWriterEndElement(xmlWriter);
//		if (bytesWritten < 0)
//		{
//			LOG_FATAL() << "Error closing XML-Element.";
//			exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
//			delete[] tempNodeName;
//			delete[] tempNodeID;
//			delete[] tempOctName;
//			throw exceptionObj;
//		}
//
//		delete[] tempNodeName;
//		delete[] tempNodeID;
//		delete[] tempOctName;
//	}
//
//	// End NodeCollection Tag
//	bytesWritten = xmlTextWriterEndElement(xmlWriter);
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error closing XML-Element.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
//		throw exceptionObj;
//	}
//
//	// End openCONFIGURATOR Tag
//	bytesWritten = xmlTextWriterEndElement(xmlWriter);
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error closing XML-Element.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
//		throw exceptionObj;
//	}
//
//	bytesWritten = xmlTextWriterEndDocument(xmlWriter);
//	if (bytesWritten < 0)
//	{
//		LOG_FATAL() << "Error closing XML-Element.";
//		exceptionObj.setErrorCode(OCFM_ERR_XML_END_DOC_FAILED);
//		throw exceptionObj;
//	}
//
//	xmlFreeTextWriter(xmlWriter);
//	fileName = new char[MAX_FILE_PATH_SIZE];
//
//#if defined(_WIN32) && defined(_MSC_VER)
//	{
//
//		sprintf(fileName, "%s\\%s\\%s.oct", projectPath, projectName,
//				projectName);
//	}
//#else
//	{
//		sprintf(fileName, "%s/%s/%s.oct", projectPath, projectName, projectName);
//	}
//#endif
//	xmlSaveFileEnc(fileName, xmlDocObj, ENCODING);
//
//	xmlFreeDoc(xmlDocObj);
//
//	delete[] fileName;
//	return true;
//}

void CreateMNPDOVar(INT32 offsetVal, INT32 dataSize, IEC_Datatype iecDataType,
		PDOType pdoType, Node *nodeObj)
{
	ocfmRetCode exceptionObj;
	if (!nodeObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	MNPdoVariable mnPDOobj;
	PIObject piObjectObj;
	piObjectObj.indexId = NULL;
	piObjectObj.sIdxId = NULL;

	/* Assign the PDO type*/
	mnPDOobj.pdoType = pdoType;
	mnPDOobj.dataSize = dataSize;
	/* Assign Index*/
	mnPDOobj.indexId = new char[4 + ALLOC_BUFFER];
	mnPDOobj.subIndexId = new char[SUBINDEX_LEN + ALLOC_BUFFER];
	switch (iecDataType)
	{
		case USINT:
		case BITSTRING:
			PDODataType dt;
			switch (dataSize)
			{
			case 8:
				dt = UNSIGNED8;
				break;
			case 16:
				dt = UNSIGNED16;
				break;
			case 32:
				dt = UNSIGNED32;
				break;
			case 64:
				dt = UNSIGNED64;
				break;
			default:
				break;
			}
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(dt, INPUT, offsetVal, dataSize);

			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(dt, OUTPUT, offsetVal, dataSize);
			}
			break;
		case SINT:
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(INTEGER8, INPUT, offsetVal, dataSize);
			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(INTEGER8, OUTPUT, offsetVal, dataSize);
			}
			break;
		case UINT:
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(UNSIGNED16, INPUT, offsetVal, dataSize);
			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(UNSIGNED16, OUTPUT, offsetVal, dataSize);
			}
			break;
		case INT:
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(INTEGER16, INPUT, offsetVal, dataSize);
			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(INTEGER16, OUTPUT, offsetVal, dataSize);
			}

			break;
		case UDINT:
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(UNSIGNED32, INPUT, offsetVal, dataSize);
			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(UNSIGNED32, OUTPUT, offsetVal, dataSize);
			}
			break;
		case DINT:
		case REAL:
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(INTEGER32, INPUT, offsetVal, dataSize);
			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(INTEGER32, OUTPUT, offsetVal, dataSize);
			}
			break;
		case LINT:
		case LREAL:
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(INTEGER64, INPUT, offsetVal, dataSize);
			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(INTEGER64, OUTPUT, offsetVal, dataSize);
			}
			break;
		case ULINT:
			if (pdoType == PDO_TPDO)
			{
				piObjectObj = GetPIAddress(UNSIGNED64, INPUT, offsetVal, dataSize);
			}
			else if (pdoType == PDO_RPDO)
			{
				piObjectObj = GetPIAddress(UNSIGNED64, OUTPUT, offsetVal, dataSize);
			}
			break;
		case BOOL:
		case BYTE:
		case _CHAR:
		case DWORD:
		case LWORD:
		case STRING:
		case WSTRING:
		{
			LOG_ERROR() << "Datatype " << iecDataType << " not supported.";
			break;
		}
		default:
			LOG_ERROR() << "Unknown data type: " << iecDataType;
			break;
	}
	//TODO: piObjectObj.indexId &/|| piObjectObj.sIdxId may be null
	strcpy(mnPDOobj.indexId, piObjectObj.indexId);
	strcpy(mnPDOobj.subIndexId, piObjectObj.sIdxId);
	/* Assign the value*/
	mnPDOobj.value = new char[10 + ALLOC_BUFFER];
	char* padStr = new char[5 + ALLOC_BUFFER];
	padStr = IntToAscii(dataSize, padStr, 16);
	padStr = PadLeft(padStr, '0', 4);
	padStr = ConvertToUpper(padStr);
	strcpy(mnPDOobj.value, "0x");
	strcat(mnPDOobj.value, padStr);
	/* Set the Offset*/
	strcat(mnPDOobj.value, "0000");
	/* Set the Reserved*/
	strcat(mnPDOobj.value, "00");

	nodeObj->AddMNPDOvar(mnPDOobj, pdoType);

}

//ocfmRetCode GetProjectSettings(AutoGenerate *autoGenStatus,
//		AutoSave *autoSaveStatus, ViewMode *viewMode,
//		bool* isExpertViewAlreadySet)
//{
//
//	ocfmRetCode exceptionObj;
//	if ((NULL == autoGenStatus) || (NULL == autoSaveStatus)
//			|| (NULL == viewMode) || (NULL == isExpertViewAlreadySet))
//	{
//		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
//		LOG_FATAL() << "Parameter 'autoGenStatus', 'autoSaveStatus', 'viewMode', 'isExpertViewAlreadySet' must not be NULL.";
//		throw exceptionObj;
//	}
//	PjtSettings* pjtSettingsObj = NULL;
//	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
//
//	try
//	{
//		if (pjtSettingsObj == NULL)
//		{
//			LOG_FATAL() << "Local variable 'pjtSettingsObj' must not be NULL.";
//			exceptionObj.setErrorCode(OCFM_ERR_PROJECT_SETTINGS);
//			throw exceptionObj;
//		}
//
//		*autoGenStatus = pjtSettingsObj->GetGenerateAttr();
//		*autoSaveStatus = pjtSettingsObj->GetSaveAttr();
//		*viewMode = pjtSettingsObj->GetViewMode();
//		*isExpertViewAlreadySet = pjtSettingsObj->GetExpertViewSelectedFlag();
//
//		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
//
//	} catch (ocfmRetCode& ex)
//	{
//		return ex;
//	}
//	return exceptionObj;
//}

//ocfmRetCode SetProjectSettings(AutoGenerate autoGenStatus,
//		AutoSave autoSaveStatus, ViewMode viewMode, bool isExpertViewAlreadySet)
//{
//	ocfmRetCode exceptionObj;
//	exceptionObj.setErrorCode(OCFM_ERR_UNKNOWN);
//	PjtSettings* pjtSettingsObj;
//	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
//
//	try
//	{
//		if (pjtSettingsObj == NULL)
//		{
//			LOG_FATAL() << "Local variable 'pjtSettingsObj' must not be NULL.";
//			exceptionObj.setErrorCode(OCFM_ERR_PROJECT_SETTINGS);
//			throw exceptionObj;
//		}
//
//		pjtSettingsObj->SetGenerateAttr(autoGenStatus);
//		pjtSettingsObj->SetSaveAttr(autoSaveStatus);
//		pjtSettingsObj->SetViewMode(viewMode);
//		pjtSettingsObj->SetExpertViewSelectedFlag(isExpertViewAlreadySet);
//		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
//	} catch (ocfmRetCode& exObj)
//	{
//		return exObj;
//	}
//	return exceptionObj;
//}

//TODO: nodeType is not used. To be removed.
void UpdateNumberOfEnteriesSIdx(Index *indexObj, NodeType nodeType)
{
	ocfmRetCode exceptionObj;
	if (!indexObj)
	{		
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	SubIndex *pobjSIdx = NULL;
	INT32 iTotalSIdxs = 0;

	pobjSIdx = indexObj->GetSubIndexbyIndexValue((char*) "00");
	/* subindexes excluding "00"*/
	iTotalSIdxs = indexObj->GetNumberofSubIndexes() - 1;

	if (pobjSIdx != NULL)
	{
		char tempStr[10];
		char* buffer = new char[10];
		strcpy(buffer, "0x");
		strcat(buffer, IntToAscii(iTotalSIdxs, tempStr, 16));
		if(CheckIfManufactureSpecificObject((char*) indexObj->GetIndexValue()))
		{
			pobjSIdx->SetDefaultValue(buffer);
		}
		pobjSIdx->SetActualValue(buffer);
		delete[] buffer;
	}
	else
	{
		LOG_ERROR() << "Subindex 0x0 does not exist.";
	}
}

void AutogenerateOtherIndexs(Node* nodeObj)
{
	ocfmRetCode exceptionObj;
	if (!nodeObj)
	{		
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	char* indexId = new char[INDEX_LEN + ALLOC_BUFFER];
	char* sidxId = new char[SUBINDEX_LEN + ALLOC_BUFFER];
	Index* indexObj;
	IndexCollection* indexCollObj;

	INT32 indexPos;

	indexCollObj = nodeObj->GetIndexCollection();
	/* 1006*/
	strcpy(indexId, "1006");
	exceptionObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);

	/*  1300*/
	strcpy(indexId, "1300");
	exceptionObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		SetBasicIndexAttributes(MN_NODEID, MN, indexId, (char*) "5000",
				(char*) "SDO_SequLayerTimeout_U32", true);
	}

	/* 1C02*/
	strcpy(indexId, "1C02");
	exceptionObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(true);
		strcpy(sidxId, "00");
		SetSIdxValue(indexId, sidxId, (char*) "3", indexCollObj,
				nodeObj->GetNodeId(), MN, false);
		/* Set subindex value 40 or 0000028 */
		strcpy(sidxId, "01");
		SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
				nodeObj->GetNodeId(), MN, false);
		strcpy(sidxId, "02");
		SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
				nodeObj->GetNodeId(), MN, false);
		strcpy(sidxId, "03");
		SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
				nodeObj->GetNodeId(), MN, false);
	}

	/*  1C09*/
	strcpy(indexId, "1C09");
	exceptionObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(true);

		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(),
				(char*) "40", false);

	}

	/*  1F26*/
	strcpy(indexId, "1F26");
	exceptionObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(true);

		char* val = new char[8];
		INT32 ilConfigDate;
		ilConfigDate = GetConfigDate();
		val = IntToAscii(ilConfigDate, val, 10);
		val = ConvertToUpper(val);
		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), val, false);
		delete[] val;

	}
	/*  1F27*/
	strcpy(indexId, "1F27");
	exceptionObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(true);

		char* val = new char[50];
		INT32 ilConfigTime;
		ilConfigTime = GetConfigTime();
		val = IntToAscii(ilConfigTime, val, 10);
		val = ConvertToUpper(val);

		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), val, false);
		delete[] val;
	}

	/* 1F84*/
	strcpy(indexId, "1F84");
	exceptionObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(true);

		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), (char*) "",
				true);

	}

	delete[] indexId;
	delete[] sidxId;
}

void UpdatedCNDateORTime(Index* indexObj, INT32 nodeId, DateTime dateOrTime)
{
	ocfmRetCode objException;
	if (!indexObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexObj'";
		objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}

	SubIndex *sidxObj = NULL;
	char indexId[INDEX_LEN];
	INT32 indexPos;

	IndexCollection* indexCollObj = NULL;
	NodeCollection* nodeCollObj = NULL;
	Node* nodeObj = NULL;
	char* subIndexId = new char[3 + STR_ALLOC_BUFFER];

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(CN, nodeId);
	indexCollObj = nodeObj->GetIndexCollection();

	subIndexId = IntToAscii(nodeId, subIndexId, 16);
	subIndexId = PadLeft(subIndexId, '0', 2);
	sidxObj = indexObj->GetSubIndexbyIndexValue(subIndexId);
	delete[] subIndexId;
	strcpy(indexId, "1020");

	if (OCFM_ERR_SUCCESS
			!= (IfIndexExists(nodeObj->GetNodeId(), CN, indexId, &indexPos)).getErrorCode())
	{
		return;
	}
	//pobjSIdx->getActualValue() convert to upper
	if (sidxObj != NULL)
	{
		if (sidxObj->GetActualValue() != NULL)
		{
			char sidxId[SUBINDEX_LEN];
			if (dateOrTime == DATE)
			{
				strcpy(sidxId, "01");
				SetSIdxValue(indexId, sidxId, (char*) sidxObj->GetActualValue(),
						indexCollObj, nodeObj->GetNodeId(), CN, false);
			}
			else if (dateOrTime == TIME)
			{
				strcpy(sidxId, "02");
				SetSIdxValue(indexId, sidxId, (char*) sidxObj->GetActualValue(),
						indexCollObj, nodeObj->GetNodeId(), CN, false);
			}
		}
	}
}

void CopyPDODefToAct(INT32 nodeId, NodeType nodeType)
{

	SubIndex* sidxObj = NULL;
	IndexCollection* indexCollObj = NULL;

	Node *nodeObj = NULL;
	NodeCollection *nodeCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
	indexCollObj = nodeObj->GetIndexCollection();
	Index* indexObj = NULL;
	for (INT32 indexLC = 0; indexLC < indexCollObj->GetNumberofIndexes();
			indexLC++)
	{
		indexObj = indexCollObj->GetIndexByPosition(indexLC);

		if (!(CheckIfNotPDO((char*) indexObj->GetIndexValue())))
		{

			for (INT32 sIdxLC = 0; sIdxLC < indexObj->GetNumberofSubIndexes();
					sIdxLC++)
			{
				sidxObj = indexObj->GetSubIndexByPosition(sIdxLC);
				if (sidxObj->GetActualValue() == NULL)
				{
					if (sidxObj->GetDefaultValue() != NULL)
					{
						sidxObj->SetActualValue(
								(char*) sidxObj->GetDefaultValue());
					}

				}
			}
		}
	}
}

Index GetPDOIndexByOffset(Index* indexObj)
{
	if (!indexObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexObj'";
		ocfmRetCode objException(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}

	Index tempIndexObj;
	tempIndexObj = *indexObj;
	SubIndex* sidxObj1 = NULL;
	SubIndex* sidxObj2 = NULL;

	for (INT32 idxLC1 = 1; idxLC1 <= (tempIndexObj.GetNumberofSubIndexes() - 1);
			idxLC1++)
	{
		for (INT32 idxLC2 = 1;
				idxLC2 <= (tempIndexObj.GetNumberofSubIndexes() - 1 - idxLC1);
				idxLC2++)
		{
			sidxObj1 = tempIndexObj.GetSubIndexByPosition(idxLC2);
			if ((sidxObj1->GetActualValue() != NULL)
					&& (0 != strcmp(sidxObj1->GetActualValue(), ""))
					&& !(CheckIfValueZero((char*) sidxObj1->GetActualValue())))
			{
				const char* actualVal1 = sidxObj1->GetActualValue();

				INT32 length1 = strlen(actualVal1);
				INT32 offsetVal1 = 0;
				char* offset1 = new char[INDEX_LEN];

				offset1 = SubString(offset1, actualVal1, length1 - 12, 4);
				offsetVal1 = (INT32) HexToInt(offset1);
				delete[] offset1;

				sidxObj2 = tempIndexObj.GetSubIndexByPosition(idxLC2 + 1);
				if ((NULL != sidxObj2->GetActualValue())
						&& (0 != strcmp(sidxObj2->GetActualValue(), ""))
						&& !(CheckIfValueZero(
								(char*) sidxObj2->GetActualValue())))
				{
					const char* actualVal2 = sidxObj2->GetActualValue();
					INT32 length2 = strlen(actualVal2);
					char* offset2 = new char[INDEX_LEN];
					INT32 offsetVal2 = 0;

					offset2 = SubString(offset2, actualVal2, length2 - 12, 4);
					offsetVal2 = (INT32) HexToInt(offset2);
					delete[] offset2;

					if (offsetVal1 > offsetVal2)
					{
						tempIndexObj.SwapSubObjects(idxLC2, idxLC2 + 1);

					}
				}
			}
		}
	}

	return tempIndexObj;
}
ocfmRetCode UpdateNodeParams(INT32 currentNodeId, INT32 newNodeID,
		NodeType nodeType, char* nodeName, StationType stationType,
		char* forcedCycleVal, bool isForcedCycle, char* presTimeoutVal)
{
	ocfmRetCode exceptionObj;
	UINT32 nodePos;
	try
	{
		bool bFlag = false;

		if (newNodeID == currentNodeId)
		{
			exceptionObj = IfNodeExists(newNodeID, nodeType, &nodePos, bFlag);
			if (exceptionObj.getErrorCode() != 0 && bFlag != true)
			{
				boost::format formatter(kMsgNodeIdInvalid);
				formatter % newNodeID;
				exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEID);
				exceptionObj.setErrorString(formatter.str());
				LOG_FATAL() << exceptionObj.getErrorString();
				throw exceptionObj;
			}

		}
		else if (newNodeID != currentNodeId)
		{
			exceptionObj = IfNodeExists(newNodeID, nodeType, &nodePos, bFlag);
			if (exceptionObj.getErrorCode() == OCFM_ERR_SUCCESS && bFlag == true)
			{
				boost::format formatter(kMsgExistingNode);
				formatter % newNodeID;
				exceptionObj.setErrorCode(OCFM_ERR_NODE_ALREADY_EXISTS);
				exceptionObj.setErrorString(formatter.str());
				LOG_FATAL() << formatter.str();
				throw exceptionObj;
			}
			else
			{
				LOG_ERROR() << "IfNodeExists() returned error code " 
					<< exceptionObj.getErrorCode() 
					<< " for node " << newNodeID << ".";
			}
		}

		Node* nodeObj;
		NodeCollection *nodeCollObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNodePtr(nodeType, currentNodeId);

		if (nodeType == CN)
		{
			if (newNodeID != currentNodeId)
			{
				nodeObj->SetNodeId(newNodeID);
				CopyOldNodeIdAssignmentObject(nodeObj, currentNodeId);
			}
			}

		if ((stationType != MULTIPLEXED)
				|| ((newNodeID != currentNodeId) && (stationType == MULTIPLEXED)))
		{
			char* cSIdx = new char[SUBINDEX_LEN];
			cSIdx = IntToAscii(currentNodeId, cSIdx, 16);
			cSIdx = PadLeft(cSIdx, '0', 2);
			nodeObj->ResetForcedCycleValue();
			delete[] cSIdx;
		}

		if ((nodeType == CN)
				&& !((presTimeoutVal == NULL)
						|| (strcmp(presTimeoutVal, "") == 0)))
		{
			nodeObj->SetPollResponseTimeout(presTimeoutVal);
		}
		nodeObj->SetNodeName(nodeName);

		if (nodeType == CN)
		{
			StationType prevStationType = nodeObj->GetStationType();
			nodeObj->SetStationType(stationType);
			if ((prevStationType == MULTIPLEXED)
					&& (stationType != prevStationType))
			{
				CheckAndReAssignMultiplex(newNodeID,
						nodeObj->GetForcedCycleValue());
			}

			if (stationType != MULTIPLEXED)
			{
				nodeObj->ResetForcedCycleValue();
			}

			if (stationType == MULTIPLEXED)
			{
				bool calcForceCycle = true;
				bool setForceCycle = true;
				if (isForcedCycle == true)
				{
					calcForceCycle = false;
				}

				if ((forcedCycleVal == NULL || strcmp(forcedCycleVal, "") == 0)
						&& isForcedCycle == false)
				{
					if (nodeObj->GetForceCycleFlag() == true)
					{

					}
					else if (nodeObj->GetForcedCycleValue() == NULL
							|| strcmp(nodeObj->GetForcedCycleValue(), "") == 0)
					{

					}
					else
					{

						INT32 actualValue = 0;
						if (strncmp(nodeObj->GetForcedCycleValue(), "0x", 2)
								== 0
								|| strncmp(nodeObj->GetForcedCycleValue(), "0X",
										2) == 0)
						{
							INT32 forcedLen = strlen(nodeObj->GetForcedCycleValue());
							char* forcedVal = new char[forcedLen];
							SubString(forcedVal, (const char*) nodeObj->GetForcedCycleValue(), 2, forcedLen - 2);
							actualValue = (INT32) HexToInt(forcedVal);
							delete[] forcedVal;
						}
						else
						{
							actualValue = atoi(nodeObj->GetForcedCycleValue());
						}

						if (true
								== IsMultiplexCycleNumberContinuous(
										actualValue))
						{
							calcForceCycle = false;
							setForceCycle = false;
						}
					}
				}

				if (calcForceCycle == true)
				{
					forcedCycleVal = GetLastAvailableCycleNumber();
				}

				if (setForceCycle == true)
				{
					exceptionObj = nodeObj->SetForcedCycle(forcedCycleVal);
				}

				if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
				{
					return exceptionObj;
				}

				nodeObj->SetForceCycleFlag(isForcedCycle);
			}
		}

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);

	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

ocfmRetCode GetNodeDataTypes(INT32 nodeId, NodeType nodeType,
		char* outDataTypes)
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	UINT32 nodePos;
	try
	{
		bool existFlag = false;
		exceptionObj = IfNodeExists(nodeId, nodeType, &nodePos, existFlag);
		if (exceptionObj.getErrorCode() != 0 && existFlag != true)
		{
			boost::format formatter(kMsgNonExistingNode);
			formatter % nodeId;
			exceptionObj.setErrorCode(OCFM_ERR_NODEID_NOT_FOUND);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}

		Node* nodeObj;
		NodeCollection *nodeCollObj;
		DataTypeCollection *dtCollObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
		dtCollObj = nodeObj->GetDataTypeCollection();

		for (UINT16 dtLC = 0; dtLC < dtCollObj->GetNumberOfDataTypes(); dtLC++)
		{
			DataType *dtObj = NULL;
			char* dtName = NULL;

			dtObj = dtCollObj->GetDataTypeElement(dtLC);
			dtName = new char[strlen(dtObj->GetName()) + STR_ALLOC_BUFFER];

			strcpy(dtName, dtObj->GetName());
			if (dtLC == 0)
			{
				strcpy(outDataTypes, dtName);
			}
			else
			{
				strcat(outDataTypes, dtName);

			}

			strcat(outDataTypes, "\n");
			delete[] dtName;
		}

		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

void GetAllNodeIdAssignment(char* Buffer1, bool isReAssignment)
{
	NodeCollection *objNodeCollection = NULL;
	Node nodeObjMN;
	IndexCollection *mnIndexCollObj = NULL;
	Index* mnIdxObj = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	nodeObjMN = objNodeCollection->GetMNNode();
	mnIndexCollObj = nodeObjMN.GetIndexCollection();
	if(mnIndexCollObj == NULL)
	{
		LOG_ERROR() << "MN OD empty.";
		return;
	}
	mnIdxObj = mnIndexCollObj->GetIndexbyIndexValue((char*) "1F81");
	if(mnIdxObj == NULL)
	{
		LOG_ERROR() << "Index 0x1F81 does not exist in MN OD.";
		return;
	}

	for(INT32 nodeLC = 0; nodeLC < objNodeCollection->GetNumberOfNodes();
		nodeLC++)
	{
		Node nodeObjCN;
		nodeObjCN = objNodeCollection->GetNodebyCollectionIndex(nodeLC);
		if (nodeObjCN.GetNodeType() == CN)
		{
			if(isReAssignment == false)
			{
				strcat(Buffer1, "//// NodeId Assignment\n");
			}
			else
			{
				strcat(Buffer1, "//// NodeId Reassignment\n");
			}
			strcat(Buffer1, "1F81");
			strcat(Buffer1, "\t");
			INT32 nodeID = nodeObjCN.GetNodeId();
			LOG_INFO() << " Writing 0x1F81 data for CN " << nodeID;
			char* tempStr = new char[10];
			tempStr = IntToAscii(nodeID, tempStr, 16);
			tempStr = PadLeft(tempStr, '0', 2);
			strcat(Buffer1, tempStr);
			strcat(Buffer1, "\t00000004\t");

			SubIndex* mnSidxObj = mnIdxObj->GetSubIndexbyIndexValue(tempStr);
			if(mnSidxObj != NULL)
			{
				char* orgValue = NULL;
				if(mnSidxObj->GetActualValue() != NULL)
				{
					orgValue = new char[strlen(mnSidxObj->GetActualValue()) + STR_ALLOC_BUFFER];
					strcpy(orgValue, mnSidxObj->GetActualValue());
				}
				else if(mnSidxObj->GetDefaultValue() != NULL)
				{
					orgValue = new char[strlen(mnSidxObj->GetDefaultValue()) + STR_ALLOC_BUFFER];
					strcpy(orgValue, mnSidxObj->GetDefaultValue());
				}
				else
				{
					LOG_DEBUG() << "No actualValue and defaultValue set.";
					orgValue = new char[11];
					strcpy(orgValue, "0x00000000");
				}

				if(strlen(orgValue) != 10)
				{
					LOG_ERROR() << "Invalid string-length for value: " << orgValue;
				}

				if ((strncmp(orgValue, "0x", 2) == 0) || (strncmp(orgValue, "0X", 2) == 0))
				{
					char* destStr = new char[9];
					SubString(destStr, orgValue, 2, 8);
					PadLeft(destStr, '0', 8);
					if(isReAssignment == false)
					{
						char* temp = new char[9];
						strcat(Buffer1, "0");
						SubString(temp, destStr, 1, 7);
						strcat(Buffer1, temp);
						delete[] temp;
					}
					else
					{
						strcat(Buffer1, destStr);
					}
					delete[] destStr;
				}
				else
				{
					char* destStr = new char[9];
					strcpy(destStr, IntToAscii(atoi(orgValue), destStr, 16));
					strcat(Buffer1, PadLeft(destStr, '0', 8));
					delete[] destStr;
				}
				delete[] orgValue;
			}
			else
			{
				LOG_ERROR() << "SubIndex 0x1F81/0x" << tempStr << " not found.";
				//handled error case
				strcat(Buffer1, "00000007");
			}

			strcat(Buffer1, "\n");
			delete[] tempStr;
		}
	}
}

char* GetNodeAssigmentBits(Node* nodeObj)
{
	ocfmRetCode exceptionObj;
	if (!nodeObj)
	{	
		boost::format formatter(kMsgNullArgument);
		formatter % "'nodeObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	char* nodeAssignData = NULL;
	ULONG tempValue;
	nodeAssignData = new char[10 + STR_ALLOC_BUFFER];
	tempValue = EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS
			| EPL_NODEASSIGN_NODE_IS_CN | EPL_NODEASSIGN_START_CN;
	switch (nodeObj->GetStationType())
	{
		case NORMAL:
			break;
		case MULTIPLEXED:
			tempValue = tempValue | EPL_NODEASSIGN_MULTIPLEXED_CN;
			break;
		case CHAINED:
			tempValue = tempValue | EPL_NODEASSIGN_CHAINED_CN;
			break;
		default:
			LOG_ERROR() << "Unknown station type: " << nodeObj->GetStationType() << ".";
			break;
	};

	nodeAssignData = IntToAscii(tempValue, nodeAssignData, 16);
	return nodeAssignData;
}

void SetPresMNNodeAssigmentBits()
{
	INT32 indexPos;
	INT32 subIndexPos;
	ocfmRetCode exceptionObj;
	exceptionObj = IfSubIndexExists(MN_NODEID, MN, (char *) "1F81", (char *) "F0",
			&subIndexPos, &indexPos);

	if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
	{
		return;
	}

	bool isPresMn = false;
	bool isMNBroadcastingPRes = false;

	//check whether Pres activated in MN
	isPresMn = IsPresMN();

	NodeCollection* nodeCollObj = NULL;
	Node nodeObj;
	IndexCollection* indexCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetMNNode();

	indexCollObj = nodeObj.GetIndexCollection();
	Index* indexObj = NULL;
	//If pres chaining is enabled MN will use 1A00 is only used for transferring the Pres data.
	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1A00");
	if(NULL != indexObj)
	{
		SubIndex *mappSidxObj = NULL;
		mappSidxObj = indexObj->GetSubIndexbyIndexValue((char *) "00");
		if (NULL != mappSidxObj)
		{
			if ((NULL != mappSidxObj->GetActualValue())
				&& (0 != strcmp(mappSidxObj->GetActualValue(), "")))
			{
				INT32 nrEntries = 0;
				nrEntries = GetDecimalValue((char *) mappSidxObj->GetActualValue());
				if (nrEntries > 0)
				{
					Index *commIndexObj = NULL;
					commIndexObj = indexCollObj->GetIndexbyIndexValue((char*) "1800");

					if (NULL != commIndexObj)
					{
						SubIndex *commSidxObj = NULL;
						commSidxObj = commIndexObj->GetSubIndexbyIndexValue((char *) "01");
						if (NULL != commSidxObj)
						{
							if ((NULL != commSidxObj->GetActualValue())
								&& (0 != strcmp(commSidxObj->GetActualValue(), "")))
							{
								INT32 tpdoMappedNodeId = 0;
								tpdoMappedNodeId = GetDecimalValue((char *) commSidxObj->GetActualValue());
								if (BROADCAST_NODEID == tpdoMappedNodeId)
								{
									isMNBroadcastingPRes = true;
									//break;
								}
							}
						}
					}
				}
			}
		}
	}

	//check whether MN is transmitting PRes
	if ((false == isPresMn) && (false == isMNBroadcastingPRes))
	{
		return;
	}

	SubIndex* sidxObj;
	sidxObj = GetMNSubIndexValues((char*) "1F81", (char*) "F0");
	if (NULL != sidxObj)
	{
		if ((true == isPresMn) || (true == isMNBroadcastingPRes))
		{
			char* nodeAssignData = NULL;
			ULONG tempValue;
			nodeAssignData = new char[10 + STR_ALLOC_BUFFER];
			tempValue = EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS
					| EPL_NODEASSIGN_MN_PRES;
			strcpy(nodeAssignData, (char *) "0x");
			IntToAscii(tempValue, &nodeAssignData[2], 16);

			sidxObj->SetActualValue(nodeAssignData);
			sidxObj->SetFlagIfIncludedCdc(true);
			delete[] nodeAssignData;
		}
		}
	}

ocfmRetCode RecalculateMultiplex()
{
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	NodeCollection *nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	try
	{
		if (false == CheckIfMultiplexedCNExist())
		{
			exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
			return exceptionObj;
		}
		INT32 indexPos = 0;
		INT32 subIndexPos = 0;
		exceptionObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &indexPos);
		if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			//reset automatically assigned cn force cycle
			ResetMultiplexedCNForceCycle();
			exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
			return exceptionObj;
		}

		char* actValue = new char[50];
		actValue[0] = 0;
		exceptionObj = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			LOG_FATAL() <<exceptionObj.getErrorCode();
			delete[] actValue;
			throw exceptionObj;
		}

		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{
			Node* nodeObj = NULL;
			nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
			if (nodeObj->GetNodeType() == CN)
			{
				static char* value = NULL;
				if (value != NULL)
				{
					delete[] value;
					value = NULL;
				}
				value = new char[SUBINDEX_LEN];
				char indexId[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
				value = IntToAscii(nodeObj->GetNodeId(), value, 16);
				value = PadLeft(value, '0', 2);
				char* sIdxActValue = new char[20];
				sIdxActValue[0] = 0;
				try
				{
					exceptionObj = GetSubIndexAttributes(MN_NODEID, MN, indexId,
							value, ACTUALVALUE, sIdxActValue);
				} catch (...)
				{
					delete[] sIdxActValue;
					continue;
				}
				if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
				{
					continue;
				}
				if (nodeObj->GetStationType() == MULTIPLEXED)
				{
					if (nodeObj->GetForceCycleFlag() == 1)
					{
						continue;
					}
					else if (nodeObj->GetForcedCycleValue() == NULL
							|| strcmp(nodeObj->GetForcedCycleValue(), "") == 0)
					{
						//continue down the loop
					}
					else if (actValue == NULL || strcmp(actValue, "") == 0)
					{
						char* sidxName = new char[50];
						sidxName[0] = 0;
						GetSubIndexAttributes(MN_NODEID, MN, indexId, value,
								NAME, sidxName);
						char* subIndFlag = new char[10];
						GetSubIndexAttributes(MN_NODEID, MN, indexId, value,
								FLAGIFINCDC, subIndFlag);
						INT32 sidxExistFlag = atoi(subIndFlag);
						SetBasicSubIndexAttributes(MN_NODEID, MN, indexId,
								value, (char*) "", sidxName,
								(sidxExistFlag != 0));
						delete[] sidxName;
						delete[] subIndFlag;
						continue;
					}
					else
					{
						if (exceptionObj.getErrorCode() == OCFM_ERR_SUCCESS)
						{
							INT32 mnMultiActualValue = 0;
							if (strncmp(actValue, "0x", 2) == 0
									|| strncmp(actValue, "0X", 2) == 0)
							{
								char *tempActVal = new char[strlen(actValue)];
								SubString(tempActVal, (const char*) actValue, 2, strlen(actValue) - 2);
								mnMultiActualValue = (INT32) HexToInt(tempActVal);
								delete[] tempActVal;
							}
							else
							{
								mnMultiActualValue = atoi(actValue);
							}

							char* forcedCycleValue = new char[50];
							strcpy(forcedCycleValue,
									nodeObj->GetForcedCycleValue());
							INT32 cnActualValue = 0;
							if (strncmp(forcedCycleValue, "0x", 2) == 0
								|| strncmp(forcedCycleValue, "0X", 2) == 0)
							{
								char *tempForcedVal = new char[strlen(forcedCycleValue)];
								SubString(tempForcedVal, (const char*) forcedCycleValue, 2, strlen(forcedCycleValue) - 2);
								cnActualValue = (INT32) HexToInt(tempForcedVal);
								delete[] tempForcedVal;
							}
							else
							{
								cnActualValue = atoi(forcedCycleValue);
							}

							if (mnMultiActualValue == 0)
							{

								char* sidxName = new char[50];
								sidxName[0] = 0;
								GetSubIndexAttributes(MN_NODEID, MN, indexId,
										value, NAME, sidxName);
								char* subIndFlag = new char[10];
								GetSubIndexAttributes(MN_NODEID, MN, indexId,
										value, FLAGIFINCDC, subIndFlag);
								INT32 iCNsubIndFlag = atoi(subIndFlag);
								SetBasicSubIndexAttributes(MN_NODEID, MN,
										indexId, value, (char*) "", sidxName,
										(iCNsubIndFlag != 0));
								delete[] sidxName;
								delete[] subIndFlag;
								continue;
							}
							else if (cnActualValue <= mnMultiActualValue)
							{
								continue;
							}
							else
							{
								//continue the function
							}
							delete[] forcedCycleValue;
						}
						else
						{

						}
					}

					char* ForcedCycle = GetLastAvailableCycleNumber();
					nodeObj->SetForcedCycle(ForcedCycle);
					delete[] ForcedCycle;
				}
				else
				{
					// station other than multiplexed
					char* subIndName = new char[50];
					subIndName[0] = 0;
					GetSubIndexAttributes(MN_NODEID, MN, indexId, value, NAME,
							subIndName);

					char* subIndFlag = new char[10];
					GetSubIndexAttributes(MN_NODEID, MN, indexId, value,
							FLAGIFINCDC, subIndFlag);

					INT32 iCNsubIndFlag = atoi(subIndFlag);
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexId, value,
							(char*) "", subIndName, (iCNsubIndFlag != 0));
					delete[] subIndName;
					delete[] subIndFlag;
				}
				delete[] value;
			} // end of if loop 1
			else
			{
				continue;
				//node type == mn
			}
		} //end of for loop
		delete[] actValue;
	} //end of try
	catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

void ResetMultiplexedCNForceCycle()
{
	NodeCollection *nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes(); nodeLC++)
	{
		Node* nodeObj = NULL;
		nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
		if (nodeObj->GetNodeType() == CN)
		{
			ocfmRetCode exceptionObj;
			static char* value = NULL;
			if (value != NULL)
			{
				delete[] value;
				value = NULL;
			}
			value = new char[SUBINDEX_LEN];

			value = IntToAscii(nodeObj->GetNodeId(), value, 16);
			value = PadLeft(value, '0', 2);
			char* sidxActValue = new char[20];
			sidxActValue[0] = 0;
			try
			{
				char indexId[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
				exceptionObj = GetSubIndexAttributes(MN_NODEID, MN, indexId,
						value, ACTUALVALUE, sidxActValue);
			} catch (...)
			{
				delete[] sidxActValue;
				continue;
			}
			if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				continue;
			}
			if (nodeObj->GetStationType() == MULTIPLEXED)
			{
				if (nodeObj->GetForceCycleFlag() == 1)
				{
					continue;
				}
				nodeObj->ResetForcedCycleValue();
				nodeObj->SetStationType(NORMAL);
			}
			delete[] value;
		}
	}
}

void CopyMNPropDefToAct(INT32 nodeId, NodeType nodeType)
{
	if (nodeType != MN)
	{
		return;
	}
	CopyIndexDefToAct(nodeId, nodeType, (char *) "1006");
	CopySubIndexDefToAct(nodeId, nodeType, false, (char *) "1F8A",
			(char *) "02");
	CopySubIndexDefToAct(nodeId, nodeType, false, (char *) "1F98",
			(char *) "07");
	CopySubIndexDefToAct(nodeId, nodeType, false, (char *) "1F98",
			(char *) "08");
}

void CopyIndexDefToAct(INT32 nodeId, NodeType nodeType, char *indexId)
{
	IndexCollection* indexCollObj = NULL;

	Node *nodeObj = NULL;
	NodeCollection *nodeCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
	indexCollObj = nodeObj->GetIndexCollection();

	Index* indexObj = NULL;

	indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
	if (indexObj == NULL)
	{
		return;
	}
	if (indexObj->GetActualValue() == NULL)
	{
		if (indexObj->GetDefaultValue() != NULL)
		{
			indexObj->SetActualValue((char*) indexObj->GetDefaultValue());
		}
	}
}

void CopySubIndexDefToAct(INT32 nodeId, NodeType nodeType, bool forceCopy,
		char *indexId, char *subIndexId)
{
	SubIndex* pobjSIndex = NULL;
	IndexCollection* pobjIdxCol = NULL;

	Node *pobjNode = NULL;
	NodeCollection *objNodeCollection = NULL;

	objNodeCollection = NodeCollection::GetNodeColObjectPointer();
	pobjNode = objNodeCollection->GetNodePtr(nodeType, nodeId);
	pobjIdxCol = pobjNode->GetIndexCollection();

	Index* pobjIndex = NULL;

	pobjIndex = pobjIdxCol->GetIndexbyIndexValue(indexId);
	if (pobjIndex == NULL)
	{
		return;
	}

	pobjSIndex = pobjIndex->GetSubIndexbyIndexValue(subIndexId);
	if (pobjSIndex == NULL)
	{
		return;
	}
	if ((pobjSIndex->GetActualValue() == NULL) || (true == forceCopy))
	{
		if (pobjSIndex->GetDefaultValue() != NULL)
		{
			pobjSIndex->SetActualValue((char*) pobjSIndex->GetDefaultValue());
		}
		else
		{
			if (true == forceCopy)
			{
				pobjSIndex->SetActualValue((char*) "");
			}
		}
	}
}

void CheckAndReAssignMultiplex(INT32 nodeId, const char* cycleValue)
{
	ocfmRetCode exceptionObj;
	if (cycleValue == NULL || strcmp(cycleValue, "") == 0)
	{
		exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
		//return stErrorInfo;
		return;
	}

	INT32 tempCycleValue = 0;
	if (strncmp(cycleValue, "0x", 2) == 0 || strncmp(cycleValue, "0X", 2) == 0)
	{
		INT32 cycleValLen = strlen(cycleValue);
		char* tempCycleStr = new char[cycleValLen];
		SubString(tempCycleStr, (const char*) cycleValue, 2, cycleValLen - 2);
		tempCycleValue = (INT32) HexToInt(tempCycleStr);
		delete[] tempCycleStr;
	}
	else
		tempCycleValue = atoi(cycleValue);

	NodeCollection *objNodeCol;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	INT32 nodesCount = 0;
	nodesCount = objNodeCol->GetCNNodesCount();

	Node* objNode;

	try
	{
		if (nodesCount == 0)
		{
			return;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		try
		{

			exceptionObj = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
					(char*) "07", ACTUALVALUE, actValue);
		} catch (...)
		{
			delete[] actValue;
			exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
			return;
		}

		for (INT32 nodeLC = 0; nodeLC < objNodeCol->GetNumberOfNodes();
				nodeLC++)
		{
			objNode = objNodeCol->GetNodebyColIndex(nodeLC);
			if (objNode->GetNodeType() == CN && objNode->GetNodeId() != nodeId
					&& objNode->GetStationType() == MULTIPLEXED)
			{
				if (objNode->GetForcedCycleValue() == NULL
						|| strcmp(objNode->GetForcedCycleValue(), "") == 0)
				{
					continue;
				}
				else
				{
					char* ForcedCycleValue = new char[strlen(
							objNode->GetForcedCycleValue()) + ALLOC_BUFFER];
					strcpy(ForcedCycleValue, objNode->GetForcedCycleValue());
					INT32 actualValueCN = 0;
					if (strncmp(ForcedCycleValue, "0x", 2) == 0
						|| strncmp(ForcedCycleValue, "0X", 2) == 0)
					{
						char *tempForcedVal = new char[strlen(ForcedCycleValue)];
						SubString(tempForcedVal, (const char*) ForcedCycleValue, 2, strlen(ForcedCycleValue) - 2);
						actualValueCN = (INT32) HexToInt(tempForcedVal);
						delete[] tempForcedVal;
					}
					else
					{
						actualValueCN = atoi(ForcedCycleValue);
					}

					delete[] ForcedCycleValue;
					if (tempCycleValue == actualValueCN)
					{
						return;
					}
					delete[] ForcedCycleValue;
				}
			} // end of if loop 1
		} //end of for loop
		for (INT32 nodeLC = 0; nodeLC < objNodeCol->GetNumberOfNodes();
				nodeLC++)
		{
			objNode = objNodeCol->GetNodebyColIndex(nodeLC);
			if (objNode->GetNodeType() == CN && objNode->GetNodeId() != nodeId
					&& objNode->GetStationType() == MULTIPLEXED
					&& objNode->GetForceCycleFlag() == false)
			{
				if (objNode->GetForcedCycleValue() == NULL
						|| strcmp(objNode->GetForcedCycleValue(), "") == 0)
				{
					continue;
				}

				char* forcedCycleValue = new char[strlen(
						objNode->GetForcedCycleValue()) + ALLOC_BUFFER];
				strcpy(forcedCycleValue, objNode->GetForcedCycleValue());
				INT32 actualValueCN = 0;
				if (strncmp(forcedCycleValue, "0x", 2) == 0
					|| strncmp(forcedCycleValue, "0X", 2) == 0)
				{
					char *tempForcedVal = new char[strlen(forcedCycleValue)];
					SubString(tempForcedVal, (const char*) forcedCycleValue, 2, strlen(forcedCycleValue) - 2);
					actualValueCN = (INT32) HexToInt(tempForcedVal);
					delete[] tempForcedVal;
				}
				else
				{
					actualValueCN = atoi(forcedCycleValue);
				}

				delete[] forcedCycleValue;
				if (tempCycleValue >= actualValueCN || actualValueCN == 1)
				{
					continue;
				}
				actualValueCN--;
				cycleNumberGlobal = actualValueCN;

				static char* tempActualValue = NULL;
				//if (tempActualValue != NULL)
				//{
				//	delete[] tempActualValue;
				//	tempActualValue = NULL;
				//}
				tempActualValue = new char[50];
				tempActualValue = IntToAscii(actualValueCN, tempActualValue,
						10);

				objNode->SetForcedCycle(tempActualValue);

				delete[] tempActualValue;
			} //end of if loop
		} //end of for loop
		delete[] actValue;
	} //end of try
	catch (...)
	{
		return;
	}
}

ocfmRetCode CheckMutliplexAssigned()
{
	ocfmRetCode exceptionObj;
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	INT32 nodesCount = 0;
	nodesCount = nodeCollObj->GetCNNodesCount();
	INT32 assignedCycle = 0;
	INT32 nodeId = 0;

	try
	{
		bool setErrFlag = false;
		if (nodesCount == 0)
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_CN_NODES_FOUND);
			exceptionObj.setErrorString(kMsgNoControlledNodesConfigured);
			LOG_FATAL() << kMsgNoControlledNodesConfigured;
			return exceptionObj;
		}
		if (false == CheckIfMultiplexedCNExist())
		{
			return exceptionObj;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		INT32 subIndexPos = 0;
		INT32 indexPos = 0;
		exceptionObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &indexPos);
		if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			exceptionObj.setErrorCode(OCFM_ERR_MULTIPLEXING_NOT_SUPPORTED);
			exceptionObj.setErrorString(kMsgMultiplexingNotSupported);
			LOG_ERROR() << kMsgMultiplexingNotSupported;
			delete[] actValue;
			return exceptionObj;
		}
		exceptionObj = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		{
			delete[] actValue;
			return exceptionObj;
		}

		INT32 tempActualValue = 0;
		if (strncmp(actValue, "0x", 2) == 0 || strncmp(actValue, "0X", 2) == 0)
		{
			INT32 actLen = strlen(actValue);
			char *tempActStr = new char[actLen];
			SubString(tempActStr, (const char*) actValue, 2, actLen - 2);
			tempActualValue = (INT32) HexToInt(tempActStr);
			delete[] tempActStr;
		}
		else
			tempActualValue = atoi(actValue);

		
		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{
			Node* nodeObj = NULL;
			nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
			if (nodeObj->GetNodeType() == CN)
			{
				if (nodeObj->GetStationType() == MULTIPLEXED)
				{
					static char* tempValue = NULL;
					if (tempValue != NULL)
					{
						delete[] tempValue;
						tempValue = NULL;
					}
					tempValue = new char[SUBINDEX_LEN];
					char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
					tempValue = IntToAscii(nodeObj->GetNodeId(), tempValue, 16);
					tempValue = PadLeft(tempValue, '0', 2);
					char* subIndActValue = new char[20];
					subIndActValue[0] = 0;
					try
					{
						exceptionObj = GetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, tempValue, ACTUALVALUE,
								subIndActValue);
					} catch (...)
					{
						delete[] subIndActValue;
						//delete[] strConvertedValue;
						continue;
					}
					if (exceptionObj.getErrorCode() == OCFM_ERR_SUCCESS)
					{
					}
					else
					{
						continue;
					}

					if (nodeObj->GetForcedCycleValue() == NULL
							|| strcmp(nodeObj->GetForcedCycleValue(), "") == 0)
					{
						//continue down the loop
					}
					else if (actValue == NULL || strcmp(actValue, "") == 0)
					{
						continue;
					}
					else
					{
						char* forcedCycleValue = new char[50];
						strcpy(forcedCycleValue,
								nodeObj->GetForcedCycleValue());
						INT32 actualValueCN = 0;
						if (strncmp(forcedCycleValue, "0x", 2) == 0
							|| strncmp(forcedCycleValue, "0X", 2) == 0)
						{
							char *tempForcedVal = new char[strlen(forcedCycleValue)];
							SubString(tempForcedVal, (const char*) forcedCycleValue, 2, strlen(forcedCycleValue) - 2);
							actualValueCN = (INT32) HexToInt(tempForcedVal);
							delete[] tempForcedVal;
							delete[] forcedCycleValue;
						}
						else
						{
							actualValueCN = atoi(forcedCycleValue);
							delete[] forcedCycleValue;
						}

						if (actualValueCN <= tempActualValue)
						{
							continue;
						}
						else
						{
							setErrFlag = true;
							assignedCycle = actualValueCN;
							nodeId = nodeObj->GetNodeId();
						}
					}
					delete[] tempValue;
				}
				else
				{
				}
			} // end of if loop 1
		} //end of for loop
		if (setErrFlag == true)
		{
			boost::format formatter(kMsgMultiplexCycleAssignInvalid);
			formatter % assignedCycle
				% nodeId
				% tempActualValue;
			exceptionObj.setErrorCode(OCFM_ERR_MULTIPLEX_ASSIGN_ERROR);
			exceptionObj.setErrorString(formatter.str());
			LOG_ERROR() << formatter.str();
		}
		delete[] actValue;
	} //end of try
	catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

UINT32 GetFreeCycleNumber(UINT32 parmCycleNumber)
{
	ocfmRetCode exceptionObj;
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	INT32 nodeCount = 0;
	nodeCount = nodeCollObj->GetCNNodesCount();

	Node* nodeObj;

	if (nodeCount == 0)
	{
		// FIXME: Refactor method to return ocfmRetCode
		exceptionObj.setErrorCode(OCFM_ERR_NO_CN_NODES_FOUND);
		exceptionObj.setErrorString(kMsgNoControlledNodesConfigured);
		LOG_FATAL() << kMsgNoControlledNodesConfigured;
		return parmCycleNumber;
	}
	for (UINT32 cycleNumberLC = 1; cycleNumberLC < parmCycleNumber;
			cycleNumberLC++)
	{
		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{
			nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
			if (nodeObj->GetNodeType() == CN
					&& nodeObj->GetStationType() == MULTIPLEXED)
			{
				if (nodeObj->GetForcedCycleValue() == NULL
						|| strcmp(nodeObj->GetForcedCycleValue(), "") == 0)
				{
					//continue;
				}
				else
				{
					char* forcedCycleValue = new char[strlen(
							nodeObj->GetForcedCycleValue()) + ALLOC_BUFFER];
					strcpy(forcedCycleValue, nodeObj->GetForcedCycleValue());
					UINT32 actualValueCN = 0;
					if (strncmp(forcedCycleValue, "0x", 2) == 0
							|| strncmp(forcedCycleValue, "0X", 2) == 0)
					{
						char *tempForcedVal = new char[strlen(forcedCycleValue)];
						SubString(tempForcedVal, (const char*) forcedCycleValue, 2, strlen(forcedCycleValue) - 2);
						actualValueCN = (INT32) HexToInt(tempForcedVal);
						delete[] tempForcedVal;
					}
					else
						actualValueCN = atoi(forcedCycleValue);

					delete[] forcedCycleValue;
					if (cycleNumberLC == actualValueCN)
					{
						break;
					}
					}
			} // end of if loop 1
			if (nodeLC == nodeCollObj->GetNumberOfNodes() - 1)
			{
				return cycleNumberLC;
			}
		} //end of for loop

	} //end of for loop
	return parmCycleNumber;
}

bool IsMultiplexCycleNumberContinuous(UINT32 parmCycleNumber)
{
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	INT32 nodeCount = 0;
	nodeCount = nodeCollObj->GetCNNodesCount();

	Node* nodeObj = NULL;

	if (nodeCount == 0)
	{
		return false;
	}
	for (UINT32 cycleNumberLC = 1; cycleNumberLC < parmCycleNumber;
			cycleNumberLC++)
	{

		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{

			nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
			if (nodeObj->GetNodeType() == CN
					&& nodeObj->GetStationType() == MULTIPLEXED)
			{

				if (nodeObj->GetForcedCycleValue() == NULL
						|| strcmp(nodeObj->GetForcedCycleValue(), "") == 0)
				{
					//continue;
				}
				else
				{
					char* forcedCycleValue = new char[strlen(
							nodeObj->GetForcedCycleValue()) + ALLOC_BUFFER];
					strcpy(forcedCycleValue, nodeObj->GetForcedCycleValue());
					UINT32 actualValueCN = 0;
					if (strncmp(forcedCycleValue, "0x", 2) == 0
						|| strncmp(forcedCycleValue, "0X", 2) == 0)
					{
						char *tempForcedVal = new char[strlen(forcedCycleValue)];
						SubString(tempForcedVal, (const char*) forcedCycleValue, 2, strlen(forcedCycleValue) - 2);
						actualValueCN = (INT32) HexToInt(tempForcedVal);
						delete[] tempForcedVal;
					}
					else
					{
						actualValueCN = atoi(forcedCycleValue);
					}

					delete[] forcedCycleValue;
					if (cycleNumberLC == actualValueCN)
					{
						break;
					}
					}
			} // end of if loop 1
			if (nodeLC == nodeCollObj->GetNumberOfNodes() - 1)
			{
				return false;
			}
		} //end of for loop

	} //end of for loop
	return true;
}

void CalculateCNPollResponse(INT32 nodeId, NodeType nodeType)
{
	if (nodeType != CN)
	{
		return;
	}
	INT32 subIndexPos;
	INT32 indexPos;
	ocfmRetCode exceptionObj;
	exceptionObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98",
			(char*) "03", &subIndexPos, &indexPos);
	if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
	{
		return;
	}

	SubIndex* sidxObj = NULL;
	IndexCollection* indexCollObj = NULL;

	Node *nodeObj = NULL;
	NodeCollection *nodeCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
	indexCollObj = nodeObj->GetIndexCollection();

	Index* indexObj = NULL;

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1F98");
	if (indexObj == NULL)
	{
		return;
	}

	sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "03");
	if (sidxObj == NULL)
	{
		return;
	}
	char *tempValStr = NULL;
	INT32 tempValue = 0;
	bool add25microsec = false;
	if (sidxObj->GetActualValue() == NULL
			|| strcmp(sidxObj->GetActualValue(), "") == 0)
	{
		if (sidxObj->GetDefaultValue() == NULL
				|| strcmp(sidxObj->GetDefaultValue(), "") == 0)
		{
			tempValStr = new char[strlen("25000") + ALLOC_BUFFER];
			strcpy(tempValStr, "25000");
		}
		else
		{
			tempValStr = new char[strlen(sidxObj->GetDefaultValue())
					+ ALLOC_BUFFER];
			strcpy(tempValStr, sidxObj->GetDefaultValue());
			add25microsec = true;
		}
	}
	else
	{
		tempValStr = new char[strlen(sidxObj->GetActualValue()) + ALLOC_BUFFER];
		strcpy(tempValStr, sidxObj->GetActualValue());
	}

	if (strncmp(tempValStr, "0x", 2) == 0 || strncmp(tempValStr, "0X", 2) == 0)
	{
		INT32 templen = strlen(tempValStr);
		char * tempSubStr = new char[templen];
		SubString(tempSubStr, (const char*) tempValStr, 2, templen - 2);
		tempValue = (INT32) HexToInt(tempSubStr);
		delete[] tempSubStr;
	}
	else
		tempValue = atoi(tempValStr);

	if (true == add25microsec)
		tempValue += 25000;

	//add with 25 micro sec
	char *tempVal = new char[30];

	tempVal = IntToAscii(tempValue, tempVal, 10);
	nodeObj->SetPollResponseTimeout(tempVal);
	delete[] tempValStr;
	delete[] tempVal;
}


bool CheckIfMultiplexedCNExist()
{
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes(); nodeLC++)
	{
		Node* nodeObj = NULL;
		nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
		if (nodeObj->GetNodeType() == CN)
		{
			if (nodeObj->GetStationType() == MULTIPLEXED)
			{
				return true;
			}
		}
	}
	return false;
}

//TODO: cnCount is not used to be removed.
void UpdateMNNodeAssignmentIndex(Node *nodeObj, INT32 cnCount, char* indexId,
		bool allowMNSubindex)
{
	if ((NULL == indexId) || (NULL == nodeObj))
	{
		return;
	}

	IndexCollection *indexCollObj = NULL;
	ocfmRetCode exceptionObj;

	INT32 indexPos;

	indexCollObj = nodeObj->GetIndexCollection();
	char* tempIndexId = new char[INDEX_LEN + ALLOC_BUFFER];

	strcpy(tempIndexId, indexId);

	exceptionObj = IfIndexExists(MN_NODEID, MN, tempIndexId, &indexPos);
	delete[] tempIndexId;
	if (exceptionObj.getErrorCode() == OCFM_ERR_SUCCESS)
	{
		Index *indexObj = NULL;
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(true);

		for (INT32 sidxLC = indexObj->GetNumberofSubIndexes() - 1; sidxLC >= 0;
				sidxLC--)
		{
			SubIndex* sidxObj;
			sidxObj = indexObj->GetSubIndexByPosition(sidxLC);
			if (NULL == sidxObj)
				continue;

			if (0 == strcmp((char*) sidxObj->GetIndexValue(), "00"))
				continue;

			try
			{
				INT32 nodeIdVal = (INT32) HexToInt((char*) sidxObj->GetIndexValue());
				NodeType nodeType;
				if (MN_NODEID == nodeIdVal)
				{
					nodeType = MN;
				}
				else
				{
					nodeType = CN;
				}
				UINT32 nodePos;
				bool bFlag = false;
				exceptionObj = IfNodeExists(nodeIdVal, nodeType, &nodePos, bFlag);

				if (((OCFM_ERR_SUCCESS == exceptionObj.getErrorCode() ) && (true == bFlag)) && ((CN == nodeType) || (true == allowMNSubindex)))
				{
					//continue
				}
				else
				{
					if (NULL == sidxObj->GetDefaultValue())
						sidxObj->SetActualValue((char *) "");
					else
						sidxObj->SetActualValue(
								(char *) sidxObj->GetDefaultValue());
				}
			} catch (...)
			{
				if (NULL == sidxObj->GetDefaultValue())
					sidxObj->SetActualValue((char *) "");
				else
					sidxObj->SetActualValue(
							(char *) sidxObj->GetDefaultValue());
			}

		}
	}
}

bool ValidateCNPresTimeout(const char* subIndexId, const char* presTimeOutVal)
{
	ocfmRetCode exceptionObj;
	INT32 sidxPos = 0;
	INT32 indexPos = 0;
	bool retval = false;

	exceptionObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F92", subIndexId,
			&sidxPos, &indexPos);
	if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
	{
		return retval;
	}

	INT32 nodeId = (INT32) HexToInt(subIndexId);
	exceptionObj = IfSubIndexExists(nodeId, CN, (char*) "1F98", (char*) "03",
			&sidxPos, &indexPos);

	char* defaultValueCN = new char[50];
	if (exceptionObj.getErrorCode() == OCFM_ERR_SUCCESS)
	{
		GetSubIndexAttributes(nodeId, CN, (char*) "1F98", (char*) "03",
				DEFAULTVALUE, defaultValueCN);
	}
	else
	{
		delete[] defaultValueCN;
		return retval;
	}

	if (!(NULL == presTimeOutVal || strcmp(presTimeOutVal, "") == 0))
	{
		if (!(NULL == defaultValueCN || strcmp(defaultValueCN, "") == 0))
		{
			INT32 tempDefaultVal = GetDecimalValue(defaultValueCN);
			INT32 iCheckValue = GetDecimalValue(presTimeOutVal);
			if (iCheckValue >= tempDefaultVal)
			{
				retval = true;
			}
			else
			{
				retval = false;
			}
		}
		else
		{
			retval = false;
		}
	}
	else
	{
		retval = false;
	}

	delete[] defaultValueCN;
	return retval;
}

void CopyOldNodeIdAssignmentObject(Node* nodeObj, INT32 oldNodeId)
{
	if (NULL == nodeObj)
	{
		return;
	}
	CopyOldNodeIdAssignmentObjectSubindex(nodeObj, oldNodeId, (char*) "1F81");
	CopyOldNodeIdAssignmentObjectSubindex(nodeObj, oldNodeId, (char*) "1F92");
	CopyOldNodeIdAssignmentObjectSubindex(nodeObj, oldNodeId, (char*) "1F9B");

}

void CopyOldNodeIdAssignmentObjectSubindex(Node* nodeObj, INT32 oldNodeId,
		char* indexId)
{
	if ((NULL == indexId) || (NULL == nodeObj))
	{
		return;
	}
	INT32 nodeId;
	ocfmRetCode exceptionObj;
	INT32 subIndexPos = 0;
	INT32 indexPos = 0;
	char* tempNodeIdCN = new char[10];
	char* tempOldNodeIdCN = new char[10];
	char* tempOldActualValue = NULL;

	nodeId = nodeObj->GetNodeId();
	tempNodeIdCN = IntToAscii(nodeId, tempNodeIdCN, 16);
	tempNodeIdCN = PadLeft(tempNodeIdCN, '0', 2);
	tempOldNodeIdCN = IntToAscii(oldNodeId, tempOldNodeIdCN, 16);
	tempOldNodeIdCN = PadLeft(tempOldNodeIdCN, '0', 2);

	IndexCollection* indexCollObj = NULL;
	indexCollObj = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(MN,
			MN_NODEID)->GetIndexCollection();
	Index *indexObj;
	SubIndex* sidxObj;

	exceptionObj = IfSubIndexExists(MN_NODEID, MN, indexId, tempOldNodeIdCN,
			&subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS == exceptionObj.getErrorCode())
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		sidxObj = indexObj->GetSubIndexbyIndexValue(tempOldNodeIdCN);
		if (NULL != sidxObj)
		{
			bool tempOldFlg = false;
			if (NULL != (char*) sidxObj->GetActualValue())
			{
				tempOldActualValue = new char[strlen(
						(char*) sidxObj->GetActualValue()) + STR_ALLOC_BUFFER];
				strcpy((char*) tempOldActualValue,
						(char*) sidxObj->GetActualValue());
				tempOldFlg = sidxObj->GetFlagIfIncludedCdc();
			}
			exceptionObj = IfSubIndexExists(MN_NODEID, MN, indexId, tempNodeIdCN,
					&subIndexPos, &indexPos);
			if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
			{
				return;
			}

			SubIndex* sidxObj1 = NULL;
			sidxObj1 = indexObj->GetSubIndexbyIndexValue(tempNodeIdCN);
			if (NULL != sidxObj1)
			{
				sidxObj1->SetActualValue((char*) "");
			}

			if ((NULL != sidxObj1) && (NULL != tempOldActualValue))
			{
				//copy the actual value to here
				sidxObj1->SetActualValue(tempOldActualValue);
				sidxObj1->SetFlagIfIncludedCdc(tempOldFlg);
			}
			// reset the actual value of the old node id subindex
			SetSubIndexAttribute(MN_NODEID, MN, indexId, tempOldNodeIdCN,
					ACTUALVALUE, (char*) "");
		}
	}
	delete[] tempNodeIdCN;
	delete[] tempOldNodeIdCN;
	delete[] tempOldActualValue;
}

bool IsDefaultActualNotEqual(BaseIndex* baseIndexObj)
{
	if (NULL == baseIndexObj || NULL == baseIndexObj->GetActualValue())
		return false;

	if (NULL == baseIndexObj->GetDefaultValue())
		return true;
	bool retValue;
	DataType dtObj = baseIndexObj->GetDataType();
	if (dtObj.dataTypeName != NULL)
	{
		if (!CheckIfStringDatatypes(dtObj.dataTypeName))
		{
			INT32 actualValue = 0;
			INT32 defaultValue = 0;
			defaultValue = GetDecimalValue(
					(char*) baseIndexObj->GetDefaultValue());
			actualValue = GetDecimalValue(
					(char*) baseIndexObj->GetActualValue());
			if (actualValue == defaultValue)
				retValue = false;
			else
				retValue = true;
		}
		else
		{
			if (0
					== strcmp(baseIndexObj->GetDefaultValue(),
							baseIndexObj->GetActualValue()))
				retValue = false;
			else
				retValue = true;
		}
	}
	else
	{
		if (0
				== strcmp(baseIndexObj->GetDefaultValue(),
						baseIndexObj->GetActualValue()))
			retValue = false;
		else
			retValue = true;
	}
	return retValue;
}

bool ReactivateMappingPDO(IndexCollection* indexCollObj, Index* indexObj)
{
	if ((NULL == indexCollObj) || (NULL == indexObj))
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexCollObj', 'indexObj'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	if (false == CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
	{
		return false;
	}

	SubIndex *sidxObj = NULL;
	sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
	if ((NULL != sidxObj) && (NULL != sidxObj->GetActualValue())
			&& (0 != strcmp(sidxObj->GetActualValue(), "")))
	{
		if (false == CheckIfValueZero((char*) sidxObj->GetActualValue()))
		{
			for (INT32 sidxLC = 0; sidxLC < indexObj->GetNumberofSubIndexes();
					sidxLC++)
			{
				if (indexObj->GetSubIndexByPosition(sidxLC)->GetActualValue() != NULL
						&& true
								== indexObj->GetSubIndexByPosition(sidxLC)->GetFlagIfIncludedCdc()
						&& true
								== IsDefaultActualNotEqual(
										indexObj->GetSubIndexByPosition(sidxLC)))
				{
					if (0
							== strcmp(
									indexObj->GetSubIndexByPosition(sidxLC)->GetIndexValue(),
									"00"))
					{
						return true;
						//continue;
					}
					else //other than 00'th subindex
					{
						if (true
								== IsDefaultActualNotEqual(
										indexObj->GetSubIndexByPosition(sidxLC)))
						{
							return true;
						}
						else
						{
							continue;
						}
					}
				}
			}
		}
		else
		{
			if (false == IsDefaultActualNotEqual(sidxObj))
			{
				return false;
			}

		}
	}

	char commIndexId[5];
	strcpy(commIndexId, (char*) indexObj->GetIndexValue());

	if (strncmp((char*) indexObj->GetIndexValue(), "1A", 2) == 0)
	{
		commIndexId[1] = '8';
	}
	else if (strncmp((char*) indexObj->GetIndexValue(), "16", 2) == 0)
	{
		commIndexId[1] = '4';
	}
	else
	{
		return false;
	}

	Index *commIndexObj = NULL;
	commIndexObj = indexCollObj->GetIndexbyIndexValue(commIndexId);
	if (NULL == commIndexObj)
	{
		return false;
	}

	sidxObj = commIndexObj->GetSubIndexbyIndexValue((char*) "00");
	if ((NULL != sidxObj) && (NULL != sidxObj->GetActualValue())
			&& (0 != strcmp(sidxObj->GetActualValue(), "")))
	{
		if (CheckIfValueZero((char*) sidxObj->GetActualValue()))
			return false;
	}

	for (INT32 sidxLC = 0; sidxLC < commIndexObj->GetNumberofSubIndexes();
			sidxLC++)
	{
		if (commIndexObj->GetSubIndexByPosition(sidxLC)->GetActualValue() != NULL
				&& true
						== commIndexObj->GetSubIndexByPosition(sidxLC)->GetFlagIfIncludedCdc()
				&& true
						== CheckAccessTypeForInclude(
								(char*) commIndexObj->GetSubIndexByPosition(sidxLC)->GetAccessType())
				&& true
						== IsDefaultActualNotEqual(
								commIndexObj->GetSubIndexByPosition(sidxLC)))
		{
			return true;
		}
	}

	return false;
}

//TODO: This function should be made unused unless it is necessary. Because some where the return value is not used.
//INT32* ArrangeNodeIDbyStation(void)
//{
//	NodeCollection* nodeCollObj = NULL;
//	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//	Node* nodeObj = NULL;
//
//	/* Check RPDO Mapped objects*/
//	INT32 nodesCNCount = 0;
//
//	nodesCNCount = nodeCollObj->GetCNNodesCount();
//
//	if (nodesCNCount == 0)
//	{
//		exit(0);
//	}
//
//	INT32 *nodeIdColl = new INT32[nodesCNCount+1];
//	INT32 arrangedNodeIdCount = 0;
//	INT32 *arrangedNodeIdColl = new INT32[nodesCNCount+1];
//	StationType *stationTypeColl = new StationType[nodesCNCount+1];
//	INT32 nodesCount = nodeCollObj->GetNumberOfNodes();
//	INT32 loopCount = 0;
//	INT32 tempVal = 0;
//	for (loopCount = 0; loopCount < nodesCount; loopCount++)
//	{
//		nodeObj = nodeCollObj->GetNodebyColIndex(loopCount);
//		if (MN == nodeObj->GetNodeType())
//		{
//			continue;
//		}
//		nodeIdColl[tempVal] = nodeObj->GetNodeId();
//		stationTypeColl[tempVal] = nodeObj->GetStationType();
//		tempVal++;
//	}
//	//get the number of poll response station and other station
//	INT32 iChainStnCnt = 0;
//	INT32 iOtherStnCnt = 0;
//	for (loopCount = 0; loopCount < nodesCount; loopCount++)
//	{
//		if (CHAINED == stationTypeColl[loopCount])
//			iChainStnCnt++;
//	}
//	iOtherStnCnt = nodesCount - iChainStnCnt;
//
//	//create arrays to store nodeid for chained station and other station
//	if (0 != iChainStnCnt)
//	{
//		INT32 *piChainStnColl = new INT32[iChainStnCnt];
//		INT32 iChainStnLoopCnt = 0;
//		for (loopCount = 0; loopCount < nodesCount; loopCount++)
//		{
//			if (CHAINED == stationTypeColl[loopCount])
//			{
//				piChainStnColl[iChainStnLoopCnt] = nodeIdColl[loopCount];
//				iChainStnLoopCnt++;
//			}
//		}
//		//sort by station no
//		SortNodeID(piChainStnColl, iChainStnCnt);
//
//		//copy the poll response staion sorted in asscending order
//		for (loopCount = 0; loopCount < iChainStnCnt; loopCount++)
//		{
//			arrangedNodeIdColl[arrangedNodeIdCount] = piChainStnColl[loopCount];
//			arrangedNodeIdCount++;
//		}
//
//		delete[] piChainStnColl;
//	}
//
//	if (0 != iOtherStnCnt)
//	{
//		INT32 *piOtherStnColl = new INT32[iOtherStnCnt+1];
//		INT32 iOtherStnLoopCnt = 0;
//		for (loopCount = 0; loopCount < nodesCount; loopCount++)
//		{
//			if (CHAINED != stationTypeColl[loopCount])
//			{
//				piOtherStnColl[iOtherStnLoopCnt] = nodeIdColl[loopCount];
//				iOtherStnLoopCnt++;
//			}
//		}
//		//copy the other station after the poll response staion
//		for (loopCount = 0; loopCount < iOtherStnCnt; loopCount++)
//		{
//			arrangedNodeIdColl[arrangedNodeIdCount] = piOtherStnColl[loopCount];
//			arrangedNodeIdCount++;
//		}
//		delete[] piOtherStnColl;
//	}
//	delete[] nodeIdColl;
//	delete[] stationTypeColl;
//
//	return arrangedNodeIdColl;
//}

void SortNodeID(INT32 *nodeIDColl, INT32 collectionSize)
{

	try
	{
		if (!nodeIDColl)
		{
			boost::format formatter(kMsgNullArgument);
			formatter % "'nodeIDColl'";
			ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
			exceptionObj.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw exceptionObj;
		}

		INT32 tempVal = 0;
		for (INT32 collLC = 0; collLC < collectionSize; collLC++)
		{
			for (INT32 iSortCount = collLC + 1; iSortCount <= collLC;
					iSortCount++)
			{
				if (nodeIDColl[collLC] > nodeIDColl[iSortCount])
				{
					tempVal = nodeIDColl[collLC];
					nodeIDColl[collLC] = nodeIDColl[iSortCount];
					nodeIDColl[iSortCount] = tempVal;
				}
			}
		}
	} catch (ocfmRetCode& ex)
	{
		LOG_FATAL() << ex.getErrorCode();
		throw ex;
	}
}

//TODO: Change the function name as IsPresEnabledCN. Because it determines the CN has the functionality to receive the Pres or not.
bool IsPresMN()
{
	NodeCollection *nodeCollObj = NULL;
	bool isPres = false;
	INT32 iNodesCount;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	iNodesCount = nodeCollObj->GetNumberOfNodes();

	for (INT32 nodeLC = 0; nodeLC < iNodesCount; nodeLC++)
	{
		Node *nodeObj = NULL;
		nodeObj = nodeCollObj->GetNodebyColIndex(nodeLC);
		if (MN == nodeObj->GetNodeType())
		{
			continue;
		}
		//No need to check for preschaining for normal and multiplexed stations
		if (CHAINED == nodeObj->GetStationType())
		{
			//return true;
			//isPres = true;
			IndexCollection* cnIdxColl = NULL;
			cnIdxColl = nodeObj->GetIndexCollection();
			/*
			1. CN is chained and mapping is enabled, and Targetnodeid is F0, then MN can send Pres
			*/
			for (INT32 indexLC = 0; indexLC < cnIdxColl->GetNumberofIndexes(); indexLC++)
			{
				Index* mappIdxObj = NULL;
				mappIdxObj = cnIdxColl->GetIndexByPosition(indexLC);
				if(mappIdxObj != NULL)
				{
					PDOType pdoType = mappIdxObj->GetPDOType();
					if(pdoType == PDO_RPDO)
					{
						if (0 == strncmp(mappIdxObj->GetIndexValue(), "16", 2))
						{
							SubIndex *mappSidxObj = NULL;
							mappSidxObj = mappIdxObj->GetSubIndexbyIndexValue((char *) "00");
							if (NULL != mappSidxObj)
							{
								if ((NULL != mappSidxObj->GetActualValue()))
								{
									if((0 != strcmp(mappSidxObj->GetActualValue(), ""))
										|| (true != CheckIfValueZero((char*) mappSidxObj->GetActualValue()))
									)
									{
										Index *commIndexObj = NULL;

										char *indexId = new char[SUBINDEX_LEN];
										SubString(indexId, mappIdxObj->GetIndexValue(), 2, 2);
										char *commIdxId = new char[INDEX_LEN + 1];
										strcpy(commIdxId, (char *) "14");
										strcat(commIdxId, indexId);
										commIndexObj = cnIdxColl->GetIndexbyIndexValue(commIdxId);
										delete[] indexId;
										delete[] commIdxId;

										if (NULL != commIndexObj)
										{
											SubIndex *sidxObj = NULL;
											sidxObj = commIndexObj->GetSubIndexbyIndexValue((char *) "01");
											if (NULL != sidxObj)
											{
												if ((NULL != sidxObj->GetActualValue())
													&& (0 != strcmp(sidxObj->GetActualValue(), "")))
												{
													INT32 rpdoMappedNodeId = 0;
													rpdoMappedNodeId = GetDecimalValue((char *) sidxObj->GetActualValue());
													if (MN_NODEID == rpdoMappedNodeId)
													{
														isPres = true;
														break;
													}
												}
											}
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
	return isPres;
}

void SetCNLossObjects(INT32 nodeId, NodeType nodeType)
{
	if (nodeType != CN)
	{
		return;
	}

	Node *nodeObj = NULL;
	NodeCollection *nodeCollObj = NULL;
	IndexCollection* indexCollObj = NULL;
	Index *indexObj = NULL;
	SubIndex* sidxObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
	indexCollObj = nodeObj->GetIndexCollection();

	//loss of SoC
	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1C0B");
	if (NULL != indexObj)
	{
		sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "03");
		if (NULL != sidxObj)
		{
			if ((sidxObj->GetActualValue() == NULL)
					|| (strcmp(sidxObj->GetActualValue(), "") == 0))
			{
				sidxObj->SetActualValue((char*) "0x50");
				sidxObj->SetFlagIfIncludedCdc(true);
				indexObj->SetFlagIfIncludedCdc(true);
			}
			else if (sidxObj->GetActualValue() != NULL
				&& IsDefaultActualNotEqual(sidxObj))
			{
				sidxObj->SetFlagIfIncludedCdc(true);
				indexObj->SetFlagIfIncludedCdc(true);
			}
		}
	}

	//loss of SoA
	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1C0C");
	if (NULL != indexObj)
	{
		sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "03");
		if (NULL != sidxObj)
		{
			if ((sidxObj->GetActualValue() == NULL)
					|| (strcmp(sidxObj->GetActualValue(), "") == 0))
			{
				sidxObj->SetActualValue((char*) "0x50");
				sidxObj->SetFlagIfIncludedCdc(true);
				indexObj->SetFlagIfIncludedCdc(true);
			}
			else if(sidxObj->GetActualValue() != NULL
				&& IsDefaultActualNotEqual(sidxObj))
			{
				sidxObj->SetFlagIfIncludedCdc(true);
				indexObj->SetFlagIfIncludedCdc(true);
			}
		}
	}

	//loss of PReq
	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1C0D");
	if (NULL != indexObj)
	{
		sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "03");
		if (NULL != sidxObj)
		{
			if ((sidxObj->GetActualValue() == NULL)
					|| (strcmp(sidxObj->GetActualValue(), "") == 0))
			{
				sidxObj->SetActualValue((char*) "0x50");
				sidxObj->SetFlagIfIncludedCdc(true);
				indexObj->SetFlagIfIncludedCdc(true);
			}
			else if(sidxObj->GetActualValue() != NULL
				&& IsDefaultActualNotEqual(sidxObj))
			{
				sidxObj->SetFlagIfIncludedCdc(true);
				indexObj->SetFlagIfIncludedCdc(true);
			}
		}
	}
}

void SetBuildTime()
{
	time(&buildTimeGlobal.rawtime);
	buildTimeGlobal.timeinfo = localtime(&buildTimeGlobal.rawtime);
	strftime(buildTimeGlobal.buffer, BUILDTIME_BUF_LEN, "%d-%b-%Y %H:%M:%S",
			buildTimeGlobal.timeinfo);
}

const char* GetBuildTime()
{
	return (const char*) &buildTimeGlobal.buffer;
}
