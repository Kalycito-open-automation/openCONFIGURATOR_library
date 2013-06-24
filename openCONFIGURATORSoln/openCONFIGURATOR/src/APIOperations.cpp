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

#include <iostream>
#include <fstream>
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
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Exception.h"
#include "../Include/Internal.h"
#include "../Include/ObjectDictionary.h"
#include "../Include/ProcessImage.h"
#include "../Include/Declarations.h"
#include "../Include/Validation.h"
#include "../Include/APIOperations.h"

/****************************************************************************/
/* Defines */

#define MY_ENCODING "UTF-8"
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

static void SetDefaultIndexAttributes(char* indexId, Index* indexObj,
		Index* dictIndexObj)
{
	if ((NULL == dictIndexObj) || (NULL == indexObj))
	{
#if defined DEBUG
		cout << "__FUNC__ FAILED" << endl;
#endif
		return;
	}

	if (NULL == indexId)
	{
#if defined DEBUG
		cout << "__FUNC__ FAILED - pIndexID empty" << endl;
#endif
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
	char* tempStr = NULL;
	if (NULL != dictIndexObj->GetPDOMapping())
	{
		tempStr = new char[strlen(dictIndexObj->GetPDOMapping())
				+ STR_ALLOC_BUFFER];
		strcpy(tempStr, dictIndexObj->GetPDOMapping());
		indexObj->SetPDOMapping(tempStr);
	}
	else
	{
		tempStr = new char[2 + STR_ALLOC_BUFFER];
		strcpy(tempStr, (char*) "");
		indexObj->SetPDOMapping(tempStr);
	}
	delete[] tempStr;

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

static void SetDefaultSubIndexAttributes(char* subIndexId, SubIndex* sidxObj,
		SubIndex* dictSidxObj)
{
	if ((NULL == dictSidxObj) || (NULL == sidxObj))
	{
#if defined DEBUG
		cout << "__FUNC__ FAILED" << endl;
#endif
		return;
	}

	if (NULL == subIndexId)
	{
#if defined DEBUG
		cout << "__FUNC__ FAILED - pSubIndexID NULL" << endl;
#endif
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

ocfmRetCode CreateNode(INT32 nodeId, NodeType nodeType, char* nodeName)
{
	ocfmRetCode errCodeObj;
	ocfmException objException;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
#if defined DEBUG
		cout << __FUNCTION__ << ": " << nodeType << endl;
#endif

		if (MN == nodeType)
		{
			if (!objectDictLoadedGlobal)
			{
#if defined(_WIN32) && defined(_MSC_VER)
				LoadObjectDictionary((char*) "od.xml");
#else
				{
					char* tmpCmdBuffer = new char[LINUX_INSTALL_DIR_LEN];
					sprintf(tmpCmdBuffer, "%s/od.xml", LINUX_INSTALL_DIR);
					LoadObjectDictionary(tmpCmdBuffer);
					delete[] tmpCmdBuffer;
				}
#endif
				objectDictLoadedGlobal = true;
			}
			cycleNumberGlobal = 0;
		}
		else if (CN == nodeType)
		{
			INT32 nodePos = 0;
			bool nodeExistFlag = false;
			errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos,
					nodeExistFlag);
			if ((errCodeObj.code == OCFM_ERR_SUCCESS)
					&& (nodeExistFlag == true))
			{
				objException.OCFMException(OCFM_ERR_NODE_ALREADY_EXISTS);
				throw objException;
			}
			else
			{
#if defined DEBUG
				cout << "Node id:" << nodeId << " not found. ErrCode: " << OCFM_ERR_NODEID_NOT_FOUND << endl;
#endif
			}
		}
		else
		{
			//Nothing other than MN & CN
		}
		Node nodeObj;
		nodeObj.SetNodeId(nodeId);
		nodeObj.SetNodeType(nodeType);
		if (NULL == nodeName)
		{
#if defined DEBUG
			cout << "Err_setting_node_name" << endl;
			nodeObj.SetNodeName((char*) "ERR_NAME");
#endif
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			return (errCodeObj);
		}
		else
		{
			nodeObj.SetNodeName(nodeName);
		}

		nodeObj.CreateIndexCollection();
		nodeObj.CreateDataTypeCollection();
		nodeObj.CreateApplicationProcess();
		nodeObj.CreateNetworkManagement();

		NodeCollection* nodeCollObj = NULL;
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			objException.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		nodeCollObj->AddNode(nodeObj);

	} catch (ocfmException* ex)
	{
		errCodeObj = ex->_ocfmRetCode;
		return (errCodeObj);
	}
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return (errCodeObj);
}

ocfmRetCode NewProjectNode(INT32 nodeId, NodeType nodeType, char* nodeName,
		char* importXmlFile)
{
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		//Creates the node with Nodeid & nodeName
		if ((NULL == nodeName))
		{
#if defined DEBUG
			cout << "varNodeName Null" << endl;
#endif
			return (errCodeObj);
		}
		errCodeObj = CreateNode(nodeId, nodeType, nodeName);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			return (errCodeObj);
		}
		//Import the xdd/xdc for the Node created
		errCodeObj.code = OCFM_ERR_UNKNOWN;
		if (NULL == importXmlFile)
		{
#if defined DEBUG
			cout << "varImportXmlFile Null" << endl;
#endif
			return (errCodeObj);
		}
		errCodeObj = ImportXML(importXmlFile, nodeId, nodeType);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			return (errCodeObj);
		}

		errCodeObj.code = OCFM_ERR_UNKNOWN;
		if (MN == nodeType)
		{
			//set the loss of SoC tolerance to 50 ms
			INT32 idxPos = 0;
			errCodeObj = IfIndexExists(nodeId, nodeType, (char *) "1C14",
					&idxPos);
			if (OCFM_ERR_SUCCESS == errCodeObj.code)
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

	} catch (ocfmException* ex)
	{
		errCodeObj = ex->_ocfmRetCode;
		return (errCodeObj);
	}
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return (errCodeObj);
}

ocfmRetCode DeleteNode(INT32 nodeId, NodeType nodeType)
{
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	INT32 nodePos = 0;
	bool nodeExist = false;

	errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, nodeExist);

	if (nodeExist != true)
	{
		/* Function didnt throw any exception but Node doesnt exist */
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
			errCodeObj.code = OCFM_ERR_NODEID_NOT_FOUND;
		}
		else
		{ /* Function threw exception*/
			// Node Doesn't Exist
		}
		return errCodeObj;
	}

	Node nodeObj;
	NodeCollection* nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	if (NULL == nodeCollObj)
	{
		errCodeObj.code = OCFM_ERR_NO_NODES_FOUND;
		return errCodeObj;
	}
	nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);

	if (CN == nodeObj.GetNodeType())
	{
		INT32 indexPos = 0;
		INT32 subIndexPos = 0;
		errCodeObj.code = OCFM_ERR_UNKNOWN;
		errCodeObj.errorString = NULL;

		if (MULTIPLEXED == nodeObj.GetStationType())
		{
			//Deleted MN's 1F9B and Subindex = old node id
			if (NULL != nodeObj.GetForcedCycleValue())
			{
				CheckAndReAssignMultiplex(nodeObj.GetNodeId(),
						nodeObj.GetForcedCycleValue());
			}
			else
			{
				//ForcedCycle NULL
			}
		}
		else
		{
			//nothing to do if not MULTIPLEXED
		}

		char* sidxStr = new char[SUBINDEX_LEN];
		sidxStr = IntToAscii(nodeObj.GetNodeId(), sidxStr, 16);
		sidxStr = PadLeft(sidxStr, '0', 2);

		errCodeObj = IfSubIndexExists(MN_NODEID, MN,
				(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, sidxStr, &subIndexPos,
				&indexPos);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
			errCodeObj = SetSubIndexAttribute(MN_NODEID, MN,
					(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, sidxStr, ACTUALVALUE,
					(char*) "");
			if (errCodeObj.code != OCFM_ERR_SUCCESS)
			{
				cout << "1F9B SetSubIndexAttribute Failed" << endl;
			}
		}
		else
		{
			//1F9B->subIdx(CN Nodeid) doesnot exists
		}
		errCodeObj = IfSubIndexExists(MN_NODEID, MN,
				(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, sidxStr, &subIndexPos,
				&indexPos);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
			errCodeObj = SetSubIndexAttribute(MN_NODEID, MN,
					(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, sidxStr,
					ACTUALVALUE, (char*) "");
			if (errCodeObj.code != OCFM_ERR_SUCCESS)
			{
				cout << "1F92 SetSubIndexAttribute Failed" << endl;
			}
		}
		else
		{
			//1F92->subIdx(CN Nodeid) doesnot exists
		}
		delete[] sidxStr;

		//Delete the auto generated pdo indexes also.
		Node *nodeObjMN = NULL;
		IndexCollection *idxCollObjMN = NULL;
		nodeObjMN = nodeCollObj->GetNodePtr(MN, MN_NODEID);
		idxCollObjMN = nodeObjMN->GetIndexCollection();

		INT32 totalIndexMN = idxCollObjMN->GetNumberofIndexes();
#if defined DEBUG
		cout<< "Deleting a Node: Total MN index" <<totalIndexMN<<endl;
#endif

		for (INT32 idxLC = 0; idxLC < totalIndexMN; idxLC++)
		{
			Index *idxCommObjMN = NULL;
			char* subStr1 = new char[SUBINDEX_LEN];
			char* subStr2 = new char[SUBINDEX_LEN];
			bool idx14Present = false;
			bool idx18Present = false;

			idxCommObjMN = idxCollObjMN->GetIndex(idxLC);
			subStr1 = SubString((char*) idxCommObjMN->GetIndexValue(), 0, 2);
			subStr2 = SubString((char*) idxCommObjMN->GetIndexValue(), 2, 2);
#if defined DEBUG
			cout<<"Entry Processing: "<<subStr1<<subStr2<<endl;
#endif

			if ((0 == strcmp(subStr1, "14")))
			{
				idx14Present = true;
			}
			else if ((0 == strcmp(subStr1, "18")))
			{
				idx18Present = true;
			}

			if ((idx14Present == true) || (idx18Present == true))
			{
				SubIndex *sidxObj = NULL;
				char* sidxActValue = new char[SUBINDEX_LEN];
				sidxObj = idxCommObjMN->GetSubIndexbyIndexValue((char*) "01");
				if (NULL != sidxObj)
				{
					INT32 nodeIdTemp;
					sidxActValue = (char*) sidxObj->GetActualValue(); //NULL
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
							if (errCodeObj.code != OCFM_ERR_SUCCESS)
							{
								cout << "00th subindex cannot be added" << endl;
							}
							sidxObj->SetActualValue((char*) "00");
						}
#if defined DEBUG
						cout<<"subidx Collection deleted"<<endl;
#endif
						delete[] idxIdTxobjMN;
					}
				}
				else
				{
					//00th subindex in the communication index is not present
				}
				//delete[] varActValue;
			}
			delete[] subStr1;
			delete[] subStr2;
		}
	}

	nodeCollObj->DeleteNode(nodePos);

#if defined DEBUG
	cout << "Deleted " << nodeType << ". NodeID:" << nodeId << endl;
#endif

	//stErrorInfo.code = OCFM_ERR_SUCCESS;
	return errCodeObj;
}

ocfmRetCode DeleteIndex(INT32 nodeId, NodeType nodeType, char* indexId)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		INT32 idxPos = 0;

		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &idxPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node* nodeObj = NULL;
		IndexCollection* indexCollObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}
		//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID);
		//pobjIndexCollection = objNode.getIndexCollection();

		nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
		if (NULL == nodeObj)
		{
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}

		indexCollObj = nodeObj->GetIndexCollection();
		if (NULL == indexCollObj)
		{
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}

		indexCollObj->DeleteIndex(idxPos);
#if defined DEBUG
		cout << "Deleted " << nodeType << ". NodeID:" << nodeId
		<< ". IndexID:" << indexId << endl;
