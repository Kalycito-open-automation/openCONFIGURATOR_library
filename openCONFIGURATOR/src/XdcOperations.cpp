/**
 ******************************************************************************
 \file		XdcOperations.cpp

 \brief		This file contains the definitions that are used to handle the xml parsing and convert each tag in the XML file to a collection of objects
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

/*****************************************************************************/
/* Includes */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <algorithm>
#include <cassert>
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>
#include <errno.h>
//#include <limits.h>
#include <string.h>
#include "../Include/Declarations.h"
#include "../Include/Exception.h"
#include "../Include/Validation.h"
#include "../Include/NodeCollection.h"
#include "../Include/Logging.h"
#include "../Include/LibraryConfiguration.h"
#include "../Include/Result.h"
#include "../Include/BoostShared.h"

using namespace openCONFIGURATOR::Library::ErrorHandling;

/*****************************************************************************/
/* Defines */

#define MY_ENCODING "UTF-8"

/*****************************************************************************/
/* Global Variables */
string* xddValidationError = NULL;
char xddErrorBuf[1024];

INT32 lastIndexParsed = 0;
static const char* gpa2bSimple[][2] =
{
	{ "BOOL", "1" },
	{ "BITSTRING", "1" },
	{ "BYTE", "8" },
	{ "CHAR", "1" },
	{ "WORD", "16" },
	{ "DWORD", "32" },
	{ "LWORD", "64" },
	{ "SINT", "8" },
	{ "INT", "16" },
	{ "DINT", "32" },
	{ "LINT", "64" },
	{ "USINT", "8" },
	{ "UINT", "16" },
	{ "UDINT", "32" },
	{ "ULINT", "64" },
	{ "REAL", "16" },
	{ "LREAL", "64" },
	{ "STRING", "1" },
	{ "WSTRING", "1" }
}; //array size in g_simple_arr_size

//==========================================================================//
// 					F U N C T I O N  D E C L E R A T I O N S		   		//
//==========================================================================//

/*****************************************************************************/
/**
 \brief		This function shall be used to assign the retrieved attribute value from xml file to the corresponding nameIdDtAttr of the ComplexDataType object in the calling function

 \param		reader    	xml pointer of xmlTextReaderPtr
 \param		cdtObj      Class pointer of ComplexDataType
 \return	void
 */
/*****************************************************************************/
static void SetCDTAttributes(xmlTextReaderPtr reader, ComplexDataType* cdtObj);
/*****************************************************************************/
/**
 \brief		This function shall be used to assign the retrieved attribute value from xml file to the corresponding nameIdDtAttr of the ComplexDataType object in the calling function

 \param		reader		xml pointer of xmlTextReaderPtr
 \param		cdtObj      Class pointer of ComplexDataType
 \return	void
 */
/*****************************************************************************/
static void SetVarDeclaration(xmlTextReaderPtr reader, ComplexDataType* cdtObj);

//==========================================================================//
// 					F U N C T I O N  D E F I N I T I O N S				    //
//==========================================================================//

