/**
 ******************************************************************************
 \file		BaseIndex.cpp

 \brief		This file contains the class member definitions that are used for handling the attributes of an Index/SubIndex object
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

#include "../Include/NodeCollection.h"
#include "../Include/BaseIndex.h"
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include "../Include/Logging.h"
#include "../Include/BoostShared.h"
#include "../Include/Result.h"
#include "../Include/Enums.h"

#include <sstream>


using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::Library::Utilities;
//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/****************************************************************************************************/
/* Constructor */

/**


 */

BaseIndex::BaseIndex(void) :
	name(NULL),
	indexId(NULL),
	lowLimit(NULL),
	highLimit(NULL),
	accessType(NULL),
	defaultValue(NULL),
	uniqueIdRef(NULL),
	actualValue(NULL),
	dataTypeValue(NULL),
	objectType(),
	dataType(),
	pdoMapping(NOT_DEFINED),
	nodeId(0),
	parameterIndex(-1),
	forceToCDC(false)
{}

/****************************************************************************************************/

/* Destructor */

/**

 */

BaseIndex::~BaseIndex(void)
{
	//Add destructor code here
}

#ifndef __GNUC__
#pragma region Properties
#endif

const char* BaseIndex::GetName()
{
	return name;
}

void BaseIndex::SetName(const char* nameStr)
{
	if (NULL != name)
	{
		delete[] name;
	}
	if (NULL != nameStr)
	{
		name = new char[strlen(nameStr) + STR_ALLOC_BUFFER];
		strcpy((char*) name, nameStr);
	}
	else
	{
		name = new char[1 + STR_ALLOC_BUFFER];
		strcpy((char*) name, "");
	}
}

const char* BaseIndex::GetIndexValue()
{
	return indexId;
}

void BaseIndex::SetIndexValue(const char* idxId)
{
	if (NULL != indexId)
	{
		delete[] indexId;
	}
	indexId = new char[strlen(idxId) + STR_ALLOC_BUFFER];
	strcpy((char*) indexId, idxId);
}

const char* BaseIndex::GetLowLimit()
{
	if (NULL != lowLimit)
	{
		return lowLimit;
	}
	else
	{
		return NULL;
	}
}

void BaseIndex::SetLowLimit(const char* lowLimitStr)
{
	if (NULL != lowLimit)
	{
		delete[] lowLimit;
	}
	lowLimit = new char[strlen(lowLimitStr) + STR_ALLOC_BUFFER];
	strcpy((char*) lowLimit, lowLimitStr);
}

const char* BaseIndex::GetHighLimit()
{
	if (NULL != highLimit)
	{
		return highLimit;
	}
	else
	{
		return NULL;
	}
}

void BaseIndex::SetHighLimit(const char* highLimitStr)
{
	if (NULL != highLimit)
	{
		delete[] highLimit;
	}
	highLimit = new char[strlen(highLimitStr) + STR_ALLOC_BUFFER];
	strcpy((char*) highLimit, highLimitStr);
}

const char* BaseIndex::GetDefaultValue()
{
	return defaultValue;
}

void BaseIndex::SetDefaultValue(const char* value)
{
	if (NULL != defaultValue)
	{
		delete[] defaultValue;
	}
	defaultValue = new char[strlen(value) + STR_ALLOC_BUFFER];
	strcpy((char*) defaultValue, value);
}

const char* BaseIndex::GetActualValue()
{
	if (NULL == actualValue)
	{
		actualValue = NULL;
	}
	return actualValue;
}

void BaseIndex::SetActualValue(const char* value)
{
	if (NULL != actualValue)
	{
		delete[] actualValue;
	}
	actualValue = new char[strlen(value) + STR_ALLOC_BUFFER];
	strcpy((char*) actualValue, value);
}

const char* BaseIndex::GetAccessType()
{
	return accessType;
}

void BaseIndex::SetAccessType(const char* accessStr)
{
	if (NULL != accessType)
	{
		delete[] accessType;
	}
	accessType = new char[strlen(accessStr) + STR_ALLOC_BUFFER];
	strcpy((char*) accessType, accessStr);
}

