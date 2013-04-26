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
#include <libxml/xmlreader.h>
#include <errno.h>
#include <string.h>
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Declarations.h"
#include "../Include/Exception.h"
#include "../Include/Validation.h"

/*****************************************************************************/
/* Defines */

#define MY_ENCODING "UTF-8"

/*****************************************************************************/
/* Global Variables */

INT32 lastIndexParsed = 0;
static const char *gpa2bSimple[][2] =
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
{ "WSTRING", "1" } }; //array size in g_simple_arr_size

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
static void SetCDTAttributes(xmlTextReaderPtr reader, ComplexDataType *cdtObj);
/*****************************************************************************/
/**
 \brief		This function shall be used to assign the retrieved attribute value from xml file to the corresponding nameIdDtAttr of the ComplexDataType object in the calling function

 \param		reader		xml pointer of xmlTextReaderPtr
 \param		cdtObj      Class pointer of ComplexDataType
 \return	void
 */
/*****************************************************************************/
static void SetVarDeclaration(xmlTextReaderPtr reader, ComplexDataType *cdtObj);

//==========================================================================//
// 					F U N C T I O N  D E F I N I T I O N S				    //
//==========================================================================//

void SetIndexAttributes(xmlTextReaderPtr reader, Index *indexObj, bool& hasPDO)
{
	const xmlChar *name = NULL;
	const xmlChar *value = NULL;
	if (NULL == indexObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
			ocfmException ex;
			ex.OCFMException(OCFM_ERR_DATATYPE_NOT_FOUND);
		}
	}
	else if (!strcmp(ConvertToUpper((char*) name), "UNIQUEIDREF"))
	{
		indexObj->SetUniqueIDRef((char*) value);
	}
	else if (!strcmp(ConvertToUpper((char*) name), "CDCFLAG"))
	{
		if (!strcmp(ConvertToUpper((char*) value), "FALSE"))
		{
			indexObj->SetFlagIfIncludedCdc(FALSE);
		}
		else if (!strcmp(ConvertToUpper((char*) value), "TRUE"))
		{
			indexObj->SetFlagIfIncludedCdc(TRUE);
		}
		else
		{
			//Nothing to be added.
		}
	}
	else
	{
		//TODO: Else added. Operation to be specified.
	}
}

void SetSubIndexAttributes(xmlTextReaderPtr reader, SubIndex *sidxObj)
{
	const xmlChar *name = NULL;
	const xmlChar *value = NULL;
	if (NULL == sidxObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
	else if (!strcmp(ConvertToUpper((char*) name), "CDCFLAG"))
	{
		if (!strcmp(ConvertToUpper((char*) value), "FALSE"))
		{
			sidxObj->SetFlagIfIncludedCdc(FALSE);
		}
		else if (!strcmp(ConvertToUpper((char*) value), "TRUE"))
		{
			sidxObj->SetFlagIfIncludedCdc(TRUE);
		}
		else
		{
			//Nothing will be executed.
		}
	}
	else
	{
#ifdef DEBUG
		cout << "Error! setSubIndexAttributes fails\n" << endl;
#endif
	}
}

void SetDataTypeAttributes(xmlTextReaderPtr reader, DataType *dtObj)
{
	const xmlChar *name = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == dtObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
			throw objException;
		}
		while (XML_READER_TYPE_ELEMENT != xmlTextReaderNodeType(reader))
		{
			iRetVal = xmlTextReaderRead(reader);
			if (iRetVal != 1)
			{
				ocfmException objException;
				objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
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
#ifdef DEBUG
			cout << __FUNCTION__ << " unhandled datatype: " << dtObj->dataTypeName
			<< endl;
#endif
		}
	}
}

