/**
 ************************************************************************************************
 \file			BaseIndex.cpp

 \brief			This Adds the feature details reference to the index for each datatype 
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

#include "../Include/NodeCollection.h"
#include "../Include/BaseIndex.h"
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/****************************************************************************************************/
/* Constructor */

/**
 
 @param void
 */

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
	m_dataType.DataTypeValue = NULL;
	m_NodeID = 0;
	m_dataType.DataSize = 0;
	//m_objectType = 0; //TODO: Review initialisation
	//m_pdoMapping = 0;
	m_ParameterIndex = -1;
	m_IsIncludedInCDC = FALSE;
}

/****************************************************************************************************/

/* Destructor */

/**
 @param void
 */

CBaseIndex::~CBaseIndex(void)
{
	//Add destructor code here
}
#ifndef __GNUC__
#pragma region Properties
#endif

/*****************************************************************************/
/**
 \brief			getName
 
 This is a member function of CBaseIndex points the Name of the Index Object
 
 \return	const char*
 */
/*****************************************************************************/

const char* CBaseIndex::getName()
{
	return m_Name;
}

/*****************************************************************************/
/**
 \brief			setName
 
 This is a member function of CBaseIndex to set the Name of the Index Object

 \param			pbName	Pointer variable to hold the name of the index object
 \return	void
 */
/*****************************************************************************/

