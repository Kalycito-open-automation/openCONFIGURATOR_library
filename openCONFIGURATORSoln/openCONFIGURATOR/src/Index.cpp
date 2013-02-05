///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  Index.cpp
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:  Kalycito Powerlink Team
//
//  COPYRIGHT NOTICE:
//
//	****************************************************************************

// (c) Kalycito Infotech Private Limited
//
//  Project:      openCONFIGURATOR 
//
//  Description:  
//
//
//  License:
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions
//    are met:
//
//    1. Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//    3. Neither the name of Kalycito Infotech Private Limited nor the names of 
//       its contributors may be used to endorse or promote products derived
//       from this software without prior written permission. For written
//       permission, please contact info@kalycito.com.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//    POSSIBILITY OF SUCH DAMAGE.
//
//    Severability Clause:
//
//        If a provision of this License is or becomes illegal, invalid or
//        unenforceable in any jurisdiction, that shall not affect:
//        1. the validity or enforceability in that jurisdiction of any other
//           provision of this License; or
//        2. the validity or enforceability in other jurisdictions of that or
//           any other provision of this License.
//
//****************************************************************************/
//
//  REVISION HISTORY:
// $Log:      $
///////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/
#include <stdio.h>
#include "../Include/Index.h"
#include "../Include/Internal.h"

/****************************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************************/

/****************************************************************************************************
 * Constructor
 ****************************************************************************************************/
CIndex::CIndex(void)
{
	m_SubIndexCount = collectionObj.Count();
	//m_PDOType = 0; //TODO: Review Initialisation
}

/****************************************************************************************************
 * Destructor
 ****************************************************************************************************/
CIndex::~CIndex(void)
{
	//Add destructor code here
}

/****************************************************************************************************
 * Function Name: CIndex::getPDOType
 * Description:  This function returns PDO type 
 * Return value: EPDOType
 ****************************************************************************************************/
EPDOType CIndex::getPDOType()
{
	return m_PDOType;
}

/****************************************************************************************************
 * Function Name: CIndex::setPDOType
 * Description: This function assigns PDO type
 * Return value: void
 ****************************************************************************************************/
void CIndex::setPDOType(EPDOType enumPDOType)
{
	m_PDOType = enumPDOType;
}

#ifndef __GNUC__
#pragma region MemberFunctions
#endif
/****************************************************************************************************
 * Function Name: CIndex::addSubIndex
 * Description: add the SubIndex in the Index Object
 * Return value: void
 ****************************************************************************************************/
void CIndex::addSubIndex(CSubIndex objSubIndex)
{
	cout << "addSubIndex function";
	INT32 iItemPosition = collectionObj.Add();
	collectionObj[iItemPosition] = objSubIndex;
	cout << "\tSubindex:" << objSubIndex.getIndexValue();
	m_SubIndexCount = collectionObj.Count();
	cout << "\t Index" << CIndex::getIndexValue() << endl;
}

/****************************************************************************************************
 * Function Name: CIndex::deleteSubIndex
 * Description: delete the SubIndex in the Index Object
 * Return value: void
 ****************************************************************************************************/
void CIndex::deleteSubIndex(INT32 iSubIndexID)
{
	collectionObj.Remove(iSubIndexID);
	m_SubIndexCount = collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CIndex::deleteSubIndexCollection
 * Description: delete the SubIndex in the Index Object
 * Return value: void
 ****************************************************************************************************/
void CIndex::deleteSubIndexCollection()
{
	collectionObj.Clear();
	m_SubIndexCount = collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CIndex::getNumberofSubIndexes
 * Description: This function returns number of subindexes
 * Return value: INT32
 ****************************************************************************************************/
INT32 CIndex::getNumberofSubIndexes()
{
	return collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CIndex::getSubIndex
 * Description: This function returns number of subindex ids
 * Return value: CSubIndex*
 ****************************************************************************************************/
CSubIndex* CIndex::getSubIndex(INT32 iSubIndexId)
{
	return &collectionObj[iSubIndexId];
}

/****************************************************************************************************
 * Function Name: CIndex::getSubIndexbyIndexValue
 * Description: This function returns number of subindex value 
 * Return value: CSubIndex*
 ****************************************************************************************************/
CSubIndex* CIndex::getSubIndexbyIndexValue(char* pbIndex)
{
	INT32 iLoopCount = 0;
	CSubIndex* pobjSIndex = NULL;

	cout << __FUNCTION__ << " : Loop running " << collectionObj.Count()
			<< "times" << endl;
	for (iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		pobjSIndex = &collectionObj[iLoopCount];
		//Check for null , alignment changes
		if (0
				== strcmp(StringToUpper((char*) pobjSIndex->getIndexValue()),
						StringToUpper(pbIndex)))
		{
			return pobjSIndex;
		}
		else
		{
			pobjSIndex = NULL;
		}
	}

	pobjSIndex = NULL;
	return pobjSIndex;
}
/****************************************************************************************************
 * Function Name: CIndex::SwapSubObjects
 * Description  : This Function swaps the subobjects  
 * Return value : void
 ****************************************************************************************************/
void CIndex::SwapSubObjects(INT32 ipos1, INT32 ipos2)
{
	swap(collectionObj[ipos1], collectionObj[ipos2]);
}
/****************************************************************************************************
 * Function Name: CIndex::UpdateArraySubObjects
 * Description  : This function updates subobjects in array  
 * Return value : void
 ****************************************************************************************************/
void CIndex::UpdateArraySubObjects()
{
	INT32 iLoopCount = 0;
	CSubIndex* pobjSIndex = NULL;

	for (iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		pobjSIndex = &collectionObj[iLoopCount];
		//Check for null , alignment changes
		if ((NULL != pobjSIndex->getIndexValue())
				&& 0 == (strcmp(pobjSIndex->getIndexValue(), "00")))
			continue;
		pobjSIndex->setDataTypeST(this->getDataType());
	}
	pobjSIndex = NULL;
}
#ifndef __GNUC__
#pragma endregion MemberFunctions
#endif
