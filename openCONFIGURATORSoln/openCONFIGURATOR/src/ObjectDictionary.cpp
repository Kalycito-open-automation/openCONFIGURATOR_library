/**
 *****************************************************************************
 \file		ObjectDictionary.cpp

 \brief		This file contains the class member definitions to handle operations on the object dictionary template od.xml which is used as a template for creating objects for all nodes
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

/****************************************************************************/
/* Includes */

#include <iostream>
#include "../Include/ObjectDictionary.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"

using namespace std;

/****************************************************************************/
/* Global Variables */

INT32 lastObjDictIdxParsed = 0;
bool ObjectDictionary::instanceFlag = false;
ObjectDictionary* ObjectDictionary::objectDictionary = NULL;
Node* ObjectDictionary::objDictNode = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

ObjectDictionary::ObjectDictionary(void)
{
	objDictNode = new Node();
	objDictNode->SetNodeId(-100);
	objDictNode->CreateIndexCollection();
	objDictNode->CreateDataTypeCollection();
	objDictNode->CreateApplicationProcess();
	objDictNode->CreateNetworkManagement();
	varsattrIdxSIdx = attribCollObj.Count();
}

/*************************************************************************/
/* Destructor */

/**
 

 */

ObjectDictionary::~ObjectDictionary(void)
{
	//Add destructor code here
}

ObjectDictionary* ObjectDictionary::GetObjDictPtr()
{
	if (!instanceFlag)
	{
		objectDictionary = new ObjectDictionary();
		instanceFlag = true;
	}
	return objectDictionary;
}

void ObjectDictionary::ProcessObjectDictionary(xmlTextReaderPtr reader)
{
	const xmlChar *name;
	const xmlChar *value;
	name = xmlTextReaderConstName(reader);

	value = xmlTextReaderConstValue(reader);
	try
	{
		if ((NULL == name) || (NULL == value))
		{
			//cout << "Got NULL for name or value\n" << endl;
		}

		//If the NodeTYPE is ELEMENT
		if (1 == xmlTextReaderNodeType(reader))
		{
			if (0 == strcmp(((char*) name), "defType"))
			{
				DataTypeCollection* dtCollObj = NULL;
				DataType dtObj;
				dtObj.dataTypeValue = NULL;
				dtObj.dataTypeName = NULL;

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetDataTypeAttributes(reader, &dtObj);
					}
				}
				dtCollObj = objDictNode->GetDataTypeCollection();
				dtCollObj->AddDataType(dtObj);
			}

			else if (0 == strcmp(((char*) name), "Object"))
			{
				IndexCollection* idxCollObj = NULL;
				Index idxObj;

				//Set the NodeID
				idxObj.SetNodeID(-100);

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						value = xmlTextReaderConstValue(reader);
						name = xmlTextReaderConstName(reader);

						if (0
								== strcmp(ConvertToUpper((char*) name),
										"DATATYPE"))
						{
							DataType *dt = NULL;
							dt =
									objDictNode->GetDataTypeCollection()->GetDataType(
											(char*) value);
							idxObj.SetDataTypeST(*dt);
						}
						else if (0
								== strcmp(ConvertToUpper((char*) name),
										"RANGE"))
						{
							CreateSameattrObject((char*) value, INDEX,
									(char*) idxObj.GetIndexValue());
						}
						else
						{
							bool hasPDO;
							SetIndexAttributes(reader, &idxObj, hasPDO);
						}
					}
				}
				idxCollObj = objDictNode->GetIndexCollection();

				//Add Index object to the IndexCollection
				idxCollObj->AddIndex(idxObj);
				lastObjDictIdxParsed = idxCollObj->GetNumberofIndexes() - 1;
			}
			else if (0 == strcmp(((char*) name), "SubObject"))
			{
				IndexCollection* idxCollObj = NULL;
				SubIndex sidxObj;
				//bool same = false;
				Index* idxObj = NULL;
				idxCollObj = objDictNode->GetIndexCollection();
				idxObj = idxCollObj->GetIndex(lastObjDictIdxParsed);

				//Set the NodeID
				sidxObj.SetNodeID(-100);
				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						value = xmlTextReaderConstValue(reader);
						name = xmlTextReaderConstName(reader);
						if (0
								== strcmp(ConvertToUpper((char*) name),
										"DATATYPE"))
						{
							DataType *dt = NULL;
							dt =
									objDictNode->GetDataTypeCollection()->GetDataType(
											(char*) value);
							sidxObj.SetDataTypeST(*dt);
						}
						else if (0
								== strcmp(ConvertToUpper((char*) name),
										"RANGE"))
						{
							CreateSameattrObject((char*) value, SUBINDEX,
									(char*) idxObj->GetIndexValue());
						}
						else
						{
							SetSubIndexAttributes(reader, &sidxObj);
						}
					}
				}
				idxObj->AddSubIndex(sidxObj);
			}
			else
			{
				//TODO: Else added. Operation to be specified
			}
		}
	} catch (ocfmException& ex)
	{
		throw ex;
	}
}

void ObjectDictionary::AddSameAttributesObjects(sattrIdxSIdx attrIdxPos)
{
	INT32 itemPos = attribCollObj.Add();

	attribCollObj[itemPos] = attrIdxPos;
	varsattrIdxSIdx = attribCollObj.Count();
}