#endif

		errCodeObj.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode DeleteSubIndex(INT32 nodeId, NodeType nodeType, char* indexId,
		char* subIndexID)
{
	INT32 sidxPos = 0;
	INT32 idxPos = 0;
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if ((NULL == indexId) || (NULL == subIndexID))
		{
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, subIndexID,
				&sidxPos, &idxPos);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		NodeCollection* nodeCollObj = NULL;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* subIndexObj = NULL;
		Node nodeObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (NULL == indexCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndex(idxPos);
		if (NULL == indexObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		subIndexObj = indexObj->GetSubIndex(sidxPos);
		if (NULL != subIndexObj)
		{
			//delete the sub-index and then updated the 00th entry
			indexObj->DeleteSubIndex(sidxPos);

			UpdateNumberOfEnteriesSIdx(indexObj, nodeType);
			errCodeObj.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			errCodeObj.code = OCFM_ERR_SUBINDEXID_NOT_FOUND;
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode AddSubIndex(INT32 nodeId, NodeType nodeType, char* indexId,
		char* subIndexId)
{
	INT32 sidxPos = 0;
	INT32 idxPos = 0;
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if ((NULL == indexId) || (NULL == subIndexId))
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw exceptionObj;
		}

		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, subIndexId,
				&sidxPos, &idxPos);
		if (OCFM_ERR_SUCCESS == errCodeObj.code)
		{
			exceptionObj.OCFMException(OCFM_ERR_SUBINDEX_ALREADY_EXISTS);
			throw exceptionObj;
		}

		if ((OCFM_ERR_NO_SUBINDEXS_FOUND == errCodeObj.code)
				|| (OCFM_ERR_SUBINDEXID_NOT_FOUND == errCodeObj.code))
		{
			NodeCollection* nodeCollObj = NULL;
			//Node objNode;
			Node *nodeObj = NULL;
			IndexCollection* idxCollObj = NULL;
			Index* pobjIndex = NULL;

			//                        CIndex objIndex;

			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			if (NULL == nodeCollObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
				throw exceptionObj;
			}

			//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID);
			nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);

			//pobjIndexCollection = objNode.getIndexCollection();
			idxCollObj = nodeObj->GetIndexCollection();
			if (NULL == idxCollObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				throw exceptionObj;
			}

			pobjIndex = idxCollObj->GetIndex(idxPos);
			if (NULL == pobjIndex)
			{
				exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
				throw exceptionObj;
			}

			/* Get the SubIndex from ObjectDictionary*/
			ObjectDictionary* dictCollObj = NULL;
			dictCollObj = ObjectDictionary::GetObjDictPtr();
			if (NULL == dictCollObj)
			{
				errCodeObj.code = OCFM_ERR_UNKNOWN;
				exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
				throw exceptionObj;
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
					sidxObj->SetObjectType((char*) "VAR");
					sidxObj->SetFlagIfIncludedCdc(TRUE);
					pobjIndex->AddSubIndex(*sidxObj);
				}
			}
			else
			{
				exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
				throw exceptionObj;
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

			errCodeObj.code = OCFM_ERR_SUCCESS;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode AddSubobject(INT32 nodeId, NodeType nodeType, char* indexId)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if (NULL == indexId)
		{
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}
#if defined DEBUG
		cout<<"Adding 00 subindex. IndexId: "<<indexId<<endl;
#endif
		errCodeObj = AddSubIndex(nodeId, nodeType, indexId, (char*) "00");
		if ((OCFM_ERR_SUCCESS == errCodeObj.code)
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
			if (OCFM_ERR_SUCCESS != errCodeObj.code)
			{
				exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
				throw exceptionObj;
			}

			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			if (NULL == nodeCollObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
				throw exceptionObj;
			}

			nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

			indexCollObj = nodeObj.GetIndexCollection();
			if (NULL == indexCollObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				throw exceptionObj;
			}

			indexObj = indexCollObj->GetIndex(indexPos);
			if (NULL == indexObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
				throw exceptionObj;
			}

			sidxObj = indexObj->GetSubIndex(sidxPos);
			if (NULL == sidxObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
				throw exceptionObj;
			}

			sidxObj->SetName((char*) "NumberOfEntries");
			sidxObj->SetObjectType((char*) "VAR");
			char* dtName = new char[15];

			strcpy(dtName, (const char*) "UNSIGNED8");
			if (true == (CheckIfDataTypeByNameExists(dtName, nodeId)))
			{
				sidxObj->SetDataTypeName(dtName, nodeId);
			}
			else
			{
				delete[] dtName;
				exceptionObj.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
				throw exceptionObj;
			}
			delete[] dtName;
		}
		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode AddIndex(INT32 nodeId, NodeType nodeType, char* indexId)
{
	ocfmRetCode errCodeObj;
	INT32 indexPos = 0;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if (NULL == indexId)
		{
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (OCFM_ERR_SUCCESS == errCodeObj.code)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEX_ALREADY_EXISTS);
			throw exceptionObj;
		}

		if ((OCFM_ERR_NO_INDEX_FOUND == errCodeObj.code)
				|| (OCFM_ERR_INDEXID_NOT_FOUND == errCodeObj.code))
		{
			NodeCollection* nodeCollObj = NULL;
			Node nodeObj;
			IndexCollection* indexCollObj = NULL;
			ObjectDictionary* dictObj = NULL;
			Index* dictIndexObj = NULL;

			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			if (NULL == nodeCollObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
				throw exceptionObj;
			}
			nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
			indexCollObj = nodeObj.GetIndexCollection();
			if (NULL == indexCollObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				throw exceptionObj;
			}

			/* Get the Index from ObjectDictionary*/
			dictObj = ObjectDictionary::GetObjDictPtr();
			if (NULL == dictObj)
			{
				errCodeObj.code = OCFM_ERR_UNKNOWN;
				exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
				throw exceptionObj;
			}
			//Validate for TPDO channels for a CN
			if ((CN == nodeObj.GetNodeType())
					&& ((0 == strncmp(indexId, "1A", 2))
							|| (0 == strncmp(indexId, "1a", 2))))
			{
				INT32 tpdoCount = 0;
				INT32 rpdoCount = 0;
				nodeObj.getPDOIndexCollection(&rpdoCount, &tpdoCount);
#if defined DEBUG
				cout<<"tpdoCount:"<<tpdoCount<<" rpdoCount:"<<rpdoCount<<endl;
#endif
				//Allowed to add TPDO only if the node has 0 TPDO's(1Axx)
				if (tpdoCount > 0)
				{
					exceptionObj.OCFMException(
							OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					char customErrStr[200] =
					{ 0 };
					sprintf(customErrStr,
							"Node id: %d (CN) cannot have more than one TPDO Channel",
							nodeObj.GetNodeId());
					CopyCustomErrorString(&(exceptionObj._ocfmRetCode),
							customErrStr);
					throw exceptionObj;
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
					objSIdx = dictIndexObj->GetSubIndex(idxLC);
					//objSIdx->SetNodeID(nodeId);
					if (NULL != objSIdx)
					{
						indexObj.AddSubIndex(*objSIdx);
					}
				}
				char *newIndexName = new char[50];
				char indexSubStr[4];
				strcpy(indexSubStr, SubString(indexId, 2, 4));
				newIndexName = dictObj->GetIndexName((char*)indexSubStr,(char*) indexObj.GetName());
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
				indexObj.SetFlagIfIncludedCdc(TRUE);
				indexCollObj->AddIndex(indexObj);
			}
			else
			{
				exceptionObj.OCFMException(OCFM_ERR_INVALID_INDEXID);
				throw exceptionObj;
			}

			errCodeObj.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			// This Part of code is never expected to happen
			exceptionObj.OCFMException(OCFM_ERR_INVALID_INDEXID);
			throw exceptionObj;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SetBasicIndexAttributes(INT32 nodeId, NodeType nodeType,
		char* indexId, char* indexValue, char* indexName, Flag includeInCDC)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if ((NULL == indexId) || (NULL == indexValue) || (NULL == indexName))
		{
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}
		INT32 indexPos = 0;
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		//Index objIndex;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			errCodeObj.code = OCFM_ERR_NO_NODES_FOUND;
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
		//objIndex.SetNodeID(nodeId);
		indexCollObj = nodeObj.GetIndexCollection();
		if (NULL == indexCollObj)
		{
			errCodeObj.code = OCFM_ERR_NO_INDEX_FOUND;
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndex(indexPos);
		if (NULL == indexObj)
		{
			errCodeObj.code = OCFM_ERR_INDEXID_NOT_FOUND;
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		indexObj->SetName(indexName);
		indexObj->SetFlagIfIncludedCdc(includeInCDC);

		if (NULL != indexValue)
		{
			if (true == indexObj->IsIndexValueValid(indexValue))
			{
				indexObj->SetActualValue(indexValue);
				errCodeObj.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				exceptionObj.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw exceptionObj;
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SetBasicSubIndexAttributes(INT32 nodeId, NodeType nodeType,
		char* indexId, char* sidxId, char* indexValue, char* indexName,
		Flag includeInCDC)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if ((NULL == indexId) || (NULL == sidxId) || (NULL == indexValue)
				|| (NULL == indexName))
		{
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}

		INT32 indexPos = 0;
		INT32 subIndexPos = 0;
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&subIndexPos, &indexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw exceptionObj;
		}

		if ((MN_NODEID == nodeId) && (MN == nodeType)
				&& (0 == strcmp(indexId, "1F92"))
				&& (0 != strcmp(sidxId, "00")))
		{
			if (false == ValidateCNPresTimeout(sidxId, indexValue))
			{
				exceptionObj.OCFMException(OCFM_ERR_LOW_CNPRESTIMEOUT);
				throw exceptionObj;
			}
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* sidxObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (NULL == indexCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndex(indexPos);
		if (NULL == indexObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		sidxObj = indexObj->GetSubIndex(subIndexPos);
		if (NULL == sidxObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		sidxObj->SetName(indexName);
		sidxObj->SetFlagIfIncludedCdc(includeInCDC);

		if (sidxObj->IsIndexValueValid(indexValue))
		{
			sidxObj->SetActualValue(indexValue);
			errCodeObj.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
			throw exceptionObj;
		}

		if ((MN_NODEID == nodeId) && (MN == nodeType)
				&& (0 == strcmp(indexId, (const char*) "1F98"))
				&& (0 == strcmp(sidxId, (const char*) "07")))
		{
			errCodeObj = RecalculateMultiplex();
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SetAllIndexAttributes(INT32 nodeId, NodeType nodeType,
		char* indexId, char* actualValue, char* indexName, char* accessType,
		char* dataTypeName, char* pdoMappingVal, char* defaultValue,
		char* highLimitVal, char* lowLimitVal, char* objectType,
		Flag includeInCDC)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	try
	{
		// || (NULL == varActualValue) || (NULL == varIndexName) || (NULL == varAccess) || (NULL == varDataTypeName) || (NULL == pdoMappingVal) || (NULL == vardefaultValue) || (NULL == varhighLimit) || (NULL == varlowLimit) || (NULL == varobjType)
		if ((NULL == indexId))
		{
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}

		INT32 indexPos;
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			errCodeObj.code = OCFM_ERR_NO_NODES_FOUND;
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (NULL == indexCollObj)
		{
			errCodeObj.code = OCFM_ERR_NO_INDEX_FOUND;
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndex(indexPos);
		if (NULL == indexObj)
		{
			errCodeObj.code = OCFM_ERR_INDEXID_NOT_FOUND;
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
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
		if (NULL != defaultValue)
		{
			indexObj->SetDefaultValue(defaultValue);
		}

		ocfmRetCode errorLimitInfo;
		errorLimitInfo.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
		if ((NULL != highLimitVal) && (NULL != lowLimitVal))
		{
			errorLimitInfo = CheckUpperAndLowerLimits(lowLimitVal,
					highLimitVal);
			if (OCFM_ERR_SUCCESS == errorLimitInfo.code)
			{
				indexObj->SetHighLimit(highLimitVal);
				indexObj->SetLowLimit(lowLimitVal);
			}
			else
			{
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
			errCodeObj = AddSubobject(nodeId, nodeType, indexId);
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
					exceptionObj.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
					throw exceptionObj;
				}
			}
		}
		if (NULL != actualValue)
		{
			if (indexObj->IsIndexValueValid(actualValue))
			{
				indexObj->SetActualValue(actualValue);
				errCodeObj.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				exceptionObj.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw exceptionObj;
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SetAllSubIndexAttributes(INT32 nodeId, NodeType nodeType,
		char* indexId, char* sidxId, char* actualValue, char* indexName,
		char* accessType, char* dataTypeName, char* pdoMappingVal,
		char* defaultValue, char* highLimitVal, char* lowLimitVal,
		char* objectType, Flag includeInCDC)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if ((NULL == indexId) || (NULL == sidxId))
		{
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}

		INT32 indexPos = 0;
		INT32 sidxPos = 0;
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&sidxPos, &indexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw exceptionObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* subIndexObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (NULL == indexCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndex(indexPos);
		if (NULL == indexObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}
		subIndexObj = indexObj->GetSubIndex(sidxPos);
		if (NULL == subIndexObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw exceptionObj;
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

		if (NULL != defaultValue)
		{
			subIndexObj->SetDefaultValue(defaultValue);
		}

		ocfmRetCode errLimitInfo;
		errLimitInfo.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
		if ((NULL != highLimitVal) && (NULL != lowLimitVal))
		{
			errLimitInfo = CheckUpperAndLowerLimits(lowLimitVal, highLimitVal);
			if (OCFM_ERR_SUCCESS == errLimitInfo.code)
			{
				subIndexObj->SetHighLimit(highLimitVal);
				subIndexObj->SetLowLimit(lowLimitVal);
			}
			else
			{
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
				errCodeObj.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				exceptionObj.OCFMException(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw exceptionObj;
			}
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
					exceptionObj.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
					throw exceptionObj;
				}
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SetSubIndexAttribute(INT32 nodeId, NodeType nodeType, char* indexId,
		char* sidxId, AttributeType attributeType, char* attributeValue)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		if ((NULL == indexId) || (NULL == sidxId))
		{
			exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
			throw exceptionObj;
		}
		INT32 sidxPos = 0;
		INT32 iIndexPos = 0;
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&sidxPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			return errCodeObj;
		}

		NodeCollection* nodeCollObj = NULL;
		Node nodeObj;
		IndexCollection* indexCollObj = NULL;
		Index* indexObj = NULL;
		SubIndex* sidxObj = NULL;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}

		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		if (NULL == indexCollObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}

		indexObj = indexCollObj->GetIndex(iIndexPos);
		if (NULL == indexObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		sidxObj = indexObj->GetSubIndex(sidxPos);
		if (NULL == sidxObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw exceptionObj;
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
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw exceptionObj;
		}

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode CheckUpperAndLowerLimits(char* lowLimitVal, char* highLimitVal)
{
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_SUCCESS;

	if ((NULL != lowLimitVal) && (NULL != highLimitVal))
	{
		if ((0 != strcmp(lowLimitVal, "")) && (0 != strcmp(highLimitVal, "")))
		{
			ULONG tempLowLimit;
			ULONG tempHighLimit;
			if (true == CheckIfHex((char*) lowLimitVal))
			{
				tempLowLimit = HexToInt(
						SubString((char*) lowLimitVal, 2,
								strlen(lowLimitVal) - 2));
			}
			else
			{
				tempLowLimit = atoi(lowLimitVal);
			}

			if (true == CheckIfHex((char*) highLimitVal))
			{
				tempHighLimit = HexToInt(
						SubString((char*) highLimitVal, 2,
								strlen(highLimitVal) - 2));
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
				errCodeObj.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
				errCodeObj.errorString = new char[150];
				INT32 iErrSprintf = 0;
				errCodeObj.errorString[0] = 0;
				iErrSprintf = sprintf(errCodeObj.errorString,
						"The lower limit(%s) is greater than upperlimit(%s)",
						lowLimitVal, highLimitVal);
				if (iErrSprintf < 0)
				{
					cout << "Sprintf Error:" << __FUNCTION__ << endl;
				}
				else
				{
					//sprintf Success
				}
				return errCodeObj;
			}
		}
	}
	return errCodeObj;
}

void EnableDisableMappingPDO(IndexCollection* indexCollObj, Index* indexObj,
		char* cdcBuffer, bool enablePDO)
{
	ocfmException exceptionObj;

	//Get the Index Value
	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw exceptionObj;
	}

	if (NULL == indexObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
		throw exceptionObj;
	}

	if (NULL == cdcBuffer)
	{
		exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
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
					noOfSubIndexes = HexToInt(
							SubString((char*) sidxObj->GetActualValue(), 2,
									strlen(sidxObj->GetActualValue()) - 2));
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

				if ((TRUE == sidxObj->GetFlagIfIncludedCdc())
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
							dataSizeStr = IntToAscii(len, dataSizeStr, 16);
							dataSizeStr = PadLeft(dataSizeStr, '0', 8);
							strcat(cdcBuffer, dataSizeStr);
							padLength = len * 2;
							isStringDt = true;
						}
						else
						{
							dataSizeStr = IntToAscii(dtObj.dataSize,
									dataSizeStr, 16);
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
			exceptionObj.OCFMException(
					OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND);
#if defined DEBUG
			cout << "No.Of Entries subindex Not found:: Index: " << indexObj->GetIndexValue() << endl;
#endif
			throw exceptionObj;
		}
	}
	else
	{
#if defined DEBUG
		cout << "INDEX: " << indexObj->GetIndexValue() << " has Zero SubIndexs"
		<< endl;
#endif
	}
}

void UpdateCNCycleTime(IndexCollection* indexCollObj, char* cycleTimeValue)
{
	ocfmException exceptionObj;
	Index* indexObj = NULL;

	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1006");
	if (NULL != indexObj)
	{
		if (NULL != cycleTimeValue)
		{
			strcpy(cycleTimeValue, ConvertToUpper(cycleTimeValue));
			indexObj->SetActualValue(cycleTimeValue);
		}
		else
		{
#if defined DEBUG
			cout << "Err_UpdateCNCycleTime" << endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1006 Not found, UpdateCNCycleTime failed" << endl;
#endif
	}

}

void UpdateCNSoCTolerance(IndexCollection* indexCollObj,
		char* socToleranceValue)
{
	ocfmException exceptionObj;
	Index* indexObj = NULL;

	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1C14");
	if (NULL != indexObj)
	{
		if (NULL != socToleranceValue)
		{
			indexObj->SetActualValue(socToleranceValue);
			indexObj->SetFlagIfIncludedCdc(TRUE);
		}
		else
		{
#if defined DEBUG
			cout << "Err_UpdateCNSoCTolerance" << endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1C14 Not found, UpdateCNSoCTolerance failed" << endl;
#endif
	}

}

void UpdateCNAsyncMTUsize(IndexCollection* indexCollObj, char* asyncMTUsize)
{
	ocfmException exceptionObj;
	Index* indexObj = NULL;

	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
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
#if defined DEBUG
				cout << "Err_UpdateCNAsyncMTUsize" << endl;
#endif
			}
		}
		else
		{
#if defined DEBUG
			cout << "Index 1F98 / 08 Not found, UpdateCNAsyncMTUsize failed"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1F98 Not found, UpdateCNAsyncMTUsize failed" << endl;
#endif
	}

}

void UpdateCNMultiPrescal(IndexCollection* indexCollObj, char* multiPrescalVal)
{
	ocfmException exceptionObj;
	Index* indexObj = NULL;

	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
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
#if defined DEBUG
				cout << "Err_UpdateCNMultiPrescal" << endl;
#endif
			}
		}
		else
		{
#if defined DEBUG
			cout << "Index 1F98 / 07 Not found, UpdateCNMultiPrescal failed"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "Index 1F98 Not found, UpdateCNMultiPrescal failed" << endl;
#endif
	}
}

//TODO: unused function
void UpdateCNNodeAssignment(Node* nodeObj)
{
	ocfmException objException;
	bool copyNodeAssignmentVal = false;

	if (NULL == nodeObj)
	{
		objException.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	copyNodeAssignmentVal = IsCNNodeAssignmentValid(nodeObj);

	if (true == copyNodeAssignmentVal)
	{
		IndexCollection* indexCollObjCN = NULL;
		Index* indexObjCN = NULL;

		indexCollObjCN = nodeObj->GetIndexCollection();
		if (NULL == indexCollObjCN)
		{
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		indexObjCN = indexCollObjCN->GetIndexbyIndexValue((char*) "1F81");
		if (NULL == indexObjCN)
		{
#if defined DEBUG
			cout << "CN: " << nodeObj->GetNodeId() << " INDEX: 1F81 Not found"
			<< endl;
#endif
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		indexObjCN->SetFlagIfIncludedCdc(TRUE);

		Index* indexObjMN = NULL;
		indexObjMN = GetMNIndexValues((char*) "1F81");
		if (NULL == indexObjMN)
		{
#if defined DEBUG
			cout << "MN INDEX: 1F81 Not found" << endl;
#endif
			objException.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		for (INT32 sidxLC = 0; sidxLC < indexObjCN->GetNumberofSubIndexes();
				sidxLC++)
		{
			SubIndex* sidxObjCN = NULL;
			sidxObjCN = indexObjCN->GetSubIndex(sidxLC);
			if (NULL == sidxObjCN)
			{
				continue;
			}

			SubIndex* sidxObjMN = NULL;
			sidxObjMN = indexObjMN->GetSubIndexbyIndexValue(
					(char*) sidxObjCN->GetIndexValue());

			if (NULL == sidxObjMN)
			{
				continue;
			}

			if (NULL != sidxObjMN->GetActualValue())
			{
				sidxObjCN->SetActualValue((char*) sidxObjMN->GetActualValue());
			}
			else
			{
				sidxObjCN->SetActualValue((char*) "");
			}
			sidxObjCN->SetFlagIfIncludedCdc(TRUE);
		}
	}
	else
	{
#if defined DEBUG
		cout << "CN Node Assignment Invalid" << endl;
#endif
	}
}

bool IsCNNodeAssignmentValid(Node* nodeObj)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	INT32 indexPos = 0;
	INT32 sidxPos = 0;
	INT32 nodeId = 0;
	bool copyNodeAssignmentVal = false;
	NodeType nodeType;

	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw exceptionObj;
	}

	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();

	errCodeObj = IfIndexExists(nodeId, nodeType, (char*) "1F9B", &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		errCodeObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98",
				(char*) "07", &sidxPos, &indexPos);
		if (OCFM_ERR_SUCCESS == errCodeObj.code)
		{
			char* multipleCycleCnt = new char[20];
			errCodeObj = GetSubIndexAttributes(nodeId, nodeType, (char*) "1F98",
					(char*) "07", ACTUALVALUE, multipleCycleCnt);
			if (OCFM_ERR_SUCCESS == errCodeObj.code)
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
#if defined DEBUG
				cout << nodeType << ":" << nodeId << "1F9B/07 GetSubIndexAttributes Failed" << endl;
#endif
			}
			delete[] multipleCycleCnt;
		}
		else
		{
#if defined DEBUG
			cout << nodeType << ":" << nodeId << "1F9B/07 Not Found" << endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << nodeType << ":" << nodeId << "1F9B Not Found" << endl;
#endif
	}

	errCodeObj = IfIndexExists(nodeId, nodeType, (char*) "1016", &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		copyNodeAssignmentVal = true;
	}
	else
	{
#if defined DEBUG
		cout << nodeType << ":" << nodeId << "1016 Not Found" << endl;
#endif
	}

	errCodeObj = IfIndexExists(nodeId, nodeType, (char*) "1F8D", &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		copyNodeAssignmentVal = true;
	}
	else
	{
#if defined DEBUG
		cout << nodeType << ":" << nodeId << "1F8D Not Found" << endl;
#endif
	}
	return copyNodeAssignmentVal;
}

void UpdateCNMultipleCycleAssign(Node* nodeObj)
{
	ocfmException exceptionObj;
	IndexCollection* indexCollObjCN = NULL;
	Index* indexObjCN = NULL;
	Index* indexObjMN = NULL;
	SubIndex* sindexObjCN = NULL;
	SubIndex* sindexObjMN = NULL;

	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw exceptionObj;
	}

	indexCollObjCN = nodeObj->GetIndexCollection();

	if (NULL == indexCollObjCN)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
		throw exceptionObj;
	}

	indexObjCN = indexCollObjCN->GetIndexbyIndexValue((char*) "1F9B");
	if (NULL == indexObjCN)
	{
		return;
	}

	indexObjCN->SetFlagIfIncludedCdc(TRUE);
	indexObjMN = GetMNIndexValues((char*) "1F9B");
	if (NULL == indexObjMN)
	{
		return;
	}

	for (INT32 sidxLC = 0; sidxLC < indexObjCN->GetNumberofSubIndexes();
			sidxLC++)
	{

		sindexObjCN = indexObjCN->GetSubIndex(sidxLC);
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
		sindexObjCN->SetFlagIfIncludedCdc(TRUE);
	}
}

void UpdateCNPresMNActLoad(Node* nodeObj)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	INT32 indexPos = 0;
	INT32 subIndexPos = 0;
	INT32 nodeId = 0;
	NodeType nodeType;

	NodeCollection* nodeCollObj = NULL;
	IndexCollection* indexCollObj = NULL;
	Index* indexObj = NULL;

	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw exceptionObj;
	}

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	if (NULL == nodeCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
		throw exceptionObj;
	}

	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();
	errCodeObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F8D",
			(char*) "F0", &subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != errCodeObj.code)
	{
#if defined DEBUG
		cout << "UpdateCNPresMNActLoad failed. 1F8D/F0 does not exist" << endl;
#endif
		return;
	}

	char *subIndexId = new char[SUBINDEX_LEN];
	strcpy(subIndexId, (char*) "F0");

	indexCollObj = nodeObj->GetIndexCollection();
	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw exceptionObj;
	}

	indexObj = indexCollObj->GetIndexbyIndexValue((char*) "1F8D");
	if (NULL == indexObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
		throw exceptionObj;
	}
	else
	{
		SubIndex *sidxObj = NULL;
		sidxObj = indexObj->GetSubIndexbyIndexValue(subIndexId);
		if (NULL == sidxObj)
		{
			exceptionObj.OCFMException(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw exceptionObj;
		}
		else
		{
			if (CHAINED == nodeObj->GetStationType())
			{
				char conValue[20];
				char actValue[22];
				Node nodeObjMN;

				memset(conValue, 0, 20 * sizeof(char));
				memset(actValue, 0, 22 * sizeof(char));
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

				indexObj->SetFlagIfIncludedCdc(TRUE);
				sidxObj->SetFlagIfIncludedCdc(TRUE);
			}
			else
			{
				sidxObj->SetActualValue((char*) "");
				sidxObj->SetFlagIfIncludedCdc(FALSE);
			}
		}
	}
	delete[] subIndexId;
}

void UpdatePreqActLoad(Node* nodeObj)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	IndexCollection* indexCollObj = NULL;
	Index* indexObj = NULL;

	INT32 indexPos = 0;
	INT32 subIndexPos = 0;
	INT32 nodeId = 0;
	NodeType nodeType;

	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw exceptionObj;
	}

	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();
	errCodeObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98",
			(char*) "04", &subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != errCodeObj.code)
	{
#if defined DEBUG
		cout << "UpdatePreqActLoad failed. 1F98/04 does not exist" << endl;
#endif
		return;
	}

	indexCollObj = nodeObj->GetIndexCollection();
	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
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
			indexObj->SetFlagIfIncludedCdc(TRUE);
			sidxObj->SetFlagIfIncludedCdc(TRUE);

			//set the value in MN
			if (CN == nodeObj->GetNodeType())
			{
				Index* indexObjMN = NULL;

				IntToAscii(nodeObj->GetNodeId(), subIndexId, 16);
				subIndexId = PadLeft(subIndexId, '0', 2);

				errCodeObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F8B",
						subIndexId, &subIndexPos, &indexPos);
				if (OCFM_ERR_SUCCESS != errCodeObj.code)
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
						indexObjMN->SetFlagIfIncludedCdc(TRUE);
						sidxObjMN->SetFlagIfIncludedCdc(TRUE);
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
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	Index* indexObj = NULL;
	IndexCollection* indexCollObj = NULL;
	INT32 indexPos = 0;
	INT32 subIndexPos = 0;
	INT32 nodeId = 0;
	char conValue[20];
	char actValue[22];
	char* subIndexId = NULL;
	NodeType nodeType;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw exceptionObj;
	}

	memset(conValue, 0, 20 * sizeof(char));
	memset(actValue, 0, 22 * sizeof(char));
	nodeId = nodeObj->GetNodeId();
	nodeType = nodeObj->GetNodeType();
	subIndexId = new char[SUBINDEX_LEN];
	strcpy(subIndexId, (char*) "05");

	errCodeObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98", subIndexId,
			&subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != errCodeObj.code)
	{
#if defined DEBUG
		cout << "UpdatePresActLoad failed. 1F98/05 does not exist" << endl;
#endif
		return;
	}

	indexCollObj = nodeObj->GetIndexCollection();
	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
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
			indexObj->SetFlagIfIncludedCdc(TRUE);
			subIndexObj->SetFlagIfIncludedCdc(TRUE);

			//set the value in MN
			if (CN == nodeObj->GetNodeType())
			{
				IntToAscii(nodeObj->GetNodeId(), subIndexId, 16);
				subIndexId = PadLeft(subIndexId, '0', 2);

				errCodeObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F8D",
						subIndexId, &subIndexPos, &indexPos);
				if (OCFM_ERR_SUCCESS != errCodeObj.code)
				{
#if defined DEBUG
					cout << "UpdatePresActLoad failed. MN 1F8D/" << subIndexId
					<< " does not exist" << endl;
#endif
					return;
				}
				indexObj = GetMNIndexValues((char *) "1F8D");
				if (NULL != indexObj)
				{
					subIndexObj = indexObj->GetSubIndexbyIndexValue(subIndexId);
					if (NULL != subIndexObj)
					{
						subIndexObj->SetActualValue(actValue);
						indexObj->SetFlagIfIncludedCdc(TRUE);
						subIndexObj->SetFlagIfIncludedCdc(TRUE);
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
#if defined DEBUG
					cout << "UpdatePresActLoad failed. MN 1F8D Index does not exist" << endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "UpdatePresActLoad failed. Processed node is MN" << endl;
#endif
			}
		}
		else
		{
#if defined DEBUG
			cout << "UpdatePresActLoad failed. 1F98/05 Index does not exist"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "UpdatePresActLoad failed. 1F98 Index does not exist" << endl;
#endif
	}
	delete[] subIndexId;
}

void UpdateCNVisibleNode(Node* nodeObj)
{
	ocfmException exceptionObj;

	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw exceptionObj;
	}

	IndexCollection* pdoIndexCollObj = NULL;
	INT32 tpdoCount = 0;
	INT32 rpdoCount = 0;
	pdoIndexCollObj = nodeObj->getPDOIndexCollection(&tpdoCount, &rpdoCount);

	if (NULL == pdoIndexCollObj)
	{
#if defined DEBUG
		cout << "UpdateCNVisibleNode failed: PDOCollection empty." << endl;
#endif
		return;
	}

	Index* indexObj = NULL;
	Index* commIndexObj = NULL;
	SubIndex* sidxObj1 = NULL;
	SubIndex* sidxObj2 = NULL;
	char* indexId = NULL;
	char* commIdxId = NULL;
	char* mappedNodeId = NULL;
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
		indexObj = pdoIndexCollObj->GetIndex(idxLC);
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
					commIdxId = new char[INDEX_LEN];
					indexId = SubString((char*) indexObj->GetIndexValue(), 2,
							2);
					strcpy(commIdxId, (char*) "14");
					strcat(commIdxId, indexId);
					//Delete may be issue bcoz of SubString function
					delete[] indexId;

					commIndexObj = pdoIndexCollObj->GetIndexbyIndexValue(
							commIdxId);
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
								if (MAX_CN_CROSS_TRAFFIC_STN < crossTxStnCnt)
								{
									exceptionObj._ocfmRetCode.code =
											OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN;
									char acCustomError[200];
									INT32 status = 0;
									status =
											sprintf(acCustomError,
													"The CN node id: %d has been configured with more than the permissible number of cross traffic stations. The maximum permitted is %d",
													nodeObj->GetNodeId(),
													MAX_CN_CROSS_TRAFFIC_STN);
									if (status < 0)
									{
										cout << "Error in sprintf" << __LINE__
												<< endl;
									}
									else
									{
										//sprintf success
									}
									CopyCustomErrorString(
											&(exceptionObj._ocfmRetCode),
											acCustomError);
									throw exceptionObj;
								}

								//copy the MN objects
								if (CheckIfHex(
										(char*) sidxObj2->GetActualValue()))
								{
									mappedNodeId = SubString(
											(char*) sidxObj2->GetActualValue(),
											2, 2);
								}
								else
								{
									mappedNodeId = new char[SUBINDEX_LEN];
									strcpy(mappedNodeId,
											sidxObj2->GetActualValue());
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
											cout
													<< "CopyMNSubindexToCN 1F8D Fails:"
													<< __FUNCTION__ << endl;
										}
									}
									else
									{
										cout << "CopyMNSubindexToCN 1F81 Fails:"
												<< __FUNCTION__ << endl;
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

bool CopyMNSubindexToCN(Node* nodeObj, char* indexId, char* subIndexId)
{
	bool sidxCopied = false;
	ocfmException exceptionObj;
	IndexCollection* indexCollObj = NULL;
	Index* indexObjMN = NULL;
	Index* indexObjCN = NULL;

	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
		throw exceptionObj;
	}

	indexCollObj = nodeObj->GetIndexCollection();
	if (NULL == indexCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
		throw exceptionObj;
	}

	if ((NULL == indexId) || (NULL == subIndexId))
	{
#if defined DEBUG
		cout << "CopyMNSubindexToCN failed: Invalid Parameters" << endl;
#endif
		exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
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
#if defined DEBUG
			cout
			<< "CopyMNSubindexToCN failed: MNSubindex/CNSubindex Not present"
			<< endl;
#endif
		}
		else if (NULL != sidxObjMN->GetActualValue())
		{
			sidxObjCN->SetActualValue((char*) sidxObjMN->GetActualValue());
			sidxObjCN->SetFlagIfIncludedCdc(TRUE);
			indexObjCN->SetFlagIfIncludedCdc(TRUE);
			sidxCopied = true;
		}
		else
		{
#if defined DEBUG
			cout
			<< "CopyMNSubindexToCN failed: pobjMNSubindex has No Actual Value configured"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout
		<< "CopyMNSubindexToCN failed: MNIndex/CNIndex is Not Present"
		<< endl;
#endif
	}
	return sidxCopied;
}

void ResetAllSubIndexFlag(Index* indexObj)
{
	if (NULL == indexObj)
	{
#if defined DEBUG
		cout << "ResetAllSubIndexFlag failed: Index Not found" << endl;
#endif
		return;
	}

	for (INT32 sidxLC = 0; sidxLC < indexObj->GetNumberofSubIndexes(); sidxLC++)
	{
		SubIndex* sidxObj = NULL;
		sidxObj = indexObj->GetSubIndex(sidxLC);
		if (NULL != sidxObj)
		{
			sidxObj->SetFlagIfIncludedCdc(FALSE);
		}
		else
		{
#if defined DEBUG
			cout << "Some subindexes are NULL. Check for Memory operations"
			<< endl;
#endif
		}
	}
	indexObj->SetFlagIfIncludedCdc(FALSE);
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
		indexObj = indexCollObj->GetIndex(idxLC);
		if ((NULL == indexObj) || (NULL == indexObj->GetIndexValue()))
		{
			continue;
		}

		if (!CheckIfNotPDO((char*) indexObj->GetIndexValue()))
		{
			INT32 totalNoOfSidx = 0;
			totalNoOfSidx = indexObj->GetNumberofSubIndexes();

			char* idxIdStr = new char[SUBINDEX_LEN];
			idxIdStr = SubString((char*) indexObj->GetIndexValue(), 0, 2);
			if ((0 == strcmp(idxIdStr, "1A")) || (0 == strcmp(idxIdStr, "1a"))
					|| (0 == strcmp(idxIdStr, "16")))
			{
				for (INT32 sidxLoopCnt = 0; sidxLoopCnt < totalNoOfSidx;
						sidxLoopCnt++)
				{
					SubIndex *sidxObj = NULL;
					sidxObj = indexObj->GetSubIndex(sidxLoopCnt);
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
					sidxObj = indexObj->GetSubIndex(sidxLoopCnt);
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
	if ((NULL == indexObj) || (NULL == cdcBuffer))
	{
#if defined DEBUG
		cout << "GetIndexData failed: Invalid Parameters" << endl;
#endif
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
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
					noOfSubIndexes = HexToInt(
							SubString((char*) sidxObj->GetActualValue(), 2,
									strlen(sidxObj->GetActualValue()) - 2));
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
#if defined DEBUG
		cout << __FUNCTION__ << "noOfSubIndexes:" << noOfSubIndexes << endl;
#endif
		for (INT32 sidxLC = 0; sidxLC < noOfSubIndexes; sidxLC++)
		{
			bool includeAccess = false;

			sidxObj = indexObj->GetSubIndex(sidxLC);
#if defined DEBUG
			cout << "Indexx:" << indexObj->GetIndexValue() << " SIdx:"
			<< sidxObj->GetIndexValue() << endl;
#endif
			includeAccess = CheckAccessTypeForInclude(
					(char*) sidxObj->GetAccessType());

			if ((sidxObj->GetActualValue() != NULL)
					&& (sidxObj->GetFlagIfIncludedCdc() == TRUE)
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
	if ((NULL == indexObj) || (NULL == cdcBuffer))
	{
#if defined DEBUG
		cout << "BRSpecificGetIndexData failed: Invalid Parameters" << endl;
#endif
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
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
					noOfSubIndexes = HexToInt(
							SubString((char*) sidxObj->GetActualValue(), 2,
									strlen(sidxObj->GetActualValue()) - 2));
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
#if defined DEBUG
		cout << __FUNCTION__ << "noOfSubIndexes:" << noOfTotalSubIndexes << endl;
#endif
		for (INT32 sidxLC = 0; sidxLC < noOfTotalSubIndexes; sidxLC++)
		{
			bool includeAccess = false;
#if defined DEBUG
			cout << "Indexx:" << indexObj->GetIndexValue() << " SIdx:"
			<< sidxObj->GetIndexValue() << endl;
#endif
			sidxObj = indexObj->GetSubIndex(sidxLC);

			includeAccess = CheckAccessTypeForInclude(
					(char*) sidxObj->GetAccessType());

			if ((sidxObj->GetActualValue() != NULL)
					&& (sidxObj->GetFlagIfIncludedCdc() == TRUE)
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
					else
					{
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
	ocfmException exceptionObj;
	NodeCollection* nodeCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	if (NULL == nodeCollObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
		throw exceptionObj;
	}

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
			if (NULL == indexCollObj)
			{
				exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
				delete[] fileptr;
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

			indexObj = GetMNIndexValues((char*) "1006");
			if (NULL != indexObj)
			{
				if (NULL != (char*) indexObj->GetActualValue())
				{
					UpdateCNCycleTime(indexCollObj,
							(char*) indexObj->GetActualValue());
				}
				else
				{
#if defined DEBUG
					cout << "MN index: 1006 ActualValue Not Present, Not Updated CNCycleTime" << endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1006 Not Found, Not Updated CNCycleTime" << endl;
#endif
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
#if defined DEBUG
					cout << "MN index: 1C14 ActualValue Not Present, Not Updated CNCycleTime" << endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1C14 Not Found, Not Updated CNSoCTolerance"
				<< endl;
#endif
			}

			indexObj = GetMNIndexValues((char*) "1F26");
			if (NULL != indexObj)
			{
				UpdatedCNDateORTime(indexObj, nodeObj.GetNodeId(), DATE);
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F26 Not Found, Not Updated CN Date" << endl;
#endif
			}

			indexObj = GetMNIndexValues((char*) "1F27");
			if (NULL != indexObj)
			{
				UpdatedCNDateORTime(indexObj, nodeObj.GetNodeId(), TIME);
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F27 Not Found, Not Updated CN Time" << endl;
#endif
			}

			indexObj = GetMNIndexValues((char*) "1F98");
			if (NULL != indexObj)
			{
				SubIndex* subIndexObj = NULL;
				subIndexObj = GetMNSubIndexValues((char*) "1F98", (char*) "08");
				if ((NULL != subIndexObj)
						&& (NULL != subIndexObj->GetActualValue()))
				{
					UpdateCNAsyncMTUsize(indexCollObj,
							(char*) subIndexObj->GetActualValue());
				}
				else
				{
#if defined DEBUG
					cout << "MN index: 1F98/08 Not Found or ActualValue not present" << endl;
#endif
				}

				subIndexObj = GetMNSubIndexValues((char*) "1F98", (char*) "07");
				if ((NULL != subIndexObj)
						&& (NULL != subIndexObj->GetActualValue()))
				{
					UpdateCNMultiPrescal(indexCollObj,
							(char*) subIndexObj->GetActualValue());
				}
				else
				{
#if defined DEBUG
					cout << "MN index: 1F98/07 Not Found or ActualValue not present" << endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F98 Not Found, Not Updated CN AsyncMTUsize & MultiPrescal" << endl;
#endif
			}

			indexObj = GetMNIndexValues((char*) "1F9B");
			if (NULL != indexObj)
			{
				UpdateCNMultipleCycleAssign(&nodeObj);
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F9B Not Found, Not Updated CN MultipleCycleAssign"<< endl;
#endif
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
			strcpy(noOfEnteries, ConvertToUpper(noOfEnteries));
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
		indexObj = indexCollObj->GetIndex(indexLC);

		if ((indexObj->GetFlagIfIncludedCdc() == TRUE)
				&& (true
						== CheckAccessTypeForInclude(
								(char*) indexObj->GetAccessType())
						|| CheckIfMappingPDO((char*) indexObj->GetIndexValue())))
		{
			if (indexObj->GetNumberofSubIndexes() == 0)
			{
				if ((indexObj->GetActualValue() != NULL)
						&& (true == IsDefaultActualNotEqual(indexObj)))
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
						for (INT32 sidxLC = 0;
								sidxLC < indexObj->GetNumberofSubIndexes();
								sidxLC++)
						{
							if ((indexObj->GetSubIndex(sidxLC)->GetActualValue()
									!= NULL)
									&& (TRUE
											== indexObj->GetSubIndex(sidxLC)->GetFlagIfIncludedCdc())
									&& (true
											== IsDefaultActualNotEqual(
													indexObj->GetSubIndex(
															sidxLC))))
							{
								if (0
										== strcmp(
												(char*) indexObj->GetSubIndex(
														sidxLC)->GetIndexValue(),
												"00"))
								{
									continue;
								}
								if (0
										== GetDecimalValue(
												(char*) indexObj->GetSubIndex(
														sidxLC)->GetActualValue()))
								{
									if ((NULL
											== indexObj->GetSubIndex(sidxLC)->GetDefaultValue())
											|| (0
													== GetDecimalValue(
															(char*) indexObj->GetSubIndex(
																	sidxLC)->GetDefaultValue())))
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
						sidxObj = indexObj->GetSubIndexbyIndexValue(
								(char*) "F0");
						if (NULL != sidxObj && sidxObj->GetActualValue() != NULL
								&& 0
										!= strcmp(
												(char*) sidxObj->GetActualValue(),
												"")
								&& true
										== CheckAccessTypeForInclude(
												(char*) sidxObj->GetAccessType())
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
					if ((indexObj->GetSubIndex(sidxLC)->GetActualValue() != NULL)
							&& (TRUE
									== indexObj->GetSubIndex(sidxLC)->GetFlagIfIncludedCdc())
							&& (true
									== CheckAccessTypeForInclude(
											(char*) indexObj->GetSubIndex(
													sidxLC)->GetAccessType()))
							&& (true
									== IsDefaultActualNotEqual(
											indexObj->GetSubIndex(sidxLC))))
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
		indexObj = indexCollObj->GetIndex(indexLC);

		if (indexObj->GetFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) indexObj->GetAccessType())
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
							&& !(CheckIfValueZero(
									(char*) sidxObj->GetActualValue())))
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

						INT32 isiTotal = indexObj->GetNumberofSubIndexes();

						for (INT32 sidxLC = 0; sidxLC < isiTotal; sidxLC++)
						{

							if (indexObj->GetSubIndex(sidxLC)->GetActualValue()
									!= NULL
									&& TRUE
											== indexObj->GetSubIndex(sidxLC)->GetFlagIfIncludedCdc()
									&& true
											== IsDefaultActualNotEqual(
													indexObj->GetSubIndex(
															sidxLC)))
							{
								if (0
										== strcmp(
												(char*) indexObj->GetSubIndex(
														sidxLC)->GetIndexValue(),
												"00"))
								{
									//the no.of subindex is determied by the 00'th entry actual value. 1 is added for 00'th entry count
									isiTotal = (GetDecimalValue(
											(char*) sidxObj->GetActualValue())
											+ 1);
									continue;
								}
								if (0
										== GetDecimalValue(
												(char*) indexObj->GetSubIndex(
														sidxLC)->GetActualValue()))
								{
									if (NULL
											== indexObj->GetSubIndex(sidxLC)->GetDefaultValue()
											|| 0
													== GetDecimalValue(
															(char*) indexObj->GetSubIndex(
																	sidxLC)->GetDefaultValue()))
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
					if (indexObj->GetSubIndex(sidxLC)->GetActualValue() != NULL
							&& TRUE
									== indexObj->GetSubIndex(sidxLC)->GetFlagIfIncludedCdc()
							&& true
									== CheckAccessTypeForInclude(
											(char*) indexObj->GetSubIndex(
													sidxLC)->GetAccessType())
							&& true
									== IsDefaultActualNotEqual(
											indexObj->GetSubIndex(sidxLC)))
					{
						if (0
								== strcmp(indexObj->GetIndexValue(),
										(char*) "1F81"))
						{

							if (0
									== strcmp(
											indexObj->GetSubIndex(sidxLC)->GetIndexValue(),
											"00")
									|| 0
											== strcmp(
													indexObj->GetSubIndex(
															sidxLC)->GetIndexValue(),
													tempNodeId))
							{
								continue;
							}
							else
							{
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

ocfmRetCode GenerateCDC(char* cdcPath)
{

	Node nodeObjMN;
	Node *nodeObj = NULL;
	IndexCollection* indexCollObj;
	char *Buffer1 = NULL;
	char *tempFileName = NULL;
	char *tempOutputFileName = NULL;
	UINT32 len;
	ocfmRetCode errCodeObj;
	ocfmRetCode errCodeObj1;
	ocfmRetCode errCodeObj2;
	ocfmException exceptionObj;
	INT32 sidxPos = 0;
	INT32 indexPos = 0;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj2.code = OCFM_ERR_UNKNOWN;
	errCodeObj1.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	errCodeObj2.errorString = NULL;
	errCodeObj1.errorString = NULL;

	try
	{
		//get the MN Node object from the NodeCollection
		NodeCollection* objNodeCollection;
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();

		/******************************* Write MN's Indexes ******************************************************************/

		nodeObjMN = objNodeCollection->GetMNNode();
		if (objNodeCollection->GetNumberOfNodes() == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		if (nodeObjMN.IsNull())
		{
			exceptionObj.OCFMException(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			throw exceptionObj;
		}
		if (objNodeCollection->GetCNNodesCount() == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_CN_NODES_FOUND);
			throw exceptionObj;
		}

		/* Check if autogeneration of MN's Object Dicitonary is set to true */

		PjtSettings* pjtSettingsObj;
		pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();

		if (pjtSettingsObj->GetGenerateAttr() == YES_AG)
		{
			errCodeObj = CheckMutliplexAssigned();
			if (errCodeObj.code != OCFM_ERR_SUCCESS)
			{
				return errCodeObj;
			}
			/*Generate MNOBD for Auto Generate On*/
			errCodeObj = GenerateMNOBD(true);
			if (OCFM_ERR_EXCESS_CHANNEL == errCodeObj.code)
			{
				//Do not throw exception here as we need the process to complete
				errCodeObj2 = errCodeObj;
			}
			else if (OCFM_ERR_SUCCESS != errCodeObj.code)
			{
				return errCodeObj;
			}
			else
			{
			}
		}
		else
		{
			for (INT32 nodeLC = 0;
					nodeLC < objNodeCollection->GetNumberOfNodes(); nodeLC++)
			{
				nodeObj = objNodeCollection->GetNodebyColIndex(nodeLC);
				/* Process PDO Objects for CN*/
				if (NULL == nodeObj)
				{
#if defined DEBUG
					cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

					exceptionObj.OCFMException(
							OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					throw exceptionObj;
				}

				if (nodeObj->GetNodeType() == MN)
				{
					indexCollObj = nodeObj->GetIndexCollection();
					if (NULL == indexCollObj)
					{
#if defined DEBUG
						cout << "Memory allocation error" << __FUNCTION__
						<< endl;
#endif

						exceptionObj.OCFMException(
								OCFM_ERR_MEMORY_ALLOCATION_ERROR);
						throw exceptionObj;
					}
				}
				else
				{
					if (!(nodeObj->HasPdoObjects()))
					{

						continue;
					}
				}
			}
		}

		FILE* fileptr = new FILE();
		tempFileName = new char[strlen(cdcPath) + 10 + strlen("mnobd")];
		sprintf(tempFileName, "%s/%s.txt", cdcPath, "mnobd");

		if ((fileptr = fopen(tempFileName, "w+")) == NULL)
		{
			exceptionObj.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw exceptionObj;
		}

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
				char* tempStr2 = new char[strlen(nodeAssignmentBitsStr)
						+ ALLOC_BUFFER + 2];
				sprintf(tempStr2, "0X%s", nodeAssignmentBitsStr);
				errCodeObj1 = IfSubIndexExists(MN_NODEID, MN, (char*) "1F81",
						tempStr, &sidxPos, &indexPos);
				if (OCFM_ERR_SUCCESS != errCodeObj1.code)
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

		nodeObjMN = objNodeCollection->GetMNNode();
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
		if (YES_AG == pjtSettingsObj->GetGenerateAttr())
		{
			CalculatePayload();
		}

		//Buffer1 = (char*)malloc(CDC_BUFFER);
		Buffer1 = new char[CDC_BUFFER];
		char* noOfEntries = new char[10];
		noOfEntries = IntToAscii(GetNodeTotalIndexSubIndex(MN_NODEID),
				noOfEntries, 16);
		noOfEntries = PadLeft(noOfEntries, '0', 8);
		strcpy(Buffer1, noOfEntries);
		strcat(Buffer1, "\n");
		len = strlen(Buffer1);
		delete[] noOfEntries;

		/* Write number of enteries */
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
#ifdef DEBUG
			cout << "Write Error in CDC" << endl;
#endif
		}
		delete[] Buffer1;

		// Add 1F81
		for (INT32 nodeLC = 0; nodeLC < objNodeCollection->GetNumberOfNodes();
				nodeLC++)
		{
			Node nodeObjCN;
			nodeObjCN = objNodeCollection->GetNodebyCollectionIndex(nodeLC);
			if (nodeObjCN.GetNodeType() == CN)
			{

				Buffer1 = new char[CDC_BUFFER];
				strcpy(Buffer1, "//// NodeId Assignment\n");
				strcat(Buffer1, "1F81");
				strcat(Buffer1, "\t");
				INT32 nodeID = nodeObjCN.GetNodeId();
				char* tempStr = new char[10];
				tempStr = IntToAscii(nodeID, tempStr, 16);
				tempStr = PadLeft(tempStr, '0', 2);
				strcat(Buffer1, tempStr);
				strcat(Buffer1, "\t00000004\t00000007\n");
				strcat(Buffer1, "\n");
				len = strlen(Buffer1);
				// write 1F81 entry in MN text file
				if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
				{
#ifdef DEBUG
					cout << "Write Error in CDC _1" << endl;
#endif
				}
				delete[] tempStr;
				delete[] Buffer1;
			}
		}

		fclose(fileptr);

		if ((fileptr = fopen(tempFileName, "a+")) == NULL)
		{
			exceptionObj.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw exceptionObj;
		}
		objNodeCollection = NodeCollection::GetNodeColObjectPointer();
		nodeObjMN = objNodeCollection->GetMNNode();
		indexCollObj = nodeObjMN.GetIndexCollection();

		//Get all the MN's Default Data in Buffer1
		Buffer1 = new char[CDC_MN_BUFFER];
		memset(Buffer1, 0, CDC_MN_BUFFER * sizeof(char));
		FormatCdc(indexCollObj, Buffer1, fileptr, MN);

		len = strlen(Buffer1);
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
#ifdef DEBUG
			cout << "Write Error in CDC _2" << endl;
#endif
		}
		delete[] Buffer1;
		fclose(fileptr);

		/*************************Write CN's Data in Buffer2***************************************************/
		WriteCNsData((char*) tempFileName);
		//INT32 ret;

		//Get all the IF81 ENTRY in Buffer1
		if (objNodeCollection->GetNumberOfNodes() != 0)
		{

			for (INT32 nodeLC = 0;
					nodeLC < objNodeCollection->GetNumberOfNodes(); nodeLC++)
			{
				Node nodeObjCN;
				nodeObjCN = objNodeCollection->GetNodebyCollectionIndex(nodeLC);
				if (nodeObjCN.GetNodeType() == CN)
				{
					if ((fileptr = fopen(tempFileName, "a+")) == NULL)
					{
						exceptionObj.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
						throw exceptionObj;
					}
					Buffer1 = new char[CDC_BUFFER];
					len = strlen(Buffer1);
					strcpy(Buffer1, "//// NodeId Reassignment\n");
					strcat(Buffer1, "1F81");
					strcat(Buffer1, "\t");
					INT32 nodeId = nodeObjCN.GetNodeId();
					char* tempStr = new char[10];
					tempStr = IntToAscii(nodeId, tempStr, 16);
					tempStr = PadLeft(tempStr, '0', 2);
					strcat(Buffer1, tempStr);

					strcat(Buffer1, "\t00000004\t");
					char* nodeAssignmentData = GetNodeAssigmentBits(&nodeObjCN);
					strcat(Buffer1, nodeAssignmentData);

					strcat(Buffer1, "\n");

					len = strlen(Buffer1);
					if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
					{

						//TODO: Need to specify warning or err if fwrite fails.
						fclose(fileptr);

					}
					else
					{
						fclose(fileptr);
					}
					delete[] tempStr;
					delete[] Buffer1;
				}
			}

		}

		// Convert CDC txt file to Binary
		INT32 returnFromTxt2Cdc;
		char* cmdBuffer;
		tempOutputFileName = new char[1000];
		sprintf(tempOutputFileName, "%s/%s.cdc", cdcPath, "mnobd");
		//TODO: Calculate size dynamically
#if defined(_WIN32) && defined(_MSC_VER)
		cmdBuffer = new char[(2 * (strlen(cdcPath) + 10 + 10)) + 25];
		sprintf(cmdBuffer, "txt2cdc.exe \"%s\" \"%s\"", tempFileName,
				tempOutputFileName);
		returnFromTxt2Cdc = system(cmdBuffer);
#else
		cmdBuffer = new char[LINUX_INSTALL_DIR_LEN + (2 * (strlen(cdcPath) + 10 + 10)) + 25];
		sprintf(cmdBuffer, "%s/txt2cdc \"%s\" \"%s\"", LINUX_INSTALL_DIR, tempFileName, tempOutputFileName);
		returnFromTxt2Cdc = system(cmdBuffer);

#endif
		delete[] cmdBuffer;

		if (OCFM_ERR_SUCCESS == returnFromTxt2Cdc)
		{
			errCodeObj.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_TXT_FOR_CDC);
			throw exceptionObj;
		}
		if (OCFM_ERR_EXCESS_CHANNEL == errCodeObj2.code)
		{
			exceptionObj.OCFMException(errCodeObj2.code);
			throw exceptionObj;
		}
		else
		{
			//continue
		}
		delete[] tempFileName;
		delete[] tempOutputFileName;
	} catch (ocfmException & ex)
	{
		delete[] tempFileName;
		delete[] tempOutputFileName;
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

//TODO: only buffer is used not the fileptr. Should be removed in header & related functions
void FormatCdc(IndexCollection *objIndexCollection, char* Buffer1,
		FILE* fileptr, NodeType eNodeType)
{
	if ((NULL == objIndexCollection) || (NULL == Buffer1))
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
		indexObj = objIndexCollection->GetIndex(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == TRUE)
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
		indexObj = objIndexCollection->GetIndex(indexLC);

		if (indexObj->GetFlagIfIncludedCdc() == TRUE
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
				if (NULL != sidxObj && TRUE == sidxObj->GetFlagIfIncludedCdc()
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
		indexObj = objIndexCollection->GetIndex(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == TRUE
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
		indexObj = objIndexCollection->GetIndex(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == TRUE)
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
	ocfmException exceptionObj;
	if ((NULL == objIndexCollection) || (NULL == Buffer1))
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
		indexObj = objIndexCollection->GetIndex(indexLC);
		if (NULL == indexObj)
		{
#if defined DEBUG
			cout << "Memory allocation error 1" << __FUNCTION__ << __LINE__
			<< endl;
#endif

			exceptionObj.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw exceptionObj;
		}
		if (indexObj->GetFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being disabled.
				SubIndex* sidxObj = NULL;
				sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
				if (NULL == sidxObj)
				{
#if defined DEBUG
					cout << "Memory allocation error2" << __FUNCTION__
					<< __LINE__ << "Index" << indexObj->GetIndexValue()
					<< endl;
#endif

					exceptionObj.OCFMException(
							OCFM_ERR_MEMORY_ALLOCATION_ERROR);
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
		indexObj = objIndexCollection->GetIndex(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == TRUE
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
				if (NULL != sidxObj && TRUE == sidxObj->GetFlagIfIncludedCdc()
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
		indexObj = objIndexCollection->GetIndex(indexLC);
		if (indexObj->GetFlagIfIncludedCdc() == TRUE
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
		indexObj = objIndexCollection->GetIndex(indexLC);
		if (NULL == indexObj)
		{
#if defined DEBUG
			cout << "Memory allocation error3" << __FUNCTION__ << __LINE__
			<< endl;
#endif

			exceptionObj.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw exceptionObj;
		}
		if (indexObj->GetFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) indexObj->GetIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being enabled.
				SubIndex *sidxObj = NULL;
				sidxObj = indexObj->GetSubIndexbyIndexValue((char*) "00");
				if (NULL == sidxObj)
				{
#if defined DEBUG
					cout << "Memory allocation error4" << __FUNCTION__
					<< __LINE__ << endl;
#endif

					exceptionObj.OCFMException(
							OCFM_ERR_MEMORY_ALLOCATION_ERROR);
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
	ocfmException exceptionObj;
	if (cdtObj == NULL)
	{
		exceptionObj.OCFMException(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
		throw exceptionObj;
	}
	if ((NULL == appProcessObj) || (NULL == nodeObj) || (NULL == parameterObj)
			|| (NULL == moduleName) || (NULL == moduleIndexId))
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	//TODO: No header definitions is for ProcessCDT
	INT32 startBitOffset = 0;
	INT32 offsetVal = 0;
	bool isNewBitStr = false;
	INT32 dataSize = 0;
	INT32 totalBytesMapped = 0;
#if defined DEBUG
	cout<<"iStartBitOffset: "<<startBitOffset<<"iOffset"<<offsetVal<<"iDataSize"<<dataSize<<"iTotalBytesMapped"<<totalBytesMapped<<endl;
#endif
	for (INT32 varDeclLC = 0;
			varDeclLC < cdtObj->varDeclarationCollection.Count(); varDeclLC++)
	{

		varDeclaration varDeclObj;
		varDeclObj.Initialize();
		varDeclObj = cdtObj->varDeclarationCollection[varDeclLC];

		if (varDeclObj.namIdDtAttr->GetDtUniqueRefId() != NULL)
		{
			cdtObj = appProcessObj->GetCDTbyUniqueID(
					varDeclObj.namIdDtAttr->GetDtUniqueRefId());
			appProcessObj->UpdatePreviousCDTUId(varDeclObj.structUniqueId,
					cdtObj->cDtObjPosition);
			lastVarIndexGlobal = varDeclLC;
#if defined DEBUG
			cout<<"ProcessCDT Internal call 1 starts"<<endl;
#endif
			ProcessCDT(cdtObj, appProcessObj, nodeObj, parameterObj, pdoType,
					moduleName, moduleIndexId);
#if defined DEBUG
			cout<<"ProcessCDT Internal call 1 End"<<endl;
#endif
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
			else
			{
				//TODO: "else" Added. Nothing to do
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
				piObj.name =
						(char*) malloc(
								strlen(
										varDeclObj.namIdDtAttr->GetName()) + strlen(moduleName) + 6 + ALLOC_BUFFER);strcpy
				(piObj.name, GetPIName(nodeObj->GetNodeId()));
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
					for (INT32 bitStrCount = varDeclLC;
							bitStrCount
									< cdtObj->varDeclarationCollection.Count();
							bitStrCount++)
					{
						varDeclaration varDeclBitStr;

						varDeclBitStr.Initialize();
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
				else
				{
					//TODO: "else" Added.Operation need to be specified
				}
			}
			/* Set the Byte Offet*/
			piObj.byteOffset = offsetVal;
			if (isNewBitStr)
			{
				/* Total bytes Mapped */
				totalBytesMapped = totalBytesMapped + (dataSize / 8);
#if defined DEBUG
				cout<<"iTotalBytesMapped:"<<totalBytesMapped<<endl;
#endif
				if (totalBytesMapped > MAX_PI_SIZE)
				{
					ocfmException objex;
					objex.OCFMException(OCFM_ERR_MAX_PI_SIZE);
					throw objex;
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
			else
			{
				//TODO: "else" Added.Operation need to be specified
			}
			nodeObj->AddProcessImage(piObj);

		}

	}

	if (cdtObj->prevUniqueId != NULL)
	{
		cdtObj = appProcessObj->GetCDTbyUniqueID(cdtObj->prevUniqueId);

		for (INT32 iLoopCount = (lastVarIndexGlobal + 1);
				iLoopCount < cdtObj->varDeclarationCollection.Count();
				iLoopCount++)
		{
			if (!cdtCompletedGlobal)
			{
				varDeclaration objVarDecl;
				objVarDecl = cdtObj->varDeclarationCollection[iLoopCount];
				if (objVarDecl.namIdDtAttr->GetDtUniqueRefId() != NULL)
				{
					cdtObj = appProcessObj->GetCDTbyUniqueID(
							objVarDecl.namIdDtAttr->GetDtUniqueRefId());
					appProcessObj->UpdatePreviousCDTUId(
							objVarDecl.structUniqueId, cdtObj->cDtObjPosition);

					lastVarIndexGlobal = iLoopCount;
#if defined DEBUG
					cout<<"ProcessCDT Internal call 2 starts"<<endl;
#endif
					ProcessCDT(cdtObj, appProcessObj, nodeObj, parameterObj,
							pdoType, moduleName, moduleIndexId);
#if defined DEBUG
					cout<<"ProcessCDT Internal call 2 End"<<endl;
#endif

				}
			}
		}
	}
	cdtCompletedGlobal = true;
#if defined DEBUG
	cout<<"iTotalBytesMapped: "<<totalBytesMapped<<" iOffset"<<offsetVal<<endl;
#endif
	//Returned current mapped size in bytes
	return totalBytesMapped;
}

INT32 DecodeUniqueIDRef(char* uniquedIdref, Node* nodeObj, PDOType pdoType,
		char* moduleName, char* moduleIndex)
{
	ocfmException exceptionObj;

	if ((NULL == uniquedIdref) || (NULL == nodeObj) || (NULL == moduleName)
			|| (NULL == moduleIndex))
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	Parameter* parameterObj = NULL;
	ApplicationProcess* appProcessObj = NULL;
	ComplexDataType* cdtObj = NULL;
	INT32 totalBytesMapped = 0;
	INT32 iStartBitOffset =  0;
	INT32 iOffset;
	bool bIsNewBitStringVar = false;
	INT32 iDataSize = 0;
	//cout<<"DecodeUniqiueIDRef"<<endl;
	try
	{
		if (nodeObj->GetApplicationProcess() != NULL)
		{
			appProcessObj = nodeObj->GetApplicationProcess();
			if (appProcessObj->ParameterCollection.Count() != 0)
			{
				parameterObj = appProcessObj->GetParameterbyUniqueIDRef(uniquedIdref);
				if (parameterObj == NULL)
				{
					exceptionObj.OCFMException(OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND);
					char customError[200] = { 0 };
					sprintf(customError, "In node id: %d object %s with unique id: %s  reference not found", nodeObj->GetNodeId(), moduleName, uniquedIdref);
					CopyCustomErrorString(&(exceptionObj._ocfmRetCode), customError);
					throw exceptionObj;
				}
				cout<<"Parameter Found"<<" Uid: "<<uniquedIdref<<endl;
				// Check if DataTypeUniqueIDref exists
				//if (parameterObj->nameIdDtAttr.dataTypeUniqueIDRef != NULL)
				if((parameterObj->nameIdDtAttr.dataTypeUniqueIDRef != NULL) && (strcmp(parameterObj->nameIdDtAttr.dataTypeUniqueIDRef, "") != 0))
				{
					cout<<"Inside CDT calculation"<<endl;
					cdtObj = appProcessObj->GetCDTbyUniqueID(parameterObj->nameIdDtAttr.dataTypeUniqueIDRef);
					if (cdtObj == NULL)
					{
						exceptionObj.OCFMException(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
						char customError[200] = { 0 };
						sprintf(customError, "In node id: %d object %s with unique id: %s, reference to dataTypeUniqueIDRef: %s not found", nodeObj->GetNodeId(), moduleName, uniquedIdref, parameterObj->nameIdDtAttr.dataTypeUniqueIDRef);
						CopyCustomErrorString(&(exceptionObj._ocfmRetCode), customError);
						throw exceptionObj;
					}
					totalBytesMapped = ProcessCDT(cdtObj, appProcessObj, nodeObj, parameterObj, pdoType, moduleName, moduleIndex);
					lastVarIndexGlobal = -1;
					cdtCompletedGlobal = false;
				}
				else if ((parameterObj->nameIdDtAttr.dataType != NULL) && (strcmp(parameterObj->nameIdDtAttr.dataType, "") != 0))
				{
					cout<<"Dt: "<<parameterObj->nameIdDtAttr.dataType<<" ModName:"<<moduleName<<" moduleIndex:"<<moduleIndex<<endl;
					//pobjAppProc, nodeObj, parameterObj, pdoType, moduleName, moduleIndex
					ProcessImage objProcessImage;
					objProcessImage.Initialize();
					objProcessImage.bitOffset = -1;
					objProcessImage.byteOffset = 0;
					if(parameterObj->accessStr != NULL)
					{
						strcpy(objProcessImage.direction, GetParameterAccess(parameterObj->accessStr));
					}
					cout<<"Access:"<<objProcessImage.direction<<endl;
					if(pdoType == PDO_TPDO)
					{
						objProcessImage.directionType = INPUT;
					}
					else if(pdoType == PDO_RPDO)
					{
						objProcessImage.directionType = OUTPUT;
					}
					objProcessImage.nodeId = nodeObj->GetNodeId();

					objProcessImage.name = new char[strlen(uniquedIdref) + strlen(moduleName) + 6 + ALLOC_BUFFER];
					strcpy(objProcessImage.name, GetPIName(nodeObj->GetNodeId()));
					strcat(objProcessImage.name, moduleName);
					strcat(objProcessImage.name, ".");
					strcat(objProcessImage.name, uniquedIdref);
					
					objProcessImage.moduleName = new char[strlen(moduleName) + ALLOC_BUFFER];
					strcpy(objProcessImage.moduleName, moduleName);
					
					objProcessImage.moduleIndex = new char[strlen(moduleIndex) + ALLOC_BUFFER];
					strcpy(objProcessImage.moduleIndex, moduleIndex);

					//objProcessImage.varDeclName = (char*)malloc(strlen(uniquedIdref) + ALLOC_BUFFER);
					//strcpy(objProcessImage.varDeclName, uniquedIdref);
					cout<<"Name: "<<objProcessImage.name<<endl;

					objProcessImage.dataInfo = *(GetIECDT(parameterObj->nameIdDtAttr.dataType, parameterObj->size));

					cout<<objProcessImage.dataInfo.dtName<<" "<<objProcessImage.dataInfo.dataSize<<" "<<objProcessImage.dataInfo.iecDtVar<<endl;
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

						if(pdoType == PDO_RPDO)
						{
							iOffset =  ComputeOUTOffset(iDataSize, pdoType);
						}
						else if(pdoType == PDO_TPDO)
						{
							iOffset =  ComputeINOffset(iDataSize, pdoType);
						}
					}
					
					cout<<"iOffset"<<iOffset<<endl;
					objProcessImage.byteOffset = iOffset;

					 totalBytesMapped = totalBytesMapped + (iDataSize / 8);
					cout<<"Create MN pdo Var"<<" iOffset:"<<iOffset<<" iDataSize:"<<iDataSize<<endl;
					CreateMNPDOVar(iOffset, iDataSize, objProcessImage.dataInfo.iecDtVar, pdoType, nodeObj);
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
#if defined DEBUG
					cout << "Data type of unique id & datatype does not exists" << endl;
#endif
				}
			}
		}

	} catch (ocfmException& ex)
	{
		throw ex;
	}
	return totalBytesMapped;
}

ocfmRetCode ProcessPDONodes()
{
	return (ProcessPDONodes(false));
}

ocfmRetCode ProcessPDONodes(bool isBuild)
{
	NodeCollection *nodeCollObj = NULL;
	ocfmException exceptionObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node *nodeObj = NULL;
	//CNode *pobjMNNode = NULL;
	INT32 totalBytesMapped = 0;
	INT32 totalChainedBytesMapped = 0;
	INT32 nodeMappedTotalBytes = 0;
	INT32 rpdoMappedNodeID = 0;

	IndexCollection *pdoIndexCollObj = NULL;
	IndexCollection *indexCollObj = NULL;
	/* Check RPDO Mapped objects*/
	INT32 nodesCount = 0;
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	// bool bChangeOffset = false;

	nodesCount = nodeCollObj->GetCNNodesCount();

#if defined DEBUG
	cout << "Nodes count" << nodesCount << endl;
#endif

	try
	{
		if (nodesCount == 0)
		{
			//exit(0);
			exceptionObj.OCFMException(OCFM_ERR_NO_CN_NODES_FOUND);
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

		INT32 *nodeIDbyStnArranged = NULL;
		nodeIDbyStnArranged = ArrangeNodeIDbyStation();
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
					cout << "objPDOCollection NULL" << endl;
					continue;
				}
				//Validate the number of TPDO's for a CN
				if (countTPDO > 1)
				{
					exceptionObj.OCFMException(
							OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					char acCustomError[200] =
					{ 0 };
					sprintf(acCustomError,
							"CN Node-Id: %d cannot have more than one TPDO Channel",
							nodeObj->GetNodeId());
					CopyCustomErrorString(&(exceptionObj._ocfmRetCode),
							acCustomError);
					throw exceptionObj;
				}

				indexCollObj = nodeObj->GetIndexCollection();

				nodeObj->SetPReqActPayloadValue(0);
				nodeObj->SetPResActPayloadValue(0);

				for (INT32 indexLC = 0;
						indexLC < pdoIndexCollObj->GetNumberofIndexes();
						indexLC++)
				{

					Index *indexObjB4Sort = NULL;
					Index indexObj;
					indexObjB4Sort = pdoIndexCollObj->GetIndex(indexLC);
					if (!(CheckIfMappingPDO(
							(char*) indexObjB4Sort->GetIndexValue())))
					{
						continue;
					}
					else
					{
#if defined DEBUG
						cout << "index:"
						<< (char*) indexObjB4Sort->GetIndexValue()
						<< endl;
#endif
					}
					nodeMappedTotalBytes = 0;
					// bChangeOffset = false;

					if (indexObjB4Sort->GetNumberofSubIndexes() > 0)
					{

						/* Sort the pdo collection */
						indexObj = GetPDOIndexByOffset(indexObjB4Sort);

						INT32 sidxCount = 1;

						// Initialised to Zero and the value will be taken from the Actual value or the default value in priority
						INT32 sidxTotalCount = 0;
						//check whether the channel is activated
						SubIndex *sidxObjB4Sort = NULL;
						sidxObjB4Sort = indexObjB4Sort->GetSubIndexbyIndexValue(
								(char *) "00");
						if (NULL == sidxObjB4Sort)
						{
							continue;
						}
						// Actual value checked for Null, Empty, non-zero
						if ((sidxObjB4Sort->GetActualValue() != NULL)
								&& (0
										!= strcmp(
												sidxObjB4Sort->GetActualValue(),
												""))
								&& !(CheckIfValueZero(
										(char*) sidxObjB4Sort->GetActualValue())))
						{
							//value is not zero the channel is activated
							sidxTotalCount = GetDecimalValue(
									(char*) sidxObjB4Sort->GetActualValue());
#if defined DEBUG
							cout << "SiTotal:" << sidxTotalCount << endl;
#endif
						}
						else
						{
							if (0
									== strcmp(sidxObjB4Sort->GetActualValue(),
											""))
							{
								//pdo channel is deactivated. Empty act value
								continue;
							}
							if (CheckIfValueZero(
									(char*) sidxObjB4Sort->GetActualValue()))
							{
								// PDO channel is deactivated
								// Zero is not set here,as it is intialised to Zero previously
								continue;
							}
							else // If the Actual values is Null or Empty, Default value is set for Total SIdx for mapping
							{
								//No need to check for value null or empty. GetDecimalValue returns zero or particular value.
								sidxTotalCount =
										GetDecimalValue(
												(char*) sidxObjB4Sort->GetDefaultValue());
							}
						}

						//Check isiTotal value is valid
						if (sidxTotalCount
								>= (indexObjB4Sort->GetNumberofSubIndexes()))
						{
							exceptionObj.OCFMException(
									OCFM_ERR_MODULE_INDEX_NOT_FOUND);
							char acCustomError[200] =
							{ 0 };
							sprintf(acCustomError,
									"Mapping objects not found in index: %s in node: %d",
									(char*) indexObjB4Sort->GetIndexValue(),
									nodeObj->GetNodeId());
							CopyCustomErrorString(&(exceptionObj._ocfmRetCode),
									acCustomError);

							throw exceptionObj;
						}

						bool pdoResult = false;
						pdoResult = CheckPdoCommParam(indexObj.GetPDOType(), isBuild, &indexObj, indexCollObj, nodeObj);
						if (pdoResult == false)
						{
							//Incorrect Target node id for a PDO(may be a cross Tx). So do not process.
							continue;
						}

						while (sidxCount <= sidxTotalCount)
						{
							SubIndex *sidxObj = NULL;
							sidxObj = indexObj.GetSubIndex(sidxCount);
#if defined DEBUG	
							cout << "\n pobjSubIdx->getIndexValue():"
							<< sidxObj->GetIndexValue() << endl;
							cout << "\n pobjSubIdx->getName():"
							<< sidxObj->GetName() << endl;
#endif
							if ((NULL != sidxObj->GetActualValue())
									&& (0
											!= strcmp(sidxObj->GetActualValue(),
													"")))
							{

								const char* actualVal =
										sidxObj->GetActualValue();
								INT32 iLength = strlen(actualVal);
#if defined DEBUG
								cout<<"Length of the value: "<<iLength<<" Actual Value: "<<actualVal<<endl;
#endif
								//Actual pdo mapping value includes 16bit of original payload mapping and two for "0x"
								if (iLength != (16 + 2))
								{
									exceptionObj.OCFMException(OCFM_ERR_INVALID_VALUE);
									char customError[200] = { 0 };
									sprintf(customError, "The node '%s id: %d' has invalid mapping value for a PDO object %s / %s", nodeObj->GetNodeName(), nodeObj->GetNodeId(), indexObj.GetIndexValue(), sidxObj->GetIndexValue() );
									CopyCustomErrorString(&(exceptionObj._ocfmRetCode), customError);
									throw exceptionObj;
								}

								char* moduleIndex = NULL;
								moduleIndex = SubString((char*) actualVal,
										iLength - 4, 4);
								moduleIndex[5] = '\0';

								/* Get the SubIndex*/
								char* varSubIndex = NULL; // = new char[SUBINDEX_SIZE + ALLOC_BUFFER];
								//varSubIndex = subString(reverseValue,2,2);
								varSubIndex = SubString((char*) actualVal,
										iLength - 6, 2);
								varSubIndex[3] = '\0';

#if defined DEBUG	
								cout << "varModuleIndex:" << moduleIndex << "varSubIndex:" << varSubIndex << endl;
#endif

								//Mapped length in bits
								INT32 mappedLength = 0;
								mappedLength = HexToInt(
										SubString((char*) actualVal, 2, 4));
#if defined DEBUG
								cout<<" IntMapLength:"<<mappedLength<<endl;
#endif

								Index *moduleIndexObj = NULL;
								SubIndex *moduleSidxObj = NULL;
								char *uniqueidRefID = NULL;
								char *sidxName = NULL;
								char *accessStr = NULL;
								char *moduleName = NULL;
								DataType dtObj;
								dtObj.dataTypeName = NULL;
								bool objMapped = false;

								moduleIndexObj =
										indexCollObj->GetIndexbyIndexValue(
												moduleIndex);
								if (moduleIndexObj == NULL)
								{
									exceptionObj.OCFMException(
											OCFM_ERR_MODULE_INDEX_NOT_FOUND);
									char acCustomError[200] =
									{ 0 };
									sprintf(acCustomError,
											"In node id: %d, Index: %s which is mapped as a PDO module does not exist",
											nodeObj->GetNodeId(), moduleIndex);
									CopyCustomErrorString(
											&(exceptionObj._ocfmRetCode),
											acCustomError);

									throw exceptionObj;
								}

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
											accessStr =
													new char[strlen(
															moduleIndexObj->GetAccessType())
															+ ALLOC_BUFFER];
											strcpy(accessStr,
													moduleIndexObj->GetAccessType());

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
										exceptionObj.OCFMException(
												OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
										char acCustomError[200] =
										{ 0 };
										sprintf(acCustomError,
												"In node id: %d, Index: %s with SubIndex: %s which is mapped as a PDO module does not exist",
												nodeObj->GetNodeId(),
												moduleIndex, varSubIndex);
										CopyCustomErrorString(
												&(exceptionObj._ocfmRetCode),
												acCustomError);
										if (moduleName != NULL)
										{
											delete[] moduleName;
										}
										throw exceptionObj;
									}
									else
									{
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
												sidxName =
														new char[strlen(
																moduleSidxObj->GetName())
																+ ALLOC_BUFFER];
												strcpy(sidxName,
														moduleSidxObj->GetName());

												if (moduleSidxObj->GetAccessType() != NULL)
												{
													accessStr =
															new char[strlen(
																	moduleSidxObj->GetAccessType())
																	+ ALLOC_BUFFER];
													strcpy(accessStr,
															moduleSidxObj->GetAccessType());
												}
												dtObj =
														moduleSidxObj->GetDataType();
											}
										}
									}

								}

								PDOType pdoType = indexObj.GetPDOType();
								if ((NULL == moduleSidxObj->GetPDOMapping())
										|| (0
												== strcmp(
														moduleSidxObj->GetPDOMapping(),
														"")))
								{
									exceptionObj.OCFMException(
											OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO);
									char acCustomError[200] =
									{ 0 };
									sprintf(acCustomError,
											"Node: %d has invalid object mapped in %s / %s. \n The object %s / %s has no pdo mapping set",
											nodeObj->GetNodeId(),
											indexObj.GetIndexValue(),
											sidxObj->GetIndexValue(),
											moduleIndex, varSubIndex);
									CopyCustomErrorString(
											&(exceptionObj._ocfmRetCode),
											acCustomError);
									throw exceptionObj;
								}
								char* pdoMappingType = new char[strlen(
										moduleSidxObj->GetPDOMapping())
										+ STR_ALLOC_BUFFER];
								pdoMappingType = strcpy(pdoMappingType,
										moduleSidxObj->GetPDOMapping());

#if defined DEBUG
								cout<<"Mapping:"<<pdoMappingType<<" pdoType:"<<pdoType<<endl;
#endif

								if ((0
										== strcmp(
												ConvertToUpper(
														(char*) pdoMappingType),
												"TPDO"))
										&& (pdoType == PDO_TPDO))
								{
								}
								else if ((0
										== strcmp(
												ConvertToUpper(
														(char*) pdoMappingType),
												"RPDO"))
										&& (pdoType == PDO_RPDO))
								{
								}
								else
								{
									char* tempStr = new char[20
											+ STR_ALLOC_BUFFER];
									if (0
											== strcmp(
													ConvertToUpper(
															(char*) pdoMappingType),
													"TPDO"))
									{
										tempStr = strcpy(tempStr, "RPDO");
									}
									else if (0
											== strcmp(
													ConvertToUpper(
															(char*) pdoMappingType),
													"RPDO"))
									{
										tempStr = strcpy(tempStr, "TPDO");
									}
									else
									{
										if (pdoType == PDO_TPDO)
										{
											tempStr = strcpy(tempStr, "TPDO");
										}
										else
										{
											tempStr = strcpy(tempStr, "RPDO");
										}
										//nothing to do
									}

									exceptionObj.OCFMException(
											OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO);
									char acCustomError[200] =
									{ 0 };
									//Sample: In Node: 123  invalid object mapped in 1A00 / 01. The object 6000 / 00 has pdo mapping set to OPTIONAL. use an object which has pdo mapping TPDO.
									sprintf(acCustomError,
											"Node: %d has invalid object mapped in %s / %s. \n The object %s / %s has pdo mapping set to %s. Use an object which has pdo mapping %s.",
											nodeObj->GetNodeId(),
											indexObj.GetIndexValue(),
											sidxObj->GetIndexValue(),
											moduleIndex, varSubIndex,
											pdoMappingType, tempStr);

									CopyCustomErrorString(
											&(exceptionObj._ocfmRetCode),
											acCustomError);
									delete[] tempStr;
									delete[] pdoMappingType;
									throw exceptionObj;
								}
								delete[] pdoMappingType;
								if (uniqueidRefID != NULL)
								{
									INT32 totalBytesMapped = 0;
									totalBytesMapped =
											DecodeUniqueIDRef(uniqueidRefID,
													nodeObj, pdoType,
													(char*) moduleIndexObj->GetName(),
													(char*) moduleIndexObj->GetIndexValue());
#if defined DEBUG
									cout<<"iMappedLength:"<<mappedLength<<" totalBytesMapped:"<<totalBytesMapped<<endl;
#endif
									if (mappedLength != (totalBytesMapped * 8))
									{
										exceptionObj.OCFMException(
												OCFM_ERR_INVALID_SIZE_MAPPED);

										char acCustomError[200] =
										{ 0 };
										sprintf(acCustomError,
												"Invalid Length for the mapping object. Index: %s SubIndex: %s in node: %d",
												(char*) indexObj.GetIndexValue(),
												(char*) sidxObj->GetIndexValue(),
												nodeObj->GetNodeId());
										CopyCustomErrorString(
												&(exceptionObj._ocfmRetCode),
												acCustomError);

										throw exceptionObj;
									}
								}
								else
								{
									ProcessImage piObj;
									piObj.Initialize();
									if (dtObj.GetName() == NULL)
									{
										exceptionObj.OCFMException(
												OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
										throw exceptionObj;
									}
									else if (!CheckAllowedDTForMapping(
											dtObj.GetName()))
									{
										exceptionObj.OCFMException(
												OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
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
											moduleIndex) + STR_ALLOC_BUFFER];
									strcpy(piObj.moduleIndex, moduleIndex);

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

#if defined DEBUG
									cout<<"DataSize: "<<dtObj.dataSize*8<<" MappedLength: "<<mappedLength<<endl;
#endif
									if ((dtObj.dataSize * 8) != mappedLength)
									{
										exceptionObj.OCFMException(
												OCFM_ERR_INVALID_SIZE_MAPPED);
										char acCustomError[200] =
										{ 0 };
										sprintf(acCustomError,
												"Invalid Length for the mapping object. Index: %s SubIndex: %s in node: %d",
												(char*) indexObj.GetIndexValue(),
												(char*) sidxObj->GetIndexValue(),
												nodeObj->GetNodeId());
										CopyCustomErrorString(
												&(exceptionObj._ocfmRetCode),
												acCustomError);
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
										exceptionObj.OCFMException(
												OCFM_ERR_MAX_PI_SIZE);
										delete[] sidxName;
										delete[] accessStr;
										delete[] moduleName;
										throw exceptionObj;
									}
									/* Datatype in hex of the Process Image variable*/
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

								if ((true == isBuild)
										&& (strncmp(indexObj.GetIndexValue(),
												"16", 2) == 0)
										&& ((MN_NODEID == rpdoMappedNodeID)
												|| (BROADCAST_NODEID
														== rpdoMappedNodeID)))
								{
									char* modOffset = new char[strlen(actualVal)
											+ 1];
									strcpy(modOffset, actualVal);
									INT32 len = 0;
									char* tempLenStr = NULL;
									//extract the length mapped 1AXX
									tempLenStr = SubString((char *) actualVal,
											2, 4);
									len = HexToInt(tempLenStr);

									char* offsetStr = new char[5];
									memset(offsetStr, 0, 5 * sizeof(char));
									if (CHAINED == stnType)
									{
										offsetStr = IntToAscii(
												totalChainedBytesMapped,
												&(offsetStr[0]), 16);
									}
									else
									{
										offsetStr = IntToAscii(
												nodeMappedTotalBytes,
												&(offsetStr[0]), 16);
									}
									offsetStr = PadLeft(&(offsetStr[0]), '0',
											4);
									INT32 offsetCopyCount;
									for (offsetCopyCount = 0;
											offsetCopyCount <= 3;
											offsetCopyCount++)
									{
										modOffset[offsetCopyCount + 2 + 4] =
												offsetStr[offsetCopyCount];
									}
									strcpy(modOffset,
											ConvertToUpper(modOffset));

									indexCollObj->GetIndexbyIndexValue(
											(char *) indexObjB4Sort->GetIndexValue())->GetSubIndexbyIndexValue(
											(char *) sidxObj->GetIndexValue())->SetActualValue(
											modOffset);

									nodeMappedTotalBytes = nodeMappedTotalBytes
											+ len;

									if (CHAINED == stnType)
									{
										totalChainedBytesMapped =
												totalChainedBytesMapped + len;

									}

									delete[] modOffset;
									delete[] tempLenStr;
									delete[] offsetStr;
								}
							}
							else
							{
								if (NULL != sidxObj)
								{
									exceptionObj.OCFMException(OCFM_ERR_INVALID_VALUE);
									char customError[200] = { 0 };
									sprintf(customError, "In node '%s' id:%d, Empty actual value configured for the PDO object %s / %s", nodeObj->GetNodeName(), nodeObj->GetNodeId(), indexObj.GetIndexValue(), sidxObj->GetIndexValue());
									CopyCustomErrorString(&(exceptionObj._ocfmRetCode), customError);
									throw exceptionObj;
								}
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

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

void CalculatePayload()
{

	NodeCollection *nodeCollObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	/*Check RPDO Mapped objects*/

	if (nodeCollObj->GetCNNodesCount() == 0)
	{
		exit(0);
	}
	ocfmException exceptionObj;
	Node *nodeObj = NULL;
	Node *nodeObjMN = NULL;
	INT32 totalChainedBytesMapped = 0;
	INT32* nodeIdbyStnArranged = NULL;
	nodeIdbyStnArranged = ArrangeNodeIDbyStation();
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

			if (pdoIdxCollObj != NULL)
			{
				IndexCollection* indexCollObj = NULL;
				indexCollObj = nodeObj->GetIndexCollection();

				nodeObj->SetPReqActPayloadValue(0);
				nodeObj->SetPResActPayloadValue(0);

				for (INT32 idxLC = 0;
						idxLC < pdoIdxCollObj->GetNumberofIndexes(); idxLC++)
				{
					Index* indexObjB4Sort;
					Index indexObj;
					indexObjB4Sort = pdoIdxCollObj->GetIndex(idxLC);
					if (!(CheckIfMappingPDO(
							(char*) indexObjB4Sort->GetIndexValue())))
					{
						continue;
					}
					//	iNodeMappedTotalBytes = 0;

					if (indexObjB4Sort->GetNumberofSubIndexes() > 0)
					{
						/* Sort the pdo collection */
						indexObj = GetPDOIndexByOffset(indexObjB4Sort);
						INT32 sidxCount = 1;
						INT32 sidxTot = 0;

						SubIndex *sidxObj;
						sidxObj = indexObjB4Sort->GetSubIndexbyIndexValue(
								(char *) "00");
						if (NULL != sidxObj)
						{
							if ((sidxObj->GetActualValue() != NULL) // Actual value checked for Null
									&& (0
											!= strcmp(sidxObj->GetActualValue(),
													"")) // Actual value checked for Empty
									&& !(CheckIfValueZero(
											(char*) sidxObj->GetActualValue()))) // Actual value checked for non-zero
							{
								//value is not zero the channel is activated
								sidxTot = GetDecimalValue(
										(char*) sidxObj->GetActualValue());
#if defined DEBUG	
								cout << "iSiTotal:" << sidxTot << endl;
#endif
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

						}
						else
						{
							//no of entries index does not exist
							continue;
						}
						//Check isiTotal value is valid
						if (sidxTot
								>= (indexObjB4Sort->GetNumberofSubIndexes()))
						{
							exceptionObj.OCFMException(
									OCFM_ERR_MODULE_INDEX_NOT_FOUND);
							char acCustomError[200] =
							{ 0 };
							sprintf(acCustomError,
									"Mapping objects not found in index: %s in node: %d",
									(char*) indexObjB4Sort->GetIndexValue(),
									nodeObj->GetNodeId());
							CopyCustomErrorString(&(exceptionObj._ocfmRetCode),
									acCustomError);

							throw exceptionObj;
						}

						INT32 rpdoMappedNodeId = -1;
						if (strncmp(indexObj.GetIndexValue(), "16", 2) == 0)
						{
							Index *commIndexObj = NULL;
							char *indexId = SubString(
									(char *) indexObj.GetIndexValue(), 2, 4);
							char *commIdxId = new char[INDEX_LEN];
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

							SubIndex* subIndexObj;
							subIndexObj = indexObj.GetSubIndex(sidxCount);

							sidxCount++;

							if ((subIndexObj->GetActualValue() == NULL)
									|| (0
											== strcmp(
													subIndexObj->GetActualValue(),
													""))
									|| (CheckIfValueZero(
											(char*) subIndexObj->GetActualValue())))
							{
								continue;
							}

							const char* actualValueStr =
									subIndexObj->GetActualValue();

							if ((strncmp(indexObj.GetIndexValue(), "16", 2) == 0)
									&& ((MN_NODEID == rpdoMappedNodeId)
											|| (BROADCAST_NODEID
													== rpdoMappedNodeId)))
							{
								char* modOffsetVal = new char[strlen(
										actualValueStr) + 1];
								strcpy(modOffsetVal, actualValueStr);
								INT32 iLength = 0;
								INT32 iOffset = 0;

								char* lengthVal = NULL;
								lengthVal = SubString((char *) actualValueStr,
										2, 4);
								iLength = HexToInt(lengthVal);

								char* offsetVal = NULL;
								offsetVal = SubString((char *) actualValueStr,
										6, 4);
								iOffset = HexToInt(offsetVal);

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

								delete[] modOffsetVal;
								delete[] lengthVal;
								delete[] offsetVal;
							}
							if (strncmp(indexObj.GetIndexValue(), "1A", 2) == 0)
							{
								char* modOffset = new char[strlen(
										actualValueStr) + 1];
								strcpy(modOffset, actualValueStr);
								INT32 len = 0;
								char* lengthStr = NULL;
								lengthStr = SubString((char *) actualValueStr,
										2, 4);
								len = HexToInt(lengthStr);

								char* varOffset = NULL;
								varOffset = SubString((char *) actualValueStr,
										6, 4);
								INT32 offsetVal = 0;
								offsetVal = HexToInt(varOffset);

								nodeObj->SetPResActPayloadValue(
										(offsetVal + len) / 8);

								delete[] modOffset;
								delete[] lengthStr;
								delete[] varOffset;
							}
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
	delete[] nodeIdbyStnArranged;
}

INT32 GetCNDataLen(char* cdcBuffer)
{
	if (NULL == cdcBuffer)
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}
	INT32 count = 0;
	INT32 noOfChars = 0;
	INT32 counter = 0;
	INT32 length = 0;
	INT32 loopCount = 0;

	char readChar = 0;
	readChar = *(cdcBuffer);

	while (readChar != '\0')
	{
		readChar = *(cdcBuffer + loopCount);
		if (readChar == '/')
		{
			while (readChar != '\n')
			{
				readChar = *(cdcBuffer + loopCount);
				loopCount++;
			}
		}
		else if (readChar != '\t' || readChar != '\n' || readChar != ' ')
		{
			INT32 readCharVal = 0;
			// Convert to Upper case
			readCharVal = toupper(readChar);
			if ((readCharVal >= 65 && readCharVal <= 70)
					|| (readCharVal >= 97 && readCharVal <= 102))
			{
				//abCnObd[iNoOfChars] = iCharRead - 55;
				noOfChars++;
			}
			else if ((readCharVal >= 48 && readCharVal <= 57))
			{
				//abCnObd[iNoOfChars] = iCharRead - 48;
				noOfChars++;
			}
			else
			{
				//TODO: 'else' added. Operation to be specified
			}
			loopCount++;
		}
		else
		{
			//TODO: 'else' added. Operation to be specified
		}

		length = noOfChars;
		//For Byte Packing
		for (counter = 0, count = 0; counter < length; counter++, count++)
		{
			//abTempCnObd[iCount] = (UINT8)( ( abCnObd[ iCtr ] << 4 ) | abCnObd[ iCtr + 1 ] );
			counter++;
		}
	}
	return count;
}

//TODO: unused function
INT32 LenOfCNBuffer(char* cdcBuffer)
{
	char cTempChar;
	INT32 len = strlen(cdcBuffer);
	INT32 loopCount = 0;
	INT32 actualLength = 0;

	while (loopCount < len)
	{
		cTempChar = *(cdcBuffer + loopCount);
		if (cTempChar != '\n' && cTempChar != '\t')
		{
			actualLength++;
		}
		loopCount++;
	}
	return actualLength;

}

Index* GetMNIndexValues(char* indexId)
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
		else
		{
			//TODO: "else" Added.Operation need to be specified
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

			/* Add an attribute with name "Name" and value  to channel. */
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "Name", BAD_CAST piObj.name);

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
	bytesWritten = xmlTextWriterStartDocument(xmlWriter, NULL, MY_ENCODING,
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

ocfmRetCode GenerateXAP(char* xapFilePath)
{
	NodeCollection* nodeCollObj = NULL;
	ocfmException exceptionObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	char* xapFileName = new char[strlen(xapFilePath) + 4 + ALLOC_BUFFER];
	ProcessImage* piInCollObj = NULL;
	ProcessImage* piOutCollObj = NULL;
	try
	{
		if (nodeCollObj->GetNumberOfNodes() == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		if (nodeCollObj->GetCNNodesCount() == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_CN_NODES_FOUND);
			throw exceptionObj;
		}

		xmlTextWriterPtr xmlWriter = NULL;
		xmlDocPtr xmlDocObj = NULL;

		piInCollObj = new ProcessImage[PI_VAR_COUNT];
		piOutCollObj = new ProcessImage[PI_VAR_COUNT];
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
	} catch (ocfmException& ex)
	{
		delete[] xapFileName;
		return ex._ocfmRetCode;
	}
	delete[] xapFileName;
	return exceptionObj._ocfmRetCode;
}

ocfmRetCode GenerateNET(char* netFilePath)
{
	NodeCollection* nodeCollObj;
	ocfmException exceptionObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	ProcessImage* piInCollObj = NULL;
	ProcessImage* piOutCollObj = NULL;
	try
	{
		if (nodeCollObj->GetNumberOfNodes() == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		if (nodeCollObj->GetCNNodesCount() == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_CN_NODES_FOUND);
			throw exceptionObj;
		}

		piInCollObj = new ProcessImage[PI_VAR_COUNT];
		piOutCollObj = new ProcessImage[PI_VAR_COUNT];
		GroupInOutPIVariables(piInCollObj, piOutCollObj);
		/*Generate Dot NET Header file */
		GenerateNETHeaderFile(netFilePath, piInCollObj, piOutCollObj,
				inVarsGlobal, outVarsGlobal);
		delete[] piInCollObj;
		delete[] piOutCollObj;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return exceptionObj._ocfmRetCode;
}

ocfmRetCode GetIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId,
		AttributeType attributeType, char* outAttributeValue)
{
	INT32 indexPos;
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		//Index objIndex;
		Index* indexObj = NULL;

		//objIndex.SetNodeID(nodeObj.GetNodeId());
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		indexObj = indexCollObj->GetIndex(indexPos);
		indexObj->SetNodeID(nodeObj.GetNodeId());

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
			DataType tempDTObj;
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
			if (indexObj->GetFlagIfIncludedCdc() == TRUE)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;
		default:
			exceptionObj.OCFMException(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			throw exceptionObj;
		}
		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode GetIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos,
		AttributeType attributeType, char* outAttributeValue)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	try
	{
		// Check for the Existance of the Node in the iNodePos
		Node nodeObj;
		NodeCollection *nodeCollObj;
		IndexCollection *indexCollObj;
		Index* indexObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();

		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();
		if (nodePos >= nodeCount)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw exceptionObj;
		}
		else if (nodeCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();
		if (indexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}
		else if (indexPos >= indexCount)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		indexObj = indexCollObj->GetIndex(indexPos);

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
			DataType tempDTObj;
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
			if (indexObj->GetFlagIfIncludedCdc() == TRUE)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;
		default:
			errCodeObj.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
			return errCodeObj;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return errCodeObj;
}

ocfmRetCode GetSubIndexAttributes(INT32 nodeId, NodeType nodeType,
		char* indexId, char* sidxId, AttributeType attributeType,
		char* outAttributeValue)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	try
	{
		INT32 sidxPos = -1;
		INT32 indexPos = -1;
		errCodeObj = IfSubIndexExists(nodeId, nodeType, indexId, sidxId,
				&sidxPos, &indexPos);
		if (errCodeObj.code != OCFM_ERR_SUCCESS)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
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
		indexObj = indexCollObj->GetIndex(indexPos);
		indexObj->SetNodeID(nodeObj.GetNodeId());

		SubIndex* subIndexObj = NULL;
		subIndexObj = indexObj->GetSubIndex(sidxPos);

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
			DataType tempDTObj;
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
			if (subIndexObj->GetFlagIfIncludedCdc() == TRUE)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;
		default:
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXID);
			throw exceptionObj;
		}

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode GetSubIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos,
		INT32 subIndexPos, AttributeType attributeType, char* outAttributeValue)
{
	ocfmRetCode errCodeObj;

	try
	{
		// Check for the Existance of the Node in the iNodePos
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		Index* indexObj = NULL;
		ocfmException exceptionObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();

		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();
		if (nodePos >= nodeCount)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw exceptionObj;
		}
		else if (nodeCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();
		if (indexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}
		else if (indexPos >= indexCount)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		indexObj = indexCollObj->GetIndex(indexPos);

		INT32 subIndexCount = indexObj->GetNumberofSubIndexes();

		if (subIndexPos >= subIndexCount)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw exceptionObj;
		}
		else if (subIndexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		SubIndex* subIndexObj = NULL;
		subIndexObj = indexObj->GetSubIndex(subIndexPos);

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
			DataType tempDTObj;
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
			if (subIndexObj->GetFlagIfIncludedCdc() == TRUE)
				strcpy(outAttributeValue, "1");
			else
				strcpy(outAttributeValue, "0");
			break;

		default:
			errCodeObj.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
			return errCodeObj;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return errCodeObj;

}

//TODO: iMnNodeId not used. to be removed in header also
ocfmRetCode GetNodeCount(INT32 nodeId, INT32* outNodeCount)
{

	ocfmRetCode errCodeObj;
	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	NodeCollection *nodeCollObj = NULL;
	ocfmException exceptionObj;

	try
	{
		if (NULL == outNodeCount)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. Nothing to do.
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (nodeCollObj->GetNumberOfNodes() < 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added.  operation to be added
		}

		*outNodeCount = nodeCollObj->GetNumberOfNodes();

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

//TODO: unused function
//TODO: change the returning char* to delete varNodeIdStr
char* GetPIName(INT32 nodeID)
{
	char* nodeIdStr = NULL;
	char* nodeIdAsci = new char[2];

	nodeIdStr = new char[5];
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
	if (NULL == accessStr)
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	if (!strcmp(ConvertToUpper(accessStr), "READ"))
		return (char*) "ro\0";
	else if (!strcmp(ConvertToUpper(accessStr), "READWRITE"))
		return (char*) "rw\0";
	else
		return (char*) "xx";

}

ocfmRetCode GetIndexCount(INT32 nodeId, NodeType nodeType, INT32* outIndexCount)
{
	ocfmException exceptionObj;

	if (NULL == outIndexCount)
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *indexCollObj = NULL;
		bool nodeExist = false;

		INT32 nodePos;
		stErrorInfo = IfNodeExists(nodeId, nodeType, &nodePos, nodeExist);
		if (stErrorInfo.code == 0 && nodeExist == true)
		{
			//continue with process
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw exceptionObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
		indexCollObj = nodeObj.GetIndexCollection();

		*outIndexCount = indexCollObj->GetNumberofIndexes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

ocfmRetCode GetSubIndexCount(INT32 nodeId, NodeType nodeType, char* indexId,
		INT32* outSubIndexCount)
{
	ocfmException exceptionObj;

	if ((NULL == indexId) || (NULL == outSubIndexCount))
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		INT32 indexPos;
		errCodeObj = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (errCodeObj.code != OCFM_ERR_SUCCESS)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
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
		indexObj = indexCollObj->GetIndex(indexPos);
		indexObj->SetNodeID(nodeObj.GetNodeId());

		*outSubIndexCount = indexObj->GetNumberofSubIndexes();

		errCodeObj.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

void LoadObjectDictionary(char* xmlFilePath)
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

ocfmRetCode GetNodeAttributesbyNodePos(INT32 nodePos, INT32* outNodeId,
		char* outNodeName, StationType* outStationType, char* outForcedCycle,
		bool* outIsForcedCycle)
{
	ocfmException exceptionObj;

	if ((NULL == outNodeId) || (NULL == outNodeName) || (NULL == outStationType)
			|| (NULL == outForcedCycle) || (NULL == outIsForcedCycle))
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	INT32 nodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		errCodeObj = GetNodeCount(MN_NODEID, &nodeCount);

		if (nodePos > nodeCount)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw exceptionObj;
		}
		else if (nodeCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		Node nodeObj;
		NodeCollection *nodeCollObj;
		//IndexCollection *pobjIndexCollection;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();

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

		errCodeObj.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode GetIndexIDbyIndexPos(INT32 nodeId, NodeType nodeType,
		INT32 indexPos, char* outIndexId)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	Node nodeObj;
	NodeCollection *nodeCollObj;
	IndexCollection *indexCollObj;
	Index* indexObj;

	INT32 nodePos;
	try
	{
		bool existFlag = false;
		errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, existFlag);
		if (errCodeObj.code == 0 && existFlag == true)
		{
			//continue with process
		}
		else
		{
			// Node Doesn't Exist
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw exceptionObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();

		if (indexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}
		else if (indexCount < indexPos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		indexObj = indexCollObj->GetIndex(indexPos);

		if (indexObj->GetIndexValue() != NULL)
			strcpy(outIndexId, (char *) indexObj->GetIndexValue());

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode GetIndexIDbyPositions(INT32 nodePos, INT32 indexPos,
		char* outIndexID)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;
	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	Node nodeObj;
	NodeCollection *nodeCollObj;
	IndexCollection *indexCollObj;
	Index* indexObj;

	try
	{
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();

		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();

		if (nodeCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		else if (nodeCount < nodePos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 indexCount = indexCollObj->GetNumberofIndexes();

		if (indexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}
		else if (indexCount < indexPos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		indexObj = indexCollObj->GetIndex(indexPos);

		if (indexObj->GetIndexValue() != NULL)
			strcpy(outIndexID, (char *) indexObj->GetIndexValue());

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;

}

ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 nodeId, NodeType nodeType,
		char* indexId, INT32 subIndexPos, char* outSubIndexID)
{
	Node nodeObj;
	NodeCollection *nodeCollObj;
	IndexCollection *indexCollObj;
	//Index objIndex;
	Index* indexObj;
	ocfmRetCode stErrorInfo;
	ocfmException exceptionObj;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		INT32 indexPos;
		stErrorInfo = IfIndexExists(nodeId, nodeType, indexId, &indexPos);
		if (stErrorInfo.code != OCFM_ERR_SUCCESS)
		{
			exceptionObj.OCFMException(OCFM_ERR_INDEXID_NOT_FOUND);
			throw exceptionObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		indexCollObj = nodeObj.GetIndexCollection();
		indexObj = indexCollObj->GetIndex(indexPos);

		INT32 subIndexCount = indexObj->GetNumberofSubIndexes();
		if (subIndexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw exceptionObj;
		}
		else if (subIndexCount < subIndexPos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		SubIndex *sidxObj = NULL;
		sidxObj = indexObj->GetSubIndex(subIndexPos);

		if (sidxObj->GetIndexValue() != NULL)
			strcpy(outSubIndexID, (char *) sidxObj->GetIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
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
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	try
	{
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();

		INT32 nodeCount = nodeCollObj->GetNumberOfNodes();

		if (nodeCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		else if (nodeCount < nodePos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodePos);
		indexCollObj = nodeObj.GetIndexCollection();

		INT32 iTempIndexCount = indexCollObj->GetNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw exceptionObj;
		}
		else if (iTempIndexCount < indexPos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_INDEXPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		indexObj = indexCollObj->GetIndex(indexPos);

		INT32 subIndexCount = indexObj->GetNumberofSubIndexes();
		if (subIndexCount == 0)
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw exceptionObj;
		}
		else if (subIndexCount < subIndexPos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw exceptionObj;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		SubIndex* sidxObj = NULL;
		sidxObj = indexObj->GetSubIndex(subIndexPos);

		if (sidxObj->GetIndexValue() != NULL)
			strcpy(outSubIndexID, (char *) sidxObj->GetIndexValue());

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode DeleteNodeObjDict(INT32 nodeId, NodeType nodeType)
{
	ocfmRetCode errCodeObj;
	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	INT32 nodePos;

	try
	{
		ocfmException exceptionObj;
		bool existFlag = false;
		errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, existFlag);

		if (errCodeObj.code == 0 && existFlag == true)
		{
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEID);
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
		if (NULL == nodeCollObj)
		{
#if defined DEBUG
			cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

			exceptionObj.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw exceptionObj;
		}
		nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
		//objNode = pobjNodeCollection->getNode(varNodeType, iNodeID); // (varNodeType, iNodeID);
		indexObj.SetNodeID(nodeObj->GetNodeId());
		//objSubIndex.SetNodeID(objNode->GetNodeId());
		dataTypeCollObj = nodeObj->GetDataTypeCollection();

		indexCollObj = nodeObj->GetIndexCollection();
		nmtObj = nodeObj->GetNetworkManagement();
		appProcessObj = nodeObj->GetApplicationProcess();
		if ((NULL == appProcessObj) || (NULL == indexCollObj)
				|| (NULL == nmtObj) || (NULL == dataTypeCollObj))
		{
#if defined DEBUG
			cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

			exceptionObj.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw exceptionObj;
		}
		// Delete IndexCollection

		Index* indexObj1 = NULL;
		INT32 noOfIndex = indexCollObj->GetNumberofIndexes();

		for (INT32 indexLC = noOfIndex; indexLC < 0; indexLC--)
		{
			indexObj1 = indexCollObj->GetIndex(indexLC);
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
		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* exCatch)
	{
		return exCatch->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SaveProject(char* projectPath, char* projectName)
{
	Node nodeObj;
	NodeCollection *nodeCollObj = NULL;
	ocfmRetCode errCodeObj;
	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	char* tempPath;
	tempPath = new char[(strlen(projectPath)) + (strlen(projectName))
			+ strlen("cdc_xap") + ALLOC_BUFFER];

	try
	{
		char* tempPjtName;
		struct stat fileInfo;
		INT32 retStatus;

		tempPjtName = new char[300];
		sprintf(tempPjtName, "%s%s/%s.oct", projectPath, projectName,
				projectName);

		retStatus = stat(tempPjtName, &fileInfo);
		delete[] tempPjtName;
		if (retStatus == 0)
		{
#if defined DEBUG
			cout << "\n\n\nProject File Already exists\n" << endl;
#endif
		}
		else
		{
#if defined(_WIN32) && defined(_MSC_VER)
			{
				sprintf(tempPath, "%s\\%s", projectPath, projectName);
				_mkdir(tempPath);
			}
#else
			{
				sprintf(tempPath, "%s/%s", projectPath, projectName);
				mkdir(tempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
			}
#endif
		}

#if defined(_WIN32) && defined(_MSC_VER)
		{
			sprintf(tempPath, "%s\\%s\\%s", projectPath, projectName,
					"cdc_xap");
			_mkdir(tempPath);
		}
#else
		{
			sprintf(tempPath, "%s/%s/%s", projectPath, projectName, "cdc_xap");
			mkdir(tempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		}
#endif

		SaveProjectXML(projectPath, projectName);
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (nodeCollObj == NULL)
		{
			ocfmException exceptionObj;
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
		if (nodeCollObj->GetNumberOfNodes() > 0)
		{
			for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
					nodeLC++)
			{
				nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);
				char* fullFileName;
				fullFileName = new char[MAX_FILE_PATH_SIZE];
#if defined(_WIN32) && defined(_MSC_VER)
				{
					sprintf(tempPath, "%s\\%s\\octx", projectPath, projectName);
					struct stat tempFileInfo;

					retStatus = stat(tempPath, &tempFileInfo);
					if (retStatus == 0)
					{

#if defined DEBUG
						cout << "Folder Already Exists\n" << endl;
#endif
					}
					else
					{
						_mkdir(tempPath);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(fullFileName, "%s\\%d.octx", tempPath,
							nodeObj.GetNodeId());
				}
#else
				{
					struct stat fileInfo;

					sprintf(tempPath, "%s/%s/octx", projectPath, projectName);
					retStatus = stat(tempPath, &fileInfo);
					if(retStatus == 0)
					{
#if defined DEBUG
						cout << "Folder Already Exists\n" << endl;
#endif
					}
					else
					{
						mkdir(tempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(fullFileName, "%s/%d.octx", tempPath, nodeObj.GetNodeId());
				}
#endif				
				SaveNode(fullFileName, nodeObj.GetNodeId(),
						nodeObj.GetNodeType());
				delete[] fullFileName;

			}
			errCodeObj.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ocfmException exceptionObj;
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
	} catch (ocfmException* ex)
	{
		delete[] tempPath;
		return ex->_ocfmRetCode;
	}
	delete[] tempPath;
	return errCodeObj;
}

void GetMNPDOSubIndex(MNPdoVariable mnPdoVarObj, INT32& prevSubIndex,
		Index* indexObj, char* indexId, INT32 prevSize)
{
	ocfmException exceptionObj;

	if ((NULL == indexObj) || (NULL == indexId))
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	SubIndex* subIndexObj = NULL;
	char* tempIndexId = new char[3];
	/* Set the MN's PDO subIndex*/
	prevSubIndex = prevSubIndex + 1;
	tempIndexId = IntToAscii(prevSubIndex, tempIndexId, 16);
	tempIndexId = PadLeft(tempIndexId, '0', 2);

#if defined DEBUG
	cout << " idx" << tempIndexId << endl;
	cout << " prevsubindex" << prevSubIndex << endl;
#endif
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
	if (NULL == subIndexObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_UNKNOWN);
		cout << "pobjSubIndex NULL:" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}
	/* Calculate the actual value of MN PDO */
	char* actValue = new char[20];
	strcpy(actValue, SubString(mnPdoVarObj.value, 0, 6));
	char* offsetVal = new char[5];

	offsetVal = IntToAscii(prevSize, offsetVal, 16);
	offsetVal = PadLeft(offsetVal, '0', 4);
	strcpy(offsetVal, ConvertToUpper(offsetVal));
	strcat(actValue, offsetVal);
	/* Add reserve byte*/
	strcat(actValue, "00");

	strcat(actValue, mnPdoVarObj.subIndexId);
	strcat(actValue, mnPdoVarObj.indexId);

	subIndexObj->SetActualValue(actValue);
#if defined DEBUG
	cout<<"Actual Value"<<actValue<<endl;
#endif
	AddPDOIndexsToMN(mnPdoVarObj.indexId, mnPdoVarObj.subIndexId,
			mnPdoVarObj.pdoType);
	delete[] offsetVal;
	delete[] actValue;
}

void SetSIdxValue(char* indexId, char* sidxId, char* value,
		IndexCollection *indexCollObj, INT32 nodeId, NodeType nodeType,
		bool setDefaultValue)
{
	//TODO: varValue (3rd parameter) to be null checked. Empty value sent in generateotherMNindexes function
	if ((NULL == indexId) || (NULL == sidxId) || (NULL == indexCollObj))
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	Index *indexObj = NULL;
	SubIndex* sidxObj = NULL;
	ocfmRetCode stRetInfo;

	indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
#if defined DEBUG	
	cout << "sidx idx" << indexId << sidxId << endl;

#endif
	if (CheckIfSubIndexExists(nodeId, nodeType, indexId, sidxId))
	{
#if defined DEBUG	
		cout << "subindex fetched" << endl;
#endif			
		sidxObj = indexObj->GetSubIndexbyIndexValue(sidxId);
		if (setDefaultValue)
			sidxObj->SetActualValue((char*) sidxObj->GetDefaultValue());
		else
			sidxObj->SetActualValue(value);
		sidxObj->SetFlagIfIncludedCdc(TRUE);
	}
	else
	{
#if defined DEBUG	
		cout << "call addindex" << endl;
#endif
		stRetInfo = AddSubIndex(nodeId, nodeType, indexId, sidxId);
#if defined DEBUG	
		cout << "retcode" << stRetInfo.code << endl;
		cout << "Index sidx added" << indexId << sidxId;
#endif
		sidxObj = indexObj->GetSubIndexbyIndexValue(sidxId);
		sidxObj->SetFlagIfIncludedCdc(TRUE);
		if (setDefaultValue)
			sidxObj->SetActualValue((char*) sidxObj->GetDefaultValue());
		else
		{
#if defined DEBUG	
			cout << "value" << value << endl;
			cout << "subindex index" << sidxObj->GetIndexValue();
#endif
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
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	//pobjIndex = pobjIdxCol->getIndexbyIndexValue(varIdx);
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	// to doStack wasnt booting up with sub index 00 for 1C09, 1F26
	if ((strcmp("1C09", indexId) == 0) || (strcmp("1F26", indexId) == 0)
			|| (strcmp("1F8B", indexId) == 0) || (strcmp("1F8D", indexId) == 0)
			|| (strcmp("1F27", indexId) == 0) || (strcmp("1F84", indexId) == 0))
	{
		strcpy(sidxId, "00");
		errCodeObj = DeleteSubIndex(nodeId, MN, indexId, sidxId);
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

//TODO: unused function
ocfmRetCode AddOtherMNIndexes(INT32 nodeID)
{
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	char* indexId = new char[INDEX_LEN];
	char* sidxId = new char[SUBINDEX_LEN];
	Index* indexObj = NULL;
	IndexCollection* indexCollObj = NULL;

	Node *nodeObj;
	NodeCollection *nodeCollObj = NULL;

	try
	{
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNodePtr(MN, nodeID);
		indexCollObj = nodeObj->GetIndexCollection();

		/* Add 1006*/
		strcpy(indexId, "1006");
#if defined DEBUG	
		cout << "string copied" << endl;
#endif
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
#if defined DEBUG	
		cout << "retcode" << errCodeObj.code << endl;
		cout << "1006 added" << endl;
#endif

		/* Set 5ms value*/
		SetBasicIndexAttributes(MN_NODEID, MN, indexId, (char*) "50000",
				(char*) "NMT_CycleLen_U32", TRUE);

		/* Add 1020*/
		strcpy(indexId, "1020");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);

		/* Add 1020*/
		strcpy(indexId, "1300");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		/* $:To do by M hard coded*/
		SetBasicIndexAttributes(MN_NODEID, MN, indexId, (char*) "5000",
				(char*) "SDO_SequLayerTimeout_U32", TRUE);

		/* Add 1C02*/
		strcpy(indexId, "1C02");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);

		if (errCodeObj.code == OCFM_ERR_SUCCESS
				|| errCodeObj.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);
			/* Set subindex value 40 or 0000028 */
			strcpy(sidxId, "00");
			SetSIdxValue(indexId, sidxId, (char*) "3", indexCollObj,
					nodeObj->GetNodeId(), MN, false);
#if defined DEBUG	
			cout << "1c02 subidex added" << endl;
#endif

			strcpy(sidxId, "01");
			SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
					nodeObj->GetNodeId(), MN, false);
#if defined DEBUG	
			cout << "1c02 subidex 01 added" << endl;
#endif
			strcpy(sidxId, "02");
			SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
					nodeObj->GetNodeId(), MN, false);

			strcpy(sidxId, "03");
			SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
					nodeObj->GetNodeId(), MN, false);

		}

		/* Add 1C09*/
		strcpy(indexId, "1C09");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		if (errCodeObj.code == OCFM_ERR_SUCCESS
				|| errCodeObj.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);

			AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(),
					(char*) "40", false);

		}

		/* Add 1F26*/
		strcpy(indexId, "1F26");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		if (errCodeObj.code == OCFM_ERR_SUCCESS
				|| errCodeObj.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);

			char* value = new char[8];
			configDateGlobal = GetConfigDate();
			value = IntToAscii(configDateGlobal, value, 10);
			//hexVal = padLeft(hexVal, '0' , 8);
			strcpy(value, ConvertToUpper(value));
			AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), value,
					false);

			//AddForEachSIdx(varMNIndex, pobjIdxCol, pobjNode->getNodeId(), hexVal, false);
			delete[] value;
		}

#if defined DEBUG	
		cout << "1F26 subidex added" << endl;
#endif
		/* Add 1F27*/
		strcpy(indexId, "1F27");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		if (errCodeObj.code == OCFM_ERR_SUCCESS
				|| errCodeObj.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);

			char* value = new char[50];
			configTimeGlobal = GetConfigTime();
			value = IntToAscii(configTimeGlobal, value, 10);
			strcpy(value, ConvertToUpper(value));
			AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), value,
					false);
			delete[] value;
		}

		/* Add 1F84*/
		strcpy(indexId, "1F84");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		if (errCodeObj.code == OCFM_ERR_SUCCESS
				|| errCodeObj.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);

			AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(),
					(char*) "", true);

		}

		/* Add 1F89*/
		strcpy(indexId, "1F89");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		if (errCodeObj.code == OCFM_ERR_SUCCESS
				|| errCodeObj.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);
			strcpy(sidxId, "02");
			SetSIdxValue(indexId, sidxId, (char*) " ", indexCollObj,
					nodeObj->GetNodeId(), MN, true);

		}

		/* Add 1F8A*/
		strcpy(indexId, "1F8A");
		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		if (errCodeObj.code == OCFM_ERR_SUCCESS
				|| errCodeObj.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);
			strcpy(sidxId, "00");
			SetSIdxValue(indexId, sidxId, (char*) " ", indexCollObj,
					nodeObj->GetNodeId(), MN, true);

			//Todo By M as subindex 01 shud be equal to 02, need to find the reason 
			strcpy(sidxId, "01");
			SetSIdxValue(indexId, sidxId, (char*) "100000", indexCollObj,
					nodeObj->GetNodeId(), MN, false);

			strcpy(sidxId, "02");
			SetSIdxValue(indexId, sidxId, (char*) " ", indexCollObj,
					nodeObj->GetNodeId(), MN, true);

		}

		/* Add 1F98*/
		strcpy(indexId, "1F98");

		errCodeObj = AddIndex(MN_NODEID, MN, indexId);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{

			indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
			/* $:set Flag to true*/
			indexObj->SetFlagIfIncludedCdc(TRUE);

			strcpy(sidxId, "05");
			SetSIdxValue(indexId, sidxId, abC_DLL_ISOCHR_MAX_PAYL, indexCollObj,
					nodeObj->GetNodeId(), MN, false);

		}

		delete[] indexId;
		delete[] sidxId;
	} catch (ocfmException & ex)
	{
		delete[] indexId;
		delete[] sidxId;
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode GenerateMNOBD()
{
	return (GenerateMNOBD(false));
}

ocfmRetCode GenerateMNOBD(bool IsBuild)
{

	Node *nodeObjMN = NULL;
	NodeCollection *nodeCollObj = NULL;
	IndexCollection *indexCollObj = NULL;

	char* indexIdMN = new char[INDEX_LEN];
	char* sidxId = new char[SUBINDEX_LEN];
	char* mappingSidxId = new char[SUBINDEX_LEN];

	ocfmRetCode errCodeObj;

	ocfmException exceptionObj;
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
			exceptionObj.OCFMException(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			throw exceptionObj;
		}
		else
		{
			/*Process PDO Nodes*/
			errCodeObj = ProcessPDONodes(IsBuild);
#if defined DEBUG
			cout << "PDO's in CN Nodes Processed" << endl;
#endif
			if (errCodeObj.code != OCFM_ERR_SUCCESS)
			{
				return errCodeObj;
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
#if defined DEBUG
			cout << "Max Number Of TPDO Channels" << maxNoOfChannels << endl;
#endif

			/* Delete the MN's old object dictionary*/
			pobjIndexCollection = nodeObjMN->GetIndexCollection();
//DO not delete and try to reset PDO indexes.
			//			pobjIndexCollection->DeletePDOs();
			ResetAllPdos(nodeObjMN->GetNodeId(), nodeObjMN->GetNodeType());
#if defined DEBUG
			//cout << "MN Node PDO's deleted" << endl;
#endif
			// Delete Process Image Objects
			pobjIndexCollection->DeletePIObjects();
#if defined DEBUG
			cout << "Deleted PI objects (Axxx indexes) in MN" << endl;
#endif
			// Autogenertate other indexs 
			AuotgenerateOtherIndexs(nodeObjMN);
			/* Add other Indexes than PDO*/
#if defined DEBUG
			cout << "Auotgenerated Other Indexs in MN" << endl;
#endif
		}

		bool isPresMnVal = false;
		isPresMnVal = IsPresMN();

		if (true == isPresMnVal)
		{
#if defined DEBUG
			cout << "PresMN: iTxChannelCount set to 1" << endl;
#endif
			txChannelCount = 1;
		}
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObjMN = nodeCollObj->GetNodePtr(MN, MN_NODEID);

		INT32* arrangedNodeIDbyStation = NULL;
		Node nodeObj;
		arrangedNodeIDbyStation = ArrangeNodeIDbyStation();
#if defined DEBUG
			cout << "NodeID Aarranged by Station" << endl;
#endif
		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{
			nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);

			StationType currCNStation;
			if (nodeObj.GetNodeType() == CN)
			{

				char* versionNumber = new char[4];
				versionNumber[0] = 0;
				currCNStation = nodeObj.GetStationType();

				nodeObjMN = nodeCollObj->GetNodePtr(MN, MN_NODEID);
				indexCollObj = nodeObjMN->GetIndexCollection();

				if (nodeObj.MNPDOOUTVarCollection.Count() != 0)
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
					/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
					Index* indexObj;
					indexPos = 0;
					char* mappNodeID = new char[SUBINDEX_LEN];
					strcpy(indexIdMN, "18");
					if (CHAINED != currCNStation)
					{
						sidxId = IntToAscii(txChannelCount, sidxId, 16);
						txChannelCount++;

						sidxId = PadLeft(sidxId, '0', 2);
						indexIdMN = strcat(indexIdMN, sidxId);
						errCodeObj = IfIndexExists(MN_NODEID, MN, indexIdMN,
								&indexPos);
						if (errCodeObj.code != OCFM_ERR_SUCCESS)
						{
#if defined DEBUG
							cout<<"AddIndex: "<<indexIdMN<<__LINE__<<endl;
#endif
							errCodeObj = AddIndex(MN_NODEID, MN, indexIdMN);
						}

						//to write cn node id in 18XX/01
						mappNodeID = IntToAscii(nodeObj.GetNodeId(), mappNodeID,
								10);
					}
					else
					{
						//1800 is used of PRes chained station
						strcpy(indexIdMN, (char *) "1800");
						strcpy(sidxId, (char *) "00");
						errCodeObj = IfIndexExists(MN_NODEID, MN, indexIdMN,
								&indexPos);
						if (errCodeObj.code != OCFM_ERR_SUCCESS)
						{
#if defined DEBUG
							cout<<"AddIndex: "<<indexIdMN<<__LINE__<<endl;
#endif
							errCodeObj = AddIndex(MN_NODEID, MN, indexIdMN);
						}

						//to write 0 in 18XX/01 to indicate PRes MN
						strcpy(mappNodeID, (char *) "0x0");
					}
					/* set bFlag to true for 1800*/
					indexObj = indexCollObj->GetIndexbyIndexValue(indexIdMN);

					if (errCodeObj.code != OCFM_ERR_SUCCESS)
					{
						exceptionObj.OCFMException(errCodeObj.code);
						delete[] arrangedNodeIDbyStation;
						throw exceptionObj;
					}

					char* tempSidxId = new char[SUBINDEX_LEN];
					strcpy(tempSidxId, "01");
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN,
							tempSidxId, mappNodeID, (char*) "NodeID_U8", TRUE);
					delete[] mappNodeID;

					GetSubIndexAttributes(nodeObj.GetNodeId(), CN,
							(char*) "1400", (char*) "02", ACTUALVALUE,
							versionNumber);
					if ((NULL == versionNumber)
							|| (strcmp(versionNumber, "") == 0))
					{
						strcpy(versionNumber, "0x0");
					}
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN,
							(char*) "02", versionNumber,
							(char*) "MappingVersion_U8", TRUE);

					strcpy(indexIdMN, "1A");
					strcat(indexIdMN, sidxId);
					/* Set the MN's PDO Index*/
					indexPos = 0;
					errCodeObj = IfIndexExists(MN_NODEID, MN, indexIdMN,
							&indexPos);
					if (errCodeObj.code != OCFM_ERR_SUCCESS)
					{
#if defined DEBUG
						cout<<"AddIndex: "<<indexIdMN<<__LINE__<<endl;
#endif
						errCodeObj = AddIndex(MN_NODEID, MN, indexIdMN);
					}

					indexObj->SetFlagIfIncludedCdc(TRUE);

					if (errCodeObj.code != OCFM_ERR_SUCCESS)
					{
						exceptionObj.OCFMException(errCodeObj.code);
						delete[] arrangedNodeIDbyStation;
						throw exceptionObj;
					}
					INT32 pdoOutLC = 0;
					for (pdoOutLC = 0;
							pdoOutLC < nodeObj.MNPDOOUTVarCollection.Count();
							pdoOutLC++)
					{
						MNPdoVariable mnPDOObj;
						mnPDOObj = nodeObj.MNPDOOUTVarCollection[pdoOutLC];
						indexObj = indexCollObj->GetIndexbyIndexValue(
								indexIdMN);

						if (indexObj != NULL)
						{
							indexObj->SetFlagIfIncludedCdc(TRUE);
							GetMNPDOSubIndex(mnPDOObj, prevSubIndex, indexObj,
									indexIdMN, outPrevSize);
							outPrevSize = outPrevSize + mnPDOObj.dataSize;
						}
					}
					SubIndex *sidxMNobj = NULL;
					sidxMNobj = indexObj->GetSubIndexbyIndexValue((char*) "00");
					if (NULL != sidxMNobj)
					{
						char *tempStr = new char[INDEX_LEN];
						tempStr = IntToAscii(pdoOutLC, tempStr, 10);
#if defined DEBUG
						cout<<"Setting Actual Value: "<<tempStr<<" in index: "<<indexObj->GetIndexValue()<<endl;
#endif
						sidxMNobj->SetActualValue(tempStr);
						delete[] tempStr;
					}

					if (CHAINED == currCNStation)
					{
						chainOutPrevSubIndex = prevSubIndex;
						chainOutPrevSize = outPrevSize;
					}
				}

				if (nodeObj.MNPDOINVarCollection.Count() != 0)
				{
					/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
					Index* indexObjTemp;
					strcpy(indexIdMN, "14");
					sidxId = IntToAscii(rxChannelCount, sidxId, 16);
					rxChannelCount++;

					sidxId = PadLeft(sidxId, '0', 2);
					indexIdMN = strcat(indexIdMN, sidxId);
					errCodeObj = IfIndexExists(MN_NODEID, MN, indexIdMN,
							&indexPos);
					if (errCodeObj.code != OCFM_ERR_SUCCESS)
					{
#if defined DEBUG
						cout<<"AddIndex: "<<indexIdMN<<__LINE__<<endl;
#endif
						errCodeObj = AddIndex(MN_NODEID, MN, indexIdMN);
						/* set bFlag to true for 1800*/
					}
					indexObjTemp = indexCollObj->GetIndexbyIndexValue(
							indexIdMN);
					if (indexObjTemp != NULL)
						indexObjTemp->SetFlagIfIncludedCdc(TRUE);

					inPrevSubIndex = 0;
					inPrevSize = 0;
					if (errCodeObj.code != OCFM_ERR_SUCCESS)
					{
						exceptionObj.OCFMException(errCodeObj.code);
						delete[] arrangedNodeIDbyStation;
						throw exceptionObj;
					}

					mappingSidxId = IntToAscii((nodeObj.GetNodeId()),
							mappingSidxId, 10);
					char* tempSidxId = new char[SUBINDEX_LEN];
					strcpy(tempSidxId, "01");
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN,
							tempSidxId, mappingSidxId, (char*) "NodeID_U8",
							TRUE);

					GetSubIndexAttributes(nodeObj.GetNodeId(), CN,
							(char*) "1800", (char*) "02", ACTUALVALUE,
							versionNumber);
					if ((NULL == versionNumber)
							|| (strcmp(versionNumber, "") == 0))
					{
						strcpy(versionNumber, "0x0");
					}
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexIdMN,
							(char*) "02", versionNumber,
							(char*) "MappingVersion_U8", TRUE);
					delete[] tempSidxId;

					strcpy(indexIdMN, "16");
					strcat(indexIdMN, sidxId);
					/* Set the MN's PDO Index*/
					errCodeObj = IfIndexExists(MN_NODEID, MN, indexIdMN,
							&indexPos);
					if (errCodeObj.code != OCFM_ERR_SUCCESS)
					{
						cout << "AddIndex: " << indexIdMN << __LINE__ << endl;
						errCodeObj = AddIndex(MN_NODEID, MN, indexIdMN);
						if (errCodeObj.code != OCFM_ERR_SUCCESS)
						{
							exceptionObj.OCFMException(errCodeObj.code);
							delete[] arrangedNodeIDbyStation;
							throw exceptionObj;
						}
					}

					indexObjTemp = indexCollObj->GetIndexbyIndexValue(
							indexIdMN);
					INT32 pdoInLC = 0;
					for (pdoInLC = 0;
							pdoInLC < nodeObj.MNPDOINVarCollection.Count();
							pdoInLC++)
					{
						MNPdoVariable mnPDOobj;
						mnPDOobj = nodeObj.MNPDOINVarCollection[pdoInLC];
						indexObjTemp = indexCollObj->GetIndexbyIndexValue(
								indexIdMN);
						indexObjTemp->SetFlagIfIncludedCdc(TRUE);
						GetMNPDOSubIndex(mnPDOobj, inPrevSubIndex, indexObjTemp,
								indexIdMN, inPrevSize);
						inPrevSize = inPrevSize + mnPDOobj.dataSize;
					}

					SubIndex *sidxObjtemp = NULL;
					sidxObjtemp = indexObjTemp->GetSubIndexbyIndexValue(
							(char*) "00");
					if (NULL != sidxObjtemp)
					{
						char *temp = new char[INDEX_LEN];
						temp = IntToAscii(pdoInLC, temp, 10);
#if defined DEBUG
						cout<<"Setting Actual Value: "<<temp<<" in index: "<<indexObjTemp->GetIndexValue()<<endl;
#endif
						//itoa(PdoInCount, temp, 16);
						sidxObjtemp->SetActualValue(temp);
						delete[] temp;
					}
				}
				delete[] versionNumber;
			}
		}
		SetPresMNNodeAssigmentBits();

		delete[] arrangedNodeIDbyStation;
		SetFlagForRequiredMNIndexes(MN_NODEID);
	}

	catch (ocfmException& exObj)
	{
		return exObj._ocfmRetCode;
	}

	if (txChannelCount > maxNoOfChannels)
	{
		errCodeObj.code = OCFM_ERR_EXCESS_CHANNEL;
	}
	else
	{
		errCodeObj.code = OCFM_ERR_SUCCESS;
	}
	delete[] indexIdMN;
	return errCodeObj;
}

//TODO: unused function
void CheckIfOffsetUsed(INT32& iOffset, PDOType varPdoType)
{
	NodeCollection* pobjNodeCol;
	pobjNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode;

	for (INT32 iNodeIndex = 0; iNodeIndex < pobjNodeCol->GetNumberOfNodes();
			iNodeIndex++)
	{
		objNode = pobjNodeCol->GetNodebyCollectionIndex(iNodeIndex);
		if (objNode.GetNodeType() == CN)
		{
			if (varPdoType == PDO_RPDO)
			{
				for (INT32 iLoopCount = 0;
						iLoopCount < objNode.MNPDOOUTVarCollection.Count();
						iLoopCount++)
				{
					if (objNode.MNPDOOUTVarCollection[iLoopCount].offsetVal
							== iOffset)
						iOffset =
								iOffset
										+ objNode.MNPDOOUTVarCollection[iLoopCount].dataSize
												/ 8;
				}
			}
			else if (varPdoType == PDO_TPDO)
			{
				for (INT32 iLoopCount = 0;
						iLoopCount < objNode.MNPDOINVarCollection.Count();
						iLoopCount++)
				{
					if (objNode.MNPDOINVarCollection[iLoopCount].offsetVal
							== iOffset)
						iOffset =
								iOffset
										+ objNode.MNPDOINVarCollection[iLoopCount].dataSize
												/ 8;
				}
			}
			else
			{
				//TODO: 'else' added. operation to be specified.
			}
		}
	}

}

//TODO: No need of varPdotype for size calculation. to be removed in header
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
		cout << "Undefined DataSize Encountered:" << dataSize <<" in "<< __FUNCTION__ << endl;
		break;
	}
	return retOffset;
}

//TODO: No need of varPdotype for size calculation. to be removed in header
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
		cout << "Undefined DataSize Encountered:" << __FUNCTION__ << endl;
		break;
	}
	return retOffset;
}

ocfmRetCode FreeProjectMemory()
{
	NodeCollection *nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	delete nodeCollObj;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

ocfmRetCode OpenProject(char* projectPath, char* projectFileName)
{
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	xmlTextReaderPtr xmlReader;
	char *tempFileName = NULL;

#if defined DEBUG
	cout << "\nStrLen for FileName:"
	<< (strlen(projectPath) + strlen(projectFileName) + 1) << endl;
#endif

	tempFileName =
			new char[(strlen(projectPath) + strlen(projectFileName) + 5)];
#if defined(_WIN32) && defined(_MSC_VER)
	{
		sprintf(tempFileName, "%s\\%s", projectPath, projectFileName);
	}
#else
	{
		sprintf(tempFileName, "%s/%s", projectPath, projectFileName);
	}
#endif

	xmlReader = xmlReaderForFile(tempFileName, NULL, 0);
	delete[] tempFileName;

	ocfmException exceptionObj;

	try
	{
		if (xmlReader != NULL)
		{
			INT32 retVal;
			retVal = xmlTextReaderRead(xmlReader);
			while (retVal == 1)
			{
				ProcessProjectXML(xmlReader, projectPath);
				retVal = xmlTextReaderRead(xmlReader);
			}
			if (retVal != 0)
			{
#if defined DEBUG 
				cout << "\nOCFM_ERR_PARSE_XML\n" << endl;
#endif
				exceptionObj.OCFMException(OCFM_ERR_PARSE_XML);
				throw exceptionObj;
			}
		}
		else
		{
#if defined DEBUG 
			cout << "\nOCFM_ERR_CANNOT_OPEN_FILE\n" << endl;
#endif
			exceptionObj.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw exceptionObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();

		char* presTimeoutVal = new char[50];
		presTimeoutVal[0] = 0;

		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
				nodeLC++)
		{
			Node nodeObj;
			INT32 nodeId;
			NodeType nodeType;
			nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);

			nodeType = nodeObj.GetNodeType();
			nodeId = nodeObj.GetNodeId();
			CopyPDODefToAct(nodeId, nodeType);
			CopyMNPropDefToAct(nodeId, nodeType);

			if ((nodeId != MN_NODEID) && (MN != nodeType))
			{
				char* value = NULL;
				value = new char[SUBINDEX_LEN];
				value = IntToAscii(nodeId, value, 16);
				value = PadLeft(value, '0', 2);
				GetSubIndexAttributes(MN_NODEID, MN, (char*) "1f92", value,
						ACTUALVALUE, presTimeoutVal);
#if defined DEBUG
				cout << "Actual Value" << nodeId << presTimeoutVal << endl;
#endif
				if (((NULL == presTimeoutVal)
						|| (strcmp(presTimeoutVal, "") == 0))
						|| (!(ValidateCNPresTimeout(value, presTimeoutVal))))
				{
					CalculateCNPollResponse(nodeId, nodeType);
				}

				delete[] value;
			}
		}
		delete[] presTimeoutVal;
	}

	catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return errCodeObj;
}

ocfmRetCode ProcessProjectXML(xmlTextReaderPtr xmlReader, char* projectPath)
{
	const xmlChar *xmlName;
	ocfmException exceptionObj;

	xmlName = xmlTextReaderConstName(xmlReader);
	if (xmlName == NULL)
	{
#if defined DEBUG
		cout << "\nGot NULL for Name\n" << endl;
#endif
	}
	//value = xmlTextReaderConstValue(pxReader);
	try
	{
		if (xmlTextReaderNodeType(xmlReader) == 1)
		{
			// Check for openCONFIGURATOR Tag
			if (strcmp(((char*) xmlName), "openCONFIGURATOR") == 0)
			{
#if defined DEBUG
				cout << "openCONFIGURATOR Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(xmlReader) == 1)
				{
					while (xmlTextReaderMoveToNextAttribute(xmlReader))
					{
						// Call Check Version number Fn
						if (IfVersionNumberMatches(xmlReader) == false)
						{
#if defined DEBUG
							cout << "openCONFIGURATOR Tag present\n" << endl;
#endif
							exceptionObj.OCFMException(
									OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH);
							throw exceptionObj;
						}
					}
				}
			}
			else if (strcmp(((char*) xmlName), "profile") == 0)
			{
#if defined DEBUG
				cout << "profile Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(xmlReader) == 1)
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw exceptionObj;
				}
			}
			else if (strcmp(((char*) xmlName), "Auto") == 0)
			{
#if defined DEBUG
				cout << "Auto Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(xmlReader) == 1)
				{
					if (SetProjectSettingsAuto(xmlReader) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
						throw exceptionObj;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw exceptionObj;
				}
			}
			else if (strcmp(((char*) xmlName), "Communication") == 0)
			{
#if defined DEBUG
				cout << "Communication Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(xmlReader) == 1)
				{
					if (SetProjectSettingsCommunication(xmlReader) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
						throw exceptionObj;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw exceptionObj;
				}
			}
			else if (strcmp(((char*) xmlName), "NodeCollection") == 0)
			{
#if defined DEBUG
				cout << "NodeCollection Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(xmlReader) == 1)
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw exceptionObj;
				}
			}
			else if (strcmp(((char*) xmlName), "Node") == 0)
			{
				if (xmlTextReaderHasAttributes(xmlReader) == 1)
				{
					if (GetandCreateNode(xmlReader, projectPath) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
						throw exceptionObj;
					}
					else
					{
						//ocfmRetCode stErrorInfo;
						//stErrorInfo.code = OCFM_ERR_SUCCESS;
						//return stErrorInfo;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					exceptionObj.OCFMException(OCFM_ERR_INVALID_PJTXML);
					throw exceptionObj;
				}
			}
		}

	} catch (ocfmException* exObj)
	{
		return exObj->_ocfmRetCode;
	}

	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return errCodeObj;
}

bool SetProjectSettingsAuto(xmlTextReaderPtr xmlReader)
{
	PjtSettings* pjtSettingsObj;
	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();

	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const xmlChar* xmlName;
		const xmlChar* xmlValue;
		//Retrieve the pxcName and Value of an attribute	
		xmlValue = xmlTextReaderConstValue(xmlReader);
		xmlName = xmlTextReaderConstName(xmlReader);

		if (xmlValue == NULL || xmlName == NULL)
			return false;
#if defined DEBUG
		cout << "\nName:" << xmlName << endl;
		cout << "\nValue:" << xmlValue << endl;
#endif
		if (strcmp(((char*) xmlName), "Generate") == 0)
		{
			if (strcmp(((char*) xmlValue), "YES") == 0)
				pjtSettingsObj->SetGenerateAttr(YES_AG);
			else if (strcmp(((char*) xmlValue), "NO") == 0)
				pjtSettingsObj->SetGenerateAttr(NO_AG);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) xmlName), "Save") == 0)
		{
			if (strcmp(((char*) xmlValue), "YES") == 0)
				pjtSettingsObj->SetSaveAttr(YES_AS);
			else if (strcmp(((char*) xmlValue), "PROMPT") == 0)
				pjtSettingsObj->SetSaveAttr(PROMPT_AS);
			else if (strcmp(((char*) xmlValue), "DISCARD") == 0)
				pjtSettingsObj->SetSaveAttr(DISCARD_AS);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) xmlName), "View") == 0)
		{
			if (strcmp(((char*) xmlValue), "General") == 0)
				pjtSettingsObj->SetViewMode(SIMPLE);
			else if (strcmp(((char*) xmlValue), "Advanced") == 0)
				pjtSettingsObj->SetViewMode(EXPERT);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) xmlName), "ExpertViewSelected") == 0)
		{
			if (strcmp(((char*) xmlValue), "true") == 0)
				pjtSettingsObj->SetExpertViewSelectedFlag(true);
			else if (strcmp(((char*) xmlValue), "false") == 0)
				pjtSettingsObj->SetExpertViewSelectedFlag(false);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else
		{
#if defined DEBUG
			cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
			return false;
		}
	}
	return true;
}

bool SetProjectSettingsCommunication(xmlTextReaderPtr xmlReader)
{

	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const xmlChar* xmlName;
		const xmlChar* xmlValue;
		//Retrieve the pxcName and Value of an attribute	
		xmlValue = xmlTextReaderConstValue(xmlReader);
		xmlName = xmlTextReaderConstName(xmlReader);

		if (xmlValue == NULL || xmlName == NULL)
			return false;
#if defined DEBUG
		cout << "\nName:" << xmlName << endl;
		cout << "\nValue:" << xmlValue << endl;
#endif
		if (strcmp(((char*) xmlName), "IP") == 0)
		{
			if ((char*) xmlValue != NULL)
			{
				PjtSettings* pjtSettingsObj;
				pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
				pjtSettingsObj->SetIP((char*) xmlValue);
			}
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Communication returning false"
				<< endl;
#endif
				return false;
			}

		}
		else
		{
#if defined DEBUG
			cout << "\nsetProjectSettings_Communication returning false"
			<< endl;
#endif
			return false;
		}
	}
	return true;
}

bool GetandCreateNode(xmlTextReaderPtr xmlReader, char* projectPath)
{

	char* nodeName = NULL;
	char* octFilePath = NULL;
	char* fileName = NULL;
	char* forceCycleValue = NULL;

	INT32 nodeId; //can't be initialised. and if it is not present false is returned.
	NodeType nodeType; //can't be initialised. and if it is not present false is returned.
	bool forceCycleFlag = false;
	StationType stationType;
	ocfmRetCode errCodeObj;

	stationType = NORMAL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const xmlChar *xmlName = NULL;
		const xmlChar *xmlValue = NULL;
		//Retrieve the pxcName and Value of an attribute	
		xmlValue = xmlTextReaderConstValue(xmlReader);
		xmlName = xmlTextReaderConstName(xmlReader);

		if (xmlValue == NULL || xmlName == NULL)
		{
			return false;
		}
		else
		{
			//TODO: 'else' added. operation to be specified.
		}

		if (strcmp(((char*) xmlName), "name") == 0)
		{
			if ((char*) xmlValue != NULL)
			{
				nodeName = new char[strlen((char*) xmlValue) + ALLOC_BUFFER];
				strcpy((char*) nodeName, (char*) xmlValue);
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) xmlName), "NodeId") == 0)
		{
			if ((char*) xmlValue != NULL)
			{
				nodeId = atoi((char*) xmlValue);
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) xmlName), "NodeType") == 0)
		{
			if (strcmp(((char*) xmlValue), "MN") == 0)
			{
				nodeType = MN;
#if defined DEBUG
				cout << "\nnodeType:" << nodeType << endl;
#endif
			}
			else if (strcmp(((char*) xmlValue), "CN") == 0)
			{
				nodeType = CN;
#if defined DEBUG
				cout << "\nnodeType:" << nodeType << endl;
#endif
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) xmlName), "xdc") == 0)
		{
			if ((char*) xmlValue != NULL)
			{
				octFilePath = new char[strlen((char*) xmlValue) + 1];
				strcpy((char*) octFilePath, (char*) xmlValue);
#if defined DEBUG
				cout << "\nxdcPath:" << octFilePath << endl;
#endif
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				octFilePath = NULL;
			}
		}
		else if (strcmp(((char*) xmlName), "ForceCycleFlag") == 0)
		{
			if (strcmp(((char*) xmlValue), "true") == 0)
			{
				forceCycleFlag = true;
#if defined DEBUG
				cout << "\nForceCycleFlag:" << forceCycleFlag << endl;
#endif
			}
			else if (strcmp(((char*) xmlValue), "false") == 0)
			{
				forceCycleFlag = false;
#if defined DEBUG
				cout << "\nForceCycleFlag:" << forceCycleFlag << endl;
#endif
			}
			else
			{
				forceCycleFlag = false;
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
			}
		}
		else if (strcmp(((char*) xmlName), "ForceCycle") == 0)
		{
			if ((char*) xmlValue != NULL && strcmp((char*) xmlValue, "") != 0)
			{
				forceCycleValue = new char[strlen((char*) xmlValue)
						+ ALLOC_BUFFER];
				strcpy((char*) forceCycleValue, (char*) xmlValue);
			}
			else
			{
				//TODO: 'else' added. operation to be specified.
			}
		}
		else if (strcmp(((char*) xmlName), "StationType") == 0)
		{
			if (strcmp(((char*) xmlValue), "Multiplexed") == 0)
			{
				stationType = MULTIPLEXED;
#if defined DEBUG
				cout << "\nStationType:" << stationType << endl;
#endif
			}
			else if (strcmp(((char*) xmlValue), "Chained") == 0)
			{
				stationType = CHAINED;
#if defined DEBUG
				cout << "\nStationType:" << stationType << endl;
#endif
			}
			else if (strcmp(((char*) xmlValue), "Normal") == 0)
			{
				stationType = NORMAL;
#if defined DEBUG
				cout << "\nStationType:" << stationType << endl;
#endif
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
			}
		}
		else
		{
			//TODO: 'else' added. operation to be specified.
		}
	}
#if defined DEBUG
	cout << "\n\n\nCan Create Node\n\n" << endl;
	cout << "\n varNodeType:" << nodeType << endl;
#endif
	//varNodeName 
	if (nodeType == 1)
	{
		errCodeObj = CreateNode(nodeId, CN, nodeName);
	}
	else if (nodeType == 0)
	{
		errCodeObj = CreateNode(nodeId, MN, nodeName);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (errCodeObj.code != OCFM_ERR_SUCCESS)
	{
		return false;
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	Node* nodeObj;
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);
	nodeObj->SetForceCycleFlag(forceCycleFlag);

#if defined DEBUG
	cout << "\nCreateNode - stErrorInfo.code:" << errCodeObj.code << endl;
#endif

	fileName = new char[(strlen(projectPath) + strlen(octFilePath) + 5)];
	sprintf(fileName, "%s/%s", projectPath, octFilePath);
	delete[] octFilePath;
	if (nodeType == 1)
	{
		errCodeObj = ParseFile(fileName, nodeId, CN);
	}
	else if (nodeType == 0)
	{
		errCodeObj = ParseFile(fileName, nodeId, MN);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (errCodeObj.code != OCFM_ERR_SUCCESS)
	{
		return false;
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (nodeType == 1 && forceCycleValue != NULL)
	{
		nodeObj->SetForcedCycle(forceCycleValue);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	nodeObj->SetStationType(stationType);

	//    copyDefToAct(iNodeID, varNodeType);
	delete[] fileName;
	delete[] nodeName;
	return true;
}

bool SaveProjectXML(char* projectPath, char* projectName)
{
	PjtSettings* pjtSettingsObj;
	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
	ocfmException exceptionObj;

	xmlTextWriterPtr xmlWriter;
	xmlDocPtr xmlDocObj;
	INT32 bytesWritten;
	char* fileName = NULL;

	/* Create a new XmlWriter for DOM, with no compression. */
	xmlWriter = xmlNewTextWriterDoc(&xmlDocObj, 0);
	if (xmlWriter == NULL)
	{
		printf("testXmlwriterDoc: Error creating the xml pxtwWriter\n");
		exceptionObj.OCFMException(OCFM_ERR_CREATE_XML_WRITER_FAILED);
		throw exceptionObj;
	}
	/* Start the document with the xml default for the version,
	 * encoding UTF-8 and the default for the standalone
	 * declaration. */
	bytesWritten = xmlTextWriterStartDocument(xmlWriter, NULL, MY_ENCODING,
			NULL);
	if (bytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_START_DOC_FAILED);
		throw exceptionObj;
	}
	bytesWritten = xmlTextWriterWriteComment(xmlWriter,
			BAD_CAST "This file was autogenerated by openCONFIGURATOR");
	xmlTextWriterSetIndent(xmlWriter, 1);
	// Start openCONFIGURATOR Tag		
	bytesWritten = xmlTextWriterStartElement(xmlWriter,
			BAD_CAST "openCONFIGURATOR");
	if (bytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw exceptionObj;
	}
	bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Version",
			BAD_CAST TOOL_VERSION);

	xmlTextWriterSetIndent(xmlWriter, 1);
	// Start profile Tag
	bytesWritten = xmlTextWriterStartElement(xmlWriter, BAD_CAST "profile");
	if (bytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw exceptionObj;
	}

	xmlTextWriterSetIndent(xmlWriter, 1);
	// Start Auto Tag		
	bytesWritten = xmlTextWriterStartElement(xmlWriter, BAD_CAST "Auto");
	if (bytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw exceptionObj;
	}

	if (pjtSettingsObj->GetGenerateAttr() == NO_AG)
	{
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
				BAD_CAST "Generate", BAD_CAST "NO");
	}
	else if (pjtSettingsObj->GetGenerateAttr() == YES_AG)
	{
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
				BAD_CAST "Generate", BAD_CAST "YES");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pjtSettingsObj->GetSaveAttr() == YES_AS)
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Save",
				BAD_CAST "YES");
	else if (pjtSettingsObj->GetSaveAttr() == PROMPT_AS)
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Save",
				BAD_CAST "PROMPT");
	else if (pjtSettingsObj->GetSaveAttr() == DISCARD_AS)
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "Save",
				BAD_CAST "DISCARD");
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pjtSettingsObj->GetViewMode() == SIMPLE)
	{
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "View",
				BAD_CAST "General");
	}
	else if (pjtSettingsObj->GetViewMode() == EXPERT)
	{
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "View",
				BAD_CAST "Advanced");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pjtSettingsObj->GetExpertViewSelectedFlag() == true)
	{
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
				BAD_CAST "ExpertViewSelected", BAD_CAST "true");
	}
	else if (pjtSettingsObj->GetExpertViewSelectedFlag() == false)
	{
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
				BAD_CAST "ExpertViewSelected", BAD_CAST "false");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	// End Auto Tag
	bytesWritten = xmlTextWriterEndElement(xmlWriter);
	if (bytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw exceptionObj;
	}

	xmlTextWriterSetIndent(xmlWriter, 1);
	// Start Communication Tag		
	bytesWritten = xmlTextWriterStartElement(xmlWriter,
			BAD_CAST "Communication");
	if (bytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw exceptionObj;
	}
	bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "IP",
			BAD_CAST "0.0.0.0");

	// End Communication Tag
	bytesWritten = xmlTextWriterEndElement(xmlWriter);
	if (bytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw exceptionObj;
	}

	// End profile Tag
	bytesWritten = xmlTextWriterEndElement(xmlWriter);
	if (bytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw exceptionObj;
	}
	xmlTextWriterSetIndent(xmlWriter, 1);
	// Start NodeCollection Tag
	bytesWritten = xmlTextWriterStartElement(xmlWriter,
			BAD_CAST "NodeCollection");
	if (bytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw exceptionObj;
	}
	//Node nodeObj;
	NodeCollection *nodeCollObj = NULL;
	//Index indexObj;
	//indexObj.SetNodeID(nodeObj.GetNodeId());
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	for (INT32 nodeCount = 0; nodeCount < nodeCollObj->GetNumberOfNodes();
			nodeCount++)
	{
		Node* nodeObj;

		nodeObj = nodeCollObj->GetNodebyColIndex(nodeCount);

		xmlTextWriterSetIndent(xmlWriter, 1);

		// Start Node Tag		
		bytesWritten = xmlTextWriterStartElement(xmlWriter, BAD_CAST "Node");

		if (bytesWritten < 0)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw exceptionObj;
		}

		char* tempNodeName;
		tempNodeName = new char[50];
		strcpy(tempNodeName, (char*) nodeObj->GetNodeName());
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "name",
				BAD_CAST tempNodeName);

		char* tempNodeID;
		tempNodeID = new char[20];

		IntToAscii(nodeObj->GetNodeId(), tempNodeID, 0);

		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "NodeId",
				BAD_CAST tempNodeID);

		NodeType tempNodeType;
		tempNodeType = nodeObj->GetNodeType();

		if (tempNodeType == 0)
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "NodeType", BAD_CAST "MN");
		if (tempNodeType == 1)
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "NodeType", BAD_CAST "CN");

		char* tempOctName = new char[50];

		sprintf(tempOctName, "%s/%s.octx", "octx", tempNodeID);
		bytesWritten = xmlTextWriterWriteAttribute(xmlWriter, BAD_CAST "xdc",
				BAD_CAST tempOctName);
		if (nodeObj->GetForceCycleFlag() == true)
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "ForceCycleFlag", BAD_CAST "true");
		}
		else if (nodeObj->GetForceCycleFlag() == false)
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "ForceCycleFlag", BAD_CAST "false");
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}

		if (nodeObj->GetForcedCycleValue() != NULL
				&& strcmp(nodeObj->GetForcedCycleValue(), ""))
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "ForceCycle",
					BAD_CAST nodeObj->GetForcedCycleValue());
		}

		if (nodeObj->GetStationType() == NORMAL)
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "StationType", BAD_CAST "Normal");
		}
		else if (nodeObj->GetStationType() == MULTIPLEXED)
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "StationType", BAD_CAST "Multiplexed");
		}
		else if (nodeObj->GetStationType() == CHAINED)
		{
			bytesWritten = xmlTextWriterWriteAttribute(xmlWriter,
					BAD_CAST "StationType", BAD_CAST "Chained");
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}

		// End Node Tag
		bytesWritten = xmlTextWriterEndElement(xmlWriter);
		if (bytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			delete[] tempNodeName;
			delete[] tempNodeID;
			delete[] tempOctName;
			throw exceptionObj;
		}

		delete[] tempNodeName;
		delete[] tempNodeID;
		delete[] tempOctName;
	}

	// End NodeCollection Tag
	bytesWritten = xmlTextWriterEndElement(xmlWriter);
	if (bytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw exceptionObj;
	}

	// End openCONFIGURATOR Tag
	bytesWritten = xmlTextWriterEndElement(xmlWriter);
	if (bytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw exceptionObj;
	}

	bytesWritten = xmlTextWriterEndDocument(xmlWriter);
	if (bytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
		exceptionObj.OCFMException(OCFM_ERR_XML_END_DOC_FAILED);
		throw exceptionObj;
	}

	xmlFreeTextWriter(xmlWriter);
	fileName = new char[MAX_FILE_PATH_SIZE];

#if defined(_WIN32) && defined(_MSC_VER)
	{

		sprintf(fileName, "%s\\%s\\%s.oct", projectPath, projectName,
				projectName);
	}
#else
	{
		sprintf(fileName, "%s/%s/%s.oct", projectPath, projectName, projectName);
	}
#endif
	xmlSaveFileEnc(fileName, xmlDocObj, MY_ENCODING);

	xmlFreeDoc(xmlDocObj);

	delete[] fileName;
	return true;
}

void CreateMNPDOVar(INT32 offsetVal, INT32 dataSize, IEC_Datatype iecDataType,
		PDOType pdoType, Node *nodeObj)
{
	ocfmException exceptionObj;
	if (NULL == nodeObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
		else
		{
			//TODO: "else" added. Operation Need to specified
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
		else
		{
			//TODO: "else" added. Operation Need to specified
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
		else
		{
			//TODO: "else" added. Operation Need to specified
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
		else
		{
			//TODO: "else" added. Operation Need to specified
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
		else
		{
			//TODO: "else" added. Operation Need to specified
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
		else
		{
			//TODO: "else" added. Operation Need to specified
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

		// Handled all values 
	case BOOL:
		cout << "Data type BOOL not handled" << endl;
		break;
	case BYTE:
		cout << "Data type BYTE not handled" << endl;
		break;
	case _CHAR:
		cout << "Data type _CHAR not handled" << endl;
		break;
	case DWORD:
		cout << "Data type DWORD not handled" << endl;
		break;
	case LWORD:
		cout << "Data type LWORD not handled" << endl;
		break;
	case STRING:
		cout << "Data type STRING not handled" << endl;
		break;
	case WSTRING:
		cout << "Data type WSTRING not handled" << endl;
		break;
	default:
		cout << "UnHandled Datatype encountered:" << iecDataType << endl;
		break;
	}
#if defined DEBUG
	cout << __FUNCTION__ << " Ind:" << piObjectObj.indexId << " SubInd:"
	<< piObjectObj.sIdxId << endl;
#endif
	strcpy(mnPDOobj.indexId, piObjectObj.indexId);
	strcpy(mnPDOobj.subIndexId, piObjectObj.sIdxId);
	/* Assign the value*/
	mnPDOobj.value = new char[10 + ALLOC_BUFFER];
	char* padStr = new char[5 + ALLOC_BUFFER];
	padStr = IntToAscii(dataSize, padStr, 16);
	padStr = PadLeft(padStr, '0', 4);
	strcpy(padStr, ConvertToUpper(padStr));
	strcpy(mnPDOobj.value, "0x");
	strcat(mnPDOobj.value, padStr);
	/* Set the Offset*/
	strcat(mnPDOobj.value, "0000");
	/* Set the Reserved*/
	strcat(mnPDOobj.value, "00");

	nodeObj->AddMNPDOvar(mnPDOobj, pdoType);

}

ocfmRetCode GetProjectSettings(AutoGenerate *autoGenStatus,
		AutoSave *autoSaveStatus, ViewMode *viewMode,
		bool* isExpertViewAlreadySet)
{

	ocfmException exceptionObj;
	if ((NULL == autoGenStatus) || (NULL == autoSaveStatus)
			|| (NULL == viewMode) || (NULL == isExpertViewAlreadySet))
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	PjtSettings* pjtSettingsObj = NULL;
	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();

	try
	{
		if (pjtSettingsObj == NULL)
		{
			exceptionObj.OCFMException(OCFM_ERR_PROJECT_SETTINGS);
			throw exceptionObj;
		}

		*autoGenStatus = pjtSettingsObj->GetGenerateAttr();
		*autoSaveStatus = pjtSettingsObj->GetSaveAttr();
		*viewMode = pjtSettingsObj->GetViewMode();
		*isExpertViewAlreadySet = pjtSettingsObj->GetExpertViewSelectedFlag();

		errCodeObj.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SetProjectSettings(AutoGenerate autoGenStatus,
		AutoSave autoSaveStatus, ViewMode viewMode, bool isExpertViewAlreadySet)
{
	ocfmRetCode errCodeObj;
	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	PjtSettings* pjtSettingsObj;
	pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();

	try
	{
		if (pjtSettingsObj == NULL)
		{
			ocfmException exceptionObj;
			exceptionObj.OCFMException(OCFM_ERR_PROJECT_SETTINGS);
			throw exceptionObj;
		}

		pjtSettingsObj->SetGenerateAttr(autoGenStatus);
		pjtSettingsObj->SetSaveAttr(autoSaveStatus);
		pjtSettingsObj->SetViewMode(viewMode);
		pjtSettingsObj->SetExpertViewSelectedFlag(isExpertViewAlreadySet);
		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& exObj)
	{
		return exObj._ocfmRetCode;
	}
	return errCodeObj;
}

//TODO: varNodetype is not used. To be removed.
void UpdateNumberOfEnteriesSIdx(Index *indexObj, NodeType nodeType)
{
	if (NULL == indexObj)
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
		pobjSIdx->SetActualValue(buffer);
		delete[] buffer;
	}
}

void AuotgenerateOtherIndexs(Node* nodeObj)
{

	if (NULL == nodeObj)
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}

	ocfmRetCode errCodeObj;
	char* indexId = new char[INDEX_LEN + ALLOC_BUFFER];
	char* sidxId = new char[SUBINDEX_LEN + ALLOC_BUFFER];
	Index* indexObj;
	IndexCollection* indexCollObj;

	INT32 indexPos;

	indexCollObj = nodeObj->GetIndexCollection();
	/* 1006*/
	strcpy(indexId, "1006");
	errCodeObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
#if defined DEBUG	
	cout << "retcode" << errCodeObj.code << endl;
	cout << "1006 added" << endl;
#endif

	/*  1300*/
	strcpy(indexId, "1300");
	errCodeObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		SetBasicIndexAttributes(MN_NODEID, MN, indexId, (char*) "5000",
				(char*) "SDO_SequLayerTimeout_U32", TRUE);
	}

	/* 1C02*/
	strcpy(indexId, "1C02");
	errCodeObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(TRUE);
		strcpy(sidxId, "00");
		SetSIdxValue(indexId, sidxId, (char*) "3", indexCollObj,
				nodeObj->GetNodeId(), MN, false);
#if defined DEBUG	
		cout << "1c02 subidex added" << endl;
#endif
		/* Set subindex value 40 or 0000028 */
		strcpy(sidxId, "01");
		SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
				nodeObj->GetNodeId(), MN, false);
#if defined DEBUG	
		cout << "1c02 subidex 01 added" << endl;
#endif
		strcpy(sidxId, "02");
		SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
				nodeObj->GetNodeId(), MN, false);

		strcpy(sidxId, "03");
		SetSIdxValue(indexId, sidxId, (char*) "40", indexCollObj,
				nodeObj->GetNodeId(), MN, false);
	}

	/*  1C09*/
	strcpy(indexId, "1C09");
	errCodeObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(TRUE);

		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(),
				(char*) "40", false);

	}

	/*  1F26*/
	strcpy(indexId, "1F26");
	errCodeObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(TRUE);

		char* val = new char[8];
		INT32 ilConfigDate;
		ilConfigDate = GetConfigDate();
		val = IntToAscii(ilConfigDate, val, 10);
		strcpy(val, ConvertToUpper(val));
		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), val, false);
		delete[] val;

	}

#if defined DEBUG	
	cout << "1F26 subidex added" << endl;
#endif
	/*  1F27*/
	strcpy(indexId, "1F27");
	errCodeObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(TRUE);

		char* val = new char[50];
		INT32 ilConfigTime;
		ilConfigTime = GetConfigTime();
		val = IntToAscii(ilConfigTime, val, 10);
		strcpy(val, ConvertToUpper(val));

		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), val, false);
		delete[] val;
	}

	/* 1F84*/
	strcpy(indexId, "1F84");
	errCodeObj = IfIndexExists(MN_NODEID, MN, indexId, &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(TRUE);

		AddForEachSIdx(indexId, indexCollObj, nodeObj->GetNodeId(), (char*) "",
				true);

	}

	delete[] indexId;
	delete[] sidxId;
}

void UpdatedCNDateORTime(Index* indexObj, INT32 nodeId, DateTime dateOrTime)
{
	if (NULL == indexObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
			!= (IfIndexExists(nodeObj->GetNodeId(), CN, indexId, &indexPos)).code)
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
			else
			{
				//TODO: 'else' added. operation to be specified
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
		indexObj = indexCollObj->GetIndex(indexLC);

		if (!(CheckIfNotPDO((char*) indexObj->GetIndexValue())))
		{

			for (INT32 sIdxLC = 0; sIdxLC < indexObj->GetNumberofSubIndexes();
					sIdxLC++)
			{
				sidxObj = indexObj->GetSubIndex(sIdxLC);
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
	if (NULL == indexObj)
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}
	else
	{
		//nothing to do
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
			sidxObj1 = tempIndexObj.GetSubIndex(idxLC2);
			if ((sidxObj1->GetActualValue() != NULL)
					&& (0 != strcmp(sidxObj1->GetActualValue(), ""))
					&& !(CheckIfValueZero((char*) sidxObj1->GetActualValue())))
			{
				const char* actualVal1 = sidxObj1->GetActualValue();

				INT32 length1 = strlen(actualVal1);
				char* offset1 = NULL;
				INT32 offsetVal1 = 0;

				offset1 = SubString((char*) actualVal1, length1 - 12, 4);
				offsetVal1 = HexToInt(offset1);
				sidxObj2 = tempIndexObj.GetSubIndex(idxLC2 + 1);
				if ((NULL != sidxObj2->GetActualValue())
						&& (0 != strcmp(sidxObj2->GetActualValue(), ""))
						&& !(CheckIfValueZero(
								(char*) sidxObj2->GetActualValue())))
				{
					const char* actualVal2 = sidxObj2->GetActualValue();
					INT32 length2 = strlen(actualVal2);
					char* offset2 = NULL;
					INT32 offsetVal2 = 0;

					offset2 = SubString((char*) actualVal2, length2 - 12, 4);
					offsetVal2 = HexToInt(offset2);

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

ocfmRetCode GetFeatureValue(INT32 nodeId, NodeType nodeType,
		FeatureType featureType, char* featureName, char* outFeatureValue)
{
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;

	try
	{
		bool existFlag = false;
		INT32 nodePos;
		errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, existFlag);
		if (errCodeObj.code == 0 && existFlag == true)
		{
			//continue with process
		}
		else
		{
			// Node Doesn't Exist
			ocfmException exceptionObj;
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw exceptionObj;
		}

		Node nodeObj;
		NodeCollection *nodeCollObj;
		NetworkManagement *nmtObj;

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeId);

		nmtObj = nodeObj.GetNetworkManagement();
		strcpy(outFeatureValue,
				nmtObj->GetNwMgmtFeatureValue(featureType, featureName));

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode UpdateNodeParams(INT32 currentNodeId, INT32 newNodeID,
		NodeType nodeType, char* nodeName, StationType stationType,
		char* forcedCycleVal, bool isForcedCycle, char* presTimeoutVal)
{
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;
	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	INT32 nodePos;
	try
	{
		bool bFlag = false;

		if (newNodeID == currentNodeId)
		{
			errCodeObj = IfNodeExists(newNodeID, nodeType, &nodePos, bFlag);
			if (errCodeObj.code == 0 && bFlag == true)
			{

			}
			else
			{
				exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEID);
				throw exceptionObj;
			}

		}
		else if (newNodeID != currentNodeId)
		{
			errCodeObj = IfNodeExists(newNodeID, nodeType, &nodePos, bFlag);
			if (errCodeObj.code == OCFM_ERR_SUCCESS && bFlag == true)
			{
				exceptionObj.OCFMException(OCFM_ERR_NODE_ALREADY_EXISTS);
				throw exceptionObj;
			}
			else
			{
#if defined DEBUG
				cout << "OCFM_ERR_NODEID_NOT_FOUND" << OCFM_ERR_NODEID_NOT_FOUND
				<< endl;
#endif
			}
		}
		else
		{
			//TODO: 'else' added. operation to be specified
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
			else
			{
				//TODO: else added. operation to be added.
			}
		}
		else
		{
			//TODO: else added. operation to be added
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
		else
		{
			//TODO: else added. operation to be added
		}

		if ((nodeType == CN)
				&& !((presTimeoutVal == NULL)
						|| (strcmp(presTimeoutVal, "") == 0)))
		{
			nodeObj->SetPollResponseTimeout(presTimeoutVal);
		}
		else
		{
			//TODO: else added. operation to be added
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
			else
			{
				//TODO: else added. operation to be added
			}

			if (stationType != MULTIPLEXED)
			{
				nodeObj->ResetForcedCycleValue();
			}
			else
			{
				//TODO: 'else' added. Operations to be specified.
			}

			if (stationType == MULTIPLEXED)
			{
				bool calcForceCycle = true;
				bool setForceCycle = true;
				if (isForcedCycle == true)
				{
					calcForceCycle = false;
				}
				else
				{
					//TODO: else added. operation to be added
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
							actualValue =
									HexToInt(
											SubString(
													nodeObj->GetForcedCycleValue(),
													2,
													strlen(
															nodeObj->GetForcedCycleValue())
															- 2));
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
						else
						{
							//TODO: else added. operation to be added
						}
					}
				}
				else
				{
					//TODO: 'else' added. operation to be specified
				}

				if (calcForceCycle == true)
				{
					forcedCycleVal = GetLastAvailableCycleNumber();
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				if (setForceCycle == true)
				{
					errCodeObj = nodeObj->SetForcedCycle(forcedCycleVal);
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				if (OCFM_ERR_SUCCESS != errCodeObj.code)
				{
					return errCodeObj;
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				nodeObj->SetForceCycleFlag(isForcedCycle);
			}
			else
			{
				//TODO: else added. operation to be added
			}
		}
		else
		{
			//TODO: else added. operation to be added
		}

		errCodeObj.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode GetNodeDataTypes(INT32 nodeId, NodeType nodeType,
		char* outDataTypes)
{
	ocfmRetCode errCodeObj;
	errCodeObj.errorString = NULL;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	INT32 nodePos;
	try
	{
		bool existFlag = false;
		errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, existFlag);
		if (errCodeObj.code == 0 && existFlag == true)
		{

		}
		else
		{
			// Node Doesn't Exist
			ocfmException exceptionObj;
			exceptionObj.OCFMException(OCFM_ERR_NODE_ALREADY_EXISTS);
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

		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

char* GetNodeAssigmentBits(Node* nodeObj)
{
	if (NULL == nodeObj)
	{
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
		cout << "Invalid station Type:" << nodeObj->GetStationType() << endl;
		break;
	};

	nodeAssignData = IntToAscii(tempValue, nodeAssignData, 16);
	return nodeAssignData;
}

void SetPresMNNodeAssigmentBits()
{
	INT32 indexPos;
	INT32 subIndexPos;
	ocfmRetCode errCodeObj;
	errCodeObj = IfSubIndexExists(MN_NODEID, MN, (char *) "1F81", (char *) "F0",
			&subIndexPos, &indexPos);

	if (OCFM_ERR_SUCCESS != errCodeObj.code)
	{
		return;
	}

	bool isPresMn = false;
	bool isMNBroadcastingPRes = false;

	isPresMn = IsPresMN();

	//check whether Pres activated in MN
	NodeCollection* nodeCollObj;
	Node nodeObj;
	IndexCollection* pdoIndexCollobj;
	IndexCollection* indexCollObj;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetMNNode();

	INT32 tpdoCount = 0;
	INT32 rpdoCount = 0;
	pdoIndexCollobj = nodeObj.getPDOIndexCollection(&rpdoCount, &tpdoCount);
	if (pdoIndexCollobj != NULL)
	{
		indexCollObj = nodeObj.GetIndexCollection();
		Index* indexObj;
		for (INT32 indexLC = 0; indexLC < pdoIndexCollobj->GetNumberofIndexes();
				indexLC++)
		{
			indexObj = pdoIndexCollobj->GetIndex(indexLC);
			if (0 == strncmp(indexObj->GetIndexValue(), "1A", 2))
			{
				Index *commIndexObj = NULL;

				char *indexId = SubString((char *) indexObj->GetIndexValue(), 2,
						4);
				char *commIdxId = new char[INDEX_LEN];
				strcpy(commIdxId, (char *) "18");
				strcat(commIdxId, indexId);
				commIndexObj = indexCollObj->GetIndexbyIndexValue(commIdxId);
				if (NULL != commIndexObj)
				{
					SubIndex *sidxObj = NULL;
					sidxObj = commIndexObj->GetSubIndexbyIndexValue(
							(char *) "01");
					if (NULL != sidxObj)
					{
						if ((NULL != sidxObj->GetActualValue())
								&& (0 != strcmp(sidxObj->GetActualValue(), "")))
						{
							INT32 tpdoMappedNodeId = 0;
							tpdoMappedNodeId = GetDecimalValue(
									(char *) sidxObj->GetActualValue());
							if (BROADCAST_NODEID == tpdoMappedNodeId)
							{
								isMNBroadcastingPRes = true;
								break;
							}
						}
					}
				}
				delete[] commIdxId;
				delete[] indexId;
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
			nodeAssignData = new char[8 + STR_ALLOC_BUFFER];
			tempValue = EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS
					| EPL_NODEASSIGN_MN_PRES;
			strcpy(nodeAssignData, (char *) "0x");
			IntToAscii(tempValue, &nodeAssignData[2], 16);

			sidxObj->SetActualValue(nodeAssignData);
			sidxObj->SetFlagIfIncludedCdc(TRUE);
			delete[] nodeAssignData;
		}
		else
		{
			// no operation
		}
	}

}

ocfmRetCode RecalculateMultiplex()
{
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_UNKNOWN;
	errCodeObj.errorString = NULL;
	NodeCollection *nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	try
	{
		if (false == CheckIfMultiplexedCNExist())
		{
			errCodeObj.code = OCFM_ERR_SUCCESS;
			return errCodeObj;
		}
		INT32 indexPos = 0;
		INT32 subIndexPos = 0;
		errCodeObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &indexPos);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			//reset automatically assigned cn force cycle
			ResetMultiplexedCNForceCycle();
			errCodeObj.code = OCFM_ERR_SUCCESS;
			return errCodeObj;
		}

		char* actValue = new char[50];
		actValue[0] = 0;
		errCodeObj = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			ocfmException exceptionObj;
			exceptionObj.OCFMException(errCodeObj.code);
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
				ocfmRetCode errCodeObj1;
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
					errCodeObj1 = GetSubIndexAttributes(MN_NODEID, MN, indexId,
							value, ACTUALVALUE, sIdxActValue);
				} catch (...)
				{
					delete[] sIdxActValue;
					continue;
				}
				if (errCodeObj1.code == OCFM_ERR_SUCCESS)
				{
				}
				else
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
						INT32 sidxExistFlag = 0;
						sidxExistFlag = atoi(subIndFlag);
						SetBasicSubIndexAttributes(MN_NODEID, MN, indexId,
								value, (char*) "", sidxName,
								(Flag) sidxExistFlag);
						delete[] sidxName;
						delete[] subIndFlag;
						continue;
					}
					else
					{
						if (errCodeObj1.code == OCFM_ERR_SUCCESS)
						{
							INT32 mnMultiActualValue = 0;
							if (strncmp(actValue, "0x", 2) == 0
									|| strncmp(actValue, "0X", 2) == 0)
								mnMultiActualValue = HexToInt(
										SubString(actValue, 2,
												strlen(actValue) - 2));
							else
								mnMultiActualValue = atoi(actValue);

							char* forcedCycleValue = new char[50];
							strcpy(forcedCycleValue,
									nodeObj->GetForcedCycleValue());
							INT32 cnActualValue = 0;
							if (strncmp(forcedCycleValue, "0x", 2) == 0
									|| strncmp(forcedCycleValue, "0X", 2) == 0)
								cnActualValue = HexToInt(
										SubString(forcedCycleValue, 2,
												strlen(forcedCycleValue) - 2));
							else
								cnActualValue = atoi(forcedCycleValue);

							if (mnMultiActualValue == 0)
							{

								char* sidxName = new char[50];
								sidxName[0] = 0;
								GetSubIndexAttributes(MN_NODEID, MN, indexId,
										value, NAME, sidxName);
								char* subIndFlag = new char[10];
								GetSubIndexAttributes(MN_NODEID, MN, indexId,
										value, FLAGIFINCDC, subIndFlag);
								INT32 iCNsubIndFlag = 0;
								iCNsubIndFlag = atoi(subIndFlag);
								SetBasicSubIndexAttributes(MN_NODEID, MN,
										indexId, value, (char*) "", sidxName,
										(Flag) iCNsubIndFlag);
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

					INT32 iCNsubIndFlag = 0;
					iCNsubIndFlag = atoi(subIndFlag);
					SetBasicSubIndexAttributes(MN_NODEID, MN, indexId, value,
							(char*) "", subIndName, (Flag) iCNsubIndFlag);
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
	catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
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
			ocfmRetCode errCodeObj;
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
				errCodeObj = GetSubIndexAttributes(MN_NODEID, MN, indexId,
						value, ACTUALVALUE, sidxActValue);
			} catch (...)
			{
				delete[] sidxActValue;
				continue;
			}
			if (errCodeObj.code == OCFM_ERR_SUCCESS)
			{
			}
			else
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

void CheckAndReAssignMultiplex(INT32 nodeId, char* cycleValue)
{
	ocfmRetCode errCodeObj;
	if (cycleValue == NULL || strcmp(cycleValue, "") == 0)
	{
		errCodeObj.code = OCFM_ERR_SUCCESS;
		//return stErrorInfo;
		return;
	}

	INT32 tempCycleValue = 0;
	if (strncmp(cycleValue, "0x", 2) == 0 || strncmp(cycleValue, "0X", 2) == 0)
		tempCycleValue = HexToInt(
				SubString(cycleValue, 2, strlen(cycleValue) - 2));
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

			errCodeObj = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
					(char*) "07", ACTUALVALUE, actValue);
		} catch (...)
		{
			delete[] actValue;
			errCodeObj.code = OCFM_ERR_SUCCESS;
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
						actualValueCN = HexToInt(
								SubString(ForcedCycleValue, 2,
										strlen(ForcedCycleValue) - 2));
					else
						actualValueCN = atoi(ForcedCycleValue);

					delete[] ForcedCycleValue;
					if (tempCycleValue == actualValueCN)
					{
						return;
					}
					else
					{
						//continue the function
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
				else
				{
				}

				char* forcedCycleValue = new char[strlen(
						objNode->GetForcedCycleValue()) + ALLOC_BUFFER];
				strcpy(forcedCycleValue, objNode->GetForcedCycleValue());
				INT32 actualValueCN = 0;
				if (strncmp(forcedCycleValue, "0x", 2) == 0
						|| strncmp(forcedCycleValue, "0X", 2) == 0)
					actualValueCN = HexToInt(
							SubString(forcedCycleValue, 2,
									strlen(forcedCycleValue) - 2));
				else
					actualValueCN = atoi(forcedCycleValue);

				delete[] forcedCycleValue;
				if (tempCycleValue >= actualValueCN || actualValueCN == 1)
				{
					continue;
				}
				else
				{
					//continue the function
				}
				actualValueCN--;
				cycleNumberGlobal = actualValueCN;

				static char* tempActualValue = NULL;
				if (tempActualValue != NULL)
				{
					delete[] tempActualValue;
					tempActualValue = NULL;
				}
				tempActualValue = new char[50];
				tempActualValue = IntToAscii(actualValueCN, tempActualValue,
						10);

				objNode->SetForcedCycle(tempActualValue);

				delete[] tempActualValue;
				delete[] forcedCycleValue;
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
	ocfmRetCode errCodeObj;
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	INT32 nodesCount = 0;
	nodesCount = nodeCollObj->GetCNNodesCount();
	errCodeObj.code = OCFM_ERR_SUCCESS;
	errCodeObj.errorString = NULL;

	try
	{
		bool setErrFlag = false;
		if (nodesCount == 0)
		{
			errCodeObj.code = OCFM_ERR_NO_CN_NODES_FOUND;
			return errCodeObj;
		}
		if (false == CheckIfMultiplexedCNExist())
		{
			return errCodeObj;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		INT32 subIndexPos = 0;
		INT32 indexPos = 0;
		errCodeObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &indexPos);
		if (errCodeObj.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			errCodeObj.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
			CopyCustomErrorString(&errCodeObj,
					(char*) "In MN the SubIndex 07 of the Index 1F98 does not exist. So multiplexing cannot be supported");
			delete[] actValue;
			return errCodeObj;
		}
		errCodeObj = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (errCodeObj.code != OCFM_ERR_SUCCESS)
		{
			delete[] actValue;
			return errCodeObj;
		}

		errCodeObj.errorString = new char[500];
		strcpy(errCodeObj.errorString, "CN with nodeid ");
		INT32 tempActualValue = 0;
		if (strncmp(actValue, "0x", 2) == 0 || strncmp(actValue, "0X", 2) == 0)
			tempActualValue = HexToInt(
					SubString(actValue, 2, strlen(actValue) - 2));
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

					ocfmRetCode errCodeObj1;
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
						errCodeObj1 = GetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, tempValue, ACTUALVALUE,
								subIndActValue);
					} catch (...)
					{
						delete[] subIndActValue;
						//delete[] strConvertedValue;
						continue;
					}
					if (errCodeObj1.code == OCFM_ERR_SUCCESS)
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
							actualValueCN = HexToInt(
									SubString(forcedCycleValue, 2,
											strlen(forcedCycleValue) - 2));
						else
							actualValueCN = atoi(forcedCycleValue);

						if (actualValueCN <= tempActualValue)
						{
							continue;
						}
						else
						{
							sprintf(errCodeObj.errorString, "%s%d, ",
									errCodeObj.errorString,
									nodeObj->GetNodeId());
							setErrFlag = true;
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
			errCodeObj.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
			errCodeObj.errorString[strlen(errCodeObj.errorString) - 2] = 0;
			strcat(errCodeObj.errorString,
					" multiplex cycle value exceeds the multiplex prescalar");
		}
		delete[] actValue;
	} //end of try
	catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

UINT32 GetFreeCycleNumber(UINT32 parmCycleNumber)
{
	ocfmRetCode errCodeObj;
	NodeCollection *nodeCollObj;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	INT32 nodeCount = 0;
	nodeCount = nodeCollObj->GetCNNodesCount();

	Node* nodeObj;

	if (nodeCount == 0)
	{
		errCodeObj.code = OCFM_ERR_NO_CN_NODES_FOUND;
		ocfmException exceptionObj;
		exceptionObj.OCFMException(errCodeObj.code);
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
						actualValueCN = HexToInt(
								SubString(forcedCycleValue, 2,
										strlen(forcedCycleValue) - 2));
					else
						actualValueCN = atoi(forcedCycleValue);

					delete[] forcedCycleValue;
					if (cycleNumberLC == actualValueCN)
					{
						break;
					}
					else
					{
						//continue the function
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
						actualValueCN = HexToInt(
								SubString(forcedCycleValue, 2,
										strlen(forcedCycleValue) - 2));
					else
						actualValueCN = atoi(forcedCycleValue);

					delete[] forcedCycleValue;
					if (cycleNumberLC == actualValueCN)
					{
						break;
					}
					else
					{
						//continue the function
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
	ocfmRetCode errCodeObj;
	errCodeObj = IfSubIndexExists(nodeId, nodeType, (char*) "1F98",
			(char*) "03", &subIndexPos, &indexPos);
	if (errCodeObj.code != OCFM_ERR_SUCCESS)
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
		tempValue = HexToInt(SubString(tempValStr, 2, strlen(tempValStr) - 2));
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

void CopyCustomErrorString(ocfmRetCode* errCodeObj, char* customErrStr)
{
	if ((NULL == errCodeObj) || (NULL == customErrStr))
	{
		return;
	}
	else
	{
		//Nothing to do
	}

	if (0 == strlen(customErrStr))
	{
		return;
	}
	else
	{
		//Nothing to do
	}

	errCodeObj->errorString = new char[strlen(customErrStr) + ALLOC_BUFFER];
	strcpy(errCodeObj->errorString, customErrStr);
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

//TODO: unused function
void RecalculateCNPresTimeout(char* sidxId)
{
	ocfmRetCode errCodeObj;
	INT32 sidxPos = 0;
	INT32 indexPos = 0;

	errCodeObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F92", sidxId,
			&sidxPos, &indexPos);
	if (errCodeObj.code == OCFM_ERR_SUCCESS)
	{
		//continue with process
	}
	else
	{
		return;
	}
	char* attribValue = new char[50];
	GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F92", sidxId, ACTUALVALUE,
			attribValue);

	if (NULL == attribValue)
	{
		return;
	}

	INT32 nodeId = HexToInt(sidxId);

	errCodeObj = IfSubIndexExists(nodeId, CN, (char*) "1F98", (char*) "03",
			&sidxPos, &indexPos);
	if (OCFM_ERR_SUCCESS != errCodeObj.code)
	{
		delete[] attribValue;
		return;
	}

	char* convertedValue;
	convertedValue = new char[SUBINDEX_LEN];
	strcpy(convertedValue, sidxId);

	char* sidxName = new char[50];
	sidxName[0] = 0;
	INT32 sidxFlagVal = 0;

	GetSubIndexAttributes(nodeId, CN, (char*) "1F98", (char*) "03", NAME,
			sidxName);
	char* subIndFlag = new char[10];
	GetSubIndexAttributes(nodeId, CN, (char*) "1F98", (char*) "03", FLAGIFINCDC,
			subIndFlag);
	sidxFlagVal = atoi(subIndFlag);

	SetBasicSubIndexAttributes(nodeId, CN, (char*) "1F98", (char*) "03",
			attribValue, sidxName, (Flag) sidxFlagVal);
	delete[] sidxName;
	delete[] subIndFlag;
	delete[] attribValue;
	delete[] convertedValue;

}

//TODO: CNsCount is not used to be removed.
void UpdateMNNodeAssignmentIndex(Node *nodeObj, INT32 cnCount, char* indexId,
		bool allowMNSubindex)
{
	if ((NULL == indexId) || (NULL == nodeObj))
	{
		return;
	}

	IndexCollection *indexCollObj = NULL;
	ocfmRetCode errCodeObj;
	errCodeObj.errorString = NULL;
	INT32 indexPos;

	indexCollObj = nodeObj->GetIndexCollection();
	char* tempIndexId = new char[INDEX_LEN + ALLOC_BUFFER];

	strcpy(tempIndexId, indexId);

	errCodeObj = IfIndexExists(MN_NODEID, MN, tempIndexId, &indexPos);
	delete[] tempIndexId;
	if (errCodeObj.code == OCFM_ERR_SUCCESS)
	{
		Index *indexObj = NULL;
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		/* $:set Flag to true*/
		indexObj->SetFlagIfIncludedCdc(TRUE);

		for (INT32 sidxLC = indexObj->GetNumberofSubIndexes() - 1; sidxLC >= 0;
				sidxLC--)
		{
			SubIndex* sidxObj;
			sidxObj = indexObj->GetSubIndex(sidxLC);
			if (NULL == sidxObj)
				continue;

			if (0 == strcmp((char*) sidxObj->GetIndexValue(), "00"))
				continue;

			try
			{
				INT32 nodeIdVal = HexToInt((char*) sidxObj->GetIndexValue());
				NodeType nodeType;
				if (MN_NODEID == nodeIdVal)
				{
					nodeType = MN;
				}
				else
				{
					nodeType = CN;
				}
				INT32 nodePos;
				bool bFlag = false;
				errCodeObj = IfNodeExists(nodeIdVal, nodeType, &nodePos, bFlag);

				if (OCFM_ERR_SUCCESS == errCodeObj.code && true == bFlag
						&& ((CN == nodeType) || (true == allowMNSubindex)))
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

bool ValidateCNPresTimeout(char* subIndexId, char* presTimeOutVal)
{
	ocfmRetCode errCodeObj;
	INT32 sidxPos = 0;
	INT32 indexPos = 0;
	bool retval = false;

	errCodeObj = IfSubIndexExists(MN_NODEID, MN, (char*) "1F92", subIndexId,
			&sidxPos, &indexPos);
	if (errCodeObj.code == OCFM_ERR_SUCCESS)
	{
		//continue with process
	}
	else
	{
		return retval;
	}

	INT32 nodeId = HexToInt(subIndexId);
	errCodeObj = IfSubIndexExists(nodeId, CN, (char*) "1F98", (char*) "03",
			&sidxPos, &indexPos);

	char* defaultValueCN = new char[50];
	if (errCodeObj.code == OCFM_ERR_SUCCESS)
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
	ocfmRetCode errCodeObj;
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

	errCodeObj = IfSubIndexExists(MN_NODEID, MN, indexId, tempOldNodeIdCN,
			&subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS == errCodeObj.code)
	{
		indexObj = indexCollObj->GetIndexbyIndexValue(indexId);
		sidxObj = indexObj->GetSubIndexbyIndexValue(tempOldNodeIdCN);
		if (NULL != sidxObj)
		{
			Flag tempOldFlg = FALSE;
			if (NULL != (char*) sidxObj->GetActualValue())
			{
				tempOldActualValue = new char[strlen(
						(char*) sidxObj->GetActualValue()) + STR_ALLOC_BUFFER];
				strcpy((char*) tempOldActualValue,
						(char*) sidxObj->GetActualValue());
				tempOldFlg = sidxObj->GetFlagIfIncludedCdc();
			}
			errCodeObj = IfSubIndexExists(MN_NODEID, MN, indexId, tempNodeIdCN,
					&subIndexPos, &indexPos);
			if (OCFM_ERR_SUCCESS == errCodeObj.code)
			{
				//set the value alone
			}
			else
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
	else
	{
		//no operation
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
		ocfmException exceptionObj;
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
				if (indexObj->GetSubIndex(sidxLC)->GetActualValue() != NULL
						&& TRUE
								== indexObj->GetSubIndex(sidxLC)->GetFlagIfIncludedCdc()
						&& true
								== IsDefaultActualNotEqual(
										indexObj->GetSubIndex(sidxLC)))
				{
					if (0
							== strcmp(
									indexObj->GetSubIndex(sidxLC)->GetIndexValue(),
									"00"))
					{
						return true;
						//continue;
					}
					else //other than 00'th subindex
					{
						if (true
								== IsDefaultActualNotEqual(
										indexObj->GetSubIndex(sidxLC)))
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
		if (commIndexObj->GetSubIndex(sidxLC)->GetActualValue() != NULL
				&& TRUE
						== commIndexObj->GetSubIndex(sidxLC)->GetFlagIfIncludedCdc()
				&& true
						== CheckAccessTypeForInclude(
								(char*) commIndexObj->GetSubIndex(sidxLC)->GetAccessType())
				&& true
						== IsDefaultActualNotEqual(
								commIndexObj->GetSubIndex(sidxLC)))
		{
			return true;
		}
	}

	return false;
}

INT32* ArrangeNodeIDbyStation(void)
{
	NodeCollection* nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node* nodeObj = NULL;

	/* Check RPDO Mapped objects*/
	INT32 nodesCNCount = 0;

	nodesCNCount = nodeCollObj->GetCNNodesCount();

	if (nodesCNCount == 0)
	{
		exit(0);
	}

	INT32 *nodeIdColl = new INT32[nodesCNCount+1];
	INT32 arrangedNodeIdCount = 0;
	INT32 *arrangedNodeIdColl = new INT32[nodesCNCount+1];
	StationType *stationTypeColl = new StationType[nodesCNCount+1];
	INT32 nodesCount = nodeCollObj->GetNumberOfNodes();
	INT32 loopCount = 0;
	INT32 tempVal = 0;
	for (loopCount = 0; loopCount < nodesCount; loopCount++)
	{
		nodeObj = nodeCollObj->GetNodebyColIndex(loopCount);
		if (MN == nodeObj->GetNodeType())
		{
			continue;
		}
		nodeIdColl[tempVal] = nodeObj->GetNodeId();
		stationTypeColl[tempVal] = nodeObj->GetStationType();
		tempVal++;
	}
	//get the number of poll response station and other station
	INT32 iChainStnCnt = 0;
	INT32 iOtherStnCnt = 0;
	for (loopCount = 0; loopCount < nodesCount; loopCount++)
	{
		if (CHAINED == stationTypeColl[loopCount])
			iChainStnCnt++;
	}
	iOtherStnCnt = nodesCount - iChainStnCnt;

	//create arrays to store nodeid for chained station and other station
	if (0 != iChainStnCnt)
	{
		INT32 *piChainStnColl = new INT32[iChainStnCnt];
		INT32 iChainStnLoopCnt = 0;
		for (loopCount = 0; loopCount < nodesCount; loopCount++)
		{
			if (CHAINED == stationTypeColl[loopCount])
			{
				piChainStnColl[iChainStnLoopCnt] = nodeIdColl[loopCount];
				iChainStnLoopCnt++;
			}
		}
		//sort by station no
		SortNodeID(piChainStnColl, iChainStnCnt);

		//copy the poll response staion sorted in asscending order
		for (loopCount = 0; loopCount < iChainStnCnt; loopCount++)
		{
			arrangedNodeIdColl[arrangedNodeIdCount] = piChainStnColl[loopCount];
			arrangedNodeIdCount++;
		}

		delete[] piChainStnColl;
	}

	if (0 != iOtherStnCnt)
	{
		INT32 *piOtherStnColl = new INT32[iOtherStnCnt];
		INT32 iOtherStnLoopCnt = 0;
		for (loopCount = 0; loopCount < nodesCount; loopCount++)
		{
			if (CHAINED != stationTypeColl[loopCount])
			{
				piOtherStnColl[iOtherStnLoopCnt] = nodeIdColl[loopCount];
				iOtherStnLoopCnt++;
			}
		}
		//copy the other station after the poll response staion
		for (loopCount = 0; loopCount < iOtherStnCnt; loopCount++)
		{
			arrangedNodeIdColl[arrangedNodeIdCount] = piOtherStnColl[loopCount];
			arrangedNodeIdCount++;
		}
		delete[] piOtherStnColl;
	}
	delete[] nodeIdColl;
	delete[] stationTypeColl;

	return arrangedNodeIdColl;
}

void SortNodeID(INT32 *nodeIDColl, INT32 collectionSize)
{

	try
	{
		if (NULL == nodeIDColl)
		{
			ocfmException exceptionObj;
			exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
	} catch (ocfmException& ex)
	{
		throw ex;
	}
}

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
		if (CHAINED == nodeObj->GetStationType())
		{
			//return true;
			isPres = true;
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
				sidxObj->SetFlagIfIncludedCdc(TRUE);
				indexObj->SetFlagIfIncludedCdc(TRUE);
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
				sidxObj->SetFlagIfIncludedCdc(TRUE);
				indexObj->SetFlagIfIncludedCdc(TRUE);
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
				sidxObj->SetFlagIfIncludedCdc(TRUE);
				indexObj->SetFlagIfIncludedCdc(TRUE);
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

