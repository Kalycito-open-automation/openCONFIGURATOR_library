/**
 ******************************************************************************
 \file		Index.cpp

 \brief		This file contains the class member definitions to handle the properties of an Index Object. This is inherited from the BaseIndex class
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
#include "../Include/Index.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E C L A R A T I O N S 					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

Index::Index(void)
{
	sidxCount = subIndexCollection.Count();
	//pdoType = 0; //TODO: Review Initialisation
}

/*************************************************************************/
/* Destructor */

/**
 

 */

Index::~Index(void)
{
	//Add destructor code here
}

PDOType Index::GetPDOType()
{
	return pdoType;
}

void Index::SetPDOType(PDOType varPDOType)
{
	pdoType = varPDOType;
}

#ifndef __GNUC__
#pragma region MemberFunctions
#endif

void Index::AddSubIndex(SubIndex objSubIndex)
{
	INT32 iItemPosition = subIndexCollection.Add();
	subIndexCollection[iItemPosition] = objSubIndex;
	sidxCount = subIndexCollection.Count();
#if defined DEBUG
	cout << __FUNCTION__ << "\t Index" << Index::GetIndexValue();
	cout << "\tSubindex:" << objSubIndex.GetIndexValue() << endl;
#endif
}

void Index::DeleteSubIndex(INT32 subIndexPosition)
{
	subIndexCollection.Remove(subIndexPosition);
	sidxCount = subIndexCollection.Count();
}

void Index::DeleteSubIndexCollection()
{
	subIndexCollection.Clear();
	sidxCount = subIndexCollection.Count();
}

INT32 Index::GetNumberofSubIndexes()
{
	return subIndexCollection.Count();
}

SubIndex* Index::GetSubIndex(INT32 subIndexPosition)
{
	return &subIndexCollection[subIndexPosition];
}

SubIndex* Index::GetSubIndexbyIndexValue(char* subIndexId)
{
	SubIndex* objSidx = NULL;

	if( NULL != subIndexId)
	{
		char *sidxIdUpper = new char[strlen(subIndexId) + STR_ALLOC_BUFFER];
		strcpy(sidxIdUpper, subIndexId);
		sidxIdUpper = ConvertToUpper(sidxIdUpper);

		for (INT32 loopCount = 0; loopCount < subIndexCollection.Count(); loopCount++)
		{
			objSidx = &subIndexCollection[loopCount];
			//Check for null , alignment changes
			if (NULL != objSidx)
			{
				if (objSidx->GetIndexValue() != NULL)
				{
					char *objSidxIdUpper = new char[strlen(objSidx->GetIndexValue()) + STR_ALLOC_BUFFER];
					strcpy(objSidxIdUpper, objSidx->GetIndexValue());
					objSidxIdUpper = ConvertToUpper(objSidxIdUpper);
					if (0 == strcmp(objSidxIdUpper, sidxIdUpper))
					{
						delete[] objSidxIdUpper;
						delete[] sidxIdUpper;
						return objSidx;
					}
					else
					{
						delete[] objSidxIdUpper;
						objSidx = NULL;
					}
				}
			}
		}
		delete[] sidxIdUpper;
	}

	objSidx = NULL;
	return objSidx;
}

void Index::SwapSubObjects(INT32 fromPosition, INT32 toPosition)
{
	swap(subIndexCollection[fromPosition], subIndexCollection[toPosition]);
}

void Index::UpdateArraySubObjects()
{
	INT32 loopCount = 0;
	SubIndex* objSidx = NULL;

	for (loopCount = 0; loopCount < subIndexCollection.Count(); loopCount++)
	{
		objSidx = &subIndexCollection[loopCount];
		//Check for null , alignment changes
		if ((NULL != objSidx->GetIndexValue())
				&& 0 == (strcmp(objSidx->GetIndexValue(), "00")))
			continue;
		objSidx->SetDataTypeST(this->GetDataType());
	}
	objSidx = NULL;
}
#ifndef __GNUC__
#pragma endregion MemberFunctions
#endif
