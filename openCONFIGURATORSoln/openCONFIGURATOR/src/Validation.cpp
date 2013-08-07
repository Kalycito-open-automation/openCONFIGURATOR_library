/**
 ************************************************************************************************
 \file		Validation.cpp

 \brief		This file contains the non API definitions that are used in checking existence of datatypes, SubIndices in collection list and version number of tool
 ************************************************************************************************
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

/****************************************************************************************************/
/* Includes */

#include <iostream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <sstream>
#include "../Include/Validation.h"
#include "../Include/Exception.h"

using namespace std;
//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

ocfmRetCode IfNodeExists(INT32 nodeId, NodeType nodeType, INT32 *nodePos,
		bool& nodeExist)
{
	Node nodeObj;
	NodeCollection *nodeCollObj = NULL;
	ocfmRetCode exceptionObj;

	try
	{

		if (NULL == nodePos)
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
#if defined DEBUG
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
#endif
			throw &exceptionObj;
		}
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
#if defined DEBUG
			cout << "IfNodeExists: pobjNodeCollection is NULL!" << endl;
#endif
			exceptionObj.setErrorCode(OCFM_ERR_UNKNOWN);
			return exceptionObj;
		}

		if (nodeCollObj->GetNumberOfNodes() > 0)
		{
			for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
					nodeLC++)
			{
				nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);

				if (nodeObj.GetNodeType() == nodeType)
				{
					if (nodeObj.GetNodeId() == nodeId)
					{
						*nodePos = nodeLC;
						exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
						nodeExist = true;

						return exceptionObj;
					}
				}
			}
			exceptionObj.setErrorCode(OCFM_ERR_NODEID_NOT_FOUND);
			throw exceptionObj;
		}
		else
		{
			exceptionObj.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
			throw exceptionObj;
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
}

ocfmRetCode IfIndexExists(INT32 nodeId, NodeType nodeType, char* indexId,
		INT32 *idxPos)
{
	INT32 nodePos;
	bool bFlag = false;
	ocfmRetCode exceptionObj;

	try
	{
		if ((NULL == indexId) || (NULL == idxPos))
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
#if defined DEBUG
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
#endif
			throw exceptionObj;
		}

		exceptionObj = IfNodeExists(nodeId, nodeType, &nodePos, bFlag);

		if ((true != bFlag) && (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode()))
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_NODEID);
			throw exceptionObj;
		}
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *idxCollObj = NULL;
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
		idxCollObj = nodeObj.GetIndexCollection();

		if (0 == idxCollObj->GetNumberofIndexes())
		{
			*idxPos = 0;
			exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
		}
		else if (idxCollObj->GetNumberofIndexes() > 0)
		{
			//Check for existance of the Index
			for (INT32 idxLC = 0; idxLC < idxCollObj->GetNumberofIndexes();
					idxLC++)
			{
				Index *idxObj = NULL;
				idxObj = idxCollObj->GetIndex(idxLC);

				char *objIdxIdUpper = new char[strlen(idxObj->GetIndexValue()) + STR_ALLOC_BUFFER];
				char *idxIdUpper = new char[strlen(indexId) + STR_ALLOC_BUFFER];
				strcpy(objIdxIdUpper, idxObj->GetIndexValue());
				objIdxIdUpper = ConvertToUpper(objIdxIdUpper);
				strcpy(idxIdUpper, indexId);
				idxIdUpper = ConvertToUpper(idxIdUpper);

				if (0 == strcmp(objIdxIdUpper, idxIdUpper))
				{
					*idxPos = idxLC;
					exceptionObj.setErrorCode(OCFM_ERR_SUCCESS);
					delete[] objIdxIdUpper;
					delete[] idxIdUpper;
					return exceptionObj;
				}
				else if (idxLC == (idxCollObj->GetNumberofIndexes() - 1))
				{
					// Index Doesn't Exist
					exceptionObj.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
					delete[] objIdxIdUpper;
					delete[] idxIdUpper;
					return exceptionObj;
				}
				delete[] objIdxIdUpper;
				delete[] idxIdUpper;
			}
		}
		else
		{
			// Indexes Doesn't Exist
			exceptionObj.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObj;
}

