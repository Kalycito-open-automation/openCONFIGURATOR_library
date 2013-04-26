/**
 ******************************************************************************
 \file		ApplicationProcess.cpp

 \brief		This file contains the class member definitions that are used for handling the objects within the 'Application Process' tag in the XML configuration file(XDD/XDC) 
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

/****************************************************************************/
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
 

 */

ApplicationProcess::ApplicationProcess(void)
{
	xddFileName = NULL;
	projectPath = NULL;
}

/*************************************************************************/
/* Destructor */

/**
 

 */

ApplicationProcess::~ApplicationProcess(void)
{
	//Add destructor code here
}

INT32 ApplicationProcess::CheckFileStatus(char* fileName)
{
	ifstream file(fileName, ios::in | ios::binary | ios::ate);

	if (TRUE == (file.is_open()))
	{
		return TRUE;
	}
	else
	{
		return FALSE;
	}
}

void ApplicationProcess::AddComplexDataType(ComplexDataType objectCdT)
{
	INT32 itemPosition = CDTCollection.Add();

	objectCdT.cDtObjPosition = itemPosition;
	CDTCollection[itemPosition] = objectCdT;
}

void ApplicationProcess::DeleteComplexDataTypeCollection()
{
	CDTCollection.Clear();
}

void ApplicationProcess::AddParameter(Parameter objectParameter)
{
	INT32 itemPosition = ParameterCollection.Add();

	objectParameter.paraIndex = itemPosition;
	ParameterCollection[itemPosition] = objectParameter;
}

void ApplicationProcess::DeleteParameterCollection()
{
	ParameterCollection.Clear();
}

INT32 ApplicationProcess::GetParameterIndexbyUniqueIDRef(char *uniqueIdRef)
{
	for (INT32 loopCount = 0; loopCount < ParameterCollection.Count();
			loopCount++)
	{
		Parameter parameterObj;
		parameterObj = ParameterCollection[loopCount];
		if (0 == strcmp(uniqueIdRef, parameterObj.nameIdDtAttr.GetUniqueID()))
		{
			return loopCount;
		}
	}
	return 0;
}

Parameter* ApplicationProcess::GetParameterbyUniqueIDRef(char *uniqueIdRef)
{
	for (INT32 loopCount = 0; loopCount < ParameterCollection.Count();
			loopCount++)
	{
		Parameter parameterObj;

		parameterObj = ParameterCollection[loopCount];
		if (0 == strcmp(uniqueIdRef, parameterObj.nameIdDtAttr.GetUniqueID()))
		{
			return &ParameterCollection[loopCount];
		}
	}
	return NULL;
}

INT32 ApplicationProcess::GetCDTUniqueIDRef(char *uniqueIdRef)
{
	for (INT32 loopCount = 0; loopCount < CDTCollection.Count(); loopCount++)
	{
		ComplexDataType complexDtObj;

		complexDtObj = CDTCollection[loopCount];
		if (0 == strcmp(uniqueIdRef, complexDtObj.nameIdAttr->GetUniqueID()))
		{
			return loopCount;
		}
	}
	return 0;
}

ComplexDataType* ApplicationProcess::GetCDTbyUniqueID(char *uniqueId)
{
	for (INT32 loopCount = 0; loopCount < CDTCollection.Count(); loopCount++)
	{
		ComplexDataType complexDtObj;

		complexDtObj = CDTCollection[loopCount];
		if (0 == strcmp(uniqueId, complexDtObj.nameIdAttr->GetUniqueID()))
		{
			return &CDTCollection[loopCount];
		}
	}
	return NULL;
}

ComplexDataType* ApplicationProcess::GetCDTByDtUniqueRefID(char *uniqueRefId)
{
	for (INT32 iLoopCount = 0; iLoopCount < CDTCollection.Count(); iLoopCount++)
	{
		ComplexDataType complexDtObj;

		complexDtObj = CDTCollection[iLoopCount];
		if (NULL != complexDtObj.nameIdAttr->GetDtUniqueRefId())
		{
			if (0
					== strcmp(uniqueRefId,
							complexDtObj.nameIdAttr->GetDtUniqueRefId()))
			{
				return &CDTCollection[iLoopCount];
			}
		}
	}
	return NULL;
}

ComplexDataType* ApplicationProcess::GetCDTByDtIndex(INT32 cDtPosition)
{
	return &CDTCollection[cDtPosition];
}

void ApplicationProcess::UpdatePreviousCDTUId(char* uniqueID, INT32 cDtPosition)
{
	ComplexDataType* complexDtObj;

	complexDtObj = CDTCollection.GetAddress(cDtPosition);
	complexDtObj->prevUniqueId = new char[strlen(uniqueID) + 1];
	strcpy(complexDtObj->prevUniqueId, uniqueID);
}

Parameter ApplicationProcess::GetUniqueIDRefbyParameterIndex(
		INT32 parameterPosition)
{
	Parameter parameterObj;

	parameterObj = ParameterCollection[parameterPosition];

	return parameterObj;
}

INT32 ApplicationProcess::GetCDTCount()
{
	return CDTCollection.Count();
}

//This function has same operation as GetCDTByDtIndex
ComplexDataType* ApplicationProcess::GetCDTbyCount(INT32 cDtPosition)
{
	return &CDTCollection[cDtPosition];
}
