///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  BaseIndex.cpp
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
#include "../Include/NodeCollection.h"
#include "../Include/BaseIndex.h"
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
/****************************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************************/

/****************************************************************************************************
 * Constructor
 ****************************************************************************************************/
CBaseIndex::CBaseIndex(void)
{
	m_Index = NULL;
	m_Name = NULL;
	m_HighLimit = NULL;
	m_dataType.Name = NULL;
	m_dataTypeValue = NULL;
	m_DefaultValue = NULL;
	m_ActualValue = NULL;
	m_accessType = NULL;
	m_LowLimit = NULL;
	m_UniqueIDRef = NULL;
	m_dataTypeValue = NULL;
	m_dataType.DataTypeValue = NULL;
	m_NodeID = 0;
	m_dataType.DataSize = 0;
	//m_objectType = 0; //TODO: Review initialisation
	//m_pdoMapping = 0;
	m_ParameterIndex = -1;
	m_IsIncludedInCDC = FALSE;
}

/****************************************************************************************************
 * Destructor
 ****************************************************************************************************/
CBaseIndex::~CBaseIndex(void)
{
	//Add destructor code here
}
#ifndef __GNUC__
#pragma region Properties
#endif
/****************************************************************************************************
 * Function Name: CBaseIndex::getName
 * Description: Returns the Name of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getName()
{
	return m_Name;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setName
 * Description: sets the Name of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setName(char* pbName)
{
	if (NULL != pbName)
	{
		m_Name = new char[strlen(pbName) + STR_ALLOC_BUFFER];
		strcpy((char*) m_Name, pbName);
	}
	else
	{
		m_Name = new char[1 + STR_ALLOC_BUFFER];
		strcpy((char*) m_Name, "");
	}
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getIndexValue
 * Description: Returns the Index of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getIndexValue()
{
	return m_Index;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setIndexValue
 * Description: sets the Index of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setIndexValue(char* pbIndex)
{
	m_Index = new char[strlen(pbIndex) + STR_ALLOC_BUFFER];
	strcpy((char*) m_Index, pbIndex);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getLowLimit
 * Description: Returns the LowLimit of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getLowLimit()
{
	if (NULL != m_LowLimit)
	{
		return m_LowLimit;
	}
	else
	{
		return NULL;
	}
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setLowLimit
 * Description: sets the LowLimit of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setLowLimit(char* pbLowLimit)
{
	m_LowLimit = new char[strlen(pbLowLimit) + STR_ALLOC_BUFFER];
	strcpy((char*) m_LowLimit, pbLowLimit);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getHighLimit
 * Description: Returns the LowLimit of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getHighLimit()
{
	if (NULL != m_HighLimit)
	{
		return m_HighLimit;
	}
	else
	{
		return NULL;
	}
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setHighLimit
 * Description: sets the LowLimit of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setHighLimit(char* pbHighLimit)
{
	m_HighLimit = new char[strlen(pbHighLimit) + STR_ALLOC_BUFFER];
	strcpy((char*) m_HighLimit, pbHighLimit);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getDefaultValue
 * Description: Returns the Default Value of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getDefaultValue()
{
	return m_DefaultValue;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setDefaultValue
 * Description: sets the Default Value of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setDefaultValue(char* pbValue)
{
	m_DefaultValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];
	strcpy((char*) m_DefaultValue, pbValue);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getActualValue
 * Description: Returns the Actual Value of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getActualValue()
{
	if (NULL == m_ActualValue)
	{
		m_ActualValue = NULL;
	}
	return m_ActualValue;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setActualValue
 * Description: sets the Actual Value of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setActualValue(char* pbValue)
{
	m_ActualValue = new char[strlen(pbValue) + ALLOC_BUFFER];
	strcpy((char*) m_ActualValue, pbValue);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getAccessType
 * Description: Returns the Access Type of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getAccessType()
{
	return m_accessType;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setAccessType
 * Description: sets the Access Type of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setAccessType(char* pbAccessType)
{
	m_accessType = new char[strlen(pbAccessType) + STR_ALLOC_BUFFER];
	strcpy((char*) m_accessType, pbAccessType);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getObjectType
 * Description: Returns the Object Type of the Index Object as const char*
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getObjectType()
{
	switch (m_objectType)
	{
	case DEFTYPE:
		return "DEFTYPE";
	case DEFSTRUCT:
		return "DEFSTRUCT";
	case VAR:
		return "VAR";
	case ARRAY:
		return "ARRAY";
	case RECORD:
		return "RECORD";
	default:
		return NULL;
	}
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getEObjectType
 * Description: Returns the Object Type of the Index Object as Enum
 * Return value: EObjectType
 ****************************************************************************************************/