ocfmRetCode IfSubIndexExists(INT32 nodeId, NodeType nodeType, char* idxId,
		char* sidxId, INT32* sidxPos, INT32* idxPos)
{
	ocfmRetCode objException;
	try
	{
		if ((NULL == idxId) || (NULL == sidxId) || (NULL == idxPos)
				|| (NULL == sidxPos))
		{
			objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
#if defined DEBUG
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
#endif
			throw objException;
		}
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *idxCollObj = NULL;
		Index *idxObj = NULL;

		objException = IfIndexExists(nodeId, nodeType, idxId, idxPos);

		if (OCFM_ERR_SUCCESS != objException.getErrorCode())
		{
			// Node Doesn't Exist
			objException.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
			return objException;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
		//idxObj->SetNodeID(nodeObj.GetNodeId());
		idxCollObj = nodeObj.GetIndexCollection();
		idxObj = idxCollObj->GetIndex(*idxPos);
		if (idxObj->GetNumberofSubIndexes() == 0)
		{
			objException.setErrorCode(OCFM_ERR_NO_SUBINDEXS_FOUND);
		}
		else if (idxObj->GetNumberofSubIndexes() > 0)
		{
			//Check for existance of the SubIndex
			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
					sidxLC++)
			{
				SubIndex* sidxObj = NULL;
				sidxObj = idxObj->GetSubIndex(sidxLC);

				char *objSidxIdUpper = new char[strlen(sidxObj->GetIndexValue()) + STR_ALLOC_BUFFER];
				char *sidxIdUpper = new char[strlen(sidxId) + STR_ALLOC_BUFFER];
				strcpy(objSidxIdUpper, sidxObj->GetIndexValue());
				strcpy(sidxIdUpper, sidxId);
				objSidxIdUpper = ConvertToUpper(objSidxIdUpper);
				sidxIdUpper = ConvertToUpper(sidxIdUpper);

				if ((strcmp(objSidxIdUpper, sidxIdUpper) == 0))
				{
					objException.setErrorCode(OCFM_ERR_SUCCESS);
					*sidxPos = sidxLC;
					delete[] objSidxIdUpper;
					delete[] sidxIdUpper;
					return objException;
				}
				else if (sidxLC == (idxObj->GetNumberofSubIndexes() - 1))
				{
					// SubIndex Doesn't Exist
					objException.setErrorCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
					delete[] objSidxIdUpper;
					delete[] sidxIdUpper;
					return objException;
				}
				delete[] objSidxIdUpper;
				delete[] sidxIdUpper;
			}
		}
		else
		{
			objException.setErrorCode(OCFM_ERR_UNKNOWN);
		}
	} catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return objException;
}

