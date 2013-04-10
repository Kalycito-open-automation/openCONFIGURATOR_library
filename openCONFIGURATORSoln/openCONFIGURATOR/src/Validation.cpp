/**
 ************************************************************************************************
 \file			Validation.cpp

 \brief			Validates Index,Subindex,node id,datatype and Projectversion
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
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../Include/Validation.h"
#include "../Include/Exception.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

ocfmRetCode IfNodeExists(INT32 nodeId, NodeType nodeType, INT32 *nodePos, bool& nodeExist)
{
	Node nodeObj;
	NodeCollection *nodeCollObj = NULL;
	ocfmRetCode errCodeObj;
	ocfmException exceptionObj;

	try
	{

		if (NULL == nodePos)
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw &exceptionObj;
		}
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		if (NULL == nodeCollObj)
		{
			cout << "IfNodeExists: pobjNodeCollection is NULL!" << endl;
			errCodeObj.code = OCFM_ERR_UNKNOWN;
			return errCodeObj;
		}

		if (nodeCollObj->GetNumberOfNodes() > 0)
		{
			for (INT32 nodeLC = 0;
					nodeLC < nodeCollObj->GetNumberOfNodes();
					nodeLC++)
			{
				nodeObj = nodeCollObj->GetNodebyCollectionIndex(
						nodeLC);

				if (nodeObj.GetNodeType() == nodeType)
				{
					if (nodeObj.GetNodeId() == nodeId)
					{
						*nodePos = nodeLC;
						errCodeObj.code = OCFM_ERR_SUCCESS;
						nodeExist = true;

						return errCodeObj;
					}
				}
			}
			exceptionObj.OCFMException(OCFM_ERR_NODEID_NOT_FOUND);
			throw &exceptionObj;
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_NO_NODES_FOUND);
			throw &exceptionObj;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	//return stErrStruct;
}

ocfmRetCode IfIndexExists(INT32 nodeId, NodeType nodeType, char* indexId, INT32 *idxPos)
{
	ocfmRetCode errCodeObj;
	INT32 nodePos;
	bool bFlag = false;
	ocfmException exceptionObj;

	try
	{
		if ((NULL == indexId) || (NULL == idxPos))
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw exceptionObj;
		}

		errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, bFlag);

		if ((true == bFlag) && (OCFM_ERR_SUCCESS == errCodeObj.code))
		{
			//continue with process
		}
		else
		{
			exceptionObj.OCFMException(OCFM_ERR_INVALID_NODEID);
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
			errCodeObj.code = OCFM_ERR_NO_INDEX_FOUND;
		}
		else if (idxCollObj->GetNumberofIndexes() > 0)
		{
			//Check for existance of the Index
			for (INT32 idxLC = 0;
					idxLC < idxCollObj->GetNumberofIndexes();
					idxLC++)
			{
				Index *idxObj = NULL;
				char *tempIndexId = NULL;

				idxObj = idxCollObj->GetIndex(idxLC);
				tempIndexId = new char[strlen((char*) idxObj->GetIndexValue())
								+ STR_ALLOC_BUFFER];
				strcpy(tempIndexId, (char*) idxObj->GetIndexValue());

				if (0 == strcmp(StringToUpper(tempIndexId), StringToUpper(indexId)))
				{
					Index *objIndexPtr = NULL;

					objIndexPtr = idxCollObj->GetIndex(idxLC);
					*idxPos = idxLC;
					errCodeObj.code = OCFM_ERR_SUCCESS;
					return errCodeObj;
				}
				else if (idxLC
						== (idxCollObj->GetNumberofIndexes() - 1))
				{
					// Index Doesn't Exist
					errCodeObj.code = OCFM_ERR_INDEXID_NOT_FOUND;
					return errCodeObj;
				}
				else
				{
				}
				delete[] tempIndexId;
			}
		}
		else
		{
			// Indexes Doesn't Exist
			errCodeObj.code = OCFM_ERR_NO_INDEX_FOUND;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode IfSubIndexExists(INT32 nodeId, NodeType nodeType, char* idxId, char* sidxId, INT32* sidxPos, INT32* idxPos)
{
	ocfmRetCode errCodeObj;
	ocfmException objException;
	try
	{
		if ((NULL == idxId) || (NULL == sidxId)
				|| (NULL == idxPos) || (NULL == sidxPos))
		{
			objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
			cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
			throw objException;
		}
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *idxCollObj = NULL;
		Index *idxObj = NULL;

		errCodeObj = IfIndexExists(nodeId, nodeType, idxId, idxPos);

		if (OCFM_ERR_SUCCESS != errCodeObj.code)
		{
			// Node Doesn't Exist
			errCodeObj.code = OCFM_ERR_INDEXID_NOT_FOUND;
			return errCodeObj;
		}

		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
		//idxObj->SetNodeID(nodeObj.GetNodeId());
		idxCollObj = nodeObj.GetIndexCollection();
		idxObj = idxCollObj->GetIndex(*idxPos);
		if (idxObj->GetNumberofSubIndexes() == 0)
		{
			errCodeObj.code = OCFM_ERR_NO_SUBINDEXS_FOUND;
		}
		else if (idxObj->GetNumberofSubIndexes() > 0)
		{
			//Check for existance of the SubIndex
			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes(); sidxLC++)
			{
				SubIndex* sidxObj = NULL;

				sidxObj = idxObj->GetSubIndex(sidxLC);
				if ((strcmp(
						StringToUpper((char*) sidxObj->GetIndexValue()),
						StringToUpper(sidxId)) == 0))
				{
					errCodeObj.code = OCFM_ERR_SUCCESS;
					*sidxPos = sidxLC;
					return errCodeObj;
				}
				else if (sidxLC
						== (idxObj->GetNumberofSubIndexes() - 1))
				{
					// SubIndex Doesn't Exist
					errCodeObj.code = OCFM_ERR_SUBINDEXID_NOT_FOUND;
					return errCodeObj;
				}
				else
				{
					//TODO: operation to be added
				}
			}
		}
		else
		{
			errCodeObj.code = OCFM_ERR_UNKNOWN;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
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

bool CheckIfSubIndexExists(INT32 nodeId, NodeType nodeType, char* indexID, char* subIndexId)
{
	Node nodeObj;
	NodeCollection *nodeCollObj = NULL;
	IndexCollection *idxCollObj = NULL;
	Index *idxObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNode(nodeType, nodeId);
	idxCollObj = nodeObj.GetIndexCollection();
	idxObj = idxCollObj->GetIndexbyIndexValue(indexID);

	if ((0 == idxObj->GetNumberofSubIndexes()))
	{
		return false;
	}
	else if (idxObj->GetNumberofSubIndexes() > 0)
	{
		//Check for existance of the SubIndex
		for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes(); sidxLC++)
		{
			SubIndex* objSubIndexPtr;
			objSubIndexPtr = idxObj->GetSubIndex(sidxLC);

			if ((0
					== strcmp(
							StringToUpper(
									(char*) objSubIndexPtr->GetIndexValue()),
							StringToUpper(subIndexId))))
			{
				return true;
			}
			else if (sidxLC == (idxObj->GetNumberofSubIndexes() - 1))
			{
				return false;
			}
			else
			{
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
	bool retVal;
	if (NULL == value || NULL == name)
	{
		return false;
	}
#if defined DEBUG
	cout << "\nName:" << name << endl;
	cout << "Value:" << value << endl;
#endif
	// Check for Version Tool-Project Version
	if (FALSE == strcmp(ConvertToUpper((char*) name), "VERSION"))
	{
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
	else
	{
		cout << "Error! IfVersionNumberMatches function can't find VERSION"
				<< endl;
		retVal = false;
	}
	return retVal;
}