const char* BaseIndex::GetObjectType()
{
	switch (objectType)
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

ObjectType BaseIndex::GetEObjectType()
{
	return objectType;
}

void BaseIndex::SetObjectType(const ObjectType objectType)
{
	this->objectType = objectType;
}

void BaseIndex::SetObjectType(const char* objTypeStr)
{
	char* tempObjtype = new char[strlen(objTypeStr) + STR_ALLOC_BUFFER];
	strcpy(tempObjtype, objTypeStr);

	tempObjtype = ConvertToUpper(tempObjtype);
	if ((0 == strcmp(tempObjtype, "5"))
	        || (0 == strcmp(tempObjtype, "DEFTYPE")))
	{
		objectType = DEFTYPE;
	}
	else if ((0 == strcmp(tempObjtype, "6"))
	         || (0 == strcmp(tempObjtype, "DEFSTRUCT")))
	{
		objectType = DEFSTRUCT;
	}
	else if ((0 == strcmp(tempObjtype, "7"))
	         || (0 == strcmp(tempObjtype, "VAR")))
	{
		objectType = VAR;
	}
	else if ((0 == strcmp(tempObjtype, "8"))
	         || (0 == strcmp(tempObjtype, "ARRAY")))
	{
		objectType = ARRAY;
	}
	else if ((0 == strcmp(tempObjtype, "9"))
	         || (0 == strcmp(tempObjtype, "RECORD")))
	{
		objectType = RECORD;
	}
	else
	{
		LOG_FATAL() << "Unknown object type '" << tempObjtype << "'.";
	}
	delete[] tempObjtype;
}

const char* BaseIndex::GetPDOMapping()
{
	switch (pdoMapping)
	{
		case NOT_DEFINED:
			return "NOT_DEFINED";
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

void BaseIndex::SetPDOMapping(const char* pdoMappingStr)
{
	char* varStrBuff = new char[strlen(pdoMappingStr) + STR_ALLOC_BUFFER];

	strcpy(varStrBuff, pdoMappingStr);
	varStrBuff = ConvertToUpper((char*) varStrBuff);
	if ((0 == strcmp(varStrBuff, "DEFAULT")) || 0 == strcmp(varStrBuff, "DEF"))
	{
		pdoMapping = DEFAULT;
	}
	else if (0 == strcmp(varStrBuff, "NO"))
	{
		pdoMapping = NO;
	}
	else if ((0 == strcmp(varStrBuff, "OPTIONAL")) || 0 == strcmp(varStrBuff, "OPT"))
	{
		pdoMapping = OPTIONAL;
	}
	else if (0 == strcmp(varStrBuff, "RPDO"))
	{
		pdoMapping = RPDO;
	}
	else if (0 == strcmp(varStrBuff, "TPDO"))
	{
		pdoMapping = TPDO;
	}
	else
	{
		pdoMapping = NOT_DEFINED;
		LOG_FATAL() << "Unknown PDOMapping type '" << varStrBuff << "'.";
	}
	delete[] varStrBuff;
}

DataType BaseIndex::GetDataType()
{
	return dataType;
}

void BaseIndex::SetDataType(char* dataTypeName)
{
	DataType* objDataType = NULL;
	NodeCollection* objNodeCol = NULL;
	DataTypeCollection* objDataCol = NULL;
	Node objNode;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNode(nodeId);
	objDataCol = objNode.GetDataTypeCollection();
	objDataType = objDataCol->GetDataType(dataTypeName);

	if (NULL != objDataType)
	{
		dataType = *objDataType;
	}
}

void BaseIndex::SetDataTypeName(const char* dataTypeName, INT32 nodeIdVal)
{
	DataType* objDataType = NULL;
	NodeCollection* objNodeCol = NULL;
	DataTypeCollection* objDataCol = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	Node objNode = objNodeCol->GetNode(nodeIdVal);

	objDataCol = objNode.GetDataTypeCollection();
	objDataType = objDataCol->GetDataTypeByName(dataTypeName);

	dataType = *objDataType;
}

INT32 BaseIndex::GetNodeID()
{
	return nodeId;
}

void BaseIndex::SetNodeID(INT32 nodeIdVal)
{
	nodeId = nodeIdVal;
}

char* BaseIndex::GetUniqueIDRef()
{
	return uniqueIdRef;
}

void BaseIndex::SetUniqueIDRef(const char* uniqueIdStr)
{
	uniqueIdRef = new char[strlen(uniqueIdStr) + STR_ALLOC_BUFFER];
	strcpy((char*) uniqueIdRef, uniqueIdStr);
}

char* BaseIndex::GetDataTypeValue()
{
	if (NULL != dataTypeValue)
	{
		return dataTypeValue;
	}
	else
	{
		return NULL;
	}
}

void BaseIndex::SetDataTypeValue(const char* dataTypeStr)
{
	dataTypeValue = new char[strlen(dataTypeStr) + STR_ALLOC_BUFFER];
	strcpy((char*) dataTypeValue, dataTypeStr);
}

void BaseIndex::SetDataTypeST(DataType dtObj)
{
	dataType = dtObj;
}
#ifndef __GNUC__
#pragma endregion Properties
#endif

#ifndef __GNUC__
#pragma region MemberFunctions
#endif

void BaseIndex::SaveChanges(char* idxIdStr, char* nameStr)
{
	indexId = idxIdStr;
	name = nameStr;
}

bool BaseIndex::IsIndexValueValid(const char* hexValue)
{
	ULONG value;
	//Determine limits for error message
	ULONG uhighLimit = 0;
	ULONG ulowLimit = 0;
	ocfmRetCode objException;

	if (highLimit != NULL)
	{
		if (CheckIfHex(this->highLimit))
			uhighLimit = HexToInt<ULONG>(string(this->highLimit));
		else
			uhighLimit = boost::lexical_cast<ULONG>(string(this->highLimit));
	}
	if (lowLimit != NULL)
	{
		if (CheckIfHex(this->lowLimit))
			ulowLimit = HexToInt<ULONG>(string(this->lowLimit));
		else
			ulowLimit = boost::lexical_cast<ULONG>(string(this->lowLimit));
	}

	if(uhighLimit == 0 && ulowLimit == 0)
		return true;

	if (0 == strcmp(hexValue, ""))
		return true;


	if (CheckIfHex(hexValue))
		value = HexToInt<ULONG>(string(hexValue));
	else
		value = boost::lexical_cast<ULONG>(string(hexValue));


	if (value < ulowLimit)
	{
		boost::format formatter(kMsgValueNotWithinRange);
		formatter
		% value
		% this->GetName()
		% nodeId
		% ulowLimit
		% uhighLimit;
		objException.setErrorCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}

	if (value > uhighLimit)
	{
		boost::format formatter(kMsgValueNotWithinRange);
		formatter
		% value
		% this->GetName()
		% nodeId
		% ulowLimit
		% uhighLimit;
		objException.setErrorCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}

	return true;
}

void BaseIndex::SetForceToCDC(bool force)
{
	forceToCDC = force;
}

bool BaseIndex::GetForceToCDC()
{
	return forceToCDC;
}

void BaseIndex::DeleteAllMemberMemory()
{
	if (indexId != NULL)
		delete[] indexId;
	if (name != NULL)
		delete[] name;
	if (highLimit != NULL)
		delete[] highLimit;
	//dataType.Initialize();
	if (dataTypeValue != NULL)
		delete[] dataTypeValue;
	if (defaultValue != NULL)
		delete[] defaultValue;
	if (actualValue != NULL)
		delete[] actualValue;
	if (accessType != NULL)
		delete[] accessType;
	if (lowLimit != NULL)
		delete[] lowLimit;
	if (uniqueIdRef != NULL)
		delete[] uniqueIdRef;
}

UINT32 BaseIndex::GetIndex() const
{
	UINT32 index = 0;
	std::stringstream convert;

	convert << std::hex << this->indexId;
	convert >> index;
	return index;
}

#ifndef __GNUC__
#pragma endregion MemberFunctions
#endif
