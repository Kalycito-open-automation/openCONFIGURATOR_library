/************************************************************************
\file	Result.h
\brief	Class to provide the result of a function execution.

		This class is meant to be used as the return type of a function, if the function
		needs to return error codes and error context information (like an error message).
		Furthermore, this class shall be mandatory as return type for all API-Functions that 
		possibly return error codes.<br/>
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	31.10.2013
************************************************************************/

#ifndef RESULT_H_
#define RESULT_H_

#include <string>
#include "Exception.h"
#include "Exports.h"

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace ErrorHandling
		{

			const std::string kMsgFileWriteFailed = "Failed to write file '%s'.";
			const std::string kMsgFileReadFailed = "Failed to read file '%s'.";
			const std::string kMsgNodeIdInvalid = "NodeId '%d' invalid.";
			const std::string kMsgLegacyError = "Error '%d'.";
			const std::string kMsgSubIndexInvalid = "Subindex 0x%X/0x%X invalid for node %d.";
			const std::string kMsgIndexInvalid = "Index 0x%X invalid for node %d.";
			const std::string kMsgUnsupportedAttributeType = "Attributetype '%d' not supported for this operation.";
			const std::string kMsgNonExistingIndex = "Index 0x%X does not exist on node %d.";
			const std::string kMsgNonExistingSubIndex = "SubIndex 0x%X/0x%X does not exist on node %d.";
			const std::string kMsgNonExistingNode = "Node with id %d does not exist.";
			const std::string kMsgExistingNode = "Node with id %d already exists.";
			const std::string kMsgExistingIndex = "Index 0x%X already exists on node %d.";
			const std::string kMsgExistingSubIndex = "SubIndex 0x%X/0x%X already exists on node %d.";
			const std::string kMsgNoNodesConfigured = "Configuration does not contain any POWERLINK-Nodes.";
			const std::string kMsgNoControlledNodesConfigured = "Configuration does not contain any POWERLINK-CNs.";
			const std::string kMsgNoManagingNodeConfigured = "Configuration does not contain any POWERLINK-MN.";
			// TODO: Refactor name
			const std::string kMsgObjectDictoryEmpty = "Object dictionary of node %d is empty.";
			const std::string kMsgIndexContainsNoSubIndices = "Index 0x%X of node %d contains no subIndices.";
			const std::string kMsgNullArgument = "Argument(s) %s must not be NULL.";
			const std::string kMsgAttributeValueInvalid = "Value '%s' invalid for attribute type '%d' of index 0x%X/0x%X, node %d. Expected value: '%s'.";
			const std::string kMsgAttributeValueNotInRange = "Value '%d' invalid for attribute type '%d' of index 0x%X/0x%X, node %d. Expected value in range: [%d-%d].";
			const std::string kMsgExternalSystemCallFailed = "External system call '%s' failed with returncode %d";
			const std::string kMsgTPDOChannelCountExceeded = "No. of TPDO-Channels on node %d exceeded. Allowed: %d, existing: %d.";
			const std::string kMsgNonExistingMappedIndex = "Mapping-Object 0x%X/0x%X: Mapped index 0x%X does not exist on node %d.";
			const std::string kMsgNonExistingMappedSubIndex = "Mapping-Object 0x%X/0x%X: Mapped subIndex 0x%X/0x%X does not exist on node %d.";
			const std::string kMsgInsufficientMappingObjects = "Insufficient number of mapping objects within index 0x%X on node %d. Expected: %d, existing %d";
			const std::string kMsgParameterNotFound = "Parameter with uniqueID '%s' not found in node %d referenced by (Sub)Index 0x%X/0x%s.";
			const std::string kMsgStructDatatypeNotFound = "Struct with uniqueID '%s' referenced by Parameter with uniqueID '%s' in node %d not found.";
			const std::string kMsgSimpleDatatypeNotFound = "Simple datatype '%s' referenced by (Sub)Index 0x%X/0x%s in node %d not found in DatatypeList of node.";
			const std::string kMsgValueNotWithinRange = "Value %d of object '%s' on node %d out of range (%d - %d).";
			const std::string kMsgChannelPayloadLimitExceeded = "Node %d, channel 0x%X: No. of bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL)";
			const std::string kMsgChannelObjectLimitExceeded = "Node %d, channel 0x%X: No. of channel objects (%d) exceeds limit (254).";
			const std::string kMsgIsochronousMaxPayloadExceeded = "Node %d: No. of %s-bytes mapped (%d) exceeds limit (1490, C_DLL_ISOCHR_MAX_PAYL).";
			const std::string kMsgPdoDatatypeInvalid = "Node %d, (Sub)Index 0x%X/0x%s: Mapping of datatype '%s' not supported. Supported data types: Integer8(16/32/64), Unsigned8(16/32/64).";
			const std::string kMsgMaxPiSizeExceeded = "Max. process image size exceeded: Actual: %d bytes, expected: max. %d bytes.";
			const std::string kMsgMultiplexingNotSupported = "Multiplexing is not supported by the managing node.";
			const std::string kMsgUnsupportedPiLanguage = "Process-Image generation for language %d not supported.";
			const std::string kMsgMultiplexCycleAssignInvalid = "Multiplexed cycle '%d' assigned to node %d exceeds multiplexed cycle length (%d).";
			const std::string kMsgObjectLimitsInvalid = "Invalid object limits (high limit '%d' < low limit '%d')";
			const std::string kMsgNodeIndexDescription = "Node %d, Index 0x%X: ";
			const std::string kMsgNodeSubIndexDescription = "Node %d, (Sub)Index 0x%X/0x%X: ";
			const std::string kMsgLowCnPresTimeout = "PResTimeout (%dns) (subIndex 0x1F92/0x%X on MN) of node %d is less than PresMaxLatency (subIndex 0x1F98/0x03 on CN).";
			const std::string kMsgCrossTrafficStationLimitExceeded = "Node %d: No. of cross traffic stations (%d) exceeds limit (%d).";
			const std::string kMsgPDOTPDOChannelCountExceeded = "Node 240: No. of TPDO-Channels exceeded. Allowed: %d (MN-Feature D_PDO_TPDOChannels_U16), existing: %d.";
			const std::string kMsgNoActualOrDefaultValue = "Object 0x%X/0x%X on node %d has neither a default value nor an actual value.";
			const std::string kMsgPowerlinkDatatypeSizeNotDefined = "POWERLINK-Datatype '0x%X' has no defined size.";
			const std::string kMsgFeatureValueNotFound = "%s-Feature '%s' not defined in XDD of node %d.";
			const std::string kMsgObjectSizeMappedInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapped data-length invalid. Actual: %d bits, expected %d bits.";
			const std::string kMsgMappingTypeForPdoInvalid = "Mapping-Object 0x%X/0x%X: (Sub)Index 0x%X/0x%s on node %d cannot be mapped. Mismatching PDOMapping ('%s' Mapping-Object, mapped object '%s').";
			const std::string kMsgAccessTypeForParameterInvalid = "Mapping-Object 0x%X/0x%X: Parameter with uniqueId '%s', referenced by mapped index 0x%X/0x%s on node %d, has invalid access type ('%s').";
			const std::string kMsgAccessTypeForPdoInvalid = "Mapping-Object 0x%X/0x%X: (Sub)Index 0x%X/0x%s on node %d cannot be mapped. Invalid access type '%s' (RPDOs must be writeable, TPDOs readable).";
			const std::string kMsgPdoOffsetInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapping offset invalid. Actual: %d bits, expected %d bits.";			
			const std::string kMsgSchemaNotFoundOrWellFormed = "XML schema file '%s' cannot be loaded or is not well-formed.";
			const std::string kMsgSchemaInvalid = "XML schema file '%s' is not valid.";
			const std::string kMsgSchemaParserContextError = "LibXML2 cannot create a parser context for the XML schema file '%s'.";
			const std::string kMsgSchemaValidationContextError = "LibXML2 cannot create a validation context for the XML schema file '%s'.";

			DLLEXPORT enum ErrorCode 
			{ 
				SUCCESS = 0,
				FILE_WRITE_FAILED,
				FILE_READ_FAILED,
				LEGACY_ERROR,
				SUBINDEX_INVALID,
				INDEX_INVALID,
				NODEID_INVALID,
				ATTRIBUTEVALUE_INVALID,
				ATTRIBUTEVALUE_NOT_IN_RANGE,
				UNSUPPORTED_ATTRIBUTETYPE,
				NO_NODES_CONFIGURED,
				NO_CONTROLLED_NODES_CONFIGURED,
				NO_MANAGING_NODE_CONFIGURED,
				OD_EMPTY,
				INDEX_CONTAINS_NO_SUBINDICES,
				NODE_DOES_NOT_EXIST,
				INDEX_DOES_NOT_EXIST,
				SUBINDEX_DOES_NOT_EXIST,
				NODE_EXISTS,
				INDEX_EXISTS,
				SUBINDEX_EXISTS,
				EXTERNAL_SYSTEM_CALL_FAILED,
				TPDO_CHANNEL_COUNT_EXCEEDED,
				NODE_CONFIGURATION_ERROR,
				MAPPED_INDEX_DOES_NOT_EXIST,
				MAPPED_SUBINDEX_DOES_NOT_EXIST,
				INSUFFICIENT_MAPPING_OBJECTS,
				PARAMETER_NOT_FOUND,
				STRUCT_DATATYPE_NOT_FOUND,
				SIMPLE_DATATYPE_NOT_FOUND,
				VALUE_NOT_WITHIN_RANGE,
				CHANNEL_PAYLOAD_LIMIT_EXCEEDED,
				CHANNEL_OBJECT_LIMIT_EXCEEDED,
				PDO_DATATYPE_INVALID,
				UNSUPPORTED_PI_GEN_LANGUAGE,
				MAX_PI_SIZE_EXCEEDED,
				MULTIPLEXING_NOT_SUPPORTED,
				MULTIPLEX_CYCLE_ASSIGN_INVALID,
				OBJECT_LIMITS_INVALID,
				LOW_CN_PRES_TIMEOUT,
				CROSS_TRAFFIC_STATION_LIMIT_EXCEEDED,
				PARAMETER_INVALID,
				UNHANDLED_EXCEPTION,
				NO_DEFAULT_OR_ACTUAL_VALUE,
				FEATURE_VALUE_NOT_FOUND,
				PLKDATATYPE_SIZE_UNDEFINED,
				OBJECT_SIZE_MAPPED_INVALID,
				MAPPING_TYPE_FOR_PDO_INVALID,
				ACCESS_TYPE_FOR_PARAMETER_INVALID,
				ACCESS_TYPE_FOR_PDO_INVALID,
				PDO_OFFSET_INVALID,
				XML_INVALID,
				SCHEMA_NOT_FOUND_OR_WELL_FORMED,
				SCHEMA_INVALID,
				SCHEMA_PARSER_CONTEXT_ERROR,
				SCHEMA_VALIDATION_CONTEXT_ERROR
				
			};		

			class DLLEXPORT Result
			{

			private:
				ErrorCode errorCode;
				std::string errorString;

			public:
				Result(ErrorCode errorCode, std::string errorString);
				explicit Result(ErrorCode errorCode = SUCCESS);

				ErrorCode GetErrorCode() const;
				const std::string& GetErrorString() const;
				bool IsSuccessful() const;			

			};// Result

			Result Translate(const ocfmRetCode& legacyError);

		}// ErrorHandling
	}// Library
}// openCONFIGURATOR

#endif