///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  APIOperations.cpp
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
#include <iostream>
#include <fstream>
#include "../Include/ApplicationProcess.h"

/****************************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************************/

/****************************************************************************************************
 * Constructor
 ****************************************************************************************************/
CApplicationProcess::CApplicationProcess(void)
{
	//Add constructor code here
	XDDfilename = NULL;
	ProjectPath = NULL;
}

/****************************************************************************************************
 * Destructor
 ****************************************************************************************************/
CApplicationProcess::~CApplicationProcess(void)
{
	//Add destructor code here
}

/****************************************************************************************************
 * Function Name: CApplicationProcess::checkFileStatus
 * Description  : This Function checks the status of the file.
 * Return value : INT32
 ****************************************************************************************************/
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

/****************************************************************************************************
 * Function Name: CApplicationProcess::addComplexDataType
 * Description  : This Function adds complex data type colletion
 * Return value : void
 ****************************************************************************************************/
void CApplicationProcess::addComplexDataType(CComplexDataType objComplexData)
{
	INT32 iItemPosition = CDTCollection.Add();

	objComplexData.Index = iItemPosition;
	CDTCollection[iItemPosition] = objComplexData;
}

/****************************************************************************************************
 * Function Name: CApplicationProcess::DeleteComplexDataTypeCollection()
 * Description  : This Function deletes complex data type collection 
 * Return value : void
 ****************************************************************************************************/
void CApplicationProcess::DeleteComplexDataTypeCollection()
{
	CDTCollection.Clear();
}
/****************************************************************************************************
 * Function Name: CApplicationProcess::addParameter
 * Description  : This function adds parameter to collection list
 * Return value : void
 ****************************************************************************************************/
void CApplicationProcess::addParameter(Parameter stParam)
{
	INT32 iItemPosition = ParameterCollection.Add();

	stParam.ParaIndex = iItemPosition;
	ParameterCollection[iItemPosition] = stParam;
}
/****************************************************************************************************
 * Function Name: CApplicationProcess::Deleteparametercollection
 * Description  : This function clears the parameter collection list
 * Return value : void
 ****************************************************************************************************/
void CApplicationProcess::DeleteParameterCollection()
{
	ParameterCollection.Clear();
}
/****************************************************************************************************
 * Function Name: CApplicationProcess::get_ParameterIndexby_UniqueIDRef
 * Description  : This function returns parameter index with reference to IDs
 * Return value : INT32
 ****************************************************************************************************/
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

/****************************************************************************************************
 * Function Name: CApplicationProcess::get_Parameterby_UniqueIDRef
 * Description  : This function returns parameter with reference to IDs
 * Return value: Parameter*
 ****************************************************************************************************/
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

/****************************************************************************************************
 * Function Name: CApplicationProcess::get_CDT_UniqueIDRef
 * Description  : This function returns unique reference ids from the collection list
 * Return value : INT32
 ****************************************************************************************************/
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

/****************************************************************************************************
 * Function Name: CApplicationProcess::getCDTbyUniqueID
 * Description  : This function returns CDT values based on unique reference ids from the collection list
 * Return value : CComplexDataType*
 ****************************************************************************************************/
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

/****************************************************************************************************
 * Function Name: CApplicationProcess::getCDTbydt_UniqueRefID
 * Description  : This function returns data type of CDT based on unique reference ids from the collection list
 * Return value : CComplexDataType*
 ****************************************************************************************************/
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

/****************************************************************************************************
 * Function Name: CApplicationProcess::getCDTbydtIndex
 * Description  : This function returns CDT index values
 * Return value : CComplexDataType*
 ****************************************************************************************************/
CComplexDataType* CApplicationProcess::getCDTbydtIndex(INT32 iIndex)
{
	return &CDTCollection[iIndex];
}

/****************************************************************************************************
 * Function Name: CApplicationProcess::updatePreviousCDT_UId
 * Description  : This function updates CDT collection list 
 * Return value : void
 ****************************************************************************************************/
void CApplicationProcess::updatePreviousCDT_UId(char* pbUniqueID, INT32 iIndex)
{
	CComplexDataType* pobjCDT;

	pobjCDT = CDTCollection.GetAddress(iIndex);
	pobjCDT->previousCDT_UId = new char[strlen(pbUniqueID) + 1];
	strcpy(pobjCDT->previousCDT_UId, pbUniqueID);
}

/****************************************************************************************************
 * Function Name: CApplicationProcess::get_UniqueIDRef_by_ParameterIndex
 * Description  : This function returns parameter index
 * Return value : Parameter
 ****************************************************************************************************/
Parameter CApplicationProcess::get_UniqueIDRef_by_ParameterIndex(
		INT32 iParameterIndex)
{
	Parameter stParams;

	stParams = ParameterCollection[iParameterIndex];

	return stParams;
}

/****************************************************************************************************
 * Function Name: CApplicationProcess::getCDTCount
 * Description  : This function returns CDT collection count
 * Return value : INT32
 ****************************************************************************************************/
INT32 CApplicationProcess::getCDTCount()
{
	return CDTCollection.Count();
}

/****************************************************************************************************
 * Function Name: CApplicationProcess::getCDTbyCount
 * Description  : This function returns CDT index collection value 
 * Return value: CComplexDataType*
 ****************************************************************************************************/
CComplexDataType* CApplicationProcess::getCDTbyCount(INT32 iIndex)
{
	return &CDTCollection[iIndex];
}