void SetParameterAttributes(xmlTextReaderPtr reader, Parameter *parameterObj)
{
	const xmlChar *xcName = NULL;
	const xmlChar *xcValue = NULL;
	if (NULL == parameterObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	xcValue = xmlTextReaderConstValue(reader);
	xcName = xmlTextReaderConstName(reader);

	if (!strcmp(ConvertToUpper((char*) xcName), "UNIQUEID"))
	{
		parameterObj->nameIdDtAttr.SetUniqueID((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "NAME"))
	{
		CheckAndCorrectName((char*) xcValue);
		parameterObj->nameIdDtAttr.SetName((char*) xcValue);
	}
	else if (!strcmp(ConvertToUpper((char*) xcName), "ACCESS"))
	{
		parameterObj->accessStr = new char[strlen((const char*) xcValue) + 1];
		strcpy(parameterObj->accessStr, (const char*) xcValue);
	}
	else
	{
		//TODO: Else added. operation to be added
	}

	/* TO DO: DATATYPE..There is no tag for it..need to check after how many reads datatype is define </label>
	 <USINT/>*/
}

void SetParaDT(xmlTextReaderPtr reader, Parameter *parameterObj)
{
	const xmlChar *name = NULL;
	const xmlChar *value = NULL;
	INT32 iRetVal;
	ocfmException exceptionObj;
	if (NULL == parameterObj)
	{
		exceptionObj.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw exceptionObj;
	}
	iRetVal = xmlTextReaderRead(reader);

	if (1 != iRetVal)
	{
		exceptionObj.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
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
			exceptionObj.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
			throw exceptionObj;
		}

		value = xmlTextReaderConstValue(reader);
		name = xmlTextReaderConstName(reader);

		char abSize[3];

		if (CheckifSimpleDT((char*) name, abSize))
		{
			parameterObj->nameIdDtAttr.SetDataType((char*) name);
		}
		if (CheckStartElement(xmlTextReaderNodeType(reader), (char*) name,
				(char*) "dataTypeIDRef"))
		{
			if (TRUE == xmlTextReaderHasAttributes(reader))
			{
				xmlTextReaderMoveToNextAttribute(reader);
				value = xmlTextReaderConstValue(reader);
				name = xmlTextReaderConstName(reader);

				if (0 == strcmp(ConvertToUpper((char*) name), "UNIQUEIDREF"))
				{
					parameterObj->nameIdDtAttr.SetDtUniqueRefId((char*) value);
				}
			}
		}
	}
}

static void SetCDTAttributes(xmlTextReaderPtr reader, ComplexDataType *cdtObj)
{
	const xmlChar *name = NULL;
	const xmlChar *value = NULL;
	if (NULL == cdtObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
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
	else
	{
		//TODO: else addded. Operation to be added.
	}
}

bool CheckifSimpleDT(char *datatypeName, char *dataSize)
{
	INT32 arrSizeLC = 0;
	char *simpleElement = NULL;

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

void SetVarDecAttributes(xmlTextReaderPtr reader, varDeclaration& vdecl)
{
	const xmlChar *name = NULL;
	const xmlChar *value = NULL;

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
	else
	{
		//TODO: Else added. Operation to be added.
	}

	if (!strcmp(ConvertToUpper((char*) name), "SIZE"))
	{
		strcpy((char*) vdecl.size, (const char*) value);
	}
}

bool CheckEndElement(INT32 elementId, char *srcElement, char *compareElement)
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

bool CheckStartElement(INT32 elementId, char *srcElement, char *compareElement)
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
static void SetVarDeclaration(xmlTextReaderPtr reader, ComplexDataType *cdtObj)
{
	const xmlChar *name = NULL;
	const xmlChar *value = NULL;
	ocfmException objException;

	if (NULL == cdtObj)
	{
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	INT32 retValue;
	varDeclaration varDeclObj;
	varDeclObj.Initialize();
	retValue = xmlTextReaderRead(reader);

	if (1 != retValue)
	{
		objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
		throw objException;
	}

	name = xmlTextReaderConstName(reader);
	value = xmlTextReaderConstValue(reader);

	while (!(CheckEndElement(xmlTextReaderNodeType(reader), (char*) name,
			(char*) "struct")))
	{
		varDeclaration objTempVarDecl;
		objTempVarDecl.Initialize();

		try
		{
			retValue = xmlTextReaderRead(reader);

			if (1 != retValue)
			{
				objException.OCFMException(OCFM_ERR_XML_FILE_CORRUPTED);
				throw objException;
			}
		} catch (ocfmException *ex)
		{
			throw ex->_ocfmRetCode;
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

ocfmRetCode ImportXML(char *fileName, INT32 nodeId, NodeType nodeType)
{
	ocfmRetCode errCodeObj;
	try
	{
		errCodeObj = ParseFile(fileName, nodeId, nodeType);

		if (0 != errCodeObj.code)
		{
			return errCodeObj;
		}
		xmlCleanupParser();
		/*
		 * this is to debug memory for regression tests
		 */
		xmlMemoryDump();

		/* Add other required index*/
		if (CN == nodeType)
		{
			SetFlagForRequiredCNIndexes(nodeId);
			/* Not required only in case of auto generation 1020 of CN should be updated else it should be as it is imported*/
			//AddOtherRequiredCNIndexes(iNodeID);
		}
		if (MN == nodeType)
		{
			SetFlagForRequiredMNIndexes(nodeId);

		}

		/* Copy default value of pdos to act value*/
		CopyPDODefToAct(nodeId, nodeType);
		CopyMNPropDefToAct(nodeId, nodeType);
		CalculateCNPollResponse(nodeId, nodeType);
		SetCNLossObjects(nodeId, nodeType);
		errCodeObj.code = OCFM_ERR_SUCCESS;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	return errCodeObj;
}

void ProcessNode(xmlTextReaderPtr reader, NodeType nodeType, INT32 nodePos)
{
	const xmlChar *name = NULL;
	NodeCollection *nodeCollObj = NULL;
	Node *nodeObj = NULL;

	name = xmlTextReaderConstName(reader);

	try
	{
		//If the NodeTYPE is ELEMENT
		if (TRUE == xmlTextReaderNodeType(reader))
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
				idxObj = idxCollObj->GetIndex(lastIndexParsed);
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
	} catch (ocfmException* ex)
	{
		throw ex->_ocfmRetCode;
	}
}

ocfmRetCode ParseFile(char* fileName, INT32 nodePos, NodeType nodeType)
{
	xmlTextReaderPtr reader;
	ocfmException objException;

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
				objException.OCFMException(OCFM_ERR_PARSE_XML);
				throw objException;
			}
		}
		else
		{
			objException.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
			throw objException;
		}
	} catch (ocfmException& ex)
	{
		DeleteNodeObjDict(nodePos, nodeType);
		return ex._ocfmRetCode;
	}
	try
	{
		if (MN == nodeType)
		{
			Node nodeObj;
			NodeCollection *nodeCollObj = NULL;
			NetworkManagement *nmtObj = NULL;
			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			if (NULL == nodeCollObj)
			{
#if defined DEBUG
				cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

				objException.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
				throw objException;
			}
			nodeObj = nodeCollObj->GetNode(nodeType, nodePos);
			nmtObj = nodeObj.GetNetworkManagement();
			if (NULL == nmtObj)
			{
#if defined DEBUG
				cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

				objException.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
				throw objException;
			}
			nmtObj->CalculateMaxPDOCount();
			return objException._ocfmRetCode;
		}
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	ocfmRetCode errCodeObj;
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return errCodeObj;
}

ocfmRetCode ReImportXML(char* fileName, INT32 nodeId, NodeType nodeType)
{
	INT32 nodePos;
	ocfmRetCode errCodeObj;
	try
	{
		bool bFlag = false;
		errCodeObj = IfNodeExists(nodeId, nodeType, &nodePos, bFlag);
		if ((0 == errCodeObj.code) && (true == bFlag))
		{

			errCodeObj = DeleteNodeObjDict(nodeId, nodeType);
			if (OCFM_ERR_SUCCESS != errCodeObj.code)
			{
#if defined DEBUG
				cout << "\nDeleteNodeObjDict in ReImport failed\n" << endl;
#endif
			}
			errCodeObj = ParseFile(fileName, nodeId, nodeType);
			if (OCFM_ERR_SUCCESS != errCodeObj.code)
			{
#if defined DEBUG
				cout << "\nparseFile in ReImport failed\n" << endl;
#endif
				errCodeObj = DeleteNodeObjDict(nodeId, nodeType);
				if (OCFM_ERR_SUCCESS != errCodeObj.code)
				{
#if defined DEBUG
					cout << "\nDeleteNodeObjDict in ReImport failed\n" << endl;
#endif
				}
			}
			xmlCleanupParser();
			/*
			 * this is to debug memory for regression tests
			 */
			xmlMemoryDump();

			/* Add other required index*/
			if (nodeType == CN)
			{
				SetFlagForRequiredCNIndexes(nodeId);
				/* Not required only in case of autogenartion 1020 of CN should be updated else it shud be as it is imported*/
			}
			if (nodeType == MN)
			{
				SetFlagForRequiredMNIndexes(nodeId);
			}
			/* Copy default value of pdos to act value*/
			CopyPDODefToAct(nodeId, nodeType);
			CopyMNPropDefToAct(nodeId, nodeType);
			CalculateCNPollResponse(nodeId, nodeType);
			SetCNLossObjects(nodeId, nodeType);
			if (nodeType == MN)
			{
				RecalculateMultiplex();
			}

			errCodeObj.code = OCFM_ERR_SUCCESS;
		}
		else
		{
			ocfmException objException;
			objException.OCFMException(OCFM_ERR_INVALID_NODEID);
			throw objException;
		}
	} catch (ocfmException* ex)
	{
		return ex->_ocfmRetCode;
	}
	return errCodeObj;
}

ocfmRetCode SaveNode(const char* fileName, INT32 nodeId, NodeType nodeType)
{
	INT32 bytesWritten;
	xmlTextWriterPtr xtwWriter;
	xmlDocPtr xdDoc;
	ocfmRetCode errCodeObj;
	ocfmException objException;

	try
	{
		Node nodeObj;
		NodeCollection *nodeCollObj = NULL;
		IndexCollection *idxCollObj = NULL;
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
			errCodeObj.code = OCFM_ERR_SUCCESS;
			return errCodeObj;
		}

		/* Create a new XmlWriter for DOM, with no compression. */
		xtwWriter = xmlNewTextWriterDoc(&xdDoc, 0);
		if (NULL == xtwWriter)
		{
			printf("testXmlpxtwWriterDoc: Error creating the xml pxtwWriter\n");
			objException.OCFMException(OCFM_ERR_CREATE_XML_WRITER_FAILED);
			throw objException;
		}
		/* Start the document with the xml default for the version,
		 * encoding UTF-8 and the default for the standalone
		 * declaration. */
		bytesWritten = xmlTextWriterStartDocument(xtwWriter, NULL, MY_ENCODING,
				NULL);
		if (0 > bytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterStartDocument\n");
			objException.OCFMException(OCFM_ERR_XML_START_DOC_FAILED);
			throw objException;
		}
		bytesWritten = xmlTextWriterWriteComment(xtwWriter,
				BAD_CAST "This file was autogenerated by openCONFIGURATOR");

		// Start ISO15745ProfileContainer Tag		
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ISO15745ProfileContainer");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ProfileBody Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ProfileBody");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ApplicationProcess Tag	
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ApplicationProcess");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		xmlTextWriterSetIndent(xtwWriter, 1);

		// Start dataTypeList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "dataTypeList");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
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
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				objCDT = appProcessObj->GetCDTbyCount(cdtLC);
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "name",
						BAD_CAST objCDT->nameIdAttr->GetName());
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "uniqueID",
						BAD_CAST objCDT->nameIdAttr->uniqueId);

				for (INT32 varDeclLC = 0;
						varDeclLC < objCDT->varDeclarationCollection.Count();
						varDeclLC++)
				{
					varDeclaration vd;
					vd.Initialize();
					vd = objCDT->varDeclarationCollection[varDeclLC];
					// Start varDeclaration Tag
					bytesWritten = xmlTextWriterStartElement(xtwWriter,
							BAD_CAST "varDeclaration");
					if (0 > bytesWritten)
					{
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "name",
							BAD_CAST vd.namIdDtAttr->GetName());
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "uniqueID",
							BAD_CAST vd.namIdDtAttr->uniqueId);
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "size", BAD_CAST vd.size);

					if (NULL != vd.namIdDtAttr->GetDataType())
					{
						bytesWritten = xmlTextWriterStartElement(xtwWriter,
								BAD_CAST vd.namIdDtAttr->GetDataType());

						if (0 > bytesWritten)
						{
							printf(
									"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}
						// End varDeclaration Tag
						bytesWritten = xmlTextWriterEndElement(xtwWriter);
						if (0 > bytesWritten)
						{
							printf(
									"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}
					}
					else if (NULL != vd.namIdDtAttr->GetDtUniqueRefId())
					{
						bytesWritten = xmlTextWriterStartElement(xtwWriter,
								BAD_CAST "dataTypeIDRef");
						if (0 > bytesWritten)
						{
							printf(
									"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_START_ELT_FAILED);
							throw objException;
						}

						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "uniqueIDRef",
								BAD_CAST vd.namIdDtAttr->GetDtUniqueRefId());

						// End dataTypeIDRef Tag
						bytesWritten = xmlTextWriterEndElement(xtwWriter);
						if (0 > bytesWritten)
						{
							printf(
									"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
							ocfmException objException;
							objException.OCFMException(
									OCFM_ERR_XML_WRITER_END_ELT_FAILED);
							throw objException;
						}
					}
					else
					{
						//TODO: Else added. Operation to be added.
					}

					// End varDeclaration Tag
					bytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (0 > bytesWritten)
					{
						printf(
								"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}
				}

				// End struct Tag
				bytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > bytesWritten)
				{
					printf(
							"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}
			}
		}

		// End dataTypeList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// Start parameterList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "parameterList");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		if (NULL != nodeObj.GetApplicationProcess())
		{
			appProcessObj = nodeObj.GetApplicationProcess();

			for (INT32 parameterLC = 0;
					parameterLC < appProcessObj->ParameterCollection.Count();
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
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "uniqueID",
						BAD_CAST parameterObj.nameIdDtAttr.GetUniqueID());
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "access", BAD_CAST parameterObj.accessStr);

				// Start dataTypeIDRef Tag
				bytesWritten = xmlTextWriterStartElement(xtwWriter,
						BAD_CAST "dataTypeIDRef");
				if (0 > bytesWritten)
				{
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
					throw objException;
				}
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "uniqueIDRef",
						BAD_CAST parameterObj.nameIdDtAttr.GetDtUniqueRefId());
				// End dataTypeIDRef Tag
				bytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > bytesWritten)
				{
					printf(
							"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}

				// End parameter Tag
				bytesWritten = xmlTextWriterEndElement(xtwWriter);
				if (0 > bytesWritten)
				{
					printf(
							"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_END_ELT_FAILED);
					throw objException;
				}
			}
		}
		// End parameterList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// End ApplicationProcess Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// End ProfileBody Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		// Start ProfileBody Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ProfileBody");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start ApplicationLayers Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ApplicationLayers");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		// Start DataTypeList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "DataTypeList");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		DataTypeCollection *dtCollObj = NULL;

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
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}

			bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
					BAD_CAST "dataType", BAD_CAST dtObj->dataTypeValue);
			// Start DataType Tag
			bytesWritten = xmlTextWriterStartElement(xtwWriter,
					BAD_CAST dtObj->GetName());
			if (0 > bytesWritten)
			{
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End DataType Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (0 > bytesWritten)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			// End defType Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (0 > bytesWritten)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}
		}
		xmlTextWriterSetIndent(xtwWriter, 1);
		// End DataTypeList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (0 > bytesWritten)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		if (0 == idxCollObj->GetNumberofIndexes())
		{
			printf("SaveNode: No Indexes Found\n");
			objException.OCFMException(OCFM_ERR_NO_INDEX_FOUND);
			throw objException;
		}

		// Start ObjectList Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "ObjectList");
		if (0 > bytesWritten)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		for (idxLC = 0; idxLC < idxCollObj->GetNumberofIndexes(); idxLC++)
		{
			// Start Object Tag
			bytesWritten = xmlTextWriterStartElement(xtwWriter,
					BAD_CAST "Object");
			if (0 > bytesWritten)
			{
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}

			idxObj = idxCollObj->GetIndex(idxLC);

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
				char *strObjectType = new char[10];
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
				if (strlen(idxObj->GetPDOMapping()) != 0)
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "PDOmapping",
							BAD_CAST idxObj->GetPDOMapping());
			if (idxObj->GetUniqueIDRef() != NULL)
				if (strlen(idxObj->GetUniqueIDRef()) != 0)
					bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
							BAD_CAST "uniqueIDRef",
							BAD_CAST idxObj->GetUniqueIDRef());
			if (idxObj->GetFlagIfIncludedCdc() == 0)
			{
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "CDCFlag", BAD_CAST "FALSE");
			}
			else if (idxObj->GetFlagIfIncludedCdc() == 1)
			{
				bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
						BAD_CAST "CDCFlag", BAD_CAST "TRUE");
			}
			else
			{
				//Nothing to do.
			}

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
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
					SubIndex* sidxObj = NULL;
					sidxObj = idxObj->GetSubIndex(sidxLC);
					if (sidxObj->GetIndexValue() != NULL)
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "subIndex",
								BAD_CAST sidxObj->GetIndexValue());
					if (sidxObj->GetName() != NULL)
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "name", BAD_CAST sidxObj->GetName());
					if (sidxObj->GetObjectType() != NULL)
					{
						char *objectTypeStr = new char[10];
						objectTypeStr = IntToAscii(sidxObj->GetEObjectType(),
								objectTypeStr, 10);
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "objectType", BAD_CAST objectTypeStr);
						delete[] objectTypeStr;
					}
					DataType dtObject;
					dtObject = sidxObj->GetDataType();
					if (dtObject.dataTypeValue != NULL)
						if (strlen(dtObject.dataTypeValue) != 0)
							bytesWritten = xmlTextWriterWriteAttribute(
									xtwWriter, BAD_CAST "dataType",
									BAD_CAST dtObject.dataTypeValue);
					if (sidxObj->GetAccessType() != NULL)
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "accessType",
								BAD_CAST sidxObj->GetAccessType());
					if (sidxObj->GetDefaultValue() != NULL)
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "defaultValue",
								BAD_CAST sidxObj->GetDefaultValue());
					if (sidxObj->GetActualValue() != NULL)
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "actualValue",
								BAD_CAST sidxObj->GetActualValue());
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
						bytesWritten = xmlTextWriterWriteAttribute(
								xtwWriter, BAD_CAST "PDOmapping",
								BAD_CAST sidxObj->GetPDOMapping());
					}

					if (sidxObj->GetUniqueIDRef() != NULL)
					{
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "uniqueIDRef",
								BAD_CAST sidxObj->GetUniqueIDRef());
					}

					if (sidxObj->GetFlagIfIncludedCdc() == 0)
					{
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "CDCFlag", BAD_CAST "FALSE");
					}
					else if (sidxObj->GetFlagIfIncludedCdc() == 1)
					{
						bytesWritten = xmlTextWriterWriteAttribute(xtwWriter,
								BAD_CAST "CDCFlag", BAD_CAST "TRUE");
					}
					else
					{
						//Nothing to do. 
					}
					// End SubObject Tag
					bytesWritten = xmlTextWriterEndElement(xtwWriter);
					if (bytesWritten < 0)
					{
						printf(
								"testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_END_ELT_FAILED);
						throw objException;
					}

				}
			}

			// End Object Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
			if (bytesWritten < 0)
			{
				printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
				objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
				throw objException;
			}

		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ObjectList Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ApplicationLayers Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}
		// Start Network Management Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "NetworkManagement");
		if (bytesWritten < 0)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);

		// Start General Features Tag
		bytesWritten = xmlTextWriterStartElement(xtwWriter,
				BAD_CAST "GeneralFeatures");
		if (bytesWritten < 0)
		{
			printf("testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_START_ELT_FAILED);
			throw objException;
		}
		NetworkManagement *nmtObj = NULL;
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
					printf(
							"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
					objException.OCFMException(
							OCFM_ERR_XML_WRITER_START_ELT_FAILED);
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
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			NetworkManagement *nmtObj = NULL;
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
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
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
				printf(
						"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
				objException.OCFMException(
						OCFM_ERR_XML_WRITER_START_ELT_FAILED);
				throw objException;
			}
			NetworkManagement *nmtObj = NULL;
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
						printf(
								"testXmlwriterMemory: Error at xmlTextWriterStartElement\n");
						objException.OCFMException(
								OCFM_ERR_XML_WRITER_START_ELT_FAILED);
						throw objException;
					}
				}
			}
			xmlTextWriterSetIndent(xtwWriter, 1);
			//End CN Features Tag
			bytesWritten = xmlTextWriterEndElement(xtwWriter);
		}
		else
		{
			//Nothing other than MN & CN
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		//End Network Management Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ProfileBody Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		xmlTextWriterSetIndent(xtwWriter, 1);
		// End ISO15745ProfileContainer Tag
		bytesWritten = xmlTextWriterEndElement(xtwWriter);
		if (bytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndElement\n");
			objException.OCFMException(OCFM_ERR_XML_WRITER_END_ELT_FAILED);
			throw objException;
		}

		bytesWritten = xmlTextWriterEndDocument(xtwWriter);
		if (bytesWritten < 0)
		{
			printf("testXmlwriterDoc: Error at xmlTextWriterEndDocument\n");
			objException.OCFMException(OCFM_ERR_XML_END_DOC_FAILED);
			throw objException;
		}

		xmlFreeTextWriter(xtwWriter);

		xmlSaveFileEnc(fileName, xdDoc, MY_ENCODING);

		xmlFreeDoc(xdDoc);
	} catch (ocfmException *ex)
	{
		throw ex->_ocfmRetCode;
	}

	errCodeObj.code = OCFM_ERR_SUCCESS;
	return errCodeObj;
}

