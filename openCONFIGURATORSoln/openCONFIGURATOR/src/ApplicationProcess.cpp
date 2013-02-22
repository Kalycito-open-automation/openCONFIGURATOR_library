/**
 ************************************************************************************************
 \file			ApplicationProcess.cpp

 \brief			Handles the collection of complex data type 
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
#include <fstream>
#include "../Include/ApplicationProcess.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 
 @param void
 */

CApplicationProcess::CApplicationProcess(void)
{
	//Add constructor code here
	XDDfilename = NULL;
	ProjectPath = NULL;
}

/*************************************************************************/
/* Destructor */

/**
 
 @param void
 */

CApplicationProcess::~CApplicationProcess(void)
{
	//Add destructor code here
}

/*****************************************************************************/
/**
 \brief			checkFileStatus
 
 This is a member Function of CApplicationProcess checks the status of the file
 
 \param			pbFilename	Character Pointer variable hold the value of file status		
 \return	INT32
 */
/*****************************************************************************/

INT32 CApplicationProcess::checkFileStatus(char* pbFilename)
{
	ifstream file(pbFilename, ios::in | ios::binary | ios::ate);

	if (TRUE == (file.is_open()))
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
 \brief			addComplexDataType
 
 This ia a member Function of CApplicationProcess which collects complex data type 
 
 \param			objComplexData	 Class variable of CComplexDataType for data type	
 \return	void
 */
/*****************************************************************************/

void CApplicationProcess::addComplexDataType(CComplexDataType objComplexData)
{
	INT32 iItemPosition = CDTCollection.Add();

	objComplexData.Index = iItemPosition;
	CDTCollection[iItemPosition] = objComplexData;
}

/*****************************************************************************/
/**
 \brief			DeleteComplexDataTypeCollection
 
 This is member Function of CApplicationProcess which clears complex data type collection
 
 \return	void
 */
/*****************************************************************************/

void CApplicationProcess::DeleteComplexDataTypeCollection()
{
	CDTCollection.Clear();
}

/*****************************************************************************/
/**
 \brief			addParameter
 
 This is a member function of CApplicationProcess to add parameter in the collection list
 
 \param			stParam		Structure Variable of addParameter to add parameter to the collection list
 \return	void
 */
/*****************************************************************************/

void CApplicationProcess::addParameter(Parameter stParam)
{
	INT32 iItemPosition = ParameterCollection.Add();

	stParam.ParaIndex = iItemPosition;
	ParameterCollection[iItemPosition] = stParam;
}

/*****************************************************************************/
/**
 \brief			Deleteparametercollection
 
 This is a member function of CApplicationProcess which clears parameter collection list
 
 \return	void
 */
/*****************************************************************************/

void CApplicationProcess::DeleteParameterCollection()
{
	ParameterCollection.Clear();
}

/*****************************************************************************/
/**
 \brief			get_ParameterIndexby_UniqueIDRef
 
 This is a member function of CApplicationProcess to point parameter index with reference to IDs

 \param			pbUniqueIdRef	Character Pointer variable to hold the value of unique refernce ids	
 \return	INT32
 */
/*****************************************************************************/

INT32 CApplicationProcess::get_ParameterIndexby_UniqueIDRef(char *pbUniqueIdRef)
{
	for (INT32 iLoopCount = 0; iLoopCount < ParameterCollection.Count();
			iLoopCount++)
	{
		Parameter stParams;
		stParams = ParameterCollection[iLoopCount];
		if (0 == strcmp(pbUniqueIdRef, stParams.name_id_dt_attr.getUniqueID()))
		{
			return iLoopCount;
		}
	}
	return 0;
}

/*****************************************************************************/
/**
 \brief			get_Parameterby_UniqueIDRef
 
 This is a member function of CApplicationProcess to point parameter with reference to IDs

 \param			pbUniqueIdRef	Character Pointer variable to hold 	Unique reference ids
 \return	Parameter*
 */
/*****************************************************************************/

Parameter* CApplicationProcess::get_Parameterby_UniqueIDRef(char *pbUniqueIdRef)
{
	for (INT32 iLoopCount = 0; iLoopCount < ParameterCollection.Count();
			iLoopCount++)
	{
		Parameter stParams;

		stParams = ParameterCollection[iLoopCount];
		if (0 == strcmp(pbUniqueIdRef, stParams.name_id_dt_attr.getUniqueID()))
		{
			return &ParameterCollection[iLoopCount];
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			get_CDT_UniqueIDRef
 
 This is a member function of CApplicationProcess to point unique reference ids from the collection list

 \param			pbUniqueIdRef   Character Pointer variable to hold the value of unique reference ids	
 \return	INT32
 */
/*****************************************************************************/

INT32 CApplicationProcess::get_CDT_UniqueIDRef(char *pbUniqueIdRef)
{
	for (INT32 iLoopCount = 0; iLoopCount < CDTCollection.Count(); iLoopCount++)
	{
		CComplexDataType objCDT;

		objCDT = CDTCollection[iLoopCount];
		if (0 == strcmp(pbUniqueIdRef, objCDT.name_id_attr->getUniqueID()))
		{
			return iLoopCount;
		}
	}
	return 0;
}

/*****************************************************************************/
/**
 \brief			getCDTbyUniqueID
 
 This is a member function of CApplicationProcess to point CDT values based on unique reference ids from the collection list

 \param			pbUniqueId	Character Pointer variable to hold the value of unique ids	
 \return	CComplexDataType*
 */
/*****************************************************************************/

CComplexDataType* CApplicationProcess::getCDTbyUniqueID(char *pbUniqueId)
{
	for (INT32 iLoopCount = 0; iLoopCount < CDTCollection.Count(); iLoopCount++)
	{
		CComplexDataType objCDT;

		objCDT = CDTCollection[iLoopCount];
		if (0 == strcmp(pbUniqueId, objCDT.name_id_attr->getUniqueID()))
		{
			return &CDTCollection[iLoopCount];
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			getCDTbydt_UniqueRefID
 
 This is member function of CApplicationProcess to point complex datatype collection reference to unique ids

 \param			pbUniqueRefId	Character pointer to hold the value of unique reference ids		
 \return	CComplexDataType*
 */
/*****************************************************************************/

CComplexDataType* CApplicationProcess::getCDTbydt_UniqueRefID(
		char *pbUniqueRefId)
{
	for (INT32 iLoopCount = 0; iLoopCount < CDTCollection.Count(); iLoopCount++)
	{
		CComplexDataType objCDT;

		objCDT = CDTCollection[iLoopCount];
		if (NULL != objCDT.name_id_attr->getDtUniqueRefId())
		{
			if (0
					== strcmp(pbUniqueRefId,
							objCDT.name_id_attr->getDtUniqueRefId()))
			{
				return &CDTCollection[iLoopCount];
			}
		}
	}
	return NULL;
}

/*****************************************************************************/
/**
 \brief			getCDTbydtIndex
 
 This is a member function of CApplicationProcess to point CDT with reference to index values

 \param			iIndex		Integer variable to hold the Index of CDT collection	
 \return	CComplexDataType*
 */
/*****************************************************************************/

CComplexDataType* CApplicationProcess::getCDTbydtIndex(INT32 iIndex)
{
	return &CDTCollection[iIndex];
}

/*****************************************************************************/
/**
 \brief			updatePreviousCDT_UId
 
 This is a member function of CApplicationProcess which updates CDT collection list 

 \param			pbUniqueID  Character Pointer variable to hold the unique id of CDT collection
 \param			iIndex		Integer variable to hold the Index of CDT collection	
 \return	void
 */
/*****************************************************************************/

void CApplicationProcess::updatePreviousCDT_UId(char* pbUniqueID, INT32 iIndex)
{
	CComplexDataType* pobjCDT;

	pobjCDT = CDTCollection.GetAddress(iIndex);
	pobjCDT->previousCDT_UId = new char[strlen(pbUniqueID) + 1];
	strcpy(pobjCDT->previousCDT_UId, pbUniqueID);
}

/*****************************************************************************/
/**
 \brief			get_UniqueIDRef_by_ParameterIndex
 
 This is a member function of CApplicationProcess to point the parameterindex reference to unique ids
 
 \param			iParameterIndex  Integer variable to hold the unique ids.	
 \return	Parameter
 */
/*****************************************************************************/

Parameter CApplicationProcess::get_UniqueIDRef_by_ParameterIndex(
		INT32 iParameterIndex)
{
	Parameter stParams;

	stParams = ParameterCollection[iParameterIndex];

	return stParams;
}

/*****************************************************************************/
/**
 \brief			getCDTCount
 
 This is a member function of CApplicationProcess returns CDT collection count

 \return	INT32
 */
/*****************************************************************************/

INT32 CApplicationProcess::getCDTCount()
{
	return CDTCollection.Count();
}

/*****************************************************************************/
/**
 \brief			getCDTbyCount
 
 This is a member function of CApplicationProcess returns CDT index collection value 

 \return	CComplexDataType*
 */
/*****************************************************************************/

CComplexDataType* CApplicationProcess::getCDTbyCount(INT32 iIndex)
{
	return &CDTCollection[iIndex];
}