void ObjectDictionary::CreateSameattrObject(char* value, ObjectType objType,
		char* idxId)
{
	sattrIdxSIdx stAttrIdx;
	char* subIdx = new char[RANGE_INDEX];
	char* idx = NULL;

	idx = strchr(value, '-');
	if (NULL != idx)
	{
		subIdx = SubString(value, 0, strlen(idx) - 1);
		idx = SubString(value, strlen(subIdx) + 1, strlen(value));

		stAttrIdx.objectType = objType;
		stAttrIdx.Idx = new char[INDEX_LEN];

		if (objType == INDEX)
		{
			stAttrIdx.startIndex = new char[INDEX_LEN];
			stAttrIdx.endIndex = new char[INDEX_LEN];
			strcpy(stAttrIdx.endIndex, SubString(idxId, 0, 4 - strlen(subIdx)));
			strcat(stAttrIdx.endIndex, idx);
			strcpy(stAttrIdx.startIndex, idxId);
		}
		else
		{
			stAttrIdx.startIndex = new char[SUBINDEX_LEN];
			stAttrIdx.endIndex = new char[SUBINDEX_LEN];
			strcpy(stAttrIdx.startIndex, subIdx);
			strcpy(stAttrIdx.endIndex, idx);
		}

		strcpy(stAttrIdx.Idx, idxId);
		AddSameAttributesObjects(stAttrIdx);
	}
	delete[] subIdx;
}

Index* ObjectDictionary::GetObjectDictIndex(char* indexId)
{
	Index* idxObj = NULL;
	IndexCollection* idxCollObj = NULL;
	//CIndex objIndex;

	idxCollObj = objDictNode->GetIndexCollection();
	idxObj = idxCollObj->GetIndexbyIndexValue(indexId);

	if (NULL != idxObj)
	{
		return idxObj;
	}
	else
	{
		for (INT32 iLoopCount = 0; iLoopCount < attribCollObj.Count();
				iLoopCount++)
		{
			sattrIdxSIdx stAttrIdx;
			stAttrIdx = attribCollObj[iLoopCount];

			if (stAttrIdx.objectType == INDEX)
			{
				if (CheckInTheRange(indexId, stAttrIdx.startIndex,
						stAttrIdx.endIndex))
				{
					idxObj = idxCollObj->GetIndexbyIndexValue(
							stAttrIdx.startIndex);
					return idxObj;
				}
			}
		}
		return NULL;
	}
}

SubIndex* ObjectDictionary::GetObjectDictSubIndex(char* idxId, char* sIdxId)
{
	SubIndex* sidxObj = NULL;
	Index* idxObj = NULL;
	IndexCollection* objIndexCol = NULL;

	objIndexCol = objDictNode->GetIndexCollection();
	idxObj = objIndexCol->GetIndexbyIndexValue(idxId);

	if (NULL == idxObj)
	{
		idxObj = GetObjectDictIndex(idxId);

		if (NULL != idxObj)
		{
			idxId = (char*) idxObj->GetIndexValue();
		}
		else
		{
			return sidxObj;
		}
	}

	sidxObj = idxObj->GetSubIndexbyIndexValue(sIdxId);

	if (NULL != sidxObj)
	{
		return sidxObj;
	}
	else
	{
		for (INT32 attribLC = 0; attribLC < attribCollObj.Count(); attribLC++)
		{
			sattrIdxSIdx attribObj;
			attribObj = attribCollObj[attribLC];

			if ((attribObj.objectType == SUBINDEX)
					&& (strcmp(attribObj.Idx, idxId) == 0))
			{
				if (CheckInTheRange(sIdxId, attribObj.startIndex,
						attribObj.endIndex))
				{
					sidxObj = idxObj->GetSubIndexbyIndexValue(
							attribObj.startIndex);
					if (NULL != sidxObj)
					{
						return sidxObj;
					}
				}
			}
		}
		return NULL;
	}
}

bool ObjectDictionary::CheckInTheRange(char* idxId, char* startIdx,
		char* endIdx)
{
	if (HexToInt(idxId) >= HexToInt(startIdx)
			&& (HexToInt(idxId) <= HexToInt(endIdx)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

//TODO: Unused function
INT32 ObjectDictionary::IfObjectDictIndexExists(char* idxId)
{
	Index* objIndex = NULL;
	IndexCollection* objIndexCol;

	objIndexCol = objDictNode->GetIndexCollection();
	objIndex = objIndexCol->GetIndexbyIndexValue(idxId);

	if (NULL == objIndex)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

//TODO: Unused function
INT32 ObjectDictionary::IfObjectDictSubIndexExists(char* idxId, char* sIdxId)
{
	SubIndex* objSIdx = NULL;
	Index* objIndex = NULL;
	IndexCollection* objIndexCol;

	objIndexCol = objDictNode->GetIndexCollection();
	objIndex = objIndexCol->GetIndexbyIndexValue(idxId);

	if (NULL == objIndex)
	{
		return TRUE;
	}

	objSIdx = objIndex->GetSubIndexbyIndexValue(sIdxId);

	if (NULL == objSIdx)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

char* ObjectDictionary::GetIndexName(char* idxId, char* objectName)
{
	char* tempObjName = NULL;

	if (NULL == objectName)
	{
		return NULL;
	}

	tempObjName = strchr(objectName, 'X');
	if (NULL != tempObjName)
	{
		INT32 len = 0;
		len = 1;
		if (0 == strcmp(SubString(tempObjName, 1, 1), "X"))
		{
			len++;
		}

		INT32 pos = strlen(tempObjName);
		INT32 count = strlen(idxId) - len;
		char* modifiedName = new char[strlen(objectName) + STR_ALLOC_BUFFER];
		strcpy(modifiedName,
				SubString(objectName, 0, strlen(objectName) - pos));
		strcat(modifiedName, SubString(idxId, count, len));
		strcat(modifiedName,
				SubString(tempObjName, len, strlen(tempObjName) - len));
		return modifiedName;
	}
	else
	{
		return objectName;
	}
}