void SetFlagForRequiredCNIndexes(INT32 nodeId)
{
	Index* idxObj = NULL;
	IndexCollection* idxCollObj = NULL;
	NodeCollection* nodeCollObj = NULL;
	Node* nodeObj = NULL;
	SubIndex* sidxObj = NULL;
	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(CN, nodeId);
	idxCollObj = nodeObj->GetIndexCollection();

	INT32 idxTotal = idxCollObj->GetNumberofIndexes();

	for (INT32 idxLC = 0; idxLC < idxTotal; idxLC++)
	{
		idxObj = idxCollObj->GetIndex(idxLC);

		if ((CheckIfNotPDO((char*) idxObj->GetIndexValue()) == false)
				|| (strcmp((char*) idxObj->GetIndexValue(), "1F98") == 0)
				|| (strcmp((char*) idxObj->GetIndexValue(), "1C14") == 0)
				|| (strcmp((char*) idxObj->GetIndexValue(), "1020") == 0)
				|| (strcmp((char*) idxObj->GetIndexValue(), "1006") == 0)
				|| CheckIfManufactureSpecificObject(
						(char*) idxObj->GetIndexValue()))
		{
			idxObj->SetFlagIfIncludedCdc(TRUE);
			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
					sidxLC++)
			{
				sidxObj = idxObj->GetSubIndex(sidxLC);
				if (sidxObj != NULL)
				{
					sidxObj->SetFlagIfIncludedCdc(TRUE);
				}

			}
		}
	}
}

