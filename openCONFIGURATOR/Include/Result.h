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
			const std::string kMsgNoProjectLoaded = "No openCONFIGURATOR project loaded in the library.";
			const std::string kMsgEmptyArgument = "Argument(s) '%s' must not be empty.";
			const std::string kMsgParameterValueInvalid = "Node %d: For parameter '%d' the given value '%s' is invalid.";
			const std::string kMsgParameterValueNotSet = "Node %d: Parameter '%d' value is not set.";
			const std::string kMsgMappingInvalid = "Mapping-Object 0x%X/0x%X on node %d: Mapping is enabled (NumberOfEntries != 0) but mapping entry is zero. Expected valid mapping entry.";
			const std::string kMsgPathExists = "The Path '%s' already exists in the openCONFIGUGRATOR project.";
			const std::string kMsgPathDoesNotExist = "The Path '%s' does not exist in the openCONFIGUGRATOR project.";
			const std::string kMsgViewSettingExists = "The ViewSetting '%s' with Type '%s' already exists in the openCONFIGUGRATOR project.";
			const std::string kMsgViewSettingDoesNotExist = "The ViewSetting '%s' with Type '%s' does not exist in the openCONFIGUGRATOR project.";
			const std::string kMsgViewSettingsDoesNotExist = "The ViewSettings with Type '%s' does not exists in the openCONFIGUGRATOR project.";
			const std::string kMsgAutoGenSettingExists = "The AutoGeneration Setting '%s' within Group ID '%s' and already exists in the openCONFIGUGRATOR project.";
			const std::string kMsgAutoGenSettingDoesNotExist = "The AutoGeneration Setting '%s' within Group ID '%s' does not exist in the openCONFIGUGRATOR project.";
			const std::string kMsgAutoGenSettingsDoesNotExist = "The AutoGenerationSettings with ID '%s' does not exists in the openCONFIGUGRATOR project.";


			/** \addtogroup apireturn
			 * @{
			 */
			/**
			\brief Error codes returned by the %openCONFIGURATOR API-Functions within Result.
			*/
			DLLEXPORT enum ErrorCode
			{
				SUCCESS = 0, /**< Operation completed successfully. */
				FILE_WRITE_FAILED, /**< Cannot write to file. */
				FILE_READ_FAILED, /**< Cannot read from file. */
				LEGACY_ERROR, /**< Deprecated error occured in operation.*/
				SUBINDEX_INVALID, /**< Invalid subindex encountered during operation. */
				INDEX_INVALID, /**< Invalid index encountered during operation. */
				NODEID_INVALID, /**< Invalid node id encountered during operation. */
				ATTRIBUTEVALUE_INVALID, /**< Invalid attribute value encountered during operation. */
				ATTRIBUTEVALUE_NOT_IN_RANGE, /**< Attribute value not in valid range. */
				UNSUPPORTED_ATTRIBUTETYPE, /**< Attribute type not supported by operation. */
				NO_NODES_CONFIGURED, /**< Network does not contain any nodes. */
				NO_CONTROLLED_NODES_CONFIGURED, /**< Network does not contain any controlled nodes. */
				NO_MANAGING_NODE_CONFIGURED, /**< Network does not contain a managing node. */
				OD_EMPTY, /**< Object dictionary is empty. */
				INDEX_CONTAINS_NO_SUBINDICES, /**< %Index does not contain subindices. */
				NODE_DOES_NOT_EXIST, /**< %Node does not exist in the network. */
				INDEX_DOES_NOT_EXIST, /**< %Index does not exist on %Node. */
				SUBINDEX_DOES_NOT_EXIST, /**< %SubIndex does not exist in %Index.  */
				NODE_EXISTS, /**< %Node already exists in the network. */
				INDEX_EXISTS, /**< %Index already exists on a %Node. */
				SUBINDEX_EXISTS, /**< %SubIndex already exists for an %Index.  */
				EXTERNAL_SYSTEM_CALL_FAILED, /**< External system call failed during operation. */
				TPDO_CHANNEL_COUNT_EXCEEDED, /**< Max. no. of TPDO-Channels exceeded. */
				NODE_CONFIGURATION_ERROR, /**< %Node configuration error occured. */
				MAPPED_INDEX_DOES_NOT_EXIST, /**< A mapped %Index does not exist on %Node. */
				MAPPED_SUBINDEX_DOES_NOT_EXIST, /**< A mapped %SubIndex does not exist on %Node. */
				INSUFFICIENT_MAPPING_OBJECTS, /**< Insufficient mapping objects on the %Node. */
				PARAMETER_NOT_FOUND, /**< %Parameter not found on a %Node. */
				STRUCT_DATATYPE_NOT_FOUND, /**< Struct datatype not found on a %Node. */
				SIMPLE_DATATYPE_NOT_FOUND, /**< Simple datatype not found on the %Node. */
				VALUE_NOT_WITHIN_RANGE, /**< Value not within a valid range. */
				CHANNEL_PAYLOAD_LIMIT_EXCEEDED, /**< Max. Channel-Payload exceeded. */
				CHANNEL_OBJECT_LIMIT_EXCEEDED, /**< Max. no. of Channel-Objects exceeded.  */
				PDO_DATATYPE_INVALID, /**< PDO datatype invalid. */
				UNSUPPORTED_PI_GEN_LANGUAGE, /**< Process image generation not supported for given programming language. */
				MAX_PI_SIZE_EXCEEDED, /**< Maximum size of process image exceeded. */
				MULTIPLEXING_NOT_SUPPORTED, /**< Multiplexing not supported. */
				MULTIPLEX_CYCLE_ASSIGN_INVALID, /**< Invalid multiplexed cycle assigned to %Node. */
				OBJECT_LIMITS_INVALID, /**< High-/Lowlimit of the object are invalid. */
				LOW_CN_PRES_TIMEOUT, /**< CNPResTimeout (0x1F92) is too low. */
				CROSS_TRAFFIC_STATION_LIMIT_EXCEEDED, /**< CN exceeds the no. of allowed cross-traffic stations (device description entry D_PDO_RPDOChannels_U16). */
				ARGUMENT_INVALID_NULL, /**< Invalid function-argument (NULL). */
				ARGUMENT_INVALID_EMPTY, /**< Invalid function-argument (empty). */
				UNHANDLED_EXCEPTION, /**< Unhandled exception occurred during operation. */
				NO_DEFAULT_OR_ACTUAL_VALUE, /**< No default or actual value defined for object. */
				FEATURE_VALUE_NOT_FOUND, /**< CN-/MN- or GeneralFeature not found in a node's XDD. */
				PLKDATATYPE_SIZE_UNDEFINED, /** The given POWERLINK data type has no defined size. */
				OBJECT_SIZE_MAPPED_INVALID, /**< Mapped object size invalid. */
				MAPPING_TYPE_FOR_PDO_INVALID, /**< Mapped object has an invalid/incompatible PDOmapping. */
				ACCESS_TYPE_FOR_PARAMETER_INVALID, /**< A mapped object's referenced %Parameter has an invalid/incompatible accessType. */
				ACCESS_TYPE_FOR_PDO_INVALID, /**< A mapped object has an invalid/incompatible accessType. */
				PDO_OFFSET_INVALID, /**< A mapped object has an invalid offset (non-contigous).*/
				XML_INVALID, /**< Processed XML file not schema-valid. */
				SCHEMA_NOT_FOUND_OR_WELL_FORMED, /**< XML schema not found or not well-formed. */
				SCHEMA_INVALID, /**< XML schema not valid. */
				SCHEMA_PARSER_CONTEXT_ERROR, /**< XML parser context error. */
				SCHEMA_VALIDATION_CONTEXT_ERROR, /**< XML schema validation context error. */
				NO_PROJECT_LOADED, /**< No project loaded. */
				MAPPING_INVALID, /**< An enabled mapping entry is empty. */
				PARAMETER_VALUE_NOT_SET, /**< A specific parameter value is not set. */
				PARAMETER_VALUE_INVALID, /**< A value is invalid for a specific parameter. */
				PATH_EXISTS, /**< A path already exists in the project configuration. */
				PATH_DOES_NOT_EXIST, /**< A path does not exists in the project configuration. */
				VIEW_SETTING_EXISTS, /**< A view setting exists in the project configuration. */
				VIEW_SETTING_DOES_NOT_EXIST, /**< A view setting does not exist in the project configuration. */
				VIEW_SETTINGS_DOES_NOT_EXIST, /**< A view settings group does not exist in the project configuration. */
				AUTO_GEN_SETTING_EXISTS, /**< An auto generation setting already exists in the project configuration. */
				AUTO_GEN_SETTING_DOES_NOT_EXIST, /**< An auto generation setting does not exist in the project configuration. */
				AUTO_GEN_SETTINGS_DOES_NOT_EXIST /**< An auto generation settings group does not exist in the project configuration. */
			};

			/**
			\brief	Result returned by all functions of the %openCONFIGURATOR API.

					Contains an ErrorCode and a descriptive error string.
			*/
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

					/**
					\return <code>True</code> if operation was successful, <code>false</code> otherwise.
					*/
					bool IsSuccessful() const;

			};// Result
			/** @} */

			Result Translate(const ocfmRetCode& legacyError);

		}// ErrorHandling
	}// Library
}// openCONFIGURATOR

#endif
