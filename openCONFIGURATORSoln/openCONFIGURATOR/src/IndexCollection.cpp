/**
 ************************************************************************************************
 \file			IndexCollection.cpp

 \brief			Hanldes Index collection for all Indexes
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
#include "../Include/IndexCollection.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 
 @param void
 */

CIndexCollection::CIndexCollection(void)
{
	m_IndexCount = 0;
}

/*************************************************************************/
/* Destructor */

/**
 
 @param void
 */

CIndexCollection::~CIndexCollection(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief			addIndex
 
 This is a member function of CIndexCollection collects the objindex

 \param			objIndex	Class variable of CIndex for object index		
 \return		void
 */
/*****************************************************************************/

void CIndexCollection::addIndex(CIndex objIndex)
{
	INT32 iItemPosition = collectionObj.Add();
	char* substr = NULL;
	substr = subString((char*) objIndex.getIndexValue(), 0, 2);
	if ((0 == strcmp(substr, "14")) || (0 == strcmp(substr, "16")))
	{
		objIndex.setPDOType(PDO_RPDO);
	}
	else if ((0 == strcmp(substr, "18")) || (0 == strcmp(substr, "1A"))
			|| (0 == strcmp(substr, "1a")))
	{
		objIndex.setPDOType(PDO_TPDO);
	}
	else
	{
		//Nothing to be added.
	}
	collectionObj[iItemPosition] = objIndex;
	//cout << "iItemPosition" << iItemPosition << endl;
	m_IndexCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			deleteIndex
 
 This is a member function of CIndexCollection deletes the index value

 \param			iIndexID	Integer Variable to hold index id			
 \return		void
 */
/*****************************************************************************/

void CIndexCollection::deleteIndex(INT32 iIndexID)
{
	collectionObj.Remove(iIndexID);
	m_IndexCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			DeleteIndexCollection
 
 This is a member function of CIndexCollection clears the indexcollection 

 \return		void
 */
/*****************************************************************************/

void CIndexCollection::DeleteIndexCollection()
{
	collectionObj.Clear();
	m_IndexCount = collectionObj.Count();
}

/*****************************************************************************/
/**
 \brief			DeletePDOs
 
 This is a member function of CIndexCollection deletes PDOs(14xx, 16xx, 18xx, 1Axx) indexes

 \return		void
 */
/*****************************************************************************/

void CIndexCollection::DeletePDOs()
{
	INT32 iLoopCount;
	char* substr = new char[SUBINDEX_LEN];
	CIndex objIndex;
	for (iLoopCount = 0; iLoopCount < m_IndexCount; iLoopCount++)
	{
		objIndex = collectionObj[iLoopCount];
		substr = subString((char*) objIndex.getIndexValue(), 0, 2);
		if ((0 == strcmp(substr, "1A")) || (0 == strcmp(substr, "1a"))
				|| (0 == strcmp(substr, "14")) || (0 == strcmp(substr, "16"))
				|| (0 == strcmp(substr, "18")))
		{
			collectionObj.Remove(iLoopCount);
			m_IndexCount = collectionObj.Count();
			iLoopCount = 0;
		}
	}
	delete[] substr;
}

/*****************************************************************************/
/**
 \brief			DeletePIObjects
 
 This is a member function of CIndexCollection deletes PIObjects

 \return		void
 */
/*****************************************************************************/

void CIndexCollection::DeletePIObjects()
{
	INT32 iLoopCount = 0;
	char* substr = new char[2];

	CIndex objIndex;
	for (iLoopCount = 0; iLoopCount < m_IndexCount; iLoopCount++)
	{
		objIndex = collectionObj[iLoopCount];
		substr = subString((char*) objIndex.getIndexValue(), 0, 1);
		if ((0 == strcmp(substr, "A")) || (0 == strcmp(substr, "a")))
		{
			collectionObj.Remove(iLoopCount);
			m_IndexCount = collectionObj.Count();
			iLoopCount = 0;
		}
	}
	delete[] substr;
}

/*****************************************************************************/
/**
 \brief			getIndex
 
 This is a member function of CIndexCollection collects index count 

 \param			iCount    Integer Variable to hold index count
 \return		CIndex*
 */
/*****************************************************************************/

CIndex* CIndexCollection::getIndex(INT32 iCount)
{
	return &collectionObj[iCount];
}

/*****************************************************************************/
/**
 \brief			getIndexbyIndexValue
 
 This is a member function of CIndexCollection collects index values 

 \param			pbIndex    Character pointer to hold index value
 \return		CIndex*
 */
/*****************************************************************************/

CIndex* CIndexCollection::getIndexbyIndexValue(char* pbIndex)
{
	INT32 iLoopCount;
	CIndex objIndex;
	for (iLoopCount = 0; iLoopCount < m_IndexCount; iLoopCount++)
	{
		objIndex = collectionObj[iLoopCount];
		//NULL check values 
		if (0
				== strcmp(StringToUpper((char*) objIndex.getIndexValue()),
						StringToUpper(pbIndex)))
		{
			return &collectionObj[iLoopCount];
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			getNumberofIndexes
 
 This is a member function of CIndexCollection returns total number of index 

 \return		INT32
 */
/*****************************************************************************/

INT32 CIndexCollection::getNumberofIndexes()
{
	return m_IndexCount;
}