void SetFlagForRequiredMNIndexes(INT32 nodeId)
{
	Index* idxObj = NULL;
	IndexCollection* idxCollObj = NULL;
	NodeCollection* nodeCollObj = NULL;
	Node* nodeObj = NULL;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	nodeObj = nodeCollObj->GetNodePtr(MN, nodeId);
	idxCollObj = nodeObj->GetIndexCollection();

	INT32 idxTotal = idxCollObj->GetNumberofIndexes();

	for (INT32 idxLC = 0; idxLC < idxTotal; idxLC++)
	{
		idxObj = idxCollObj->GetIndex(idxLC);

		if (CheckIfNotPDO((char*) idxObj->GetIndexValue()) == false
				|| strcmp((char*) idxObj->GetIndexValue(), "1006") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1020") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1300") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1C02") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1C09") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1C14") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1F26") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1F27") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1F84") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1F8B") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1F8D") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1F92") == 0
				|| strcmp((char*) idxObj->GetIndexValue(), "1F98") == 0)
		{
			idxObj->SetFlagIfIncludedCdc(TRUE);
			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
					sidxLC++)
			{
				SubIndex* sidxObj = NULL;
				sidxObj = idxObj->GetSubIndex(sidxLC);
				sidxObj->SetFlagIfIncludedCdc(TRUE);
			}
		}
		else if (strcmp((char*) idxObj->GetIndexValue(), "1F8A") == 0)
		{
			idxObj->SetFlagIfIncludedCdc(TRUE);
			for (INT32 sidxLC = 0; sidxLC < idxObj->GetNumberofSubIndexes();
					sidxLC++)
			{
				SubIndex* sidxObj = NULL;
				sidxObj = idxObj->GetSubIndex(sidxLC);
				if (strcmp((char*) sidxObj->GetIndexValue(), "02") == 0)
				{
					sidxObj->SetFlagIfIncludedCdc(TRUE);
					break;
				}
			}
		}
		else
		{
			//Nothing to be added.
		}
	}
}

