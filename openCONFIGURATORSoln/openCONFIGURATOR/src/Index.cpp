/**
 ************************************************************************************************
 \file			Index.cpp

 \brief			Handles adding index to all PDO types
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

#include <stdio.h>
#include "../Include/Index.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E C L A R A T I O N S 					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 
 @param void
 */

CIndex::CIndex(void)
{
	m_SubIndexCount = collectionObj.Count();
	//m_PDOType = 0; //TODO: Review Initialisation
}

/*************************************************************************/
/* Destructor */

/**
 
 @param void
 */

CIndex::~CIndex(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief			getPDOType
 
 This function returns PDO type
 
 \return		EPDOType
 */
/*****************************************************************************/

EPDOType CIndex::getPDOType()
{
	return m_PDOType;
}

/*****************************************************************************/
/**
 \brief			setPDOType
 
 This is a member function of setPDOType to assign PDO type
 
 \param			enumPDOType	Enum Variable of EPDOType to hold the value of PDO type			
 \return		void
 */
/*****************************************************************************/

void CIndex::setPDOType(EPDOType enumPDOType)
{
	m_PDOType = enumPDOType;
}

#ifndef __GNUC__
#pragma region MemberFunctions
#endif

/*****************************************************************************/
/**
 \brief			addSubIndex
 
 This is a member function of CIndex to add SubIndex in Index Object
 
 \param			objSubIndex		Class variable of CSubIndex for object subindex		
 \return		void
 */
/*****************************************************************************/

void CIndex::addSubIndex(CSubIndex objSubIndex)
{
	INT32 iItemPosition = collectionObj.Add();
	collectionObj[iItemPosition] = objSubIndex;
	m_SubIndexCount = collectionObj.Count();
#if defined DEBUG
	cout << "addSubIndex function:";
	cout << "\t Index" << CIndex::getIndexValue();
	cout << "\tSubindex:" << objSubIndex.getIndexValue() << endl;
#endif
}

/*****************************************************************************/
/**
 \brief			deleteSubIndex
 
 This is a member function of CIndex  to delete the SubIndex in Index Object
 
 \param			iSubIndexID		Integer Variable to hold the value of subindex id		
 \return		void
 */
/*****************************************************************************/

void CIndex::deleteSubIndex(INT32 iSubIndexID)
{
	collectionObj.Remove(iSubIndexID);
	m_SubIndexCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			deleteSubIndexCollection
 
 This is a member function of CIndex to delete the SubIndex from Index Object
 
 \return		void
 */
/*****************************************************************************/

void CIndex::deleteSubIndexCollection()
{
	collectionObj.Clear();
	m_SubIndexCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			getNumberofSubIndexes
 
 This is a member function of CIndex returns number of subindexes
 
 \return		INT32
 */
/*****************************************************************************/

INT32 CIndex::getNumberofSubIndexes()
{
	return collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			getSubIndex
 
 This is a member function of CIndex returns number of subindex ids
 \param         iSubIndexId Integer variable to hold the value of subindex id   
 
 \return		CSubIndex*
 */
/*****************************************************************************/

CSubIndex* CIndex::getSubIndex(INT32 iSubIndexId)
{
	return &collectionObj[iSubIndexId];
}

/*****************************************************************************/
/**
 \brief			getSubIndexbyIndexValue
 
 This is a member function of CIndex returns number of subindex value 
 
 \param			pbIndex		Character pointer to hold index value
 \return		CSubIndex*
 */
/*****************************************************************************/

CSubIndex* CIndex::getSubIndexbyIndexValue(char* pbIndex)
{
	INT32 iLoopCount = 0;
	CSubIndex* pobjSIndex = NULL;

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

/*****************************************************************************/
/**
 \brief			SwapSubObjects
 
 This is a member function of CIndex swaps the subobjects
 
 \param			ipos1    Integer Variable to hold the subobject data
 \param			ipos2	 Integer Variable to hold the subobject data		
 \return		void
 */
/*****************************************************************************/

void CIndex::SwapSubObjects(INT32 ipos1, INT32 ipos2)
{
	swap(collectionObj[ipos1], collectionObj[ipos2]);
}

/*****************************************************************************/
/**
 \brief			UpdateArraySubObjects
 
 This is a member function of CIndex updates subobjects in array  
 
 \return		void
 */
/*****************************************************************************/

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
