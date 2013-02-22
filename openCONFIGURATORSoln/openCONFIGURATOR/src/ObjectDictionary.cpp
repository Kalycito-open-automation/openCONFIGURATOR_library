/**
 ************************************************************************************************
 \file			ObjectDictionary.cpp

 \brief			Creats object dictionary for each node and Process the Node value,Name and its attributes
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
#include "../Include/ObjectDictionary.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"

using namespace std;

/****************************************************************************************************/
/* Global Variables */

INT32 iLastObjDictIndexParsed = 0;
bool CObjectDictionary::instanceFlag = false;
CObjectDictionary* CObjectDictionary::objectDictionary = NULL;
CNode* CObjectDictionary::objDictNode = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 
 @param void
 */

CObjectDictionary::CObjectDictionary(void)
{
	objDictNode = new CNode();
	objDictNode->setNodeId(-100);
	objDictNode->CreateIndexCollection();
	objDictNode->CreateDataTypeCollection();
	objDictNode->CreateApplicationProcess();
	objDictNode->CreateNetworkManagament();
	m_s_attrIdx_SIdx = collectionObj.Count();
}

/*************************************************************************/
/* Destructor */

/**
 
 @param void
 */

CObjectDictionary::~CObjectDictionary(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief			getObjDictPtr
 
 This is a member function of CObjectDictionary returns object dictionary based on the value of instance flag 

 \return		CObjectDictionary*
 */
/*****************************************************************************/

CObjectDictionary* CObjectDictionary::getObjDictPtr()
{
	if (!instanceFlag)
	{
		objectDictionary = new CObjectDictionary();
		instanceFlag = true;
	}
	return objectDictionary;
}

/*****************************************************************************/
/**
 \brief			ProcessObjectDictionary
 
 This is a member function of CObjectDictionary Process the Node value,Name and its attributes
 
 \param			reader  xml Variable of type xmlTextReaderPtr 
 \return		void
 */
/*****************************************************************************/

void CObjectDictionary::ProcessObjectDictionary(xmlTextReaderPtr reader)
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
				CDataTypeCollection* objDataTypeCollection = NULL;
				DataType objDataType;
				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						setDataTypeAttributes(reader, &objDataType);
					}
				}
				objDataTypeCollection = objDictNode->getDataTypeCollection();
				objDataTypeCollection->addDataType(objDataType);
			}

			else if (0 == strcmp(((char*) name), "Object"))
			{
				CIndexCollection* objIndexCollection = NULL;
				CIndex objIndex;

				//Set the NodeID
				objIndex.setNodeID(-100);

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
									objDictNode->getDataTypeCollection()->getDataType(
											(char*) value);
							objIndex.setDataTypeST(*dt);
						}
						else if (0
								== strcmp(ConvertToUpper((char*) name),
										"RANGE"))
						{
							createSameattrObject((char*) value, INDEX,
									(char*) objIndex.getIndexValue());
						}
						else
						{
							bool hasPDO;
							setIndexAttributes(reader, &objIndex, hasPDO);
						}
					}
				}
				objIndexCollection = objDictNode->getIndexCollection();

				//Add Index object to the IndexCollection
				objIndexCollection->addIndex(objIndex);
				iLastObjDictIndexParsed =
						objIndexCollection->getNumberofIndexes() - 1;
			}
			else if (0 == strcmp(((char*) name), "SubObject"))
			{
				CIndexCollection* objIndexCollection = NULL;
				CSubIndex objSubIndex;
				//bool same = false;
				CIndex* objIndexPtr = NULL;
				objIndexCollection = objDictNode->getIndexCollection();
				objIndexPtr = objIndexCollection->getIndex(
						iLastObjDictIndexParsed);

				//Set the NodeID
				objSubIndex.setNodeID(-100);
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
									objDictNode->getDataTypeCollection()->getDataType(
											(char*) value);
							objSubIndex.setDataTypeST(*dt);
						}
						else if (0
								== strcmp(ConvertToUpper((char*) name),
										"RANGE"))
						{
							//same =true;
							createSameattrObject((char*) value, SUBINDEX,
									(char*) objIndexPtr->getIndexValue());
						}
						else
						{
							setSubIndexAttributes(reader, &objSubIndex);
						}
					}
				}
				objIndexPtr->addSubIndex(objSubIndex);
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

/*****************************************************************************/
/**
 \brief			addSameAttributesObjects
 
 This is a member function of CObjectDictionary add the SubIndex in the Index Object
 
 \param			stAttrIdx Structure Variable of s_attrIdx_SIdx to hold the attribute index
 \return		void
 */
/*****************************************************************************/