//TODO: unused function
ocfmRetCode AddOtherRequiredCNIndexes(INT32 nodeId)
{
	ocfmRetCode errCodeObj;
	char* mnIndex = new char[INDEX_LEN];
	char* sIdx = new char[SUBINDEX_LEN];
	IndexCollection* idxCollObj = NULL;
	NodeCollection* nodeCollObj = NULL;
	Node* nodeObj = NULL;

	try
	{
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeObj = nodeCollObj->GetNodePtr(CN, nodeId);
		idxCollObj = nodeObj->GetIndexCollection();

		/* Add 1006*/
		strcpy(mnIndex, "1020");
#if defined DEBUG	
		cout << "string copied" << endl;
#endif
		errCodeObj = AddIndex(nodeObj->GetNodeId(), CN, mnIndex);
#if defined DEBUG	
		cout << "stRetCode" << errCodeObj.code << endl;
		cout << "1020 added" << endl;
#endif
		if ((errCodeObj.code != 0)
				&& (errCodeObj.code != OCFM_ERR_INDEX_ALREADY_EXISTS))
		{
			return errCodeObj;
		}

		char* valueStr = new char[16];
		valueStr = IntToAscii(configDateGlobal, valueStr, 10);

		/* Set 5ms pxcValue*/
		/* Set subindex pxcValue 40 or 0000028 */
		strcpy(sIdx, "01");
		SetSIdxValue(mnIndex, sIdx, valueStr, idxCollObj, nodeObj->GetNodeId(),
				CN, false);

		valueStr = IntToAscii(configTimeGlobal, valueStr, 10);

		strcpy(sIdx, "02");
		SetSIdxValue(mnIndex, sIdx, valueStr, idxCollObj, nodeObj->GetNodeId(),
				CN, false);

		delete[] valueStr;
	} catch (ocfmException& ex)
	{
		return ex._ocfmRetCode;
	}
	errCodeObj.code = OCFM_ERR_SUCCESS;
	return errCodeObj;
}

