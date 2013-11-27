/******************************************************************************
 \file		Exception.cpp
 \brief		This file contains the class member definitions that are used to throw exception to the GUI when an error occurs
 ******************************************************************************/

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

#include <iostream>
#include <sstream>
#include <exception>
#include <string>
#include <libxml/xmlerror.h>
#include "../Include/Exception.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E C L A R A T I O N S 					//
//==========================================================================//

ocfmRetCode::ocfmRetCode(void)
	: errorString(), code(OCFM_ERR_SUCCESS)
{}

ocfmRetCode::ocfmRetCode(ConfiguratorError errorCode)
	: errorString(), code(errorCode)
{}

ocfmRetCode::~ocfmRetCode(void) throw ()
{

}

void ocfmRetCode::setErrorCode(ConfiguratorError error)
{
	this->code = error;
	switch (error)
	{
	case OCFM_ERR_SUCCESS:
		this->errorString = "Success";
		break;
	case OCFM_ERR_FILE_NOT_PRESENT:
		this->errorString = "File not present";
		break;
	case OCFM_ERR_FILE_CANNOT_OPEN:
		this->errorString = "File cannot open";
		break;
	case OCFM_ERR_INVALID_NODEID:
		this->errorString = "Invalid Node ID";
		break;
	case OCFM_ERR_INVALID_NODEPOS:
		this->errorString = "Invalid Node position";
		break;
	case OCFM_ERR_INVALID_NODETYPE:
		this->errorString = "Invalid Node type";
		break;
	case OCFM_ERR_DATATYPE_NOT_FOUND:
		this->errorString = "DataType Not Found in the DataTypeList of XDC";
		break;
	case OCFM_ERR_INVALID_INDEXID:
		this->errorString = "Invalid Index ID";
		break;
	case OCFM_ERR_INVALID_INDEXPOS:
		this->errorString = "Invalid Index position";
		break;
	case OCFM_ERR_INVALID_SUBINDEXID:
		this->errorString = "Invalid SubIndex ID";
		break;
	case OCFM_ERR_INVALID_SUBINDEXPOS:
		this->errorString = "Invalid SubIndex position";
		break;
	case OCFM_ERR_INVALID_ATTRIBUTETYPE:
		this->errorString = "Invalid Attribute type";
		break;
	case OCFM_ERR_NO_NODES_FOUND:
		this->errorString = "There are no nodes configured. Node collection is NULL.";
		break;
	case OCFM_ERR_NO_INDEX_FOUND:
		this->errorString = "No Indexes found";
		break;
	case OCFM_ERR_NO_SUBINDEXS_FOUND:
		this->errorString = "No SubIndexes found";
		break;
	case OCFM_ERR_NODEID_NOT_FOUND:
		this->errorString = "Node ID not found";
		break;
	case OCFM_ERR_INDEXID_NOT_FOUND:
		this->errorString = "Index ID not found";
		break;
	case OCFM_ERR_SUBINDEXID_NOT_FOUND:
		this->errorString = "SubIndex ID not found";
		break;
	case OCFM_ERR_NODE_ALREADY_EXISTS:
		this->errorString = "Node Already Exists";
		break;
	case OCFM_ERR_INDEX_ALREADY_EXISTS:
		this->errorString = "Index Already Exists";
		break;
	case OCFM_ERR_SUBINDEX_ALREADY_EXISTS:
		this->errorString = "SubIndex Already Exists";
		break;
	case OCFM_ERR_INVALID_VALUE:
		this->errorString = "Invalid value";
		break;
	case OCFM_ERR_INVALID_NAME:
		this->errorString = "Invalid name";
		break;
	case OCFM_ERR_XML_FILE_CORRUPTED:
		this->errorString = "XML file corrupted";
		break;
	case OCFM_ERR_CANNOT_OPEN_FILE:
		this->errorString = "Cannot open file";
		break;
	case OCFM_ERR_PARSE_XML:
		this->errorString = "Cannot parse XML";
		break;
	case OCFM_ERR_MODULE_INDEX_NOT_FOUND:
		this->errorString = "PDO Mapped Module Index Not Found";
		break;
	case OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND:
		this->errorString = "PDO Mapped Module SubIndex Not Found";
		break;
	case OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND:
		this->errorString = "Unique ID reference not found";
		break;
	case OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND:
		this->errorString = "Struct Datatype not found";
		break;
	case OCFM_ERR_NO_CN_NODES_FOUND:
		this->errorString = "NO CN Nodes are found";
		break;
	case OCFM_ERR_UNKNOWN:
		this->errorString = "Unknown error";
		break;
	case OCFM_ERR_VALUE_NOT_WITHIN_RANGE:
		this->errorString = "Value out of range";
		break;
	case OCFM_ERR_MN_NODE_DOESNT_EXIST:
		this->errorString = "MN Node doesnt exist";
		break;
	case OCFM_ERR_CREATE_XML_WRITER_FAILED:
		this->errorString = "Error creating the xml writer";
		break;
	case OCFM_ERR_XML_WRITER_START_ELT_FAILED:
		this->errorString = "Error at xmlTextWriterStartElement";
		break;
	case OCFM_ERR_XML_WRITER_END_ELT_FAILED:
		this->errorString = "Error at xmlTextWriterEndElement";
		break;
	case OCFM_ERR_XML_START_DOC_FAILED:
		this->errorString = "Error at xmlTextWriterStartDocument";
		break;
	case OCFM_ERR_XML_END_DOC_FAILED:
		this->errorString = "Error at xmlTextWriterEndDocument";
		break;
	case OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH:
		this->errorString = "Cannot open project: Tool-Project version mismatch";
		break;
	case OCFM_ERR_INVALID_PJTXML:
		this->errorString = "Cannot open project: Invalid Project XML";
		break;
	case OCFM_ERR_PROJECT_SETTINGS:
		this->errorString = "Project Settings are NULL";
		break;
	case OCFM_ERR_INVALID_DATATYPE_FOR_PDO:
		this->errorString = "Invalid Datatype for Mapped Object";
		break;
	case OCFM_ERR_XAP_FILE_NOT_WRITTEN:
		this->errorString = "XAP File Not Written";
		break;
	case OCFM_ERR_MAX_PI_SIZE:
	{
		ostringstream buffer;
		buffer << "MAX PI Size (" << MAX_PI_SIZE << " bytes) exceeded";
		this->errorString = buffer.str();
		break;
	}
	case OCFM_ERR_INVALID_UPPERLOWER_LIMITS:
		this->errorString = "Invalid upper and lower limits";
		break;
	case OCFM_ERR_LOW_CNPRESTIMEOUT:
		this->errorString = "Value is less than minimum value of CN PresTimeout";
		break;
	case OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN:
		this->errorString = "CN Exceeds Cross Traffic Station";
		break;
	case OCFM_ERR_EXCESS_CHANNEL:
		this->errorString = "The number of Channels has exceeded than the defined value for the MN";
		break;
	case OCFM_ERR_INVALID_TXT_FOR_CDC:
		this->errorString = "The text file as the input for cdc, is incorrectly formatted";
		break;
	case OCFM_ERR_MEMORY_ALLOCATION_ERROR:
		this->errorString = "Memory allocation failed. Try restarting the application";
		break;
	case OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS:
		this->errorString = "Number of TPDO channels exceeded";
		break;
	case OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND:
		this->errorString = "NoOfEntries SubIndex not present";
		break;
	case OCFM_ERR_INVALID_PARAMETER:
		this->errorString = "Invalid parameter: Internal Error. Try restarting the Application";
		break;
	case OCFM_ERR_INVALID_SIZE_MAPPED:
		this->errorString = "Invalid Size mapped for the PDO. Check the length for the according datatype";
		break;
	case OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO:
		this->errorString = "Invalid mapping type for the PDO. Check the pdo mapping for the mapped object";
		break;
	case OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PDO:
		this->errorString = "Invalid access type for the PDO. Check the access type for the mapped object";
		break;
	case OCFM_ERR_INVALID_PDO_OFFSET:
		this->errorString = "Invalid offset in the PDO object. Check the offset value in the mapping";
		break;
	case COMPATIBILITY_INFO_PRE_130_PDOMAPPING:
		this->errorString = "It seems you have opened the project created with pre 1.3.0 version.\nErrors with respect to 'PDOMapping' and 'AccessType' can be expected during build process.";
		break;
	case OCFM_ERR_SCHEMA_VALIDATION_FAILED:
		this->errorString = "XDD schema validation failed";
		break;
	case OCFM_ERR_XDD_SCHEMA_NOT_FOUND:
		this->errorString = "XDD schema file not found";
		break;
	case OCFM_ERR_XDD_SCHEMA_NOT_VALID:
		this->errorString = "XDD schema itself is not valid";
		break;
	case OCFM_ERR_XDD_SCHEMA_PARSER_CONTEXT_ERROR:
		this->errorString = "XDD schema parser context creation failed";
		break;
	case OCFM_ERR_XDD_SCHEMA_VALIDATION_CONTEXT_ERROR:
		this->errorString = "XDD schema validation context creation failed";
		break;
	case OCFM_ERR_XML_PARSING_ERROR:
		this->errorString = "XDD parsing failed due to wrong XML syntax";
		break;
	case OCFM_ERR_CANNOT_CREATE_DIR:
		this->errorString = "Can not create project directory";
		break;
	case OCFM_ERR_NO_PROJECT_LOADED:
		this->errorString = "No project loaded in the library";
		break;	
	case OCFM_ERR_CHAINED_AND_MULTIPLEXED:
		this->errorString = "A node can not be chained and multiplexed at the same time";
		break;
	default:
		this->errorString = "No error string specified";
		break;
	}
}

ConfiguratorError ocfmRetCode::getErrorCode(void) const
{
	return this->code;
}

void ocfmRetCode::setErrorString(const string& errorString)
{
	this->errorString = errorString;
}

const string& ocfmRetCode::getErrorString(void) const
{
	return this->errorString;
}



