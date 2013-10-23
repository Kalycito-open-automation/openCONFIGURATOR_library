/**
 ******************************************************************************
 \file		IndexCollection.cpp

 \brief		This file contains the class member definitions to handle the properties of objects of the Index Collection
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

/****************************************************************************************************/
/* Includes */

#include <stdio.h>
#include <sstream>
#include <algorithm>

#include "../Include/IndexCollection.h"
#include "../Include/Internal.h"
#include "../Include/Logging.h"
#include "../Include/BoostShared.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

IndexCollection::IndexCollection(void)
{

}

/*************************************************************************/
/* Destructor */

/**
 

 */

IndexCollection::~IndexCollection(void)
{
	//Add destructor code here
}

void IndexCollection::AddIndex(Index objIndex)
{
	
	char* subStr = new char[SUBINDEX_LEN];
	subStr = SubString(subStr, objIndex.GetIndexValue(), 0, 2);
	if ((0 == strcmp(subStr, "14")) || (0 == strcmp(subStr, "16")))
	{
		objIndex.SetPDOType(PDO_RPDO);
	}
	else if ((0 == strcmp(subStr, "18")) || (0 == strcmp(subStr, "1A"))
			|| (0 == strcmp(subStr, "1a")))
	{
		objIndex.SetPDOType(PDO_TPDO);
	}
	else
	{
		objIndex.SetPDOType(PDO_INVALID);
	}
	delete[] subStr;
	
	indexCollection.push_back(objIndex);
}

void IndexCollection::DeleteIndex(INT32 indexPosition)
{
	indexCollection.erase(indexCollection.begin() + indexPosition);
}

void IndexCollection::DeleteIndexCollection()
{
	indexCollection.clear();
}

//TODO: unused function
void IndexCollection::DeletePDOs()
{
	UINT32 loopCount;
	char* subStr = new char[SUBINDEX_LEN];
	Index objIndex;
	for (loopCount = 0; loopCount < indexCollection.size(); loopCount++)
	{
		objIndex = indexCollection[loopCount];
		subStr = SubString(subStr, objIndex.GetIndexValue(), 0, 2);
		if ((0 == strcmp(subStr, "1A")) || (0 == strcmp(subStr, "1a"))
				|| (0 == strcmp(subStr, "14")) || (0 == strcmp(subStr, "16"))
				|| (0 == strcmp(subStr, "18")))
		{
			indexCollection.erase(indexCollection.begin() + loopCount);
			loopCount = 0;
		}
	}
	delete[] subStr;
}

void IndexCollection::DeletePIObjects()
{
	UINT32 iLoopCount = 0;
	char* subStr = new char[1+1];

	Index objIndex;
	for (iLoopCount = 0; iLoopCount < indexCollection.size(); iLoopCount++)
	{
		objIndex = indexCollection[iLoopCount];
		subStr = SubString(subStr, objIndex.GetIndexValue(), 0, 1);
		if ((0 == strcmp(subStr, "A")) || (0 == strcmp(subStr, "a")))
		{
			indexCollection.erase(indexCollection.begin() + iLoopCount);
			iLoopCount = 0;
		}
	}
	delete[] subStr;
}

Index* IndexCollection::GetIndexByPosition(INT32 indexPosition)
{
	return &indexCollection[indexPosition];
}

Index* IndexCollection::GetIndexbyIndexValue(const char* indexId)
{
	Index objIndex;
	if(NULL != indexId)
	{
		char *idxIdUpper = new char[strlen(indexId) + STR_ALLOC_BUFFER];
		strcpy(idxIdUpper, indexId);
		idxIdUpper = ConvertToUpper(idxIdUpper);

		for (UINT32 loopCount = 0; loopCount < indexCollection.size(); loopCount++)
		{
			objIndex = indexCollection[loopCount];
			//NULL check values 
			if(objIndex.GetIndexValue() != NULL)
			{
				char *objIdxIdUpper = new char[strlen(objIndex.GetIndexValue()) + STR_ALLOC_BUFFER];
				strcpy(objIdxIdUpper, objIndex.GetIndexValue());
				objIdxIdUpper = ConvertToUpper(objIdxIdUpper);
				if (0 == strcmp(objIdxIdUpper, idxIdUpper))
				{
					delete[] idxIdUpper;
					delete[] objIdxIdUpper;
					return &indexCollection[loopCount];
				}
				else
				{
					delete[] objIdxIdUpper;
				}
			}
		}
		delete[] idxIdUpper;
	}
	return NULL;
}

INT32 IndexCollection::GetNumberofIndexes()
{
	return indexCollection.size();
}

bool IndexCollection::ContainsIndex(const UINT32 index, const UINT32 subIndex)
{
	Index* indexPtr = this->GetIndexPtr(index);
	// Index not found
	if (!indexPtr)
		return false;
	// Index has no subIndices and subIndex 0 was requested
	if (subIndex == 0 && !indexPtr->HasSubIndices())
		return true;
	// Index has subIndices and subIndex != 0 was requested
	return indexPtr->ContainsSubIndex(subIndex);
}

Index* IndexCollection::GetIndexPtr(const UINT32 index)
{
	vector<Index>::iterator it = this->indexCollection.begin();
	for (;
		it != this->indexCollection.end() && (it->GetIndex() != index);
		++it)
	{}
	if (it == this->indexCollection.end())
		return NULL;
	else
		return &(*it);
}

Index& IndexCollection::GetIndexRef(const UINT32 index)
{
	Index* indexPtr = this->GetIndexPtr(index);
	if (indexPtr)
	{
		return *indexPtr;
	}
	else
	{
		boost::format formatter("Index %#x does not exist in OD.");
		formatter % index;
		ocfmRetCode result(OCFM_ERR_INDEXID_NOT_FOUND);
		result.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw result;
	}
}

bool IndexCollection::IsEmpty() const
{
	return this->indexCollection.empty();
}

UINT32 IndexCollection::Size() const
{
	return this->indexCollection.size();
}
