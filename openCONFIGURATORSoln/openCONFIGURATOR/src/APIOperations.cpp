/**
 ************************************************************************************************
 \file		APIOperations.cpp

 \brief		This file handles all the operations regarding the API calls
 ************************************************************************************************
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

/****************************************************************************************************/
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

/****************************************************************************************************/
/* Defines */

#define MY_ENCODING "UTF-8"
#define CDC_BUFFER 5000
#define CDC_MN_BUFFER 200000
//#define PI_VAR_COUNT 4000
#define MAX_FILE_PATH_SIZE 500

/****************************************************************************************************/
/* Global Variables */

INT32 iLastVarIndex = -1;
static bool bCDTCompleted = false;
static bool ObjectDictLoaded = false;

static stOffsets stSize8INOffset;
static stOffsets stSize16INOffset;
static stOffsets stSize32INOffset;
static stOffsets stSize64INOffset;
static stOffsets stSize8OUTOffset;
static stOffsets stSize16OUTOffset;
static stOffsets stSize32OUTOffset;
static stOffsets stSize64OUTOffset;

//INT32 iNo8Offsets = 0;
//INT32 iNo16Offsets = 0;
//INT32 iNo32Offsets = 0;
//INT32 iNo64Offsets = 0;

char abC_DLL_ISOCHR_MAX_PAYL[5] = "1490";
//char abC_DLL_MIN_ASYNC_MTU[4] = "300";
INT32 iConfigDate; //global used in xdcoperations
INT32 iConfigTime; //global used in xdcoperations
UINT32 uiCycleNumber; //global used in Utility

static sBuildTime sBuildTime_g;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*****************************************************************************/
/**
 \brief		AddIndexAttributes

 Adds the default attributes to the Index, when addded. The default values are fetched from the object dictionary

 \param		pIndexID			Character pointer to hold the IndexID
 \param		pobjIndex			Class pointer of CIndex to add the attributes
 \param		pobjDictIndex		Class pointer of CIndex form object dictionary

 \return	void
 */
/*****************************************************************************/
//TODO: No API but static
static void AddIndexAttributes(char* pIndexID, CIndex* pobjIndex,
		CIndex* pobjDictIndex)
{
	if ((NULL == pobjDictIndex) || (NULL == pobjIndex))
	{
#if defined DEBUG
		cout << "AddIndexAttributes FAILED" << endl;
#endif
		return;
	}

	if (NULL == pIndexID)
	{
#if defined DEBUG
		cout << "AddIndexAttributes FAILED - pIndexID empty" << endl;
#endif
		return;
	}
	else
	{
		pobjIndex->setIndexValue(pIndexID);
	}

	//The Value for all the attributes is retrieved from objDictCollection[ObjDict.xml]
	if (NULL != pobjDictIndex->getName())
	{
		pobjIndex->setName((char*) pobjDictIndex->getName());
	}
	else
	{
		pobjIndex->setName((char*) "");
	}

	if (NULL != pobjDictIndex->getObjectType())
	{
		pobjIndex->setObjectType((char*) pobjDictIndex->getObjectType());
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		pobjIndex->setObjectType((char*) "");
	}

	if (NULL != pobjDictIndex->getLowLimit())
	{
		pobjIndex->setLowLimit((char*) pobjDictIndex->getLowLimit());
	}
	else
	{
		pobjIndex->setLowLimit((char*) "");
	}

	if (NULL != pobjDictIndex->getHighLimit())
	{
		pobjIndex->setHighLimit((char*) pobjDictIndex->getHighLimit());
	}
	else
	{
		pobjIndex->setHighLimit((char*) "");
	}

	if (NULL != pobjDictIndex->getAccessType())
	{
		pobjIndex->setAccessType((char*) pobjDictIndex->getAccessType());
	}
	else
	{
		pobjIndex->setAccessType((char*) "");
	}
	char* pbPdoMapStr = NULL;
	if (NULL != pobjDictIndex->getPDOMapping())
	{
		pbPdoMapStr = new char[strlen(pobjDictIndex->getPDOMapping())
				+ STR_ALLOC_BUFFER];
		strcpy(pbPdoMapStr, pobjDictIndex->getPDOMapping());
		pobjIndex->setPDOMapping(pbPdoMapStr);
	}
	else
	{
		pbPdoMapStr = new char[2 + STR_ALLOC_BUFFER];
		strcpy(pbPdoMapStr, (char*) "");
		pobjIndex->setPDOMapping(pbPdoMapStr);
	}
	delete[] pbPdoMapStr;

	if (NULL != pobjDictIndex->getDefaultValue())
	{
		pobjIndex->setDefaultValue((char*) pobjDictIndex->getDefaultValue());
	}
	else
	{
		pobjIndex->setDefaultValue((char*) "");
	}

	if (NULL != pobjDictIndex->getActualValue())
	{
		pobjIndex->setActualValue((char*) pobjDictIndex->getActualValue());
	}
	else
	{
		pobjIndex->setActualValue((char*) "");
	}

	if (NULL != pobjDictIndex->getDataType().getName())
	{
		pobjIndex->setDataTypeST(pobjDictIndex->getDataType());
	}
	else
	{
		pobjIndex->setDataType((char*) "");
	}
}

/*****************************************************************************/
/**
 \brief		AddSubIndexAttributes

 Adds the default attributes to the SubIndex, when addded. The default values are fetched from the object dictionary

 \param		pSubIndexID			Character pointer to hold the IndexID
 \param		pobjSubIndex		Class pointer of CSubIndex to add the attributes
 \param		pobjDictSIndex		Class pointer of CSubIndex from object dictionary

 \return	void
 */
/*****************************************************************************/

//TODO: No API but static
static void AddSubIndexAttributes(char* pSubIndexID, CSubIndex* pobjSubIndex,
		CSubIndex* pobjDictSIndex)
{
	if ((NULL == pobjDictSIndex) || (NULL == pobjSubIndex))
	{
#if defined DEBUG
		cout << "AddSubIndexAttributes FAILED" << endl;
#endif
		return;
	}

	if (NULL == pSubIndexID)
	{
#if defined DEBUG
		cout << "AddSubIndexAttributes FAILED - pSubIndexID NULL" << endl;
#endif
		return;
	}
	else
	{
		// Setting the Index Value for the SUBindex--
		pobjSubIndex->setIndexValue(pSubIndexID);
	}

	//The Value for all the attributes is retrieved from objDictCollection[ObjDict.xml]
	if (NULL != pobjDictSIndex->getName())
	{
		pobjSubIndex->setName((char*) pobjDictSIndex->getName());
	}
	else
	{
		pobjSubIndex->setName((char*) "");
	}

	if (NULL != pobjDictSIndex->getObjectType())
	{
		pobjSubIndex->setObjectType((char*) (pobjDictSIndex->getObjectType()));
	}
	else
	{
		//Setting "0", so default case is hit, when setting
		pobjSubIndex->setObjectType((char*) "");
	}

	if (NULL != pobjDictSIndex->getLowLimit())
	{
		pobjSubIndex->setLowLimit((char*) pobjDictSIndex->getLowLimit());
	}
	else
	{
		pobjSubIndex->setLowLimit((char*) "");
	}

	if (NULL != pobjDictSIndex->getHighLimit())
	{
		pobjSubIndex->setHighLimit((char*) pobjDictSIndex->getHighLimit());
	}
	else
	{
		pobjSubIndex->setHighLimit((char*) "");
	}

	if (NULL != pobjDictSIndex->getAccessType())
	{
		pobjSubIndex->setAccessType((char*) pobjDictSIndex->getAccessType());
	}
	else
	{
		pobjSubIndex->setAccessType((char*) "");
	}

	if (NULL != pobjDictSIndex->getDefaultValue())
	{
		pobjSubIndex->setDefaultValue(
				(char*) pobjDictSIndex->getDefaultValue());
	}
	else
	{
		pobjSubIndex->setDefaultValue((char*) "");
	}

	if (NULL != pobjDictSIndex->getActualValue())
	{
		pobjSubIndex->setActualValue((char*) pobjDictSIndex->getActualValue());
	}
	else
	{
		pobjSubIndex->setActualValue((char*) "");
	}

	if (NULL != pobjDictSIndex->getDataType().getName())
	{
		pobjSubIndex->setDataTypeST(pobjDictSIndex->getDataType());
	}
	else
	{
		pobjSubIndex->setDataType((char*) "");
	}
	return;
}

/*****************************************************************************/
/**
 \brief		CreateNode

 This API shall be used to create a node with the given node ID, node type and node name.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		pbNodeName		Char pointer to hold the name of the node to be created

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode CreateNode(INT32 iNodeID, ENodeType enumNodeType, char* pbNodeName)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		cout << __FUNCTION__ << ": " << enumNodeType << endl;
		if (MN == enumNodeType)
		{
			if (!ObjectDictLoaded)
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
				ObjectDictLoaded = true;
			}
			uiCycleNumber = 0;
		}
		else if (CN == enumNodeType)
		{
			INT32 iNodePos = 0;
			bool bFlag = false;
			cout << "Entering If Node Exists" << endl;
			stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
			cout << "Completed If Node Exists" << endl;
			if ((stErrorInfo.code == OCFM_ERR_SUCCESS) && (bFlag == true))
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);
				throw objException;
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
			//Nothing other than MN & CN
		}
		cout << "Completed" << __LINE__ << endl;
		CNode objNode;
		objNode.setNodeId(iNodeID);
		objNode.setNodeType(enumNodeType);
		if (NULL == pbNodeName)
		{
#if defined DEBUG
			cout << "Err_setting_node_name" << endl;
			objNode.setNodeName((char*) "ERR_NAME");
#endif
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			return (stErrorInfo);
		}
		else
		{
			objNode.setNodeName(pbNodeName);
		}

		objNode.CreateIndexCollection();
		objNode.CreateDataTypeCollection();
		objNode.CreateApplicationProcess();
		objNode.CreateNetworkManagament();
		cout << "Completed" << __LINE__ << endl;
		CNodeCollection* pobjNodeCollection = NULL;
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		cout << "Completed" << __LINE__ << endl;
		pobjNodeCollection->addNode(objNode);
	} catch (ocfmException* ex)
	{
		stErrorInfo = ex->_ocfmRetCode;
		return (stErrorInfo);
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	cout << "Completed" << __LINE__ << endl;
	return (stErrorInfo);
}

/*****************************************************************************/
/**
 \brief		NewProjectNode

 This API shall be used to create a new node(CN or MN)  with attributes nodeid, nodetype, nodename, nodeConfigurationfile(xdd/xdc path)

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		enumNodeType		Enum to hold the Node type of the node
 \param		pbNodeName			Character pointer to hold the name of the node to be created
 \param		pbImportXmlFile		Character pointer to hold the full path of the xml(xdd/xdc) file to be imported for the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode NewProjectNode(INT32 iNodeID, ENodeType enumNodeType,
		char* pbNodeName, char* pbImportXmlFile)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		//Creates the node with Nodeid & nodeName
		if ((NULL == pbNodeName))
		{
#if defined DEBUG
			cout << "pbNodeName Null" << endl;
#endif
			return (stErrorInfo);
		}
		stErrorInfo = CreateNode(iNodeID, enumNodeType, pbNodeName);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			return (stErrorInfo);
		}
		//Import the xdd/xdc for the Node created
		stErrorInfo.code = OCFM_ERR_UNKNOWN;
		if (NULL == pbImportXmlFile)
		{
#if defined DEBUG
			cout << "pbImportXmlFile Null" << endl;
#endif
			return (stErrorInfo);
		}
		stErrorInfo = ImportXML(pbImportXmlFile, iNodeID, enumNodeType);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			return (stErrorInfo);
		}

		stErrorInfo.code = OCFM_ERR_UNKNOWN;
		if (MN == enumNodeType)
		{
			//set the loss of SoC tolerance to 50 ms
			INT32 iIndexPos = 0;
			stErrorInfo = IfIndexExists(iNodeID, enumNodeType, (char *) "1C14",
					&iIndexPos);
			if (OCFM_ERR_SUCCESS == stErrorInfo.code)
			{
				CIndex* pobjIndex = NULL;

				pobjIndex = getMNIndexValues((char*) "1C14");
				if ((NULL != pobjIndex))
				{
					if (NULL != pobjIndex->getActualValue())
					{
						pobjIndex->setActualValue((char*) "50000000");
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
		stErrorInfo = ex->_ocfmRetCode;
		return (stErrorInfo);
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return (stErrorInfo);
}

/*****************************************************************************/
/**
 \brief		DeleteNode

 This API shall be used to delete a node with the given respective node ID and node type. Deleting a node will also delete the object dictionary for the corresponding node.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteNode(INT32 iNodeID, ENodeType enumNodeType)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iNodePos = 0;
	bool bFlag = false;

	stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);

	if (bFlag != true)
	{
		/* Function didnt throw any exception but Node doesnt exist */
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			stErrorInfo.code = OCFM_ERR_NODEID_NOT_FOUND;
		}
		else
		{ /* Function threw exception*/
			// Node Doesn't Exist
		}
		return stErrorInfo;
	}

	CNode objNode;
	CNodeCollection* pobjNodeCollection = NULL;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	if (NULL == pobjNodeCollection)
	{
		stErrorInfo.code = OCFM_ERR_NO_NODES_FOUND;
		return stErrorInfo;
	}
	objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);

	if (CN == objNode.getNodeType())
	{
		ocfmRetCode stErrStructInfo;
		INT32 IndexPos = 0;
		INT32 subIndexPos = 0;
		stErrStructInfo.code = OCFM_ERR_UNKNOWN;
		stErrorInfo.errorString = NULL;

		if (MULTIPLEXED == objNode.getStationType())
		{
			//Deleted MN's 1F9B and Subindex = old node id
			if (NULL != objNode.getForcedCycle())
			{
				CheckAndReAssignMultiplex(objNode.getNodeId(),
						objNode.getForcedCycle());
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

		char* pbSIdx = new char[SUBINDEX_LEN];
		pbSIdx = _IntToAscii(objNode.getNodeId(), pbSIdx, 16);
		pbSIdx = padLeft(pbSIdx, '0', 2);

		stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
				(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, pbSIdx, &subIndexPos,
				&IndexPos);
		if (stErrStructInfo.code == OCFM_ERR_SUCCESS)
		{
			stErrStructInfo = SetSubIndexAttributesByAttribute(MN_NODEID, MN,
					(char*) MULTIPL_CYCLE_ASSIGN_OBJECT, pbSIdx, ACTUALVALUE,
					(char*) "");
			if (stErrStructInfo.code != OCFM_ERR_SUCCESS)
			{
				cout << "1F9B SetSubIndexAttributesByAttribute Failed" << endl;
			}
		}
		else
		{
			//1F9B->subIdx(CN Nodeid) doesnot exists
		}
		stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
				(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, pbSIdx, &subIndexPos,
				&IndexPos);
		if (stErrStructInfo.code == OCFM_ERR_SUCCESS)
		{
			stErrStructInfo = SetSubIndexAttributesByAttribute(MN_NODEID, MN,
					(char*) MNCN_POLLRESPONSE_TIMEOUT_OBJECT, pbSIdx,
					ACTUALVALUE, (char*) "");
			if (stErrStructInfo.code != OCFM_ERR_SUCCESS)
			{
				cout << "1F92 SetSubIndexAttributesByAttribute Failed" << endl;
			}
		}
		else
		{
			//1F92->subIdx(CN Nodeid) doesnot exists
		}
		delete[] pbSIdx;

		//Delete the auto generated pdo indexes also.
		CNode *pobjMNnode = NULL;
		CIndexCollection *pobjMnIdxCol = NULL;
		pobjMNnode = pobjNodeCollection->getNodePtr(MN, MN_NODEID);
		pobjMnIdxCol = pobjMNnode->getIndexCollection();
		INT32 iTotalMNidx = pobjMnIdxCol->getNumberofIndexes();
		cout<< "iTotalMNidx" <<iTotalMNidx<<endl;
		for(INT32 iLoopCount = 0; iLoopCount < iTotalMNidx; iLoopCount++)
		{
			CIndex *pobjMnCommIndex = NULL;
			char* pbSubstr1 = new char[SUBINDEX_LEN];
			char* pbSubstr2 = new char[SUBINDEX_LEN];
			bool b14entry = false;
			bool b18entry = false;

			pobjMnCommIndex = pobjMnIdxCol->getIndex(iLoopCount);
			pbSubstr1 = subString((char*) pobjMnCommIndex->getIndexValue(), 0, 2);
			pbSubstr2 = subString((char*) pobjMnCommIndex->getIndexValue(), 2, 2);
			cout<<"Entry Processing: "<<pbSubstr1<<pbSubstr2<<endl;
			if ((0 == strcmp(pbSubstr1, "14")))
			{
				b14entry = true;
			}
			else if((0 == strcmp(pbSubstr1, "18")))
			{
				b18entry = true;
			}


			if ((b14entry == true) || (b18entry == true))
			{
				CSubIndex *pobjSubIndex = NULL;
				char* pbActValue = new char[SUBINDEX_LEN];
				pobjSubIndex = pobjMnCommIndex->getSubIndexbyIndexValue((char*) "01");
				if(NULL != pobjSubIndex)
				{
					INT32 iTempNodeid;
					pbActValue = (char*)pobjSubIndex->getActualValue(); //NULL
					iTempNodeid = GetDecimalValue(pbActValue);

					if(iNodeID == iTempNodeid)
					{
						CIndex *pbojMnTxIndex = NULL;
						char *pbMnTxIndexId =  new char[INDEX_LEN];
						if(b14entry)
						{
							pbMnTxIndexId = strcpy(pbMnTxIndexId, "16");
							pbMnTxIndexId = strcat(pbMnTxIndexId, pbSubstr2);

							pbojMnTxIndex = pobjMnIdxCol->getIndexbyIndexValue(pbMnTxIndexId);
						}
						else if(b18entry)
						{
							pbMnTxIndexId = strcpy(pbMnTxIndexId, "1A");
							pbMnTxIndexId = strcat(pbMnTxIndexId, pbSubstr2);

							pbojMnTxIndex = pobjMnIdxCol->getIndexbyIndexValue(pbMnTxIndexId);
						}
						else
						{
							//no others will enter
						}

						if (NULL != pbojMnTxIndex)
						{
							ocfmRetCode stErrorInfo;
							pbojMnTxIndex->deleteSubIndexCollection();
							stErrorInfo = AddSubIndex(MN_NODEID, MN, pbMnTxIndexId, (char*) "00");
							if (stErrorInfo.code != OCFM_ERR_SUCCESS)
							{
								cout<<"00th subindex cannot be added"<<endl;
							}
							pobjSubIndex->setActualValue((char*)"00");
						}
						cout<<"subidx Coll deleted"<<endl;
						delete[] pbMnTxIndexId;
					}
				}
				else
				{
					//00th subindex in the communication index is not present
				}
				//delete[] pbActValue;
			}
			delete[] pbSubstr1;
			delete[] pbSubstr2;
		}
	}

	pobjNodeCollection->deleteNode(iNodePos);


#if defined DEBUG
	cout << "Deleted " << enumNodeType << ". NodeID:" << iNodeID << endl;
#endif

	//stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		DeleteIndex

 This API shall be used to delete a particular index of a node given the respective IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		pIndexID		Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteIndex(INT32 iNodeID, ENodeType enumNodeType, char* pIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		INT32 iIndexPos = 0;

		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		CNodeCollection* pobjNodeCollection = NULL;
		CNode* pobjNode = NULL;
		CIndexCollection* pobjIndexCollection = NULL;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}
		//                objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		//                pobjIndexCollection = objNode.getIndexCollection();

		pobjNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeID);
		if (NULL == pobjNode)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		pobjIndexCollection = pobjNode->getIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		pobjIndexCollection->deleteIndex(iIndexPos);
#if defined DEBUG
		cout << "Deleted " << enumNodeType << ". NodeID:" << iNodeID
		<< ". IndexID:" << pIndexID << endl;
#endif

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		DeleteSubIndex

 This API shall be used to delete a particular SuIndex in a particular Index of a node given the respective SubIndexID, IndexID in the required node ID of node type 'NodeType'

 \param		iNodeID			Integer variable to hold the Node Id of a node.
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		pbIndexID		Character pointer to hold the IndexID
 \param		pbSubIndexID	Character pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteSubIndex(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbSubIndexID)
{
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == pbIndexID) || (NULL == pbSubIndexID))
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}
		stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID,
				pbSubIndexID, &iSubIndexPos, &iIndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		CNodeCollection* pobjNodeCollection = NULL;
		CIndexCollection* pobjIndexCollection = NULL;
		CIndex* pobjIndex = NULL;
		CSubIndex* pobjSIDx = NULL;
		CNode objNode;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		//TODO: why not getNodeptr() ?

		pobjIndexCollection = objNode.getIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSIDx = pobjIndex->getSubIndex(iSubIndexPos);
		if (NULL != pobjSIDx)
		{
			//delete the sub-index and then updated the 00th entry
			pobjIndex->deleteSubIndex(iSubIndexPos);

			UpdateNumberOfEnteriesSIdx(pobjIndex, enumNodeType);
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			stErrorInfo.code = OCFM_ERR_SUBINDEXID_NOT_FOUND;
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}
/*****************************************************************************/
/**
 \brief		AddSubIndex

 This API shall be used to add a particular SubIndex to a particular Index of a node given the respective SubIndexID, IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		pbIndexID		Character pointer to hold the IndexID
 \param		pbSubIndexID	Character pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/
ocfmRetCode AddSubIndex(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID,
		char* pbSubIndexID)
{
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == pbIndexID) || (NULL == pbSubIndexID))
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}

		stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID,
				pbSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS == stErrorInfo.code)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_SUBINDEX_ALREADY_EXISTS);
			throw objException;
		}

		if ((OCFM_ERR_NO_SUBINDEXS_FOUND == stErrorInfo.code)
				|| (OCFM_ERR_SUBINDEXID_NOT_FOUND == stErrorInfo.code))
		{
			CNodeCollection* pobjNodeCollection = NULL;
			//CNode objNode;
			CNode *objNode = NULL;
			CIndexCollection* pobjIndexCollection = NULL;
			CIndex* pobjIndex = NULL;

			//                        CIndex objIndex;

			pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
			if (NULL == pobjNodeCollection)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}

			//objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
			objNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeID);

			//pobjIndexCollection = objNode.getIndexCollection();
			pobjIndexCollection = objNode->getIndexCollection();
			if (NULL == pobjIndexCollection)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
				throw objException;
			}

			pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
			if (NULL == pobjIndex)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
				throw objException;
			}
			CSubIndex* pobjSubIndex = NULL;
			CSubIndex* objDictSIdx = NULL;

			/* Get the SubIndex from ObjectDictionary*/
			CObjectDictionary* pobjOBD = NULL;
			pobjOBD = CObjectDictionary::getObjDictPtr();
			if (NULL == pobjOBD)
			{
				stErrorInfo.code = OCFM_ERR_UNKNOWN;
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
				throw objException;
			}

			objDictSIdx = pobjOBD->getObjectDictSubIndex(pbIndexID,
					pbSubIndexID);
			if (NULL != objDictSIdx)
			{
				pobjSubIndex = new CSubIndex;
				pobjSubIndex->setNodeID(iNodeID);
				AddSubIndexAttributes(pbSubIndexID, pobjSubIndex, objDictSIdx);
				if (NULL != pobjIndex)
				{
					pobjIndex->addSubIndex(*pobjSubIndex);
				}
			}
			else if ((enumNodeType == MN)
					&& (true == CheckIfProcessImageIdx(pbIndexID)))
			{
				pobjSubIndex = new CSubIndex;
				pobjSubIndex->setNodeID(iNodeID);
				pobjSubIndex->setIndexValue(pbSubIndexID);
				if (NULL != pobjIndex)
				{
					pobjIndex->addSubIndex(*pobjSubIndex);
				}
			}
			else if (true == CheckIfManufactureSpecificObject(pbIndexID))
			{
				pobjSubIndex = new CSubIndex;
				pobjSubIndex->setNodeID(iNodeID);
				pobjSubIndex->setIndexValue(pbSubIndexID);

				if (NULL != pobjIndex)
				{
					if ((ARRAY == pobjIndex->getEObjectType())
							&& (0 != strcmp(pbSubIndexID, (const char*) "00")))
					{
						//If objectType='ARRAY', all subobjects (except 0x00) have got the same dataType as the object
						pobjSubIndex->setDataTypeST(pobjIndex->getDataType());
					}
					//all the subobjects is of type VAR
					pobjSubIndex->setObjectType((char*) "VAR");
					pobjSubIndex->setFlagIfIncludedCdc(TRUE);
					pobjIndex->addSubIndex(*pobjSubIndex);
				}
			}
			else
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}
			/* Update subindex "00"*/
			if ((NULL != pobjSubIndex)
					&& (NULL != pobjSubIndex->getIndexValue()))
			{
				if (0
						!= strcmp(pobjSubIndex->getIndexValue(),
								(const char*) "00"))
				{
					UpdateNumberOfEnteriesSIdx(pobjIndex, enumNodeType);
				}
			}

			//AddSubIndexAttributes(pbSubIndexID, &pobjSubIndex);

			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		AddSubobject

 This function is to add the 00'th entry for manufacture specific objects automatically while setting all index attributes

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		pbIndexID		Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode AddSubobject(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		if (NULL == pbIndexID)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = AddSubIndex(iNodeID, enumNodeType, pbIndexID,
				(char*) "00");
		if ((OCFM_ERR_SUCCESS == stErrorInfo.code)
				&& (true == CheckIfManufactureSpecificObject(pbIndexID)))
		{
			CNodeCollection* pobjNodeCollection = NULL;
			CNode objNode;
			CIndexCollection* pobjIndexCollection = NULL;

			CIndex* pobjIndex = NULL;
			CSubIndex* pobjSubIndex = NULL;

			INT32 iIndexPos = 0;
			INT32 iSubIndexPos = 0;

			stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID,
					(char*) "00", &iSubIndexPos, &iIndexPos);
			if (OCFM_ERR_SUCCESS != stErrorInfo.code)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
				throw objException;
			}

			pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
			if (NULL == pobjNodeCollection)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}

			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

			pobjIndexCollection = objNode.getIndexCollection();
			if (NULL == pobjIndexCollection)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
				throw objException;
			}

			pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
			if (NULL == pobjIndex)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
				throw objException;
			}

			pobjSubIndex = pobjIndex->getSubIndex(iSubIndexPos);
			if (NULL == pobjSubIndex)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
				throw objException;
			}

			pobjSubIndex->setName((char*) "NumberOfEntries");
			pobjSubIndex->setObjectType((char*) "VAR");
			char* pbDataTypeName = new char[15];

			strcpy(pbDataTypeName, (const char*) "UNSIGNED8");
			if (true == (CheckIfDataTypeByNameExists(pbDataTypeName, iNodeID)))
			{
				pobjSubIndex->setDataType(pbDataTypeName, iNodeID);
			}
			else
			{
				delete[] pbDataTypeName;
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
				throw objException;
			}
			delete[] pbDataTypeName;
		}
		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		AddIndex

 This API shall be used to add a particular Index to a particular node given the respective IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		pbIndexID		Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/
ocfmRetCode AddIndex(INT32 iNodeID, ENodeType enumNodeType, char* pbIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos = 0;
	ocfmException objException;

	try
	{
		if (NULL == pbIndexID)
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS == stErrorInfo.code)
		{
			objException.ocfm_Excpetion(OCFM_ERR_INDEX_ALREADY_EXISTS);
			throw objException;
		}

		if ((OCFM_ERR_NO_INDEX_FOUND == stErrorInfo.code)
				|| (OCFM_ERR_INDEXID_NOT_FOUND == stErrorInfo.code))
		{
			CNodeCollection* pobjNodeCollection = NULL;
			CNode objNode;
			CIndexCollection* pobjIndexCollection = NULL;
			CObjectDictionary* pobjOBD = NULL;
			CIndex* pobjDictIndex = NULL;

			pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
			if (NULL == pobjNodeCollection)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
				throw objException;
			}
			objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
			pobjIndexCollection = objNode.getIndexCollection();
			if (NULL == pobjIndexCollection)
			{
				objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
				throw objException;
			}

			/* Get the Index from ObjectDictionary*/
			pobjOBD = CObjectDictionary::getObjDictPtr();
			if (NULL == pobjOBD)
			{
				stErrorInfo.code = OCFM_ERR_UNKNOWN;
				objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
				throw objException;
			}

			//Validate for TPDO channels for a CN
			if((CN == objNode.getNodeType()) && ( (0 == strncmp(pbIndexID, "1A", 2)) || (0 == strncmp(pbIndexID, "1a", 2))))
			{
				INT32 tpdoCount = 0;
				INT32 rpdoCount = 0;
				objNode.getPDOIndexCollection(&rpdoCount, &tpdoCount);
				#if defined DEBUG
					cout<<"tpdoCount:"<<tpdoCount<<" rpdoCount:"<<rpdoCount<<endl;
				#endif
				//Allowed to add TPDO only if the node has 0 TPDO's(1Axx)
				if(tpdoCount > 0)
				{
					objException.ocfm_Excpetion(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					char acCustomError[200] = { 0 };
					sprintf(acCustomError, "CN cannot have more than one TPDO Channel");
					CopyCustomErrorString(&(objException._ocfmRetCode), acCustomError);
					throw objException;
				}
			}

			pobjDictIndex = pobjOBD->getObjectDictIndex(pbIndexID);
			if (NULL != pobjDictIndex)
			{
				CSubIndex* objSIdx = NULL;
				CIndex objIndex;
				objIndex.setNodeID(iNodeID);
				AddIndexAttributes(pbIndexID, &objIndex, pobjDictIndex);
				for (INT32 iLoopCount = 0;
						iLoopCount < pobjDictIndex->getNumberofSubIndexes();
						iLoopCount++)
				{
					objSIdx = pobjDictIndex->getSubIndex(iLoopCount);
					if (NULL != objSIdx)
					{
						objIndex.addSubIndex(*objSIdx);
					}
				}
				//NULL check for getName
				objIndex.setName(
						pobjOBD->getIndexName(subString(pbIndexID, 2, 4),
								(char*) objIndex.getName()));
				//update the no of entries for the subindex added
				UpdateNumberOfEnteriesSIdx(&objIndex, enumNodeType);
				pobjIndexCollection->addIndex(objIndex);
			}
			else if ((MN == enumNodeType)
					&& (true == CheckIfProcessImageIdx(pbIndexID)))
			{
				CIndex objIndex;
				objIndex.setNodeID(iNodeID);
				objIndex.setIndexValue(pbIndexID);
				pobjIndexCollection->addIndex(objIndex);
			}
			else if (true == CheckIfManufactureSpecificObject(pbIndexID))
			{
				CIndex objIndex;
				objIndex.setNodeID(iNodeID);
				objIndex.setIndexValue(pbIndexID);
				objIndex.setFlagIfIncludedCdc(TRUE);
				pobjIndexCollection->addIndex(objIndex);
			}
			else
			{
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXID);
				throw objException;
			}

			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			// This Part of code is never expected to happen
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXID);
			throw objException;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetIndexAttributes

 This API shall be used to set the index attributes [IndexValue, IndexName & flagIfInCdc] of a particular Index of a node given the respective IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		enumNodeType			Enum to hold the Node type of the node
 \param		pbIndexID				Character pointer to hold the IndexID
 \param		pbIndexValue			Character Pointer to hold the IndexValue
 \param		pbIndexName				Character Pointer to hold the IndexName
 \param		enumIsIncludedInCdc		Enumeration Flag to flagIfInCdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetIndexAttributes(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbIndexValue, char* pbIndexName,
		EFlag enumIsIncludedInCdc)
{
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		if ((NULL == pbIndexID) || (NULL == pbIndexValue)
				|| (NULL == pbIndexName))
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		CNodeCollection* pobjNodeCollection = NULL;
		CNode objNode;
		CIndexCollection* pobjIndexCollection = NULL;
		CIndex* pobjIndex = NULL;
		CIndex objIndex;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_NODES_FOUND;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		objIndex.setNodeID(iNodeID);
		pobjIndexCollection = objNode.getIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_INDEX_FOUND;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			stErrorInfo.code = OCFM_ERR_INDEXID_NOT_FOUND;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjIndex->setName(pbIndexName);
		pobjIndex->setFlagIfIncludedCdc(enumIsIncludedInCdc);

		if (NULL != pbIndexValue)
		{
			if (true == pobjIndex->IsIndexVaueValid(pbIndexValue))
			{
				pobjIndex->setActualValue(pbIndexValue);
				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetSubIndexAttributes

 This API shall be used to set the SubIndex attributes [IndexValue, IndexName & flagIfInCdc] of a particular SuIndex in a particular Index of a node given the respective SubIndexID, IndexID in the required node ID of node type 'NodeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		enumNodeType			Enum to hold the Node type of the node
 \param		pbIndexID				Character pointer to hold the IndexID
 \param		pbSubIndexID			Character pointer to hold the SubIndexID
 \param		pbIndexValue			Character Pointer to hold the IndexValue
 \param		pbIndexName				Character Pointer to hold the IndexName
 \param		enumIsIncludedInCdc		Enumeration Flag to flagIfInCdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetSubIndexAttributes(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbSubIndexID, char* pbIndexValue,
		char* pbIndexName, EFlag enumIsIncludedInCdc)
{
	INT32 iIndexPos = 0;
	INT32 iSubIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == pbIndexID) || (NULL == pbSubIndexID)
				|| (NULL == pbIndexValue) || (NULL == pbIndexName))
		{
			stErrorInfo.code = OCFM_ERR_UNKNOWN;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID,
				pbSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		if ((MN_NODEID == iNodeID) && (MN == enumNodeType)
				&& (0 == strcmp(pbIndexID, "1F92"))
				&& (0 != strcmp(pbSubIndexID, "00")))
		{
			if (false == ValidateCNPresTimeout(pbSubIndexID, pbIndexValue))
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_LOW_CNPRESTIMEOUT);
				throw objException;
			}
		}

		CNodeCollection* pobjNodeCollection = NULL;
		CNode objNode;
		CIndexCollection* pobjIndexCollection = NULL;
		CIndex* pobjIndex = NULL;
		CSubIndex* pobjSubIndex = NULL;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSubIndex = pobjIndex->getSubIndex(iSubIndexPos);
		if (NULL == pobjSubIndex)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSubIndex->setName(pbIndexName);
		pobjSubIndex->setFlagIfIncludedCdc(enumIsIncludedInCdc);

		if (pobjSubIndex->IsIndexVaueValid(pbIndexValue))
		{
			pobjSubIndex->setActualValue(pbIndexValue);
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
			throw objException;
		}

		if ((MN_NODEID == iNodeID) && (MN == enumNodeType)
				&& (0 == strcmp(pbIndexID, (const char*) "1F98"))
				&& (0 == strcmp(pbSubIndexID, (const char*) "07")))
		{
			stErrorInfo = RecalculateMultiplex();
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetALLIndexAttributes

 This API shall be used to set all the attributes of an Index in a single shot.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		enumNodeType		Enum to hold the Node type of the node
 \param		pbIndexID			Character pointer to hold the IndexID
 \param		pbActualValue		Character pointer to hold the ActualValue
 \param		pbIndexName			Character pointer to hold the IndexName
 \param		pbAccess			Character pointer to hold the Access type
 \param		pbDataTypeName		Character pointer to hold the Datatype
 \param		pdoMappingVal		Character pointer to hold the PDO Mapping
 \param		pbdefaultValue		Character pointer to hold the Default value
 \param		pbhighLimit			Character pointer to hold the Highlimit
 \param		pblowLimit			Character pointer to hold the Lowlimit
 \param		pbobjType			Character pointer to hold the Object type
 \param		enumIncludeInCdc	Enumeration flag for include in cdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetAllIndexAttributes(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbActualValue, char* pbIndexName, char* pbAccess,
		char* pbDataTypeName, char* pdoMappingVal, char* pbdefaultValue,
		char* pbhighLimit, char* pblowLimit, char* pbobjType,
		EFlag enumIncludeInCdc)
{
	INT32 iIndexPos;
	ocfmRetCode stErrorInfo;
	try
	{
		// || (NULL == pbActualValue) || (NULL == pbIndexName) || (NULL == pbAccess) || (NULL == pbDataTypeName) || (NULL == pdoMappingVal) || (NULL == pbdefaultValue) || (NULL == pbhighLimit) || (NULL == pblowLimit) || (NULL == pbobjType)
		if ((NULL == pbIndexID))
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID,
				&iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		CNodeCollection* pobjNodeCollection = NULL;
		CNode objNode;
		CIndexCollection* pobjIndexCollection = NULL;
		CIndex* pobjIndex = NULL;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_NODES_FOUND;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			stErrorInfo.code = OCFM_ERR_NO_INDEX_FOUND;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			stErrorInfo.code = OCFM_ERR_INDEXID_NOT_FOUND;
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		/* Check if the value is valid*/
		if (NULL != pbIndexName)
		{
			if (strcmp(pbIndexName, "") != 0)
				pobjIndex->setName(pbIndexName);
		}

		if (NULL != pbAccess)
		{
			if (strcmp(pbAccess, "") != 0)
				pobjIndex->setAccessType(pbAccess);
		}

		if (NULL != pdoMappingVal)
		{
			pobjIndex->setPDOMapping(pdoMappingVal);
		}
		if (NULL != pbdefaultValue)
		{
			pobjIndex->setDefaultValue(pbdefaultValue);
		}

		ocfmRetCode stErrorLimitInfo;
		stErrorLimitInfo.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
		if ((NULL != pbhighLimit) && (NULL != pblowLimit))
		{
			stErrorLimitInfo = CheckUpperAndLowerLimits(pblowLimit,
					pbhighLimit);
			if (OCFM_ERR_SUCCESS == stErrorLimitInfo.code)
			{
				pobjIndex->setHighLimit(pbhighLimit);
				pobjIndex->setLowLimit(pblowLimit);
			}
			else
			{
				return stErrorLimitInfo;
			}
		}

		if (NULL != pbobjType)
		{
			pobjIndex->setObjectType(pbobjType);
			if (pobjIndex->getEObjectType() == ARRAY)
			{
				pobjIndex->UpdateArraySubObjects();
			}
		}

		pobjIndex->setFlagIfIncludedCdc(enumIncludeInCdc);

		if ((ARRAY == pobjIndex->getEObjectType())
				|| (RECORD == pobjIndex->getEObjectType()))
		{
			stErrorInfo = AddSubobject(iNodeID, enumNodeType, pbIndexID);
		}

		if (NULL != pbDataTypeName)
		{
			if (strcmp(pbDataTypeName, "") != 0)
			{
				if ((CheckIfDataTypeByNameExists(pbDataTypeName,
						pobjIndex->getNodeID())) == true)
				{
					pobjIndex->setDataType(pbDataTypeName, iNodeID);
					DataType objDataType;
					objDataType = pobjIndex->getDataType();
					if (pobjIndex->getEObjectType() == ARRAY)
					{
						pobjIndex->UpdateArraySubObjects();
					}
				}
				else
				{
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
					throw objException;
				}
			}
		}
		if (NULL != pbActualValue)
		{
			if (pobjIndex->IsIndexVaueValid(pbActualValue))
			{
				pobjIndex->setActualValue(pbActualValue);

				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetAllSubIndexAttributes

 This API shall be used to set all the attributes of an SubIndex in a single shot.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		enumNodeType		Enum to hold the Node type of the node
 \param		pbIndexID			Character pointer to hold the IndexID
 \param		pbSubIndexID		Character pointer to hold the SubIndexID
 \param		pbActualValue		Character pointer to hold the ActualValue
 \param		pbIndexName			Character pointer to hold the IndexName
 \param		pbAccess			Character pointer to hold the Access type
 \param		pbDataTypeName		Character pointer to hold the Datatype
 \param		pbPdoMappingVal		Character pointer to hold the PDO Mapping
 \param		pbDefaultValue		Character pointer to hold the Default value
 \param		pbHighLimit			Character pointer to hold the Highlimit
 \param		pbLowLimit			Character pointer to hold the Lowlimit
 \param		pbObjType			Character pointer to hold the Object type
 \param		enumIncludeInCdc	Enumeration flag for include in cdc

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetAllSubIndexAttributes(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbSubIndexID, char* pbActualValue,
		char* pbIndexName, char* pbAccess, char* pbDataTypeName,
		char* pbPdoMappingVal, char* pbDefaultValue, char* pbHighLimit,
		char* pbLowLimit, char* pbObjType, EFlag enumIncludeInCdc)
{
	INT32 iIndexPos = 0;
	INT32 iSubIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == pbIndexID) || (NULL == pbSubIndexID))
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}

		stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID,
				pbSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		CNodeCollection* pobjNodeCollection = NULL;
		CNode objNode;
		CIndexCollection* pobjIndexCollection = NULL;
		CIndex* pobjIndex = NULL;
		CSubIndex* pobjSubIndex = NULL;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		pobjSubIndex = pobjIndex->getSubIndex(iSubIndexPos);
		if (NULL == pobjSubIndex)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		if (NULL != pbIndexName)
		{
			pobjSubIndex->setName(pbIndexName);
		}

		if (NULL != pbAccess)
		{
			pobjSubIndex->setAccessType(pbAccess);
		}

		if (NULL != pbPdoMappingVal)
		{
			pobjSubIndex->setPDOMapping(pbPdoMappingVal);
		}

		if (NULL != pbDefaultValue)
		{
			pobjSubIndex->setDefaultValue(pbDefaultValue);
		}

		ocfmRetCode stErrorLimitInfo;
		stErrorLimitInfo.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
		if ((NULL != pbHighLimit) && (NULL != pbLowLimit))
		{
			stErrorLimitInfo = CheckUpperAndLowerLimits(pbLowLimit,
					pbHighLimit);
			if (OCFM_ERR_SUCCESS == stErrorLimitInfo.code)
			{
				pobjSubIndex->setHighLimit(pbHighLimit);
				pobjSubIndex->setLowLimit(pbLowLimit);
			}
			else
			{
				return stErrorLimitInfo;
			}
		}

		if (NULL != pbObjType)
		{
			pobjSubIndex->setObjectType(pbObjType);
		}

		pobjSubIndex->setFlagIfIncludedCdc(enumIncludeInCdc);

		if (NULL != pbActualValue)
		{
			if (pobjSubIndex->IsIndexVaueValid(pbActualValue))
			{
				pobjSubIndex->setActualValue(pbActualValue);
				stErrorInfo.code = OCFM_ERR_SUCCESS;
			}
			else
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
				throw objException;
			}
		}
		if (NULL != pbDataTypeName)
		{
			if (0 != strcmp(pbDataTypeName, (const char*) ""))
			{
				if (true
						== (CheckIfDataTypeByNameExists(pbDataTypeName,
								pobjSubIndex->getNodeID())))
				{
					pobjSubIndex->setDataType(pbDataTypeName, iNodeID);
				}
				else
				{
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_DATATYPE_NOT_FOUND);
					throw objException;
				}
			}
		}

	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SetSubIndexAttributesByAttribute

 This function shall be used to set any SubIndex attributes of a particular SuIndex in a particular Index of a node by selecting any of the types from the enumerator EAttributeType

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		enumNodeType			Enum to hold the Node type of the node
 \param		pbIndexID				Character pointer to hold the IndexID
 \param		pbSubIndexID			Character pointer to hold the SubIndexID
 \param		enumAttributeType		Enum to hold the attribute type of the subindex
 \param		pbInAttributeValue		Character pointer to hold the value to be set for the subindex attribute

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetSubIndexAttributesByAttribute(INT32 iNodeID,
		ENodeType enumNodeType, char* pbIndexID, char* pbSubIndexID,
		EAttributeType enumAttributeType, char* pbInAttributeValue)
{
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		if ((NULL == pbIndexID) || (NULL == pbSubIndexID))
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
			throw objException;
		}
		stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID,
				pbSubIndexID, &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS != stErrorInfo.code)
		{
			return stErrorInfo;
		}

		CNodeCollection* pobjNodeCollection = NULL;
		CNode objNode;
		CIndexCollection* pobjIndexCollection = NULL;
		CIndex* pobjIndex = NULL;
		CSubIndex* pobjSubIndex = NULL;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}

		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		if (NULL == pobjIndexCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);
		if (NULL == pobjIndex)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjSubIndex = pobjIndex->getSubIndex(iSubIndexPos);
		if (NULL == pobjSubIndex)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}

		switch (enumAttributeType)
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
			if (NULL != pbInAttributeValue)
				pobjSubIndex->setActualValue(pbInAttributeValue);
			else
				pobjSubIndex->setActualValue((char*) "");
			break;
		case PDOMAPPING:
			// No Operation
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
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		CheckUpperAndLowerLimits

 This function shall be used to validate the upper & lower limit values while setting the Index/Subindex attributes limits

 \param		pcLowLimit		Character pointer to hold the lowerlimit value
 \param		pcHighLimit		Character pointer to hold the higherlimit value

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode CheckUpperAndLowerLimits(char* pcLowLimit, char* pcHighLimit)
{
	ocfmRetCode stError;
	stError.code = OCFM_ERR_SUCCESS;

	if ((NULL != pcLowLimit) && (NULL != pcHighLimit))
	{
		if ((0 != strcmp(pcLowLimit, "")) && (0 != strcmp(pcHighLimit, "")))
		{
			ULONG ulLowlimit;
			ULONG ulHighLimit;
			if (true == CheckIfHex((char*) pcLowLimit))
			{
				ulLowlimit = hex2int(
						subString((char*) pcLowLimit, 2,
								strlen(pcLowLimit) - 2));
			}
			else
			{
				ulLowlimit = atoi(pcLowLimit);
			}

			if (true == CheckIfHex((char*) pcHighLimit))
			{
				ulHighLimit = hex2int(
						subString((char*) pcHighLimit, 2,
								strlen(pcHighLimit) - 2));
			}
			else
			{
				ulHighLimit = atoi(pcHighLimit);
			}

			if (ulHighLimit >= ulLowlimit)
			{
				return stError;
			}
			else
			{
				stError.code = OCFM_ERR_INVALID_UPPERLOWER_LIMITS;
				stError.errorString = new char[150];
				INT32 iErrSprintf = 0;
				stError.errorString[0] = 0;
				iErrSprintf = sprintf(stError.errorString,
						"The lower limit(%s) is greater than upperlimit(%s)",
						pcLowLimit, pcHighLimit);
				if (iErrSprintf < 0)
				{
					cout << "Sprintf Error:" << __FUNCTION__ << endl;
				}
				else
				{
					//sprintf Success
				}
				return stError;
			}
		}
	}
	return stError;
}

/*****************************************************************************/
/**
 \brief		EnableDisableMappingPDO

 This function Enables or Disables the Mapping pdo's before the pdo contents are written to the cdc by setting the EnableFlag to true or false respectiveley

 \param		pobjIdxCol		Class pointer of CIndexCollection for a single node
 \param		pobjIndex		Class pointer of CIndex for the PDO index
 \param		Buffer			Character pointer to hold the values to be written to the cdc
 \param		EnableFlag		Boolean value to enable or disable the pdo

 \return	void
 */
/*****************************************************************************/

void EnableDisableMappingPDO(CIndexCollection* pobjIdxCol, CIndex* pobjIndex,
		char* Buffer, bool EnableFlag)
{
//TODO: objException thrown is Not catched. Try Catch block needed
	// INT32 len;

	//Get the Index Value
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	if (NULL == pobjIndex)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
		throw objException;
	}

	if (NULL == Buffer)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		throw objException;
	}

	strcpy(Buffer, "");
	if (0 != pobjIndex->getNumberofSubIndexes())
	{

		//bool resetValueAdded = false;
		CSubIndex* pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*) "00");
		if (NULL != pobjSubIndex)
		{
			if (NULL != pobjSubIndex->getActualValue())
			{
				INT32 noOfSubIndexes = 0; //= pobjIndex->getNumberofSubIndexes();
				if (true == CheckIfHex((char*) pobjSubIndex->getActualValue()))
				{
					noOfSubIndexes = hex2int(
							subString((char*) pobjSubIndex->getActualValue(), 2,
									strlen(pobjSubIndex->getActualValue())
											- 2));
				}
				else
				{
					noOfSubIndexes = atoi(pobjSubIndex->getActualValue());
				}

				/* No need to reinitailize mapping pdo to zero again */
				if ((0 == noOfSubIndexes) && (true == EnableFlag))
				{
					return;
				}

				if ((TRUE == pobjSubIndex->getFlagIfIncludedCdc())
						&& ((true == ReactivateMappingPDO(pobjIdxCol, pobjIndex))
								|| (true
										== IsDefaultActualNotEqual(pobjSubIndex))))
				{
					bool IfStringDT = false;
					//Format: 1600    01   00000000   Act_value
					strcat(Buffer, pobjIndex->getIndexValue());
					//Place a tab
					strcat(Buffer, "\t");

					strcat(Buffer, pobjSubIndex->getIndexValue());
					strcat(Buffer, "\t");
					//Add datatype
					DataType dt;
					dt = pobjSubIndex->getDataType();
					INT32 padLength = 0;
					char* pbDataSize = new char[8 + STR_ALLOC_BUFFER];

					if (NULL != dt.Name)
					{
						if (true == checkIfStringDatatypes(dt.Name))
						{
							INT32 len = strlen(pobjSubIndex->getActualValue());
							pbDataSize = _IntToAscii(len, pbDataSize, 16);
							pbDataSize = padLeft(pbDataSize, '0', 8);
							strcat(Buffer, pbDataSize);
							padLength = len * 2;
							IfStringDT = true;
						}
						else
						{
							pbDataSize = _IntToAscii(dt.DataSize, pbDataSize,
									16);
							pbDataSize = padLeft(pbDataSize, '0', 8);
							strcat(Buffer, pbDataSize);
							padLength = dt.DataSize * 2;
							IfStringDT = false;
						}
					}
					else
					{
						//txt2cdc will not be success
						strcat(Buffer, (const char*) "00000000");
					}
					delete[] pbDataSize;
					strcat(Buffer, "\t");

					if (false == EnableFlag)
					{
						// Actual value checked for Empty check for 00'th subindex non-Zero value.
						if ((0 != strcmp(pobjSubIndex->getActualValue(), ""))
								&& !(checkIfValueZero(
										(char*) pobjSubIndex->getActualValue())))
						{
							char actvalue[20];
							strcpy(actvalue, "0");
							strcat(Buffer, padLeft(actvalue, '0', padLength));
						}
						else
						{
							//No need to enable/disable the mapping pdo if actual value set to zero or empty
						}
					}
					else
					{
						char actvalue[64];
						actvalue[0] = '\0';

						if (true == IfStringDT)
						{
							strcpy(actvalue,
									(char*) pobjSubIndex->getActualValue());
							strcpy(actvalue,
									ConvertStringToHex((char*) actvalue));
							strcat(Buffer, actvalue);
						}
						else
						{
							//non empty non-zero actual values are only written to cdc
							if ((0 != strcmp(pobjSubIndex->getActualValue(), ""))
									&& (!(checkIfValueZero(
											(char*) pobjSubIndex->getActualValue()))))
							{
								if (true
										== CheckIfHex(
												(char*) pobjSubIndex->getActualValue()))
								{
									INT32 ilen =
											strlen(
													(char*) pobjSubIndex->getActualValue());
									strncpy(actvalue,
											((char*) (pobjSubIndex->getActualValue()
													+ 2)), ilen - 2);
									actvalue[ilen - 2] = '\0';

									strcat(Buffer,
											padLeft(actvalue, '0', padLength));
								}
								else
								{
									strcpy(actvalue,
											_IntToAscii(
													atoi(
															pobjSubIndex->getActualValue()),
													actvalue, 16));
									strcat(Buffer,
											padLeft(actvalue, '0', padLength));
								}
							}
						}
					}
					strcat(Buffer, "\n");
				}
			}
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND);
#if defined DEBUG
			cout << "NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND: Index: " << pobjIndex->getIndexValue() << endl;
#endif
			throw objException;
		}
	}
	else
	{
#if defined DEBUG
		cout << "INDEX: " << pobjIndex->getIndexValue() << " has Zero SubIndexs"
		<< endl;
#endif
	}
}

/*****************************************************************************/
/**
 \brief		UpdateCNCycleTime

 This function updates the cycle time index value(1006) of the CN

 \param		pobjIdxCol		Class pointer of CIndexCollection for a CN node
 \param		pbCycleTime		Character pointer to hold the new cycle time value

 \return	void
 */
/*****************************************************************************/

void UpdateCNCycleTime(CIndexCollection* pobjIdxCol, char* pbCycleTime)
{
	CIndex* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1006");
	if (NULL != pobjIndex)
	{
		if (NULL != pbCycleTime)
		{
			strcpy(pbCycleTime, ConvertToUpper(pbCycleTime));
			pobjIndex->setActualValue(pbCycleTime);
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

/*****************************************************************************/
/**
 \brief		UpdateCNSoCTolerance

 This function updates the Loss SoC Tolerance index value(1C14) of the CN

 \param		pobjIdxCol			Class pointer of CIndexCollection for a CN node
 \param		pbSocTolerance		Character pointer to hold the new Loss SoC Tolerance value

 \return	void
 */
/*****************************************************************************/

void UpdateCNSoCTolerance(CIndexCollection* pobjIdxCol, char* pbSocTolerance)
{
	CIndex* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1C14");
	if (NULL != pobjIndex)
	{
		if (NULL != pbSocTolerance)
		{
			pobjIndex->setActualValue(pbSocTolerance);
			pobjIndex->setFlagIfIncludedCdc(TRUE);
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

/*****************************************************************************/
/**
 \brief		UpdateCNAsyncMTUsize

 This function updates the Asynchronous MTU size(1F98/08) of the CN

 \param		pobjIdxCol			Class pointer of CIndexCollection for a CN node
 \param		pbAsyncMTUsize		Character pointer to hold the new AsyncMTUsize value

 \return	void
 */
/*****************************************************************************/

void UpdateCNAsyncMTUsize(CIndexCollection* pobjIdxCol, char* pbAsyncMTUsize)
{
	CIndex* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1F98");
	if (NULL != pobjIndex)
	{
		CSubIndex* pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*) "08");
		if (NULL != pobjSubIndex)
		{
			if (NULL != pbAsyncMTUsize)
			{
				pobjSubIndex->setActualValue(pbAsyncMTUsize);
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

/*****************************************************************************/
/**
 \brief		UpdateCNMultiPrescal

 This function updates the Multilexing prescaler (1F98/07) of the CN

 \param		pobjIdxCol			Class pointer of CIndexCollection for a CN node
 \param		pbMultiPrescal		Character pointer to hold the new Multilexing prescaler value

 \return	void
 */
/*****************************************************************************/

void UpdateCNMultiPrescal(CIndexCollection* pobjIdxCol, char* pbMultiPrescal)
{
	CIndex* pobjIndex = NULL;
	if (NULL == pobjIdxCol)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1F98");
	if (NULL != pobjIndex)
	{
		CSubIndex* pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*) "07");
		if (NULL != pobjSubIndex)
		{
			if (NULL != pbMultiPrescal)
			{
				pobjSubIndex->setActualValue(pbMultiPrescal);
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

/*****************************************************************************************/
/**
 \brief		UpdateCNNodeAssignment

 This function updates the subobjects of 1F81 in CN to MN if conditions are satisfied

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/
//TODO: unused function
void UpdateCNNodeAssignment(CNode* pobjNode)
{
	bool bCopy1F81 = false;
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	bCopy1F81 = ISCNNodeAssignmentValid(pobjNode);

	if (true == bCopy1F81)
	{
		CIndexCollection* objCNIndexCollection = NULL;
		CIndex* pobjCN1F81idx = NULL;

		objCNIndexCollection = pobjNode->getIndexCollection();
		if (NULL == objCNIndexCollection)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		pobjCN1F81idx = objCNIndexCollection->getIndexbyIndexValue(
				(char*) "1F81");
		if (NULL == pobjCN1F81idx)
		{
#if defined DEBUG
			cout << "CN: " << pobjNode->getNodeId() << " INDEX: 1F81 Not found"
			<< endl;
#endif
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjCN1F81idx->setFlagIfIncludedCdc(TRUE);

		CIndex* pobjMN1F81idx = NULL;
		pobjMN1F81idx = getMNIndexValues((char*) "1F81");
		if (NULL == pobjMN1F81idx)
		{
#if defined DEBUG
			cout << "MN INDEX: 1F81 Not found" << endl;
#endif
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		CSubIndex* pobjCN1F81Sidx = NULL;
		CSubIndex* pobjMN1F81Sidx = NULL;

		for (INT32 iLoopCount = 0;
				iLoopCount < pobjCN1F81idx->getNumberofSubIndexes();
				iLoopCount++)
		{
			pobjCN1F81Sidx = pobjCN1F81idx->getSubIndex(iLoopCount);
			if (NULL == pobjCN1F81Sidx)
			{
				continue;
			}

			pobjMN1F81Sidx = pobjMN1F81idx->getSubIndexbyIndexValue(
					(char*) pobjCN1F81Sidx->getIndexValue());

			if (NULL == pobjMN1F81Sidx)
			{
				continue;
			}

			if (NULL != pobjMN1F81Sidx->getActualValue())
			{
				pobjCN1F81Sidx->setActualValue(
						(char*) pobjMN1F81Sidx->getActualValue());
			}
			else
			{
				pobjCN1F81Sidx->setActualValue((char*) "");
			}
			pobjCN1F81Sidx->setFlagIfIncludedCdc(TRUE);
		}
	}
	else
	{
#if defined DEBUG
		cout << "CN Node Assignment Invalid" << endl;
#endif
	}
}

/*****************************************************************************************/
/**
 \brief		ISCNNodeAssignmentValid

 This function checks whether the CN can implement 1F81 object or not

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	BOOL

 \retval		TRUE			if a CN can implement 1F81 object
 \retval		FALSE			if a CN cannot implement 1F81 object
 */
/******************************************************************************************/

bool ISCNNodeAssignmentValid(CNode* pobjNode)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos = 0;
	INT32 iSubIndexPos = 0;
	INT32 iNodeId = 0;
	bool bCopy1F81 = false;
	ENodeType enumNodeType;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	iNodeId = pobjNode->getNodeId();
	enumNodeType = pobjNode->getNodeType();

	stErrorInfo = IfIndexExists(iNodeId, enumNodeType, (char*) "1F9B",
			&iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		stErrorInfo = IfSubIndexExists(iNodeId, enumNodeType, (char*) "1F98",
				(char*) "07", &iSubIndexPos, &iIndexPos);
		if (OCFM_ERR_SUCCESS == stErrorInfo.code)
		{
			char* pcMultiplCycleCnt = new char[20];
			stErrorInfo = GetSubIndexAttributes(iNodeId, enumNodeType,
					(char*) "1F98", (char*) "07", ACTUALVALUE,
					pcMultiplCycleCnt);
			if (OCFM_ERR_SUCCESS == stErrorInfo.code)
			{
				if ((NULL != pcMultiplCycleCnt)
						&& (0 != strcmp(pcMultiplCycleCnt, ""))
						&& !(checkIfValueZero(pcMultiplCycleCnt)))
				{
					bCopy1F81 = true;
				}
			}
			else
			{
#if defined DEBUG
				cout << enumNodeType << ":" << iNodeId
				<< "1F9B/07 GetSubIndexAttributes Failed" << endl;
#endif
			}
			delete[] pcMultiplCycleCnt;
		}
		else
		{
#if defined DEBUG
			cout << enumNodeType << ":" << iNodeId << "1F9B/07 Not Found"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << enumNodeType << ":" << iNodeId << "1F9B Not Found" << endl;
#endif
	}

	stErrorInfo = IfIndexExists(iNodeId, enumNodeType, (char*) "1016",
			&iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		bCopy1F81 = true;
	}
	else
	{
#if defined DEBUG
		cout << enumNodeType << ":" << iNodeId << "1016 Not Found" << endl;
#endif
	}

	stErrorInfo = IfIndexExists(iNodeId, enumNodeType, (char*) "1F8D",
			&iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		bCopy1F81 = true;
	}
	else
	{
#if defined DEBUG
		cout << enumNodeType << ":" << iNodeId << "1F8D Not Found" << endl;
#endif
	}
	return bCopy1F81;
}

/*****************************************************************************************/
/**
 \brief		UpdateCNMultipleCycleAssign

 This function copies all the subobjects of 1F9B in MN to CN if conditions are satisfied

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdateCNMultipleCycleAssign(CNode* pobjNode)
{
	CIndexCollection* pobjCNIdxCol = NULL;
	CIndex* pobjCN1f9bIdx = NULL;
	CIndex* pobjMN1f9bIdx = NULL;
	CSubIndex* pobjCN1f9bSidx = NULL;
	CSubIndex* pobjMN1f9bSidx = NULL;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	pobjCNIdxCol = pobjNode->getIndexCollection();

	if (NULL == pobjCNIdxCol)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
		throw objException;
	}

	pobjCN1f9bIdx = pobjCNIdxCol->getIndexbyIndexValue((char*) "1F9B");
	if (NULL == pobjCN1f9bIdx)
	{
		return;
	}

	pobjCN1f9bIdx->setFlagIfIncludedCdc(TRUE);
	pobjMN1f9bIdx = getMNIndexValues((char*) "1F9B");
	if (NULL == pobjMN1f9bIdx)
	{
		return;
	}

	for (INT32 iLoopCount = 0;
			iLoopCount < pobjCN1f9bIdx->getNumberofSubIndexes(); iLoopCount++)
	{

		pobjCN1f9bSidx = pobjCN1f9bIdx->getSubIndex(iLoopCount);
		if (NULL == pobjCN1f9bSidx)
			continue;

		if (NULL != pobjCN1f9bSidx->getIndexValue())
		{
			pobjMN1f9bSidx = pobjMN1f9bIdx->getSubIndexbyIndexValue(
					(char*) pobjCN1f9bSidx->getIndexValue());
		}

		if (NULL == pobjMN1f9bSidx)
			continue;

		if (NULL != pobjMN1f9bSidx->getActualValue())
		{
			pobjCN1f9bSidx->setActualValue(
					(char*) pobjMN1f9bSidx->getActualValue());
		}
		else
		{
			pobjCN1f9bSidx->setActualValue((char*) "");
		}
		pobjCN1f9bSidx->setFlagIfIncludedCdc(TRUE);
	}
}

/*****************************************************************************************/
/**
 \brief		UpdateCNPresMNActLoad

 This function set the calculated PresMN load value in CN 1F8D object

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdateCNPresMNActLoad(CNode* pobjNode)
{
	ocfmRetCode stErrStructInfo;
	stErrStructInfo.code = OCFM_ERR_UNKNOWN;
	stErrStructInfo.errorString = NULL;

	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;
	INT32 iNodeId = 0;
	ENodeType eNodeType;

	CNodeCollection* pobjNodeCollection = NULL;
	CIndexCollection* pobjIndexCollection = NULL;
	CIndex* pobj1F8Didx = NULL;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	if (NULL == pobjNodeCollection)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
		throw objException;
	}

	iNodeId = pobjNode->getNodeId();
	eNodeType = pobjNode->getNodeType();
	stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char*) "1F8D",
			(char*) "F0", &subIndexPos, &IndexPos);
	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
#if defined DEBUG
		cout << "UpdateCNPresMNActLoad falied. 1F8D/F0 does not exitst" << endl;
#endif
		return;
	}

	char *pcSubindexId = new char[SUBINDEX_LEN];
	strcpy(pcSubindexId, (char*) "F0");

	pobjIndexCollection = pobjNode->getIndexCollection();
	if (NULL == pobjIndexCollection)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobj1F8Didx = pobjIndexCollection->getIndexbyIndexValue((char*) "1F8D");
	if (NULL == pobj1F8Didx)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
		throw objException;
	}
	else
	{
		CSubIndex *pobj1f8dSidx = NULL;
		pobj1f8dSidx = pobj1F8Didx->getSubIndexbyIndexValue(pcSubindexId);
		if (NULL == pobj1f8dSidx)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			throw objException;
		}
		else
		{
			if (CHAINED == pobjNode->getStationType())
			{
				char convalue[20];
				char actvalue[22];
				CNode pobjMNnode;

				memset(convalue, 0, 20 * sizeof(char));
				memset(actvalue, 0, 22 * sizeof(char));
				pobjMNnode = pobjNodeCollection->getMNNode();

				if (PRES_DEFAULT_PAYLOAD > pobjMNnode.getPResActPayloadValue())
				{
					_IntToAscii(PRES_DEFAULT_PAYLOAD, convalue, 16);
				}
				else
				{
					_IntToAscii(pobjMNnode.getPResActPayloadValue(), convalue,
							16);
				}
				strcpy(actvalue, (char*) "0x");
				strcat(actvalue, convalue);
				pobj1f8dSidx->setActualValue(actvalue);

				pobj1F8Didx->setFlagIfIncludedCdc(TRUE);
				pobj1f8dSidx->setFlagIfIncludedCdc(TRUE);
			}
			else
			{
				pobj1f8dSidx->setActualValue((char*) "");
				pobj1f8dSidx->setFlagIfIncludedCdc(FALSE);
			}
		}
	}
	delete[] pcSubindexId;
}

/*****************************************************************************************/
/**
 \brief		UpdatePreqActLoad

 This function set the calculated Preq load value in CN

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdatePreqActLoad(CNode* pobjNode)
{
	ocfmRetCode stErrStructInfo;
	stErrStructInfo.code = OCFM_ERR_UNKNOWN;
	stErrStructInfo.errorString = NULL;

	CIndexCollection* pobjIndexColl = NULL;
	CIndex* pobj1f9bIdx = NULL;

	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;
	INT32 iNodeId = 0;
	ENodeType eNodeType;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	iNodeId = pobjNode->getNodeId();
	eNodeType = pobjNode->getNodeType();
	stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char*) "1F98",
			(char*) "04", &subIndexPos, &IndexPos);
	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
#if defined DEBUG
		cout << "UpdatePreqActLoad falied. 1F98/04 does not exitst" << endl;
#endif
		return;
	}

	pobjIndexColl = pobjNode->getIndexCollection();
	if (NULL == pobjIndexColl)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	pobj1f9bIdx = pobjIndexColl->getIndexbyIndexValue((char*) "1F98");
	if (NULL != pobj1f9bIdx)
	{
		CSubIndex* pobjSubIndex = NULL;
		char* pcSubindexId = new char[SUBINDEX_LEN];
		strcpy(pcSubindexId, (char*) "04");
		pobjSubIndex = pobj1f9bIdx->getSubIndexbyIndexValue(pcSubindexId);
		if (NULL != pobjSubIndex)
		{
			char convalue[20];
			memset(convalue, 0, 20 * sizeof(char));
			char actvalue[22];
			memset(actvalue, 0, 22 * sizeof(char));

			if (PREQ_DEFAULT_PAYLOAD > pobjNode->getPReqActPayloadValue())
			{
				_IntToAscii(PREQ_DEFAULT_PAYLOAD, convalue, 16);
			}
			else
			{
				_IntToAscii(pobjNode->getPReqActPayloadValue(), convalue, 16);
			}
			strcpy((char*) convalue, ConvertToUpper((char*) convalue));
			strcpy(actvalue, (char*) "0x");
			strcat(actvalue, convalue);
			pobjSubIndex->setActualValue(actvalue);
			pobj1f9bIdx->setFlagIfIncludedCdc(TRUE);
			pobjSubIndex->setFlagIfIncludedCdc(TRUE);

			//set the value in MN
			if (CN == pobjNode->getNodeType())
			{
				CIndex* pobj1f8bMnIdx = NULL;

				_IntToAscii(pobjNode->getNodeId(), pcSubindexId, 16);
				pcSubindexId = padLeft(pcSubindexId, '0', 2);

				stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
						(char*) "1F8B", pcSubindexId, &subIndexPos, &IndexPos);
				if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
				{
					return;
				}
				pobj1f8bMnIdx = getMNIndexValues((char*) "1F8B");
				if (NULL != pobj1f8bMnIdx)
				{
					CSubIndex* pobj1f8bMnSubIdx = NULL;
					pobj1f8bMnSubIdx = pobj1f8bMnIdx->getSubIndexbyIndexValue(
							pcSubindexId);
					if (NULL != pobj1f8bMnSubIdx)
					{
						pobj1f8bMnSubIdx->setActualValue(actvalue);
						pobj1f8bMnIdx->setFlagIfIncludedCdc(TRUE);
						pobj1f8bMnSubIdx->setFlagIfIncludedCdc(TRUE);
					}

					//00'th sub index is set to FE(a max value) for make to reflect in cdc
					pobj1f8bMnSubIdx = pobj1f8bMnIdx->getSubIndexbyIndexValue(
							(char*) "00");
					if (NULL != pobj1f8bMnSubIdx)
					{
						strcpy(actvalue, (char*) "0xFE"); //to make display of 1f8b in cdc if act != def value
						pobj1f8bMnSubIdx->setActualValue(actvalue);
					}
				}
			}
		}
		delete[] pcSubindexId;
	}

}

/*****************************************************************************************/
/**
 \brief		UpdatePresActLoad

 This function sets the calculated Pres load value in CN 

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdatePresActLoad(CNode* pobjNode)
{
	ocfmRetCode stErrStructInfo;
	stErrStructInfo.code = OCFM_ERR_UNKNOWN;
	stErrStructInfo.errorString = NULL;

	CIndex* pobjIndex = NULL;

	CIndexCollection* pobjIndexColl = NULL;
	INT32 IndexPos = 0;
	INT32 subIndexPos = 0;
	INT32 iNodeId = 0;
	char convalue[20];
	char actvalue[22];
	char* pcSubindexId = NULL;
	ENodeType eNodeType;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	memset(convalue, 0, 20 * sizeof(char));
	memset(actvalue, 0, 22 * sizeof(char));
	iNodeId = pobjNode->getNodeId();
	eNodeType = pobjNode->getNodeType();
	pcSubindexId = new char[SUBINDEX_LEN];
	strcpy(pcSubindexId, (char*) "05");

	stErrStructInfo = IfSubIndexExists(iNodeId, eNodeType, (char*) "1F98",
			pcSubindexId, &subIndexPos, &IndexPos);
	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
#if defined DEBUG
		cout << "UpdatePresActLoad falied. 1F98/05 does not exitst" << endl;
#endif
		return;
	}

	pobjIndexColl = pobjNode->getIndexCollection();
	if (NULL == pobjIndexColl)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}
	pobjIndex = pobjIndexColl->getIndexbyIndexValue((char*) "1F98");
	if (NULL != pobjIndex)
	{
		CSubIndex *pobjSubIndex = NULL;
		pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(pcSubindexId);
		if (NULL != pobjSubIndex)
		{
			if (PRES_DEFAULT_PAYLOAD > pobjNode->getPResActPayloadValue())
			{
				_IntToAscii(PRES_DEFAULT_PAYLOAD, convalue, 16);
			}
			else
			{
				_IntToAscii(pobjNode->getPResActPayloadValue(), convalue, 16);
			}
			strcpy((char*) convalue, ConvertToUpper((char*) convalue));

			strcpy(actvalue, (char*) "0x");
			strcat(actvalue, convalue);
			pobjSubIndex->setActualValue(actvalue);
			pobjIndex->setFlagIfIncludedCdc(TRUE);
			pobjSubIndex->setFlagIfIncludedCdc(TRUE);

			//set the value in MN
			if (CN == pobjNode->getNodeType())
			{
				_IntToAscii(pobjNode->getNodeId(), pcSubindexId, 16);
				pcSubindexId = padLeft(pcSubindexId, '0', 2);

				stErrStructInfo = IfSubIndexExists(MN_NODEID, MN,
						(char*) "1F8D", pcSubindexId, &subIndexPos, &IndexPos);
				if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
				{
#if defined DEBUG
					cout << "UpdatePresActLoad falied. MN 1F8D/" << pcSubindexId
					<< " does not exitst" << endl;
#endif
					return;
				}
				pobjIndex = getMNIndexValues((char *) "1F8D");
				if (NULL != pobjIndex)
				{
					pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(
							pcSubindexId);
					if (NULL != pobjSubIndex)
					{
						pobjSubIndex->setActualValue(actvalue);
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						pobjSubIndex->setFlagIfIncludedCdc(TRUE);
					}

					//00'th sub index is set to FE(a max value) for make to reflect in cdc
					pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(
							(char*) "00");
					if (NULL != pobjSubIndex)
					{
						strcpy(actvalue, (char*) "0xFE"); //to make display of 1f8d in cdc if act != def value
						pobjSubIndex->setActualValue(actvalue);
					}
				}
				else
				{
#if defined DEBUG
					cout
					<< "UpdatePresActLoad falied. MN 1F8D Index does not exitst"
					<< endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "UpdatePresActLoad falied. Processed node is MN"
				<< endl;
#endif
			}
		}
		else
		{
#if defined DEBUG
			cout << "UpdatePresActLoad falied. 1F98/05 Index does not exitst"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout << "UpdatePresActLoad falied. 1F98 Index does not exitst" << endl;
#endif
	}
	delete[] pcSubindexId;
}

/*****************************************************************************************/
/**
 \brief		UpdateCNVisibleNode

 This function updates 1F81 and 1F8D of cross trafficked stations

 \param		pobjNode	Class pointer of CNode for a CN node

 \return	void
 */
/******************************************************************************************/

void UpdateCNVisibleNode(CNode* pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	CIndexCollection* objPDOCollection = NULL;
	INT32 tpdoCount = 0;
	INT32 rpdoCount = 0;
	objPDOCollection = pobjNode->getPDOIndexCollection(&tpdoCount, &rpdoCount);

	if (NULL == objPDOCollection)
	{
#if defined DEBUG
		cout << "UpdateCNVisibleNode falied: PDOCollection empty." << endl;
#endif
		return;
	}

	CIndex* pobjIndex = NULL;
	CIndex* pobjCommIndex = NULL;
	CSubIndex* pobjEntriesSubIndex = NULL;
	CSubIndex* pobjNodeIDSubIndex = NULL;
	char* pcIdx = NULL;
	char* pcCommIdx = NULL;
	char* pcMappedNodeId = NULL;
	INT32 iCrossStnCnt = 0;

	ResetAllSubIndexFlag(
			(pobjNode->getIndexCollection())->getIndexbyIndexValue(
					(char*) "1F81"));
	ResetAllSubIndexFlag(
			(pobjNode->getIndexCollection())->getIndexbyIndexValue(
					(char*) "1F8D"));

	for (INT32 iLoopCount = 0;
			iLoopCount < objPDOCollection->getNumberofIndexes(); iLoopCount++)
	{
		pobjIndex = objPDOCollection->getIndex(iLoopCount);
		if ((NULL != pobjIndex)
				&& (0 == strncmp(pobjIndex->getIndexValue(), "16", 2)))
		{
			pobjEntriesSubIndex = pobjIndex->getSubIndexbyIndexValue(
					(char*) "00");

			if (NULL != pobjEntriesSubIndex)
			{
				if ((NULL != pobjEntriesSubIndex->getActualValue())
						&& (0
								!= strcmp(pobjEntriesSubIndex->getActualValue(),
										""))
						&& !(checkIfValueZero(
								(char*) pobjEntriesSubIndex->getActualValue())))
				{
					pcCommIdx = new char[INDEX_LEN];
					pcIdx = subString((char*) pobjIndex->getIndexValue(), 2, 2);
					strcpy(pcCommIdx, (char*) "14");
					strcat(pcCommIdx, pcIdx);
					delete[] pcIdx;

					pobjCommIndex = objPDOCollection->getIndexbyIndexValue(
							pcCommIdx);
					delete[] pcCommIdx;

					if (NULL != pobjCommIndex)
					{
						pobjNodeIDSubIndex =
								pobjCommIndex->getSubIndexbyIndexValue(
										(char*) "01");
						if (NULL != pobjNodeIDSubIndex)
						{
							if ((NULL != pobjNodeIDSubIndex->getActualValue())
									&& (0
											!= strcmp(
													pobjNodeIDSubIndex->getActualValue(),
													""))
									&& !(checkIfValueZero(
											(char*) pobjNodeIDSubIndex->getActualValue())))
							{
								iCrossStnCnt++;
								if (MAX_CN_CROSS_TRAFFIC_STN < iCrossStnCnt)
								{
									ocfmException objocfmException;
									objocfmException._ocfmRetCode.code =
											OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN;
									char acCustomError[200];
									INT32 iSprintf = 0;
									iSprintf =
											sprintf(acCustomError,
													"The cross trafficking in CN Node ID:%d excceds the maximum permissible station %d",
													pobjNode->getNodeId(),
													MAX_CN_CROSS_TRAFFIC_STN);
									if (iSprintf < 0)
									{
										cout << "Error in sprintf" << __LINE__
												<< endl;
									}
									else
									{
										//sprintf success
									}
									CopyCustomErrorString(
											&(objocfmException._ocfmRetCode),
											acCustomError);
									throw objocfmException;
								}

								//copy the MN objects
								if (CheckIfHex(
										(char*) pobjNodeIDSubIndex->getActualValue()))
								{
									pcMappedNodeId =
											subString(
													(char*) pobjNodeIDSubIndex->getActualValue(),
													2, 2);
								}
								else
								{
									pcMappedNodeId = new char[SUBINDEX_LEN];
									strcpy(pcMappedNodeId,
											pobjNodeIDSubIndex->getActualValue());
								}
								pcMappedNodeId = padLeft(pcMappedNodeId, '0',
										2);

								if (true == ISCNNodeAssignmentValid(pobjNode))
								{
									if (true
											== CopyMNSubindexToCN(pobjNode,
													(char*) "1F81",
													pcMappedNodeId))
									{
										if (true
												== CopyMNSubindexToCN(pobjNode,
														(char*) "1F8D",
														pcMappedNodeId))
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

								delete[] pcMappedNodeId;
							}
						}
					}

				}
			}

		}
	}

}

/*****************************************************************************************/
/**
 \brief		CopyMNSubindexToCN

 This function copies the MN subindex value into corresponding CN subindex 

 \param		pobjNode		Class pointer of CNode for a CN node
 \param		pcIndex			Character pointer to hold the Index value
 \param		pcSubIndex		Character pointer to hold the subindex value


 \return	BOOL

 \retval		TRUE			if successfuly copied
 \retval		FALSE			if not copied
 */
/******************************************************************************************/

bool CopyMNSubindexToCN(CNode* pobjNode, char* pcIndex, char* pcSubIndex)
{
	bool bCopied = false;

	CIndexCollection* pobjIndexCollection = NULL;
	CIndex* pobjMNIndex = NULL;
	CIndex* pobjCNIndex = NULL;

	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NODEID_NOT_FOUND);
		throw objException;
	}

	pobjIndexCollection = pobjNode->getIndexCollection();
	if (NULL == pobjIndexCollection)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
		throw objException;
	}

	if ((NULL == pcIndex) || (NULL == pcSubIndex))
	{
#if defined DEBUG
		cout << "CopyMNSubindexToCN falied: Invalid Parameters" << endl;
#endif
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		throw objException;
	}
	pobjCNIndex = pobjIndexCollection->getIndexbyIndexValue(pcIndex);
	pobjMNIndex = getMNIndexValues(pcIndex);
	if ((NULL != pobjCNIndex) && (NULL != pobjMNIndex))
	{
		CSubIndex* pobjMNSubindex = NULL;
		CSubIndex* pobjCNSubindex = NULL;
		pobjCNSubindex = pobjCNIndex->getSubIndexbyIndexValue(pcSubIndex);
		pobjMNSubindex = pobjMNIndex->getSubIndexbyIndexValue(pcSubIndex);

		if ((NULL == pobjCNSubindex) || (NULL == pobjMNSubindex))
		{
#if defined DEBUG
			cout
			<< "CopyMNSubindexToCN falied: MNSubindex/CNSubindex Not present"
			<< endl;
#endif
		}
		else if (NULL != pobjMNSubindex->getActualValue())
		{
			pobjCNSubindex->setActualValue(
					(char*) pobjMNSubindex->getActualValue());
			pobjCNSubindex->setFlagIfIncludedCdc(TRUE);
			pobjCNIndex->setFlagIfIncludedCdc(TRUE);
			bCopied = true;
		}
		else
		{
#if defined DEBUG
			cout
			<< "CopyMNSubindexToCN falied: pobjMNSubindex has No Actual Value configured"
			<< endl;
#endif
		}
	}
	else
	{
#if defined DEBUG
		cout
		<< "CopyMNSubindexToCN falied: MNIndex/CNIndex is Not Present"
		<< endl;
#endif
	}
	return bCopied;
}

/*****************************************************************************************/
/**
 \brief		ResetAllSubIndexFlag

 This function resets the Include in cdc flag for all the sub index

 \param		pobjIndex	Class pointer of CIndex to get all the subindexes

 \return	void
 */
/******************************************************************************************/

void ResetAllSubIndexFlag(CIndex* pobjIndex)
{
	if (NULL == pobjIndex)
	{
#if defined DEBUG
		cout << "ResetAllSubIndexFlag falied: Index Not found" << endl;
#endif
		return;
	}
	CSubIndex* pobjSubIndex = NULL;
	for (INT32 iLoopCount = 0; iLoopCount < pobjIndex->getNumberofSubIndexes();
			iLoopCount++)
	{
		pobjSubIndex = pobjIndex->getSubIndex(iLoopCount);
		if (NULL != pobjSubIndex)
		{
			pobjSubIndex->setFlagIfIncludedCdc(FALSE);
		}
		else
		{
#if defined DEBUG
			cout << "Some subindexes are NULL. Check for Memory operations"
			<< endl;
#endif
		}
	}
	pobjIndex->setFlagIfIncludedCdc(FALSE);
}

/*****************************************************************************************/
/**
 \brief		GetIndexData

 This function writes the buffer(char pointer) with the index data(id, datatype value, actual value) to be used for the cdc

 \param		objIndex	Class pointer of CIndex for node
 \param		Buffer		Character pointer to hold the buffer data

 \return	void
 */
/******************************************************************************************/

void GetIndexData(CIndex* objIndex, char* Buffer)
{
	if ((NULL == objIndex) || (NULL == Buffer))
	{
#if defined DEBUG
		cout << "GetIndexData falied: Invalid Parameters" << endl;
#endif
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		throw objException;
	}

	bool IfStringDT = false;
	//Get the Index Value

	strcpy(Buffer, "");
	if ((0 == objIndex->getNumberofSubIndexes()))
	{
		if (NULL != objIndex->getActualValue())
		{
			if (true == IsDefaultActualNotEqual(objIndex))
			{
				strcpy(Buffer, objIndex->getIndexValue());
				strcat(Buffer, "\t");

				//There are no subindexes, So add "00"
				strcat(Buffer, "00");
				strcat(Buffer, "\t");

				DataType dt;
				INT32 iPadLength = 0;
				char* pbDataSize = new char[8 + STR_ALLOC_BUFFER];

				dt = objIndex->getDataType();
				if (NULL != dt.Name)
				{
					if (!checkIfStringDatatypes(dt.Name))
					{
						pbDataSize = _IntToAscii(dt.DataSize, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						iPadLength = dt.DataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objIndex->getActualValue());
						pbDataSize = _IntToAscii(iLen, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						iPadLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					//txt2cdc will not be successful
					strcat(Buffer, "00000000");
				}
				delete[] pbDataSize;

				strcat(Buffer, "\t");

				char actvalue[64];
				actvalue[0] = '\0';
				if (IfStringDT)
				{
					strcpy(actvalue, (char*) objIndex->getActualValue());
					strcpy(actvalue, ConvertStringToHex((char*) actvalue));
					strcat(Buffer, actvalue);
				}
				else
				{
					if (CheckIfHex((char*) objIndex->getActualValue()))
					{
						INT32 iLen = strlen((char*) objIndex->getActualValue());
						strncpy(actvalue, (objIndex->getActualValue() + 2),
								iLen - 2);
						actvalue[iLen - 2] = '\0';
						strcat(Buffer, padLeft(actvalue, '0', iPadLength));
					}
					else
					{
						strcpy(actvalue,
								_IntToAscii(atoi(objIndex->getActualValue()),
										actvalue, 16));
						strcat(Buffer, padLeft(actvalue, '0', iPadLength));
					}
				}
				strcat(Buffer, "\n");
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
		bool Indexadded = false;
		bool resetValueAdded = false;
		bool flag_No_of_enteriesAdded = false;
		bool bMappingPDO = false;
		CSubIndex* objSubIndex = NULL;

		bMappingPDO = CheckIfMappingPDO((char*) objIndex->getIndexValue());
		noOfTotalSubIndexes = objIndex->getNumberofSubIndexes();
		objSubIndex = objIndex->getSubIndexbyIndexValue((char*) "00");
		if (objSubIndex != NULL)
		{
			if (objSubIndex->getActualValue() != NULL)
			{
				if (CheckIfHex((char*) objSubIndex->getActualValue()))
				{
					noOfSubIndexes = hex2int(
							subString((char*) objSubIndex->getActualValue(), 2,
									strlen(objSubIndex->getActualValue()) - 2));
				}
				else
				{
					noOfSubIndexes = atoi(objSubIndex->getActualValue());
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

		if (bMappingPDO)
		{
			if (NULL == objSubIndex->getActualValue())
			{
				noOfSubIndexes = noOfTotalSubIndexes + 1;
			}
		}
#if defined DEBUG
		cout << __FUNCTION__ << "noOfSubIndexes:" << noOfSubIndexes << endl;
#endif
		for (INT32 i = 0; i < noOfSubIndexes; i++)
		{
			bool bAccessType = false;

			objSubIndex = objIndex->getSubIndex(i);
#if defined DEBUG
			cout << "Indexx:" << objIndex->getIndexValue() << " SIdx:"
			<< objSubIndex->getIndexValue() << endl;
#endif
			bAccessType = CheckAccessTypeForInclude(
					(char*) objSubIndex->getAccessType());

			if ((objSubIndex->getActualValue() != NULL)
					&& (objSubIndex->getFlagIfIncludedCdc() == TRUE)
					&& ((true == bAccessType) || (true == bMappingPDO))
					&& (true == IsDefaultActualNotEqual(objSubIndex)))
			{
				noOfValidSubIndexes = noOfValidSubIndexes + 1;

				if (noOfValidSubIndexes == noOfSubIndexes)
				{
					i = noOfTotalSubIndexes - 1;
				}

				if (bMappingPDO)
				{
					if (0 == strcmp((char*) objSubIndex->getIndexValue(), "00"))
					{
						continue;
					}
					if (0
							== GetDecimalValue(
									(char*) objSubIndex->getActualValue()))
					{
						if ((NULL == objSubIndex->getDefaultValue())
								|| (0
										== GetDecimalValue(
												(char*) objSubIndex->getDefaultValue())))
						{
							continue;
						}
						continue; //non-Zero actual values should NOT be added
					}
				}

				if (Indexadded)
				{
					strcat(Buffer, objIndex->getIndexValue());
				}
				else
				{
					strcpy(Buffer, objIndex->getIndexValue());
					Indexadded = true;
				}

				//Place a tab
				strcat(Buffer, "\t");

				strcat(Buffer, objSubIndex->getIndexValue());
				strcat(Buffer, "\t");
				//Add datatype
				DataType dt;
				dt = objSubIndex->getDataType();
				INT32 padLength = 0;
				char* pbDataSize = new char[8 + STR_ALLOC_BUFFER];

				if (NULL != dt.Name)
				{
					if (!checkIfStringDatatypes(dt.Name))
					{
						pbDataSize = _IntToAscii(dt.DataSize, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						padLength = dt.DataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objSubIndex->getActualValue());
						pbDataSize = _IntToAscii(iLen, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						padLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					strcat(Buffer, "00000000");
				}

				delete[] pbDataSize;
				strcat(Buffer, "\t");

				// Add the reset value for that Index,SubIndex
				if ((0 == strcmp(objSubIndex->getIndexValue(), "00"))
						&& bMappingPDO && (false == resetValueAdded))
				{
					char actvalue[20];
					strcpy(actvalue, "0");
					strcat(Buffer, padLeft(actvalue, '0', padLength));
					resetValueAdded = true;
				}
				else
				{
					char actvalue[64];
					actvalue[0] = '\0';
					if (IfStringDT)
					{
						strcpy(actvalue, (char*) objSubIndex->getActualValue());
						strcpy(actvalue, ConvertStringToHex((char*) actvalue));
						strcat(Buffer, actvalue);
					}
					else
					{
						if (CheckIfHex((char*) objSubIndex->getActualValue()))
						{
							INT32 len = strlen(
									(char*) objSubIndex->getActualValue());
							strncpy(actvalue,
									(objSubIndex->getActualValue() + 2),
									len - 2);
							actvalue[len - 2] = '\0';

							strcat(Buffer, padLeft(actvalue, '0', padLength));
						}
						else
						{
							strcpy(actvalue,
									_IntToAscii(
											atoi(objSubIndex->getActualValue()),
											actvalue, 16));
							strcat(Buffer, padLeft(actvalue, '0', padLength));
						}
					}
				}

				strcat(Buffer, "\n");
				if ((0 == i) && bMappingPDO
						&& (true == flag_No_of_enteriesAdded))
				{
					i = noOfTotalSubIndexes - 1;
				}
				if ((i == noOfTotalSubIndexes - 1) && bMappingPDO
						&& (false == flag_No_of_enteriesAdded)
						&& (true == resetValueAdded))
				{
					i = -1;
					flag_No_of_enteriesAdded = true;
				}
			}
		}

	}
}

/*****************************************************************************************/
/**
 \brief		BRSpecificGetIndexData

 This function writes the buffer(char pointer) with the index data(id, datatype value, actual value) to be used for the cdc in a BnR specific format.

 \param		objIndex	Class pointer of CIndex for node
 \param		Buffer		Character pointer to hold the buffer data
 \param		iNodeId		Integer variable to hold the Node id to be written to the 1F81 index

 \return	void
 */
/******************************************************************************************/

void BRSpecificGetIndexData(CIndex* objIndex, char* Buffer, INT32 iNodeId)
{
	if ((NULL == objIndex) || (NULL == Buffer))
	{
#if defined DEBUG
		cout << "BRSpecificGetIndexData falied: Invalid Parameters" << endl;
#endif
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		throw objException;
	}

	bool IfStringDT = false;
	//Get the Index Value

	strcpy(Buffer, "");
	if ((0 == objIndex->getNumberofSubIndexes()))
	{
		if (NULL != objIndex->getActualValue())
		{
			if (true == IsDefaultActualNotEqual(objIndex))
			{
				strcpy(Buffer, objIndex->getIndexValue());
				strcat(Buffer, "\t");

				//There are no subindexes, So add "00"
				strcat(Buffer, "00");
				strcat(Buffer, "\t");

				DataType dt;
				INT32 iPadLength = 0;
				char* pbDataSize = new char[8 + STR_ALLOC_BUFFER];

				dt = objIndex->getDataType();
				if (NULL != dt.Name)
				{
					if (!checkIfStringDatatypes(dt.Name))
					{
						pbDataSize = _IntToAscii(dt.DataSize, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						iPadLength = dt.DataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objIndex->getActualValue());
						pbDataSize = _IntToAscii(iLen, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						iPadLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					strcat(Buffer, "00000000");
				}
				delete[] pbDataSize;

				strcat(Buffer, "\t");

				char actvalue[64];
				actvalue[0] = '\0';
				if (IfStringDT)
				{
					strcpy(actvalue, (char*) objIndex->getActualValue());
					strcpy(actvalue, ConvertStringToHex((char*) actvalue));
					strcat(Buffer, actvalue);
				}
				else
				{
					if (CheckIfHex((char*) objIndex->getActualValue()))
					{
						INT32 iLen = strlen((char*) objIndex->getActualValue());
						strncpy(actvalue, (objIndex->getActualValue() + 2),
								iLen - 2);
						actvalue[iLen - 2] = '\0';
						strcat(Buffer, padLeft(actvalue, '0', iPadLength));
					}
					else
					{
						strcpy(actvalue,
								_IntToAscii(atoi(objIndex->getActualValue()),
										actvalue, 16));
						strcat(Buffer, padLeft(actvalue, '0', iPadLength));
					}
				}
				strcat(Buffer, "\n");
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
		bool Indexadded = false;
		bool resetValueAdded = false;
		bool flag_No_of_enteriesAdded = false;
		bool bMappingPDO = false;
		CSubIndex* objSubIndex = NULL;
		char pb1F81nodeid[10];

		bMappingPDO = CheckIfMappingPDO((char*) objIndex->getIndexValue());
		noOfTotalSubIndexes = objIndex->getNumberofSubIndexes();
		objSubIndex = objIndex->getSubIndexbyIndexValue((char*) "00");
		if (objSubIndex != NULL)
		{
			if (objSubIndex->getActualValue() != NULL)
			{
				if (CheckIfHex((char*) objSubIndex->getActualValue()))
				{
					noOfSubIndexes = hex2int(
							subString((char*) objSubIndex->getActualValue(), 2,
									strlen(objSubIndex->getActualValue()) - 2));
				}
				else
				{
					noOfSubIndexes = atoi(objSubIndex->getActualValue());
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

		if (bMappingPDO)
		{
			if (NULL == objSubIndex->getActualValue())
			{
				noOfSubIndexes = noOfTotalSubIndexes + 1;
			}
		}

		if ((0 == strcmp(objIndex->getIndexValue(), (char*) "1F81")))
		{
			_IntToAscii(iNodeId, pb1F81nodeid, 16);
			padLeft(pb1F81nodeid, '0', 2);
		}
#if defined DEBUG
		cout << __FUNCTION__ << "noOfSubIndexes:" << noOfTotalSubIndexes << endl;
#endif
		for (INT32 i = 0; i < noOfTotalSubIndexes; i++)
		{
			bool bAccessType = false;
#if defined DEBUG
			cout << "Indexx:" << objIndex->getIndexValue() << " SIdx:"
			<< objSubIndex->getIndexValue() << endl;
#endif
			objSubIndex = objIndex->getSubIndex(i);

			bAccessType = CheckAccessTypeForInclude(
					(char*) objSubIndex->getAccessType());

			if ((objSubIndex->getActualValue() != NULL)
					&& (objSubIndex->getFlagIfIncludedCdc() == TRUE)
					&& ((true == bAccessType) || (true == bMappingPDO))
					&& (true == IsDefaultActualNotEqual(objSubIndex)))
			{
				if ((0 == strcmp(objIndex->getIndexValue(), (char*) "1F81")))
				{
					if ((0
							== strcmp(objSubIndex->getIndexValue(),
									(const char*) "00"))
							|| (0
									== strcmp(objSubIndex->getIndexValue(),
											pb1F81nodeid)))
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
					i = noOfTotalSubIndexes - 1;
				}

				if (bMappingPDO)
				{
					if (0 == strcmp((char*) objSubIndex->getIndexValue(), "00"))
					{
						continue;
					}
					if (0
							== GetDecimalValue(
									(char*) objSubIndex->getActualValue()))
					{
						if ((NULL == objSubIndex->getDefaultValue())
								|| (0
										== GetDecimalValue(
												(char*) objSubIndex->getDefaultValue())))
						{
							continue;
						}
						continue; //non-Zero actual values should NOT be added
					}
				}

				if (Indexadded)
				{
					strcat(Buffer, objIndex->getIndexValue());
				}
				else
				{
					strcpy(Buffer, objIndex->getIndexValue());
					Indexadded = true;
				}

				//Place a tab
				strcat(Buffer, "\t");

				strcat(Buffer, objSubIndex->getIndexValue());
				strcat(Buffer, "\t");
				//Add datatype
				DataType dt;
				dt = objSubIndex->getDataType();
				INT32 padLength = 0;
				char* pbDataSize = new char[8 + STR_ALLOC_BUFFER];

				if (NULL != dt.Name)
				{
					if (!checkIfStringDatatypes(dt.Name))
					{
						pbDataSize = _IntToAscii(dt.DataSize, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						padLength = dt.DataSize * 2;
						IfStringDT = false;
					}
					else
					{
						INT32 iLen = strlen(objSubIndex->getActualValue());
						pbDataSize = _IntToAscii(iLen, pbDataSize, 16);
						pbDataSize = padLeft(pbDataSize, '0', 8);
						strcat(Buffer, pbDataSize);
						padLength = iLen * 2;
						IfStringDT = true;
					}
				}
				else
				{
					strcat(Buffer, "00000000");
				}

				delete[] pbDataSize;
				strcat(Buffer, "\t");

				// Add the reset value for that Index,SubIndex
				if ((0 == strcmp(objSubIndex->getIndexValue(), "00"))
						&& bMappingPDO && (false == resetValueAdded))
				{
					char actvalue[20];
					strcpy(actvalue, "0");
					strcat(Buffer, padLeft(actvalue, '0', padLength));
					resetValueAdded = true;
				}
				else
				{
					char actvalue[64];
					actvalue[0] = '\0';
					if (IfStringDT)
					{
						strcpy(actvalue, (char*) objSubIndex->getActualValue());
						strcpy(actvalue, ConvertStringToHex((char*) actvalue));
						strcat(Buffer, actvalue);
					}
					else
					{
						if (CheckIfHex((char*) objSubIndex->getActualValue()))
						{
							INT32 len = strlen(
									(char*) objSubIndex->getActualValue());
							strncpy(actvalue,
									(objSubIndex->getActualValue() + 2),
									len - 2);
							actvalue[len - 2] = '\0';

							strcat(Buffer, padLeft(actvalue, '0', padLength));
						}
						else
						{
							strcpy(actvalue,
									_IntToAscii(
											atoi(objSubIndex->getActualValue()),
											actvalue, 16));
							strcat(Buffer, padLeft(actvalue, '0', padLength));
						}
					}
				}

				strcat(Buffer, "\n");
				if ((0 == i) && bMappingPDO
						&& (true == flag_No_of_enteriesAdded))
				{
					i = noOfTotalSubIndexes - 1;
				}
				if ((i == noOfTotalSubIndexes - 1) && bMappingPDO
						&& (false == flag_No_of_enteriesAdded)
						&& (true == resetValueAdded))
				{
					i = -1;
					flag_No_of_enteriesAdded = true;
				}
			}
		}

	}
}

/*****************************************************************************************/
/**
 \brief		WriteCNsData

 This function writes the CN's all index datas into CDC file(mnobd.txt)

 \param		fileName	Character pointer to hold the full path of the file (mnobd.txt)

 \return	void
 */
/******************************************************************************************/

void WriteCNsData(char* fileName)
{
	char* Buffer2 = NULL;
	char* Buffer3 = NULL;
	INT32 len = 0;
	char* c = new char[50];
	FILE* fileptr = new FILE();

	CNodeCollection* objNodeCollection = NULL;
	CNode objNode;
	CIndexCollection* objIndexCollection = NULL;

	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	if (NULL == objNodeCollection)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
		delete[] c;
		throw objException;
	}

	INT32 CNCount = 0;

	for (INT32 count = 0; count < objNodeCollection->getNumberOfNodes();
			count++)
	{
		objNode = objNodeCollection->getNodebyCollectionIndex(count);
		if (CN == objNode.getNodeType())
		{
			if (NULL == (fileptr = fopen(fileName, "a+")))
			{
				//cout << "Problem" <<endl;
			}

			//AddOtherRequiredCNIndexes(objNode.getNodeId());
			objIndexCollection = objNode.getIndexCollection();
			if (NULL == objIndexCollection)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
				delete[] fileptr;
				throw objException;
			}

			char* comment = new char[50];
			_IntToAscii(CNCount + 1, c, 10);
			CNCount++;

			strcpy(comment, "////Configuration Data for CN-");
			comment = strcat(comment, c);
			comment = strcat(comment, "\n");
			len = strlen(comment);
			if ((fwrite(comment, sizeof(char), len, fileptr)) != 0)
			{
				fclose(fileptr);
			}

			delete[] comment;

			CIndex* objIndex = NULL;
			char* Buffer4 = NULL;

			/*************WRITE MN'S 1006,1020 Indexes Values *******************************/
			Buffer4 = new char[30000];
			Buffer2 = new char[60000];

			objIndex = getMNIndexValues((char*) "1006");
			if (NULL != objIndex)
			{
				if (NULL != (char*) objIndex->getActualValue())
				{
					UpdateCNCycleTime(objIndexCollection,
							(char*) objIndex->getActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1006 ActualValue Not Present, Not Updated CNCycleTime"
					<< endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1006 Not Found, Not Updated CNCycleTime"
				<< endl;
#endif
			}

			objIndex = getMNIndexValues((char*) "1C14");
			if (NULL != objIndex)
			{
				if (NULL != (char*) objIndex->getActualValue())
				{
					UpdateCNSoCTolerance(objIndexCollection,
							(char*) objIndex->getActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1C14 ActualValue Not Present, Not Updated CNCycleTime"
					<< endl;
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

			objIndex = getMNIndexValues((char*) "1F26");
			if (NULL != objIndex)
			{
				UpdatedCNDateORTime(objIndex, objNode.getNodeId(), DATE);
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F26 Not Found, Not Updated CN Date" << endl;
#endif
			}

			objIndex = getMNIndexValues((char*) "1F27");
			if (NULL != objIndex)
			{
				UpdatedCNDateORTime(objIndex, objNode.getNodeId(), TIME);
			}
			else
			{
#if defined DEBUG
				cout << "MN index: 1F27 Not Found, Not Updated CN Time" << endl;
#endif
			}

			objIndex = getMNIndexValues((char*) "1F98");
			if (NULL != objIndex)
			{
				CSubIndex* pobjSubIndex = NULL;
				pobjSubIndex = getMNSubIndexValues((char*) "1F98",
						(char*) "08");
				if ((NULL != pobjSubIndex)
						&& (NULL != pobjSubIndex->getActualValue()))
				{
					UpdateCNAsyncMTUsize(objIndexCollection,
							(char*) pobjSubIndex->getActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1F98/08 Not Found or ActualValue not present"
					<< endl;
#endif
				}

				pobjSubIndex = getMNSubIndexValues((char*) "1F98",
						(char*) "07");
				if ((NULL != pobjSubIndex)
						&& (NULL != pobjSubIndex->getActualValue()))
				{
					UpdateCNMultiPrescal(objIndexCollection,
							(char*) pobjSubIndex->getActualValue());
				}
				else
				{
#if defined DEBUG
					cout
					<< "MN index: 1F98/07 Not Found or ActualValue not present"
					<< endl;
#endif
				}
			}
			else
			{
#if defined DEBUG
				cout
				<< "MN index: 1F98 Not Found, Not Updated CN AsyncMTUsize & MultiPrescal"
				<< endl;
#endif
			}

			objIndex = getMNIndexValues((char*) "1F9B");
			if (NULL != objIndex)
			{
				UpdateCNMultipleCycleAssign(&objNode);
			}
			else
			{
#if defined DEBUG
				cout
				<< "MN index: 1F9B Not Found, Not Updated CN MultipleCycleAssign"
				<< endl;
#endif
			}

			UpdateCNVisibleNode(&objNode);
			UpdateCNPresMNActLoad(&objNode);

			strcpy(Buffer2, "");
			strcpy(Buffer4, "");

			char* NoOfenteries = new char[10];
			//workaround for B&R Bus Controller stack
			//NoOfenteries = _IntToAscii(getNodeTotalIndexSubIndex(objNode.getNodeId()), NoOfenteries, 16);
			NoOfenteries = _IntToAscii(
					BRSpecificgetCNsTotalIndexSubIndex(objNode.getNodeId()),
					NoOfenteries, 16);
			/*1 is not added for the size*/
			NoOfenteries = padLeft(NoOfenteries, '0', 8);
			strcpy(NoOfenteries, ConvertToUpper(NoOfenteries));
			strcpy(Buffer4, NoOfenteries);
			strcat(Buffer4, "\n");
			strcpy(Buffer2, Buffer4);
			delete[] NoOfenteries;

			////workaround for B&R Bus Controller stack
			//FormatCdc(objIndexCollection, Buffer4, fileptr, CN);
			BRSpecificFormatCdc(objIndexCollection, Buffer4, fileptr, CN,
					objNode.getNodeId());
			strcat(Buffer2, Buffer4);
			delete[] Buffer4;

			//Convert CN NodeID to Hex
			_IntToAscii(objNode.getNodeId(), c, 16);
			char* CNDataLength = new char[50];
			strcpy(CNDataLength, "1F22\t");
			c = padLeft(c, '0', 2);

			///write CN-n NodeID  in the next to 1F22
			strcat(CNDataLength, c);
			strcat(CNDataLength, "\t");

			//write the size of CN-n Buffer
			INT32 len = getCNDataLen(Buffer2);

			//Convert length to Hex
			_IntToAscii(len, c, 16);
			//printf("c%s",c);

			c = padLeft(c, '0', 8);
			strcat(CNDataLength, c);

			// First write the IF22 data in a Buffer and then CN-ns Object Dictionary
			Buffer3 = new char[strlen(Buffer2) + 50];
			strcpy(Buffer3, CNDataLength);
			strcat(Buffer3, "\n");
			strcat(Buffer3, Buffer2);
			delete[] CNDataLength;
			delete[] Buffer2;

			//write all CNs data in the file
			len = strlen(Buffer3);
			if (NULL == (fileptr = fopen(fileName, "a+")))
			{
				//cout << "Problem" <<endl;
			}
			if (0 != (fwrite(Buffer3, sizeof(char), len, fileptr)))
			{
				fclose(fileptr);
			}
			delete[] Buffer3;
		}

	}
	delete[] c;
}

/*****************************************************************************************/
/**
 \brief		getNodeTotalIndexSubIndex

 This function returns the total no.of index and subindex that can be written to the CDC

 \param		iNodeId		Integer variable to hold the node id to which the total values to be fetched

 \return	INT32
 */
/******************************************************************************************/

INT32 getNodeTotalIndexSubIndex(INT32 iNodeId)
{
	CNodeCollection *objNodeCol = NULL;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode objNode;
	INT32 iNumberOfEntries = 0;
	CIndexCollection *pobjIdxCol = NULL;

	if (MN_NODEID == iNodeId)
	{
		/* Number of 1F81 enteries is twice the number of CN's*/
		iNumberOfEntries = 2 * objNodeCol->getCNNodesCount();
		/* include the number of CN's*/
		iNumberOfEntries = iNumberOfEntries + objNodeCol->getCNNodesCount();
	}

	objNode = objNodeCol->getNode(iNodeId);

	pobjIdxCol = objNode.getIndexCollection();

	for (INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->getNumberofIndexes();
			iLoopCount++)
	{

		CIndex *pobjIndex = NULL;
		pobjIndex = pobjIdxCol->getIndex(iLoopCount);

		if (pobjIndex->getFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) pobjIndex->getAccessType())
						|| CheckIfMappingPDO((char*) pobjIndex->getIndexValue())))
		{
			if (pobjIndex->getNumberofSubIndexes() == 0)
			{
				if (pobjIndex->getActualValue() != NULL
						&& true == IsDefaultActualNotEqual(pobjIndex))
				{
					iNumberOfEntries = iNumberOfEntries + 1;
				}
			}
			else
			{
				if (CheckIfMappingPDO((char*) pobjIndex->getIndexValue()))
				{
					CSubIndex* pobjSubIndex = NULL;
					pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(
							(char*) "00");

					if ((NULL != pobjSubIndex)
							&& (NULL != pobjSubIndex->getActualValue())
							&& (0 != strcmp(pobjSubIndex->getActualValue(), "")))
					{
						if (true == ReactivateMappingPDO(pobjIdxCol, pobjIndex)
								|| true
										== IsDefaultActualNotEqual(
												pobjSubIndex))
						{
							iNumberOfEntries = iNumberOfEntries + 1; /* to initalize 00 entry subindex */
						}
						if (checkIfValueZero(
								(char*) pobjSubIndex->getActualValue()))
						{
							continue;
						}
						if (true == ReactivateMappingPDO(pobjIdxCol, pobjIndex)
								|| true
										== IsDefaultActualNotEqual(
												pobjSubIndex))
						{
							iNumberOfEntries = iNumberOfEntries + 1; /* to reinitalize 00 entry subindex */
						}
						for (INT32 iLoopCount = 0;
								iLoopCount < pobjIndex->getNumberofSubIndexes();
								iLoopCount++)
						{
							if (pobjIndex->getSubIndex(iLoopCount)->getActualValue()
									!= NULL
									&& TRUE
											== pobjIndex->getSubIndex(
													iLoopCount)->getFlagIfIncludedCdc()
									&& true
											== IsDefaultActualNotEqual(
													pobjIndex->getSubIndex(
															iLoopCount)))
							{
								if (0
										== strcmp(
												(char*) pobjIndex->getSubIndex(
														iLoopCount)->getIndexValue(),
												"00"))
								{
									continue;
								}
								if (0
										== GetDecimalValue(
												(char*) pobjIndex->getSubIndex(
														iLoopCount)->getActualValue()))
								{
									if (NULL
											== pobjIndex->getSubIndex(
													iLoopCount)->getDefaultValue()
											|| 0
													== GetDecimalValue(
															(char*) pobjIndex->getSubIndex(
																	iLoopCount)->getDefaultValue()))
									{
										continue;
									}
								}
								iNumberOfEntries = iNumberOfEntries + 1;

							}
						}
					}
					continue;
				}

				if (MN_NODEID == iNodeId)
				{
					if (0 == strcmp((char*) pobjIndex->getIndexValue(), "1F81"))
					{
						CSubIndex* pobj1F81SubIndex;
						pobj1F81SubIndex = pobjIndex->getSubIndexbyIndexValue(
								(char*) "F0");
						if (NULL != pobj1F81SubIndex
								&& pobj1F81SubIndex->getActualValue() != NULL
								&& 0
										!= strcmp(
												(char*) pobj1F81SubIndex->getActualValue(),
												"")
								&& true
										== CheckAccessTypeForInclude(
												(char*) pobj1F81SubIndex->getAccessType())
								&& true
										== IsDefaultActualNotEqual(
												pobj1F81SubIndex))
						{
							iNumberOfEntries = iNumberOfEntries + 1;
						}
						continue;

					}
				}

				CSubIndex* pobjNoOfEntriesSubIndex = NULL;
				pobjNoOfEntriesSubIndex = pobjIndex->getSubIndexbyIndexValue(
						(char*) "00");
				if ((NULL != pobjNoOfEntriesSubIndex)
						&& (NULL != pobjNoOfEntriesSubIndex->getActualValue())
						&& (0
								!= strcmp(
										pobjNoOfEntriesSubIndex->getActualValue(),
										"")))
				{
					if (checkIfValueZero(
							(char*) pobjNoOfEntriesSubIndex->getActualValue()))
						continue;
				}

				for (INT32 iLoopCount = 0;
						iLoopCount < pobjIndex->getNumberofSubIndexes();
						iLoopCount++)
				{
					if (pobjIndex->getSubIndex(iLoopCount)->getActualValue()
							!= NULL
							&& TRUE
									== pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc()
							&& true
									== CheckAccessTypeForInclude(
											(char*) pobjIndex->getSubIndex(
													iLoopCount)->getAccessType())
							&& true
									== IsDefaultActualNotEqual(
											pobjIndex->getSubIndex(iLoopCount)))
					{
						iNumberOfEntries = iNumberOfEntries + 1;
					}
				}
			}

		}
	}
	return iNumberOfEntries;

}

/*****************************************************************************************/
/**
 \brief		BRSpecificgetCNsTotalIndexSubIndex

 This function returns the total no.of index and subindex(recommended by BnR) that can be written to the CDC

 \param		iNodeID

 \return	INT32
 */
/******************************************************************************************/

INT32 BRSpecificgetCNsTotalIndexSubIndex(INT32 iNodeID)
{
	CNodeCollection* objNodeCol = NULL;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode objNode;
	INT32 iNumberOfEntries = 0;
	CIndexCollection* pobjIdxCol = NULL;
	char pb1F81nodeid[10];
	_IntToAscii(iNodeID, pb1F81nodeid, 16);
	padLeft(pb1F81nodeid, '0', 2);

	objNode = objNodeCol->getNode(iNodeID);
	pobjIdxCol = objNode.getIndexCollection();
	for (INT32 iLoopCount = 0; iLoopCount < pobjIdxCol->getNumberofIndexes();
			iLoopCount++)
	{

		CIndex* pobjIndex = NULL;
		pobjIndex = pobjIdxCol->getIndex(iLoopCount);

		if (pobjIndex->getFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) pobjIndex->getAccessType())
						|| CheckIfMappingPDO((char*) pobjIndex->getIndexValue())))
		{
			{
				if (pobjIndex->getNumberofSubIndexes() == 0)
				{
					if (pobjIndex->getActualValue() != NULL
							&& true == IsDefaultActualNotEqual(pobjIndex))
					{
						iNumberOfEntries = iNumberOfEntries + 1;
					}
				}
				else
				{
					if (CheckIfMappingPDO((char*) pobjIndex->getIndexValue()))
					{
						CSubIndex* pobjSubIndex = NULL;
						pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(
								(char*) "00");

						//actual value checked for non-Zero
						if ((NULL != pobjSubIndex)
								&& (NULL != pobjSubIndex->getActualValue())
								&& (0
										!= strcmp(
												pobjSubIndex->getActualValue(),
												""))
								&& !(checkIfValueZero(
										(char*) pobjSubIndex->getActualValue())))
						{
							if (true
									== ReactivateMappingPDO(pobjIdxCol,
											pobjIndex)
									|| true
											== IsDefaultActualNotEqual(
													pobjSubIndex))
							{
								iNumberOfEntries = iNumberOfEntries + 1; /* to initalize 00 entry subindex */
							}

							if (true
									== ReactivateMappingPDO(pobjIdxCol,
											pobjIndex)
									|| true
											== IsDefaultActualNotEqual(
													pobjSubIndex))
							{
								iNumberOfEntries = iNumberOfEntries + 1; /* to reinitalize 00 entry subindex */
							}

							INT32 isiTotal = pobjIndex->getNumberofSubIndexes();

							for (INT32 iLoopCount = 0; iLoopCount < isiTotal;
									iLoopCount++)
							{

								if (pobjIndex->getSubIndex(iLoopCount)->getActualValue()
										!= NULL
										&& TRUE
												== pobjIndex->getSubIndex(
														iLoopCount)->getFlagIfIncludedCdc()
										&& true
												== IsDefaultActualNotEqual(
														pobjIndex->getSubIndex(
																iLoopCount)))
								{
									if (0
											== strcmp(
													(char*) pobjIndex->getSubIndex(
															iLoopCount)->getIndexValue(),
													"00"))
									{
										//the no.of subindex is determied by the 00'th entry actual value. 1 is added for 00'th entry count
										isiTotal =
												(GetDecimalValue(
														(char*) pobjSubIndex->getActualValue())
														+ 1);
										continue;
									}
									if (0
											== GetDecimalValue(
													(char*) pobjIndex->getSubIndex(
															iLoopCount)->getActualValue()))
									{
										if (NULL
												== pobjIndex->getSubIndex(
														iLoopCount)->getDefaultValue()
												|| 0
														== GetDecimalValue(
																(char*) pobjIndex->getSubIndex(
																		iLoopCount)->getDefaultValue()))
										{
											continue;
										}
									}
									iNumberOfEntries = iNumberOfEntries + 1;

								}
							}
						}
						continue;
					}

					CSubIndex* pobjSubIndex = NULL;
					pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(
							(char*) "00");
					if ((NULL != pobjSubIndex)
							&& (NULL != pobjSubIndex->getActualValue())
							&& (0 != strcmp(pobjSubIndex->getActualValue(), "")))
					{
						if (checkIfValueZero(
								(char*) pobjSubIndex->getActualValue()))
							continue;
					}

					for (INT32 iLoopCount = 0;
							iLoopCount < pobjIndex->getNumberofSubIndexes();
							iLoopCount++)
					{
						if (pobjIndex->getSubIndex(iLoopCount)->getActualValue()
								!= NULL
								&& TRUE
										== pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc()
								&& true
										== CheckAccessTypeForInclude(
												(char*) pobjIndex->getSubIndex(
														iLoopCount)->getAccessType())
								&& true
										== IsDefaultActualNotEqual(
												pobjIndex->getSubIndex(
														iLoopCount)))
						{
							if (0
									== strcmp(pobjIndex->getIndexValue(),
											(char*) "1F81"))
							{

								if (0
										== strcmp(
												pobjIndex->getSubIndex(
														iLoopCount)->getIndexValue(),
												"00")
										|| 0
												== strcmp(
														pobjIndex->getSubIndex(
																iLoopCount)->getIndexValue(),
														pb1F81nodeid))
								{
									continue;
								}
								else
								{
								}
							}
							iNumberOfEntries = iNumberOfEntries + 1;
						}
					}

				}
			}
		}
	}
	return iNumberOfEntries;
}

/*****************************************************************************************/
/**
 \brief		GenerateCDC

 This API shall be used to generate the CDC for the project. This API writes the compiled CDC into the mnobd.txt file. The generated mnobd.txt is converted into binary [mnobd.cdc] file using txt2cdc [elf].

 \param		CDCLocation		Char pointer to hold the path of the location to which the generated cdc contents will be written into.

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GenerateCDC(char* CDCLocation)
{

	CNode objMnNode;
	CNode *pobjNode = NULL;
	CIndexCollection* objIndexCollection;
	//ofstream file;
	char *Buffer1 = NULL;
	char *tempFileName = NULL;
	char *tempOutputFileName = NULL;
	UINT32 len;
	ocfmRetCode retCode;
	ocfmRetCode ret1F81Code;
	ocfmRetCode retCode_temp;
	ocfmException ex;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;

	tempFileName = new char[strlen(CDCLocation) + 10 + strlen("mnobd")];
	sprintf(tempFileName, "%s/%s.txt", CDCLocation, "mnobd");

	tempOutputFileName = new char[1000];
	sprintf(tempOutputFileName, "%s/%s.cdc", CDCLocation, "mnobd");
	retCode.code = OCFM_ERR_UNKNOWN;
	retCode_temp.code = OCFM_ERR_UNKNOWN;
	ret1F81Code.code = OCFM_ERR_UNKNOWN;
	retCode.errorString = NULL;
	retCode_temp.errorString = NULL;
	ret1F81Code.errorString = NULL;

	try
	{
		//get the MN Node object from the NodeCollection
		CNodeCollection* objNodeCollection;
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();

		/******************************* Write MN's Indexes ******************************************************************/

		objMnNode = objNodeCollection->getMNNode();
		if (objNodeCollection->getNumberOfNodes() == 0)
		{
			ex.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw ex;
		}
		if (objMnNode.isNull())
		{
			ex.ocfm_Excpetion(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			throw ex;
		}
		if (objNodeCollection->getCNNodesCount() == 0)
		{
			ex.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
			throw ex;
		}

		/* Check if autogeneration of MN's Object Dicitonary is set to true */

		CPjtSettings* stPjtSettings;
		stPjtSettings = CPjtSettings::getPjtSettingsPtr();

		if (stPjtSettings->getGenerateAttr() == YES_AG)
		{
			retCode = CheckMutliplexAssigned();
			if (retCode.code != OCFM_ERR_SUCCESS)
			{
				return retCode;
			}
			/*Generate MNOBD for Auto Generate On*/
			retCode = GenerateMNOBD(true);
			if (OCFM_ERR_EXCESS_CHANNEL == retCode.code)
			{
				//Do not throw exception here as we need the process to complete
				retCode_temp = retCode;
			}
			else if (OCFM_ERR_SUCCESS != retCode.code)
			{
				return retCode;
			}
			else
			{
			}
		}
		else
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < objNodeCollection->getNumberOfNodes();
					iLoopCount++)
			{
				pobjNode = objNodeCollection->getNodebyColIndex(iLoopCount);
				/* Process PDO Objects for CN*/
				if (NULL == pobjNode)
				{
#if defined DEBUG
					cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

					ex.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					throw ex;
				}

				if (pobjNode->getNodeType() == MN)
				{
					objIndexCollection = pobjNode->getIndexCollection();
					if (NULL == objIndexCollection)
					{
#if defined DEBUG
						cout << "Memory allocation error" << __FUNCTION__
						<< endl;
#endif

						ex.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
						throw ex;
					}
				}
				else
				{
					if (!(pobjNode->HasPdoObjects()))
					{

						continue;
					}
				}
			}
		}

		FILE* fileptr = new FILE();
		if ((fileptr = fopen(tempFileName, "w+")) == NULL)
		{
			ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
			throw ex;
		}

		for (INT32 i = 0; i < objNodeCollection->getNumberOfNodes(); i++)
		{
			CNode objCnNode;
			objCnNode = objNodeCollection->getNodebyCollectionIndex(i);
			if (objCnNode.getNodeType() == CN)
			{
				INT32 NodeID = objCnNode.getNodeId();
				char* hex = new char[10];
				hex = _IntToAscii(NodeID, hex, 16);
				hex = padLeft(hex, '0', 2);

				char* pcTemp1F81Data = setNodeAssigmentBits(&objCnNode);
				char* pb1F81Data = new char[strlen(pcTemp1F81Data)
						+ ALLOC_BUFFER + 2];
				sprintf(pb1F81Data, "0X%s", pcTemp1F81Data);
				ret1F81Code = IfSubIndexExists(MN_NODEID, MN, (char*) "1F81",
						hex, &iSubIndexPos, &iIndexPos);
				if (OCFM_ERR_SUCCESS != ret1F81Code.code)
				{
					continue;
				}

				SetSIdxValue((char*) "1F81", hex, pb1F81Data,
						objNodeCollection->getMNNode().getIndexCollection(),
						MN_NODEID, MN, false);
				delete[] hex;
				delete[] pb1F81Data;
			}
		}

		objMnNode = objNodeCollection->getMNNode();
		INT32 iCNsCount = 0;
		iCNsCount = objNodeCollection->getCNNodesCount();
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F81",
				true);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F92",
				false);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F8D",
				true);
		//1c07,1c08,1f22,1f84,1f8e,1f8f to be added
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F8B",
				true);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F26",
				false);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1F27",
				false);
		UpdateMNNodeAssignmentIndex(&objMnNode, iCNsCount, (char*) "1C09",
				true);
		if (YES_AG == stPjtSettings->getGenerateAttr())
		{
			CalculatePayload();
		}

		//Buffer1 = (char*)malloc(CDC_BUFFER);
		Buffer1 = new char[CDC_BUFFER];
		char* NoOfenteries = new char[10];
		NoOfenteries = _IntToAscii(getNodeTotalIndexSubIndex(MN_NODEID),
				NoOfenteries, 16);
		NoOfenteries = padLeft(NoOfenteries, '0', 8);
		strcpy(Buffer1, NoOfenteries);
		strcat(Buffer1, "\n");
		len = strlen(Buffer1);
		delete[] NoOfenteries;

		/* Write number of enteries */
		if ((len != (fwrite(Buffer1, sizeof(char), len, fileptr))))
		{
#ifdef DEBUG
			cout << "Write Error in CDC" << endl;
#endif
		}
		delete[] Buffer1;

		// Add 1F81
		for (INT32 i = 0; i < objNodeCollection->getNumberOfNodes(); i++)
		{
			CNode objCnNode;
			objCnNode = objNodeCollection->getNodebyCollectionIndex(i);
			if (objCnNode.getNodeType() == CN)
			{

				Buffer1 = new char[CDC_BUFFER];
				strcpy(Buffer1, "//// NodeId Assignment\n");
				strcat(Buffer1, "1F81");
				strcat(Buffer1, "\t");
				INT32 NodeID = objCnNode.getNodeId();
				char* hex = new char[10];
				hex = _IntToAscii(NodeID, hex, 16);
				hex = padLeft(hex, '0', 2);
				strcat(Buffer1, hex);
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
				delete[] hex;
				delete[] Buffer1;
			}
		}

		fclose(fileptr);

		if ((fileptr = fopen(tempFileName, "a+")) == NULL)
		{
			ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
			throw ex;
		}
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objMnNode = objNodeCollection->getMNNode();
		objIndexCollection = objMnNode.getIndexCollection();

		//Get all the MN's Default Data in Buffer1
		Buffer1 = new char[CDC_MN_BUFFER];
		memset(Buffer1, 0, CDC_MN_BUFFER * sizeof(char));
		FormatCdc(objIndexCollection, Buffer1, fileptr, MN);

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
		if (objNodeCollection->getNumberOfNodes() != 0)
		{

			for (INT32 i = 0; i < objNodeCollection->getNumberOfNodes(); i++)
			{
				CNode objCnNode;
				objCnNode = objNodeCollection->getNodebyCollectionIndex(i);
				if (objCnNode.getNodeType() == CN)
				{
					if ((fileptr = fopen(tempFileName, "a+")) == NULL)
					{
						ex.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
						throw ex;
					}
					Buffer1 = new char[CDC_BUFFER];
					len = strlen(Buffer1);
					strcpy(Buffer1, "//// NodeId Reassignment\n");
					strcat(Buffer1, "1F81");
					strcat(Buffer1, "\t");
					INT32 NodeID = objCnNode.getNodeId();
					char* hex = new char[10];
					hex = _IntToAscii(NodeID, hex, 16);
					hex = padLeft(hex, '0', 2);
					strcat(Buffer1, hex);

					strcat(Buffer1, "\t00000004\t");
					char* pcTemp1F81Data = setNodeAssigmentBits(&objCnNode);
					strcat(Buffer1, pcTemp1F81Data);

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
					delete[] hex;
					delete[] Buffer1;
				}
			}

		}

		// Convert CDC txt file to Binary
		INT32 iReturnFromtxt2cdc;
		char* cmdBuffer;
#if defined(_WIN32) && defined(_MSC_VER)
		cmdBuffer = new char[(2 * (strlen(CDCLocation) + 10 + 10)) + 25]; //TODO: Calculate size dynamically
		sprintf(cmdBuffer, "txt2cdc.exe \"%s\" \"%s\"", tempFileName,
				tempOutputFileName);
		iReturnFromtxt2cdc = system(cmdBuffer);
#else
		cmdBuffer = new char[LINUX_INSTALL_DIR_LEN + (2 * (strlen(CDCLocation) + 10 + 10)) + 25];
		sprintf(cmdBuffer, "%s/txt2cdc \"%s\" \"%s\"", LINUX_INSTALL_DIR, tempFileName, tempOutputFileName);
		iReturnFromtxt2cdc = system(cmdBuffer);

#endif
		delete[] cmdBuffer;

		if (OCFM_ERR_SUCCESS == iReturnFromtxt2cdc)
		{
			retCode.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ex.ocfm_Excpetion(OCFM_ERR_INVALID_TXT_FOR_CDC);
			throw ex;
		}
		if (OCFM_ERR_EXCESS_CHANNEL == retCode_temp.code)
		{
			ex.ocfm_Excpetion(retCode_temp.code);
			throw ex;
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
	return retCode;
}

/*****************************************************************************************/
/**
 \brief		FormatCdc

 This function writes the buffer with the index data in the index collection that can be written in the CDC

 \param		objIndexCollection		Class pointer of CIndexCollection for node
 \param		Buffer1					Character pointer to hold the buffer data
 \param		fileptr					
 \param		eNodeType				Enumeration flag for Node type (CN/MN)

 \return	void
 */
/******************************************************************************************/

//TODO: only buffer is used not the fileptr. should be removed in header & releated.
void FormatCdc(CIndexCollection *objIndexCollection, char* Buffer1,
		FILE* fileptr, ENodeType eNodeType)
{
	if ((NULL == objIndexCollection) || (NULL == Buffer1))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char *TempBuffer1 = NULL;
	strcpy(Buffer1, "");
	// UINT32 len;
	INT32 NumberOfIndexes = objIndexCollection->getNumberofIndexes();
	//disable mapping pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex* objIndex;
		objIndex = objIndexCollection->getIndex(i);
		if (objIndex->getFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->getIndexValue()))
			{
				TempBuffer1 = new char[CDC_BUFFER];
				//len = strlen(Buffer1);      
				//GetIndexData(objIndex,Buffer1);
				EnableDisableMappingPDO(objIndexCollection, objIndex,
						TempBuffer1, false);
				strcat(Buffer1, TempBuffer1);
				delete[] TempBuffer1;
			}
		}
	}
	// write all objects except pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex* objIndex;
		objIndex = objIndexCollection->getIndex(i);

		if (objIndex->getFlagIfIncludedCdc() == TRUE
				&& true
						== CheckAccessTypeForInclude(
								(char*) objIndex->getAccessType())
				&& CheckIfNotPDO((char*) objIndex->getIndexValue()))
		{
			if ((CN == eNodeType)
					|| (strcmp(objIndex->getIndexValue(), "1F81") != 0
							&& MN == eNodeType))
			{
				TempBuffer1 = new char[15000];
				GetIndexData(objIndex, TempBuffer1);
				strcat(Buffer1, TempBuffer1);

				delete[] TempBuffer1;
			}
			else
			{
				CSubIndex* objSubIndex = objIndex->getSubIndexbyIndexValue(
						(char*) "F0");
				if (NULL != objSubIndex
						&& TRUE == objSubIndex->getFlagIfIncludedCdc()
						&& true
								== CheckAccessTypeForInclude(
										(char*) objSubIndex->getAccessType())
						&& NULL != objSubIndex->getActualValue()
						&& 0
								!= strcmp((char*) objSubIndex->getActualValue(),
										"")
						&& true == IsDefaultActualNotEqual(objSubIndex))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					strcpy(TempBuffer1, "1F81");
					strcat(TempBuffer1, "\t");
					strcat(TempBuffer1, "F0");
					strcat(TempBuffer1, "\t00000004\t");

					char actvalue[20];
					actvalue[0] = '\0';
					if (CheckIfHex((char*) objSubIndex->getActualValue()))
					{
						INT32 len = strlen(
								(char*) objSubIndex->getActualValue());
						strncpy(actvalue, (objSubIndex->getActualValue() + 2),
								len - 2);
						actvalue[len - 2] = '\0';
						strcat(TempBuffer1, padLeft(actvalue, '0', 8));
					}
					else
					{
						strcpy(actvalue,
								_IntToAscii(atoi(objSubIndex->getActualValue()),
										actvalue, 16));
						strcat(TempBuffer1, padLeft(actvalue, '0', 8));
					}

					strcat(TempBuffer1, "\n");
					// len = strlen(TempBuffer1);
					strcat(Buffer1, TempBuffer1);
					delete[] TempBuffer1;
				}
			}
		}
	}
	//Write the pdo configuration
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex* objIndex;
		objIndex = objIndexCollection->getIndex(i);
		if (objIndex->getFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) objIndex->getAccessType())
						|| CheckIfMappingPDO((char*) objIndex->getIndexValue()))
				&& !CheckIfNotPDO((char*) objIndex->getIndexValue()))
		{
			TempBuffer1 = new char[3 * CDC_BUFFER];
			GetIndexData(objIndex, TempBuffer1);
			strcat(Buffer1, TempBuffer1);

			delete[] TempBuffer1;
		}
	}
	//reenable the pdos
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex* objIndex;
		objIndex = objIndexCollection->getIndex(i);
		if (objIndex->getFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->getIndexValue()))
			{
				TempBuffer1 = new char[CDC_BUFFER];
				EnableDisableMappingPDO(objIndexCollection, objIndex,
						TempBuffer1, true);
				strcat(Buffer1, TempBuffer1);
				delete[] TempBuffer1;
			}
		}
	}
}

/*****************************************************************************************/
/**
 \brief		BRSpecificFormatCdc

 This function writes the buffer with the index data in the index collection that can be written in the CDC

 \param		objIndexCollection		Class pointer of CIndexCollection for node
 \param		Buffer1					Character pointer to hold the buffer data
 \param		fileptr
 \param		eNodeType				Enumeration flag for Node type (CN/MN)
 \param		iNodeId					Integer variable to hold the Node id to be written to the 1F81 index

 \return	void
 */
/******************************************************************************************/

//TODO: only buffer is used not the fileptr. should be removed in header & releated.
void BRSpecificFormatCdc(CIndexCollection *objIndexCollection, char* Buffer1,
		FILE* fileptr, ENodeType eNodeType, INT32 iNodeId)
{
	ocfmException objException;
	if ((NULL == objIndexCollection) || (NULL == Buffer1))
	{
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	char *TempBuffer1 = NULL;
	strcpy(Buffer1, "");
	// UINT32 len;
	INT32 NumberOfIndexes = objIndexCollection->getNumberofIndexes();
	//disable mapping pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex* objIndex = NULL;
		objIndex = objIndexCollection->getIndex(i);
		if (NULL == objIndex)
		{
#if defined DEBUG
			cout << "Memory allocation error 1" << __FUNCTION__ << __LINE__
			<< endl;
#endif

			objException.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw objException;
		}
		if (objIndex->getFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->getIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being disabled.
				CSubIndex* pobjSubIndex = NULL;
				pobjSubIndex = objIndex->getSubIndexbyIndexValue((char*) "00");
				if (NULL == pobjSubIndex)
				{
#if defined DEBUG
					cout << "Memory allocation error2" << __FUNCTION__
					<< __LINE__ << "Index" << objIndex->getIndexValue()
					<< endl;
#endif

					objException.ocfm_Excpetion(
							OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					throw objException;
				}
				if (!checkIfValueZero((char*) pobjSubIndex->getActualValue()))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					//len = strlen(Buffer1);      
					EnableDisableMappingPDO(objIndexCollection, objIndex,
							TempBuffer1, false);
					strcat(Buffer1, TempBuffer1);
					delete[] TempBuffer1;
				}
			}
		}
	}
	// write all objects except pdo
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex* objIndex;
		objIndex = objIndexCollection->getIndex(i);
		if (objIndex->getFlagIfIncludedCdc() == TRUE
				&& true
						== CheckAccessTypeForInclude(
								(char*) objIndex->getAccessType())
				&& CheckIfNotPDO((char*) objIndex->getIndexValue()))
		{
			if ((CN == eNodeType)
					|| (strcmp(objIndex->getIndexValue(), "1F81") != 0
							&& MN == eNodeType))
			{
				TempBuffer1 = new char[15000];
				//commented the GetIndexData fn and BRSpecificGetIndexData is used
				//GetIndexData(objIndex,TempBuffer1);
				BRSpecificGetIndexData(objIndex, TempBuffer1, iNodeId);
				strcat(Buffer1, TempBuffer1);

				delete[] TempBuffer1;
			}
			else
			{
				CSubIndex* objSubIndex = objIndex->getSubIndexbyIndexValue(
						(char*) "F0");
				if (NULL != objSubIndex
						&& TRUE == objSubIndex->getFlagIfIncludedCdc()
						&& true
								== CheckAccessTypeForInclude(
										(char*) objSubIndex->getAccessType())
						&& NULL != objSubIndex->getActualValue()
						&& 0
								!= strcmp((char*) objSubIndex->getActualValue(),
										"")
						&& true == IsDefaultActualNotEqual(objSubIndex))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					strcpy(TempBuffer1, "1F81");
					strcat(TempBuffer1, "\t");
					strcat(TempBuffer1, "F0");
					strcat(TempBuffer1, "\t00000004\t");

					char actvalue[20];
					actvalue[0] = '\0';
					if (CheckIfHex((char*) objSubIndex->getActualValue()))
					{
						INT32 len = strlen(
								(char*) objSubIndex->getActualValue());
						strncpy(actvalue, (objSubIndex->getActualValue() + 2),
								len - 2);
						actvalue[len - 2] = '\0';
						strcat(TempBuffer1, padLeft(actvalue, '0', 8));
					}
					else
					{
						strcpy(actvalue,
								_IntToAscii(atoi(objSubIndex->getActualValue()),
										actvalue, 16));
						strcat(TempBuffer1, padLeft(actvalue, '0', 8));
					}

					strcat(TempBuffer1, "\n");
					//  len = strlen(TempBuffer1);
					strcat(Buffer1, TempBuffer1);
					delete[] TempBuffer1;
				}
			}
		}
	}
	//Write the pdo configuration
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex *objIndex = NULL;
		objIndex = objIndexCollection->getIndex(i);
		if (objIndex->getFlagIfIncludedCdc() == TRUE
				&& (true
						== CheckAccessTypeForInclude(
								(char*) objIndex->getAccessType())
						|| CheckIfMappingPDO((char*) objIndex->getIndexValue()))
				&& !CheckIfNotPDO((char*) objIndex->getIndexValue()))
		{
			TempBuffer1 = new char[3 * CDC_BUFFER];
			GetIndexData(objIndex, TempBuffer1);
			strcat(Buffer1, TempBuffer1);

			delete[] TempBuffer1;
		}
	}
	//reenable the pdos
	for (INT32 i = 0; i < NumberOfIndexes; i++)
	{
		CIndex* objIndex;
		objIndex = objIndexCollection->getIndex(i);
		if (NULL == objIndex)
		{
#if defined DEBUG
			cout << "Memory allocation error3" << __FUNCTION__ << __LINE__
			<< endl;
#endif

			objException.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw objException;
		}
		if (objIndex->getFlagIfIncludedCdc() == TRUE)
		{
			if (CheckIfMappingPDO((char*) objIndex->getIndexValue()))
			{
				//non-Zero values of 00'th subindex are only being enabled.
				CSubIndex *pobjSubIndex = NULL;
				pobjSubIndex = objIndex->getSubIndexbyIndexValue((char*) "00");
				if (NULL == pobjSubIndex)
				{
#if defined DEBUG
					cout << "Memory allocation error4" << __FUNCTION__
					<< __LINE__ << endl;
#endif

					objException.ocfm_Excpetion(
							OCFM_ERR_MEMORY_ALLOCATION_ERROR);
					throw objException;
				}
				if (!checkIfValueZero((char*) pobjSubIndex->getActualValue()))
				{
					TempBuffer1 = new char[CDC_BUFFER];
					EnableDisableMappingPDO(objIndexCollection, objIndex,
							TempBuffer1, true);
					strcat(Buffer1, TempBuffer1);

					delete[] TempBuffer1;
				}
			}
		}
	}
}

/*****************************************************************************************/
/**
 \brief		ProcessCDT

 This function process the Complex datatype collection and generates the process image for the node

 \param		pobjCDT				Class pointer of CComplexDataType for node
 \param		pobjAppProc			Class pointer of CApplicationProcess for node
 \param		pobjNode			Class pointer of CNode for node
 \param		pobjParameter		Struct pointer of Parameter for node
 \param		enumPdoType			Enumeration flag for PDO type
 \param		pbModuleName		Character pointer to hold the Module name
 \param		pbModuleIndex		Character pointer to hold the Module Index value

 \return	INT32
 */
/******************************************************************************************/

INT32 ProcessCDT(CComplexDataType* pobjCDT, CApplicationProcess* pobjAppProc,
		CNode* pobjNode, Parameter* pobjParameter, EPDOType enumPdoType,
		char* pbModuleName, char* pbModuleIndex)
{
	ocfmException objocfmException;
	if (pobjCDT == NULL)
	{
		objocfmException.ocfm_Excpetion(OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
		throw objocfmException;
	}
	if ((NULL == pobjAppProc) || (NULL == pobjNode) || (NULL == pobjParameter)
			|| (NULL == pbModuleName) || (NULL == pbModuleIndex))
	{
		objocfmException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objocfmException;
	}

	//TODO: No header definitions is for ProcessCDT
	INT32 iStartBitOffset = 0;
	INT32 iOffset = 0;
	bool bIsNewBitStringVar = false;
	INT32 iDataSize = 0;
	INT32 iTotalBytesMapped = 0;
	#if defined DEBUG
	cout<<"iStartBitOffset: "<<iStartBitOffset<<"iOffset"<<iOffset<<"iDataSize"<<iDataSize<<"iTotalBytesMapped"<<iTotalBytesMapped<<endl;
	#endif
	for (INT32 iLoopCount = 0; iLoopCount < pobjCDT->varCollection.Count();
			iLoopCount++)
	{

		varDeclaration objVarDecl;

		objVarDecl.Initialize();
		objVarDecl = pobjCDT->varCollection[iLoopCount];
		if (objVarDecl.nam_id_dt_attr->getDtUniqueRefId() != NULL)
		{
			pobjCDT = pobjAppProc->getCDTbyUniqueID(
					objVarDecl.nam_id_dt_attr->getDtUniqueRefId());
			pobjAppProc->updatePreviousCDT_UId(objVarDecl.StructUniqueId,
					pobjCDT->Index);
			iLastVarIndex = iLoopCount;
			#if defined DEBUG
			cout<<"ProcessCDT Internal call 1 starts"<<endl;
			#endif
			ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter,
					enumPdoType, pbModuleName, pbModuleIndex);
			#if defined DEBUG
			cout<<"ProcessCDT Internal call 1 End"<<endl;
			#endif
		}
		if (!bCDTCompleted)
		{
			// add rest of the contents
			ProcessImage objProcessImage;
			objProcessImage.Initialize();
			objProcessImage.BitOffset = -1;
			objProcessImage.ByteOffset = 0;
			if (pobjParameter->access != NULL)
				strcpy(objProcessImage.Direction,
						getParameterAccess(pobjParameter->access));

			if (enumPdoType == PDO_TPDO)
			{
				objProcessImage.DirectionType = INPUT;
			}
			else if (enumPdoType == PDO_RPDO)
			{
				objProcessImage.DirectionType = OUTPUT;
			}
			else
			{
				//TODO: "else" Added. Nothing to do
			}

			if (objVarDecl.size != NULL)
			{
				objProcessImage.DataInfo.DataSize = atoi(objVarDecl.size);
			}
			else
			{
			}
			if (objVarDecl.nam_id_dt_attr->getName() != NULL)
			{
				objProcessImage.Name =
						(char*) malloc(
								strlen(
										objVarDecl.nam_id_dt_attr->getName()) + strlen(pbModuleName) + 6 + ALLOC_BUFFER);
				strcpy(objProcessImage.Name, getPIName(pobjNode->getNodeId()));
				strcat(objProcessImage.Name, pbModuleName);
				strcat(objProcessImage.Name, ".");
				strcat(objProcessImage.Name,
						objVarDecl.nam_id_dt_attr->getName());

				objProcessImage.ModuleName = new char[strlen(pbModuleName)
						+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.ModuleName, pbModuleName);

				objProcessImage.ModuleIndex = new char[strlen(pbModuleIndex)
						+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.ModuleIndex, pbModuleIndex);

				objProcessImage.VarName =
						new char[strlen(objVarDecl.nam_id_dt_attr->getName())
								+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.VarName,
						objVarDecl.nam_id_dt_attr->getName());
				//TODO: delete new char (memory Issue)
			}

			objProcessImage.CNNodeID = pobjNode->getNodeId();

			if (objVarDecl.nam_id_dt_attr->getDataType() != NULL)
			{
				objProcessImage.DataInfo._dt_Name = new char[strlen(
						objVarDecl.nam_id_dt_attr->getDataType())
						+ STR_ALLOC_BUFFER];
				strcpy(objProcessImage.DataInfo._dt_Name,
						(const char*) objVarDecl.nam_id_dt_attr->getDataType());
			}

			/* Set the IEC DT*/
			objProcessImage.DataInfo = *(getIECDT(
					objVarDecl.nam_id_dt_attr->getDataType(),
					objProcessImage.DataInfo.DataSize));

			/* Calculate Offset*/
			if (((objProcessImage.DataInfo._dt_enum != BITSTRING)
					&& (objProcessImage.DataInfo.DataSize >= 8))
					|| ((objProcessImage.DataInfo._dt_enum == BITSTRING)
							&& (iStartBitOffset == 0 || iStartBitOffset == 8
									|| iStartBitOffset == 16
									|| iStartBitOffset == 32
									|| iStartBitOffset == 64)))
			{

				bIsNewBitStringVar = true;
				if (objProcessImage.DataInfo._dt_enum == BITSTRING)
				{
					iStartBitOffset = 0;
					iDataSize = 0;
					for (INT32 iBitStringCount = iLoopCount;
							iBitStringCount < pobjCDT->varCollection.Count();
							iBitStringCount++)
					{
						varDeclaration objVarDeclBitStr;

						objVarDeclBitStr.Initialize();
						objVarDeclBitStr =
								pobjCDT->varCollection[iBitStringCount];
						if (BITSTRING
								== ((*(getIECDT(
										objVarDeclBitStr.nam_id_dt_attr->getDataType(),
										objProcessImage.DataInfo.DataSize)))._dt_enum))
						{
							iDataSize += atoi(objVarDeclBitStr.size);
							if (8 == iDataSize || 16 == iDataSize
									|| 32 == iDataSize || 64 == iDataSize)
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
					iDataSize = objProcessImage.DataInfo.DataSize;

				}

				if (enumPdoType == PDO_RPDO)
				{
					iOffset = ComputeOUTOffset(iDataSize, enumPdoType);
				}
				else if (enumPdoType == PDO_TPDO)
				{
					iOffset = ComputeINOffset(iDataSize, enumPdoType);
				}
				else
				{
					//TODO: "else" Added.Operation need to be specified
				}
			}
			/* Set the Byte Offet*/
			objProcessImage.ByteOffset = iOffset;
			if (bIsNewBitStringVar)
			{
				/* Total bytes Mapped */
				iTotalBytesMapped = iTotalBytesMapped + (iDataSize / 8);
				if (iTotalBytesMapped > MAX_PI_SIZE)
				{
					ocfmException objex;
					objex.ocfm_Excpetion(OCFM_ERR_MAX_PI_SIZE);
					throw objex;
				}
				CreateMNPDOVar(iOffset, iDataSize,
						objProcessImage.DataInfo._dt_enum, enumPdoType,
						pobjNode);
			}

			if ((objProcessImage.DataInfo.DataSize >= 8)
					&& (iStartBitOffset != 0)
					&& (objProcessImage.DataInfo._dt_enum != BITSTRING))
			{
				iStartBitOffset = 0;
			}
			else if (objProcessImage.DataInfo._dt_enum == BITSTRING)
			{
				objProcessImage.BitOffset = iStartBitOffset;
				iStartBitOffset = iStartBitOffset
						+ objProcessImage.DataInfo.DataSize;
				objProcessImage.ByteOffset = iOffset;
				bIsNewBitStringVar = false;
			}
			else
			{
				//TODO: "else" Added.Operation need to be specified
			}
			pobjNode->addProcessImage(objProcessImage);


		}

	}

	if (pobjCDT->previousCDT_UId != NULL)
	{
		pobjCDT = pobjAppProc->getCDTbyUniqueID(pobjCDT->previousCDT_UId);

		for (INT32 iLoopCount = (iLastVarIndex + 1);
				iLoopCount < pobjCDT->varCollection.Count(); iLoopCount++)
		{
			if (!bCDTCompleted)
			{
				varDeclaration objVarDecl;
				objVarDecl = pobjCDT->varCollection[iLoopCount];
				if (objVarDecl.nam_id_dt_attr->getDtUniqueRefId() != NULL)
				{
					pobjCDT = pobjAppProc->getCDTbyUniqueID(
							objVarDecl.nam_id_dt_attr->getDtUniqueRefId());
					pobjAppProc->updatePreviousCDT_UId(
							objVarDecl.StructUniqueId, pobjCDT->Index);

					iLastVarIndex = iLoopCount;
					#if defined DEBUG
					cout<<"ProcessCDT Internal call 2 starts"<<endl;
					#endif
					ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter,
							enumPdoType, pbModuleName, pbModuleIndex);
					#if defined DEBUG
					cout<<"ProcessCDT Internal call 2 End"<<endl;
					#endif
					

				}
			}
		}
	}
	bCDTCompleted = true;
	#if defined DEBUG
	cout<<"iStartBitOffset: "<<iStartBitOffset<<"iOffset"<<iOffset<<endl;
	#endif
	//Returned current offset in bytes
	return iOffset;
}

/*****************************************************************************************/
/**
 \brief		DecodeUniqiueIDRef

 This function Decodes the uniqiueID Ref & process the complex data type collection accordingly

 \param		uniquedIdref	Character pointer to hold the uniquedId reference
 \param		pobjNode		Class pointer of CNode for node
 \param		enumPdoType		Enumeration flag for PDO type
 \param		pbModuleName	Character pointer to hold the Module name
 \param		pbModuleIndex	Character pointer to hold the Module Index value

 \return	INT32	Returns the current offset in bytes
 */
/******************************************************************************************/

INT32 DecodeUniqiueIDRef(char* uniquedIdref, CNode* pobjNode,
		EPDOType enumPdoType, char* pbModuleName, char* pbModuleIndex)
{
	if ((NULL == uniquedIdref) || (NULL == pobjNode) || (NULL == pbModuleName)
			|| (NULL == pbModuleIndex))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	ocfmException objocfmException;
	Parameter* pobjParameter = NULL;
	CApplicationProcess* pobjAppProc = NULL;
	CComplexDataType* pobjCDT = NULL;
	INT32 iCurOffset = 0;
	try
	{
		if (pobjNode->getApplicationProcess() != NULL)
		{

			pobjAppProc = pobjNode->getApplicationProcess();
			if (pobjAppProc->ParameterCollection.Count() != 0)
			{

				pobjParameter = pobjAppProc->get_Parameterby_UniqueIDRef(
						uniquedIdref);
				if (pobjParameter == NULL)
				{
					objocfmException.ocfm_Excpetion(
							OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND);
					throw objocfmException;
				}

				// Check if DataTypeUniqueIDref exists
				if (pobjParameter->name_id_dt_attr.dataTypeUniqueIDRef != NULL)
				{

					pobjCDT = pobjAppProc->getCDTbyUniqueID(
							pobjParameter->name_id_dt_attr.dataTypeUniqueIDRef);
					if (pobjCDT == NULL)
					{
						objocfmException.ocfm_Excpetion(
								OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND);
						throw objocfmException;
					}
					iCurOffset = ProcessCDT(pobjCDT, pobjAppProc, pobjNode, pobjParameter,
							enumPdoType, pbModuleName, pbModuleIndex);
					iLastVarIndex = -1;
					bCDTCompleted = false;
				}
				else
				{
#if defined DEBUG
					cout << "Data type of unique id does not exists" << endl;
#endif
				}
			}

		}

	} catch (ocfmException& ex)
	{
		throw ex;
	}
	return iCurOffset;
}

/*****************************************************************************************/
/**
 \brief		ProcessPDONodes

 This API Processes the PDO nodes of the Controlled node

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode ProcessPDONodes()
{
	return (ProcessPDONodes(false));
}

/*****************************************************************************************/
/**
 \brief		ProcessPDONodes

 This function Processes the PDO Nodes of the controlled node and generates a pdo collection that is used to create the MN pdo's & the process image variables

 \param		IsBuild		Boolean flag to hold the value true if it is Build process

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode ProcessPDONodes(bool IsBuild)
{
	CNodeCollection *objNodeCol = NULL;
	ocfmException objocfmException;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode *pobjNode = NULL;
	//CNode *pobjMNNode = NULL;
	INT32 iTotalBytesMapped = 0;
	INT32 iTotalChainedBytesMapped = 0;
	INT32 iNodeMappedTotalBytes = 0;
	INT32 iNodeRPDOMappedNodeID = 0;

	CIndexCollection *objPDOCollection = NULL;
	CIndexCollection *pobjIndexCollection = NULL;
	/* Check RPDO Mapped objects*/
	INT32 iNodesCount = 0;
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	stRetInfo.errorString = NULL;
	// bool bChangeOffset = false;

	iNodesCount = objNodeCol->getCNNodesCount();

#if defined DEBUG
	cout << "Nodes count" << iNodesCount << endl;
#endif

	try
	{
		if (iNodesCount == 0)
		{
			exit(0);
		}

		stSize8INOffset.currOffset = 0;
		stSize8INOffset.prevOffset = 0;
		stSize16INOffset.currOffset = 0;
		stSize16INOffset.prevOffset = 0;
		stSize32INOffset.currOffset = 0;
		stSize32INOffset.prevOffset = 0;
		stSize64INOffset.currOffset = 0;
		stSize64INOffset.prevOffset = 0;

		stSize8OUTOffset.currOffset = 0;
		stSize8OUTOffset.prevOffset = 0;
		stSize16OUTOffset.currOffset = 0;
		stSize16OUTOffset.prevOffset = 0;
		stSize32OUTOffset.currOffset = 0;
		stSize32OUTOffset.prevOffset = 0;
		stSize64OUTOffset.currOffset = 0;
		stSize64OUTOffset.prevOffset = 0;

		INT32 *pArrangedNodeIDbyStation = NULL;
		pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();
		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
				iLoopCount++)
		{

			pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
			/* Process PDO Objects for CN*/

			if (pobjNode->getNodeType() == CN)
			{
				if (!(pobjNode->HasPdoObjects()))
				{
					continue;
				}
				EStationType eNodeStation = pobjNode->getStationType();

				/* Empty ProcessImage collection (Axxx objects)*/
				pobjNode->DeleteCollectionsForPI();

				INT32 countTPDO = 0;
				INT32 countRPDO = 0;
				objPDOCollection = pobjNode->getPDOIndexCollection(&countRPDO, &countTPDO);

				if (objPDOCollection == NULL)
				{
					cout << "objPDOCollection NULL" << endl;
					continue;
				}
				//Validate the number of TPDO's for a CN
				if(countTPDO > 1)
				{
					objocfmException.ocfm_Excpetion(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
					char acCustomError[200] = { 0 };
					sprintf(acCustomError, "CN Node-Id: %d cannot have more than one TPDO Channel", pobjNode->getNodeId());
					CopyCustomErrorString(&(objocfmException._ocfmRetCode), acCustomError);
					throw objocfmException;
				}

				pobjIndexCollection = pobjNode->getIndexCollection();

				pobjNode->setPReqActPayloadValue(0);
				pobjNode->setPResActPayloadValue(0);

				for (INT32 iLoopCount = 0;
						iLoopCount < objPDOCollection->getNumberofIndexes();
						iLoopCount++)
				{

					CIndex *pobjBforeSortIndex = NULL;
					CIndex objIndex;
					pobjBforeSortIndex = objPDOCollection->getIndex(iLoopCount);
					if (!(CheckIfMappingPDO(
							(char*) pobjBforeSortIndex->getIndexValue())))
					{
						continue;
					}
					else
					{
#if defined DEBUG
						cout << "index:"
						<< (char*) pobjBforeSortIndex->getIndexValue()
						<< endl;
#endif
					}
					iNodeMappedTotalBytes = 0;
					// bChangeOffset = false;

					if (pobjBforeSortIndex->getNumberofSubIndexes() > 0)
					{

						/* Sort the pdo collection */
						objIndex = getPDOIndexByOffset(pobjBforeSortIndex);

						INT32 iSiCount = 1;

						// Initialised to Zero and the value will be taken from the Actual value or the default value in priority
						INT32 iSiTotal = 0;
						//check whether the channel is activated
						CSubIndex *pobjNoofEntriesSubIndex = NULL;
						pobjNoofEntriesSubIndex =
								pobjBforeSortIndex->getSubIndexbyIndexValue(
										(char *) "00");
						if (NULL == pobjNoofEntriesSubIndex)
						{
							continue;
						}
						// Actual value checked for Null, Empty, non-zero
						if ((pobjNoofEntriesSubIndex->getActualValue() != NULL)
								&& (0
										!= strcmp(
												pobjNoofEntriesSubIndex->getActualValue(),
												""))
								&& !(checkIfValueZero(
										(char*) pobjNoofEntriesSubIndex->getActualValue())))
						{
							//value is not zero the channel is activated
							iSiTotal =
									GetDecimalValue(
											(char*) pobjNoofEntriesSubIndex->getActualValue());
#if defined DEBUG
							cout << "iSiTotal:" << iSiTotal << endl;
#endif
						}
						else
						{
							if (0
									== strcmp(
											pobjNoofEntriesSubIndex->getActualValue(),
											""))
							{
								//pdo channel is deactivated. Empty act value
								continue;
							}
							if (checkIfValueZero(
									(char*) pobjNoofEntriesSubIndex->getActualValue()))
							{
								// PDO channel is deactivated
								// Zero is not set here,as it is intialised to Zero previously
								continue;
							}
							else // If the Actual values is Null or Empty, Default value is set for Total SIdx for mapping
							{
								//No need to check for value null or empty. GetDecimalValue returns zero or particular value.
								iSiTotal =
										GetDecimalValue(
												(char*) pobjNoofEntriesSubIndex->getDefaultValue());
							}
						}

						//Check isiTotal value is valid
						if (iSiTotal
								>= (pobjBforeSortIndex->getNumberofSubIndexes()))
						{
							objocfmException.ocfm_Excpetion(
									OCFM_ERR_MODULE_INDEX_NOT_FOUND);
							char acCustomError[200] =
							{ 0 };
							sprintf(acCustomError,
									"Mapping objects not found in index: %s in node: %d",
									(char*) pobjBforeSortIndex->getIndexValue(),
									pobjNode->getNodeId());
							CopyCustomErrorString(
									&(objocfmException._ocfmRetCode),
									acCustomError);

							throw objocfmException;
						}

						//set the correponding 14xx/01 to f0
						if ((true == IsBuild)
								&& (strncmp(objIndex.getIndexValue(), "16", 2)
										== 0))
						{
							CIndex *pobjCommIndex = NULL;

							char *pcIdx = subString(
									(char *) objIndex.getIndexValue(), 2, 4);
							char *pcCommIdx = new char[INDEX_LEN];
							strcpy(pcCommIdx, (char *) "14");
							strcat(pcCommIdx, pcIdx);
							pobjCommIndex =
									pobjIndexCollection->getIndexbyIndexValue(
											pcCommIdx);
							if (NULL != pobjCommIndex)
							{
								CSubIndex *pobjNodeIDSubIndex = NULL;
								pobjNodeIDSubIndex =
										pobjCommIndex->getSubIndexbyIndexValue(
												(char *) "01");
								if (NULL != pobjNodeIDSubIndex)
								{
									if ((NULL
											!= pobjNodeIDSubIndex->getActualValue())
											&& (0
													!= strcmp(
															pobjNodeIDSubIndex->getActualValue(),
															"")))
									{
										iNodeRPDOMappedNodeID =
												GetDecimalValue(
														(char *) pobjNodeIDSubIndex->getActualValue());
										if ((CHAINED == eNodeStation)
												&& (BROADCAST_NODEID
														== iNodeRPDOMappedNodeID))
										{
											pobjNodeIDSubIndex->setActualValue(
													(char *) "0xF0");
											//  bChangeOffset = true;
										}

										if ((CHAINED != eNodeStation)
												&& (MN_NODEID
														== iNodeRPDOMappedNodeID))
										{
											pobjNodeIDSubIndex->setActualValue(
													(char *) "0x0");
											//  bChangeOffset = true;
										}
										iNodeRPDOMappedNodeID =
												GetDecimalValue(
														(char *) pobjNodeIDSubIndex->getActualValue());
										if ((BROADCAST_NODEID
												!= iNodeRPDOMappedNodeID))
										{
											if ((MN_NODEID
													!= iNodeRPDOMappedNodeID))
											{
												continue;
											}
										}
									}
								}
							}
							delete[] pcCommIdx;
							delete[] pcIdx;
						}
						//CN's cannot have 18xx & 1Axx other than 1800 & 1A00 mapped for MN

						INT32 iPrevOffsetUniqueId = -1;
						while (iSiCount <= iSiTotal)
						{
							CSubIndex *pobjSubIdx = NULL;
							pobjSubIdx = objIndex.getSubIndex(iSiCount);
#if defined DEBUG	
							cout << "\n pobjSubIdx->getIndexValue():"
							<< pobjSubIdx->getIndexValue() << endl;
							cout << "\n pobjSubIdx->getName():"
							<< pobjSubIdx->getName() << endl;
#endif
							if ((NULL != pobjSubIdx->getActualValue())
									&& (0
											!= strcmp(
													pobjSubIdx->getActualValue(),
													"")))
							{

								const char* pbActualVal =
										pobjSubIdx->getActualValue();
								INT32 iLength = strlen(pbActualVal);
								char* pbModuleIndex = NULL; // = new char[INDEX_SIZE + ALLOC_BUFFER];
								pbModuleIndex = subString((char*) pbActualVal,
										iLength - 4, 4);
								pbModuleIndex[5] = '\0';

								/* Get the SubIndex*/
								char* pbSubIndex = NULL; // = new char[SUBINDEX_SIZE + ALLOC_BUFFER];
								//pbSubIndex = subString(reverseValue,2,2);
								pbSubIndex = subString((char*) pbActualVal,
										iLength - 6, 2);
								pbSubIndex[3] = '\0';

#if defined DEBUG	
								cout << "pbModuleIndex:" << pbModuleIndex << "pbSubIndex:" << pbSubIndex << endl;
#endif

								//Mapped length in bits
								INT32 iMappedLength = 0;
								iMappedLength = hex2int(subString((char*) pbActualVal, 2, 4));
								cout<<" IntMapLength:"<<iMappedLength<<endl;

								CIndex *pobjModuleIndex = NULL;
								CSubIndex *pobjModuleSIndex = NULL;
								char *uniqueidRefID = NULL;
								char *pbSIdxName = NULL;
								char *Access = NULL;
								char *pbModuleName = NULL;
								DataType dt;
								dt.Name = NULL;
								bool bObjectMapped = false;

								pobjModuleIndex =
										pobjIndexCollection->getIndexbyIndexValue(
												pbModuleIndex);
								if (pobjModuleIndex == NULL)
								{
									objocfmException.ocfm_Excpetion(
											OCFM_ERR_MODULE_INDEX_NOT_FOUND);
									char acCustomError[200] =
									{ 0 };
									sprintf(acCustomError,
											"PDO Mapped Module Index Not Found, Index:%s in Node ID:%d",
											pbModuleIndex,
											pobjNode->getNodeId());
									CopyCustomErrorString(
											&(objocfmException._ocfmRetCode),
											acCustomError);

									throw objocfmException;
								}

								if (pobjModuleIndex->getName() != NULL)
								{

									pbModuleName = new char[strlen(
											pobjModuleIndex->getName())
											+ ALLOC_BUFFER];
									strcpy(pbModuleName,
											pobjModuleIndex->getName());
								}
								if (pobjModuleIndex->getNumberofSubIndexes()
										== 0 && (strcmp(pbSubIndex, "00") == 0))
								{
									bObjectMapped = true;
									if (pobjModuleIndex->getUniqueIDRef()!= NULL)
									{
										uniqueidRefID =
												new char[strlen(
														pobjModuleIndex->getUniqueIDRef())
														+ ALLOC_BUFFER];
										strcpy(uniqueidRefID,
												pobjModuleIndex->getUniqueIDRef());
									}
									else
									{
										if (pobjModuleIndex->getAccessType() != NULL)
										{
											Access =
													new char[strlen(
															pobjModuleIndex->getAccessType())
															+ ALLOC_BUFFER];
											strcpy(Access,
													pobjModuleIndex->getAccessType());

										}
										dt = pobjModuleIndex->getDataType();
									}

								}
								else
								{
									pobjModuleSIndex =
											pobjModuleIndex->getSubIndexbyIndexValue(
													pbSubIndex);
									if (pobjModuleSIndex == NULL)
									{
										objocfmException.ocfm_Excpetion(
												OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
										char acCustomError[200] =
										{ 0 };
										sprintf(acCustomError,
												"PDO Mapped Module SubIndex Not Found, Index:%s Subindex:%s in Node ID:%d",
												pbModuleIndex, pbSubIndex,
												pobjNode->getNodeId());
										CopyCustomErrorString(
												&(objocfmException._ocfmRetCode),
												acCustomError);
										if (pbModuleName != NULL)
										{
											delete[] pbModuleName;
										}
										throw objocfmException;
									}
									else
									{
										if (pobjModuleSIndex->getUniqueIDRef() != NULL)
										{
											uniqueidRefID =
													new char[strlen(
															pobjModuleSIndex->getUniqueIDRef())
															+ ALLOC_BUFFER];
											strcpy(uniqueidRefID,
													pobjModuleSIndex->getUniqueIDRef());
										}
										else
										{
											if (pobjModuleSIndex->getName() != NULL)
											{
												pbSIdxName =
														new char[strlen(
																pobjModuleSIndex->getName())
																+ ALLOC_BUFFER];
												strcpy(pbSIdxName,
														pobjModuleSIndex->getName());

												if (pobjModuleSIndex->getAccessType() != NULL)
												{
													Access =
															new char[strlen(
																	pobjModuleSIndex->getAccessType())
																	+ ALLOC_BUFFER];
													strcpy(Access,
															pobjModuleSIndex->getAccessType());
												}
												dt =
														pobjModuleSIndex->getDataType();
											}
										}
									}

								}

								EPDOType pdoType = objIndex.getPDOType();

								if (uniqueidRefID != NULL)
								{
									INT32 iOffset = 0;
									iOffset = DecodeUniqiueIDRef(uniqueidRefID, pobjNode,
											pdoType,
											(char*) pobjModuleIndex->getName(),
											(char*) pobjModuleIndex->getIndexValue());
									if(iMappedLength != ((iOffset - iPrevOffsetUniqueId)*8))
									{
										ocfmException objex;
										objex.ocfm_Excpetion(OCFM_ERR_INVALID_SIZE_MAPPED);

										char acCustomError[200] = { 0 };
										sprintf(acCustomError, "Invalid Length for the mapping object. Index: %s SubIndex: %s in node: %d", (char*)objIndex.getIndexValue(), (char*)pobjSubIdx->getIndexValue(), pobjNode->getNodeId());
										CopyCustomErrorString(&(objex._ocfmRetCode), acCustomError);

										throw objex;
									}
									iPrevOffsetUniqueId = iOffset;
								}
								else
								{
									ProcessImage objProcessImage;
									objProcessImage.Initialize();
									if (dt.getName() == NULL)
									{
										objocfmException.ocfm_Excpetion(
												OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
										throw objocfmException;
									}
									else if (!CheckAllowedDTForMapping(
											dt.getName()))
									{
										objocfmException.ocfm_Excpetion(
												OCFM_ERR_INVALID_DATATYPE_FOR_PDO);
										throw objocfmException;
									}
									objProcessImage.CNNodeID =
											pobjNode->getNodeId();
									/* Name of the Process Image variable*/
									objProcessImage.Name = (char*) malloc(
											6 + ALLOC_BUFFER);
									strcpy(objProcessImage.Name,
											getPIName(pobjNode->getNodeId()));

									if (pbModuleName != NULL)
									{
										objProcessImage.Name =
												(char*) realloc(
														objProcessImage.Name,
														strlen(
																objProcessImage.Name)+ strlen(pbModuleName) + ALLOC_BUFFER);
										strcat(objProcessImage.Name,
												pbModuleName);

										objProcessImage.ModuleName =
												new char[strlen(pbModuleName)
														+ STR_ALLOC_BUFFER];
										strcpy(objProcessImage.ModuleName,
												pbModuleName);
									}
									else
									{
										/*If the object name is empty, Ixxxx (xxxx is the index number in hex) */
										pbModuleName = new char[INDEX_LEN + 1
												+ ALLOC_BUFFER];
										strcpy(pbModuleName, "I");
										strcat(pbModuleName,
												objIndex.getIndexValue());

										objProcessImage.Name =
												(char*) realloc(
														objProcessImage.Name,
														strlen(
																objProcessImage.Name)+ strlen(pbModuleName) + ALLOC_BUFFER);
										strcat(objProcessImage.Name,
												pbModuleName);

										objProcessImage.ModuleName =
												new char[strlen(pbModuleName)
														+ STR_ALLOC_BUFFER];
										strcpy(objProcessImage.ModuleName,
												pbModuleName);
									}

									objProcessImage.ModuleIndex =
											new char[strlen(pbModuleIndex)
													+ STR_ALLOC_BUFFER];
									strcpy(objProcessImage.ModuleIndex,
											pbModuleIndex);

									if (bObjectMapped)
									{
										objProcessImage.VarName =
												new char[strlen(pbModuleName)
														+ STR_ALLOC_BUFFER];
										strcpy(objProcessImage.VarName,
												pbModuleName);
									}
									else
									{
										if (pbSIdxName != NULL)
										{
											objProcessImage.VarName =
													new char[strlen(pbSIdxName)
															+ STR_ALLOC_BUFFER];
											strcpy(objProcessImage.VarName,
													pbSIdxName);

											objProcessImage.Name =
													(char*) realloc(
															objProcessImage.Name,
															(strlen(
																	objProcessImage.Name)
																	+ strlen(
																			pbSIdxName)
																	+ 1
																	+ ALLOC_BUFFER));
											strcat(objProcessImage.Name, ".");
											strcat(objProcessImage.Name,
													pbSIdxName);
										}
										else
										{
											//TODO: pobjModulesubindex may be null. Check for null befor used.

											/*If the subobject name is empty, use Sxx (xx is the subindex number in hex) */
											pbSIdxName = new char[SUBINDEX_LEN
													+ 1 + ALLOC_BUFFER];

											strcpy(pbSIdxName, "S");
											strcat(pbSIdxName,
													pobjModuleSIndex->getIndexValue());

											objProcessImage.VarName =
													new char[strlen(pbSIdxName)
															+ STR_ALLOC_BUFFER];
											strcpy(objProcessImage.VarName,
													pbSIdxName);

											objProcessImage.Name =
													(char*) realloc(
															objProcessImage.Name,
															(strlen(
																	objProcessImage.Name)
																	+ strlen(
																			pbSIdxName)
																	+ 1
																	+ ALLOC_BUFFER));
											strcat(objProcessImage.Name, ".");
											strcat(objProcessImage.Name,
													pbSIdxName);

										}
									}

#if defined DEBUG
									cout<<"DataSize: "<<dt.DataSize*8<<" MappedLength: "<<iMappedLength<<endl;
#endif
									if((dt.DataSize * 8) != iMappedLength)
									{
										ocfmException objex;
										objex.ocfm_Excpetion(OCFM_ERR_INVALID_SIZE_MAPPED);
										char acCustomError[200] = { 0 };
										sprintf(acCustomError, "Invalid Length for the mapping object. Index: %s SubIndex: %s in node: %d", (char*)objIndex.getIndexValue(), (char*)pobjSubIdx->getIndexValue(), pobjNode->getNodeId());
										CopyCustomErrorString(&(objex._ocfmRetCode), acCustomError);

										throw objex;
									}
									objProcessImage.DataInfo.DataSize =
											dt.DataSize * 8;

									/* Total bytes Mapped */
									iTotalBytesMapped = iTotalBytesMapped
											+ dt.DataSize;
									if (iTotalBytesMapped > MAX_PI_SIZE)
									{
										ocfmException objex;
										objex.ocfm_Excpetion(
												OCFM_ERR_MAX_PI_SIZE);
										delete[] pbSIdxName;
										delete[] Access;
										delete[] pbModuleName;
										throw objex;
									}
									/* Datatype in hex of the Process Image variable*/
									objProcessImage.DataInfo._dt_Name =
											new char[strlen(dt.Name)
													+ STR_ALLOC_BUFFER];
									strcpy(objProcessImage.DataInfo._dt_Name,
											dt.Name);
									objProcessImage.DataInfo._dt_enum =
											dt.IEC_dt;

									if (pdoType == PDO_TPDO)
									{
										objProcessImage.ByteOffset =
												ComputeINOffset(
														objProcessImage.DataInfo.DataSize,
														pdoType);
										objProcessImage.DirectionType = INPUT;
									}
									else if (pdoType == PDO_RPDO)
									{
										objProcessImage.ByteOffset =
												ComputeOUTOffset(
														objProcessImage.DataInfo.DataSize,
														pdoType);
										objProcessImage.DirectionType = OUTPUT;
									}

									objProcessImage.BitOffset = 0;
									CreateMNPDOVar(objProcessImage.ByteOffset,
											objProcessImage.DataInfo.DataSize,
											objProcessImage.DataInfo._dt_enum,
											pdoType, pobjNode);
									pobjNode->addProcessImage(objProcessImage);
									delete[] pbModuleName;
									delete[] pbSIdxName;
								}

								if ((true == IsBuild)
										&& (strncmp(objIndex.getIndexValue(),
												"16", 2) == 0)
										&& ((MN_NODEID == iNodeRPDOMappedNodeID)
												|| (BROADCAST_NODEID
														== iNodeRPDOMappedNodeID)))
								{
									char* pbModOffset = new char[strlen(
											pbActualVal) + 1];
									strcpy(pbModOffset, pbActualVal);
									INT32 iLength = 0;
									char* pcLength = NULL;
									//extract the length mapped 1AXX
									pcLength = subString((char *) pbActualVal,
											2, 4);
									iLength = hex2int(pcLength);

									char* offset = new char[5];
									memset(offset, 0, 5 * sizeof(char));
									if (CHAINED == eNodeStation)
									{
										offset = _IntToAscii(
												iTotalChainedBytesMapped,
												&(offset[0]), 16);
									}
									else
									{
										offset = _IntToAscii(
												iNodeMappedTotalBytes,
												&(offset[0]), 16);
									}
									offset = padLeft(&(offset[0]), '0', 4);
									INT32 iOffsetCopyCount;
									for (iOffsetCopyCount = 0;
											iOffsetCopyCount <= 3;
											iOffsetCopyCount++)
									{
										pbModOffset[iOffsetCopyCount + 2 + 4] =
												offset[iOffsetCopyCount];
									}
									strcpy(pbModOffset,
											ConvertToUpper(pbModOffset));

									pobjIndexCollection->getIndexbyIndexValue(
											(char *) pobjBforeSortIndex->getIndexValue())->getSubIndexbyIndexValue(
											(char *) pobjSubIdx->getIndexValue())->setActualValue(
											pbModOffset);

									iNodeMappedTotalBytes =
											iNodeMappedTotalBytes + iLength;

									if (CHAINED == eNodeStation)
									{
										iTotalChainedBytesMapped =
												iTotalChainedBytesMapped
														+ iLength;

									}

									delete[] pbModOffset;
									delete[] pcLength;
									delete[] offset;
								}
							}
							iSiCount++;
						}
					}
				}
			}
			else
			{
				// pobjMNNode = pobjNode;
			}

		}
		delete[] pArrangedNodeIDbyStation;
		//The PI variable name should be unique
		SetUniquePIVarName();
		//find the time of build
		SetBuildTime();

		stRetInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************************/
/**
 \brief		CalculatePayload

 This function calculates the PReq and PRes payload value for node

 \return	void
 */
/******************************************************************************************/

void CalculatePayload()
{
//TODO: objocfmException is not catched. Try catch block should be used.
	CNodeCollection *objNodeCol = NULL;

	objNodeCol = CNodeCollection::getNodeColObjectPointer();

	//	INT32 iNodeMappedTotalBytes = 0;

	/*Check RPDO Mapped objects*/

	if (objNodeCol->getCNNodesCount() == 0)
	{
		exit(0);
	}
	ocfmException objocfmException;
	CNode *pobjNode = NULL;
	CNode *pobjMNNode = NULL;
	INT32 iTotalChainedBytesMapped = 0;
	INT32* pArrangedNodeIDbyStation = NULL;
	pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();
	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
			iLoopCount++)
	{
		pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
		/* Process PDO Objects for CN*/

		if (pobjNode->getNodeType() == MN)
		{
			pobjMNNode = pobjNode;
		}
		else
		{
			if (!(pobjNode->HasPdoObjects()))
			{
				continue;
			}

			EStationType eNodeStation = pobjNode->getStationType();
			CIndexCollection* objPDOCollection = NULL;
			INT32 tpdoCount = 0;
			INT32 rpdoCount = 0;
			objPDOCollection = pobjNode->getPDOIndexCollection(&rpdoCount, &tpdoCount);

			if (objPDOCollection != NULL)
			{
				CIndexCollection* pobjIndexCollection = NULL;
				pobjIndexCollection = pobjNode->getIndexCollection();

				pobjNode->setPReqActPayloadValue(0);
				pobjNode->setPResActPayloadValue(0);

				for (INT32 iLoopCount = 0;
						iLoopCount < objPDOCollection->getNumberofIndexes();
						iLoopCount++)
				{
					CIndex* pobjBforeSortIndex;
					CIndex objIndex;
					pobjBforeSortIndex = objPDOCollection->getIndex(iLoopCount);
					if (!(CheckIfMappingPDO(
							(char*) pobjBforeSortIndex->getIndexValue())))
					{
						continue;
					}
					//	iNodeMappedTotalBytes = 0;

					if (pobjBforeSortIndex->getNumberofSubIndexes() > 0)
					{
						/* Sort the pdo collection */
						objIndex = getPDOIndexByOffset(pobjBforeSortIndex);
						INT32 iSiCount = 1;
						INT32 iSiTotal = 0;

						CSubIndex *pobjNoofEntriesSubIndex;
						pobjNoofEntriesSubIndex =
								pobjBforeSortIndex->getSubIndexbyIndexValue(
										(char *) "00");
						if (NULL != pobjNoofEntriesSubIndex)
						{
							if ((pobjNoofEntriesSubIndex->getActualValue()
									!= NULL) // Actual value checked for Null
									&& (0
											!= strcmp(
													pobjNoofEntriesSubIndex->getActualValue(),
													"")) // Actual value checked for Empty
									&& !(checkIfValueZero(
											(char*) pobjNoofEntriesSubIndex->getActualValue()))) // Actual value checked for non-zero
							{
								//value is not zero the channel is activated
								iSiTotal =
										GetDecimalValue(
												(char*) pobjNoofEntriesSubIndex->getActualValue());
#if defined DEBUG	
								cout << "iSiTotal:" << iSiTotal << endl;
#endif
							}
							else
							{
								if (0
										== strcmp(
												pobjNoofEntriesSubIndex->getActualValue(),
												""))
								{
									//pdo channel is deactivated. Empty act value
									continue;
								}
								if (checkIfValueZero(
										(char*) pobjNoofEntriesSubIndex->getActualValue()))
								{
									// PDO channel is deactivated
									// Zero is not set here,as it is intialised to Zero previously
									continue;
								}
								else // If the Actual values is Null or Empty, Default value is set for Total SIdx for mapping
								{
									//No need to check for value null or empty. GetDecimalValue returns zero or particular value.
									iSiTotal =
											GetDecimalValue(
													(char*) pobjNoofEntriesSubIndex->getDefaultValue());
								}
							}

						}
						else
						{
							//no of entries index does not exist
							continue;
						}
						//Check isiTotal value is valid
						if (iSiTotal
								>= (pobjBforeSortIndex->getNumberofSubIndexes()))
						{
							objocfmException.ocfm_Excpetion(
									OCFM_ERR_MODULE_INDEX_NOT_FOUND);
							char acCustomError[200] =
							{ 0 };
							sprintf(acCustomError,
									"Mapping objects not found in index: %s in node: %d",
									(char*) pobjBforeSortIndex->getIndexValue(),
									pobjNode->getNodeId());
							CopyCustomErrorString(
									&(objocfmException._ocfmRetCode),
									acCustomError);

							throw objocfmException;
						}

						INT32 iNodeRPDOMappedNodeID = -1;
						if (strncmp(objIndex.getIndexValue(), "16", 2) == 0)
						{
							CIndex *pobjCommIndex = NULL;
							char *pcIdx = subString(
									(char *) objIndex.getIndexValue(), 2, 4);
							char *pcCommIdx = new char[INDEX_LEN];
							strcpy(pcCommIdx, (char *) "14");
							strcat(pcCommIdx, pcIdx);
							pobjCommIndex =
									pobjIndexCollection->getIndexbyIndexValue(
											pcCommIdx);
							if (NULL != pobjCommIndex)
							{
								CSubIndex *pobjNodeIDSubIndex = NULL;
								pobjNodeIDSubIndex =
										pobjCommIndex->getSubIndexbyIndexValue(
												(char *) "01");
								if (NULL != pobjNodeIDSubIndex)
								{
									iNodeRPDOMappedNodeID =
											GetDecimalValue(
													(char*) pobjNodeIDSubIndex->getActualValue());
								}
							}
							delete[] pcCommIdx;
							delete[] pcIdx;
						}

						while (iSiCount <= iSiTotal)
						{

							CSubIndex* pobjSubIdx;
							pobjSubIdx = objIndex.getSubIndex(iSiCount);

							iSiCount++;

							if ((pobjSubIdx->getActualValue() == NULL)
									|| (0
											== strcmp(
													pobjSubIdx->getActualValue(),
													""))
									|| (checkIfValueZero(
											(char*) pobjSubIdx->getActualValue())))
							{
								continue;
							}

							const char* pbActualVal =
									pobjSubIdx->getActualValue();

							if ((strncmp(objIndex.getIndexValue(), "16", 2) == 0)
									&& ((MN_NODEID == iNodeRPDOMappedNodeID)
											|| (BROADCAST_NODEID
													== iNodeRPDOMappedNodeID)))
							{
								char* pbModOffset = new char[strlen(pbActualVal)
										+ 1];
								strcpy(pbModOffset, pbActualVal);
								INT32 iLength = 0;
								INT32 iOffset = 0;

								char* pcLength = NULL;
								pcLength = subString((char *) pbActualVal, 2,
										4);
								iLength = hex2int(pcLength);

								char* offset = NULL;
								offset = subString((char *) pbActualVal, 6, 4);
								iOffset = hex2int(offset);

								//	iNodeMappedTotalBytes = iOffset + iLength;

								if (CHAINED == eNodeStation)
								{
									iTotalChainedBytesMapped = iOffset
											+ iLength;
								}
								if (BROADCAST_NODEID == iNodeRPDOMappedNodeID)
								{
									pobjNode->setPReqActPayloadValue(
											(iOffset + iLength) / 8);
								}

								delete[] pbModOffset;
								delete[] pcLength;
								delete[] offset;
							}
							if (strncmp(objIndex.getIndexValue(), "1A", 2) == 0)
							{
								char* pbModOffset = new char[strlen(pbActualVal)
										+ 1];
								strcpy(pbModOffset, pbActualVal);
								INT32 iLength = 0;
								char* pcLength = NULL;
								pcLength = subString((char *) pbActualVal, 2,
										4);
								iLength = hex2int(pcLength);

								char* pcOffset = NULL;
								pcOffset = subString((char *) pbActualVal, 6,
										4);
								INT32 iOffset = 0;
								iOffset = hex2int(pcOffset);

								pobjNode->setPResActPayloadValue(
										(iOffset + iLength) / 8);

								delete[] pbModOffset;
								delete[] pcLength;
								delete[] pcOffset;
							}
						}
					}
				}
			}
			UpdatePreqActLoad(pobjNode);
			UpdatePresActLoad(pobjNode);
		}
	}
	if ((NULL != pobjMNNode) && (true == IsPresMN()))
	{
		pobjMNNode->setPResActPayloadValue(iTotalChainedBytesMapped / 8);
		UpdatePresActLoad(pobjMNNode);
	}
	delete[] pArrangedNodeIDbyStation;
}

/*****************************************************************************************/
/**
 \brief		getCNDataLen

 This function returns the total size of data in the buffer

 \param		pbBuffer		Character pointer to hold the buffer data

 \return	INT32
 */
/******************************************************************************************/

INT32 getCNDataLen(char* pbBuffer)
{
	if (NULL == pbBuffer)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	INT32 iCount = 0;
	INT32 iNoOfChars = 0;
	char cCharRead = 0;
	INT32 iCtr = 0;
	INT32 iLength = 0;
	INT32 iLoopCount = 0;
	//UINT8 abCnObd[60000];
	//UINT8 abTempCnObd[60000];

	cCharRead = *(pbBuffer);

	while (cCharRead != '\0')
	{
		cCharRead = *(pbBuffer + iLoopCount);
		if (cCharRead == '/')
		{
			while (cCharRead != '\n')
			{
				cCharRead = *(pbBuffer + iLoopCount);
				iLoopCount++;
			}
		}
		else if (cCharRead != '\t' || cCharRead != '\n' || cCharRead != ' ')
		{
			INT32 iCharRead = 0;
			// Convert to Upper case
			iCharRead = toupper(cCharRead);
			if ((iCharRead >= 65 && iCharRead <= 70)
					|| (iCharRead >= 97 && iCharRead <= 102))
			{
				//abCnObd[iNoOfChars] = iCharRead - 55;
				iNoOfChars++;
			}
			else if ((iCharRead >= 48 && iCharRead <= 57))
			{
				//abCnObd[iNoOfChars] = iCharRead - 48;
				iNoOfChars++;
			}
			else
			{
				//TODO: 'else' added. Operation to be specified
			}
			iLoopCount++;
		}
		else
		{
			//TODO: 'else' added. Operation to be specified
		}

		iLength = iNoOfChars;
		//For Byte Packing
		for (iCtr = 0, iCount = 0; iCtr < iLength; iCtr++, iCount++)
		{
			//abTempCnObd[iCount] = (UINT8)( ( abCnObd[ iCtr ] << 4 ) | abCnObd[ iCtr + 1 ] );
			iCtr++;
		}
	}
	return iCount;
}

/*****************************************************************************************/
/**
 \brief		lenOfCNBuffer

 This function returns the total size of data in the buffer

 \param		pbBuffer		Character pointer to hold the buffer data

 \return	INT32
 */
/******************************************************************************************/
//TODO: unused function
INT32 lenOfCNBuffer(char* pbBuffer)
{
	char cTempChar;
	INT32 iLength = strlen(pbBuffer);
	INT32 iLoopCount = 0;
	INT32 iActualLen = 0;

	while (iLoopCount < iLength)
	{
		cTempChar = *(pbBuffer + iLoopCount);
		if (cTempChar != '\n' && cTempChar != '\t')
			iActualLen++;
		iLoopCount++;
	}
	return iActualLen;

}

/*****************************************************************************************/
/**
 \brief			getMNIndexValues

 This function returns the CIndex class pointer for the index id

 \param			pbIndex		Character pointer to hold the Index id value

 \return	CIndex*
 */
/******************************************************************************************/

CIndex* getMNIndexValues(char* pbIndex)
{
	CNodeCollection* pobjNodeCollection;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	CIndexCollection* objIndexCol;
	CNode *objNode;
//TODO: Review
	//CNode objNode;
	//objNode = pobjNodeCollection->getMNNode();
	//objIndexCol = objNode.getIndexCollection();
	objNode = pobjNodeCollection->getNodePtr(MN, MN_NODEID);
	objIndexCol = objNode->getIndexCollection();
	CIndex* pobjIndex = NULL;
	pobjIndex = objIndexCol->getIndexbyIndexValue(pbIndex);
	return pobjIndex;
}

/*****************************************************************************************/
/**
 \brief		getMNSubIndexValues

 This function returns the CSubIndex class pointer for the specific index & SubIndex

 \param		pbIndex			Character pointer to hold the Index id value
 \param		pbSubIndex		Character pointer to hold the SubIndex id value

 \return	CSubIndex* / NULL
 */
/******************************************************************************************/

CSubIndex* getMNSubIndexValues(char* pbIndex, char* pbSubIndex)
{
	CNodeCollection* pobjNodeCollection;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	CIndexCollection* objIndexCol;
	CNode objNode;

	objNode = pobjNodeCollection->getMNNode();
	objIndexCol = objNode.getIndexCollection();
	CIndex* pobjIndex = NULL;
	pobjIndex = objIndexCol->getIndexbyIndexValue(pbIndex);
	if (NULL == pobjIndex)
		return NULL;

	CSubIndex* pobjSubIndex = NULL;
	pobjSubIndex = pobjIndex->getSubIndexbyIndexValue(pbSubIndex);
	return pobjSubIndex;
}

/*****************************************************************************************/
/**
 \brief		WriteXAPElements

 This function writes the xap configurations

 \param		aobjPICol		struct array of type ProcessImage to hold the collection
 \param		pxtwWriter		xml pointer of type xmlTextWriterPtr
 \param		iVarCount		Integer variable to hold processimage variable count
 \param		piType			Enumeration flag of type EPIDirectionType(input/output)

 \return	void
 */
/******************************************************************************************/

void WriteXAPElements(ProcessImage aobjPICol[], xmlTextWriterPtr& pxtwWriter,
		INT32 iVarCount, EPIDirectionType piType)
{

	if (iVarCount != 0)
	{
		INT32 iHighBitOffset = 0;
		INT32 iHighBitOffsetDatasize = 0;
		INT32 iBytesWritten;
		/* Start an element named "Channel". Since thist is the first
		 * element, this will be the root element of the document. */
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
				BAD_CAST "ProcessImage");
		if (iBytesWritten < 0)
		{
			return;
		}
		iBytesWritten = -1;
		if (piType == INPUT)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "type", BAD_CAST "output");
		}
		else if (piType == OUTPUT)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "type", BAD_CAST "input");
		}
		else
		{
			//TODO: "else" Added.Operation need to be specified
		}

		if (iBytesWritten < 0)
		{
			return;
		}

		//find the size and write it to attribute
		for (INT32 iLoopCount = 0; iLoopCount < iVarCount; iLoopCount++)
		{
			ProcessImage objProcessImage;
			objProcessImage.Initialize();
			objProcessImage = aobjPICol[iLoopCount];

			if (iHighBitOffset
					<= ((objProcessImage.ByteOffset * 8)
							+ objProcessImage.BitOffset))
			{
				//save the higher offset of process image and its datasize
				iHighBitOffset = (objProcessImage.ByteOffset * 8)
						+ objProcessImage.BitOffset; //multiply by 8 for byte to bit conversion
				iHighBitOffsetDatasize = objProcessImage.DataInfo.DataSize;
			}
		}
		char *pbPISize = new char[20];
		pbPISize = _IntToAscii((iHighBitOffset + iHighBitOffsetDatasize) / 8,
				pbPISize, 10); //divide by 8 for bit to byte conversion
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "size",
				BAD_CAST pbPISize);
		delete[] pbPISize;
		if (iBytesWritten < 0)
		{
			return;
		}

		for (INT32 iLoopCount = 0; iLoopCount < iVarCount; iLoopCount++)
		{
			ProcessImage objProcessImage;
			objProcessImage.Initialize();
			objProcessImage = aobjPICol[iLoopCount];
			/* Start an element named "Channel". Since thist is the first
			 * element, this will be the root element of the document. */
			iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
					BAD_CAST "Channel");
			if (iBytesWritten < 0)
			{
				return;
			}

			/* Add an attribute with name "Name" and value  to channel. */
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "Name", BAD_CAST objProcessImage.Name);

			if (iBytesWritten < 0)
			{
				return;
			}
			/* Add an attribute with name "DataType" and value to channel */
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "dataType",
					BAD_CAST objProcessImage.DataInfo._dt_Name);
			if (iBytesWritten < 0)
			{
				return;
			}

			///* Add an attribute with name "dataSize" and value to channel */
			char * pbDataSize = new char[20];
			pbDataSize = _IntToAscii(objProcessImage.DataInfo.DataSize,
					pbDataSize, 10);

			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "dataSize", BAD_CAST pbDataSize);
			delete[] pbDataSize;
			if (iBytesWritten < 0)
			{
				return;
			}

			///* Add an attribute with name "dataSize" and value to channel */
			char* pbByteOffset = new char[6];
			pbByteOffset = _IntToAscii(objProcessImage.ByteOffset, pbByteOffset,
					16);
			pbByteOffset = ConvertToHexformat(pbByteOffset, 4, 1);

			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "PIOffset", BAD_CAST pbByteOffset);
			delete[] pbByteOffset;
			if (iBytesWritten < 0)
			{
				return;
			}

			if (objProcessImage.BitOffset != -1)
			{
				char* pbBitOffset = new char[4];
				pbBitOffset = _IntToAscii(objProcessImage.BitOffset,
						pbBitOffset, 16);
				pbBitOffset = ConvertToHexformat(pbBitOffset, 2, 1);

				iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
						BAD_CAST "BitOffset", BAD_CAST pbBitOffset);
				delete[] pbBitOffset;
			}
			if (iBytesWritten < 0)
			{
				return;
			}
			///* Close the element named Channel. */
			iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
			if (iBytesWritten < 0)
			{
				return;
			}
		}
		///* Close the element named ProcessImage. */
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			return;
		}

	}
}

/*****************************************************************************************/
/**
 \brief		StartXAPxml

 This function writes the xml tag & autogeneraged context for the xap.h
 
 \param		pxtwWriter		xml pointer of type xmlTextWriterPtr
 \param		pxdDoc			xml pointer of type xmlDocPtr

 \return	void
 */
/******************************************************************************************/

void StartXAPxml(xmlTextWriterPtr& pxtwWriter, xmlDocPtr& pxdDoc)
{
	INT32 iBytesWritten;

	/* Create a new XmlWriter for DOM, with no compression. */
	pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
	if (pxtwWriter == NULL)
	{
		return;
	}

	/* Start the document with the xml default for the version,
	 * encoding UTF-8 and the default for the standalone
	 * declaration. */
	iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING,
			NULL);
	if (iBytesWritten < 0)
	{
		return;
	}

	/* Write a comment as child of ORDER */
	char* pcComment = new char[strlen(BUILD_COMMENT) + BUILDTIME_BUF_LEN];
	strcpy(pcComment, BUILD_COMMENT);
	strcat(pcComment, GetBuildTime());
	iBytesWritten = xmlTextWriterWriteComment(pxtwWriter, BAD_CAST pcComment);
	delete[] pcComment;
	if (iBytesWritten < 0)
	{
		return;
	}

	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "ApplicationProcess");
	if (iBytesWritten < 0)
	{
		return;
	}
}

/*****************************************************************************************/
/**
 \brief		EndWrtitingXAP

 This function completes the xap configuration & saves the file
 
 \param		pxtwWriter		xml pointer of type xmlTextWriterPtr
 \param		pbFileName		Char pointer to hold the file name
 \param		pxdDoc			xml pointer of type xmlDocPtr

 \return	void
 */
/******************************************************************************************/

void EndWrtitingXAP(xmlTextWriterPtr& pxtwWriter, char* pbFileName,
		xmlDocPtr& pxdDoc)
{
	INT32 iBytesWritten;

	// Close the element named ApplicationProcess.
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		return;
	}
	iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
	if (iBytesWritten < 0)
	{
		return;
	}

	xmlFreeTextWriter(pxtwWriter);

	xmlSaveFormatFile((const char*) pbFileName, pxdDoc, 1);

	xmlFreeDoc(pxdDoc);
}

/*****************************************************************************************/
/**
 \brief		GenerateXAP

 This API writes the process image variables to the XAP files [.h and .xml] in the location specified

 Note: This API is recommended to be called after the GenerateCDC API.

 \param		pbFileName		Char pointer to hold the path of the location to which the generated xap contents will be written into

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GenerateXAP(char* pbFileName)
{
	CNodeCollection* pobjNodeCollection = NULL;
	ocfmException objException;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

	char* xapFileName = new char[strlen(pbFileName) + 4 + ALLOC_BUFFER];
	ProcessImage* aobjPiInCol = NULL;
	ProcessImage* aobjPiOutCol = NULL;
	try
	{
		if (pobjNodeCollection->getNumberOfNodes() == 0)
		{
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		if (pobjNodeCollection->getCNNodesCount() == 0)
		{
			objException.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
			throw objException;
		}

		//CNode objNode;
		xmlTextWriterPtr pxtwWriter = NULL;
		xmlDocPtr pxdDoc = NULL;

		//ProcessImage aobjPiInCol[PI_VAR_COUNT] = {};
		//ProcessImage aobjPiOutCol[PI_VAR_COUNT] = {};

		aobjPiInCol = new ProcessImage[PI_VAR_COUNT];
		aobjPiOutCol = new ProcessImage[PI_VAR_COUNT];
		GroupInOutPIVariables(aobjPiInCol, aobjPiOutCol);

		StartXAPxml(pxtwWriter, pxdDoc);
		if (iInVars != 0)
			WriteXAPElements(aobjPiInCol, pxtwWriter, iInVars, INPUT);

		if (iOutVars != 0)
			WriteXAPElements(aobjPiOutCol, pxtwWriter, iOutVars, OUTPUT);

		strcpy(xapFileName, pbFileName);
		strcat(xapFileName, ".xml");
		EndWrtitingXAP(pxtwWriter, xapFileName, pxdDoc);

		/*Generate Header file */
		GenerateXAPHeaderFile(pbFileName, aobjPiInCol, aobjPiOutCol, iInVars,
				iOutVars);
		delete[] aobjPiInCol;
		delete[] aobjPiOutCol;
	} catch (ocfmException& ex)
	{
		delete[] xapFileName;
		return ex._ocfmRetCode;
	}
	delete[] xapFileName;
	return objException._ocfmRetCode;
}

/*****************************************************************************************/
/**
 \brief		GenerateNET

 This API shall be used to generate the process image variables in a C# .NET usable format.

 Note: This API is recommended to be called after the GenerateCDC API.

 \param		pbFileName		Char pointer to hold the path of the location to which the generated NET contents will be written into

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GenerateNET(char* pbFileName)
{
	CNodeCollection* pobjNodeCollection;
	ocfmException objException;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	ProcessImage* aobjPiInCol = NULL;
	ProcessImage* aobjPiOutCol = NULL;
	try
	{
		if (pobjNodeCollection->getNumberOfNodes() == 0)
		{
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		if (pobjNodeCollection->getCNNodesCount() == 0)
		{
			objException.ocfm_Excpetion(OCFM_ERR_NO_CN_NODES_FOUND);
			throw objException;
		}

		//CNode objNode;
		//                ProcessImage aobjPiInCol[PI_VAR_COUNT] = {};
		//                ProcessImage aobjPiOutCol[PI_VAR_COUNT] = {};
		aobjPiInCol = new ProcessImage[PI_VAR_COUNT];
		aobjPiOutCol = new ProcessImage[PI_VAR_COUNT];
		GroupInOutPIVariables(aobjPiInCol, aobjPiOutCol);
		/*Generate Dot NET Header file */
		GenerateNETHeaderFile(pbFileName, aobjPiInCol, aobjPiOutCol, iInVars,
				iOutVars);
		delete[] aobjPiInCol;
		delete[] aobjPiOutCol;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return objException._ocfmRetCode;
}

/*****************************************************************************************/
/**
 \brief		GetIndexAttributes

 This API shall be used to get the attributes of an Index. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', this API fills the 'pbOutAttributeValue' variable with the value for the specified 'enumAttributeType'. Refer the table for the  'enumAttributeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		enumNodeType			Enum to hold the Node type of the node
 \param		pbIndexID				Character pointer to hold the IndexID
 \param		enumAttributeType		Enum to hold the Attribute type of the index 
 \param		pbOutAttributeValue		Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GetIndexAttributes(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, EAttributeType enumAttributeType,
		char* pbOutAttributeValue)
{
	INT32 iIndexPos;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	try
	{
		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID,
				&iIndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}
		CNode objNode;
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex objIndex;
		CIndex* pobjIndexPtr;

		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);

		switch (enumAttributeType)
		{
		case NAME:
			if (pobjIndexPtr->getName() != NULL)
				strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getName());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjIndexPtr->getObjectType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getObjectType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjIndexPtr->getDataType();
			if (objTmpDataType.Name != NULL)
				strcpy(pbOutAttributeValue, objTmpDataType.Name);
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (pobjIndexPtr->getAccessType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getAccessType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjIndexPtr->getDefaultValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getDefaultValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjIndexPtr->getActualValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getActualValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjIndexPtr->getPDOMapping() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getPDOMapping());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjIndexPtr->getLowLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getLowLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjIndexPtr->getHighLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getHighLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			/* Flag if it should be included in cdc*/
			if (pobjIndexPtr->getFlagIfIncludedCdc() == TRUE)
				strcpy(pbOutAttributeValue, "1");
			else
				strcpy(pbOutAttributeValue, "0");
			break;
		default:
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			throw objException;
		}
		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************************/
/**
 \brief		GetIndexAttributesbyPositions

 This API shall be used to get the attributes of an Index given the position of the Node, position of the Index and the attribute type. Refer the EAttributeType table for the attributes. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the respective attribute's value is filled in the 'piOutAttributeValue'.

 \param		iNodePos				Integer variable to hold the position of the node
 \param		iIndexPos				Integer variable to hold the position of the Index
 \param		enumAttributeType		Enum to hold the Attribute type of the index .
 \param		pbOutAttributeValue		Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/******************************************************************************************/

ocfmRetCode GetIndexAttributesbyPositions(INT32 iNodePos, INT32 iIndexPos,
		EAttributeType enumAttributeType, char* pbOutAttributeValue)
{
	ocfmRetCode stErrorInfo;
	try
	{
		// Check for the Existance of the Node in the iNodePos
		CNode objNode;
		CNodeCollection *pobjNodeCollection;
		CIndexCollection *pobjIndexCollection;
		CIndex* pobjIndexPtr;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();
		if (iNodePos >= iTempNodeCount)
		{
			ocfmException objException;                //new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else if (iTempNodeCount == 0)
		{
			ocfmException objException;                // = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);

		switch (enumAttributeType)
		{
		case NAME:
			if (pobjIndexPtr->getName() != NULL)
				strcpy(pbOutAttributeValue, (char *) pobjIndexPtr->getName());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjIndexPtr->getObjectType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getObjectType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjIndexPtr->getDataType();
			if (objTmpDataType.Name != NULL)
			{
				strcpy(pbOutAttributeValue, objTmpDataType.Name);
			}
			else
			{
				strcpy(pbOutAttributeValue, "");
			}
			break;
		case ACCESSTYPE:
			if (pobjIndexPtr->getAccessType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getAccessType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjIndexPtr->getDefaultValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getDefaultValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjIndexPtr->getActualValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getActualValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjIndexPtr->getPDOMapping() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getPDOMapping());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjIndexPtr->getLowLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getLowLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjIndexPtr->getHighLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjIndexPtr->getHighLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			/* Flag if it should be included in cdc*/
			if (pobjIndexPtr->getFlagIfIncludedCdc() == TRUE)
				strcpy(pbOutAttributeValue, "1");
			else
				strcpy(pbOutAttributeValue, "0");
			break;
		default:
			stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
			return stErrorInfo;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexAttributes

 This API shall be used to get the attributes of a SubIndex. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', this API fills the 'pbOutAttributeValue' variable with the value for the specified 'enumAttributeType'. Refer the EattributeType table for the  'enumAttributeType'.

 \param		iNodeID					Integer variable to hold the Node Id of a node
 \param		enumNodeType			Enum to hold the Node type of the node
 \param		pbIndexID				Character pointer to hold the IndexID
 \param		pbSubIndexID			Character pointer to hold the SubIndexID
 \param		enumAttributeType		Enum to hold the Attribute type of the index .
 \param		pbOutAttributeValue		Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexAttributes(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, char* pbSubIndexID, EAttributeType enumAttributeType,
		char* pbOutAttributeValue)
{
	CNode objNode;
	CNodeCollection *pobjNodeCollection = NULL;
	CIndexCollection *pobjIndexCollection = NULL;
	CIndex objIndex;
	CIndex* pobjSubIndex = NULL;
	INT32 iSubIndexPos = -1;
	INT32 iIndexPos = -1;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{

		stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, pbIndexID,
				pbSubIndexID, &iSubIndexPos, &iIndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjSubIndex = pobjIndexCollection->getIndex(iIndexPos);

		CSubIndex* pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);

		switch (enumAttributeType)
		{
		case NAME:
			if (pobjSubIndexPtr->getName() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getName());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjSubIndexPtr->getObjectType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getObjectType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjSubIndexPtr->getDataType();
			if (objTmpDataType.Name != NULL)
				strcpy(pbOutAttributeValue, objTmpDataType.Name);
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (pobjSubIndexPtr->getAccessType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getAccessType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjSubIndexPtr->getDefaultValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getDefaultValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjSubIndexPtr->getActualValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getActualValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjSubIndexPtr->getPDOMapping() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getPDOMapping());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjSubIndexPtr->getLowLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getLowLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjSubIndexPtr->getHighLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getHighLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			if (pobjSubIndexPtr->getFlagIfIncludedCdc() == TRUE)
				strcpy(pbOutAttributeValue, "1");
			else
				strcpy(pbOutAttributeValue, "0");
			break;
		default:
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXID);
			throw objException;
		}

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexAttributesbyPositions

 This API shall be used to get the attributes of a SubIndex given the position of the Node, position of the Index, position of the SubIndex and the attribute type. Refer the EAttributeType table for the attributes. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the respective attribute's value is filled in the 'piOutAttributeValue'.

 \param		iNodePos				Integer variable to hold the position of the node
 \param		iIndexPos				Integer variable to hold the position of the Index
 \param		iSubIndexPos			Integer variable to hold the position of the SubIndex
 \param		enumAttributeType		Enum to hold the Attribute type of the index
 \param		pbOutAttributeValue		Character pointer to hold the AttributeValue

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexAttributesbyPositions(INT32 iNodePos, INT32 iIndexPos,
		INT32 iSubIndexPos, EAttributeType enumAttributeType,
		char* pbOutAttributeValue)
{
	ocfmRetCode stErrorInfo;
	try
	{
		// Check for the Existance of the Node in the iNodePos
		CNode objNode;
		CNodeCollection *pobjNodeCollection = NULL;
		CIndexCollection *pobjIndexCollection = NULL;
		CIndex* pobjIndexPtr = NULL;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();
		if (iNodePos >= iTempNodeCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iIndexPos >= iTempIndexCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndexPtr = pobjIndexCollection->getIndex(iIndexPos);

		INT32 iTempSubIndexCount = pobjIndexPtr->getNumberofSubIndexes();

		if (iSubIndexPos >= iTempSubIndexCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;
		}
		else if (iTempSubIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		CSubIndex* pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjIndexPtr->getSubIndex(iSubIndexPos);

		switch (enumAttributeType)
		{
		case NAME:
			if (pobjSubIndexPtr->getName() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getName());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case OBJECTTYPE:
			if (pobjSubIndexPtr->getObjectType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getObjectType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DATATYPE:
			DataType objTmpDataType;
			objTmpDataType = pobjSubIndexPtr->getDataType();
			if (objTmpDataType.Name != NULL)
				strcpy(pbOutAttributeValue, objTmpDataType.Name);
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case ACCESSTYPE:
			if (pobjSubIndexPtr->getAccessType() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getAccessType());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case DEFAULTVALUE:
			if (pobjSubIndexPtr->getDefaultValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getDefaultValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case ACTUALVALUE:
			if (pobjSubIndexPtr->getActualValue() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getActualValue());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case PDOMAPPING:
			if (pobjSubIndexPtr->getPDOMapping() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getPDOMapping());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case LOWLIMIT:
			if (pobjSubIndexPtr->getLowLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getLowLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case HIGHLIMIT:
			if (pobjSubIndexPtr->getHighLimit() != NULL)
				strcpy(pbOutAttributeValue,
						(char *) pobjSubIndexPtr->getHighLimit());
			else
				strcpy(pbOutAttributeValue, "");
			break;
		case FLAGIFINCDC:
			if (pobjSubIndexPtr->getFlagIfIncludedCdc() == TRUE)
				strcpy(pbOutAttributeValue, "1");
			else
				strcpy(pbOutAttributeValue, "0");
			break;

		default:
			stErrorInfo.code = OCFM_ERR_INVALID_ATTRIBUTETYPE;
			return stErrorInfo;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;

}

/*****************************************************************************/
/**
 \brief		GetNodeCount
 
 This API shall be used to get the number of controlled nodes under a MN. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', 'piOutNodeCount' is filled with the Node count value.
 
 \param		iMnNodeId				Integer variable to hold the node id
 \param		piOutNodeCount		Integer Pointer to NodeCount

 \return	ocfmRetCode
 */
/*****************************************************************************/

//TODO: iMnNodeId not used. to be removed in header also
ocfmRetCode GetNodeCount(INT32 iMnNodeId, INT32* piOutNodeCount)
{

	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	//CNode objNode;
	CNodeCollection *pobjNodeCollection = NULL;
	ocfmException objException;

	try
	{
		if (NULL == piOutNodeCount)
		{
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}
		else
		{
			//TODO: 'else' added. Nothing to do.
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (pobjNodeCollection->getNumberOfNodes() < 0)
		{
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added.  operation to be added
		}

		*piOutNodeCount = pobjNodeCollection->getNumberOfNodes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		getPIName
 
 This function returns the string with cn node id
 
 \param		iNodeID		Integer variable to hold the Node Id of a node

 \return	char*
 */
/*****************************************************************************/
//TODO: unused function
//TODO: change the returning char* to delete pbNodeIdStr
char* getPIName(INT32 iNodeID)
{
	char* pbNodeIdStr = NULL;
	char* pbIdAsci = new char[2];

	pbNodeIdStr = new char[5];
	strcpy(pbNodeIdStr, "CN");
	pbIdAsci = _IntToAscii(iNodeID, pbIdAsci, 10);
	strcat(pbNodeIdStr, pbIdAsci);
	strcat(pbNodeIdStr, ".");
	strcat(pbNodeIdStr, "\0");

	delete[] pbIdAsci;
	return pbNodeIdStr;
}

/*****************************************************************************/
/**
 \brief		getParameterAccess
 
 This function gets the parameter access code by providing the access type
 
 \param		pbAccess	Character pointer to hold the access type

 \return	char*
 */
/*****************************************************************************/

char* getParameterAccess(char* pbAccess)
{
	if (NULL == pbAccess)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	if (!strcmp(ConvertToUpper(pbAccess), "READ"))
		return (char*) "ro\0";
	else if (!strcmp(ConvertToUpper(pbAccess), "READWRITE"))
		return (char*) "rw\0";
	else
		return (char*) "xx";

}

/*****************************************************************************/
/**
 \brief		GetIndexCount

 This API shall be used to get the number of Indexes under a node. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', 'piOutIndexCount' is filled with the Index count.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		enumNodeType		Enum to hold the Node type of the node
 \param		piOutIndexCount		Integer Pointer to hold the  IndexCount

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetIndexCount(INT32 iNodeID, ENodeType enumNodeType,
		INT32* piOutIndexCount)
{
	if (NULL == piOutIndexCount)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	try
	{
		CNode objNode;
		CNodeCollection *pobjNodeCollection = NULL;
		CIndexCollection *pobjIndexCollection = NULL;
		//CIndex objIndex;
		bool bFlag = false;

		INT32 iNodePos;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);
		pobjIndexCollection = objNode.getIndexCollection();

		*piOutIndexCount = pobjIndexCollection->getNumberofIndexes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexCount

 This API shall be used to get the number of SubIndexes under an Index. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', 'piOutSubIndexCount' is filled with the SubIndex count.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		enumNodeType		Enum to hold the Node type of the node
 \param		pbIndexID			Character pointer to hold the IndexID
 \param		piOutSubIndexCount	Integer Pointer to hold the IndexCount

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexCount(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, INT32* piOutSubIndexCount)
{
	if ((NULL == pbIndexID) || (NULL == piOutSubIndexCount))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	CNode objNode;
	CNodeCollection *pobjNodeCollection = NULL;
	CIndexCollection *pobjIndexCollection = NULL;
	CIndex objIndex;
	CIndex* pobjSubIndex = NULL;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos;

	try
	{
		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID,
				&iIndexPos);
		if (stErrorInfo.code == 0)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		objIndex.setNodeID(objNode.getNodeId());
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjSubIndex = pobjIndexCollection->getIndex(iIndexPos);

		*piOutSubIndexCount = pobjSubIndex->getNumberofSubIndexes();

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		LoadObjectDictionary

 This API shall be used to load the predefined object dictionary xml. This API loads the objects that the tool will refer to when adding/editing an Index and/or a SubIndex.

 \param		pbFileName		Char pointer  to hold the path of the file [XDD or XDC]

 \return	void
 */
/*****************************************************************************/

void LoadObjectDictionary(char* pbFileName)
{

	xmlTextReaderPtr pxReader;

	CObjectDictionary* objDict;
	objDict = CObjectDictionary::getObjDictPtr();
	/*
	 xmlTextReaderPtr xmlReaderForFile (const char * filename, const char * encoding, int options)
	 Returns: the new reader or NULL in case of error.
	 */

	pxReader = xmlReaderForFile(pbFileName, NULL, 0);
	if (pxReader != NULL)
	{
		INT32 iRetVal = 0;
		/*
		 int xmlTextReaderRead(xmlTextReaderPtr reader)
		 Return:   1 if the node was read successfully, 0 if there is no more nodes to read, or -1 in case of error
		 */
		iRetVal = xmlTextReaderRead(pxReader);
		while (iRetVal == 1)
		{
			//const xmlChar* pxcName	= NULL;
			//const xmlChar* pxcValue	= NULL;

			//pxcName = xmlTextReaderConstName(pxReader);

			//pxcValue = xmlTextReaderConstValue(pxReader);
			objDict->ProcessObjectDictionary(pxReader);
			iRetVal = xmlTextReaderRead(pxReader);
		}
		if (iRetVal != 0)
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

/*****************************************************************************/
/**
 \brief		GetNodeAttributesbyNodePos

 This API shall be used to get the node attributes [NodeID, NodeName, Stationtype, Forcedcycle_value, Forcedcycle_flag] of a node given the position of the node under a MN. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the attributes will be filled with the corresponding values in the parameter variables.


 \param		iNodePos			Integer variable to hold the position of the node
 \param		piOutNodeID			Integer Pointer to hold the NodeID
 \param		piOutNodeName		Character pointer to hold the NodeName
 \param		eOutStationType		Character pointer to hold the station type
 \param		pbOutForcedCycle	Character pointer to hold the forced cycle value
 \param		bForcedCycleFlag	boolean pointer to hold the forced cycle flag

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetNodeAttributesbyNodePos(INT32 iNodePos, INT32* piOutNodeID,
		char* piOutNodeName, EStationType* eOutStationType,
		char* pbOutForcedCycle, bool* bForcedCycleFlag)
{
	if ((NULL == piOutNodeID) || (NULL == piOutNodeName)
			|| (NULL == eOutStationType) || (NULL == pbOutForcedCycle)
			|| (NULL == bForcedCycleFlag))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iTempNodeCount;
	try
	{
		// We do not have support for multiple MNs in this version.
		stErrorInfo = GetNodeCount(MN_NODEID, &iTempNodeCount);

		if (iNodePos > iTempNodeCount)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		CNode objNode;
		CNodeCollection *pobjNodeCollection;
		//CIndexCollection *pobjIndexCollection;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

		//pobjIndexCollection = objNode.getIndexCollection();

		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		*piOutNodeID = objNode.getNodeId();
		if (objNode.getNodeName() != NULL)
			strcpy(piOutNodeName, objNode.getNodeName());
		else
			piOutNodeName = NULL;

		if (objNode.getForcedCycle() != NULL)
			strcpy(pbOutForcedCycle, objNode.getForcedCycle());
		else
			pbOutForcedCycle = NULL;

		*eOutStationType = objNode.getStationType();
		*bForcedCycleFlag = objNode.getForceCycleFlag();

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetIndexIDbyIndexPos

 This API shall be used to get the Index ID  of an Index, given the position of the Index under a node and the Node ID. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the Index ID is filled into the 'piOutIndexID'.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node
 \param		iIndexPos		Integer variable to hold the index position
 \param		pbOutIndexID	Character pointer to hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetIndexIDbyIndexPos(INT32 iNodeID, ENodeType enumNodeType,
		INT32 iIndexPos, char* pbOutIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	CNode objNode;
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	CIndex* pobjIndex;

	INT32 iNodePos;
	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{
			//continue with process
		}
		else
		{
			// Node Doesn't Exist
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();

		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);

		if (pobjIndex->getIndexValue() != NULL)
			strcpy(pbOutIndexID, (char *) pobjIndex->getIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetIndexIDbyPositions

 This API shall be used to get the Index ID  of an Index given the position of the  Index under a node and the position of the Node. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the Index ID is filled into the 'piOutIndexID'.

 \param		iNodePos		Integer variable to hold the position of the node
 \param		iIndexPos		Integer variable to hold the position of the Index
 \param		pbOutIndexID	Character Pointer to  hold the IndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetIndexIDbyPositions(INT32 iNodePos, INT32 iIndexPos,
		char* pbOutIndexID)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	CNode objNode;
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	CIndex* pobjIndex;

	try
	{
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();

		if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if (iTempNodeCount < iNodePos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();

		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);

		if (pobjIndex->getIndexValue() != NULL)
			strcpy(pbOutIndexID, (char *) pobjIndex->getIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexIDbySubIndexPos

 This API shall be used to get the SubIndex ID of a SubIndex given the position of the  SubIndex under an Index, the Index ID and the Node ID. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the SubIndex ID is filled into the 'piOutSubIndexID'.

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		enumNodeType		Enum to hold the Node type of the node
 \param		pbIndexID			Character pointer to hold the IndexID
 \param		iSubIndexPos		Integer variable to hold the position of SubIndex
 \param		pbOutSubIndexID		Character Pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 iNodeID, ENodeType enumNodeType,
		char* pbIndexID, INT32 iSubIndexPos, char* pbOutSubIndexID)
{
	CNode objNode;
	CNodeCollection *pobjNodeCollection;
	CIndexCollection *pobjIndexCollection;
	//CIndex objIndex;
	CIndex* pobjSubIndex;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iIndexPos;
	try
	{

		stErrorInfo = IfIndexExists(iNodeID, enumNodeType, pbIndexID,
				&iIndexPos);
		if (stErrorInfo.code == 0)
		{
			//continue with process
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INDEXID_NOT_FOUND);
			throw objException;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID);

		pobjIndexCollection = objNode.getIndexCollection();
		pobjSubIndex = pobjIndexCollection->getIndex(iIndexPos);

		INT32 iTempSubIndexCount = pobjSubIndex->getNumberofSubIndexes();
		if (iTempSubIndexCount == 0)
		{
			ocfmException objException;         // = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;
		}
		else if (iTempSubIndexCount < iSubIndexPos)
		{
			ocfmException objException;         // = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		CSubIndex *pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjSubIndex->getSubIndex(iSubIndexPos);

		if (pobjSubIndexPtr->getIndexValue() != NULL)
			strcpy(pbOutSubIndexID, (char *) pobjSubIndexPtr->getIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetSubIndexIDbyPositions

 This API shall be used to get the SubIndex ID of a SubIndex given the position of the  SubIndex under an Index, the position of the Index under the node and the position of the node under the MN. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the SubIndex ID is filled into the 'piOutSubIndexID'.

 \param		iNodePos			Integer variable to hold the position of the node
 \param		iIndexPos			Integer variable to hold the position of the Index
 \param		iSubIndexPos		Integer variable to hold the SubIndex position
 \param		pbOutSubIndexID		Character pointer to hold the SubIndexID

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetSubIndexIDbyPositions(INT32 iNodePos, INT32 iIndexPos,
		INT32 iSubIndexPos, char* pbOutSubIndexID)

{
	CNode objNode;
	CNodeCollection *pobjNodeCollection = NULL;
	CIndexCollection *pobjIndexCollection = NULL;
	//CIndex objIndex;
	CIndex* pobjIndex = NULL;
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	try
	{
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

		INT32 iTempNodeCount = pobjNodeCollection->getNumberOfNodes();

		if (iTempNodeCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		else if (iTempNodeCount < iNodePos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		objNode = pobjNodeCollection->getNodebyCollectionIndex(iNodePos);
		pobjIndexCollection = objNode.getIndexCollection();

		INT32 iTempIndexCount = pobjIndexCollection->getNumberofIndexes();
		if (iTempIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}
		else if (iTempIndexCount < iIndexPos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_INDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		pobjIndex = pobjIndexCollection->getIndex(iIndexPos);

		INT32 iTempSubIndexCount = pobjIndex->getNumberofSubIndexes();
		if (iTempSubIndexCount == 0)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_SUBINDEXS_FOUND);
			throw objException;
		}
		else if (iTempSubIndexCount < iSubIndexPos)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_SUBINDEXPOS);
			throw objException;
		}
		else
		{
			//TODO: 'else' added. operation to be specified
		}

		CSubIndex* pobjSubIndexPtr = NULL;
		pobjSubIndexPtr = pobjIndex->getSubIndex(iSubIndexPos);

		if (pobjSubIndexPtr->getIndexValue() != NULL)
			strcpy(pbOutSubIndexID, (char *) pobjSubIndexPtr->getIndexValue());

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		DeleteNodeObjDict

 This API shall be used to delete the object dictionary of a node given its respective node ID and node type. This API will not delete the node.

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode DeleteNodeObjDict(INT32 iNodeID, ENodeType enumNodeType)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	INT32 iNodePos;

	try
	{
		ocfmException ex;
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, enumNodeType, &iNodePos, bFlag);

		if (stErrorInfo.code == 0 && bFlag == true)
		{
		}
		else
		{
			ex.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw ex;
		}
		//CNode objNode;
		CNode* pobjNode = NULL;
		CNodeCollection *pobjNodeCollection = NULL;
		CIndexCollection *pobjIndexCollection = NULL;
		CDataTypeCollection *pobjDataTypeCollection = NULL;
		CIndex objIndex;
		//CSubIndex pobjSubIndex;
		CNetworkManagement *pobjNetworkManagement = NULL;
		CApplicationProcess *pobjApplicationProcess = NULL;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (NULL == pobjNodeCollection)
		{
#if defined DEBUG
			cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

			ex.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw ex;
		}
		pobjNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeID);
		//objNode = pobjNodeCollection->getNode(enumNodeType, iNodeID); // (enumNodeType, iNodeID);
		objIndex.setNodeID(pobjNode->getNodeId());
		//objSubIndex.setNodeID(objNode->getNodeId());
		pobjDataTypeCollection = pobjNode->getDataTypeCollection();

		pobjIndexCollection = pobjNode->getIndexCollection();
		pobjNetworkManagement = pobjNode->getNetworkManagement();
		pobjApplicationProcess = pobjNode->getApplicationProcess();
		if ((NULL == pobjApplicationProcess) || (NULL == pobjIndexCollection)
				|| (NULL == pobjNetworkManagement)
				|| (NULL == pobjDataTypeCollection))
		{
#if defined DEBUG
			cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

			ex.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
			throw ex;
		}
		// Delete IndexCollection

		CIndex* pobjIndexPtr = NULL;
		INT32 iNofIndexes = pobjIndexCollection->getNumberofIndexes();

		for (INT32 i = iNofIndexes; i < 0; i--)
		{
			pobjIndexPtr = pobjIndexCollection->getIndex(i);
			if (pobjIndexPtr->getNumberofSubIndexes() > 0)
			{
				pobjIndexPtr->deleteSubIndexCollection();
			}
		}

		pobjIndexCollection->DeleteIndexCollection();
		// Delete DataTypeCollection
		pobjDataTypeCollection->DeleteDataTypeCollection();
		//Delete Network management collectionObj
		pobjNetworkManagement->DeleteFeatureCollections();
		//Delete ComplexDataTypeCollection
		pobjApplicationProcess->DeleteParameterCollection();
		pobjApplicationProcess->DeleteComplexDataTypeCollection();
		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* exCatch)
	{
		return exCatch->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		SaveProject

 This API shall be used to save the project. This API calls the SaveNode API recursively and saves all the nodes under the project and also saves a project file 'ProjectName.oct' in the project location.

 \param		pbProjectPath		Character pointer to hold the Project Path
 \param		pbProjectName		Character pointer to hold the Project name

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SaveProject(char* pbProjectPath, char* pbProjectName)
{
	CNode objNode;
	CNodeCollection *pobjNodeCollection = NULL;
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	char* pbTempPath;
	pbTempPath = new char[(strlen(pbProjectPath)) + (strlen(pbProjectName))
			+ strlen("cdc_xap") + ALLOC_BUFFER];

	try
	{
		char* pbTempPjtName;
		struct stat stFileInfo;
		INT32 iIntStat;

		pbTempPjtName = new char[300];
		sprintf(pbTempPjtName, "%s%s/%s.oct", pbProjectPath, pbProjectName,
				pbProjectName);

		iIntStat = stat(pbTempPjtName, &stFileInfo);
		delete[] pbTempPjtName;
		if (iIntStat == 0)
		{
#if defined DEBUG
			cout << "\n\n\nProject File Already exists\n" << endl;
#endif
		}
		else
		{
#if defined(_WIN32) && defined(_MSC_VER)
			{
				sprintf(pbTempPath, "%s\\%s", pbProjectPath, pbProjectName);
				_mkdir(pbTempPath);
			}
#else
			{
				sprintf(pbTempPath, "%s/%s", pbProjectPath, pbProjectName);
				mkdir(pbTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
			}
#endif
		}

#if defined(_WIN32) && defined(_MSC_VER)
		{
			sprintf(pbTempPath, "%s\\%s\\%s", pbProjectPath, pbProjectName,
					"cdc_xap");
			_mkdir(pbTempPath);
		}
#else
		{
			sprintf(pbTempPath, "%s/%s/%s", pbProjectPath, pbProjectName, "cdc_xap");
			mkdir(pbTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
		}
#endif

		saveProjectXML(pbProjectPath, pbProjectName);
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		if (pobjNodeCollection == NULL)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
		if (pobjNodeCollection->getNumberOfNodes() > 0)
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < pobjNodeCollection->getNumberOfNodes();
					iLoopCount++)
			{
				objNode = pobjNodeCollection->getNodebyCollectionIndex(
						iLoopCount);
				char* pbFileName;
				pbFileName = new char[MAX_FILE_PATH_SIZE];
#if defined(_WIN32) && defined(_MSC_VER)
				{
					sprintf(pbTempPath, "%s\\%s\\octx", pbProjectPath,
							pbProjectName);
					struct stat stFileInfo;

					iIntStat = stat(pbTempPath, &stFileInfo);
					if (iIntStat == 0)
					{

#if defined DEBUG
						cout << "Folder Already Exists\n" << endl;
#endif
					}
					else
					{
						_mkdir(pbTempPath);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(pbFileName, "%s\\%d.octx", pbTempPath,
							objNode.getNodeId());
				}
#else
				{
					struct stat stFileInfo;

					sprintf(pbTempPath, "%s/%s/octx", pbProjectPath, pbProjectName);
					iIntStat = stat(pbTempPath, &stFileInfo);
					if(iIntStat == 0)
					{
#if defined DEBUG
						cout << "Folder Already Exists\n" << endl;
#endif
					}
					else
					{
						mkdir(pbTempPath, S_IRWXU | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
					}
					// Saves the nodes with their nodeId as the name
					sprintf(pbFileName, "%s/%d.octx", pbTempPath, objNode.getNodeId());
				}
#endif				
				SaveNode(pbFileName, objNode.getNodeId(),
						objNode.getNodeType());
				delete[] pbFileName;

			}
			stErrorInfo.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NO_NODES_FOUND);
			throw objException;
		}
	} catch (ocfmException* ex)
	{
		delete[] pbTempPath;
		return ex->_ocfmRetCode;
	}
	delete[] pbTempPath;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetMNPDOSubIndex

 This function sets the MN pdo actual value calculated from the MNPdoVariable struct generated by the Processpdonodes amd the iPrevSubIndex value is updated

 \param		stMNPdoVar		Struct variable of type MNPdoVariable
 \param		iPrevSubIndex	Integer variable to update the previous subindex used
 \param		pobjIdx			Class pointer of the class CIndex of the CN's PDO index
 \param		pbMNIndex		Character pointer to hold the MN index value
 \param		iPrevSize		Integer variable to update the previous size used

 \return	void
 */
/*****************************************************************************/

void GetMNPDOSubIndex(MNPdoVariable stMNPdoVar, INT32& iPrevSubIndex,
		CIndex* pobjIdx, char* pbMNIndex, INT32 iPrevSize)
{
	if ((NULL == pobjIdx) || (NULL == pbMNIndex))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	CSubIndex* pobjSubIndex = NULL;
	char* pbIdx = new char[3];
	/* Set the MN's PDO subIndex*/
	iPrevSubIndex = iPrevSubIndex + 1;
	pbIdx = _IntToAscii(iPrevSubIndex, pbIdx, 16);
	pbIdx = padLeft(pbIdx, '0', 2);

#if defined DEBUG
	cout << " idx" << pbIdx << endl;
	cout << " prevsubindex" << iPrevSubIndex << endl;
#endif
	if (CheckIfSubIndexExists(MN_NODEID, MN, pbMNIndex, pbIdx))
	{
		pobjSubIndex = pobjIdx->getSubIndexbyIndexValue(pbIdx);
	}
	else
	{
		AddSubIndex(MN_NODEID, MN, pbMNIndex, pbIdx);
		pobjSubIndex = pobjIdx->getSubIndexbyIndexValue(pbIdx);
	}
	delete[] pbIdx;
	if (NULL == pobjSubIndex)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_UNKNOWN);
		cout << "pobjSubIndex NULL:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	/* Calculate the actual value of MN PDO */
	char* pbActValue = new char[20];
	strcpy(pbActValue, subString(stMNPdoVar.Value, 0, 6));
	char* pbOffset = new char[5];

	pbOffset = _IntToAscii(iPrevSize, pbOffset, 16);
	pbOffset = padLeft(pbOffset, '0', 4);
	strcpy(pbOffset, ConvertToUpper(pbOffset));
	strcat(pbActValue, pbOffset);
	/* Add reserve byte*/
	strcat(pbActValue, "00");

	strcat(pbActValue, stMNPdoVar.SubIndex);
	strcat(pbActValue, stMNPdoVar.Index);

	pobjSubIndex->setActualValue(pbActValue);
	#if defined DEBUG
	cout<<"Actual Value"<<pbActValue<<endl;
	#endif
	AddPDOIndexsToMN(stMNPdoVar.Index, stMNPdoVar.SubIndex, stMNPdoVar.pdoType);
	delete[] pbOffset;
	delete[] pbActValue;
}

/*****************************************************************************/
/**
 \brief		SetSIdxValue

 This function sets the actual value to the sub index in the index specified

 \param		pbIdx				Character pointer to hold the index id
 \param		pbSIdx				Character pointer to hold the Sub index id
 \param		pbvalue				Character pointer to hold the actual value
 \param		pobjIdxCol			Class pointer of the class CIndexCollection
 \param		iNodeId				Integer variable to hold the node id
 \param		enumNodeType		Enum of type ENodeType to hold node type
 \param		setDefaultValue		Boolean flag to set for Default Value

 \return	void
 */
/*****************************************************************************/

void SetSIdxValue(char* pbIdx, char* pbSIdx, char* pbvalue,
		CIndexCollection *pobjIdxCol, INT32 iNodeId, ENodeType enumNodeType,
		bool setDefaultValue)
{
	//TODO: pbValue (3rd parameter) to be null checked. Empty value sent in generateotherMNindexes function
	if ((NULL == pbIdx) || (NULL == pbSIdx) || (NULL == pobjIdxCol))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	CIndex *pobjIndex = NULL;
	CSubIndex* pobjSIdx = NULL;
	ocfmRetCode stRetInfo;

	pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbIdx);
#if defined DEBUG	
	cout << "sidx idx" << pbIdx << pbSIdx << endl;

#endif
	if (CheckIfSubIndexExists(iNodeId, enumNodeType, pbIdx, pbSIdx))
	{
#if defined DEBUG	
		cout << "subindex fetched" << endl;
#endif			
		pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);
		if (setDefaultValue)
			pobjSIdx->setActualValue((char*) pobjSIdx->getDefaultValue());
		else
			pobjSIdx->setActualValue(pbvalue);
		pobjSIdx->setFlagIfIncludedCdc(TRUE);
	}
	else
	{
#if defined DEBUG	
		cout << "call addindex" << endl;
#endif
		stRetInfo = AddSubIndex(iNodeId, enumNodeType, pbIdx, pbSIdx);
#if defined DEBUG	
		cout << "retcode" << stRetInfo.code << endl;
		cout << "Index sidx added" << pbIdx << pbSIdx;
#endif
		pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);
		pobjSIdx->setFlagIfIncludedCdc(TRUE);
		if (setDefaultValue)
			pobjSIdx->setActualValue((char*) pobjSIdx->getDefaultValue());
		else
		{
#if defined DEBUG	
			cout << "value" << pbvalue << endl;
			cout << "subindex index" << pobjSIdx->getIndexValue();
#endif
			pobjSIdx->setActualValue(pbvalue);
		}
	}
}

/*****************************************************************************/
/**
 \brief		AddForEachSIdx

 This function adds the specified attribute for each subindex 

 \param		pbIdx				Character pointer to hold the index id
 \param		pobjIdxCol			Class pointer of the class CIndexCollection
 \param		iMNNodeID			Integer variable to hold the node id of the MN
 \param		pbValue				Character pointer to hold the value to set to the subindex
 \param		bIsDefaultValueSet	Boolean flag to set for Default Value

 \return	void
 */
/*****************************************************************************/

void AddForEachSIdx(char *pbIdx, CIndexCollection *pobjIdxCol, INT32 iMNNodeID,
		char *pbValue, bool bIsDefaultValueSet)
{
	CNodeCollection *objNodeCol = NULL;
	CNode objCNNode;
	//CIndex *pobjIndex;
	char *pbSIdx = new char[3];
	char *pbIndexNo = new char[3];
	char *pbHexIndexNo = new char[5];
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	//pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbIdx);
	objNodeCol = CNodeCollection::getNodeColObjectPointer();

	// to doStack wasnt booting up with sub index 00 for 1C09, 1F26
	if ((strcmp("1C09", pbIdx) == 0) || (strcmp("1F26", pbIdx) == 0)
			|| (strcmp("1F8B", pbIdx) == 0) || (strcmp("1F8D", pbIdx) == 0)
			|| (strcmp("1F27", pbIdx) == 0) || (strcmp("1F84", pbIdx) == 0))
	{
		strcpy(pbSIdx, "00");
		stRetInfo = DeleteSubIndex(iMNNodeID, MN, pbIdx, pbSIdx);
	}
	else
	{
		if (objNodeCol->getCNNodesCount() != 0)
		{
			strcpy(pbSIdx, "00");
			strcpy(pbHexIndexNo, "0x");

			pbIndexNo = _IntToAscii(objNodeCol->getCNNodesCount(), pbIndexNo,
					16);
			pbIndexNo = padLeft(pbIndexNo, '0', 2);
			strcat(pbHexIndexNo, pbIndexNo);
			SetSIdxValue(pbIdx, pbSIdx, pbHexIndexNo, pobjIdxCol, iMNNodeID, MN,
					false);
		}
	}

	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
			iLoopCount++)
	{
		objCNNode = objNodeCol->getNodebyCollectionIndex(iLoopCount);

		if (objCNNode.getNodeType() == CN)
		{
			pbSIdx = _IntToAscii(objCNNode.getNodeId(), pbSIdx, 16);
			pbSIdx = padLeft(pbSIdx, '0', 2);

			if (strcmp("1F84", pbIdx) == 0)
			{
				CIndex* objCNIndex;
				//CSubIndex objCNSIdx;
				CIndexCollection* pobjCNIdxCol;
				char* pbMIndex = new char[INDEX_LEN];
				pobjCNIdxCol = objCNNode.getIndexCollection();

				strcpy(pbMIndex, "1000");
				objCNIndex = pobjCNIdxCol->getIndexbyIndexValue(pbMIndex);

				if (objCNIndex != NULL)
				{
					if (objCNIndex->getActualValue() != NULL)
					{
						SetSIdxValue(pbIdx, pbSIdx,
								(char*) objCNIndex->getActualValue(),
								pobjIdxCol, iMNNodeID, MN, false);
					}
					else
					{
						SetSIdxValue(pbIdx, pbSIdx,
								(char*) objCNIndex->getActualValue(),
								pobjIdxCol, iMNNodeID, MN, true);
					}
				}
				delete[] pbMIndex;

			}
			else
			{
				SetSIdxValue(pbIdx, pbSIdx, pbValue, pobjIdxCol, iMNNodeID, MN,
						bIsDefaultValueSet);
			}

		}

	}
	delete[] pbHexIndexNo;
	delete[] pbIndexNo;
	delete[] pbSIdx;
}

/*****************************************************************************/
/**
 \brief		AddOtherMNIndexes

 This function creates other required MN indexes

 \param		NodeID		Integer variable to hold the node id

 \return	ocfmRetCode
 */
/*****************************************************************************/
//TODO: unused function
ocfmRetCode AddOtherMNIndexes(INT32 NodeID)
{
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	stRetInfo.errorString = NULL;
	char* pbMNIndex = new char[INDEX_LEN];
	char* pbSidx = new char[SUBINDEX_LEN];
	CIndex* pobjIndex;
	CIndexCollection* pobjIdxCol;

	CNode *pobjNode;
	CNodeCollection *objNodeCollection = NULL;

	try
	{
		objNodeCollection = CNodeCollection::getNodeColObjectPointer();
		pobjNode = objNodeCollection->getNodePtr(MN, NodeID);
		pobjIdxCol = pobjNode->getIndexCollection();

		/* Add 1006*/
		strcpy(pbMNIndex, "1006");
#if defined DEBUG	
		cout << "string copied" << endl;
#endif
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
#if defined DEBUG	
		cout << "retcode" << stRetInfo.code << endl;
		cout << "1006 added" << endl;
#endif

		/* Set 5ms value*/
		SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*) "50000",
				(char*) "NMT_CycleLen_U32", TRUE);

		/* Add 1020*/
		strcpy(pbMNIndex, "1020");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);

		/* Add 1020*/
		strcpy(pbMNIndex, "1300");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		/* $:To do by M hard coded*/
		SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*) "5000",
				(char*) "SDO_SequLayerTimeout_U32", TRUE);

		/* Add 1C02*/
		strcpy(pbMNIndex, "1C02");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);

		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);
			/* Set subindex value 40 or 0000028 */
			strcpy(pbSidx, "00");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) "3", pobjIdxCol,
					pobjNode->getNodeId(), MN, false);
#if defined DEBUG	
			cout << "1c02 subidex added" << endl;
#endif

			strcpy(pbSidx, "01");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) "40", pobjIdxCol,
					pobjNode->getNodeId(), MN, false);
#if defined DEBUG	
			cout << "1c02 subidex 01 added" << endl;
#endif
			strcpy(pbSidx, "02");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) "40", pobjIdxCol,
					pobjNode->getNodeId(), MN, false);

			strcpy(pbSidx, "03");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) "40", pobjIdxCol,
					pobjNode->getNodeId(), MN, false);

		}

		/* Add 1C09*/
		strcpy(pbMNIndex, "1C09");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);

			AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(),
					(char*) "40", false);

		}

		/* Add 1F26*/
		strcpy(pbMNIndex, "1F26");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);

			char* pbVal = new char[8];
			iConfigDate = getConfigDate();
			pbVal = _IntToAscii(iConfigDate, pbVal, 10);
			//hexVal = padLeft(hexVal, '0' , 8);
			strcpy(pbVal, ConvertToUpper(pbVal));
			AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), pbVal,
					false);

			//AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), hexVal, false);
			delete[] pbVal;
		}

#if defined DEBUG	
		cout << "1F26 subidex added" << endl;
#endif
		/* Add 1F27*/
		strcpy(pbMNIndex, "1F27");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);

			char* pbVal = new char[50];
			iConfigTime = getConfigTime();
			pbVal = _IntToAscii(iConfigTime, pbVal, 10);
			strcpy(pbVal, ConvertToUpper(pbVal));
			AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), pbVal,
					false);
			delete[] pbVal;
		}

		/* Add 1F84*/
		strcpy(pbMNIndex, "1F84");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);

			AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(),
					(char*) "", true);

		}

		/* Add 1F89*/
		strcpy(pbMNIndex, "1F89");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);
			strcpy(pbSidx, "02");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) " ", pobjIdxCol,
					pobjNode->getNodeId(), MN, true);

		}

		/* Add 1F8A*/
		strcpy(pbMNIndex, "1F8A");
		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS
				|| stRetInfo.code == OCFM_ERR_INDEX_ALREADY_EXISTS)
		{
			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);
			strcpy(pbSidx, "00");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) " ", pobjIdxCol,
					pobjNode->getNodeId(), MN, true);

			//Todo By M as subindex 01 shud be equal to 02, need to find the reason 
			strcpy(pbSidx, "01");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) "100000", pobjIdxCol,
					pobjNode->getNodeId(), MN, false);

			strcpy(pbSidx, "02");
			SetSIdxValue(pbMNIndex, pbSidx, (char*) " ", pobjIdxCol,
					pobjNode->getNodeId(), MN, true);

		}

		/* Add 1F98*/
		strcpy(pbMNIndex, "1F98");

		stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
		if (stRetInfo.code == OCFM_ERR_SUCCESS)
		{

			pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
			/* $:set Flag to true*/
			pobjIndex->setFlagIfIncludedCdc(TRUE);

			strcpy(pbSidx, "05");
			SetSIdxValue(pbMNIndex, pbSidx, abC_DLL_ISOCHR_MAX_PAYL, pobjIdxCol,
					pobjNode->getNodeId(), MN, false);

		}

		delete[] pbMNIndex;
		delete[] pbSidx;
	} catch (ocfmException & ex)
	{
		delete[] pbMNIndex;
		delete[] pbSidx;
		return ex._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		GenerateMNOBD

 This API shall be used to auto generate the MN's object dictionary.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GenerateMNOBD()
{
	return (GenerateMNOBD(false));
}

/*****************************************************************************/
/**
 \brief		GenerateMNOBD

 This function automatically generates the MN's object dictionary 

 \param		IsBuild		Boolean value to set if this is a build process or not

 \return	ocfmRetCode
 */
/*****************************************************************************/
ocfmRetCode GenerateMNOBD(bool IsBuild)
{

	CNode *pobjMNNode = NULL;
	CNodeCollection *pobjNodeCollection = NULL;
	CIndexCollection *objMNIndexCol = NULL;

	char* pbMNIndex = new char[INDEX_LEN];
	char* pbIdx = new char[SUBINDEX_LEN];
	char* pbMappIdx = new char[SUBINDEX_LEN];

	ocfmRetCode stRetInfo;

	ocfmException objocfmException;
	INT32 iOutPrevSubIndex = 0;
	INT32 iOutPrevSize = 0;
	INT32 iChainOutPrevSubIndex = 0;
	INT32 iChainOutPrevSize = 0;
	INT32 iInPrevSubIndex = 0;
	INT32 iInPrevSize = 0;
	INT32 iIndexPos = 0;
	INT32 iRxChannelCount = 0;
	INT32 iTxChannelCount = 0;
	INT32 iMaxNumberOfChannels = 0;

	EStationType eCurrCNStation;
	try
	{
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		pobjMNNode = pobjNodeCollection->getNodePtr(MN, MN_NODEID);
		if (pobjMNNode == NULL)
		{
			objocfmException.ocfm_Excpetion(OCFM_ERR_MN_NODE_DOESNT_EXIST);
			throw objocfmException;
		}
		else
		{
			/*Process PDO Nodes*/
			stRetInfo = ProcessPDONodes(IsBuild);
#if defined DEBUG
			cout << "PDO's in CN Nodes Processed" << endl;
#endif
			if (stRetInfo.code != OCFM_ERR_SUCCESS)
			{
				return stRetInfo;
			}

			//CNode objMNNode;
			//CNodeCollection *pobjNodeCollection;
			CIndexCollection *pobjIndexCollection = NULL;
			//CIndex objIndex;

			//pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
			//objMNNode = pobjNodeCollection->getNode(MN, MN_NODEID);
			//pobjIndexCollection = objMNNode.getIndexCollection();

			CNetworkManagement *pobjNwManagement = NULL;
			pobjNwManagement = pobjMNNode->getNetworkManagement();
			iMaxNumberOfChannels = pobjNwManagement->getMaxPDOCount();
#if defined DEBUG
			cout << "Max Number Of Channels" << iMaxNumberOfChannels << endl;
#endif

			/* Delete the MN's old object dictionary*/
			pobjIndexCollection = pobjMNNode->getIndexCollection();
			pobjIndexCollection->DeletePDOs();
#if defined DEBUG
			cout << "MN Node PDO's deleted" << endl;
#endif
			// Delete Process Image Objects
			pobjIndexCollection->DeletePIObjects();
#if defined DEBUG
			cout << "Deleted PI objects (Axxx indexes) in MN" << endl;
#endif
			// Autogenertate other indexs 
			AuotgenerateOtherIndexs(pobjMNNode);
			/* Add other Indexes than PDO*/
#if defined DEBUG
			cout << "Auotgenerated Other Indexs in MN" << endl;
#endif
		}

		bool bIsPresMN = false;
		bIsPresMN = IsPresMN();

		if (true == bIsPresMN)
		{
#if defined DEBUG
			cout << "PresMN: iTxChannelCount set to 1" << endl;
#endif
			iTxChannelCount = 1;
		}
		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		pobjMNNode = pobjNodeCollection->getNodePtr(MN, MN_NODEID);

		INT32* pArrangedNodeIDbyStation = NULL;
		CNode objNode;
		pArrangedNodeIDbyStation = ArrangeNodeIDbyStation();

		for (INT32 iLoopCount = 0;
				iLoopCount < pobjNodeCollection->getNumberOfNodes();
				iLoopCount++)
		{
			objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);

			if (objNode.getNodeType() == CN)
			{

				char* pbVersionNumber = new char[4];
				pbVersionNumber[0] = 0;
				eCurrCNStation = objNode.getStationType();

				pobjMNNode = pobjNodeCollection->getNodePtr(MN, MN_NODEID);
				objMNIndexCol = pobjMNNode->getIndexCollection();

				if (objNode.MNPDOOUTVarCollection.Count() != 0)
				{
					if (CHAINED != eCurrCNStation)
					{
						iOutPrevSubIndex = 0;
						iOutPrevSize = 0;
					}
					else
					{
						iOutPrevSubIndex = iChainOutPrevSubIndex;
						iOutPrevSize = iChainOutPrevSize;
					}
					/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
					CIndex* pobjIndex;
					char* pbMappNodeID = new char[SUBINDEX_LEN];
					strcpy(pbMNIndex, "18");
					if (CHAINED != eCurrCNStation)
					{
						pbIdx = _IntToAscii(iTxChannelCount, pbIdx, 16);
						iTxChannelCount++;

						pbIdx = padLeft(pbIdx, '0', 2);
						pbMNIndex = strcat(pbMNIndex, pbIdx);
						stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);

						//to write cn node id in 18XX/01
						pbMappNodeID = _IntToAscii(objNode.getNodeId(),
								pbMappNodeID, 10);
					}
					else
					{
						//1800 is used of PRes chained station
						iIndexPos = 0;
						strcpy(pbMNIndex, (char *) "1800");
						strcpy(pbIdx, (char *) "00");
						stRetInfo = IfIndexExists(MN_NODEID, MN, pbMNIndex,
								&iIndexPos);
						if (stRetInfo.code != OCFM_ERR_SUCCESS)
						{
							stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
						}

						//to write 0 in 18XX/01 to indicate PRes MN
						strcpy(pbMappNodeID, (char *) "0x0");
					}
					/* set bFlag to true for 1800*/
					pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);

					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						objocfmException.ocfm_Excpetion(stRetInfo.code);
						delete[] pArrangedNodeIDbyStation;
						throw objocfmException;
					}

					char* pbSidx = new char[SUBINDEX_LEN];
					strcpy(pbSidx, "01");
					SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex, pbSidx,
							pbMappNodeID, (char*) "NodeID_U8", TRUE);
					delete[] pbMappNodeID;

					GetSubIndexAttributes(objNode.getNodeId(), CN,
							(char*) "1400", (char*) "02", ACTUALVALUE,
							pbVersionNumber);
					if ((NULL == pbVersionNumber)
							|| (strcmp(pbVersionNumber, "") == 0))
					{
						strcpy(pbVersionNumber, "0x0");
					}
					SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex,
							(char*) "02", pbVersionNumber,
							(char*) "MappingVersion_U8", TRUE);

					strcpy(pbMNIndex, "1A");
					strcat(pbMNIndex, pbIdx);
					/* Set the MN's PDO Index*/
					iIndexPos = 0;
					stRetInfo = IfIndexExists(MN_NODEID, MN, pbMNIndex,
							&iIndexPos);
					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);
					}

					pobjIndex->setFlagIfIncludedCdc(TRUE);

					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						objocfmException.ocfm_Excpetion(stRetInfo.code);
						delete[] pArrangedNodeIDbyStation;
						throw objocfmException;
					}

					for (INT32 iLoopCount = 0;
							iLoopCount < objNode.MNPDOOUTVarCollection.Count();
							iLoopCount++)
					{
						MNPdoVariable stMNPdoVar;
						stMNPdoVar = objNode.MNPDOOUTVarCollection[iLoopCount];
						pobjIndex = objMNIndexCol->getIndexbyIndexValue(
								pbMNIndex);

						if (pobjIndex != NULL)
						{
							pobjIndex->setFlagIfIncludedCdc(TRUE);
							GetMNPDOSubIndex(stMNPdoVar, iOutPrevSubIndex,
									pobjIndex, pbMNIndex, iOutPrevSize);
							iOutPrevSize = iOutPrevSize + stMNPdoVar.DataSize;
						}
					}

					if (CHAINED == eCurrCNStation)
					{
						iChainOutPrevSubIndex = iOutPrevSubIndex;
						iChainOutPrevSize = iOutPrevSize;
					}
				}

				if (objNode.MNPDOINVarCollection.Count() != 0)
				{
					/* Create PDO_TxCommParam_XXh_REC 1800 INdex*/
					CIndex* pobjIndex;
					strcpy(pbMNIndex, "14");
					pbIdx = _IntToAscii(iRxChannelCount, pbIdx, 16);
					iRxChannelCount++;

					pbIdx = padLeft(pbIdx, '0', 2);
					pbMNIndex = strcat(pbMNIndex, pbIdx);

					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);					/* set bFlag to true for 1800*/
					pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
					if (pobjIndex != NULL)
						pobjIndex->setFlagIfIncludedCdc(TRUE);

					iInPrevSubIndex = 0;
					iInPrevSize = 0;
					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						objocfmException.ocfm_Excpetion(stRetInfo.code);
						delete[] pArrangedNodeIDbyStation;
						throw objocfmException;
					}

					pbMappIdx = _IntToAscii((objNode.getNodeId()), pbMappIdx,
							10);
					char* pbSidx = new char[SUBINDEX_LEN];
					strcpy(pbSidx, "01");
					SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex, pbSidx,
							pbMappIdx, (char*) "NodeID_U8", TRUE);

					GetSubIndexAttributes(objNode.getNodeId(), CN,
							(char*) "1800", (char*) "02", ACTUALVALUE,
							pbVersionNumber);
					if ((NULL == pbVersionNumber)
							|| (strcmp(pbVersionNumber, "") == 0))
					{
						strcpy(pbVersionNumber, "0x0");
					}
					SetSubIndexAttributes(MN_NODEID, MN, pbMNIndex,
							(char*) "02", pbVersionNumber,
							(char*) "MappingVersion_U8", TRUE);
					delete[] pbSidx;

					strcpy(pbMNIndex, "16");
					strcat(pbMNIndex, pbIdx);
					/* Set the MN's PDO Index*/
					stRetInfo = AddIndex(MN_NODEID, MN, pbMNIndex);


					if (stRetInfo.code != OCFM_ERR_SUCCESS)
					{
						objocfmException.ocfm_Excpetion(stRetInfo.code);
						delete[] pArrangedNodeIDbyStation;
						throw objocfmException;
					}

					pobjIndex = objMNIndexCol->getIndexbyIndexValue(pbMNIndex);
					for (INT32 iLoopCount = 0;
							iLoopCount < objNode.MNPDOINVarCollection.Count();
							iLoopCount++)
					{
						MNPdoVariable stMNPdoVar;
						stMNPdoVar = objNode.MNPDOINVarCollection[iLoopCount];
						pobjIndex = objMNIndexCol->getIndexbyIndexValue(
								pbMNIndex);
						pobjIndex->setFlagIfIncludedCdc(TRUE);
						GetMNPDOSubIndex(stMNPdoVar, iInPrevSubIndex, pobjIndex,
								pbMNIndex, iInPrevSize);
						iInPrevSize = iInPrevSize + stMNPdoVar.DataSize;
					}
				}
				delete[] pbVersionNumber;
			}
		}
		setPresMNNodeAssigmentBits();

		delete[] pArrangedNodeIDbyStation;
		setFlagForRequiredMNIndexes(MN_NODEID);
	}

	catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}

	if (iTxChannelCount > iMaxNumberOfChannels)
	{
		stRetInfo.code = OCFM_ERR_EXCESS_CHANNEL;
	}
	else
	{
		stRetInfo.code = OCFM_ERR_SUCCESS;
	}
	delete[] pbMNIndex;
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		checkIfOffsetUsed

 This function checks for if the offset is used or not.

 \param		iOffset			Integer variable to hold the offset value
 \param		enumPdoType		Enum of type EPDOType to hold pdo type

 \return	void
 */
/*****************************************************************************/
//TODO: unused function
void checkIfOffsetUsed(INT32& iOffset, EPDOType enumPdoType)
{
	CNodeCollection* pobjNodeCol;
	pobjNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode objNode;

	for (INT32 iNodeIndex = 0; iNodeIndex < pobjNodeCol->getNumberOfNodes();
			iNodeIndex++)
	{
		objNode = pobjNodeCol->getNodebyCollectionIndex(iNodeIndex);
		if (objNode.getNodeType() == CN)
		{
			if (enumPdoType == PDO_RPDO)
			{
				for (INT32 iLoopCount = 0;
						iLoopCount < objNode.MNPDOOUTVarCollection.Count();
						iLoopCount++)
				{
					if (objNode.MNPDOOUTVarCollection[iLoopCount].Offset
							== iOffset)
						iOffset =
								iOffset
										+ objNode.MNPDOOUTVarCollection[iLoopCount].DataSize
												/ 8;
				}
			}
			else if (enumPdoType == PDO_TPDO)
			{
				for (INT32 iLoopCount = 0;
						iLoopCount < objNode.MNPDOINVarCollection.Count();
						iLoopCount++)
				{
					if (objNode.MNPDOINVarCollection[iLoopCount].Offset
							== iOffset)
						iOffset =
								iOffset
										+ objNode.MNPDOINVarCollection[iLoopCount].DataSize
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

/*****************************************************************************/
/**
 \brief		ComputeOUTOffset

 This function computes and returns the OUTPUT offset for the process image variables

 \param		iDataSize		Integer variable to hold the data size
 \param		enumPdoType		Enum of type EPDOType to hold pdo type

 \return	INT32
 */
/*****************************************************************************/
//TODO: No need of enumPdotype for size calculation. to be removed in header
INT32 ComputeOUTOffset(INT32 iDataSize, EPDOType enumPdoType)
{
	INT32 iOffset = 0;

	switch (iDataSize)
	{

	/*Unsigned8, Int8*/
	case 8:
		stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
		iOffset = stSize8OUTOffset.currOffset;
		stSize8OUTOffset.currOffset = stSize8OUTOffset.currOffset + 1;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize16OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset;
			stSize16OUTOffset.currOffset = stSize16OUTOffset.currOffset + 2;
		}

		/* if greater no change*/
		if (stSize32OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset;
			stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (stSize64OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
			stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		}
		break;

		/*Unsigned16, Int16*/
	case 16:
		stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset;
		iOffset = stSize16OUTOffset.currOffset;
		stSize16OUTOffset.currOffset = stSize16OUTOffset.currOffset + 2;

		/* Set other DataType Offsets*/

		/* if greater no change*/
		if (stSize8OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
		{
		}
		else
		{
			stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
			stSize8OUTOffset.currOffset = stSize16OUTOffset.currOffset;
		}

		/* if greater no change*/
		if (stSize32OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
		{
			//
		}
		else
		{
			stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset;
			stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (stSize64OUTOffset.currOffset >= stSize16OUTOffset.currOffset)
		{
		}
		else
		{
			stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
			stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		}
		break;
		/*Unsigned32, Int32*/
	case 32:
		stSize32OUTOffset.prevOffset = stSize32OUTOffset.currOffset;
		iOffset = stSize32OUTOffset.currOffset;
		stSize32OUTOffset.currOffset = stSize32OUTOffset.currOffset + 4;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize8OUTOffset.currOffset >= stSize32OUTOffset.currOffset)
		{
			//
		}
		else
		{
			stSize8OUTOffset.prevOffset = stSize8OUTOffset.currOffset;
			stSize8OUTOffset.currOffset = stSize32OUTOffset.currOffset;

		}

		/* if greater no change*/
		if (stSize16OUTOffset.currOffset >= stSize32OUTOffset.currOffset)
		{
		}
		else
		{
			stSize16OUTOffset.prevOffset = stSize16OUTOffset.currOffset;
			stSize16OUTOffset.currOffset = stSize32OUTOffset.currOffset;

		}
		/* if greater no change*/
		if (stSize64OUTOffset.currOffset >= stSize8OUTOffset.currOffset)
		{
		}
		else
		{
			stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
			stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		}
		/*Unsigned64, Int64*/
		break;
	case 64:
		stSize64OUTOffset.prevOffset = stSize64OUTOffset.currOffset;
		iOffset = stSize64OUTOffset.currOffset;
		stSize64OUTOffset.currOffset = stSize64OUTOffset.currOffset + 8;
		break;
	default:
		cout << "Undefined DataSize Encountered:" << __FUNCTION__ << endl;
		break;
	}
	return iOffset;
}

/*****************************************************************************/
/**
 \brief		ComputeINOffset

 This function computes and returns the INPUT offset for the process image variables

 \param		iDataSize		Integer variable to hold the data size
 \param		enumPdoType		Enum of type EPDOType to hold pdo type

 \return	INT32
 */
/*****************************************************************************/

//TODO: No need of enumPdotype for size calculation. to be removed in header
INT32 ComputeINOffset(INT32 iDataSize, EPDOType enumPdoType)
{
	INT32 iOffset = 0;

	switch (iDataSize)
	{

	/*Unsigned8, Int8*/
	case 8:
		stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
		iOffset = stSize8INOffset.currOffset;
		stSize8INOffset.currOffset = stSize8INOffset.currOffset + 1;
		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize16INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{
			stSize16INOffset.prevOffset = stSize16INOffset.currOffset;
			stSize16INOffset.currOffset = stSize16INOffset.currOffset + 2;
		}

		/* if greater no change*/
		if (stSize32INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{

			stSize32INOffset.prevOffset = stSize32INOffset.currOffset;
			stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;

		}

		/* if greater no change*/
		if (stSize64INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{
			stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
			stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		}
		break;

		/*Unsigned16, Int16*/
	case 16:
		stSize16INOffset.prevOffset = stSize16INOffset.currOffset;
		iOffset = stSize16INOffset.currOffset;
		stSize16INOffset.currOffset = stSize16INOffset.currOffset + 2;

		/* Set other DataType Offsets*/

		/* if greater no change*/
		if (stSize8INOffset.currOffset >= stSize16INOffset.currOffset)
		{
		}
		else
		{
			stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
			stSize8INOffset.currOffset = stSize16INOffset.currOffset;

		}

		/* if greater no change*/
		if (stSize32INOffset.currOffset >= stSize16INOffset.currOffset)
		{
		}
		else
		{
			stSize32INOffset.prevOffset = stSize32INOffset.currOffset;
			stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;
		}

		/* if greater no change*/
		if (stSize64INOffset.currOffset >= stSize16INOffset.currOffset)
		{
		}
		else
		{
			stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
			stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		}
		break;
		/*Unsigned32, Int32*/
	case 32:
		stSize32INOffset.prevOffset = stSize32INOffset.currOffset;
		iOffset = stSize32INOffset.currOffset;
		stSize32INOffset.currOffset = stSize32INOffset.currOffset + 4;

		/* Set other DataType Offsets*/
		/* if greater no change*/
		if (stSize8INOffset.currOffset >= stSize32INOffset.currOffset)
		{
		}
		else
		{
			stSize8INOffset.prevOffset = stSize8INOffset.currOffset;
			stSize8INOffset.currOffset = stSize32INOffset.currOffset;

		}

		/* if greater no change*/
		if (stSize16INOffset.currOffset >= stSize32INOffset.currOffset)
		{
		}
		else
		{
			stSize16INOffset.prevOffset = stSize16INOffset.currOffset;
			stSize16INOffset.currOffset = stSize32INOffset.currOffset;

		}
		/* if greater no change*/
		if (stSize64INOffset.currOffset >= stSize8INOffset.currOffset)
		{
		}
		else
		{
			stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
			stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		}
		break;
		/*Unsigned64, Int64*/
	case 64:
		stSize64INOffset.prevOffset = stSize64INOffset.currOffset;
		iOffset = stSize64INOffset.currOffset;
		stSize64INOffset.currOffset = stSize64INOffset.currOffset + 8;
		break;
	default:
		cout << "Undefined DataSize Encountered:" << __FUNCTION__ << endl;
		break;
	}
	return iOffset;
}

/*****************************************************************************/
/**
 \brief		FreeProjectMemory

 This API shall be used to free the memory occupied by the shared library objects for the current project& settings. After calling this API, the project configurations & setting are no more valid.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode FreeProjectMemory()
{
	CNodeCollection *pobjNodeCollection = NULL;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	delete pobjNodeCollection;
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		OpenProject

 This API shall be used to open a saved openCONFIGURATOR project. This API reads through the 'ProjectName'.oct file and from which reads the respective 'NodeNumber'.octx files in the project location and loads the shared library objects for the project.

 \param		pbPjtPath				Character pointer to hold the project path
 \param		pbProjectXmlFileName	Character pointer to hold the file name of the project xml

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode OpenProject(char* pbPjtPath, char* pbProjectXmlFileName)
{
	CNodeCollection *pobjNodeCollection;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	xmlTextReaderPtr pxReader;
	INT32 iRetVal;
	char *pbFileName = NULL;

#if defined DEBUG
	cout << "\nStrLen for FileName:"
	<< (strlen(pbPjtPath) + strlen(pbProjectXmlFileName) + 1) << endl;
#endif

	pbFileName =
			new char[(strlen(pbPjtPath) + strlen(pbProjectXmlFileName) + 5)];
#if defined(_WIN32) && defined(_MSC_VER)
	{
		sprintf(pbFileName, "%s\\%s", pbPjtPath, pbProjectXmlFileName);
	}
#else
	{
		sprintf(pbFileName, "%s/%s", pbPjtPath, pbProjectXmlFileName);
	}
#endif

	pxReader = xmlReaderForFile(pbFileName, NULL, 0);
	delete[] pbFileName;
	try
	{
		if (pxReader != NULL)
		{
			iRetVal = xmlTextReaderRead(pxReader);
			while (iRetVal == 1)
			{
				processProjectXML(pxReader, pbPjtPath);
				iRetVal = xmlTextReaderRead(pxReader);
			}
			if (iRetVal != 0)
			{
				ocfmException objException;
#if defined DEBUG 
				cout << "\nOCFM_ERR_PARSE_XML\n" << endl;
#endif
				objException.ocfm_Excpetion(OCFM_ERR_PARSE_XML);
				throw objException;
			}
		}
		else
		{
#if defined DEBUG 
			cout << "\nOCFM_ERR_CANNOT_OPEN_FILE\n" << endl;
#endif
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_CANNOT_OPEN_FILE);
			throw objException;
		}

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		CNode objNode;
		INT32 iNodeID;
		ENodeType iNodeType;

		char* pbPresTimeoutVal = new char[50];
		pbPresTimeoutVal[0] = 0;

		for (INT32 iLoopCount = 0;
				iLoopCount < pobjNodeCollection->getNumberOfNodes();
				iLoopCount++)
		{
			objNode = pobjNodeCollection->getNodebyCollectionIndex(iLoopCount);

			iNodeType = objNode.getNodeType();
			iNodeID = objNode.getNodeId();
			copyPDODefToAct(iNodeID, iNodeType);
			copyMNPropDefToAct(iNodeID, iNodeType);

			if ((iNodeID != MN_NODEID) && (MN != iNodeType))
			{
				char* strConvertedValue = NULL;
				strConvertedValue = new char[SUBINDEX_LEN];
				strConvertedValue = _IntToAscii(iNodeID, strConvertedValue, 16);
				strConvertedValue = padLeft(strConvertedValue, '0', 2);
				GetSubIndexAttributes(MN_NODEID, MN, (char*) "1f92",
						strConvertedValue, ACTUALVALUE, pbPresTimeoutVal);
#if defined DEBUG
				cout << "Actual Value" << iNodeID << pbPresTimeoutVal << endl;
#endif
				if (((NULL == pbPresTimeoutVal)
						|| (strcmp(pbPresTimeoutVal, "") == 0))
						|| (!(ValidateCNPresTimeout(strConvertedValue,
								pbPresTimeoutVal))))
				{
					calculateCNPollResponse(iNodeID, iNodeType);
				}

				delete[] strConvertedValue;
			}
		}
		delete[] pbPresTimeoutVal;
	}

	catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		processProjectXML

 Process the Node value, Name and its attributes

 \param		pxReader	xml pointer of type xmlTextReaderPtr
 \param		pbPjtPath	Character pointer to hold the Project path

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode processProjectXML(xmlTextReaderPtr pxReader, char* pbPjtPath)
{
	const xmlChar *pxcName;

	pxcName = xmlTextReaderConstName(pxReader);
	if (pxcName == NULL)
	{
#if defined DEBUG
		cout << "\nGot NULL for Name\n" << endl;
#endif
	}
	//value = xmlTextReaderConstValue(pxReader);
	try
	{
		if (xmlTextReaderNodeType(pxReader) == 1)
		{
			// Check for openCONFIGURATOR Tag
			if (strcmp(((char*) pxcName), "openCONFIGURATOR") == 0)
			{
#if defined DEBUG
				cout << "openCONFIGURATOR Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					while (xmlTextReaderMoveToNextAttribute(pxReader))
					{
						// Call Check Version number Fn
						if (IfVersionNumberMatches(pxReader) == false)
						{
#if defined DEBUG
							cout << "openCONFIGURATOR Tag present\n" << endl;
#endif
							ocfmException objException;
							objException.ocfm_Excpetion(
									OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH);
							throw objException;
						}
					}
				}
			}
			else if (strcmp(((char*) pxcName), "profile") == 0)
			{
#if defined DEBUG
				cout << "profile Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "Auto") == 0)
			{
#if defined DEBUG
				cout << "Auto Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					if (setProjectSettings_Auto(pxReader) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						ocfmException objException;
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
						throw objException;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "Communication") == 0)
			{
#if defined DEBUG
				cout << "Communication Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					if (setProjectSettings_Communication(pxReader) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						ocfmException objException;
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
						throw objException;
					}
				}
				else
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "NodeCollection") == 0)
			{
#if defined DEBUG
				cout << "NodeCollection Tag present\n" << endl;
#endif
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
#if defined DEBUG
					cout << "Cannot open project: Invalid Project XML\n"
					<< endl;
#endif
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
			else if (strcmp(((char*) pxcName), "Node") == 0)
			{
				if (xmlTextReaderHasAttributes(pxReader) == 1)
				{
					if (getandCreateNode(pxReader, pbPjtPath) == false)
					{
#if defined DEBUG
						cout << "Cannot open project: Invalid Project XML\n"
						<< endl;
#endif
						ocfmException objException;
						objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
						throw objException;
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
					ocfmException objException;
					objException.ocfm_Excpetion(OCFM_ERR_INVALID_PJTXML);
					throw objException;
				}
			}
		}

	} catch (ocfmException* objocfmException)
	{
		return objocfmException->_ocfmRetCode;
	}

	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		setProjectSettings_Auto

 Gets the Auto info from the Pjt xml and stores in the object

 \param		pxReader	xml pointer of type xmlTextReaderPtr

 \return	BOOL

 \retval	TRUE		if successful
 \retval	FALSE		if there is already a message pending
 */
/*****************************************************************************/

bool setProjectSettings_Auto(xmlTextReaderPtr pxReader)
{
	const xmlChar* pxcName;
	const xmlChar* pxcValue;
	CPjtSettings* pobjPjtSettings;
	pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();

	while (xmlTextReaderMoveToNextAttribute(pxReader))
	{
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);

		if (pxcValue == NULL || pxcName == NULL)
			return false;
#if defined DEBUG
		cout << "\nName:" << pxcName << endl;
		cout << "\nValue:" << pxcValue << endl;
#endif
		if (strcmp(((char*) pxcName), "Generate") == 0)
		{
			if (strcmp(((char*) pxcValue), "YES") == 0)
				pobjPjtSettings->setGenerateAttr(YES_AG);
			else if (strcmp(((char*) pxcValue), "NO") == 0)
				pobjPjtSettings->setGenerateAttr(NO_AG);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) pxcName), "Save") == 0)
		{
			if (strcmp(((char*) pxcValue), "YES") == 0)
				pobjPjtSettings->setSaveAttr(YES_AS);
			else if (strcmp(((char*) pxcValue), "PROMPT") == 0)
				pobjPjtSettings->setSaveAttr(PROMPT_AS);
			else if (strcmp(((char*) pxcValue), "DISCARD") == 0)
				pobjPjtSettings->setSaveAttr(DISCARD_AS);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) pxcName), "View") == 0)
		{
			if (strcmp(((char*) pxcValue), "General") == 0)
				pobjPjtSettings->setViewMode(SIMPLE);
			else if (strcmp(((char*) pxcValue), "Advanced") == 0)
				pobjPjtSettings->setViewMode(EXPERT);
			else
			{
#if defined DEBUG
				cout << "\nsetProjectSettings_Auto returning false" << endl;
#endif
				return false;
			}
		}
		else if (strcmp(((char*) pxcName), "ExpertViewSelected") == 0)
		{
			if (strcmp(((char*) pxcValue), "true") == 0)
				pobjPjtSettings->setExpertViewSelectedFlag(true);
			else if (strcmp(((char*) pxcValue), "false") == 0)
				pobjPjtSettings->setExpertViewSelectedFlag(false);
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

/*****************************************************************************/
/**
 \brief		setProjectSettings_Communication

 Gets the communication info from the Pjt xml and stores in the object

 \param		pxReader	xml pointer of type xmlTextReaderPtr

 \return	BOOL

 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool setProjectSettings_Communication(xmlTextReaderPtr pxReader)
{
	const xmlChar* pxcName;
	const xmlChar* pxcValue;
	CPjtSettings* pobjPjtSettings;
	pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();

	while (xmlTextReaderMoveToNextAttribute(pxReader))
	{
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);

		if (pxcValue == NULL || pxcName == NULL)
			return false;
#if defined DEBUG
		cout << "\nName:" << pxcName << endl;
		cout << "\nValue:" << pxcValue << endl;
#endif
		if (strcmp(((char*) pxcName), "IP") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				pobjPjtSettings->setPOWERLINK_IP((char*) pxcValue);
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

/*****************************************************************************/
/**
 \brief		getandCreateNode

 Gets the Node properties from the Pjt xml and Creates the Nodes

 \param		pxReader		xml pointer of type xmlTextReaderPtr
 \param		pbPjtPath		Character pointer to hold the Project path

 \return	BOOL

 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool getandCreateNode(xmlTextReaderPtr pxReader, char* pbPjtPath)
{
	const xmlChar *pxcName = NULL;
	const xmlChar *pxcValue = NULL;

	char* pbNodeName = NULL;
	char* pbXdcPath = NULL;
	char* pbFileName = NULL;
	char* pbForceCycleValue = NULL;

	INT32 iNodeID; //can't be initialised. and if it is not present false is returned.
	ENodeType enumNodeType; //can't be initialised. and if it is not present false is returned.
	bool bForceCycleFlag = false;
	EStationType eStationType;
	ocfmRetCode stErrorInfo;

	eStationType = NORMAL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;

	//CPjtSettings* pobjPjtSettings;
	//pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();

	while (xmlTextReaderMoveToNextAttribute(pxReader))
	{
		//Retrieve the pxcName and Value of an attribute	
		pxcValue = xmlTextReaderConstValue(pxReader);
		pxcName = xmlTextReaderConstName(pxReader);

		if (pxcValue == NULL || pxcName == NULL)
		{
			return false;
		}
		else
		{
			//TODO: 'else' added. operation to be specified.
		}

		if (strcmp(((char*) pxcName), "name") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				pbNodeName = new char[strlen((char*) pxcValue) + ALLOC_BUFFER];
				strcpy((char*) pbNodeName, (char*) pxcValue);
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) pxcName), "NodeId") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				iNodeID = atoi((char*) pxcValue);
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				return false;
			}

		}
		else if (strcmp(((char*) pxcName), "NodeType") == 0)
		{
			if (strcmp(((char*) pxcValue), "MN") == 0)
			{
				enumNodeType = MN;
#if defined DEBUG
				cout << "\nnodeType:" << enumNodeType << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "CN") == 0)
			{
				enumNodeType = CN;
#if defined DEBUG
				cout << "\nnodeType:" << enumNodeType << endl;
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
		else if (strcmp(((char*) pxcName), "xdc") == 0)
		{
			if ((char*) pxcValue != NULL)
			{
				pbXdcPath = new char[strlen((char*) pxcValue) + 1];
				strcpy((char*) pbXdcPath, (char*) pxcValue);
#if defined DEBUG
				cout << "\nxdcPath:" << pbXdcPath << endl;
#endif
			}
			else
			{
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
				pbXdcPath = NULL;
			}
		}
		else if (strcmp(((char*) pxcName), "ForceCycleFlag") == 0)
		{
			if (strcmp(((char*) pxcValue), "true") == 0)
			{
				bForceCycleFlag = true;
#if defined DEBUG
				cout << "\nForceCycleFlag:" << bForceCycleFlag << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "false") == 0)
			{
				bForceCycleFlag = false;
#if defined DEBUG
				cout << "\nForceCycleFlag:" << bForceCycleFlag << endl;
#endif
			}
			else
			{
				bForceCycleFlag = false;
#if defined DEBUG
				cout << "\ngetandCreateNode returning false" << endl;
#endif
			}
		}
		else if (strcmp(((char*) pxcName), "ForceCycle") == 0)
		{
			if ((char*) pxcValue != NULL && strcmp((char*) pxcValue, "") != 0)
			{
				pbForceCycleValue = new char[strlen((char*) pxcValue)
						+ ALLOC_BUFFER];
				strcpy((char*) pbForceCycleValue, (char*) pxcValue);
			}
			else
			{
				//TODO: 'else' added. operation to be specified.
			}
		}
		else if (strcmp(((char*) pxcName), "StationType") == 0)
		{
			if (strcmp(((char*) pxcValue), "Multiplexed") == 0)
			{
				eStationType = MULTIPLEXED;
#if defined DEBUG
				cout << "\nStationType:" << eStationType << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "Chained") == 0)
			{
				eStationType = CHAINED;
#if defined DEBUG
				cout << "\nStationType:" << eStationType << endl;
#endif
			}
			else if (strcmp(((char*) pxcValue), "Normal") == 0)
			{
				eStationType = NORMAL;
#if defined DEBUG
				cout << "\nStationType:" << eStationType << endl;
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
	cout << "\n enumNodeType:" << enumNodeType << endl;
#endif
	//pbNodeName 
	if (enumNodeType == 1)
	{
		stErrorInfo = CreateNode(iNodeID, CN, pbNodeName);
	}
	else if (enumNodeType == 0)
	{
		stErrorInfo = CreateNode(iNodeID, MN, pbNodeName);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (stErrorInfo.code != OCFM_ERR_SUCCESS)
	{
		return false;
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	CNode* objNode;
	CNodeCollection *pobjNodeCollection;
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
	objNode = pobjNodeCollection->getNodePtr(enumNodeType, iNodeID);
	objNode->setForceCycleFlag(bForceCycleFlag);

#if defined DEBUG
	cout << "\nCreateNode - stErrorInfo.code:" << stErrorInfo.code << endl;
#endif

	pbFileName = new char[(strlen(pbPjtPath) + strlen(pbXdcPath) + 5)];
	sprintf(pbFileName, "%s/%s", pbPjtPath, pbXdcPath);
	delete[] pbXdcPath;
	if (enumNodeType == 1)
	{
		stErrorInfo = parseFile(pbFileName, iNodeID, CN);
	}
	else if (enumNodeType == 0)
	{
		stErrorInfo = parseFile(pbFileName, iNodeID, MN);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (stErrorInfo.code != OCFM_ERR_SUCCESS)
	{
		return false;
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	if (enumNodeType == 1 && pbForceCycleValue != NULL)
	{
		objNode->setForcedCycle(pbForceCycleValue);
	}
	else
	{
		//TODO: 'else' added. operation to be specified.
	}

	objNode->setStationType(eStationType);

	//    copyDefToAct(iNodeID, enumNodeType);
	delete[] pbFileName;
	delete[] pbNodeName;
	return true;
}

/*****************************************************************************/
/**
 \brief		saveProjectXML

 Saves the project details into the Project location

 \param		pbProjectPath	Character pointer to hold the Project Path
 \param		pbProjectName	Character pointer to hold the Project Name

 \return	BOOL

 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending
 */
/*****************************************************************************/

bool saveProjectXML(char* pbProjectPath, char* pbProjectName)
{

	CPjtSettings* pobjPjtSettings;
	pobjPjtSettings = CPjtSettings::getPjtSettingsPtr();

	xmlTextWriterPtr pxtwWriter;
	xmlDocPtr pxdDoc;
	INT32 iBytesWritten;
	char* pbFileName = NULL;

	/* Create a new XmlWriter for DOM, with no compression. */
	pxtwWriter = xmlNewTextWriterDoc(&pxdDoc, 0);
	if (pxtwWriter == NULL)
	{
		printf("testXmlwriterDoc: Error creating the xml pxtwWriter\n");
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_CREATE_XML_WRITER_FAILED);
		throw objException;
	}
	/* Start the document with the xml default for the version,
	 * encoding UTF-8 and the default for the standalone
	 * declaration. */
	iBytesWritten = xmlTextWriterStartDocument(pxtwWriter, NULL, MY_ENCODING,
			NULL);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_START_DOC_FAILED);
		throw objException;
	}
	iBytesWritten = xmlTextWriterWriteComment(pxtwWriter,
			BAD_CAST "This file was autogenerated by openCONFIGURATOR");
	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start openCONFIGURATOR Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "openCONFIGURATOR");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}
	iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Version",
			BAD_CAST TOOL_VERSION);

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start profile Tag
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "profile");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start Auto Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Auto");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}

	if (pobjPjtSettings->getGenerateAttr() == NO_AG)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "Generate", BAD_CAST "NO");
	}
	else if (pobjPjtSettings->getGenerateAttr() == YES_AG)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "Generate", BAD_CAST "YES");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pobjPjtSettings->getSaveAttr() == YES_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save",
				BAD_CAST "YES");
	else if (pobjPjtSettings->getSaveAttr() == PROMPT_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save",
				BAD_CAST "PROMPT");
	else if (pobjPjtSettings->getSaveAttr() == DISCARD_AS)
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "Save",
				BAD_CAST "DISCARD");
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pobjPjtSettings->getViewMode() == SIMPLE)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "View",
				BAD_CAST "General");
	}
	else if (pobjPjtSettings->getViewMode() == EXPERT)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "View",
				BAD_CAST "Advanced");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	if (pobjPjtSettings->getExpertViewSelectedFlag() == true)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "ExpertViewSelected", BAD_CAST "true");
	}
	else if (pobjPjtSettings->getExpertViewSelectedFlag() == false)
	{
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "ExpertViewSelected", BAD_CAST "false");
	}
	else
	{
		//TODO: "else" added. Operation Need to specified
	}

	// End Auto Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start Communication Tag		
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "Communication");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}
	iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "IP",
			BAD_CAST "0.0.0.0");

	// End Communication Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	// End profile Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}
	xmlTextWriterSetIndent(pxtwWriter, 1);
	// Start NodeCollection Tag
	iBytesWritten = xmlTextWriterStartElement(pxtwWriter,
			BAD_CAST "NodeCollection");
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
		throw objException;
	}
	CNode objNode;
	CNodeCollection *pobjNodeCollection = NULL;
	CIndex objIndex;
	objIndex.setNodeID(objNode.getNodeId());
	pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();

	for (INT32 nodeCount = 0;
			nodeCount < pobjNodeCollection->getNumberOfNodes(); nodeCount++)
	{
		CNode* pobjNode;

		pobjNode = pobjNodeCollection->getNodebyColIndex(nodeCount);

		xmlTextWriterSetIndent(pxtwWriter, 1);

		// Start Node Tag		
		iBytesWritten = xmlTextWriterStartElement(pxtwWriter, BAD_CAST "Node");

		if (iBytesWritten < 0)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			ocfmException objException;        // = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		char* pbTempNodeName;
		pbTempNodeName = new char[50];
		strcpy(pbTempNodeName, (char*) pobjNode->getNodeName());
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "name",
				BAD_CAST pbTempNodeName);

		char* pbTempNodeID;
		pbTempNodeID = new char[20];

		_IntToAscii(pobjNode->getNodeId(), pbTempNodeID, 0);

		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
				BAD_CAST "NodeId", BAD_CAST pbTempNodeID);

		ENodeType tmp_NodeType;
		tmp_NodeType = pobjNode->getNodeType();

		if (tmp_NodeType == 0)
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "NodeType", BAD_CAST "MN");
		if (tmp_NodeType == 1)
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "NodeType", BAD_CAST "CN");

		char* pbTempXdcName = new char[50];

		sprintf(pbTempXdcName, "%s/%s.octx", "octx", pbTempNodeID);
		iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter, BAD_CAST "xdc",
				BAD_CAST pbTempXdcName);
		if (pobjNode->getForceCycleFlag() == true)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "ForceCycleFlag", BAD_CAST "true");
		}
		else if (pobjNode->getForceCycleFlag() == false)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "ForceCycleFlag", BAD_CAST "false");
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}

		if (pobjNode->getForcedCycle() != NULL
				&& strcmp(pobjNode->getForcedCycle(), ""))
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "ForceCycle", BAD_CAST pobjNode->getForcedCycle());
		}

		if (pobjNode->getStationType() == NORMAL)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "StationType", BAD_CAST "Normal");
		}
		else if (pobjNode->getStationType() == MULTIPLEXED)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "StationType", BAD_CAST "Multiplexed");
		}
		else if (pobjNode->getStationType() == CHAINED)
		{
			iBytesWritten = xmlTextWriterWriteAttribute(pxtwWriter,
					BAD_CAST "StationType", BAD_CAST "Chained");
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}

		// End Node Tag
		iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
		if (iBytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			ocfmException objException;        // = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			delete[] pbTempNodeName;
			delete[] pbTempNodeID;
			delete[] pbTempXdcName;
			throw objException;
		}

		delete[] pbTempNodeName;
		delete[] pbTempNodeID;
		delete[] pbTempXdcName;
	}

	// End NodeCollection Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	// End openCONFIGURATOR Tag
	iBytesWritten = xmlTextWriterEndElement(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
		throw objException;
	}

	iBytesWritten = xmlTextWriterEndDocument(pxtwWriter);
	if (iBytesWritten < 0)
	{
		printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
		ocfmException objException;        // = new ocfmException;
		objException.ocfm_Excpetion(OCFM_ERR_XML_END_DOC_FAILED);
		throw objException;
	}

	xmlFreeTextWriter(pxtwWriter);
	pbFileName = new char[MAX_FILE_PATH_SIZE];

#if defined(_WIN32) && defined(_MSC_VER)
	{

		sprintf(pbFileName, "%s\\%s\\%s.oct", pbProjectPath, pbProjectName,
				pbProjectName);
	}
#else
	{
		sprintf(pbFileName, "%s/%s/%s.oct", pbProjectPath, pbProjectName, pbProjectName);
	}
#endif
	xmlSaveFileEnc(pbFileName, pxdDoc, MY_ENCODING);

	xmlFreeDoc(pxdDoc);

	delete[] pbFileName;
	return true;
}

/*****************************************************************************/
/**
 \brief		CreateMNPDOVar

This function creates the MN pdo's in a collection used for process image manipulation

 \param		iOffset			Integer variable to hold the offset value
 \param		iDataSize		Integer variable to hold the size of the data
 \param		enumDataType	Enum of type IEC_Datatype to hold datatype
 \param		enumPdoType		Enum of type EPDOType to hold pdo type
 \param		pobjNode		Class pointer of the class CNode

 \return	void
 */
/*****************************************************************************/

void CreateMNPDOVar(INT32 iOffset, INT32 iDataSize, IEC_Datatype enumDataType,
		EPDOType enumPdoType, CNode *pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	MNPdoVariable objPDOvar;
	//CNodeCollection* pobjNodeCol;
	PIObject objpi;
	objpi.Index = NULL;
	objpi.SubIndex = NULL;
	//pobjNodeCol =  CNodeCollection::getNodeColObjectPointer();

	/* Assign the PDO type*/
	objPDOvar.pdoType = enumPdoType;
	objPDOvar.DataSize = iDataSize;
	/* Assign Index*/
	objPDOvar.Index = new char[4 + ALLOC_BUFFER];
	objPDOvar.SubIndex = new char[SUBINDEX_LEN + ALLOC_BUFFER];
	switch (enumDataType)
	{

	case USINT:
	case BITSTRING:
		PDODataType dt;
		switch (iDataSize)
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
		if (enumPdoType == PDO_TPDO)
		{
			objpi = getPIAddress(dt, INPUT, iOffset, iDataSize);

		}
		else if (enumPdoType == PDO_RPDO)
		{
			objpi = getPIAddress(dt, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case SINT:
		if (enumPdoType == PDO_TPDO)
		{
			objpi = getPIAddress(INTEGER8, INPUT, iOffset, iDataSize);
		}
		else if (enumPdoType == PDO_RPDO)
		{
			objpi = getPIAddress(INTEGER8, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case UINT:
		if (enumPdoType == PDO_TPDO)
		{
			objpi = getPIAddress(UNSIGNED16, INPUT, iOffset, iDataSize);
		}
		else if (enumPdoType == PDO_RPDO)
		{
			objpi = getPIAddress(UNSIGNED16, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case INT:
		if (enumPdoType == PDO_TPDO)
		{
			objpi = getPIAddress(INTEGER16, INPUT, iOffset, iDataSize);
		}
		else if (enumPdoType == PDO_RPDO)
		{
			objpi = getPIAddress(INTEGER16, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case UDINT:
		if (enumPdoType == PDO_TPDO)
		{
			objpi = getPIAddress(UNSIGNED32, INPUT, iOffset, iDataSize);
		}
		else if (enumPdoType == PDO_RPDO)
		{
			objpi = getPIAddress(UNSIGNED32, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case DINT:
	case REAL:
		if (enumPdoType == PDO_TPDO)
		{
			objpi = getPIAddress(INTEGER32, INPUT, iOffset, iDataSize);
		}
		else if (enumPdoType == PDO_RPDO)
		{
			objpi = getPIAddress(INTEGER32, OUTPUT, iOffset, iDataSize);
		}
		else
		{
			//TODO: "else" added. Operation Need to specified
		}
		break;
	case LINT:
	case LREAL:
		if(enumPdoType == PDO_TPDO)
		{
			objpi =  getPIAddress(INTEGER64, INPUT, iOffset, iDataSize);
		}
		else if(enumPdoType == PDO_RPDO)
		{
			objpi =  getPIAddress(INTEGER64, OUTPUT, iOffset, iDataSize);
		}
		break;
	case ULINT:
		if(enumPdoType == PDO_TPDO)
		{
			objpi =  getPIAddress(UNSIGNED64, INPUT, iOffset, iDataSize);
		}
		else if(enumPdoType == PDO_RPDO)
		{
			objpi =  getPIAddress(UNSIGNED64, OUTPUT, iOffset, iDataSize);
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
		cout << "UnHandled Datatype encountered:" << enumDataType << endl;
		break;
	}
#if defined DEBUG
	cout << __FUNCTION__ << " Ind:" << objpi.Index << " SubInd:"
	<< objpi.SubIndex << endl;
#endif
	strcpy(objPDOvar.Index, objpi.Index);
	strcpy(objPDOvar.SubIndex, objpi.SubIndex);
	/* Assign the value*/
	objPDOvar.Value = new char[10 + ALLOC_BUFFER];
	char* pbPadBuff = new char[5 + ALLOC_BUFFER];
	pbPadBuff = _IntToAscii(iDataSize, pbPadBuff, 16);
	pbPadBuff = padLeft(pbPadBuff, '0', 4);
	strcpy(pbPadBuff, ConvertToUpper(pbPadBuff));
	strcpy(objPDOvar.Value, "0x");
	strcat(objPDOvar.Value, pbPadBuff);
	/* Set the Offset*/
	strcat(objPDOvar.Value, "0000");
	/* Set the Reserved*/
	strcat(objPDOvar.Value, "00");

	pobjNode->addMNPDOvar(objPDOvar, enumPdoType);

}

/*****************************************************************************/
/**
 \brief		GetProjectSettings

 This API shall be used to get the Project settings information [auto save, auto generate] for the active project. If 'ocfmRetCode' is equal to ' OCFM_ERR_SUCCESS', the auto save information is filled into the 'enumAutoSave' and the auto generate information is filled into the 'enumAutoGen'. Refer to the EAutoGenerate and EautoSave enumerations.

 \param		enumAutoGen				Enum to hold the Auto generation option of the project.
 \param		enumAutoSave			Enum to hold the Auto save option of the project.
 \param		enumviewMode			Enum to hold the view mode option of the project.
 \param		bExpertViewAlreadySet	Boolean pointer to hold the view type option of the project.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetProjectSettings(EAutoGenerate *enumAutoGen,
		EAutoSave *enumAutoSave, EViewMode *enumviewMode,
		bool* bExpertViewAlreadySet)
{

	if ((NULL == enumAutoGen) || (NULL == enumAutoSave)
			|| (NULL == enumviewMode) || (NULL == bExpertViewAlreadySet))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	ocfmRetCode stRetInfo;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	stRetInfo.errorString = NULL;
	CPjtSettings* pobjPrjtSettings = NULL;
	pobjPrjtSettings = CPjtSettings::getPjtSettingsPtr();

	try
	{
		if (pobjPrjtSettings == NULL)
		{
			ocfmException objException;        // = new ocfmException;
			objException.ocfm_Excpetion(OCFM_ERR_PROJECT_SETTINGS);
			throw objException;
		}

		*enumAutoGen = pobjPrjtSettings->getGenerateAttr();
		*enumAutoSave = pobjPrjtSettings->getSaveAttr();
		*enumviewMode = pobjPrjtSettings->getViewMode();
		*bExpertViewAlreadySet = pobjPrjtSettings->getExpertViewSelectedFlag();

		stRetInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		SetProjectSettings

 This API shall be used to set the Project settings information [auto save, auto generate] for the active project. Refer to the EAutoGenerate and EautoSave enumerations.

 \param		enumAutoGen				Enum to hold the Auto generation option of the project.
 \param		enumAutoSave			Enum to hold the Auto save option of the project.
 \param		enumViewMode			Enum to hold the view mode option of the project.
 \param		bExpertViewAlreadySet	Boolean to hold the view type option of the project.

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode SetProjectSettings(EAutoGenerate enumAutoGen,
		EAutoSave enumAutoSave, EViewMode enumViewMode,
		bool bExpertViewAlreadySet)
{

	ocfmRetCode stRetInfo;
	stRetInfo.errorString = NULL;
	stRetInfo.code = OCFM_ERR_UNKNOWN;
	CPjtSettings* pobjPrjtSettings;
	pobjPrjtSettings = CPjtSettings::getPjtSettingsPtr();

	try
	{
		if (pobjPrjtSettings == NULL)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_PROJECT_SETTINGS);
			throw objException;
		}

		pobjPrjtSettings->setGenerateAttr(enumAutoGen);
		pobjPrjtSettings->setSaveAttr(enumAutoSave);
		pobjPrjtSettings->setViewMode(enumViewMode);
		pobjPrjtSettings->setExpertViewSelectedFlag(bExpertViewAlreadySet);
		stRetInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& objocfmException)
	{
		return objocfmException._ocfmRetCode;
	}
	return stRetInfo;
}

/*****************************************************************************/
/**
 \brief		UpdateNumberOfEnteriesSIdx

 Updates subindex "00"/NumberofEnteries with the total number of subindexes excluding subindex with id "00"

 \param		pobjIndex		Class pointer of the class CIndex
 \param		enumNodeType	Enum of type ENodeType to hold node type

 \return	void
 */
/*****************************************************************************/

//TODO: enumNodetype is not used. To be removed.
void UpdateNumberOfEnteriesSIdx(CIndex *pobjIndex, ENodeType enumNodeType)
{
	if (NULL == pobjIndex)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	CSubIndex *pobjSIdx = NULL;
	INT32 iTotalSIdxs = 0;

	pobjSIdx = pobjIndex->getSubIndexbyIndexValue((char*) "00");

	/* subindexes excluding "00"*/
	iTotalSIdxs = pobjIndex->getNumberofSubIndexes() - 1;

	if (pobjSIdx != NULL)
	{
		char pbAsciBuff[10];
		char* buffer = new char[10];
		strcpy(buffer, "0x");
		strcat(buffer, _IntToAscii(iTotalSIdxs, pbAsciBuff, 16));
		pobjSIdx->setActualValue(buffer);
		delete[] buffer;
	}
}

/*****************************************************************************/
/**
 \brief		AuotgenerateOtherIndexs

 This function Auot generates the values in the other required MN indexes

 \param		pobjNode	Class pointer of the class CNode

 \return	void
 */
/*****************************************************************************/

void AuotgenerateOtherIndexs(CNode* pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	ocfmRetCode retCode;
	char* pbMNIndex = new char[INDEX_LEN + ALLOC_BUFFER];
	char* pbSidx = new char[SUBINDEX_LEN + ALLOC_BUFFER];
	CIndex* pobjIndex;
	CIndexCollection* pobjIdxCol;

	INT32 IndexPos;

	pobjIdxCol = pobjNode->getIndexCollection();
	/* 1006*/
	strcpy(pbMNIndex, "1006");
#if defined DEBUG	
	cout << "string copied" << endl;
#endif
	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
#if defined DEBUG	
	cout << "retcode" << retCode.code << endl;
	cout << "1006 added" << endl;
#endif

	/*  1300*/
	strcpy(pbMNIndex, "1300");
	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		SetIndexAttributes(MN_NODEID, MN, pbMNIndex, (char*) "5000",
				(char*) "SDO_SequLayerTimeout_U32", TRUE);
	}

	/* 1C02*/
	strcpy(pbMNIndex, "1C02");
	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
		/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);
		/* Set subindex value 40 or 0000028 */
		strcpy(pbSidx, "00");
		SetSIdxValue(pbMNIndex, pbSidx, (char*) "3", pobjIdxCol,
				pobjNode->getNodeId(), MN, false);
#if defined DEBUG	
		cout << "1c02 subidex added" << endl;
#endif

		strcpy(pbSidx, "01");
		SetSIdxValue(pbMNIndex, pbSidx, (char*) "40", pobjIdxCol,
				pobjNode->getNodeId(), MN, false);
#if defined DEBUG	
		cout << "1c02 subidex 01 added" << endl;
#endif
		strcpy(pbSidx, "02");
		SetSIdxValue(pbMNIndex, pbSidx, (char*) "40", pobjIdxCol,
				pobjNode->getNodeId(), MN, false);

		strcpy(pbSidx, "03");
		SetSIdxValue(pbMNIndex, pbSidx, (char*) "40", pobjIdxCol,
				pobjNode->getNodeId(), MN, false);
	}

	/*  1C09*/
	strcpy(pbMNIndex, "1C09");
	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
		/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);

		AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(),
				(char*) "40", false);

	}

	/*  1F26*/
	strcpy(pbMNIndex, "1F26");
	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
		/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);

		char* Val = new char[8];
		INT32 ilConfigDate;
		ilConfigDate = getConfigDate();
		Val = _IntToAscii(ilConfigDate, Val, 10);
		strcpy(Val, ConvertToUpper(Val));
		AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), Val,
				false);
		delete[] Val;

	}

#if defined DEBUG	
	cout << "1F26 subidex added" << endl;
#endif
	/*  1F27*/
	strcpy(pbMNIndex, "1F27");
	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
		/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);

		char* Val = new char[50];
		INT32 ilConfigTime;
		ilConfigTime = getConfigTime();
		Val = _IntToAscii(ilConfigTime, Val, 10);
		strcpy(Val, ConvertToUpper(Val));

		AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), Val,
				false);
		delete[] Val;
	}

	/* 1F84*/
	strcpy(pbMNIndex, "1F84");
	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
	if (OCFM_ERR_SUCCESS == retCode.code)
	{
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pbMNIndex);
		/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);

		AddForEachSIdx(pbMNIndex, pobjIdxCol, pobjNode->getNodeId(), (char*) "",
				true);

	}

	delete[] pbMNIndex;
	delete[] pbSidx;
}

/*****************************************************************************/
/**
 \brief		UpdatedCNDateORTime

 This function updates the CN's Date or Time in the MN's corresponding CN's node id subindex

 \param		pobjMNIndex		Class pointer of the class CIndex
 \param		iNodeId			Integer variable to hold the Node Id of a node
 \param		enumDT			Enum of type EDateTime to hold either DATE or TIME to updated

 \return	void
 */
/*****************************************************************************/

void UpdatedCNDateORTime(CIndex* pobjMNIndex, INT32 iNodeId, EDateTime enumDT)
{
	if (NULL == pobjMNIndex)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	else
	{
		//nothing to do
	}

	CSubIndex *pobjSIdx = NULL;
	char Index[INDEX_LEN];
	INT32 IndexPos;

	CIndexCollection* pobjIdxCol = NULL;
	CNodeCollection* pobjNodeCol = NULL;
	CNode* pobjNode = NULL;
	char* subIndexHexValue = new char[3 + ALLOC_BUFFER];

	pobjNodeCol = CNodeCollection::getNodeColObjectPointer();
	pobjNode = pobjNodeCol->getNodePtr(CN, iNodeId);
	pobjIdxCol = pobjNode->getIndexCollection();

	subIndexHexValue = _IntToAscii(iNodeId, subIndexHexValue, 16);
	subIndexHexValue = padLeft(subIndexHexValue, '0', 2);
	pobjSIdx = pobjMNIndex->getSubIndexbyIndexValue(subIndexHexValue);
	delete[] subIndexHexValue;
	strcpy(Index, "1020");

	if (OCFM_ERR_SUCCESS
			!= (IfIndexExists(pobjNode->getNodeId(), CN, Index, &IndexPos)).code)
	{
		return;
	}
	//pobjSIdx->getActualValue() convert to upper
	if (pobjSIdx != NULL)
	{
		if (pobjSIdx->getActualValue() != NULL)
		{
			char Sidx[SUBINDEX_LEN];
			if (enumDT == DATE)
			{
				strcpy(Sidx, "01");
				SetSIdxValue(Index, Sidx, (char*) pobjSIdx->getActualValue(),
						pobjIdxCol, pobjNode->getNodeId(), CN, false);
			}
			else if (enumDT == TIME)
			{
				strcpy(Sidx, "02");
				SetSIdxValue(Index, Sidx, (char*) pobjSIdx->getActualValue(),
						pobjIdxCol, pobjNode->getNodeId(), CN, false);
			}
			else
			{
				//TODO: 'else' added. operation to be specified
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		copyPDODefToAct

 Copy pdos (16xx, 1Axx) default value to actual value

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node

 \return	void
 */
/*****************************************************************************/

void copyPDODefToAct(INT32 iNodeID, ENodeType enumNodeType)
{

	CSubIndex* pobjSIndex = NULL;
	CIndexCollection* pobjIdxCol = NULL;

	CNode *pobjNode = NULL;
	CNodeCollection *objNodeCollection = NULL;

	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
	pobjIdxCol = pobjNode->getIndexCollection();
	CIndex* pobjIndex = NULL;
	for (INT32 iIndexLoopCount = 0;
			iIndexLoopCount < pobjIdxCol->getNumberofIndexes();
			iIndexLoopCount++)
	{
		//	CIndex* pobjIndex;

		pobjIndex = pobjIdxCol->getIndex(iIndexLoopCount);

		if (!(CheckIfNotPDO((char*) pobjIndex->getIndexValue())))
		{

			for (INT32 iSIdxLoopCount = 0;
					iSIdxLoopCount < pobjIndex->getNumberofSubIndexes();
					iSIdxLoopCount++)
			{
				pobjSIndex = pobjIndex->getSubIndex(iSIdxLoopCount);
				if (pobjSIndex->getActualValue() == NULL)
				{
					if (pobjSIndex->getDefaultValue() != NULL)
					{
						pobjSIndex->setActualValue(
								(char*) pobjSIndex->getDefaultValue());
					}

				}
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		getPDOIndexByOffset

 This function returns the CIndex after sorting the collection by offset value

 \param		pobjIndex	Class pointer of the class CIndex

 \return	CIndex
 */
/*****************************************************************************/

CIndex getPDOIndexByOffset(CIndex* pobjIndex)
{
	if (NULL == pobjIndex)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	else
	{
		//nothing to do
	}

	CIndex objIndex;
	objIndex = *pobjIndex;
	CSubIndex* pObjSIdx1 = NULL;
	CSubIndex* pObjSIdx2 = NULL;

	for (INT32 iIndexLoopCount = 1;
			iIndexLoopCount <= (objIndex.getNumberofSubIndexes() - 1);
			iIndexLoopCount++)
	{
		for (INT32 iIdxLoopCount = 1;
				iIdxLoopCount
						<= (objIndex.getNumberofSubIndexes() - 1
								- iIndexLoopCount); iIdxLoopCount++)
		{

			pObjSIdx1 = objIndex.getSubIndex(iIdxLoopCount);
			if ((pObjSIdx1->getActualValue() != NULL)
					&& (0 != strcmp(pObjSIdx1->getActualValue(), ""))
					&& !(checkIfValueZero((char*) pObjSIdx1->getActualValue())))
			{
				const char* pbActualVal1 = pObjSIdx1->getActualValue();

				INT32 iLength1 = strlen(pbActualVal1);
				char* pbOffset1 = NULL;
				INT32 iOffset1 = 0;

				pbOffset1 = subString((char*) pbActualVal1, iLength1 - 12, 4);
				iOffset1 = hex2int(pbOffset1);
				pObjSIdx2 = objIndex.getSubIndex(iIdxLoopCount + 1);
				if ((NULL != pObjSIdx2->getActualValue())
						&& (0 != strcmp(pObjSIdx2->getActualValue(), ""))
						&& !(checkIfValueZero(
								(char*) pObjSIdx2->getActualValue())))
				{
					const char* pbActualVal2 = pObjSIdx2->getActualValue();
					INT32 iLength2 = strlen(pbActualVal2);
					char* pbOffset2 = NULL;
					INT32 iOffset2 = 0;

					pbOffset2 = subString((char*) pbActualVal2, iLength2 - 12,
							4);
					iOffset2 = hex2int(pbOffset2);

					if (iOffset1 > iOffset2)
					{
						objIndex.SwapSubObjects(iIdxLoopCount,
								iIdxLoopCount + 1);

					}
				}
			}
		}
	}

	return objIndex;
}

/*****************************************************************************/
/**
 \brief		GetFeatureValue

 This API shall be used to get the specific network management feature value for the specified node

 \param		iNodeID				Integer variable to hold the Node Id of a node
 \param		eNodeType			Enum to hold the Node type of the node
 \param		eFeatureType		Enum to hold the Feature type of the node
 \param		FeatureName			Character pointer to hold the Feature name
 \param		Out_FeatureValue	Character pointer to get the feature value

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetFeatureValue(INT32 iNodeID, ENodeType eNodeType,
		EFeatureType eFeatureType, char* FeatureName, char* Out_FeatureValue)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	INT32 iNodePos;

	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeID, eNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{
			//continue with process
		}
		else
		{
			// Node Doesn't Exist
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}

		CNode objNode;
		CNodeCollection *pobjNodeCollection;
		CNetworkManagement *pobjNwManagement;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		objNode = pobjNodeCollection->getNode(iNodeID);

		pobjNwManagement = objNode.getNetworkManagement();
		strcpy(Out_FeatureValue,
				pobjNwManagement->getFeatureValue(eFeatureType, FeatureName));

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		UpdateNodeParams

 This API shall be used to update all the attributes of a node including the nodeid.

 \param		iCurrNodeId				Integer variable to hold the current node id of a node
 \param		iNewNodeID				Integer variable to hold the new node id to be set to the node
 \param		eNodeType				Enum to hold the node type of the node
 \param		NodeName				Character pointer to hold the Node Name
 \param		eStationType			Enum to hold the station type of the node
 \param		ForcedCycle				Character pointer to hold the ForcedCycle value
 \param		ForcedCycleFlag			Boolean to hold the Forced Cycle Flag of the node
 \param		PollResponseTimeout		Character pointer to hold the Poll Response Timeout value for the node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID,
		ENodeType eNodeType, char* NodeName, EStationType eStationType,
		char* ForcedCycle, bool ForcedCycleFlag, char* PollResponseTimeout)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	INT32 iNodePos;
	try
	{
		bool bFlag = false;

		if (iNewNodeID == iCurrNodeId)
		{
			stErrorInfo = IfNodeExists(iNewNodeID, eNodeType, &iNodePos, bFlag);
			if (stErrorInfo.code == 0 && bFlag == true)
			{

			}
			else
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_INVALID_NODEID);
				throw objException;
			}

		}
		else if (iNewNodeID != iCurrNodeId)
		{
			stErrorInfo = IfNodeExists(iNewNodeID, eNodeType, &iNodePos, bFlag);
			if (stErrorInfo.code == OCFM_ERR_SUCCESS && bFlag == true)
			{
				ocfmException objException;
				objException.ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);
				throw objException;
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

		CNode* pobjNode;
		CNodeCollection *pobjNodeCollection;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		pobjNode = pobjNodeCollection->getNodePtr(eNodeType, iCurrNodeId);

		if (eNodeType == CN)
		{
			if (iNewNodeID != iCurrNodeId)
			{
				pobjNode->setNodeId(iNewNodeID);
				CopyOldNodeIdAssignmentObject(pobjNode, iCurrNodeId);
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

		if ((eStationType != MULTIPLEXED)
				|| ((iNewNodeID != iCurrNodeId) && (eStationType == MULTIPLEXED)))
		{
			char* cSIdx = new char[SUBINDEX_LEN];
			cSIdx = _IntToAscii(iCurrNodeId, cSIdx, 16);
			cSIdx = padLeft(cSIdx, '0', 2);
			pobjNode->resetForcedCycleValue();
			delete[] cSIdx;
		}
		else
		{
			//TODO: else added. operation to be added
		}

		if ((eNodeType == CN)
				&& !((PollResponseTimeout == NULL)
						|| (strcmp(PollResponseTimeout, "") == 0)))
		{
			pobjNode->setPollResponseTimeout(PollResponseTimeout);
		}
		else
		{
			//TODO: else added. operation to be added
		}
		pobjNode->setNodeName(NodeName);

		if (eNodeType == CN)
		{
			EStationType eNodePrevStationType = pobjNode->getStationType();
			pobjNode->setStationType(eStationType);
			if ((eNodePrevStationType == MULTIPLEXED)
					&& (eStationType != eNodePrevStationType))
			{
				CheckAndReAssignMultiplex(iNewNodeID,
						pobjNode->getForcedCycle());
			}
			else
			{
				//TODO: else added. operation to be added
			}

			if (eStationType != MULTIPLEXED)
			{
				pobjNode->resetForcedCycleValue();
			}
			else
			{
				//TODO: 'else' added. Operations to be specified.
			}

			if (eStationType == MULTIPLEXED)
			{
				bool bCalcForceCycle = true;
				bool bSetForceCycle = true;
				if (ForcedCycleFlag == true)
				{
					bCalcForceCycle = false;
				}
				else
				{
					//TODO: else added. operation to be added
				}

				if ((ForcedCycle == NULL || strcmp(ForcedCycle, "") == 0)
						&& ForcedCycleFlag == false)
				{
					if (pobjNode->getForceCycleFlag() == true)
					{

					}
					else if (pobjNode->getForcedCycle() == NULL
							|| strcmp(pobjNode->getForcedCycle(), "") == 0)
					{

					}
					else
					{

						INT32 iCNActualValue = 0;
						if (strncmp(pobjNode->getForcedCycle(), "0x", 2) == 0
								|| strncmp(pobjNode->getForcedCycle(), "0X", 2)
										== 0)
						{
							iCNActualValue = hex2int(
									subString(pobjNode->getForcedCycle(), 2,
											strlen(pobjNode->getForcedCycle())
													- 2));
						}
						else
						{
							iCNActualValue = atoi(pobjNode->getForcedCycle());
						}

						if (true
								== IsMultiplexCycleNumberContinuous(
										iCNActualValue))
						{
							bCalcForceCycle = false;
							bSetForceCycle = false;
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

				if (bCalcForceCycle == true)
				{
					ForcedCycle = getLastAvailableCycleNumber();
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				if (bSetForceCycle == true)
				{
					stErrorInfo = pobjNode->setForcedCycle(ForcedCycle);
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				if (OCFM_ERR_SUCCESS != stErrorInfo.code)
				{
					return stErrorInfo;
				}
				else
				{
					//TODO: 'else' added. operation to be specified.
				}

				pobjNode->setForceCycleFlag(ForcedCycleFlag);
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

		stErrorInfo.code = OCFM_ERR_SUCCESS;

	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		GetNodeDataTypes

 This API shall be used to get all the datatypes available for the particular node.

 \param		iNodeId				Integer variable to hold the Node Id of a node
 \param		eNodeType			Enum to hold the Node type of the node
 \param		pbOutDataTypes		Character pointer to hold all the datatypes available for the specifed node

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode GetNodeDataTypes(INT32 iNodeId, ENodeType eNodeType,
		char* pbOutDataTypes)
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.errorString = NULL;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	INT32 iNodePos;
	try
	{
		bool bFlag = false;
		stErrorInfo = IfNodeExists(iNodeId, eNodeType, &iNodePos, bFlag);
		if (stErrorInfo.code == 0 && bFlag == true)
		{

		}
		else
		{
			// Node Doesn't Exist
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_NODE_ALREADY_EXISTS);
			throw objException;
		}

		CNode* pobjNode;
		CNodeCollection *pobjNodeCollection;
		CDataTypeCollection *pobjDtCol;

		pobjNodeCollection = CNodeCollection::getNodeColObjectPointer();
		pobjNode = pobjNodeCollection->getNodePtr(eNodeType, iNodeId);
		pobjDtCol = pobjNode->getDataTypeCollection();

		for (UINT16 uiLoopCount = 0;
				uiLoopCount < pobjDtCol->getNumberOfDataTypes(); uiLoopCount++)
		{
			DataType *dt = NULL;
			char* strdtName = NULL;

			dt = pobjDtCol->getDataTypeElement(uiLoopCount);
			strdtName = new char[strlen(dt->getName()) + STR_ALLOC_BUFFER];

			strcpy(strdtName, dt->getName());
			if (uiLoopCount == 0)
			{
				strcpy(pbOutDataTypes, strdtName);
			}
			else
			{
				strcat(pbOutDataTypes, strdtName);

			}

			strcat(pbOutDataTypes, "\n");
			delete[] strdtName;
		}

		stErrorInfo.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		setNodeAssigmentBits

 Returns the 1F81 object data depending upon the condition (if Multiplex set bit 8, if chained set bit 14)

 \param		pobjNode	Class pointer of the class CNode

 \return	char*
 */
/*****************************************************************************/

char* setNodeAssigmentBits(CNode* pobjNode)
{
	if (NULL == pobjNode)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char* pb1F81Data = NULL;
	ULONG ulValue;
	pb1F81Data = new char[8 + STR_ALLOC_BUFFER];
	ulValue = EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS
			| EPL_NODEASSIGN_NODE_IS_CN | EPL_NODEASSIGN_START_CN;
	switch (pobjNode->getStationType())
	{
	case NORMAL:
		break;

	case MULTIPLEXED:
		ulValue = ulValue | EPL_NODEASSIGN_MULTIPLEXED_CN;
		break;

	case CHAINED:
		ulValue = ulValue | EPL_NODEASSIGN_CHAINED_CN;
		break;

	default:
		cout << "Invalid station Type:" << pobjNode->getStationType() << endl;
		break;
	};

	pb1F81Data = _IntToAscii(ulValue, pb1F81Data, 16);
	return pb1F81Data;
}

/*****************************************************************************/
/**
 \brief		setPresMNNodeAssigmentBits

 sets Pres MN Node Assigment Bits for 1F81 index depending upon the condition (if Multiplex set bit 8, if chained set bit 14)

 \return	void*
 */
/*****************************************************************************/

void setPresMNNodeAssigmentBits()
{
	INT32 IndexPos;
	INT32 subIndexPos;
	ocfmRetCode stErrStructInfo;
	stErrStructInfo = IfSubIndexExists(MN_NODEID, MN, (char *) "1F81",
			(char *) "F0", &subIndexPos, &IndexPos);

	if (OCFM_ERR_SUCCESS != stErrStructInfo.code)
	{
		return;
	}

	bool bIsPresMN = false;
	bool bIsMNBroadcastingPRes = false;

	bIsPresMN = IsPresMN();

	//check whether Pres activated in MN
	CNodeCollection* pobjNodeCol;
	CNode objMNNode;
	CIndexCollection* objPDOCollection;
	CIndexCollection* pobjIndexCollection;

	pobjNodeCol = CNodeCollection::getNodeColObjectPointer();
	objMNNode = pobjNodeCol->getMNNode();
	INT32 tpdoCount = 0;
	INT32 rpdoCount = 0;
	objPDOCollection = objMNNode.getPDOIndexCollection(&rpdoCount, &tpdoCount);
	if (objPDOCollection != NULL)
	{
		pobjIndexCollection = objMNNode.getIndexCollection();
		CIndex* pobjIndex;
		for (INT32 iLoopCount = 0;
				iLoopCount < objPDOCollection->getNumberofIndexes();
				iLoopCount++)
		{
			pobjIndex = objPDOCollection->getIndex(iLoopCount);
			if (0 == strncmp(pobjIndex->getIndexValue(), "1A", 2))
			{
				CIndex *pobjCommIndex = NULL;

				char *pcIdx = subString((char *) pobjIndex->getIndexValue(), 2,
						4);
				char *pcCommIdx = new char[INDEX_LEN];
				strcpy(pcCommIdx, (char *) "18");
				strcat(pcCommIdx, pcIdx);
				pobjCommIndex = pobjIndexCollection->getIndexbyIndexValue(
						pcCommIdx);
				if (NULL != pobjCommIndex)
				{
					CSubIndex *pobjNodeIDSubIndex = NULL;
					pobjNodeIDSubIndex = pobjCommIndex->getSubIndexbyIndexValue(
							(char *) "01");
					if (NULL != pobjNodeIDSubIndex)
					{
						if ((NULL != pobjNodeIDSubIndex->getActualValue())
								&& (0
										!= strcmp(
												pobjNodeIDSubIndex->getActualValue(),
												"")))
						{
							INT32 iNodeTPDOMappedNodeID = 0;
							iNodeTPDOMappedNodeID =
									GetDecimalValue(
											(char *) pobjNodeIDSubIndex->getActualValue());
							if (BROADCAST_NODEID == iNodeTPDOMappedNodeID)
							{
								bIsMNBroadcastingPRes = true;
								break;
							}
						}
					}
				}
				delete[] pcCommIdx;
				delete[] pcIdx;
			}
		}
	}

	//check whether MN is transmitting PRes
	if ((false == bIsPresMN) && (false == bIsMNBroadcastingPRes))
	{
		return;
	}

	CSubIndex* pobjSubindex;
	pobjSubindex = getMNSubIndexValues((char*) "1F81", (char*) "F0");
	if (NULL != pobjSubindex)
	{
		if ((true == bIsPresMN) || (true == bIsMNBroadcastingPRes))
		{
			char* pb1F81Data = NULL;
			ULONG ulValue;
			pb1F81Data = new char[8 + STR_ALLOC_BUFFER];
			ulValue = EPL_NODEASSIGN_VALID | EPL_NODEASSIGN_NODE_EXISTS
					| EPL_NODEASSIGN_MN_PRES;
			strcpy(pb1F81Data, (char *) "0x");
			_IntToAscii(ulValue, &pb1F81Data[2], 16);

			pobjSubindex->setActualValue(pb1F81Data);
			pobjSubindex->setFlagIfIncludedCdc(TRUE);
			delete[] pb1F81Data;
		}
		else
		{
			// no operation
		}
	}

}

/*****************************************************************************/
/**
 \brief		RecalculateMultiplex

 Recalculates the multiplex cycle for CNs

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode RecalculateMultiplex()
{
	ocfmRetCode stErrorInfo;
	stErrorInfo.code = OCFM_ERR_UNKNOWN;
	stErrorInfo.errorString = NULL;
	CNodeCollection *objNodeCol = NULL;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	//INT32 iNodesCount = 0;
	//iNodesCount = objNodeCol->getCNNodesCount();

	CNode* objNode = NULL;

	try
	{
		if (false == CheckIfMultiplexedCNExist())
		{
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			return stErrorInfo;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		INT32 IndexPos = 0;
		INT32 subIndexPos = 0;
		stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &IndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			//reset automatically assigned cn force cycle
			ResetMultiplexedCNForceCycle();
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			delete[] actValue;
			return stErrorInfo;
		}
		stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			ocfmException objException;
			objException.ocfm_Excpetion(stErrorInfo.code);
			delete[] actValue;
			throw objException;
		}

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN)
			{
				ocfmRetCode stErrorInfoSubindexMultiplCycl;
				static char* strConvertedValue = NULL;
				if (strConvertedValue != NULL)
				{
					delete[] strConvertedValue;
					strConvertedValue = NULL;
				}
				strConvertedValue = new char[SUBINDEX_LEN];
				char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
				strConvertedValue = _IntToAscii(objNode->getNodeId(),
						strConvertedValue, 16);
				strConvertedValue = padLeft(strConvertedValue, '0', 2);
				char* subIndActValue = new char[20];
				subIndActValue[0] = 0;
				try
				{
					stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(
							MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, ACTUALVALUE, subIndActValue);
				} catch (...)
				{
					delete[] subIndActValue;
					continue;
				}
				if (stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
				{
				}
				else
				{
					continue;
				}
				if (objNode->getStationType() == MULTIPLEXED)
				{
					if (objNode->getForceCycleFlag() == 1)
					{
						continue;
					}
					else if (objNode->getForcedCycle() == NULL
							|| strcmp(objNode->getForcedCycle(), "") == 0)
					{
						//continue down the loop
					}
					else if (actValue == NULL || strcmp(actValue, "") == 0)
					{
						char* subIndName = new char[50];
						subIndName[0] = 0;
						GetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, strConvertedValue, NAME,
								subIndName);
						char* subIndFlag = new char[10];
						GetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, strConvertedValue,
								FLAGIFINCDC, subIndFlag);
						INT32 iCNsubIndFlag = 0;
						iCNsubIndFlag = atoi(subIndFlag);
						SetSubIndexAttributes(MN_NODEID, MN,
								acMultiCycleAssignObj, strConvertedValue,
								(char*) "", subIndName, (EFlag) iCNsubIndFlag);
						delete[] subIndName;
						delete[] subIndFlag;
						continue;
					}
					else
					{
						if (stErrorInfoSubindexMultiplCycl.code
								== OCFM_ERR_SUCCESS)
						{
							INT32 iMNMultiActualValue = 0;
							if (strncmp(actValue, "0x", 2) == 0
									|| strncmp(actValue, "0X", 2) == 0)
								iMNMultiActualValue = hex2int(
										subString(actValue, 2,
												strlen(actValue) - 2));
							else
								iMNMultiActualValue = atoi(actValue);

							char* ForcedCycleValue = new char[50];
							strcpy(ForcedCycleValue, objNode->getForcedCycle());
							INT32 iCNActualValue = 0;
							if (strncmp(ForcedCycleValue, "0x", 2) == 0
									|| strncmp(ForcedCycleValue, "0X", 2) == 0)
								iCNActualValue = hex2int(
										subString(ForcedCycleValue, 2,
												strlen(ForcedCycleValue) - 2));
							else
								iCNActualValue = atoi(ForcedCycleValue);

							if (iMNMultiActualValue == 0)
							{

								char* subIndName = new char[50];
								subIndName[0] = 0;
								GetSubIndexAttributes(MN_NODEID, MN,
										acMultiCycleAssignObj,
										strConvertedValue, NAME, subIndName);
								char* subIndFlag = new char[10];
								GetSubIndexAttributes(MN_NODEID, MN,
										acMultiCycleAssignObj,
										strConvertedValue, FLAGIFINCDC,
										subIndFlag);
								INT32 iCNsubIndFlag = 0;
								iCNsubIndFlag = atoi(subIndFlag);
								SetSubIndexAttributes(MN_NODEID, MN,
										acMultiCycleAssignObj,
										strConvertedValue, (char*) "",
										subIndName, (EFlag) iCNsubIndFlag);
								delete[] subIndName;
								delete[] subIndFlag;
								continue;
							}
							else if (iCNActualValue <= iMNMultiActualValue)
							{
								continue;
							}
							else
							{
								//continue the function
							}
							delete[] ForcedCycleValue;
						}
						else
						{

						}
					}

					char* ForcedCycle = getLastAvailableCycleNumber();
					objNode->setForcedCycle(ForcedCycle);
					delete[] ForcedCycle;
				}
				else
				{
					// station other than multiplexed
					char* subIndName = new char[50];
					subIndName[0] = 0;
					GetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, NAME, subIndName);

					char* subIndFlag = new char[10];
					GetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, FLAGIFINCDC, subIndFlag);

					INT32 iCNsubIndFlag = 0;
					iCNsubIndFlag = atoi(subIndFlag);
					SetSubIndexAttributes(MN_NODEID, MN, acMultiCycleAssignObj,
							strConvertedValue, (char*) "", subIndName,
							(EFlag) iCNsubIndFlag);
					delete[] subIndName;
					delete[] subIndFlag;
				}
				delete[] strConvertedValue;
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
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		ResetMultiplexedCNForceCycle

 This function resets the Multiplexed CN's forced cycle value

 \return	void
 */
/*****************************************************************************/

void ResetMultiplexedCNForceCycle()
{
	CNodeCollection *objNodeCol = NULL;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode* objNode = NULL;

	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
			iLoopCount++)
	{
		objNode = objNodeCol->getNodebyColIndex(iLoopCount);
		if (objNode->getNodeType() == CN)
		{
			ocfmRetCode stErrorInfoSubindexMultiplCycl;
			static char* strConvertedValue = NULL;
			if (strConvertedValue != NULL)
			{
				delete[] strConvertedValue;
				strConvertedValue = NULL;
			}
			strConvertedValue = new char[SUBINDEX_LEN];
			char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
			strConvertedValue = _IntToAscii(objNode->getNodeId(),
					strConvertedValue, 16);
			strConvertedValue = padLeft(strConvertedValue, '0', 2);
			char* subIndActValue = new char[20];
			subIndActValue[0] = 0;
			try
			{
				stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(
						MN_NODEID, MN, acMultiCycleAssignObj, strConvertedValue,
						ACTUALVALUE, subIndActValue);
			} catch (...)
			{
				delete[] subIndActValue;
				continue;
			}
			if (stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
			{
			}
			else
			{
				continue;
			}
			if (objNode->getStationType() == MULTIPLEXED)
			{
				if (objNode->getForceCycleFlag() == 1)
				{
					continue;
				}
				objNode->resetForcedCycleValue();
				objNode->setStationType(NORMAL);
			}
			delete[] strConvertedValue;
		}
	}
}

/*****************************************************************************/
/**
 \brief		copyMNPropDefToAct

 This function copies the (1006, 1F8A/02, 1F98/07, 1F98/08) indexes default value to actual value

 \param		iNodeID			Integer variable to hold the node id
 \param		enumNodeType	Enum of type ENodeType to hold the Node type

 \return	void
 */
/*****************************************************************************/

void copyMNPropDefToAct(INT32 iNodeID, ENodeType enumNodeType)
{
	if (enumNodeType != MN)
	{
		return;
	}
	copyIndexDefToAct(iNodeID, enumNodeType, (char *) "1006");
	copySubIndexDefToAct(iNodeID, enumNodeType, false, (char *) "1F8A",
			(char *) "02");
	copySubIndexDefToAct(iNodeID, enumNodeType, false, (char *) "1F98",
			(char *) "07");
	copySubIndexDefToAct(iNodeID, enumNodeType, false, (char *) "1F98",
			(char *) "08");
}

/*****************************************************************************/
/**
 \brief		copyIndexDefToAct

 This function copies the index's default value to the actual value.

 \param		iNodeID			Integer variable to hold the node id
 \param		enumNodeType	Enum of type ENodeType to hold the Node type
 \param		indexId			Character pointer to hold the Index Id

 \return	void
 */
/*****************************************************************************/

void copyIndexDefToAct(INT32 iNodeID, ENodeType enumNodeType, char *indexId)
{
	CIndexCollection* pobjIdxCol = NULL;

	CNode *pobjNode = NULL;
	CNodeCollection *objNodeCollection = NULL;

	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
	pobjIdxCol = pobjNode->getIndexCollection();

	CIndex* pobjIndex = NULL;

	pobjIndex = pobjIdxCol->getIndexbyIndexValue(indexId);
	if (pobjIndex == NULL)
	{
		return;
	}
	if (pobjIndex->getActualValue() == NULL)
	{
		if (pobjIndex->getDefaultValue() != NULL)
		{
			pobjIndex->setActualValue((char*) pobjIndex->getDefaultValue());
		}
	}
}

/*****************************************************************************/
/**
 \brief		copySubIndexDefToAct

 This function copies the subindexes default value to the actual value. If for copy is set empty values will be copied while the default value is not configured.

 \param		iNodeID			Integer variable to hold the node id
 \param		enumNodeType	Enum of type ENodeType to hold the Node type
 \param		bForce			Boolean value to set force copy or not
 \param		indexId			Character pointer to hold the Index Id
 \param		subIndexId		Character pointer to hold the subIndex Id

 \return	void
 */
/*****************************************************************************/

void copySubIndexDefToAct(INT32 iNodeID, ENodeType enumNodeType, bool bForce,
		char *indexId, char *subIndexId)
{
	CSubIndex* pobjSIndex = NULL;
	CIndexCollection* pobjIdxCol = NULL;

	CNode *pobjNode = NULL;
	CNodeCollection *objNodeCollection = NULL;

	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
	pobjIdxCol = pobjNode->getIndexCollection();

	CIndex* pobjIndex = NULL;

	pobjIndex = pobjIdxCol->getIndexbyIndexValue(indexId);
	if (pobjIndex == NULL)
	{
		return;
	}

	pobjSIndex = pobjIndex->getSubIndexbyIndexValue(subIndexId);
	if (pobjSIndex == NULL)
	{
		return;
	}
	if ((pobjSIndex->getActualValue() == NULL) || (true == bForce))
	{
		if (pobjSIndex->getDefaultValue() != NULL)
		{
			pobjSIndex->setActualValue((char*) pobjSIndex->getDefaultValue());
		}
		else
		{
			if (true == bForce)
			{
				pobjSIndex->setActualValue((char*) "");
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		CheckAndReAssignMultiplex

 This function checks and re-assigns the Multiplexd cycle value

 \param		iCNNodeId		Integer variable to hold the node id of the CN
 \param		CycleValue		Character pointer to hold the Multiplexd cycle value

 \return	void
 */
/*****************************************************************************/

void CheckAndReAssignMultiplex(INT32 iCNNodeId, char* CycleValue)
{
	ocfmRetCode stErrorInfo;
	if (CycleValue == NULL || strcmp(CycleValue, "") == 0)
	{
		stErrorInfo.code = OCFM_ERR_SUCCESS;
		//return stErrorInfo;
		return;
	}

	INT32 iCycleValue = 0;
	if (strncmp(CycleValue, "0x", 2) == 0 || strncmp(CycleValue, "0X", 2) == 0)
		iCycleValue = hex2int(subString(CycleValue, 2, strlen(CycleValue) - 2));
	else
		iCycleValue = atoi(CycleValue);

	CNodeCollection *objNodeCol;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->getCNNodesCount();

	CNode* objNode;

	try
	{
		if (iNodesCount == 0)
		{
			return;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		try
		{

			stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
					(char*) "07", ACTUALVALUE, actValue);
		} catch (...)
		{
			delete[] actValue;
			stErrorInfo.code = OCFM_ERR_SUCCESS;
			return;
		}

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN
					&& objNode->getNodeId() != iCNNodeId
					&& objNode->getStationType() == MULTIPLEXED)
			{
				if (objNode->getForcedCycle() == NULL
						|| strcmp(objNode->getForcedCycle(), "") == 0)
				{
					continue;
				}
				else
				{
					char* ForcedCycleValue = new char[strlen(
							objNode->getForcedCycle()) + ALLOC_BUFFER];
					strcpy(ForcedCycleValue, objNode->getForcedCycle());
					INT32 iCNActualValue = 0;
					if (strncmp(ForcedCycleValue, "0x", 2) == 0
							|| strncmp(ForcedCycleValue, "0X", 2) == 0)
						iCNActualValue = hex2int(
								subString(ForcedCycleValue, 2,
										strlen(ForcedCycleValue) - 2));
					else
						iCNActualValue = atoi(ForcedCycleValue);

					delete[] ForcedCycleValue;
					if (iCycleValue == iCNActualValue)
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
		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN
					&& objNode->getNodeId() != iCNNodeId
					&& objNode->getStationType() == MULTIPLEXED
					&& objNode->getForceCycleFlag() == false)
			{
				if (objNode->getForcedCycle() == NULL
						|| strcmp(objNode->getForcedCycle(), "") == 0)
				{
					continue;
				}
				else
				{
				}

				char* ForcedCycleValue = new char[strlen(
						objNode->getForcedCycle()) + ALLOC_BUFFER];
				strcpy(ForcedCycleValue, objNode->getForcedCycle());
				INT32 iCNActualValue = 0;
				if (strncmp(ForcedCycleValue, "0x", 2) == 0
						|| strncmp(ForcedCycleValue, "0X", 2) == 0)
					iCNActualValue = hex2int(
							subString(ForcedCycleValue, 2,
									strlen(ForcedCycleValue) - 2));
				else
					iCNActualValue = atoi(ForcedCycleValue);

				delete[] ForcedCycleValue;
				if (iCycleValue >= iCNActualValue || iCNActualValue == 1)
				{
					continue;
				}
				else
				{
					//continue the function
				}
				iCNActualValue--;
				uiCycleNumber = iCNActualValue;

				static char* strCNActualValue = NULL;
				if (strCNActualValue != NULL)
				{
					delete[] strCNActualValue;
					strCNActualValue = NULL;
				}
				strCNActualValue = new char[50];
				strCNActualValue = _IntToAscii(iCNActualValue, strCNActualValue,
						10);

				objNode->setForcedCycle(strCNActualValue);

				delete[] strCNActualValue;
				delete[] ForcedCycleValue;
			} //end of if loop
		} //end of for loop
		delete[] actValue;
	} //end of try
	catch (...)
	{
		return;
	}
}

/*****************************************************************************/
/**
 \brief		CheckMutliplexAssigned

 This function checks for the multiplexed (1F98/07) assigned or not

 \return	ocfmRetCode
 */
/*****************************************************************************/

ocfmRetCode CheckMutliplexAssigned()
{
	ocfmRetCode stErrorInfo;
	CNodeCollection *objNodeCol;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->getCNNodesCount();
	stErrorInfo.code = OCFM_ERR_SUCCESS;
	stErrorInfo.errorString = NULL;

	bool bErrorFlag = false;
	CNode* objNode;

	try
	{
		if (iNodesCount == 0)
		{
			stErrorInfo.code = OCFM_ERR_NO_CN_NODES_FOUND;
			return stErrorInfo;
		}
		if (false == CheckIfMultiplexedCNExist())
		{
			return stErrorInfo;
		}
		char* actValue = new char[50];
		actValue[0] = 0;
		INT32 subIndexPos = 0;
		INT32 IndexPos = 0;
		stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", &subIndexPos, &IndexPos);
		if (stErrorInfo.code == OCFM_ERR_SUCCESS)
		{
		}
		else
		{
			stErrorInfo.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
			CopyCustomErrorString(&stErrorInfo,
					(char*) "The Subindex 07 of Index 1F98 does not exist Multiplexing not supported");
			delete[] actValue;
			return stErrorInfo;
		}
		stErrorInfo = GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F98",
				(char*) "07", ACTUALVALUE, actValue);
		if (stErrorInfo.code != OCFM_ERR_SUCCESS)
		{
			delete[] actValue;
			return stErrorInfo;
		}

		stErrorInfo.errorString = new char[500];
		strcpy(stErrorInfo.errorString, "CN with nodeid ");
		INT32 iMNMultiActualValue = 0;
		if (strncmp(actValue, "0x", 2) == 0 || strncmp(actValue, "0X", 2) == 0)
			iMNMultiActualValue = hex2int(
					subString(actValue, 2, strlen(actValue) - 2));
		else
			iMNMultiActualValue = atoi(actValue);

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN)
			{
				if (objNode->getStationType() == MULTIPLEXED)
				{

					ocfmRetCode stErrorInfoSubindexMultiplCycl;
					static char* strConvertedValue = NULL;
					if (strConvertedValue != NULL)
					{
						delete[] strConvertedValue;
						strConvertedValue = NULL;
					}
					strConvertedValue = new char[SUBINDEX_LEN];
					char acMultiCycleAssignObj[] = MULTIPL_CYCLE_ASSIGN_OBJECT;
					strConvertedValue = _IntToAscii(objNode->getNodeId(),
							strConvertedValue, 16);
					strConvertedValue = padLeft(strConvertedValue, '0', 2);
					char* subIndActValue = new char[20];
					subIndActValue[0] = 0;
					try
					{
						stErrorInfoSubindexMultiplCycl = GetSubIndexAttributes(
								MN_NODEID, MN, acMultiCycleAssignObj,
								strConvertedValue, ACTUALVALUE, subIndActValue);
					} catch (...)
					{
						delete[] subIndActValue;
						//delete[] strConvertedValue;
						continue;
					}
					if (stErrorInfoSubindexMultiplCycl.code == OCFM_ERR_SUCCESS)
					{
					}
					else
					{
						continue;
					}

					if (objNode->getForcedCycle() == NULL
							|| strcmp(objNode->getForcedCycle(), "") == 0)
					{
						//continue down the loop
					}
					else if (actValue == NULL || strcmp(actValue, "") == 0)
					{
						continue;
					}
					else
					{
						char* ForcedCycleValue = new char[50];
						strcpy(ForcedCycleValue, objNode->getForcedCycle());
						INT32 iCNActualValue = 0;
						if (strncmp(ForcedCycleValue, "0x", 2) == 0
								|| strncmp(ForcedCycleValue, "0X", 2) == 0)
							iCNActualValue = hex2int(
									subString(ForcedCycleValue, 2,
											strlen(ForcedCycleValue) - 2));
						else
							iCNActualValue = atoi(ForcedCycleValue);

						if (iCNActualValue <= iMNMultiActualValue)
						{
							continue;
						}
						else
						{
							sprintf(stErrorInfo.errorString, "%s%d, ",
									stErrorInfo.errorString,
									objNode->getNodeId());
							bErrorFlag = true;
						}
					}
					delete[] strConvertedValue;
				}
				else
				{
				}
			} // end of if loop 1
		} //end of for loop
		if (bErrorFlag == true)
		{
			stErrorInfo.code = OCFM_ERR_MULTIPLEX_ASSIGN_ERROR;
			stErrorInfo.errorString[strlen(stErrorInfo.errorString) - 2] = 0;
			strcat(stErrorInfo.errorString,
					" multiplex cycle value exceeds the multiplex prescalar");
		}
		delete[] actValue;
	} //end of try
	catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return stErrorInfo;
}

/*****************************************************************************/
/**
 \brief		getFreeCycleNumber

 This function returns the cycle number which is avaliable

 \param		uiParmCycleNumber	Unsigned Integer variable to hold the maximum limit for the cycle number.

 \return	UINT32
 */
/*****************************************************************************/

UINT32 getFreeCycleNumber(UINT32 uiParmCycleNumber)
{
	ocfmRetCode stErrorInfo;
	CNodeCollection *objNodeCol;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->getCNNodesCount();

	CNode* objNode;

	if (iNodesCount == 0)
	{
		stErrorInfo.code = OCFM_ERR_NO_CN_NODES_FOUND;
		ocfmException objException; // = new ocfmException;
		objException.ocfm_Excpetion(stErrorInfo.code);
		return uiParmCycleNumber;
	}
	for (UINT32 uiCycleNumberCount = 1; uiCycleNumberCount < uiParmCycleNumber;
			uiCycleNumberCount++)
	{
		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
				iLoopCount++)
		{
			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN
					&& objNode->getStationType() == MULTIPLEXED)
			{
				if (objNode->getForcedCycle() == NULL
						|| strcmp(objNode->getForcedCycle(), "") == 0)
				{
					//continue;
				}
				else
				{
					char* ForcedCycleValue = new char[strlen(
							objNode->getForcedCycle()) + ALLOC_BUFFER];
					strcpy(ForcedCycleValue, objNode->getForcedCycle());
					UINT32 uiCNActualValue = 0;
					if (strncmp(ForcedCycleValue, "0x", 2) == 0
							|| strncmp(ForcedCycleValue, "0X", 2) == 0)
						uiCNActualValue = hex2int(
								subString(ForcedCycleValue, 2,
										strlen(ForcedCycleValue) - 2));
					else
						uiCNActualValue = atoi(ForcedCycleValue);

					delete[] ForcedCycleValue;
					if (uiCycleNumberCount == uiCNActualValue)
					{
						break;
					}
					else
					{
						//continue the function
					}
				}
			} // end of if loop 1
			if (iLoopCount == objNodeCol->getNumberOfNodes() - 1)
			{
				return uiCycleNumberCount;
			}
		} //end of for loop

	} //end of for loop
	return uiParmCycleNumber;
}

/*****************************************************************************/
/**
 \brief		IsMultiplexCycleNumberContinuous

 This function validates the Multiplexd Cycle Number is Continuous or not

 \param		uiParmCycleNumber	Unsigned Integer variable to hold the maximum limit for the cycle number.

 \return	BOOL

 \retval	TRUE		if continuous multiplexed cycle number is present
 \retval	FALSE		if multiplexed cycle number is not continuous
 */
/*****************************************************************************/

bool IsMultiplexCycleNumberContinuous(UINT32 uiParmCycleNumber)
{
	CNodeCollection *objNodeCol;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	INT32 iNodesCount = 0;
	iNodesCount = objNodeCol->getCNNodesCount();

	CNode* objNode;

	if (iNodesCount == 0)
	{
		return false;
	}
	for (UINT32 uiCycleNumberCount = 1; uiCycleNumberCount < uiParmCycleNumber;
			uiCycleNumberCount++)
	{

		for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
				iLoopCount++)
		{

			objNode = objNodeCol->getNodebyColIndex(iLoopCount);
			if (objNode->getNodeType() == CN
					&& objNode->getStationType() == MULTIPLEXED)
			{

				if (objNode->getForcedCycle() == NULL
						|| strcmp(objNode->getForcedCycle(), "") == 0)
				{
					//continue;
				}
				else
				{
					char* ForcedCycleValue = new char[strlen(
							objNode->getForcedCycle()) + ALLOC_BUFFER];
					strcpy(ForcedCycleValue, objNode->getForcedCycle());
					UINT32 uiCNActualValue = 0;
					if (strncmp(ForcedCycleValue, "0x", 2) == 0
							|| strncmp(ForcedCycleValue, "0X", 2) == 0)
						uiCNActualValue = hex2int(
								subString(ForcedCycleValue, 2,
										strlen(ForcedCycleValue) - 2));
					else
						uiCNActualValue = atoi(ForcedCycleValue);

					delete[] ForcedCycleValue;
					if (uiCycleNumberCount == uiCNActualValue)
					{
						break;
					}
					else
					{
						//continue the function
					}
				}
			} // end of if loop 1
			if (iLoopCount == objNodeCol->getNumberOfNodes() - 1)
			{
				return false;
			}
		} //end of for loop

	} //end of for loop
	return true;
}

/*****************************************************************************/
/**
 \brief		calculateCNPollResponse

 This function calculates the CN Poll Response time of the CN

 \param		iNodeID			Integer variable to hold the node id.
 \param		enumNodeType	Enum to hold the Node type

 \return	void
 */
/*****************************************************************************/

void calculateCNPollResponse(INT32 iNodeID, ENodeType enumNodeType)
{
	if (enumNodeType != CN)
	{
		return;
	}
	INT32 SubIndexPos;
	INT32 IndexPos;
	ocfmRetCode stErrorInfo;
	stErrorInfo = IfSubIndexExists(iNodeID, enumNodeType, (char*) "1F98",
			(char*) "03", &SubIndexPos, &IndexPos);
	if (stErrorInfo.code != OCFM_ERR_SUCCESS)
	{
		return;
	}

	CSubIndex* pobjSIndex = NULL;
	CIndexCollection* pobjIdxCol = NULL;

	CNode *pobjNode = NULL;
	CNodeCollection *objNodeCollection = NULL;

	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
	pobjIdxCol = pobjNode->getIndexCollection();

	CIndex* pobjIndex = NULL;

	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1F98");
	if (pobjIndex == NULL)
	{
		return;
	}

	pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*) "03");
	if (pobjSIndex == NULL)
	{
		return;
	}
	char *pcValue = NULL;
	INT32 iValue = 0;
	bool add25microsec = false;
	if (pobjSIndex->getActualValue() == NULL
			|| strcmp(pobjSIndex->getActualValue(), "") == 0)
	{
		if (pobjSIndex->getDefaultValue() == NULL
				|| strcmp(pobjSIndex->getDefaultValue(), "") == 0)
		{
			pcValue = new char[strlen("25000") + ALLOC_BUFFER];
			strcpy(pcValue, "25000");
		}
		else
		{
			pcValue = new char[strlen(pobjSIndex->getDefaultValue())
					+ ALLOC_BUFFER];
			strcpy(pcValue, pobjSIndex->getDefaultValue());
			add25microsec = true;
		}
	}
	else
	{
		pcValue = new char[strlen(pobjSIndex->getActualValue()) + ALLOC_BUFFER];
		strcpy(pcValue, pobjSIndex->getActualValue());
	}

	if (strncmp(pcValue, "0x", 2) == 0 || strncmp(pcValue, "0X", 2) == 0)
		iValue = hex2int(subString(pcValue, 2, strlen(pcValue) - 2));
	else
		iValue = atoi(pcValue);

	if (true == add25microsec)
		iValue += 25000;

	//add with 25 micro sec
	char *convValue = new char[30];

	convValue = _IntToAscii(iValue, convValue, 10);
	pobjNode->setPollResponseTimeout(convValue);
	delete[] pcValue;
	delete[] convValue;
}

/*****************************************************************************/
/**
 \brief		CopyCustomErrorString

 This function copies the custom error description to be thrown to the exception handler.

 \param		stRet				Struct pointer of type ocfmRetCode to hold the Errorcode & error string
 \param		pcCustomErrString	Character pointer to hold the custom error description

 \return	void
 */
/*****************************************************************************/

void CopyCustomErrorString(ocfmRetCode* stRet, char* pcCustomErrString)
{
	if ((NULL == stRet) || (NULL == pcCustomErrString))
	{
		return;
	}
	else
	{
		//Nothing to do
	}

	if (0 == strlen(pcCustomErrString))
	{
		return;
	}
	else
	{
		//Nothing to do
	}

	stRet->errorString = new char[strlen(pcCustomErrString) + ALLOC_BUFFER];
	strcpy(stRet->errorString, pcCustomErrString);
}

/*****************************************************************************/
/**
 \brief		CheckIfMultiplexedCNExist

 This function checks for the existence of a CN with muliplexed station type

 \return	BOOL

 \retval	TRUE			if Multiplexed CN Exist
 \retval	FALSE			if Multiplexed CN does not Exist
 */
/*****************************************************************************/
bool CheckIfMultiplexedCNExist()
{
	CNodeCollection *objNodeCol;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode* objNode;
	for (INT32 iLoopCount = 0; iLoopCount < objNodeCol->getNumberOfNodes();
			iLoopCount++)
	{
		objNode = objNodeCol->getNodebyColIndex(iLoopCount);
		if (objNode->getNodeType() == CN)
		{
			if (objNode->getStationType() == MULTIPLEXED)
			{
				return true;
			}
		}
	}
	return false;
}

/*****************************************************************************/
/**
 \brief			RecalculateCNPresTimeout

 \param			pbSubIndexId
 \return	void
 */
/*****************************************************************************/
//TODO: unused function
void RecalculateCNPresTimeout(char* pbSubIndexId)
{
	ocfmRetCode stErrorInfo;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F92", pbSubIndexId,
			&iSubIndexPos, &iIndexPos);
	if (stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		//continue with process
	}
	else
	{
		return;
	}
	char* pcMNIndValue = new char[50];
	GetSubIndexAttributes(MN_NODEID, MN, (char*) "1F92", pbSubIndexId,
			ACTUALVALUE, pcMNIndValue);

	if (NULL == pcMNIndValue)
	{
		return;
	}

	INT32 iCNNodeId = hex2int(pbSubIndexId);

	stErrorInfo = IfSubIndexExists(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			&iSubIndexPos, &iIndexPos);
	if (OCFM_ERR_SUCCESS != stErrorInfo.code)
	{
		delete[] pcMNIndValue;
		return;
	}

	char* strConvertedValue;
	strConvertedValue = new char[SUBINDEX_LEN];
	strcpy(strConvertedValue, pbSubIndexId);

	char* subIndName = new char[50];
	subIndName[0] = 0;
	INT32 iCNsubIndFlag = 0;

	GetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03", NAME,
			subIndName);
	char* subIndFlag = new char[10];
	GetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			FLAGIFINCDC, subIndFlag);
	iCNsubIndFlag = atoi(subIndFlag);

	SetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			pcMNIndValue, subIndName, (EFlag) iCNsubIndFlag);
	delete[] subIndName;
	delete[] subIndFlag;
	delete[] pcMNIndValue;
	delete[] strConvertedValue;

}

/*****************************************************************************/
/**
 \brief		UpdateMNNodeAssignmentIndex

This function updates the MN indexes & subindexes with the values corresponding to the cn node id

 \param		pobjNode			Class pointer of the class CNode
 \param		CNsCount			Integer variable to hold the total no.of CN's count
 \param		pcIndex				Character pointer to hold the IndexID
 \param		allowMNSubindex		Boolean value to allow or disallow the subindex in MN

 \return	void
 */
/*****************************************************************************/

//TODO: CNsCount is not used to be removed.
void UpdateMNNodeAssignmentIndex(CNode *pobjNode, INT32 CNsCount, char* pcIndex,
		bool allowMNSubindex)
{
	if ((NULL == pcIndex) || (NULL == pobjNode))
	{
		return;
	}

	CIndexCollection *pobjIdxCol = NULL;
	ocfmRetCode retCode;
	retCode.errorString = NULL;
	INT32 IndexPos;

	pobjIdxCol = pobjNode->getIndexCollection();
	char* pbMNIndex = new char[INDEX_LEN + ALLOC_BUFFER];

	strcpy(pbMNIndex, pcIndex);

	retCode = IfIndexExists(MN_NODEID, MN, pbMNIndex, &IndexPos);
	delete[] pbMNIndex;
	if (retCode.code == OCFM_ERR_SUCCESS)
	{
		CIndex *pobjIndex = NULL;
		pobjIndex = pobjIdxCol->getIndexbyIndexValue(pcIndex);
		/* $:set Flag to true*/
		pobjIndex->setFlagIfIncludedCdc(TRUE);
		CSubIndex* pobjSubIndex;
		for (INT32 iSidxCount = pobjIndex->getNumberofSubIndexes() - 1;
				iSidxCount >= 0; iSidxCount--)
		{
			pobjSubIndex = pobjIndex->getSubIndex(iSidxCount);
			if (NULL == pobjSubIndex)
				continue;

			if (0 == strcmp((char*) pobjSubIndex->getIndexValue(), "00"))
				continue;

			try
			{
				INT32 iNodeidValue = hex2int(
						(char*) pobjSubIndex->getIndexValue());
				ENodeType iNodeType;
				if (MN_NODEID == iNodeidValue)
				{
					iNodeType = MN;
				}
				else
				{
					iNodeType = CN;
				}
				INT32 iNodePos;
				bool bFlag = false;
				retCode = IfNodeExists(iNodeidValue, iNodeType, &iNodePos,
						bFlag);

				if (OCFM_ERR_SUCCESS == retCode.code && true == bFlag
						&& ((CN == iNodeType) || (true == allowMNSubindex)))
				{
					//continue
				}
				else
				{
					if (NULL == pobjSubIndex->getDefaultValue())
						pobjSubIndex->setActualValue((char *) "");
					else
						pobjSubIndex->setActualValue(
								(char *) pobjSubIndex->getDefaultValue());
				}
			} catch (...)
			{
				if (NULL == pobjSubIndex->getDefaultValue())
					pobjSubIndex->setActualValue((char *) "");
				else
					pobjSubIndex->setActualValue(
							(char *) pobjSubIndex->getDefaultValue());
			}

		}
	}
}

/*****************************************************************************/
/**
 \brief		ValidateCNPresTimeout

 This function validates CN's PresTimeout with the Pres default timeout value.

 \param		pbSubIndexId	Character pointer to hold the IndexID
 \param		pcCheckValue	Character pointer to hold the new PresTimeout value

 \return	BOOL
 \retval	TRUE			if successful
 \retval	FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool ValidateCNPresTimeout(char* pbSubIndexId, char* pcCheckValue)
{
	ocfmRetCode stErrorInfo;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	bool retval = false;

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, (char*) "1F92", pbSubIndexId,
			&iSubIndexPos, &iIndexPos);
	if (stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		//continue with process
	}
	else
	{
		return retval;
	}

	INT32 iCNNodeId = hex2int(pbSubIndexId);
	stErrorInfo = IfSubIndexExists(iCNNodeId, CN, (char*) "1F98", (char*) "03",
			&iSubIndexPos, &iIndexPos);

	char* pcCNDefaultValue = new char[50];
	if (stErrorInfo.code == OCFM_ERR_SUCCESS)
	{
		GetSubIndexAttributes(iCNNodeId, CN, (char*) "1F98", (char*) "03",
				DEFAULTVALUE, pcCNDefaultValue);
	}
	else
	{
		delete[] pcCNDefaultValue;
		return retval;
	}

	if (!(NULL == pcCheckValue || strcmp(pcCheckValue, "") == 0))
	{
		if (!(NULL == pcCNDefaultValue || strcmp(pcCNDefaultValue, "") == 0))
		{
			INT32 minimumDefaultValue = GetDecimalValue(pcCNDefaultValue);
			INT32 iCheckValue = GetDecimalValue(pcCheckValue);
			if (iCheckValue >= minimumDefaultValue)
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

	delete[] pcCNDefaultValue;
	return retval;
}

/*****************************************************************************/
/**
 \brief			CopyOldNodeIdAssignmentObject


 \param		pobjNode		Class pointer of the class CNode
 \param		iOldNodeId		Integer variable to hold the old Node Id of a CN node

 \return	void
 */
/*****************************************************************************/

void CopyOldNodeIdAssignmentObject(CNode* pobjNode, INT32 iOldNodeId)
{
	if (NULL == pobjNode)
	{
		return;
	}
	CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*) "1F81");
	CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*) "1F92");
	CopyOldNodeIdAssignmentObjectSubindex(pobjNode, iOldNodeId, (char*) "1F9B");

}

/*****************************************************************************/
/**
 \brief			CopyOldNodeIdAssignmentObjectSubindex

This function sets the MN's corresponding subindex of the cn's node id with the actual value of the Mn's (cn's old node id subindex) subindex actual value
 
 \param		pobjNode		Class pointer of the class CBaseIndex
 \param		iOldNodeId		Integer variable to hold the old Node Id of a CN node
 \param		pcIndex			Character pointer to hold the IndexID

 \return	void
 */
/*****************************************************************************/

void CopyOldNodeIdAssignmentObjectSubindex(CNode* pobjNode, INT32 iOldNodeId,
		char* pcIndex)
{
	if ((NULL == pcIndex) || (NULL == pobjNode))
	{
		return;
	}
	INT32 iCNNodeId = pobjNode->getNodeId();
	ocfmRetCode stErrorInfo;
	INT32 iSubIndexPos = 0;
	INT32 iIndexPos = 0;
	char* pcCNNodeId = new char[10];
	pcCNNodeId = _IntToAscii(iCNNodeId, pcCNNodeId, 16);
	pcCNNodeId = padLeft(pcCNNodeId, '0', 2);
	char* pcCNOldNodeId = new char[10];
	pcCNOldNodeId = _IntToAscii(iOldNodeId, pcCNOldNodeId, 16);
	pcCNOldNodeId = padLeft(pcCNOldNodeId, '0', 2);
	char* TempOldActualValue = NULL;

	CIndexCollection* pobjMNIdxCol = NULL;
	pobjMNIdxCol = CNodeCollection::getNodeColObjectPointer()->getNodePtr(MN,
			MN_NODEID)->getIndexCollection();
	CIndex *pobjMNIndex;
	CSubIndex* pobjMNOldCNNodeIdSubIndex;

	stErrorInfo = IfSubIndexExists(MN_NODEID, MN, pcIndex, pcCNOldNodeId,
			&iSubIndexPos, &iIndexPos);
	if (OCFM_ERR_SUCCESS == stErrorInfo.code)
	{
		pobjMNIndex = pobjMNIdxCol->getIndexbyIndexValue(pcIndex);
		pobjMNOldCNNodeIdSubIndex = pobjMNIndex->getSubIndexbyIndexValue(
				pcCNOldNodeId);
		if (NULL != pobjMNOldCNNodeIdSubIndex)
		{
			EFlag TempOldFlg = FALSE;
			if (NULL != (char*) pobjMNOldCNNodeIdSubIndex->getActualValue())
			{
				TempOldActualValue = new char[strlen(
						(char*) pobjMNOldCNNodeIdSubIndex->getActualValue())
						+ STR_ALLOC_BUFFER];
				strcpy((char*) TempOldActualValue,
						(char*) pobjMNOldCNNodeIdSubIndex->getActualValue());
				TempOldFlg = pobjMNOldCNNodeIdSubIndex->getFlagIfIncludedCdc();
			}
			stErrorInfo = IfSubIndexExists(MN_NODEID, MN, pcIndex, pcCNNodeId,
					&iSubIndexPos, &iIndexPos);
			if (OCFM_ERR_SUCCESS == stErrorInfo.code)
			{
				//set the value alone
			}
			else
			{
				return;
			}

			CSubIndex* pobjMNCNNodeIdSubIndex = NULL;
			pobjMNCNNodeIdSubIndex = pobjMNIndex->getSubIndexbyIndexValue(
					pcCNNodeId);
			if (NULL != pobjMNCNNodeIdSubIndex)
			{
				pobjMNCNNodeIdSubIndex->setActualValue((char*) "");
			}

			if ((NULL != pobjMNCNNodeIdSubIndex)
					&& (NULL != TempOldActualValue))
			{
				//copy the actual value to here
				pobjMNCNNodeIdSubIndex->setActualValue(TempOldActualValue);
				pobjMNCNNodeIdSubIndex->setFlagIfIncludedCdc(TempOldFlg);
			}
			// reset the actual value of the old node id subindex
			SetSubIndexAttributesByAttribute(MN_NODEID, MN, pcIndex,
					pcCNOldNodeId, ACTUALVALUE, (char*) "");
		}
	}
	else
	{
		//no operation
	}
	delete[] pcCNNodeId;
	delete[] pcCNOldNodeId;
	delete[] TempOldActualValue;
}

/*****************************************************************************/
/**
 \brief		IsDefaultActualNotEqual

 This function checks the default value is same as the actual value or not

 \param		pBaseIndexObject	Class pointer of the class CBaseIndex

 \return	BOOL

 \retval	TRUE			if default value equal to actual value
 \retval	FALSE			if default value not equal to actual value
 */
/*****************************************************************************/

bool IsDefaultActualNotEqual(CBaseIndex* pBaseIndexObject)
{
	if (NULL == pBaseIndexObject || NULL == pBaseIndexObject->getActualValue())
		return false;

	if (NULL == pBaseIndexObject->getDefaultValue())
		return true;
	bool bReturnValue;
	DataType dt = pBaseIndexObject->getDataType();
	if (dt.Name != NULL)
	{
		if (!checkIfStringDatatypes(dt.Name))
		{
			INT32 iActualValue = 0;
			INT32 iDefaultValue = 0;
			iDefaultValue = GetDecimalValue(
					(char*) pBaseIndexObject->getDefaultValue());
			iActualValue = GetDecimalValue(
					(char*) pBaseIndexObject->getActualValue());
			if (iActualValue == iDefaultValue)
				bReturnValue = false;
			else
				bReturnValue = true;
		}
		else
		{
			if (0
					== strcmp(pBaseIndexObject->getDefaultValue(),
							pBaseIndexObject->getActualValue()))
				bReturnValue = false;
			else
				bReturnValue = true;
		}
	}
	else
	{
		if (0
				== strcmp(pBaseIndexObject->getDefaultValue(),
						pBaseIndexObject->getActualValue()))
			bReturnValue = false;
		else
			bReturnValue = true;
	}
	return bReturnValue;
}

/*****************************************************************************/
/**
 \brief		ReactivateMappingPDO

This function analyses the settings of a mapping object is returns the eligibility to enable/disable the mapping pdo

 \param		pobjIndexCol	Class pointer of the class CIndexCollection
 \param		pobjIndex		Class pointer of the class CIndex

 \return	BOOL

 \retval	TRUE			if the mapping pdo can be reactivated
 \retval	FALSE			if the mapping pdo cannot be reactivated
 */
/*****************************************************************************/

bool ReactivateMappingPDO(CIndexCollection* pobjIndexCol, CIndex* pobjIndex)
{
	if ((NULL == pobjIndexCol) || (NULL == pobjIndex))
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	if (false == CheckIfMappingPDO((char*) pobjIndex->getIndexValue()))
	{
		return false;
	}

	CSubIndex *pobjSubIndex = NULL;
	pobjSubIndex = pobjIndex->getSubIndexbyIndexValue((char*) "00");
	if ((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue())
			&& (0 != strcmp(pobjSubIndex->getActualValue(), "")))
	{
		if (false == checkIfValueZero((char*) pobjSubIndex->getActualValue()))
		{
			for (INT32 iLoopCount = 0;
					iLoopCount < pobjIndex->getNumberofSubIndexes();
					iLoopCount++)
			{
				if (pobjIndex->getSubIndex(iLoopCount)->getActualValue() != NULL
						&& TRUE
								== pobjIndex->getSubIndex(iLoopCount)->getFlagIfIncludedCdc()
						&& true
								== IsDefaultActualNotEqual(
										pobjIndex->getSubIndex(iLoopCount)))
				{
					if (0
							== strcmp(
									pobjIndex->getSubIndex(iLoopCount)->getIndexValue(),
									"00"))
					{
						return true;
						//continue;
					}
					else //other than 00'th subindex
					{
						if (true
								== IsDefaultActualNotEqual(
										pobjIndex->getSubIndex(iLoopCount)))
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
			if (false == IsDefaultActualNotEqual(pobjSubIndex))
			{
				return false;
			}

		}
	}

	char aCommParam[5];
	strcpy(aCommParam, (char*) pobjIndex->getIndexValue());

	if (strncmp((char*) pobjIndex->getIndexValue(), "1A", 2) == 0)
	{
		aCommParam[1] = '8';
	}
	else if (strncmp((char*) pobjIndex->getIndexValue(), "16", 2) == 0)
	{
		aCommParam[1] = '4';
	}
	else
	{
		return false;
	}

	CIndex *pObjCommParam = NULL;
	pObjCommParam = pobjIndexCol->getIndexbyIndexValue(aCommParam);
	if (NULL == pObjCommParam)
	{
		return false;
	}

	pobjSubIndex = pObjCommParam->getSubIndexbyIndexValue((char*) "00");
	if ((NULL != pobjSubIndex) && (NULL != pobjSubIndex->getActualValue())
			&& (0 != strcmp(pobjSubIndex->getActualValue(), "")))
	{
		if (checkIfValueZero((char*) pobjSubIndex->getActualValue()))
			return false;
	}

	for (INT32 iLoopCount = 0;
			iLoopCount < pObjCommParam->getNumberofSubIndexes(); iLoopCount++)
	{
		if (pObjCommParam->getSubIndex(iLoopCount)->getActualValue() != NULL
				&& TRUE
						== pObjCommParam->getSubIndex(iLoopCount)->getFlagIfIncludedCdc()
				&& true
						== CheckAccessTypeForInclude(
								(char*) pObjCommParam->getSubIndex(iLoopCount)->getAccessType())
				&& true
						== IsDefaultActualNotEqual(
								pObjCommParam->getSubIndex(iLoopCount)))
		{
			return true;
		}
	}

	return false;
}

/*****************************************************************************/
/**
 \brief		ArrangeNodeIDbyStation

 This function sorts the node id by the station type

 \return	INT32*
 */
/*****************************************************************************/

INT32* ArrangeNodeIDbyStation(void)
{
	CNodeCollection* objNodeCol = NULL;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode* pobjNode = NULL;

	/* Check RPDO Mapped objects*/
	INT32 iCNNodesCount = 0;

	iCNNodesCount = objNodeCol->getCNNodesCount();

	if (iCNNodesCount == 0)
	{
		exit(0);
	}

	INT32 *piNodeIDColl = new INT32[iCNNodesCount];
	INT32 iArrangedNodeIDCount = 0;
	INT32 *piArrangedNodeIDColl = new INT32[iCNNodesCount];
	EStationType *piStationTypeColl = new EStationType[iCNNodesCount];
	INT32 iNodesCount = objNodeCol->getNumberOfNodes();
	INT32 iLoopCount = 0;
	INT32 iCNNodeLoopCnt = 0;
	for (iLoopCount = 0; iLoopCount < iNodesCount; iLoopCount++)
	{
		pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
		if (MN == pobjNode->getNodeType())
		{
			continue;
		}
		piNodeIDColl[iCNNodeLoopCnt] = pobjNode->getNodeId();
		piStationTypeColl[iCNNodeLoopCnt] = pobjNode->getStationType();
		iCNNodeLoopCnt++;
	}
	//get the number of poll response station and other station
	INT32 iChainStnCnt = 0;
	INT32 iOtherStnCnt = 0;
	for (iLoopCount = 0; iLoopCount < iCNNodesCount; iLoopCount++)
	{
		if (CHAINED == piStationTypeColl[iLoopCount])
			iChainStnCnt++;
	}
	iOtherStnCnt = iCNNodesCount - iChainStnCnt;

	//create arrays to store nodeid for chained station and other station
	if (0 != iChainStnCnt)
	{
		INT32 *piChainStnColl = new INT32[iChainStnCnt];
		INT32 iChainStnLoopCnt = 0;
		for (iLoopCount = 0; iLoopCount < iCNNodesCount; iLoopCount++)
		{
			if (CHAINED == piStationTypeColl[iLoopCount])
			{
				piChainStnColl[iChainStnLoopCnt] = piNodeIDColl[iLoopCount];
				iChainStnLoopCnt++;
			}
		}
		//sort by station no
		SortNodeID(piChainStnColl, iChainStnCnt);

		//copy the poll response staion sorted in asscending order
		for (iLoopCount = 0; iLoopCount < iChainStnCnt; iLoopCount++)
		{
			piArrangedNodeIDColl[iArrangedNodeIDCount] =
					piChainStnColl[iLoopCount];
			iArrangedNodeIDCount++;
		}

		delete[] piChainStnColl;
	}

	if (0 != iOtherStnCnt)
	{
		INT32 *piOtherStnColl = new INT32[iOtherStnCnt];
		INT32 iOtherStnLoopCnt = 0;
		for (iLoopCount = 0; iLoopCount < iCNNodesCount; iLoopCount++)
		{
			if (CHAINED != piStationTypeColl[iLoopCount])
			{
				piOtherStnColl[iOtherStnLoopCnt] = piNodeIDColl[iLoopCount];
				iOtherStnLoopCnt++;
			}
		}
		//copy the other station after the poll response staion
		for (iLoopCount = 0; iLoopCount < iOtherStnCnt; iLoopCount++)
		{
			piArrangedNodeIDColl[iArrangedNodeIDCount] =
					piOtherStnColl[iLoopCount];
			iArrangedNodeIDCount++;
		}
		delete[] piOtherStnColl;
	}
	delete[] piNodeIDColl;
	delete[] piStationTypeColl;

	return piArrangedNodeIDColl;
}

/*****************************************************************************/
/**
 \brief		SortNodeID

This function sorts the node id in a given node id collection

 \param		piNodeIDColl	Integer pointer variable to hold the collection of all Nodes
 \param		iColSize		Integer variable to hold the collection size

 \return	void
 */
/*****************************************************************************/

void SortNodeID(INT32 *piNodeIDColl, INT32 iColSize)
{

	try
	{
		if (NULL == piNodeIDColl)
		{
			ocfmException objException;
			objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}

		INT32 iTemp = 0;
		for (INT32 iLoopCount = 0; iLoopCount < iColSize; iLoopCount++)
		{
			for (INT32 iSortCount = iLoopCount + 1; iSortCount <= iLoopCount;
					iSortCount++)
			{
				if (piNodeIDColl[iLoopCount] > piNodeIDColl[iSortCount])
				{
					iTemp = piNodeIDColl[iLoopCount];
					piNodeIDColl[iLoopCount] = piNodeIDColl[iSortCount];
					piNodeIDColl[iSortCount] = iTemp;
				}
			}
		}
	} catch (ocfmException& ex)
	{
		throw ex;
	}
}

/*****************************************************************************/
/**
 \brief		IsPresMN

 This function checks for the MN node is a Pres(chained station) or not

 \return	BOOL

 \retval		TRUE			if pres MN
 \retval		FALSE			if not a pres MN
 */
/*****************************************************************************/

bool IsPresMN()
{
	CNodeCollection *objNodeCol = NULL;
	objNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode *pobjNode = NULL;
	INT32 iLoopCount;
	bool bPres = false;
	INT32 iNodesCount = objNodeCol->getNumberOfNodes();
	for (iLoopCount = 0; iLoopCount < iNodesCount; iLoopCount++)
	{
		pobjNode = objNodeCol->getNodebyColIndex(iLoopCount);
		if (MN == pobjNode->getNodeType())
		{
			continue;
		}
		if (CHAINED == pobjNode->getStationType())
		{
			//return true;
			bPres = true;
		}
	}
	return bPres;
}

/*****************************************************************************/
/**
 \brief		SetCNLossObjects

 This function sets values to 1COB, 1C0C and 1C0D objects

 \param		iNodeID			Integer variable to hold the Node Id of a node
 \param		enumNodeType	Enum to hold the Node type of the node

 \return	void
 */
/*****************************************************************************/

void SetCNLossObjects(INT32 iNodeID, ENodeType enumNodeType)
{
	if (enumNodeType != CN)
	{
		return;
	}

	CNode *pobjNode = NULL;
	CNodeCollection *objNodeCollection = NULL;
	CIndexCollection* pobjIdxCol = NULL;
	CIndex *pobjIndex = NULL;
	CSubIndex* pobjSIndex = NULL;

	objNodeCollection = CNodeCollection::getNodeColObjectPointer();
	pobjNode = objNodeCollection->getNodePtr(enumNodeType, iNodeID);
	pobjIdxCol = pobjNode->getIndexCollection();

	//loss of SoC
	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1C0B");
	if (NULL != pobjIndex)
	{
		pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*) "03");
		if (NULL != pobjSIndex)
		{
			if ((pobjSIndex->getActualValue() == NULL)
					|| (strcmp(pobjSIndex->getActualValue(), "") == 0))
			{
				pobjSIndex->setActualValue((char*) "0x50");
				pobjSIndex->setFlagIfIncludedCdc(TRUE);
				pobjIndex->setFlagIfIncludedCdc(TRUE);
			}
		}
	}

	//loss of SoA
	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1C0C");
	if (NULL != pobjIndex)
	{
		pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*) "03");
		if (NULL != pobjSIndex)
		{
			if ((pobjSIndex->getActualValue() == NULL)
					|| (strcmp(pobjSIndex->getActualValue(), "") == 0))
			{
				pobjSIndex->setActualValue((char*) "0x50");
				pobjSIndex->setFlagIfIncludedCdc(TRUE);
				pobjIndex->setFlagIfIncludedCdc(TRUE);
			}
		}
	}

	//loss of PReq
	pobjIndex = pobjIdxCol->getIndexbyIndexValue((char*) "1C0D");
	if (NULL != pobjIndex)
	{
		pobjSIndex = pobjIndex->getSubIndexbyIndexValue((char*) "03");
		if (NULL != pobjSIndex)
		{
			if ((pobjSIndex->getActualValue() == NULL)
					|| (strcmp(pobjSIndex->getActualValue(), "") == 0))
			{
				pobjSIndex->setActualValue((char*) "0x50");
				pobjSIndex->setFlagIfIncludedCdc(TRUE);
				pobjIndex->setFlagIfIncludedCdc(TRUE);
			}
		}
	}
}

/*****************************************************************************/
/**
 \brief		SetBuildTime

 Gets the system time during time of build - to be called from ProcessPDOnodes

 \return	void
 */
/*****************************************************************************/

void SetBuildTime()
{
	time(&sBuildTime_g.rawtime);
	sBuildTime_g.timeinfo = localtime(&sBuildTime_g.rawtime);
	strftime(sBuildTime_g.buffer, BUILDTIME_BUF_LEN, "%d-%b-%Y %H:%M:%S",
			sBuildTime_g.timeinfo);
}

/*****************************************************************************/
/**
 \brief		GetBuildTime

 Gets the calculated system time during time of build to be called from API generating interface files

 \return	const char*
 */
/*****************************************************************************/
const char* GetBuildTime()
{
	return (const char*) &sBuildTime_g.buffer;
}