bool CheckIfDataTypeExists(char* dataValue, INT32 nodeId)
{
	NodeCollection* nodeCollObj = NULL;
	DataTypeCollection* dtCollObj = NULL;
	DataType* dtObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node nodeObj = nodeCollObj->GetNode(nodeId);
	dtCollObj = nodeObj.GetDataTypeCollection();

	dtObj = dtCollObj->GetDataType(dataValue);

	if (NULL == dtObj)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CheckIfDataTypeByNameExists(char* dtName, INT32 nodeId)
{
	NodeCollection *nodeCollObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node nodeObj = nodeCollObj->GetNode(nodeId);
	DataTypeCollection *dtCollObj = NULL;
	dtCollObj = nodeObj.GetDataTypeCollection();
	DataType *dtObj = NULL;
	dtObj = dtCollObj->GetDataTypeByName(dtName);
	if (NULL == dtObj)
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CheckIfSubIndexExists(INT32 nodeId, NodeType nodeType, char* indexID,
		char* subIndexId)
{
	Node nodeObj;
	NodeCollection *nodeCollObj = NULL;
	IndexCollection *idxCollObj = NULL;
	Index *idxObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
	idxCollObj = nodeObj.GetIndexCollection();
	idxObj = idxCollObj->GetIndexbyIndexValue(indexID);

	if (NULL == idxObj)
	{
		return false;
	}
	else if ((0 == idxObj->GetNumberofSubIndexes()))
	{
		return false;
	}
	else if (idxObj->GetNumberofSubIndexes() > 0)
	{
		//Check for existance of the SubIndex
		for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
				sidxLC++)
		{
			SubIndex* objSubIndexPtr;
			objSubIndexPtr = idxObj->GetSubIndex(sidxLC);

			char *objSidxIdUpper = new char[strlen(objSubIndexPtr->GetIndexValue()) + STR_ALLOC_BUFFER];
			char *sidxIdUpper = new char[strlen(subIndexId) + STR_ALLOC_BUFFER];
			strcpy(objSidxIdUpper, objSubIndexPtr->GetIndexValue());
			strcpy(sidxIdUpper, subIndexId);
			objSidxIdUpper = ConvertToUpper(objSidxIdUpper);
			sidxIdUpper = ConvertToUpper(sidxIdUpper);

			if ((0 == strcmp(objSidxIdUpper, sidxIdUpper)))
			{
				delete[] sidxIdUpper;
				delete[] objSidxIdUpper;
				return true;
			}
			else if (sidxLC == (idxObj->GetNumberofSubIndexes() - 1))
			{
				delete[] sidxIdUpper;
				delete[] objSidxIdUpper;
				return false;
			}
			else
			{
				delete[] sidxIdUpper;
				delete[] objSidxIdUpper;
				//TODO: Nothing to be added. Continue to run the loop
			}
		}
		return false;
	}
	else
	{
		return false;
	}
}

bool IfVersionNumberMatches(xmlTextReaderPtr reader)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;
	//Retrieve the name and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);
	bool retVal = false;
	if (NULL == value || NULL == name)
	{
		return retVal;
	}
#if defined DEBUG
	cout << "\nName:" << name << endl;
	cout << "Value:" << value << endl;
#endif
	// Check for Version Tool-Project Version
	if (0 == strcmp(ConvertToUpper((char*) name), "VERSION"))
	{
		if((char*) value != NULL)
		{
			PjtSettings* pjtSettingsObj;
			pjtSettingsObj = PjtSettings::GetPjtSettingsPtr();
			pjtSettingsObj->SetPjtVersion((const char*) value);

			if (true == CheckToolVersion((char*) value))
			{
#if defined DEBUG
				cout << "Version number matched" << endl;
#endif
				retVal = true;
			}
			else
			{
#if defined DEBUG
				cout << "Version number MisMatch" << endl;
#endif
				retVal = false;
			}
		}
	}
	else
	{
#if defined DEBUG
		cout << "Error! IfVersionNumberMatches function can't find VERSION" << endl;
#endif
		retVal = false;
	}
	return retVal;
}

