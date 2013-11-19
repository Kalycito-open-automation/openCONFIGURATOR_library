/************************************************************************
\file	Result.cpp
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	31.10.2013
************************************************************************/

#include "../Include/Result.h"
#include "../Include/BoostShared.h"

using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace std;

Result::Result(ErrorCode errorCode, std::string errorString) :
	errorCode(errorCode),
	errorString(errorString)
{}

Result::Result(ErrorCode errorCode) :
	errorCode(errorCode),
	errorString()
{}

ErrorCode Result::GetErrorCode() const
{
	return this->errorCode;
}

const std::string& Result::GetErrorString() const
{
	return this->errorString;
}

bool Result::IsSuccessful() const
{
	return (this->errorCode == SUCCESS);
}

Result openCONFIGURATOR::Library::ErrorHandling::Translate(const ocfmRetCode& legacyError)
{
	ConfiguratorError errorCode = legacyError.getErrorCode();
	string errorString = legacyError.getErrorString();

	switch (errorCode)
	{
		case OCFM_ERR_SUCCESS:
			return Result();
		case OCFM_ERR_FILE_CANNOT_OPEN:
			return Result(FILE_WRITE_FAILED, errorString);
		case OCFM_ERR_INVALID_NODEID:
			return Result(NODEID_INVALID, errorString);		
		case OCFM_ERR_INVALID_SUBINDEXID:
			return Result(SUBINDEX_INVALID, errorString);
		case OCFM_ERR_INVALID_INDEXID:
			return Result(INDEX_INVALID, errorString);
		case OCFM_ERR_INVALID_ATTRIBUTETYPE:
			return Result(UNSUPPORTED_ATTRIBUTETYPE, errorString);
		case OCFM_ERR_NO_NODES_FOUND:
			return Result(NO_NODES_CONFIGURED, errorString);
		case OCFM_ERR_NO_INDEX_FOUND:
			return Result(OD_EMPTY, errorString);
		case OCFM_ERR_NO_SUBINDEXS_FOUND:
			return Result(INDEX_CONTAINS_NO_SUBINDICES, errorString);
		case OCFM_ERR_NODEID_NOT_FOUND:
			return Result(NODE_DOES_NOT_EXIST, errorString);
		case OCFM_ERR_INDEXID_NOT_FOUND:
			return Result(INDEX_DOES_NOT_EXIST, errorString);
		case OCFM_ERR_SUBINDEXID_NOT_FOUND:
			return Result(SUBINDEX_DOES_NOT_EXIST, errorString);
		case OCFM_ERR_NODE_ALREADY_EXISTS:
			return Result(NODE_EXISTS, errorString);
		case OCFM_ERR_INDEX_ALREADY_EXISTS:
			return Result(INDEX_EXISTS, errorString);
		case OCFM_ERR_SUBINDEX_ALREADY_EXISTS:
			return Result(SUBINDEX_EXISTS, errorString);
		case OCFM_ERR_INVALID_VALUE:
			return Result(ATTRIBUTEVALUE_INVALID, errorString);
		case OCFM_ERR_CANNOT_OPEN_FILE:
			return Result(FILE_READ_FAILED, errorString);
		case OCFM_ERR_NO_CN_NODES_FOUND:
			return Result(NO_CONTROLLED_NODES_CONFIGURED, errorString);
		case OCFM_ERR_MN_NODE_DOESNT_EXIST:
			return Result(NO_MANAGING_NODE_CONFIGURED, errorString);
		case OCFM_ERR_INVALID_TXT_FOR_CDC:
			return Result(EXTERNAL_SYSTEM_CALL_FAILED, errorString);
		case OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS:
			return Result(TPDO_CHANNEL_COUNT_EXCEEDED, errorString);
		case OCFM_ERR_CHAINED_AND_MULTIPLEXED:
			return Result(NODE_CONFIGURATION_ERROR, errorString);	
		case OCFM_ERR_MODULE_INDEX_NOT_FOUND:
			return Result(MAPPED_INDEX_DOES_NOT_EXIST, errorString);
		case OCFM_ERR_INSUFFICIENT_MAPPING_OBJECTS:
			return Result(INSUFFICIENT_MAPPING_OBJECTS, errorString);
		case OCFM_ERR_MODULE_SUBINDEX_NOT_FOUND:
			return Result(MAPPED_SUBINDEX_DOES_NOT_EXIST, errorString);
		case OCFM_ERR_UNIQUE_ID_REF_NOT_FOUND:
			return Result(PARAMETER_NOT_FOUND, errorString);
		case OCFM_ERR_STRUCT_DATATYPE_NOT_FOUND:
			return Result(STRUCT_DATATYPE_NOT_FOUND, errorString);
		case OCFM_ERR_DATATYPE_NOT_FOUND:
			return Result(SIMPLE_DATATYPE_NOT_FOUND, errorString);
		case OCFM_ERR_VALUE_NOT_WITHIN_RANGE:
			return Result(VALUE_NOT_WITHIN_RANGE, errorString);
		case OCFM_ERR_CHANNEL_PAYLOAD_LIMIT_EXCEEDED:
			return Result(CHANNEL_PAYLOAD_LIMIT_EXCEEDED, errorString);
		case OCFM_ERR_CHANNEL_OBJECT_LIMIT_EXCEEDED:
			return Result(CHANNEL_OBJECT_LIMIT_EXCEEDED, errorString);
		case OCFM_ERR_INVALID_DATATYPE_FOR_PDO:
			return Result(PDO_DATATYPE_INVALID, errorString);

		case OCFM_ERR_MAX_PI_SIZE:
		case OCFM_ERR_UNKNOWN:
		case OCFM_ERR_MULTIPLEX_ASSIGN_ERROR:
		case OCFM_ERR_INVALID_UPPERLOWER_LIMITS:
		case OCFM_ERR_LOW_CNPRESTIMEOUT:
		case OCFM_ERR_CN_EXCEEDS_CROSS_TRAFFIC_STN:
		case OCFM_ERR_EXCESS_CHANNEL:
		case OCFM_ERR_NUMBER_OF_ENTRIES_SUBINDEX_NOT_FOUND:
		case OCFM_ERR_INVALID_PARAMETER:
		case OCFM_ERR_INVALID_SIZE_MAPPED:
		case OCFM_ERR_INVALID_MAPPING_TYPE_FOR_PDO:
		case OCFM_ERR_INVALID_ACCESS_TYPE_FOR_PDO:
		case OCFM_ERR_INVALID_PDO_OFFSET:
		case OCFM_ERR_SCHEMA_VALIDATION_FAILED:
		case OCFM_ERR_XDD_SCHEMA_NOT_FOUND:
		case OCFM_ERR_XDD_SCHEMA_NOT_VALID:
		case OCFM_ERR_XDD_SCHEMA_PARSER_CONTEXT_ERROR:
		case OCFM_ERR_XDD_SCHEMA_VALIDATION_CONTEXT_ERROR:
		case OCFM_ERR_XML_PARSING_ERROR:
		case OCFM_ERR_NO_PROJECT_LOADED:
		case OCFM_ERR_EMPTY_PATH:
		

		// Covered by XDD/XDC XML-Schema validation
		case OCFM_ERR_XML_FILE_CORRUPTED:
		case OCFM_ERR_PARSE_XML:
		// Unused throughout library codebase
		case OCFM_ERR_FILE_NOT_PRESENT:
		case OCFM_ERR_INVALID_NAME:
		case OCFM_ERR_INVALID_NODETYPE:
		case OCFM_ERR_CANNOT_OPEN_PROJECT_VER_MISMATCH:
		case OCFM_ERR_XAP_FILE_NOT_WRITTEN:
		case COMPATIBILITY_INFO_PRE_130_PDOMAPPING:
		case OCFM_ERR_CANNOT_CREATE_DIR:
		// Will most likely never occur, unless a no-throw constructor is 
		// explicitely called (which is not the case atm).
		case OCFM_ERR_MEMORY_ALLOCATION_ERROR:
		// Legacy
		case OCFM_ERR_INVALID_NODEPOS:
		case OCFM_ERR_INVALID_INDEXPOS:
		case OCFM_ERR_INVALID_SUBINDEXPOS:
		case OCFM_ERR_CREATE_XML_WRITER_FAILED:
		case OCFM_ERR_XML_WRITER_START_ELT_FAILED:
		case OCFM_ERR_XML_WRITER_END_ELT_FAILED:
		case OCFM_ERR_XML_START_DOC_FAILED:
		case OCFM_ERR_XML_END_DOC_FAILED:
		case OCFM_ERR_INVALID_PJTXML:
		case OCFM_ERR_PROJECT_SETTINGS:
		default:
		{
			boost::format formatter(kMsgLegacyError);
			formatter % ((int) errorCode);
			return Result(LEGACY_ERROR, formatter.str() + errorString);
		}
	}// switch
}// Translate