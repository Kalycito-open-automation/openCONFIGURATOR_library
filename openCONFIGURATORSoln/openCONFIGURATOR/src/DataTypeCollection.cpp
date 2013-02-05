///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  DataTypeCollection.cpp
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
#include "../Include/DataTypeCollection.h"
#include "../Include/Internal.h"

/****************************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************************/

/****************************************************************************************************
 * Constructor
 ****************************************************************************************************/
CDataTypeCollection::CDataTypeCollection(void)
{
	m_DataTypeCount = collectionObj.Count();
}

/****************************************************************************************************
 * Destructor
 ****************************************************************************************************/
CDataTypeCollection::~CDataTypeCollection(void)
{
	//Add destructor code here
}

/****************************************************************************************************
 * Function Name: CDataTypeCollection::addDataType
 * Description  : This Function counts the data type
 * Return value : void
 ****************************************************************************************************/
void CDataTypeCollection::addDataType(DataType objDataType)
{
	INT32 iItemPosition = collectionObj.Add();
	collectionObj[iItemPosition] = objDataType;
	m_DataTypeCount = collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CDataTypeCollection::DeleteDataTypeCollection
 * Description: This Function clears the datatype collection count
 * Return value: void
 ****************************************************************************************************/
void CDataTypeCollection::DeleteDataTypeCollection()
{
	collectionObj.Clear();
	m_DataTypeCount = collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CDataTypeCollection::getNumberOfDataTypes
 * Description  : This function returns the total number object count
 * Return value : INT32
 ****************************************************************************************************/
INT32 CDataTypeCollection::getNumberOfDataTypes()
{
	return collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CDataTypeCollection::getDataTypeElement
 * Description:   This function return the data type ID
 * Return value: DataType*
 ****************************************************************************************************/
DataType* CDataTypeCollection::getDataTypeElement(INT32 iDataTypeId)
{
	return &collectionObj[iDataTypeId];
}

/****************************************************************************************************
 * Function Name: CDataTypeCollection::getDataType
 * Description:   This function return the data type value
 * Return value: DataType*
 ****************************************************************************************************/
DataType* CDataTypeCollection::getDataType(char* pbDataTypeValue)
{
	DataType* pobjDataType = NULL;

	for (INT32 iLoopCount = 0; iLoopCount < this->getNumberOfDataTypes();
			iLoopCount++)
	{
		pobjDataType = this->getDataTypeElement(iLoopCount);

		if (0 == strcmp(pobjDataType->DataTypeValue, pbDataTypeValue))
		{
			return pobjDataType;
		}
	}
	pobjDataType = NULL;
	return pobjDataType;
}

/****************************************************************************************************
 * Function Name: CDataTypeCollection::getDataTypeByName
 * Description:   This function return the data type name
 * Return value: DataType*
 ****************************************************************************************************/
DataType* CDataTypeCollection::getDataTypeByName(char* pbDataTypeValue)
{
	DataType* pobjDataType = NULL;

	for (INT32 iLoopCount = 0; iLoopCount < this->getNumberOfDataTypes();
			iLoopCount++)
	{
		pobjDataType = this->getDataTypeElement(iLoopCount);

		if (0
				== strcmp(StringToUpper(pobjDataType->getName()),
						StringToUpper(pbDataTypeValue)))
		{
			return pobjDataType;
		}
	}
	pobjDataType = NULL;
	return pobjDataType;
}