bool CheckPdoCommParam(PDOType pdoTypeVar, bool isBuild, Index *indexObj, IndexCollection *indexCollObj, Node *nodeObj)
{
	ocfmRetCode exceptionObj;
	char *varCommIdx = new char[INDEX_LEN];
	char customError[200] = { 0 };
	ostringstream errorString;

	if (pdoTypeVar == PDO_TPDO)
	{
		char *varIdx = new char[SUBINDEX_LEN];
		varIdx = SubString(varIdx, indexObj->GetIndexValue(), 2, 2);
		strcpy(varCommIdx, (char *) "18");
		strcat(varCommIdx, varIdx);
		//If varIdx != "00" throw error as only the 1st object 1A00 shall be implemented on a CN
		if (0 != strcmp(varIdx, "00"))
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_INDEXID);
			errorString<<"The TPDO object is not valid for CN with Node name: '"<<nodeObj->GetNodeName()<<"', Node ID: '"<<nodeObj->GetNodeId()<<"' \nReason: Only the TPDO object pair 1800 and 1A00 shall be implemented for a CN";
			exceptionObj.setErrorString(errorString.str());
			delete[] varIdx;
			delete[] varCommIdx;
			throw exceptionObj;
		}
		delete[] varIdx;
	}
	else if (pdoTypeVar == PDO_RPDO)
	{
		char *varIdx = new char[SUBINDEX_LEN];
		varIdx = SubString(varIdx, indexObj->GetIndexValue(), 2, 2);
		strcpy(varCommIdx, (char *) "14");
		strcat(varCommIdx, varIdx);
		delete[] varIdx;
	}
	else
	{
		delete[] varCommIdx;
		return false;
	}

	Index *commIndexObj = NULL;
	commIndexObj = indexCollObj->GetIndexbyIndexValue(varCommIdx);
	if (NULL == commIndexObj)
	{
		//throw exception as matching communication index not found for a CN
		exceptionObj.setErrorCode(OCFM_ERR_MODULE_INDEX_NOT_FOUND);
		errorString<<"Communication Param object 0x"<<varCommIdx<<" not found in the node "<<nodeObj->GetNodeName()<<"( "<<nodeObj->GetNodeId()<<" )";
		exceptionObj.setErrorString(errorString.str());
		delete[] varCommIdx;
		throw exceptionObj;
	}
	delete[] varCommIdx;

	SubIndex *subIndexObj = NULL;
	subIndexObj = commIndexObj->GetSubIndexbyIndexValue((char *) "01");
	if (NULL == subIndexObj)
	{
		//Throw exception as Target node id sidx not found in a CN TPDO comm param object
		exceptionObj.setErrorCode(OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND);
		errorString<<"In CN: "<<nodeObj->GetNodeId()<<". SubObject PDO_Target_Node_Id(0x01) in Object 0x"<<commIndexObj->GetIndexValue()<<" not found.";
		
		throw exceptionObj;
	}

	if (NULL != subIndexObj->GetActualValue())
	{
		if (0 == strlen(subIndexObj->GetActualValue()))
		{
			//Throw exception as wrong Target node id for TPDO CN
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
			errorString<<"In CN: "<<nodeObj->GetNodeId()<<". Invalid PDO_Target_Node_Id value configured in Object "<<commIndexObj->GetIndexValue()<<"/01";
			exceptionObj.setErrorString(errorString.str());
			throw exceptionObj;
		}
		INT32 mappedNodeId = GetDecimalValue((char *) subIndexObj->GetActualValue());

		if (pdoTypeVar == PDO_TPDO)
		{
			if ((BROADCAST_NODEID != mappedNodeId))
			{
				//Throw exception as wrong Target node id for TPDO CN
				exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
				errorString<<"In CN: "<<nodeObj->GetNodeId()<<". Invalid PDO_Target_Node_Id value configured in Object "<<commIndexObj->GetIndexValue()<<"/01. It Should be always 0 for a CN's TPDO";
				exceptionObj.setErrorString(errorString.str());
				throw exceptionObj;
			}
			else
			{
				//Correct Target node id for a CN TPDO
			}
		}
		else if (pdoTypeVar == PDO_RPDO)
		{
			if (isBuild == true)
			{
				StationType stnType = nodeObj->GetStationType();
				if ((CHAINED == stnType)
					&& (BROADCAST_NODEID == mappedNodeId))
				{
					subIndexObj->SetActualValue((char *) "0xF0");
				}

				if ((CHAINED != stnType)
					&& (MN_NODEID == mappedNodeId))
				{
					subIndexObj->SetActualValue((char *) "0x0");
				}

				if ((BROADCAST_NODEID != mappedNodeId))
				{
					if ((MN_NODEID != mappedNodeId))
					{
						return false;
					}
				}
			}
		}
		else
		{
			return false;
		}
	}
	else if (NULL != subIndexObj->GetDefaultValue())
	{
		//Actual value not configured. So checking default value
		if (0 == strlen(subIndexObj->GetDefaultValue()))
		{
			exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
			errorString<<"In CN: "<<nodeObj->GetNodeId()<<". Invalid default PDO_Target_Node_Id value configured in Object "<<commIndexObj->GetIndexValue()<<"/01";
			exceptionObj.setErrorString(errorString.str());
			throw exceptionObj;
		}
		INT32 mappedNodeId = GetDecimalValue((char *) subIndexObj->GetDefaultValue());
		if (pdoTypeVar == PDO_TPDO)
		{
			if ((BROADCAST_NODEID != mappedNodeId))
			{
				//Throw exception as wrong Target node id for TPDO CN
				exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
				errorString<<"In CN: "<<nodeObj->GetNodeId()<<". Invalid default PDO_Target_Node_Id value configured in Object "<<commIndexObj->GetIndexValue()<<"/01. It Should be always 0 for a CN's TPDO";
				exceptionObj.setErrorString(errorString.str());
				throw exceptionObj;
			}
			else
			{
				//Correct Target node id for a CN TPDO
			}
		}
		else if (pdoTypeVar == PDO_RPDO)
		{
			if (isBuild == true)
			{
				StationType stnType = nodeObj->GetStationType();
				if ((CHAINED == stnType)
					&& (BROADCAST_NODEID == mappedNodeId))
				{
					subIndexObj->SetActualValue((char *) "0xF0");
				}

				if ((CHAINED != stnType)
					&& (MN_NODEID == mappedNodeId))
				{
					subIndexObj->SetActualValue((char *) "0x0");
				}

				if ((BROADCAST_NODEID != mappedNodeId))
				{
					if ((MN_NODEID != mappedNodeId))
					{
						return false;
					}
				}
			}
		}
		else
		{
			return false;
		}
	}
	else
	{
		//Throw exception as Both default & Actual Target node id  value is not configured
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_VALUE);
		errorString<<"In CN: "<<nodeObj->GetNodeId()<<". PDO_Target_Node_Id value not configured in Object "<<commIndexObj->GetIndexValue()<<"/01";
		exceptionObj.setErrorString(errorString.str());
		throw exceptionObj;
	}

	return true;
}
bool CheckForValidPDOMapping(PDOType pdoTypeVal, Index* indexObj)
{

	bool retVal = false;
	if (strcmp(indexObj->GetPDOMapping(), "NO") == 0)
	{
		retVal = false;
	}
	else if ((strcmp(indexObj->GetPDOMapping(), "OPTIONAL") == 0) || (strcmp(indexObj->GetPDOMapping(), "DEFAULT") == 0))
	{
		retVal = true;
	}
	else
	{
		switch(pdoTypeVal)
		{
			case PDO_TPDO:
					if( (strcmp(indexObj->GetPDOMapping(), "TPDO") == 0))
					{
						retVal = true;
					}
					else
					{
						//default pdo mapping ?
					}
					break;
			case PDO_RPDO:
					if( (strcmp(indexObj->GetPDOMapping(), "RPDO") == 0))
					{
						retVal = true;
					}
					else
					{
						//default pdo mapping ?
					}
					break;
			default:
					retVal = false;
					break;
		}
	}
#if defined DEBUG
	cout<<indexObj->GetPDOMapping()<<"PDO Mapping in Index:"<<indexObj->GetIndexValue()<<endl;
#endif
	return retVal;
}
//TODO: Remove indexObj - unused
bool CheckForValidPDOMapping(PDOType pdoTypeVal, Index* indexObj, SubIndex* sidxObj)
{
	bool retVal = false;
	if (strcmp(sidxObj->GetPDOMapping(), "NO") == 0)
	{
		retVal = false;
	}
	else if ((strcmp(sidxObj->GetPDOMapping(), "OPTIONAL") == 0) || (strcmp(sidxObj->GetPDOMapping(), "DEFAULT") == 0))
	{
		retVal = true;
	}
	else
	{
		switch(pdoTypeVal)
		{
			case PDO_TPDO:
					if( (strcmp(sidxObj->GetPDOMapping(), "TPDO") == 0))
					{
						retVal = true;
					}
					else
					{
						//default pdo mapping ?
					}
					break;
			case PDO_RPDO:
					if( (strcmp(sidxObj->GetPDOMapping(), "RPDO") == 0))
					{
						retVal = true;
					}
					else
					{
						//default pdo mapping ?
					}
					break;
			default:
					retVal = false;
					break;
		}
	}
#if defined DEBUG
	cout<<sidxObj->GetPDOMapping()<<" PDO Mapping in "<<indexObj->GetIndexValue()<<"/"<<sidxObj->GetIndexValue()<<endl;
#endif
	return retVal;
}