void CObjectDictionary::addSameAttributesObjects(s_attrIdx_SIdx stAttrIdx)
{
	INT32 iItemPosition = collectionObj.Add();

	collectionObj[iItemPosition] = stAttrIdx;
	m_s_attrIdx_SIdx = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			createSameattrObject
 
 This is a member function of CObjectDictionary creats object dictionary
 
 \param			pbValue          Character Pointer Variable to hold the value of the object
 \param			enumObjType      Enum Variable of  ObjectType to hold the value of Object type
 \param			pbIdx            Character Pointer Variable to hold the value of Index
 \return		void
 */
/*****************************************************************************/

void CObjectDictionary::createSameattrObject(char* pbValue,
		ObjectType enumObjType, char* pbIdx)
{
	s_attrIdx_SIdx stAttrIdx;
	char* pbSubIdx = new char[RANGE_INDEX];
	char* pbEIdx = NULL;

	pbEIdx = strchr(pbValue, '-');
	if (NULL != pbEIdx)
	{
		pbSubIdx = subString(pbValue, 0, strlen(pbEIdx) - 1);
		pbEIdx = subString(pbValue, strlen(pbSubIdx) + 1, strlen(pbValue));

		stAttrIdx.objectType = enumObjType;
		stAttrIdx.Idx = new char[INDEX_LEN];

		if (enumObjType == INDEX)
		{
			stAttrIdx.start_Index = new char[INDEX_LEN];
			stAttrIdx.end_Index = new char[INDEX_LEN];
			strcpy(stAttrIdx.end_Index,
					subString(pbIdx, 0, 4 - strlen(pbSubIdx)));
			strcat(stAttrIdx.end_Index, pbEIdx);
			strcpy(stAttrIdx.start_Index, pbIdx);
		}
		else
		{
			stAttrIdx.start_Index = new char[SUBINDEX_LEN];
			stAttrIdx.end_Index = new char[SUBINDEX_LEN];
			strcpy(stAttrIdx.start_Index, pbSubIdx);
			strcpy(stAttrIdx.end_Index, pbEIdx);
		}

		strcpy(stAttrIdx.Idx, pbIdx);
		addSameAttributesObjects(stAttrIdx);
	}
	delete[] pbSubIdx;
}

/*****************************************************************************/
/**
 \brief			getObjectDictIndex
 
 This is a member function of CObjectDictionary returns index value of the object dictionary
 
 \param			pbIdx            Character Pointer Variable to hold the value of Index
 
 \return		CIndex*
 */
/*****************************************************************************/

CIndex* CObjectDictionary::getObjectDictIndex(char* pbIdx)
{
	CIndex* pobjIndex = NULL;
	CIndexCollection* pobjIndexCol = NULL;
	//CIndex objIndex;

	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex = pobjIndexCol->getIndexbyIndexValue(pbIdx);

	if (NULL != pobjIndex)
	{
		return pobjIndex;
	}
	else
	{
		for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count();
				iLoopCount++)
		{
			s_attrIdx_SIdx stAttrIdx;
			stAttrIdx = collectionObj[iLoopCount];

			if (stAttrIdx.objectType == INDEX)
			{
				if (checkInTheRange(pbIdx, stAttrIdx.start_Index,
						stAttrIdx.end_Index))
				{
					pobjIndex = pobjIndexCol->getIndexbyIndexValue(
							stAttrIdx.start_Index);
					return pobjIndex;
				}
			}
		}
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			getObjectDictSubIndex
 
 This is a member function of CObjectDictionary returns the object dictonary for sub index
 
 \param			pbIdx       Character pointer variable to hold the value of Index
 \param			pbSIdx      Character pointer variable to hold the value of Sub-Index
 \return		CIndex*
 */
/*****************************************************************************/

CSubIndex* CObjectDictionary::getObjectDictSubIndex(char* pbIdx, char* pbSIdx)
{
	CSubIndex* pobjSIdx = NULL;
	CIndex* pobjIndex = NULL;
	CIndexCollection* pobjIndexCol = NULL;

	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex = pobjIndexCol->getIndexbyIndexValue(pbIdx);

	if (NULL == pobjIndex)
	{
		pobjIndex = getObjectDictIndex(pbIdx);

		if (NULL != pobjIndex)
		{
			pbIdx = (char*) pobjIndex->getIndexValue();
		}
		else
		{
			return pobjSIdx;
		}
	}

	pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);

	if (NULL != pobjSIdx)
	{
		return pobjSIdx;
	}
	else
	{
		for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count();
				iLoopCount++)
		{
			s_attrIdx_SIdx stAttrIdx;
			stAttrIdx = collectionObj[iLoopCount];

			if ((stAttrIdx.objectType == SUBINDEX)
					&& (strcmp(stAttrIdx.Idx, pbIdx) == 0))
			{
				if (checkInTheRange(pbSIdx, stAttrIdx.start_Index,
						stAttrIdx.end_Index))
				{
					pobjSIdx = pobjIndex->getSubIndexbyIndexValue(
							stAttrIdx.start_Index);
					if (NULL != pobjSIdx)
					{
						return pobjSIdx;
					}
				}
			}
		}
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			checkInTheRange
 
 This is a member function of CObjectDictionary returns the range of the object dictionary ids
 
 \param			pbIdx              Character Pointer Variable to hold the value of Index
 \param			pbStartIdx         Character Pointer Variable to hold the value of Start Index 
 \param			pbEndIdx           Character Pointer Variable to hold the value of End Index
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool CObjectDictionary::checkInTheRange(char* pbIdx, char* pbStartIdx,
		char* pbEndIdx)
{
	if (hex2int(pbIdx) >= hex2int(pbStartIdx)
			&& (hex2int(pbIdx) <= hex2int(pbEndIdx)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/*****************************************************************************/
/**
 \brief			printall
 
 This is a member function of CObjectDictionary collects the object ids,start index and end index
 
 \return		void
 */
/*****************************************************************************/
//TODO: Unused function
void CObjectDictionary::printall()
{
	for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		s_attrIdx_SIdx stAttrIdx;
		stAttrIdx = collectionObj[iLoopCount];
	}
}

/*****************************************************************************/
/**
 \brief			ifObjectDictIndexExists
 
 This is a member function of CObjectDictionaryreturns the obj dictionary value from the collection list based on the index value
 
 \param			pbIdx		Character Pointer Variable to hold the value of index		
 \return		INT32
 */
/*****************************************************************************/
//TODO: Unused function
INT32 CObjectDictionary::ifObjectDictIndexExists(char* pbIdx)
{
	CIndex* pobjIndex = NULL;
	CIndexCollection* pobjIndexCol;

	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex = pobjIndexCol->getIndexbyIndexValue(pbIdx);

	if (NULL == pobjIndex)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
/**
 \brief			ifObjectDictSubIndexExists
 
 This is a member function of CObjectDictionary returns the obj dictionary value from the collection list based on the subindex value
 
 \param			pbIdx      Character Pointer Variable to hold the value of index		
 \param			pbSIdx	   Character Pointer Variable to hold the value of Subindex		
 \return		INT32
 */
/*****************************************************************************/
//TODO: Unused function
INT32 CObjectDictionary::ifObjectDictSubIndexExists(char* pbIdx, char* pbSIdx)
{
	CSubIndex* pobjSIdx = NULL;
	CIndex* pobjIndex = NULL;
	CIndexCollection* pobjIndexCol;

	pobjIndexCol = objDictNode->getIndexCollection();
	pobjIndex = pobjIndexCol->getIndexbyIndexValue(pbIdx);

	if (NULL == pobjIndex)
	{
		return TRUE;
	}

	pobjSIdx = pobjIndex->getSubIndexbyIndexValue(pbSIdx);

	if (NULL == pobjSIdx)
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

/*****************************************************************************/
/**
 \brief			getIndexName
 
 This is a member function of CObjectDictionary  returns the index name
 
 \param			pbObjectIndex  Character Pointer Variable to hold the value of Object index
 \param			pbObjectName   Character Pointer Variable to hold the value of Object name				
 \return		char*
 */
/*****************************************************************************/

char* CObjectDictionary::getIndexName(char* pbObjectIndex, char* pbObjectName)
{
	char* pbName = NULL;
	char* pbModifiedName = new char[strlen(pbObjectName) + STR_ALLOC_BUFFER];

	INT32 iLen = 0;
	if (NULL == pbObjectName)
	{
		return NULL;
	}

	pbName = strchr(pbObjectName, 'X');
	if (NULL != pbName)
	{
		iLen = 1;
		if (0 == strcmp(subString(pbName, 1, 1), "X"))
		{
			iLen++;
		}

		INT32 pos = strlen(pbName);
		INT32 iCount = strlen(pbObjectIndex) - iLen;
		strcpy(pbModifiedName,
				subString(pbObjectName, 0, strlen(pbObjectName) - pos));
		strcat(pbModifiedName, subString(pbObjectIndex, iCount, iLen));
		strcat(pbModifiedName, subString(pbName, iLen, strlen(pbName) - iLen));
		return pbModifiedName;
	}
	else
	{
		return pbObjectName;
	}
}
