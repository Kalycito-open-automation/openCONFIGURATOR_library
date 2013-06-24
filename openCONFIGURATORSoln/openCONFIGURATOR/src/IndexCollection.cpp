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
#include "../Include/IndexCollection.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

IndexCollection::IndexCollection(void)
{
	indexCount = 0;
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
	INT32 itemPosition = indexCollection.Add();
	char* subStr = NULL;
	subStr = SubString((char*) objIndex.GetIndexValue(), 0, 2);
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
		//Nothing to be added.
	}
	indexCollection[itemPosition] = objIndex;
	//cout << "iItemPosition" << iItemPosition << endl;
	indexCount = indexCollection.Count();
}

void IndexCollection::DeleteIndex(INT32 indexPosition)
{
	indexCollection.Remove(indexPosition);
	indexCount = indexCollection.Count();
}

void IndexCollection::DeleteIndexCollection()
{
	indexCollection.Clear();
	indexCount = indexCollection.Count();
}

//TODO: unused function
void IndexCollection::DeletePDOs()
{
	INT32 loopCount;
	char* subStr = new char[SUBINDEX_LEN];
	Index objIndex;
	for (loopCount = 0; loopCount < indexCount; loopCount++)
	{
		objIndex = indexCollection[loopCount];
		subStr = SubString((char*) objIndex.GetIndexValue(), 0, 2);
		if ((0 == strcmp(subStr, "1A")) || (0 == strcmp(subStr, "1a"))
				|| (0 == strcmp(subStr, "14")) || (0 == strcmp(subStr, "16"))
				|| (0 == strcmp(subStr, "18")))
		{
			indexCollection.Remove(loopCount);
			indexCount = indexCollection.Count();
			loopCount = 0;
		}
	}
	delete[] subStr;
}

void IndexCollection::DeletePIObjects()
{
	INT32 iLoopCount = 0;
	char* subStr = new char[2];

	Index objIndex;
	for (iLoopCount = 0; iLoopCount < indexCount; iLoopCount++)
	{
		objIndex = indexCollection[iLoopCount];
		subStr = SubString((char*) objIndex.GetIndexValue(), 0, 1);
		if ((0 == strcmp(subStr, "A")) || (0 == strcmp(subStr, "a")))
		{
			indexCollection.Remove(iLoopCount);
			indexCount = indexCollection.Count();
			iLoopCount = 0;
		}
	}
	delete[] subStr;
}

Index* IndexCollection::GetIndex(INT32 indexPosition)
{
	return &indexCollection[indexPosition];
}

Index* IndexCollection::GetIndexbyIndexValue(char* indexId)
{
	Index objIndex;
	if(NULL != indexId)
	{
		char *idxIdUpper = new char[strlen(indexId) + STR_ALLOC_BUFFER];
		strcpy(idxIdUpper, indexId);
		idxIdUpper = ConvertToUpper(idxIdUpper);

		for (INT32 loopCount = 0; loopCount < indexCount; loopCount++)
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
	return indexCount;
}