bool IsValidAccessTypeForPdo(PDOType pdoTypeVal, char* pdoMappingVal, char* accessType)
{
	bool retVal = false;
	char* accessTypeUpper = new char[strlen(accessType) + STR_ALLOC_BUFFER];
	char* pdoMappingUpper = new char[strlen(pdoMappingVal) + STR_ALLOC_BUFFER];

	strcpy(accessTypeUpper, accessType);
	strcpy(pdoMappingUpper, pdoMappingVal);
	accessTypeUpper = ConvertToUpper(accessTypeUpper);
	pdoMappingUpper = ConvertToUpper(pdoMappingUpper);
	
	switch(pdoTypeVal)
	{
	case PDO_RPDO:
		#if defined DEBUG
			cout<<"Checking for AccesType: "<<accessTypeUpper<<" with PDOmapping: "<<pdoMappingUpper<<" in an RPDO";
		#endif
			if ((strcmp(pdoMappingUpper, "DEFAULT") == 0) || (strcmp(pdoMappingUpper, "OPTIONAL") == 0))
			{
				if((strcmp(accessTypeUpper, "WO") == 0) || (strcmp(accessTypeUpper, "WRITE") == 0) || (strcmp(accessTypeUpper, "RW") == 0) || (strcmp(accessTypeUpper, "READWRITE") == 0) || (strcmp(accessTypeUpper, "READWRITEOUTPUT") == 0))
				{
					retVal = true;
				}
				else
				{
					retVal = false;
				}
			}
			else if ((strcmp(pdoMappingUpper, "RPDO") == 0))
			{
				if((strcmp(accessTypeUpper, "WO") == 0) || (strcmp(accessTypeUpper, "WRITE") == 0) || (strcmp(accessTypeUpper, "RW") == 0) || (strcmp(accessTypeUpper, "READWRITE") == 0) || (strcmp(accessTypeUpper, "READWRITEOUTPUT") == 0))
				{
					retVal = true;
				}
				else
				{
					retVal = false;
				}
			}
			else
			{
				retVal = false;
			}
			break;
	case PDO_TPDO:
		#if defined DEBUG
			cout<<"Checking for AccesType: "<<accessTypeUpper<<" with PDOmapping: "<<pdoMappingUpper<<" in an TPDO";
		#endif
			if ((strcmp(pdoMappingUpper, "DEFAULT") == 0) || (strcmp(pdoMappingUpper, "OPTIONAL") == 0))
			{
				if((strcmp(accessTypeUpper, "RO") == 0) || (strcmp(accessTypeUpper, "READ") == 0) || (strcmp(accessTypeUpper, "RW") == 0) || (strcmp(accessTypeUpper, "READWRITE") == 0) || (strcmp(accessTypeUpper, "READWRITEINPUT") == 0))
				{
					retVal = true;
				}
				else
				{
					retVal = false;
				}
			}
			else if ((strcmp(pdoMappingUpper, "TPDO") == 0))
			{
				if((strcmp(accessTypeUpper, "RO") == 0) || (strcmp(accessTypeUpper, "READ") == 0) || (strcmp(accessTypeUpper, "RW") == 0) || (strcmp(accessTypeUpper, "READWRITE") == 0) || (strcmp(accessTypeUpper, "READWRITEINPUT") == 0))
				{
					retVal = true;
				}
				else
				{
					retVal = false;
				}
			}
			else
			{
				retVal = false;
			}
			break;
	default:
		retVal = false;
		break;
	}

	delete[] pdoMappingUpper;
	delete[] accessTypeUpper;
	#if defined DEBUG
		cout<<" : Returning: "<<retVal<<endl;
	#endif
	return retVal;
}