void CBaseIndex::setName(char* pbName)
{
	if(NULL != m_Name)
	{
		delete[] m_Name;
	}

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

/*****************************************************************************/
/**
 \brief			getIndexValue
 
 This is a member function of CBaseIndex returns the Index value of the Object
 
 \return	const char*
 */
/*****************************************************************************/

const char* CBaseIndex::getIndexValue()
{
	return m_Index;
}

/*****************************************************************************/
/**
 \brief			setIndexValue
 
 This is a member function of CBaseIndex to set the Index of the Object
 
 \param			pbIndex Pointer variable to hold the Index value
 \return	void
 */
/*****************************************************************************/

void CBaseIndex::setIndexValue(char* pbIndex)
{
	if(NULL != m_Index)
	{
		delete[] m_Index;
	}
	m_Index = new char[strlen(pbIndex) + STR_ALLOC_BUFFER];
	strcpy((char*) m_Index, pbIndex);
}

/*****************************************************************************/
/**
 \brief			getLowLimit
 
 This is a member function of CBaseIndex to return the LowLimit of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

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

/*****************************************************************************/
/**
 \brief			setLowLimit
 
 This is a member function of CBaseIndex to set the LowLimit of the Index Object
 
 \param	        pbLowLimit  Pointer variable to hold lowlimit value
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setLowLimit(char* pbLowLimit)
{
	if(NULL != m_LowLimit)
	{
		delete[] m_LowLimit;
	}
	m_LowLimit = new char[strlen(pbLowLimit) + STR_ALLOC_BUFFER];
	strcpy((char*) m_LowLimit, pbLowLimit);
}

/*****************************************************************************/
/**
 \brief			getHighLimit
 
 This is a member function of CBaseIndex to return the HighLimit of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

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

/*****************************************************************************/
/**
 \brief			setHighLimit
 
 This is a member function of CBaseIndex to set the LowLimit of the Index Object
 
 \param			pbHighLimit		Pointer variable to hold highlimit value	
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setHighLimit(char* pbHighLimit)
{
	if(NULL != m_HighLimit)
	{
		delete[] m_HighLimit;
	}
	m_HighLimit = new char[strlen(pbHighLimit) + STR_ALLOC_BUFFER];
	strcpy((char*) m_HighLimit, pbHighLimit);
}

/*****************************************************************************/
/**
 \brief			getDefaultValue
 
 This is a member function of CBaseIndex to return the Default Value of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* CBaseIndex::getDefaultValue()
{
	return m_DefaultValue;
}

/*****************************************************************************/
/**
 \brief			setDefaultValue
 
 This is a member function of CBaseIndex to set the Default Value of the Index Object
 
 \param			pbValue		Pointer variable to hold default value	
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setDefaultValue(char* pbValue)
{
	if(NULL != m_DefaultValue)
	{
		delete[] m_DefaultValue;
	}
	m_DefaultValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];
	strcpy((char*) m_DefaultValue, pbValue);
}

/*****************************************************************************/
/**
 \brief			getActualValue
 
 This is a member function of CBaseIndex to set the Default Value of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* CBaseIndex::getActualValue()
{
	if (NULL == m_ActualValue)
	{
		m_ActualValue = NULL;
	}
	return m_ActualValue;
}

/*****************************************************************************/
/**
 \brief			setActualValue
 
 This is a member function of CBaseIndex to set the Actual Value of the Index Object
 
 \param			pbValue		Pointer variable to hold Actual value			
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setActualValue(char* pbValue)
{
	if(NULL != m_ActualValue)
	{
		delete[] m_ActualValue;
	}
	m_ActualValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];
	strcpy((char*) m_ActualValue, pbValue);
}

/*****************************************************************************/
/**
 \brief			getAccessType
 
 This is a member function of CBaseIndex returns the Access Type of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

const char* CBaseIndex::getAccessType()
{
	return m_accessType;
}

/*****************************************************************************/
/**
 \brief			setAccessType
 
 This is a member function of CBaseIndex sets the Access Type of the Index Object
 
 \param			pbAccessType Pointer variable to hold Access type of the object
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setAccessType(char* pbAccessType)
{
	if(NULL != m_accessType)
	{
		delete[] m_accessType;
	}
	m_accessType = new char[strlen(pbAccessType) + STR_ALLOC_BUFFER];
	strcpy((char*) m_accessType, pbAccessType);
}

/*****************************************************************************/
/**
 \brief			getObjectType
 
 This is a member function of CBaseIndex returns the Object Type of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

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

/*****************************************************************************/
/**
 \brief			getObjectType
 
 This is a member function of CBaseIndex returns the Object Type of the Index Object as Enum
 
 \return		EObjectType
 */
/*****************************************************************************/

EObjectType CBaseIndex::getEObjectType()
{
	return m_objectType;
}

/******************************************************************************/
/**
 \brief			setObjectType
 
 This is a member function of CBaseIndex sets the Object of the Index Object
 
 \param			pbObjectType	Pointer variable to hold object type 		
 \return		void
 */
/*****************************************************************************/

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

/******************************************************************************/
/**
 \brief			getPDOMapping
 
 This is a member function of CBaseIndex returns the PDOMapping of the Index Object
 
 \return		const char*
 */
/*****************************************************************************/

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

/******************************************************************************/
/**
 \brief			setPDOMapping
 
 This is a member function of CBaseIndex sets the PDOMapping of the Index Object
 
 \param			pbPdoMapping  Character Pointer Variable to hold PDOMapping of the Index Object			
 \return		void
 */
/*****************************************************************************/

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

/******************************************************************************/
/**
 \brief			getDataType
 
 This is member function of CBaseIndex returns the DataType of the Index Object
 
 \return		DataType
 */
/*****************************************************************************/

DataType CBaseIndex::getDataType()
{
	return m_dataType;
}

/******************************************************************************/
/**
 \brief			setDataType
 
 This is member function of CBaseIndex sets the DataType of the Index Object
 
 \param			pbDataTypeName	Character pointer variable to hold datatype name
 \return		void
 */
/*****************************************************************************/

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

/******************************************************************************/
/**
 \brief			setDataType
 
 This is member function of CBaseIndex sets the DataType of the Index Object
 
 \param			pbDataTypeName Character pointer variable to hold datatype name
 \param			iNodeID		   Integer variable to hold node id of each node 		
 \return		void
 */
/*****************************************************************************/
//TODO: unused function
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

/******************************************************************************/
/**
 \brief			getNodeID
 
 This is member function of CBaseIndex returns the NodeID of Index Object
 
 \return		INT32
 */
/*****************************************************************************/

INT32 CBaseIndex::getNodeID()
{
	return m_NodeID;
}

/******************************************************************************/
/**
 \brief			setNodeID
 
 This is member function of CBaseIndex sets the NodeID of the Index Object
 
 \param			NodeID	Integer Variable to hold the node id of the objects			
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setNodeID(INT32 NodeID)
{
	m_NodeID = NodeID;
}

/******************************************************************************/
/**
 \brief			getUniqueIDRef
 
 This is member function of CBaseIndex returns the UniqueIDRef of the Node 
 
 \return		char*
 */
/*****************************************************************************/

char* CBaseIndex::getUniqueIDRef()
{
	return m_UniqueIDRef;
}

/******************************************************************************/
/**
 \brief			setUniqueIDRef
 
 This is member function of CBaseIndex sets the UniqueIDRef of the Index Object
 
 \param        pbUniqueID  Character pointer variable to hold unique id of the objects
 
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setUniqueIDRef(char* pbUniqueID)
{
	m_UniqueIDRef = new char[strlen(pbUniqueID) + STR_ALLOC_BUFFER];
	strcpy((char*) m_UniqueIDRef, pbUniqueID);
}

/******************************************************************************/
/**
 \brief			getDataTypeValue
 
 This is member function of CBaseIndex returns datatype of Node
 
 \return		char*
 */
/*****************************************************************************/

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

/******************************************************************************/
/**
 \brief			setDataTypeValue
 
 This is member function of CBaseIndex sets the datatype value of the Index Object
 
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::setDataTypeValue(char* pbValue)
{
	m_dataTypeValue = new char[strlen(pbValue) + STR_ALLOC_BUFFER];
	strcpy((char*) m_dataTypeValue, pbValue);
}

/******************************************************************************/
/**
 \brief			setDataTypeST
 
 This is member function of CBaseIndex sets the datatype value of the Index Object
 
 \param			objDataType		Class Variable of DataType to hold value of object datatype		
 \return		void
 */
/*****************************************************************************/

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

/******************************************************************************/
/**
 \brief			SaveChanges
 
 This is member function of CBaseIndex to Save the changes of the Index properties
 
 \param			pbIndex  Character pointer varibale to hold the value of index
 \param			pbName	 Character pointer varibale to hold the value of name			
 \return		void
 */
/*****************************************************************************/

void CBaseIndex::SaveChanges(char* pbIndex, char* pbName)
{
	m_Index = pbIndex;
	m_Name = pbName;
}

/******************************************************************************/
/**
 \brief			IsIndexVaueValid
 
 This is member function of CBaseIndex to check the valid index value
 
 \param			pbHexValue  Character pointer variable to hold the hex value
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending
 */
/*****************************************************************************/

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

/******************************************************************************/
/**
 \brief			setFlagIfIncludedCdc
 
 This is a member function of CBaseIndex sets the Enumflag 
 
 \param			enumFlag  Enumeration Flag to flagIfInCdc  
 \return		void			
 */
/*****************************************************************************/

void CBaseIndex::setFlagIfIncludedCdc(EFlag enumFlag)
{
	m_IsIncludedInCDC = enumFlag;
}

/******************************************************************************/
/**
 \brief			getFlagIfIncludedCdc
 
 This is a member function of CBaseIndex checks whether the Index shall be included in cdc
 
 \return		EFlag			
 */
/*****************************************************************************/

EFlag CBaseIndex::getFlagIfIncludedCdc()
{
	return m_IsIncludedInCDC;
}
#ifndef __GNUC__
#pragma endregion MemberFunctions
#endif