INT32 GetDataSize(char* dataTypeVal)
{
	if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED8") == 0
			|| strcmp(StringToUpper(dataTypeVal), "BOOLEAN") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER8") == 0)
	{
		return 1;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED16") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER16") == 0)
	{
		return 2;
	}

	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER24") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED24") == 0)
	{
		return 3;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED32") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER32") == 0
			|| strcmp(StringToUpper(dataTypeVal), "REAL32") == 0)
	{
		return 4;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER40") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED40") == 0)
	{
		return 5;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER48") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED48") == 0)
	{
		return 6;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "INTEGER56") == 0
			|| strcmp(StringToUpper(dataTypeVal), "UNSIGNED56") == 0)
	{
		return 7;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "UNSIGNED64") == 0
			|| strcmp(StringToUpper(dataTypeVal), "INTEGER64") == 0
			|| strcmp(StringToUpper(dataTypeVal), "REAL64") == 0)
	{
		return 8;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "MAC_ADDRESS") == 0)
	{
		return 6;
	}
	else if (strcmp(StringToUpper(dataTypeVal), "IP_ADDRESS") == 0)
	{
		return 4;
	}
	/* NETTIME is composed as follows:
	 STRUCT OF
	 UNSIGNED32 seconds
	 UNSIGNED32 nanoseconds */
	else if (strcmp(StringToUpper(dataTypeVal), "NETTIME") == 0)
	{
		return 8;
	}
	/*STRUCT OF
	 UNSIGNED28 ms,
	 VOID4 reserved,
	 UNSIGNED16 days
	 TIME_DIFFERENCE*/
	else if (strcmp(StringToUpper(dataTypeVal), "TIME_DIFF") == 0)
	{
		return 6;
	}
	/*STRUCT OF
	 UNSIGNED28 ms,
	 VOID4 reserved,
	 UNSIGNED16 days
	 TIME_DIFFERENCE*/
	else if (strcmp(StringToUpper(dataTypeVal), "TIME_OF_DAY") == 0)
	{
		return 6;
	}
	else
	{
#if defined DEBUG
		cout << __FUNCTION__ << " Unhandled datatype:" << dataTypeVal << endl;
#endif
	}
	return 1; //by default
}

bool CheckIfStringDatatypes(char* dataTypeValue)
{
	if (strcmp(StringToUpper(dataTypeValue), "VISIBLE_STRING") == 0
			|| strcmp(StringToUpper(dataTypeValue), "OCTET_STRING") == 0
			|| strcmp(StringToUpper(dataTypeValue), "UNICODE_STRING") == 0)
		return true;
	else
		return false;
}

void SetFeatures(xmlTextReaderPtr reader, Feature *featureObj)
{
	const xmlChar *name = NULL;
	const xmlChar *value = NULL;
	if (NULL == featureObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	//Retrieve the pxcName and Value of an attribute
	value = xmlTextReaderConstValue(reader);
	name = xmlTextReaderConstName(reader);

	featureObj->name = new char[strlen((char*) name) + STR_ALLOC_BUFFER];
	strcpy(featureObj->name, (const char*) name);

	featureObj->value = new char[strlen((char*) value) + STR_ALLOC_BUFFER];
	strcpy(featureObj->value, (const char*) value);

}