void SetIndexAttributes(xmlTextReaderPtr reader, Index* indexObj, bool& hasPDO)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;
	ocfmRetCode objException;

	if (!indexObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'indexObj'";
		objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);

	if (!strcmp(ConvertToUpper((char*) name), "INDEX"))
	{

		// Setting the Index Value
		indexObj->SetIndexValue((char*) value);

		/*$S Commented out the TPDO and RPDO grouping $S*/
		//TODO: Check point 1
		char* tempValue = new char[strlen((char*) value) + STR_ALLOC_BUFFER];
		strcpy((char*) tempValue, (char*) value);
		if ((!strncmp(tempValue, "14", 2)) || (!strncmp(tempValue, "16", 2)))
		{
			indexObj->SetPDOType(PDO_RPDO);
			hasPDO = true;
		}
		else if ((!strncmp(tempValue, "18", 2))
		         || (!strncmp(tempValue, "1A", 2)))
		{
			indexObj->SetPDOType(PDO_TPDO);
			hasPDO = true;
		}
		else
		{
			//No operations for Non-PDO's
		}
		delete[] tempValue;

	}
	else if (!(strcmp(ConvertToUpper((char*) name), "NAME")))
	{
		CheckAndCorrectName((char*) value);
		indexObj->SetName((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "OBJECTTYPE"))
	{
		indexObj->SetObjectType((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "LOWLIMIT"))
	{
		indexObj->SetLowLimit((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "HIGHLIMIT"))
	{
		indexObj->SetHighLimit((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "ACCESSTYPE"))
	{
		indexObj->SetAccessType((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "PDOMAPPING"))
	{
		indexObj->SetPDOMapping((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "DEFAULTVALUE"))
	{
		indexObj->SetDefaultValue((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "ACTUALVALUE"))
	{
		indexObj->SetActualValue((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "DATATYPE"))
	{
		if (CheckIfDataTypeExists((char*) value, indexObj->GetNodeID()))
		{
			indexObj->SetDataType((char*) value);
		}
		else
		{
			boost::format formatter(kMsgSimpleDatatypeNotFound);
			formatter
			% (char*) value
			% indexObj->GetIndex()
			% "-"
			% indexObj->GetNodeID();
			objException.setErrorCode(OCFM_ERR_DATATYPE_NOT_FOUND);
			objException.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
		}
	}
	else if (!strcmp(ConvertToUpper((char*) name), "UNIQUEIDREF"))
	{
		indexObj->SetUniqueIDRef((char*) value);
	}
	//else if (!strcmp(ConvertToUpper((char*) name), "CDCFLAG"))
	//{
	//	if (!strcmp(ConvertToUpper((char*) value), "FALSE"))
	//	{
	//		indexObj->SetFlagIfIncludedCdc(false);
	//	}
	//	else if (!strcmp(ConvertToUpper((char*) value), "TRUE"))
	//	{
	//		indexObj->SetFlagIfIncludedCdc(true);
	//	}
	//}
	else
	{
		LOG_ERROR() << "Unknown XML-Attribute '" << name << "'.";
	}
}

void SetSubIndexAttributes(xmlTextReaderPtr reader, SubIndex* sidxObj)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;
	ocfmRetCode objException;

	if (!sidxObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'sidxObj'";
		objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);

	if (!strcmp(ConvertToUpper((char*) name), "SUBINDEX"))
	{
		sidxObj->SetIndexValue((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "NAME"))
	{
		CheckAndCorrectName((char*) value);
		sidxObj->SetName((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "OBJECTTYPE"))
	{
		sidxObj->SetObjectType((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "LOWLIMIT"))
	{
		sidxObj->SetLowLimit((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "HIGHLIMIT"))
	{
		sidxObj->SetHighLimit((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "ACCESSTYPE"))
	{
		sidxObj->SetAccessType((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "PDOMAPPING"))
	{
		sidxObj->SetPDOMapping((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "DEFAULTVALUE"))
	{
		sidxObj->SetDefaultValue((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "ACTUALVALUE"))
	{
		sidxObj->SetActualValue((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "DATATYPE"))
	{
		sidxObj->SetDataType((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "UNIQUEIDREF"))
	{
		sidxObj->SetUniqueIDRef((char*) value);
	}
	//else if (!strcmp(ConvertToUpper((char*) name), "CDCFLAG"))
	//{
	//	if (!strcmp(ConvertToUpper((char*) value), "FALSE"))
	//	{
	//		sidxObj->SetFlagIfIncludedCdc(false);
	//	}
	//	else if (!strcmp(ConvertToUpper((char*) value), "TRUE"))
	//	{
	//		sidxObj->SetFlagIfIncludedCdc(true);
	//	}
	//}
	else
	{
		LOG_ERROR() << "Unknown XML-Attribute '" << name << "'.";
	}
}

void SetDataTypeAttributes(xmlTextReaderPtr reader, DataType* dtObj)
{
	const xmlChar* name = NULL;
	const xmlChar* xcValue = NULL;
	ocfmRetCode objException;

	if (!dtObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'dtObj'";
		objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}
	dtObj->dataTypeName = NULL;
	dtObj->dataTypeValue = NULL;
	dtObj->dataSize = 0;

	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);

	if (!strcmp(ConvertToUpper((char*) name), "DATATYPE"))
	{
		INT32 iRetVal;
		dtObj->dataTypeValue = new char[strlen((char*) xcValue) + 1];
		strcpy(dtObj->dataTypeValue, (char*) xcValue);
		//Read the Equivalent pxcName of a datatype
		iRetVal = xmlTextReaderRead(reader);
		if (1 != iRetVal)
		{
			objException.setErrorCode(OCFM_ERR_XML_FILE_CORRUPTED);
			LOG_FATAL() << objException.getErrorString();
			throw objException;
		}
		while (XML_READER_TYPE_ELEMENT != xmlTextReaderNodeType(reader))
		{
			iRetVal = xmlTextReaderRead(reader);
			if (iRetVal != 1)
			{
				objException.setErrorCode(OCFM_ERR_XML_FILE_CORRUPTED);
				LOG_FATAL() << objException.getErrorString();
				throw objException;
			}
		}
		//pxcValue 	= xmlTextReaderConstValue(pxtrReader);
		name = xmlTextReaderConstName(reader);

		dtObj->dataTypeName = new char[strlen((char*) name) + ALLOC_BUFFER];
		strcpy(dtObj->dataTypeName, (char*) name);

		dtObj->dataSize = GetDataSize(dtObj->dataTypeName);
		if ((0 == strcmp(dtObj->dataTypeName, "Unsigned8"))
		        || (0 == strcmp(dtObj->dataTypeName, "UNSIGNED8")))
		{
			dtObj->iecDataType = USINT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Boolean"))
		         || (0 == strcmp(dtObj->dataTypeName, "BOOLEAN")))
		{
			dtObj->iecDataType = BOOL;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Integer8"))
		         || (0 == strcmp(dtObj->dataTypeName, "INTEGER8")))
		{
			dtObj->iecDataType = SINT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Unsigned16"))
		         || (0 == strcmp(dtObj->dataTypeName, "UNSIGNED16")))
		{
			dtObj->iecDataType = UINT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Integer16"))
		         || (0 == strcmp(dtObj->dataTypeName, "INTEGER16")))
		{
			dtObj->iecDataType = INT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Unsigned32"))
		         || (0 == strcmp(dtObj->dataTypeName, "UNSIGNED32")))
		{
			dtObj->iecDataType = UDINT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Integer32"))
		         || (0 == strcmp(dtObj->dataTypeName, "INTEGER32")))
		{
			dtObj->iecDataType = DINT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Unsigned64"))
		         || (0 == strcmp(dtObj->dataTypeName, "UNSIGNED64")))
		{
			dtObj->iecDataType = ULINT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Integer64"))
		         || (0 == strcmp(dtObj->dataTypeName, "INTEGER64")))
		{
			dtObj->iecDataType = LINT;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Real32"))
		         || (0 == strcmp(dtObj->dataTypeName, "REAL32")))
		{
			dtObj->iecDataType = REAL;
		}
		else if ((0 == strcmp(dtObj->dataTypeName, "Real64"))
		         || (0 == strcmp(dtObj->dataTypeName, "REAL64")))
		{
			dtObj->iecDataType = LREAL;
		}
		else
		{
			LOG_ERROR() << "Unknown data type: " << dtObj->dataTypeName;
		}
	}
}

void SetParameterAttributes(xmlTextReaderPtr reader, Parameter* parameterObj)
{
	const xmlChar* xcName = NULL;
	const xmlChar* xcValue = NULL;
	ocfmRetCode objException;

	if (!parameterObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'parameterObj'";
		objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(reader);
	xcName = xmlTextReaderConstName(reader);

	if (!strcmp(ConvertToUpper((char*) xcName), "UNIQUEID"))
	{
		parameterObj->nameIdDtAttr->SetUniqueID((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "NAME"))
	{
		CheckAndCorrectName((char*) xcValue);
		parameterObj->nameIdDtAttr->SetName((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "ACCESS"))
	{
		parameterObj->accessStr = new char[strlen((const char*) xcValue) + 1];
		strcpy(parameterObj->accessStr, (const char*) xcValue);
	}

	/* TO DO: DATATYPE..There is no tag for it..need to check after how many reads datatype is define </label>
	 <USINT/>*/
}

void SetParaDT(xmlTextReaderPtr reader, Parameter* parameterObj)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;
	INT32 iRetVal;
	ocfmRetCode exceptionObj;

	if (!parameterObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'parameterObj'";
		exceptionObj.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	iRetVal = xmlTextReaderRead(reader);

	if (1 != iRetVal)
	{
		exceptionObj.setErrorCode(OCFM_ERR_XML_FILE_CORRUPTED);
		LOG_FATAL() << exceptionObj.getErrorString();
		throw exceptionObj;
	}

	name = xmlTextReaderConstName(reader);
	value = xmlTextReaderConstValue(reader);

	while (!(CheckEndElement(xmlTextReaderNodeType(reader), (char*) name,
	                         (char*) "parameter")))
	{
		iRetVal = xmlTextReaderRead(reader);

		if (1 != iRetVal)
		{
			exceptionObj.setErrorCode(OCFM_ERR_XML_FILE_CORRUPTED);
			LOG_FATAL() << exceptionObj.getErrorString();
			throw exceptionObj;
		}

		value = xmlTextReaderConstValue(reader);
		name = xmlTextReaderConstName(reader);

		char abSize[3];

		if (CheckifSimpleDT((char*) name, (char*) abSize))
		{
			parameterObj->nameIdDtAttr->SetDataType((char*) name);
			parameterObj->size = atoi((char*) abSize);
			LOG_DEBUG() << "Setting DataType: " << parameterObj->nameIdDtAttr->GetDataType() << ", Size: " << parameterObj->size;
		}
		if (CheckStartElement(xmlTextReaderNodeType(reader), (char*) name,
		                      (char*) "dataTypeIDRef"))
		{
			if (1 == xmlTextReaderHasAttributes(reader))
			{
				xmlTextReaderMoveToNextAttribute(reader);
				value = xmlTextReaderConstValue(reader);
				name = xmlTextReaderConstName(reader);

				if (0 == strcmp(ConvertToUpper((char*) name), "UNIQUEIDREF"))
				{
					parameterObj->nameIdDtAttr->SetDtUniqueRefId((char*) value);
				}
			}
		}
	}
}

static void SetCDTAttributes(xmlTextReaderPtr reader, ComplexDataType* cdtObj)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;
	ocfmRetCode objException;

	if (!cdtObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'cdtObj'";
		objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);

	if (0 == strcmp(ConvertToUpper((char*) name), "UNIQUEID"))
	{
		cdtObj->nameIdAttr->SetUniqueID((char*) value);
	}

	else if (0 == strcmp(ConvertToUpper((char*) name), "NAME"))
	{
		CheckAndCorrectName((char*) value);
		cdtObj->nameIdAttr->SetName((char*) value);
	}
}

bool CheckifSimpleDT(const char* datatypeName, char* dataSize)
{
	INT32 arrSizeLC = 0;
	char* simpleElement = NULL;

	while (arrSizeLC < gSimpleArrSize)
	{
		simpleElement = (char*) gpa2bSimple[arrSizeLC][0];

		if (!strcmp(simpleElement, datatypeName))
		{
			strcpy(dataSize, (char*) gpa2bSimple[arrSizeLC][1]);
			return true;
		}

		arrSizeLC++;
	}
	return false;
}

void SetVarDecAttributes(xmlTextReaderPtr reader, VarDeclaration& vdecl)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;

	//Retrieve the pxcName and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);

	if (!strcmp(ConvertToUpper((char*) name), "UNIQUEID"))
	{
		vdecl.namIdDtAttr->SetUniqueID((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "NAME"))
	{
		CheckAndCorrectName((char*) value);
		vdecl.namIdDtAttr->SetName((char*) value);
	}

	if (!strcmp(ConvertToUpper((char*) name), "SIZE"))
	{
		strcpy((char*) vdecl.size, (const char*) value);
	}
}

bool CheckEndElement(INT32 elementId, const char* srcElement, const char* compareElement)
{
	if ((XML_READER_TYPE_END_ELEMENT == elementId)
	        && (!strcmp(srcElement, compareElement)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CheckStartElement(INT32 elementId, const char* srcElement, const char* compareElement)
{
	if ((XML_READER_TYPE_ELEMENT == elementId)
	        && (!strcmp(srcElement, compareElement)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
/*****************************************************************************/
/**
 \brief		This Function shall parse and Adds the varDeclaration object and its attributes

 \param		reader		xml pointer of xmlTextReaderPtr
 \param		cdtObj		Class pointer of ComplexDataType

 \return	void
 */
/*****************************************************************************/
static void SetVarDeclaration(xmlTextReaderPtr reader, ComplexDataType* cdtObj)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;
	ocfmRetCode objException;

	if (!cdtObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'cdtObj'";
		objException.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		objException.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw objException;
	}

	INT32 retValue;
	VarDeclaration varDeclObj;
	retValue = xmlTextReaderRead(reader);

	if (1 != retValue)
	{
		objException.setErrorCode(OCFM_ERR_XML_FILE_CORRUPTED);
		LOG_FATAL() << objException.getErrorString();
		throw objException;
	}

	name = xmlTextReaderConstName(reader);
	value = xmlTextReaderConstValue(reader);

	while (!(CheckEndElement(xmlTextReaderNodeType(reader), (char*) name,
	                         (char*) "struct")))
	{
		VarDeclaration objTempVarDecl;

		try
		{
			retValue = xmlTextReaderRead(reader);

			if (1 != retValue)
			{
				objException.setErrorCode(OCFM_ERR_XML_FILE_CORRUPTED);
				LOG_FATAL() << objException.getErrorString();
				throw objException;
			}
		}
		catch (const ocfmRetCode& ex)
		{
			LOG_FATAL() << ex.getErrorString();
			throw;
		}
		name = xmlTextReaderConstName(reader);
		value = xmlTextReaderConstValue(reader);

		if (CheckEndElement(xmlTextReaderNodeType(reader), (char*) name,
		                    (char*) "varDeclaration"))
		{
			varDeclObj.structUniqueId = new char[strlen(
			        cdtObj->nameIdAttr->GetUniqueID()) + 1];
			strcpy(varDeclObj.structUniqueId,
			       cdtObj->nameIdAttr->GetUniqueID());
			cdtObj->AddVarDeclaration(varDeclObj);
			varDeclObj = objTempVarDecl;
		}
		if (CheckStartElement(xmlTextReaderNodeType(reader), (char*) name,
		                      (char*) "varDeclaration"))
		{
			if (1 == xmlTextReaderHasAttributes(reader))
			{
				while (xmlTextReaderMoveToNextAttribute(reader))
				{
					SetVarDecAttributes(reader, varDeclObj);
					value = xmlTextReaderConstValue(reader);
					name = xmlTextReaderConstName(reader);
				}
			}
		}
		char sizeStr[3];

		if (CheckifSimpleDT((char*) name, sizeStr))
		{
			varDeclObj.namIdDtAttr->SetDataType((char*) name);

			if (!strcmp(varDeclObj.size, ""))
			{
				strcpy(varDeclObj.size, sizeStr);
			}
		}
		if (CheckStartElement(xmlTextReaderNodeType(reader), (char*) name,
		                      (char*) "dataTypeIDRef"))
		{
			if (1 == xmlTextReaderHasAttributes(reader))
			{

				xmlTextReaderMoveToNextAttribute(reader);
				value = xmlTextReaderConstValue(reader);
				name = xmlTextReaderConstName(reader);

				if (!strcmp(ConvertToUpper((char*) name), "UNIQUEIDREF"))
				{
					varDeclObj.namIdDtAttr->SetDtUniqueRefId((char*) value);
				}
			}
		}
	}
}

ocfmRetCode ValidateXDDFile(const char* fileName)
{
	ocfmRetCode exceptionObject;

	//File name must not be null
	if (fileName != NULL)
	{
		//Parse XML
		xmlDocPtr xdd_file_ptr = xmlParseFile(fileName);

		if (xdd_file_ptr != NULL)
		{
			//Call Validation Function with the parse file
			exceptionObject = ValidateXMLFile(xdd_file_ptr, LibraryConfiguration::GetXddSchemaFilePath().c_str());
		}
		else
		{
			exceptionObject.setErrorCode(OCFM_ERR_XML_PARSING_ERROR);
		}

		//CleanUp XML Parser
		xmlCleanupParser();
		xmlMemoryDump();
	}
	else
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'fileName'";
		exceptionObject.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObject.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
	}
	if (exceptionObject.getErrorCode() == OCFM_ERR_SCHEMA_VALIDATION_FAILED)
	{
		string errorString(exceptionObject.getErrorString());
		errorString.append(" Error occured in file: " + string(fileName));
		exceptionObject.setErrorString(errorString);
	}
	return exceptionObject;
}

Result ValidateProjectFile(const string& fileName)
{
	ocfmRetCode exceptionObject;
	if (!fileName.empty())
	{
		//Parse XML
		xmlDocPtr xdd_file_ptr = xmlParseFile(fileName.c_str());

		if (xdd_file_ptr != NULL)
		{
			//Call Validation Function with the parse file
			exceptionObject = ValidateXMLFile(xdd_file_ptr, LibraryConfiguration::GetProjectFileSchemaFilePath().c_str());
		}
		else
		{
			exceptionObject.setErrorCode(OCFM_ERR_XML_PARSING_ERROR);
			exceptionObject.setErrorString("Project file parsing failed due to wrong XML syntax.");
		}

		//CleanUp XML Parser
		xmlCleanupParser();
		xmlMemoryDump();
	}
	else
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'fileName'";
		exceptionObject.setErrorCode(OCFM_ERR_INVALID_PARAMETER);
		exceptionObject.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
	}
	return Translate(exceptionObject);
}

ocfmRetCode ImportXML(const char* fileName, INT32 nodeId, NodeType nodeType)
{
	ocfmRetCode exceptionObject;

	try
	{

		exceptionObject = ParseFile(fileName, nodeId, nodeType);

		if (OCFM_ERR_SUCCESS != exceptionObject.getErrorCode())
		{
			return exceptionObject;
		}
		xmlCleanupParser();
		/*
		 * this is to debug memory for regression tests
		 */
		xmlMemoryDump();

		///* Add other required index*/
		//if (CN == nodeType)
		//{
		//	//SetFlagForRequiredCNIndexes(nodeId);
		//	/* Not required only in case of auto generation 1020 of CN should be updated else it should be as it is imported*/
		//	//AddOtherRequiredCNIndexes(iNodeID);
		//}
		//if (MN == nodeType)
		//{
		//	//SetFlagForRequiredMNIndexes(nodeId);

		//}

		/* Copy default value of pdos to act value*/
		CopyPDODefToAct(nodeId, nodeType);
		//CopyMNPropDefToAct(nodeId, nodeType);
		CalculateCNPollResponse(nodeId, nodeType);
		SetCNLossObjects(nodeId, nodeType);
		exceptionObject.setErrorCode(OCFM_ERR_SUCCESS);
	}
	catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return exceptionObject;
}

ocfmRetCode ValidateXMLFile(const xmlDocPtr doc, const char* schema_filename)
{

	ocfmRetCode exceptionObject;
	exceptionObject.setErrorCode(OCFM_ERR_SUCCESS);

	xmlDocPtr schema_doc = xmlReadFile(schema_filename, NULL, XML_PARSE_NONET);
	if (schema_doc == NULL)
	{
		// the schema cannot be loaded or is not well-formed
		boost::format formatter(kMsgSchemaNotFoundOrWellFormed);
		formatter
		% schema_filename;
		exceptionObject.setErrorCode(OCFM_ERR_XDD_SCHEMA_NOT_FOUND);
		exceptionObject.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		return exceptionObject;
	}
	xmlSchemaParserCtxtPtr parser_ctxt = xmlSchemaNewDocParserCtxt(schema_doc);
	if (parser_ctxt == NULL)
	{
		// unable to create a parser context for the schema
		xmlFreeDoc(schema_doc);
		boost::format formatter(kMsgSchemaParserContextError);
		formatter
		% schema_filename;
		exceptionObject.setErrorCode(OCFM_ERR_XDD_SCHEMA_PARSER_CONTEXT_ERROR);
		exceptionObject.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		return exceptionObject;
	}

	xmlSchemaPtr schema = xmlSchemaParse(parser_ctxt);
	if (schema == NULL)
	{
		// the schema itself is not valid
		xmlSchemaFreeParserCtxt(parser_ctxt);
		xmlFreeDoc(schema_doc);

		boost::format formatter(kMsgSchemaInvalid);
		formatter
		% schema_filename;
		exceptionObject.setErrorCode(OCFM_ERR_XDD_SCHEMA_NOT_VALID);
		exceptionObject.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		return exceptionObject;
	}

	xmlSchemaValidCtxtPtr valid_ctxt = xmlSchemaNewValidCtxt(schema);
	if (valid_ctxt == NULL)
	{
		// unable to create a validation context for the schema
		xmlSchemaFree(schema);
		xmlSchemaFreeParserCtxt(parser_ctxt);
		xmlFreeDoc(schema_doc);
		boost::format formatter(kMsgSchemaValidationContextError);
		formatter
		% schema_filename;
		exceptionObject.setErrorCode(OCFM_ERR_XDD_SCHEMA_VALIDATION_CONTEXT_ERROR);
		exceptionObject.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		return exceptionObject;
	}

	//Set error handler for validation errors
	xmlSchemaSetValidErrors(valid_ctxt,
	                        (xmlSchemaValidityErrorFunc) HandleSchemaValidationError,
	                        (xmlSchemaValidityWarningFunc) HandleSchemaValidationError,
	                        stderr);


	if (xmlSchemaValidateDoc(valid_ctxt, doc) == 0)
	{
		exceptionObject.setErrorCode(OCFM_ERR_SUCCESS);
	}
	else
	{
		exceptionObject.setErrorCode(OCFM_ERR_SCHEMA_VALIDATION_FAILED);
		exceptionObject.setErrorString(*xddValidationError);
	}

	xmlSchemaFreeValidCtxt(valid_ctxt);
	xmlSchemaFree(schema);
	xmlSchemaFreeParserCtxt(parser_ctxt);
	xmlFreeDoc(schema_doc);

	return exceptionObject;
}

void HandleSchemaValidationError(void*, const char* msg, ...)
{
	va_list args;
	va_start(args, msg);
	int len = vsprintf(xddErrorBuf, msg, args);
	va_end(args);

	if (len == 0)
	{
		xddValidationError = new string("Error could not be retrieved");
	}
	else
	{
		xddValidationError = new string(xddErrorBuf);
	}
}

void ProcessNode(xmlTextReaderPtr reader, NodeType nodeType, INT32 nodePos)
{
	const xmlChar* name = NULL;
	NodeCollection* nodeCollObj = NULL;
	Node* nodeObj = NULL;

	name = xmlTextReaderConstName(reader);

	try
	{
		//If the NodeTYPE is ELEMENT
		if (1 == xmlTextReaderNodeType(reader))
		{
			if (!strcmp(((char*) name), "defType"))
			{
				nodeCollObj = NodeCollection::GetNodeColObjectPointer();
				DataTypeCollection* dtCollObj;
				DataType dtObj;
				dtObj.dataTypeName = NULL;
				dtObj.dataTypeValue = NULL;

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetDataTypeAttributes(reader, &dtObj);
					}
				}
				nodeObj = nodeCollObj->GetNodePtr(nodeType, nodePos);
				dtCollObj = nodeObj->GetDataTypeCollection();
				dtCollObj->AddDataType(dtObj);
			}
			else if (!strcmp(((char*) name), "parameter"))
			{
				nodeCollObj = NodeCollection::GetNodeColObjectPointer();
				ApplicationProcess* appProcessObj;
				Parameter parameterObj;
				nodeObj = nodeCollObj->GetNodePtr(nodeType, nodePos);
				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetParameterAttributes(reader, &parameterObj);
					}
				}
				SetParaDT(reader, &parameterObj);
				// Add parameter to the parameter collection of a node
				appProcessObj = nodeObj->GetApplicationProcess();
				appProcessObj->AddParameter(parameterObj);
			}
			else if (!strcmp(((char*) name), "struct"))
			{
				nodeCollObj = NodeCollection::GetNodeColObjectPointer();
				ApplicationProcess* appProcessObj;
				ComplexDataType cdtObj;

				nodeObj = nodeCollObj->GetNodePtr(nodeType, nodePos);

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetCDTAttributes(reader, &cdtObj);
					}
				}

				SetVarDeclaration(reader, &cdtObj);

				appProcessObj = nodeObj->GetApplicationProcess();
				appProcessObj->AddComplexDataType(cdtObj);
			}
			else if (!strcmp(((char*) name), "Object"))
			{
				nodeCollObj = NodeCollection::GetNodeColObjectPointer();
				IndexCollection* idxCollObj;
				Index idxObj;

				nodeObj = nodeCollObj->GetNodePtr(nodeType, nodePos);
				//Set the NodeID
				idxObj.SetNodeID(nodeObj->GetNodeId());
				bool bhasPDO = false;

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetIndexAttributes(reader, &idxObj, bhasPDO);
					}
				}
				if (bhasPDO)
				{
					nodeObj->SetFlagForPdoObjects(bhasPDO);
				}
				idxCollObj = nodeObj->GetIndexCollection();

				NormalizeAttributeValue(&idxObj, DEFAULTVALUE);
				NormalizeAttributeValue(&idxObj, ACTUALVALUE);

				//Add Index object to the IndexCollection
				idxCollObj->AddIndex(idxObj);
				lastIndexParsed = idxCollObj->GetNumberofIndexes() - 1;
			}
			else if (!strcmp(((char*) name), "SubObject"))
			{
				IndexCollection* idxCollObj = NULL;
				SubIndex sidxObj;
				Index* idxObj = NULL;
				nodeCollObj = NodeCollection::GetNodeColObjectPointer();
				nodeObj = nodeCollObj->GetNodePtr(nodeType, nodePos);
				//Set the NodeID
				sidxObj.SetNodeID(nodeObj->GetNodeId());

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetSubIndexAttributes(reader, &sidxObj);
					}
				}

				idxCollObj = nodeObj->GetIndexCollection();
				idxObj = idxCollObj->GetIndexByPosition(lastIndexParsed);

				NormalizeAttributeValue(&sidxObj, DEFAULTVALUE);
				NormalizeAttributeValue(&sidxObj, ACTUALVALUE);

				idxObj->AddSubIndex(sidxObj);
			}
			else if ((!strcmp(((char*) name), "GeneralFeatures"))
			         || (!strcmp(((char*) name), "MNFeatures"))
			         || (!strcmp(((char*) name), "CNFeatures")))
			{
				nodeCollObj = NodeCollection::GetNodeColObjectPointer();
				Feature featureObj;
				FeatureType featureTypeObj;

				nodeObj = nodeCollObj->GetNodePtr(nodeType, nodePos);

				//Todo: Check case of feature types read from xdd/xdc
				if (!strcmp(((char*) name), "GeneralFeatures"))
				{
					featureTypeObj = GENERAL_FEATURES;
				}
				else if (!strcmp(((char*) name), "MNFeatures"))
				{
					featureTypeObj = MN_FEATURES;
				}
				else if (!strcmp(((char*) name), "CNFeatures"))
				{
					featureTypeObj = CN_FEATURES;
				}
				else
				{
					//Todo: if no features are here "eFeatureType" is uninitialized. Also this loop will not run unless another feature is added
					featureTypeObj = GENERAL_FEATURES;
					//Added other as a general features. To avoid errors
				}

				if (1 == xmlTextReaderHasAttributes(reader))
				{
					NetworkManagement* nmtObj = NULL;
					while (xmlTextReaderMoveToNextAttribute(reader))
					{
						SetFeatures(reader, &featureObj);
						featureObj.featureType = featureTypeObj;
						nmtObj = nodeObj->GetNetworkManagement();
						nmtObj->AddFeature(featureObj);
					}
				}
			}
		}
	}
	catch (const ocfmRetCode& ex)
	{
		LOG_FATAL() << ex.getErrorString();
		throw;
	}
}

ocfmRetCode ParseFile(const char* fileName, INT32 nodePos, NodeType nodeType)
{
	xmlTextReaderPtr reader;
	ocfmRetCode objException;

	reader = xmlReaderForFile(fileName, NULL, 0);
	try
	{
		if (NULL != reader)
		{
			INT32 retValue;
			retValue = xmlTextReaderRead(reader);
			while (1 == retValue)
			{
				ProcessNode(reader, nodeType, nodePos);
				retValue = xmlTextReaderRead(reader);
			}
			xmlFreeTextReader(reader);
			if (0 != retValue)
			{
				objException.setErrorCode(OCFM_ERR_PARSE_XML);
				LOG_FATAL() << objException.getErrorString();
				throw objException;
			}
		}
		else
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % fileName;
			objException.setErrorCode(OCFM_ERR_CANNOT_OPEN_FILE);
			objException.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw objException;
		}
	}
	catch (ocfmRetCode& ex)
	{
		DeleteNodeObjDict(nodePos, nodeType);
		return ex;
	}
	try
	{
		if (MN == nodeType)
		{
			Node nodeObj;
			NodeCollection* nodeCollObj = NULL;
			NetworkManagement* nmtObj = NULL;
			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			assert(nodeCollObj);
			nodeObj = nodeCollObj->GetNode(nodeType, nodePos);
			nmtObj = nodeObj.GetNetworkManagement();
			nmtObj->CalculateMaxPDOCount();
			return objException;
		}
	}
	catch (ocfmRetCode& ex)
	{
		return ex;
	}
	objException.setErrorCode(OCFM_ERR_SUCCESS);
	return objException;
}

ocfmRetCode ReImportXML(const char* fileName, INT32 nodeId, NodeType nodeType)
{
	UINT32 nodePos;
	ocfmRetCode objException;

	try
	{
		bool bFlag = false;
		objException = IfNodeExists(nodeId, nodeType, &nodePos, bFlag);
		if ((0 == objException.getErrorCode()) && (true == bFlag))
		{

			objException = DeleteNodeObjDict(nodeId, nodeType);
			if (OCFM_ERR_SUCCESS != objException.getErrorCode())
			{
				LOG_FATAL() << "DeleteNodeObjDict() failed for node " << nodeId << ".";
			}

			objException = ParseFile(fileName, nodeId, nodeType);
			if (OCFM_ERR_SUCCESS != objException.getErrorCode())
			{
				LOG_FATAL() << "ParseFile() failed for file '" << fileName << "' and node " << nodeId << ".";
				objException = DeleteNodeObjDict(nodeId, nodeType);
				if (OCFM_ERR_SUCCESS != objException.getErrorCode())
				{
					LOG_FATAL() << "DeleteNodeObjDict() failed for node " << nodeId << ".";
				}
			}
			xmlCleanupParser();
			/*
			 * this is to debug memory for regression tests
			 */
			xmlMemoryDump();

			///* Add other required index*/
			//if (nodeType == CN)
			//{
			//	//SetFlagForRequiredCNIndexes(nodeId);
			//	/* Not required only in case of autogenartion 1020 of CN should be updated else it shud be as it is imported*/
			//}
			//if (nodeType == MN)
			//{
			//	//SetFlagForRequiredMNIndexes(nodeId);
			//}
			/* Copy default value of pdos to act value*/
			CopyPDODefToAct(nodeId, nodeType);
			//CopyMNPropDefToAct(nodeId, nodeType);
			CalculateCNPollResponse(nodeId, nodeType);
			SetCNLossObjects(nodeId, nodeType);
			if (nodeType == MN)
			{
				RecalculateMultiplex();
			}

			objException.setErrorCode(OCFM_ERR_SUCCESS);

			Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
			node->SetXddPath(boost::filesystem::path(fileName));
			node->SetXdcPath("");
		}
		else
		{
			boost::format formatter(kMsgNodeIdInvalid);
			formatter % nodeId;
			objException.setErrorCode(OCFM_ERR_INVALID_NODEID);
			objException.setErrorString(formatter.str());
			LOG_FATAL() << objException.getErrorString();
			throw objException;
		}
	}
	catch (ocfmRetCode& ex)
	{
		return ex;
	}
	return objException;
}

ocfmRetCode SaveNode(const char* fileName, INT32 nodeId, NodeType nodeType)
{
	INT32 bytesWritten;
	xmlTextWriterPtr xtwWriter;
	xmlDocPtr xdDoc;
	ocfmRetCode objException;

	try
	{
		Node nodeObj;
		NodeCollection* nodeCollObj = NULL;
		IndexCollection* idxCollObj = NULL;
		//Index idxObj1;
		Index* idxObj = NULL;
		ApplicationProcess* appProcessObj = NULL;
		INT32 idxLC = 0;

		//idxObj1.SetNodeID(nodeObj.GetNodeId());
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

		idxCollObj = nodeObj.GetIndexCollection();

		if (0 == idxCollObj->GetNumberofIndexes())
		{
			objException.setErrorCode(OCFM_ERR_SUCCESS);
			return objException;
		}

		/* Create a new XmlWriter for DOM, with no compression. */
		xtwWriter = xmlNewTextWriterDoc(&xdDoc, 0);
		if (NULL == xtwWriter)
		{
			LOG_FATAL() << "Error creating XML-TextWriter.";
			objException.setErrorCode(OCFM_ERR_CREATE_XML_WRITER_FAILED);
			throw objException;
		}
		/* Start the document with the xml default for the version,
		 * encoding UTF-8 and the default for the standalone
		 * declaration. */
		bytesWritten = xmlTextWriterStartDocument(xtwWriter, NULL, MY_ENCODING,
		               NULL);
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Document.";
			objException.setErrorCode(OCFM_ERR_XML_START_DOC_FAILED);
			throw objException;
		}
		bytesWritten = xmlTextWriterWriteComment(xtwWriter,
		               BAD_CAST "This file was autogenerated by openCONFIGURATOR");

		// Start ISO15745ProfileContainer Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "ISO15745ProfileContainer");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'ISO15745ProfileContainer'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		xmlTextWriterSetIndent(xtwWriter, 1);
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "ISO15745Profile");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'ISO15745Profile'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ProfileBody Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "ProfileBody");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'ProfileBody'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ApplicationProcess Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "ApplicationProcess");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'ApplicationProcess'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		xmlTextWriterSetIndent(xtwWriter, 1);

		// Start dataTypeList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "dataTypeList");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'dataTypeList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		ComplexDataType* objCDT = NULL;
		appProcessObj = nodeObj.GetApplicationProcess();

		if (0 < appProcessObj->GetCDTCount())
		{
			for (INT32 cdtLC = 0; cdtLC < appProcessObj->GetCDTCount(); cdtLC++)
			{
				// Start struct Tag
				bytesWritten = xmlTextWriterStartElement(xtwWriter,
				               BAD_CAST "struct");
				if (0 > bytesWritten)
				{
					LOG_FATAL() << "Error starting XML-Element 'struct'.";
					objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				objCDT = appProcessObj->GetCDTByDtIndex(cdtLC);
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST "name",
				               BAD_CAST objCDT->nameIdAttr->GetName());
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST "uniqueID",
				               BAD_CAST objCDT->nameIdAttr->GetUniqueID());

				for (UINT32 varDeclLC = 0;
				        varDeclLC < objCDT->varDeclarationCollection.size();
				        varDeclLC++)
				{
					VarDeclaration vd;
					vd = objCDT->varDeclarationCollection[varDeclLC];
					// Start varDeclaration Tag
					bytesWritten = xmlTextWriterStartElement(xtwWriter,
					               BAD_CAST "varDeclaration");
					if (0 > bytesWritten)
					{
						LOG_FATAL() << "Error starting XML-Element 'varDeclaration'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "name",
					               BAD_CAST vd.namIdDtAttr->GetName());
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "uniqueID",
					               BAD_CAST vd.namIdDtAttr->GetUniqueID());
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "size", BAD_CAST vd.size);

					if (NULL != vd.namIdDtAttr->GetDataType())
					{
						bytesWritten = xmlTextWriterStartElement(xtwWriter,
						               BAD_CAST vd.namIdDtAttr->GetDataType());

						if (0 > bytesWritten)
						{
							LOG_FATAL() << "Error starting XML-Element '" << vd.namIdDtAttr->GetDataType() << "'.";
							objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}
						// End varDeclaration Tag
						bytesWritten = xmlTextWriterEndElement(xtwWriter);
						if (0 > bytesWritten)
						{
							LOG_FATAL() << "Error closing XML-Element 'varDeclaration'.";
							objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}
					}
					else if (NULL != vd.namIdDtAttr->GetDtUniqueRefId())
					{
						bytesWritten = xmlTextWriterStartElement(xtwWriter,
						               BAD_CAST "dataTypeIDRef");
						if (0 > bytesWritten)
						{
							LOG_FATAL() << "Error starting XML-Element 'dataTypeIDRef'.";
							objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}

						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						               BAD_CAST "uniqueIDRef",
						               BAD_CAST vd.namIdDtAttr->GetDtUniqueRefId());

						// End dataTypeIDRef Tag
						bytesWritten = xmlTextWriterEndElement(xtwWriter);
						if (0 > bytesWritten)
						{
							LOG_FATAL() << "Error closing XML-Element 'dataTypeIDRef'.";
							objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}
					}

					// End varDeclaration Tag
					bytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (0 > bytesWritten)
					{
						LOG_FATAL() << "Error closing XML-Element 'varDeclaration'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}
				}

				// End struct Tag
				bytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > bytesWritten)
				{
					LOG_FATAL() << "Error closing XML-Element 'struct'.";
					objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}
			}
		}

		// End dataTypeList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error closing XML-Element 'dataTypeList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// Start parameterList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "parameterList");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'parameterList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		if (NULL != nodeObj.GetApplicationProcess())
		{
			appProcessObj = nodeObj.GetApplicationProcess();

			for (UINT32 parameterLC = 0;
			        parameterLC < appProcessObj->GetParameterCollection().size();
			        parameterLC++)
			{
				Parameter parameterObj;
				parameterObj = appProcessObj->GetUniqueIDRefbyParameterIndex(
				                   parameterLC);
				// Start parameter Tag
				bytesWritten = xmlTextWriterStartElement(xtwWriter,
				               BAD_CAST "parameter");
				if (0 > bytesWritten)
				{
					LOG_FATAL() << "Error starting XML-Element 'parameter'.";
					objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST "uniqueID",
				               BAD_CAST parameterObj.nameIdDtAttr->GetUniqueID());
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST "access", BAD_CAST parameterObj.accessStr);

				if (parameterObj.nameIdDtAttr->GetDataType() != NULL)
				{
					bytesWritten = xmlTextWriterStartElement(xtwWriter, BAD_CAST parameterObj.nameIdDtAttr->GetDataType());
					if (bytesWritten < 0)
					{
						LOG_FATAL() << "Error starting XML-Element '" << parameterObj.nameIdDtAttr->GetDataType() << "'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw& objException;
					}
					bytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (bytesWritten < 0)
					{
						LOG_FATAL() << "Error closing XML-Element '" << parameterObj.nameIdDtAttr->GetDataType() << "'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw& objException;
					}
				}

				if (parameterObj.nameIdDtAttr->GetDtUniqueRefId() != NULL)
				{
					// Start dataTypeIDRef Tag
					bytesWritten = xmlTextWriterStartElement(xtwWriter, BAD_CAST "dataTypeIDRef");
					if (0 > bytesWritten)
					{
						LOG_FATAL() << "Error starting XML-Element 'dataTypeIDRef'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter, BAD_CAST "uniqueIDRef", BAD_CAST parameterObj.nameIdDtAttr->GetDtUniqueRefId());
					// End dataTypeIDRef Tag
					bytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (0 > bytesWritten)
					{
						LOG_FATAL() << "Error closing XML-Element 'dataTypeIDRef'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}
				}

				// End parameter Tag
				bytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > bytesWritten)
				{
					LOG_FATAL() << "Error closing XML-Element 'parameter'.";
					objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}
			}
		}
		// End parameterList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error closing XML-Element 'parameterList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// End ApplicationProcess Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error closing XML-Element 'ApplicationProcess'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// End ProfileBody Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error closing XML-Element 'ProfileBody'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// Start ProfileBody Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "ProfileBody");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'ProfileBody'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ApplicationLayers Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "ApplicationLayers");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'ApplicationLayers'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start DataTypeList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "DataTypeList");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'DataTypeList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		DataTypeCollection* dtCollObj = NULL;

		dtCollObj = nodeObj.GetDataTypeCollection();

		for (INT32 dtLC = 0; dtLC < dtCollObj->GetNumberOfDataTypes(); dtLC++)
		{
			DataType* dtObj = NULL;
			dtObj = dtCollObj->GetDataTypeElement(dtLC);

			// Start defType Tag
			bytesWritten = xmlTextWriterStartElement(xtwWriter,
			               BAD_CAST "defType");
			if (0 > bytesWritten)
			{
				LOG_FATAL() << "Error starting XML-Element 'defType'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}

			bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
			               BAD_CAST "dataType", BAD_CAST dtObj->dataTypeValue);
			// Start DataType Tag
			bytesWritten = xmlTextWriterStartElement(xtwWriter,
			               BAD_CAST dtObj->GetName());
			if (0 > bytesWritten)
			{
				LOG_FATAL() << "Error starting XML-Element '" << dtObj->GetName() << "'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End DataType Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (0 > bytesWritten)
			{
				LOG_FATAL() << "Error closing XML-Element 'DataType'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End defType Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (0 > bytesWritten)
			{
				LOG_FATAL() << "Error closing XML-Element 'defType'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
		}
		xmlTextWriterSetIndent(xtwWriter, 1);
		// End DataTypeList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error closing XML-Element 'DataTypeList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		if (idxCollObj->GetNumberofIndexes() == 0)
		{
			boost::format formatter(kMsgObjectDictoryEmpty);
			formatter % nodeObj.GetNodeId();
			objException.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
			objException.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			throw objException;
		}

		// Start ObjectList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "ObjectList");
		if (0 > bytesWritten)
		{
			LOG_FATAL() << "Error starting XML-Element 'ObjectList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		for (idxLC = 0; idxLC < idxCollObj->GetNumberofIndexes(); idxLC++)
		{
			// Start Object Tag
			bytesWritten = xmlTextWriterStartElement(xtwWriter,
			               BAD_CAST "Object");
			if (0 > bytesWritten)
			{
				LOG_FATAL() << "Error starting XML-Element 'Object'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}

			idxObj = idxCollObj->GetIndexByPosition(idxLC);

			if (NULL != idxObj->GetIndexValue())
			{
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST "index", BAD_CAST idxObj->GetIndexValue());
			}
			if (NULL != idxObj->GetName())
			{
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST "name", BAD_CAST idxObj->GetName());
			}
			if (NULL != idxObj->GetObjectType())
			{
				char* strObjectType = new char[10];
				strObjectType = IntToAscii(idxObj->GetEObjectType(),
				                           strObjectType, 10);
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST "objectType", BAD_CAST strObjectType);
				delete[] strObjectType;
			}
			DataType dtObj;
			dtObj = idxObj->GetDataType();
			if (NULL != dtObj.dataTypeValue)
				if (strlen(dtObj.dataTypeValue) != 0)
				{
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "dataType", BAD_CAST dtObj.dataTypeValue);
				}
			if ((idxObj->GetAccessType() != NULL))
				if (strlen(idxObj->GetAccessType()) != 0)
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "accessType",
					               BAD_CAST idxObj->GetAccessType());
			if ((idxObj->GetDefaultValue() != NULL))
				if (strlen(idxObj->GetDefaultValue()) != 0)
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "defaultValue",
					               BAD_CAST idxObj->GetDefaultValue());
			if ((idxObj->GetActualValue() != NULL))
				if (strlen(idxObj->GetActualValue()) != 0)
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "actualValue",
					               BAD_CAST idxObj->GetActualValue());
			if (idxObj->GetLowLimit() != NULL)
				if (strlen(idxObj->GetLowLimit()) != 0)
				{
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "lowLimit",
					               BAD_CAST idxObj->GetLowLimit());
				}
			if (idxObj->GetHighLimit() != NULL)
				if (strlen(idxObj->GetHighLimit()) != 0)
				{
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "highLimit",
					               BAD_CAST idxObj->GetHighLimit());
				}
			if ((idxObj->GetPDOMapping() != NULL))
			{
				if ((strlen(idxObj->GetPDOMapping()) != 0) && (0 != strcmp(idxObj->GetPDOMapping(), "NOT_DEFINED")))
				{
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "PDOmapping",
					               BAD_CAST idxObj->GetPDOMapping());
				}
			}
			if (idxObj->GetUniqueIDRef() != NULL)
				if (strlen(idxObj->GetUniqueIDRef()) != 0)
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST "uniqueIDRef",
					               BAD_CAST idxObj->GetUniqueIDRef());

			xmlTextWriterSetIndent(xtwWriter, 1);

			if (!(idxObj->GetNumberofSubIndexes() <= 0))
			{
				//SubIndexPos = 0;
				for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
				        sidxLC++)
				{
					// Start SubObject Tag
					bytesWritten = xmlTextWriterStartElement(xtwWriter,
					               BAD_CAST "SubObject");
					if (bytesWritten < 0)
					{
						LOG_FATAL() << "Error starting XML-Element 'SubObject'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					SubIndex* sidxObj = NULL;
					sidxObj = idxObj->GetSubIndexByPosition(sidxLC);
					if (sidxObj->GetIndexValue() != NULL)
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						               BAD_CAST "subIndex",
						               BAD_CAST sidxObj->GetIndexValue());
					if (sidxObj->GetName() != NULL)
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						               BAD_CAST "name", BAD_CAST sidxObj->GetName());
					if (sidxObj->GetObjectType() != NULL)
					{
						char* objectTypeStr = new char[10];
						objectTypeStr = IntToAscii(sidxObj->GetEObjectType(),
						                           objectTypeStr, 10);
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						               BAD_CAST "objectType", BAD_CAST objectTypeStr);
						delete[] objectTypeStr;
					}
					DataType dtObject;
					dtObject = sidxObj->GetDataType();
					if (dtObject.dataTypeValue != NULL)
					{
						if (strlen(dtObject.dataTypeValue) != 0)
						{
							bytesWritten = xmlTextWriterWriteAttribute(
							                   xtwWriter, BAD_CAST "dataType",
							                   BAD_CAST dtObject.dataTypeValue);
						}
					}
					if (sidxObj->GetAccessType() != NULL)
					{
						if (strlen(sidxObj->GetAccessType()) != 0)
						{
							bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							               BAD_CAST "accessType",
							               BAD_CAST sidxObj->GetAccessType());
						}
					}
					if (sidxObj->GetDefaultValue() != NULL)
					{
						if (strlen(sidxObj->GetDefaultValue()) != 0)
						{
							bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							               BAD_CAST "defaultValue",
							               BAD_CAST sidxObj->GetDefaultValue());
						}
					}
					if (sidxObj->GetActualValue() != NULL)
					{
						if (strlen(sidxObj->GetActualValue()) != 0)
						{
							bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							               BAD_CAST "actualValue",
							               BAD_CAST sidxObj->GetActualValue());
						}
					}
					if (sidxObj->GetLowLimit() != NULL)
					{
						if (strlen(sidxObj->GetLowLimit()) != 0)
						{
							bytesWritten = xmlTextWriterWriteAttribute(
							                   xtwWriter, BAD_CAST "lowLimit",
							                   BAD_CAST sidxObj->GetLowLimit());
						}
					}

					if (sidxObj->GetHighLimit() != NULL)
					{
						if (strlen(sidxObj->GetHighLimit()) != 0)
						{
							bytesWritten = xmlTextWriterWriteAttribute(
							                   xtwWriter, BAD_CAST "highLimit",
							                   BAD_CAST sidxObj->GetHighLimit());
						}
					}

					if (sidxObj->GetPDOMapping() != NULL)
					{
						if ((strlen(sidxObj->GetPDOMapping()) != 0)
						        && (0 != strcmp(sidxObj->GetPDOMapping(), "NOT_DEFINED")))
						{
							bytesWritten = xmlTextWriterWriteAttribute(
							                   xtwWriter, BAD_CAST "PDOmapping",
							                   BAD_CAST sidxObj->GetPDOMapping());
						}
					}

					if (sidxObj->GetUniqueIDRef() != NULL)
					{
						if ((strlen(sidxObj->GetUniqueIDRef()) != 0))
						{
							bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							               BAD_CAST "uniqueIDRef",
							               BAD_CAST sidxObj->GetUniqueIDRef());
						}
					}
					// End SubObject Tag
					bytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (bytesWritten < 0)
					{
						LOG_FATAL() << "Error closing XML-Element 'SubObject'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}

				}
			}

			// End Object Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (bytesWritten < 0)
			{
				LOG_FATAL() << "Error closing XML-Element 'Object'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}

		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ObjectList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error closing XML-Element 'ObjectList'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ApplicationLayers Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error closing XML-Element 'ApplicationLayers'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		// Start Network Management Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter, BAD_CAST "NetworkManagement");
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error starting XML-Element 'NetworkManagement'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);

		// Start General Features Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
		               BAD_CAST "GeneralFeatures");
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error starting XML-Element 'GeneralFeatures'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		NetworkManagement* nmtObj = NULL;
		nmtObj = nodeObj.GetNetworkManagement();

		for (UINT32 featureLC = 0; featureLC < nmtObj->GetNumberOfFeatures();
		        featureLC++)
		{
			Feature* featureObj = NULL;
			featureObj = nmtObj->GetFeature(featureLC);

			if (featureObj->featureType == GENERAL_FEATURES)
			{
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
				               BAD_CAST featureObj->name, BAD_CAST featureObj->value);
				if (bytesWritten < 0)
				{
					LOG_FATAL() << "Error starting XML-Attribute '" << featureObj->name << "'.";
					objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
			}
		}
		xmlTextWriterSetIndent(xtwWriter, 1);
		// End General Features Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);

		//Write MN Features for MN Node
		if (nodeObj.GetNodeType() == MN)
		{
			//Start MN Features Tag
			bytesWritten = xmlTextWriterStartElement(xtwWriter,
			               BAD_CAST "MNFeatures");
			if (bytesWritten < 0)
			{
				LOG_FATAL() << "Error starting XML-Element 'MNFeatures'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			nmtObj = nodeObj.GetNetworkManagement();

			for (UINT32 featureLC = 0;
			        featureLC < nmtObj->GetNumberOfFeatures(); featureLC++)
			{
				Feature* featureObj = NULL;
				featureObj = nmtObj->GetFeature(featureLC);

				if (featureObj->featureType == MN_FEATURES)
				{
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST featureObj->name,
					               BAD_CAST featureObj->value);
					if (bytesWritten < 0)
					{
						LOG_FATAL() << "Error starting XML-Attribute '" << featureObj->name << "'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
				}
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End MN Features Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);

		}

		//Write CN Features for CN Node
		else if (nodeObj.GetNodeType() == CN)
		{
			//Start CN Features Tag
			//Start MN Features Tag
			bytesWritten = xmlTextWriterStartElement(xtwWriter,
			               BAD_CAST "CNFeatures");
			if (bytesWritten < 0)
			{
				LOG_FATAL() << "Error starting XML-Element 'CNFeatures'.";
				objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			nmtObj = nodeObj.GetNetworkManagement();

			for (UINT32 featureLC = 0;
			        featureLC < nmtObj->GetNumberOfFeatures(); featureLC++)
			{
				Feature* featureObj = NULL;
				featureObj = nmtObj->GetFeature(featureLC);

				if (featureObj->featureType == CN_FEATURES)
				{
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					               BAD_CAST featureObj->name,
					               BAD_CAST featureObj->value);
					if (bytesWritten < 0)
					{
						LOG_FATAL() << "Error starting XML-Attribute '" << featureObj->name << "'.";
						objException.setErrorCode(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
				}
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			//End CN Features Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		//End Network Management Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ProfileBody Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error closing XML-Element 'ProfileBody'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ISO15745Profile Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error closing XML-Element 'ISO15745Profile'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ISO15745ProfileContainer Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error closing XML-Element 'ISO15745ProfileContainer'.";
			objException.setErrorCode(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		bytesWritten = xmlTextWriterEndDocument(xtwWriter);
		if (bytesWritten < 0)
		{
			LOG_FATAL() << "Error closing XML-Document.";
			objException.setErrorCode(OCFM_ERR_XML_END_DOC_FAILED);
			throw objException;
		}

		xmlFreeTextWriter(xtwWriter);

		xmlSaveFileEnc(fileName, xdDoc, MY_ENCODING);

		xmlFreeDoc(xdDoc);
	}
	catch (const ocfmRetCode& ex)
	{
		LOG_FATAL() << ex.getErrorString();
		throw;
	}

	objException.setErrorCode(OCFM_ERR_SUCCESS);
	return objException;
}

//FIXME: Not needed if Flag is behaviour is fixed
//void SetFlagForRequiredCNIndexes(INT32 nodeId)
//{
//	Index* idxObj = NULL;
//	IndexCollection* idxCollObj = NULL;
//	NodeCollection* nodeCollObj = NULL;
//	Node* nodeObj = NULL;
//	SubIndex* sidxObj = NULL;
//	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//	nodeObj = nodeCollObj->GetNodePtr(CN, nodeId);
//	idxCollObj = nodeObj->GetIndexCollection();
//
//	INT32 idxTotal = idxCollObj->GetNumberofIndexes();
//
//	for (INT32 idxLC = 0; idxLC < idxTotal; idxLC++)
//	{
//		idxObj = idxCollObj->GetIndexByPosition(idxLC);
//
//		if ((CheckIfNotPDO((char*) idxObj->GetIndexValue()) == false)
//		        || (strcmp((char*) idxObj->GetIndexValue(), "1F98") == 0)
//		        || (strcmp((char*) idxObj->GetIndexValue(), "1C14") == 0)
//		        || (strcmp((char*) idxObj->GetIndexValue(), "1020") == 0)
//		        || (strcmp((char*) idxObj->GetIndexValue(), "1006") == 0)
//		        || CheckIfManufactureSpecificObject(
//		            (char*) idxObj->GetIndexValue()))
//		{
//			idxObj->SetFlagIfIncludedCdc(true);
//			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
//			        sidxLC++)
//			{
//				sidxObj = idxObj->GetSubIndexByPosition(sidxLC);
//				if (sidxObj != NULL)
//				{
//					sidxObj->SetFlagIfIncludedCdc(true);
//				}
//
//			}
//		}
//	}
//}
//
//void SetFlagForRequiredMNIndexes(INT32 nodeId)
//{
//	Index* idxObj = NULL;
//	IndexCollection* idxCollObj = NULL;
//	NodeCollection* nodeCollObj = NULL;
//	Node* nodeObj = NULL;
//
//	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//	nodeObj = nodeCollObj->GetNodePtr(MN, nodeId);
//	idxCollObj = nodeObj->GetIndexCollection();
//
//	INT32 idxTotal = idxCollObj->GetNumberofIndexes();
//
//	for (INT32 idxLC = 0; idxLC < idxTotal; idxLC++)
//	{
//		idxObj = idxCollObj->GetIndexByPosition(idxLC);
//
//		if (CheckIfNotPDO((char*) idxObj->GetIndexValue()) == false
//		        || strcmp((char*) idxObj->GetIndexValue(), "1006") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1020") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1300") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1C02") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1C09") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1C14") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1F26") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1F27") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1F84") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1F8B") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1F8D") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1F92") == 0
//		        || strcmp((char*) idxObj->GetIndexValue(), "1F98") == 0)
//		{
//			idxObj->SetFlagIfIncludedCdc(true);
//			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
//			        sidxLC++)
//			{
//				SubIndex* sidxObj = NULL;
//				sidxObj = idxObj->GetSubIndexByPosition(sidxLC);
//				sidxObj->SetFlagIfIncludedCdc(true);
//			}
//		}
//		else if (strcmp((char*) idxObj->GetIndexValue(), "1F8A") == 0)
//		{
//			idxObj->SetFlagIfIncludedCdc(true);
//			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
//			        sidxLC++)
//			{
//				SubIndex* sidxObj = NULL;
//				sidxObj = idxObj->GetSubIndexByPosition(sidxLC);
//				if (strcmp((char*) sidxObj->GetIndexValue(), "02") == 0)
//				{
//					sidxObj->SetFlagIfIncludedCdc(true);
//					break;
//				}
//			}
//		}
//	}
//}

//TODO: unused function
//ocfmRetCode AddOtherRequiredCNIndexes(INT32 nodeId)
//{
//	ocfmRetCode objException;
//	char* mnIndex = new char[INDEX_LEN];
//	char* sIdx = new char[SUBINDEX_LEN];
//	IndexCollection* idxCollObj = NULL;
//	NodeCollection* nodeCollObj = NULL;
//	Node* nodeObj = NULL;
//
//	try
//	{
//		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
//		nodeObj = nodeCollObj->GetNodePtr(CN, nodeId);
//		idxCollObj = nodeObj->GetIndexCollection();
//
//		/* Add 1006*/
//		strcpy(mnIndex, "1020");
//		objException = AddIndex(nodeObj->GetNodeId(), CN, mnIndex);
//		if ((objException.getErrorCode() != 0)
//		        && (objException.getErrorCode() != OCFM_ERR_INDEX_ALREADY_EXISTS))
//		{
//			return objException;
//		}
//
//		char* valueStr = new char[16];
//		valueStr = IntToAscii(configDateGlobal, valueStr, 10);
//
//		/* Set 5ms pxcValue*/
//		/* Set subindex pxcValue 40 or 0000028 */
//		strcpy(sIdx, "01");
//		SetSIdxValue(mnIndex, sIdx, valueStr, idxCollObj, nodeObj->GetNodeId(),
//		             CN, false);
//
//		valueStr = IntToAscii(configTimeGlobal, valueStr, 10);
//
//		strcpy(sIdx, "02");
//		SetSIdxValue(mnIndex, sIdx, valueStr, idxCollObj, nodeObj->GetNodeId(),
//		             CN, false);
//
//		delete[] valueStr;
//	}
//	catch (ocfmRetCode& ex)
//	{
//		return ex;
//	}
//	objException.setErrorCode(OCFM_ERR_SUCCESS);
//	return objException;
//}

UINT32 GetDataSize(const string& dataTypeVal)
{
	string workDataTypeVal(dataTypeVal);
	transform(workDataTypeVal.begin(), workDataTypeVal.end(), workDataTypeVal.begin(), ::toupper);
	//Set default 0 for error case
	UINT32 retSize = 0;

	if (workDataTypeVal == "UNSIGNED8"
	        || workDataTypeVal == "BOOLEAN"
	        || workDataTypeVal == "INTEGER8")
	{
		retSize = 1;
	}
	else if (workDataTypeVal == "UNSIGNED16"
	         || workDataTypeVal == "INTEGER16")
	{
		retSize = 2;
	}
	else if (workDataTypeVal == "INTEGER24"
	         || workDataTypeVal == "UNSIGNED24")
	{
		retSize = 3;
	}
	else if (workDataTypeVal == "UNSIGNED32"
	         || workDataTypeVal == "INTEGER32"
	         || workDataTypeVal == "REAL32")
	{
		retSize = 4;
	}
	else if (workDataTypeVal == "INTEGER40"
	         || workDataTypeVal == "UNSIGNED40")
	{
		retSize = 5;
	}
	else if (workDataTypeVal == "INTEGER48"
	         || workDataTypeVal == "UNSIGNED48")
	{
		retSize = 6;
	}
	else if (workDataTypeVal == "INTEGER56"
	         || workDataTypeVal == "UNSIGNED56")
	{
		retSize = 7;
	}
	else if (workDataTypeVal == "UNSIGNED64"
	         || workDataTypeVal == "INTEGER64"
	         || workDataTypeVal == "REAL64")
	{
		retSize = 8;
	}
	else if (workDataTypeVal == "MAC_ADDRESS")
	{
		retSize = 6;
	}
	else if (workDataTypeVal == "IP_ADDRESS")
	{
		retSize = 4;
	}
	/* NETTIME is composed as follows:
	 STRUCT OF
	 UNSIGNED32 seconds
	 UNSIGNED32 nanoseconds */
	else if (workDataTypeVal == "NETTIME")
	{
		retSize = 8;
	}
	/*STRUCT OF
	 UNSIGNED28 ms,
	 VOID4 reserved,
	 UNSIGNED16 days
	 TIME_DIFFERENCE*/
	else if (workDataTypeVal == "TIME_DIFF")
	{
		retSize = 6;
	}
	/*STRUCT OF
	 UNSIGNED28 ms,
	 VOID4 reserved,
	 UNSIGNED16 days
	 TIME_DIFFERENCE*/
	else if (workDataTypeVal == "TIME_OF_DAY")
	{
		retSize = 6;
	}
	return retSize;
}

bool CheckIfStringDatatypes(const char* dataTypeValue)
{
	bool retVal = false;
	if (NULL != dataTypeValue)
	{
		char* dataTypeUpper = new char[strlen(dataTypeValue) + STR_ALLOC_BUFFER];
		strcpy(dataTypeUpper, dataTypeValue);

		dataTypeUpper = ConvertToUpper(dataTypeUpper);

		if ((strcmp(dataTypeUpper, "VISIBLE_STRING") == 0)
		        || (strcmp(dataTypeUpper, "OCTET_STRING") == 0)
		        || (strcmp(dataTypeUpper, "UNICODE_STRING") == 0))
		{
			retVal = true;
		}
		else
		{
			retVal = false;
		}
		delete[] dataTypeUpper;
	}
	else
	{
		LOG_FATAL() << "Parameter 'dataTypeValue' must not be NULL.";
	}
	return retVal;
}

void SetFeatures(xmlTextReaderPtr reader, Feature* featureObj)
{
	const xmlChar* name = NULL;
	const xmlChar* value = NULL;
	if (!featureObj)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'featureObj'";
		ocfmRetCode result(OCFM_ERR_INVALID_PARAMETER);
		result.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw result;
	}
	//Retrieve the pxcName and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);

	featureObj->name = new char[strlen((char*) name) + STR_ALLOC_BUFFER];
	strcpy(featureObj->name, (const char*) name);

	featureObj->value = new char[strlen((char*) value) + STR_ALLOC_BUFFER];
	strcpy(featureObj->value, (const char*) value);
}

void NormalizeAttributeValue(BaseIndex* const idxObj, AttributeType attrType)
{
	//Var Declaration for data type range checking of attribute values
	/*
	ocfmException limitError;
	unsigned long long upperLimit = 0;
	signed long long lowerLimit = 0;
	*/

	if ((attrType == ACTUALVALUE) || (attrType == DEFAULTVALUE))
	{
		DataType dtObj = idxObj->GetDataType();
		if ((NULL != dtObj.dataTypeName)
		        && (((idxObj->GetActualValue() != NULL) && (attrType == ACTUALVALUE))
		            || ((idxObj->GetDefaultValue() != NULL) && (attrType == DEFAULTVALUE))))
		{
			//TODO: system specifif code needed to determine the size of datatypes.
			//To avoid issues with big data types only type with maximum of 2 bytes are converted.
			if (CheckIfStandardDatatypes(dtObj.dataTypeName) && dtObj.dataSize <= 2)
			{
				LOG_DEBUG() << "Normalizing value for attribute type '" << "'" << attrType << ".";
				LOG_DEBUG() << "Datatype size[bytes]: " << dtObj.dataSize;
				const char* tmpValue = NULL;
				if (attrType == ACTUALVALUE)
				{
					tmpValue = idxObj->GetActualValue();
				}
				else
				{
					tmpValue = idxObj->GetDefaultValue();
				}
				LOG_DEBUG() << "Value before normalization: " << tmpValue;
				//Set the upper and lower limits for data type range checking
				/*
				if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED8") == 0)
				{
				lowerLimit = 0;
				upperLimit = UCHAR_MAX;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED16") == 0)
				{
				lowerLimit = 0;
				upperLimit = UINT_MAX;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED24") == 0)
				{
				lowerLimit = 0;
				upperLimit = 16777215;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED32") == 0)
				{
				lowerLimit = 0;
				upperLimit = 4294967295;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED40") == 0)
				{
				lowerLimit = 0;
				upperLimit = 1099511627775;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED48") == 0)
				{
				lowerLimit = 0;
				upperLimit = 281474976710655;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED56") == 0)
				{
				lowerLimit = 0;
				upperLimit = 72057594037927935;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"UNSIGNED64") == 0)
				{
				lowerLimit = 0;
				upperLimit = ULLONG_MAX;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER8") == 0)
				{
				lowerLimit = SCHAR_MIN;
				upperLimit = SCHAR_MAX;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER16") == 0)
				{
				lowerLimit = SHRT_MIN;
				upperLimit = SHRT_MAX;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER24") == 0)
				{
				lowerLimit = -8388608;
				upperLimit = 8388607;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER32") == 0)
				{
				lowerLimit = LONG_MIN;
				upperLimit = LONG_MAX;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER40") == 0)
				{
				lowerLimit = -549755813888;
				upperLimit = 549755813887;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER48") == 0)
				{
				lowerLimit = -140737488355328;
				upperLimit = 140737488355327;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER56") == 0)
				{
				lowerLimit = -36028797018963968;
				upperLimit = 36028797018963967;
				}
				else if(strcmp(StringToUpper(dtObj.dataTypeName),"INTEGER64") == 0)
				{
				lowerLimit = LLONG_MIN;
				upperLimit = LLONG_MAX;
				}
				*/
				//check hex values
				if ((strncmp(tmpValue, "0x", 2) == 0)
				        || (strncmp(tmpValue, "0X", 2) == 0))
				{
					//convert hex value into long
					long number = strtol(tmpValue, NULL, 16);
					LOG_DEBUG() << "Value converted to long: " << number;

					//Check the data type range for specific attribute
					/*
					if(!(number >= lowerLimit && number <=upperLimit))
					{
					limitError.OCFMException(OCFM_ERR_DATA_TYPE_RANGE_EXCEED);
					throw limitError;
					}
					*/
					// Allocate space for normalized hex-value: '0x' + hexValue + null-termination
					char* hexVal = new char[strlen(tmpValue) + 1];
					// Reformat
					sprintf(hexVal, "0x%X", (unsigned int) number);
					// Set
					if (attrType == ACTUALVALUE)
					{
						idxObj->SetActualValue(hexVal);
					}
					else
					{
						idxObj->SetDefaultValue(hexVal);
					}
					LOG_DEBUG() << "Value after normalization: " << hexVal;
					delete[] hexVal;
				}
				//Check data type attributes with non hex values
				/*else
				{
				int number = (int)strtol(tempActVal, NULL, 10);
				if(!(number >= lowerLimit && number <=upperLimit))
				{
				limitError.OCFMException(OCFM_ERR_DATA_TYPE_RANGE_EXCEED);
				throw limitError;
				}
				}*/
			}


			const char* tmpValue = NULL;
			if (attrType == ACTUALVALUE)
				tmpValue = idxObj->GetActualValue();
			else
				tmpValue = idxObj->GetDefaultValue();

			const string value(tmpValue);

			//Convert the IP_ADDRESS to hex value
			if (strcmp(dtObj.dataTypeName, "IP_ADDRESS") == 0)
			{
				if(value.compare(0, 2, "0x") != 0 && value.compare(0, 2, "0X") != 0)
				{
					string completeValue = "0x";
					vector<std::string> ipAddressParts;
					boost::split(ipAddressParts, value, boost::is_any_of("."));

					for (vector<string>::iterator it = ipAddressParts.begin(); it != ipAddressParts.end(); ++it)
					{
						stringstream stream;

						stream << std::hex << atol(it->c_str());
						string tempNr = stream.str();
						if (tempNr.size() == 1)
							tempNr.insert(0, "0");
						completeValue += tempNr;
					}

					if (attrType == ACTUALVALUE)
						idxObj->SetActualValue(completeValue.c_str());
					else
						idxObj->SetDefaultValue(completeValue.c_str());
				}
			}
		}
	}
}

bool CheckIfStandardDatatypes(const char* dataTypeValue)
{
	bool retVal = false;
	if (NULL != dataTypeValue)
	{
		char* dataTypeUpper = new char[strlen(dataTypeValue) + STR_ALLOC_BUFFER];
		strcpy(dataTypeUpper, dataTypeValue);

		dataTypeUpper = ConvertToUpper(dataTypeUpper);

		if ((strcmp(dataTypeUpper, "UNSIGNED8") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER8") == 0)
		        || (strcmp(dataTypeUpper, "UNSIGNED16") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER16") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER24") == 0)
		        || (strcmp(dataTypeUpper, "UNSIGNED24") == 0)
		        || (strcmp(dataTypeUpper, "UNSIGNED32") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER32") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER40") == 0)
		        || (strcmp(dataTypeUpper, "UNSIGNED40") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER48") == 0)
		        || (strcmp(dataTypeUpper, "UNSIGNED48") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER56") == 0)
		        || (strcmp(dataTypeUpper, "UNSIGNED56") == 0)
		        || (strcmp(dataTypeUpper, "INTEGER64") == 0)
		        || (strcmp(dataTypeUpper, "UNSIGNED64") == 0))
		{
			retVal = true;
		}
		else
		{
			retVal = false;
		}
		delete[] dataTypeUpper;
	}
	else
	{
		LOG_FATAL() << "Parameter 'dataTypeValue' must not be NULL.";
	}
	return retVal;
}