EObjectType CBaseIndex::getEObjectType()
{
	return m_objectType;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setObjectType
 * Description: sets the Object of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setObjectType(char* pbObjectType)
{
	char* pbStrBuff = new char[strlen(pbObjectType) + STR_ALLOC_BUFFER];

	strcpy(pbStrBuff, pbObjectType);
	if ((0 == strcmp(pbStrBuff, "5"))
			|| (0 == strcmp(ConvertToUpper(pbStrBuff), "DEFTYPE")))
	{
		m_objectType = DEFTYPE;
	}
	else if ((0 == strcmp(pbStrBuff, "6"))
			|| (0 == strcmp(pbStrBuff, "DEFSTRUCT")))
	{
		m_objectType = DEFSTRUCT;
	}
	else if ((0 == strcmp(pbStrBuff, "7")) || (0 == strcmp(pbStrBuff, "VAR")))
	{
		m_objectType = VAR;
	}
	else if ((0 == strcmp(pbStrBuff, "8")) || (0 == strcmp(pbStrBuff, "ARRAY")))
	{
		m_objectType = ARRAY;
	}
	else if ((0 == strcmp(pbStrBuff, "9"))
			|| (0 == strcmp(pbStrBuff, "RECORD")))
	{
		m_objectType = RECORD;
	}
	else
	{
#ifdef DEBUG
		cout << "Error! setObjectType failed\n" << endl;
#endif
	}
	delete[] pbStrBuff;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getPDOMapping
 * Description: Returns the PDOMapping of the Index Object
 * Return value: const char*
 ****************************************************************************************************/
const char* CBaseIndex::getPDOMapping()
{
	switch (m_pdoMapping)
	{
	case NO:
		return "NO";
	case DEFAULT:
		return "DEFAULT";
	case OPTIONAL:
		return "OPTIONAL";
	case RPDO:
		return "RPDO";
	case TPDO:
		return "TPDO";
	default:
		return NULL;
	}
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setPDOMapping
 * Description: sets the PDOMapping of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setPDOMapping(char* pbPdoMapping)
{
	char* pbStrBuff = new char[strlen(pbPdoMapping) + STR_ALLOC_BUFFER];

	strcpy(pbStrBuff, ConvertToUpper((char*) pbPdoMapping));
	if (0 == strcmp(ConvertToUpper(pbStrBuff), "DEFAULT"))
	{
		m_pdoMapping = DEFAULT;
	}
	else if (0 == strcmp(ConvertToUpper(pbStrBuff), "NO"))
	{
		m_pdoMapping = NO;
	}
	else if (0 == strcmp(ConvertToUpper(pbStrBuff), "OPTIONAL"))
	{
		m_pdoMapping = OPTIONAL;
	}
	else if (0 == strcmp(ConvertToUpper(pbStrBuff), "RPDO"))
	{
		m_pdoMapping = RPDO;
	}
	else if (0 == strcmp(ConvertToUpper(pbStrBuff), "TPDO"))
	{
		m_pdoMapping = TPDO;
	}
	else
	{
#ifdef DEBUG
		cout << "Error! setPDOMapping failed\n" << endl;
#endif
	}
	delete[] pbStrBuff;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getDataType
 * Description: Returns the DataType of the Index Object
 * Return value: DataType
 ****************************************************************************************************/
DataType CBaseIndex::getDataType()
{
	return m_dataType;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setDataType
 * Description: sets the DataType of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setDataType(char* pbDataTypeName)
{
	DataType* pobjDataType = NULL;
	CNodeCollection* pobjNodeCol = NULL;
	CDataTypeCollection* pobjDataCol = NULL;
	CNode objNode;

	pobjNodeCol = CNodeCollection::getNodeColObjectPointer();
	objNode = pobjNodeCol->getNode(m_NodeID);
	pobjDataCol = objNode.getDataTypeCollection();
	pobjDataType = pobjDataCol->getDataType(pbDataTypeName);

	if (NULL != pobjDataType)
	{
		m_dataType = *pobjDataType;
	}
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setDataType
 * Description: sets the DataType of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setDataType(char* pbDataTypeName, INT32 iNodeID)
{
	DataType* pobjDataType = NULL;
	CNodeCollection* pobjNodeCol = NULL;
	CDataTypeCollection* pobjDataCol = NULL;

	pobjNodeCol = CNodeCollection::getNodeColObjectPointer();
	CNode objNode = pobjNodeCol->getNode(iNodeID);

	pobjDataCol = objNode.getDataTypeCollection();
	pobjDataType = pobjDataCol->getDataTypeByName(pbDataTypeName);
	m_dataType = *pobjDataType;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getNodeID
 * Description: Returns the NodeID of the Node of the Index Object
 * Return value: INT32
 ****************************************************************************************************/
INT32 CBaseIndex::getNodeID()
{
	return m_NodeID;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setNodeID
 * Description: sets the NodeID of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setNodeID(INT32 NodeID)
{
	m_NodeID = NodeID;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getUniqueIDRef
 * Description: Returns the UniqueIDRef of the Node of the Index Object
 * Return value: char*
 ****************************************************************************************************/
char* CBaseIndex::getUniqueIDRef()
{
	return m_UniqueIDRef;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setUniqueIDRef
 * Description: sets the UniqueIDRef of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setUniqueIDRef(char* pbUniqueID)
{
	m_UniqueIDRef = new char[strlen(pbUniqueID) + STR_ALLOC_BUFFER];
	strcpy((char*) m_UniqueIDRef, pbUniqueID);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getDataTypeValue
 * Description: Returns only datatype value of the Node of the Index Object
 * Return value: char*
 ****************************************************************************************************/
char* CBaseIndex::getDataTypeValue()
{
	if (NULL != m_dataTypeValue)
	{
		return m_dataTypeValue;
	}
	else
	{
		return NULL;
	}
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setDataTypeValue
 * Description: sets the datatype value  of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setDataTypeValue(char* pbValue)
{
	m_dataTypeValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];
	strcpy((char*) m_dataTypeValue, pbValue);
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setDataTypeST
 * Description: sets the datatype value  of the Index Object
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setDataTypeST(DataType objDataType)
{
	m_dataType = objDataType;
}
#ifndef __GNUC__
#pragma endregion Properties
#endif

#ifndef __GNUC__
#pragma region MemberFunctions
#endif
/****************************************************************************************************
 * Function Name: CBaseIndex::SaveChanges
 * Description: Saves the changes of the Index properties
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::SaveChanges(char* pbIndex, char* pbName)
{
	m_Index = pbIndex;
	m_Name = pbName;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::IsIndexVaueValid
 * Description: Saves the changes of the Index properties
 * Return value: bool
 ****************************************************************************************************/
bool CBaseIndex::IsIndexVaueValid(char* pbHexValue)
{
	ULONG ulValue;
	bool bFlag = true;

	if (0 == strcmp(pbHexValue, ""))
	{
		return true;
	}

	if (CheckIfHex(pbHexValue))
	{
		ulValue = hex2int(subString(pbHexValue, 2, (strlen(pbHexValue) - 2)));
	}
	else
	{
		return true;
	}

	if (NULL != this->m_LowLimit)
	{
		if (0 != strcmp(this->m_LowLimit, ""))
		{
			ULONG ulLowlimit;
			if (CheckIfHex((char*) this->m_LowLimit))
			{
				ulLowlimit = hex2int(
						subString((char*) m_LowLimit, 2,
								strlen(m_LowLimit) - 2));
			}
			else
			{
				ulLowlimit = atoi(m_LowLimit);
			}
			if (ulValue >= ulLowlimit)
			{
				bFlag = true;
			}
			else
			{
				ocfmException objException;
				objException._ocfmRetCode.code =
						OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
				objException._ocfmRetCode.errorString = new char[150];
				objException._ocfmRetCode.errorString[0] = 0;
				sprintf(objException._ocfmRetCode.errorString,
						"The entered value(%s) is less than the lower limit(%s)",
						pbHexValue, this->m_LowLimit);
				throw objException;
				//bFlag = false;
				//return bFlag;
			}
		}
	}

	if (NULL != this->m_HighLimit)
	{
		if (0 != strcmp(this->m_HighLimit, ""))
		{
			ULONG ulHighLimit;
			if (CheckIfHex((char*) this->m_HighLimit))
			{
				ulHighLimit = hex2int(
						subString((char*) m_HighLimit, 2,
								(strlen(m_HighLimit) - 2)));
			}
			else
			{
				ulHighLimit = atoi(m_HighLimit);
			}
			if (ulValue <= ulHighLimit)
			{
				bFlag = true;
			}
			else
			{
				ocfmException objException;
				objException._ocfmRetCode.code =
						OCFM_ERR_VALUE_NOT_WITHIN_RANGE;
				objException._ocfmRetCode.errorString = new char[150];
				objException._ocfmRetCode.errorString[0] = 0;
				sprintf(objException._ocfmRetCode.errorString,
						"The entered value(%s) is greater than the upper limit(%s)",
						pbHexValue, this->m_HighLimit);
				throw objException;
				//bFlag = false;
			}
		}
	}
	return bFlag;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::setFlagIfIncludedCdc
 * Description: sets the flag whether the Index should be included in cdc
 * Return value: void
 ****************************************************************************************************/
void CBaseIndex::setFlagIfIncludedCdc(EFlag enumFlag)
{
	m_IsIncludedInCDC = enumFlag;
}

/****************************************************************************************************
 * Function Name: CBaseIndex::getFlagIfIncludedCdc
 * Description: Returns whether the Index should be included in cdc
 * Return value: EFlag
 ****************************************************************************************************/
EFlag CBaseIndex::getFlagIfIncludedCdc()
{
	return m_IsIncludedInCDC;
}
#ifndef __GNUC__
#pragma endregion MemberFunctions
#endif
