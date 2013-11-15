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
#include "../Include/Exception.h"

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
			const std::string kMsgSubIndexInvalid = "Subindex %#x/%#x invalid for node %d.";
			const std::string kMsgIndexInvalid = "Index %#x invalid for node %d.";
			const std::string kMsgUnsupportedAttributeType = "Attributetype '%d' not supported for this operation.";
			const std::string kMsgNonExistingIndex = "Index %#x does not exist on node %d.";
			const std::string kMsgNonExistingSubIndex = "SubIndex %#x/%#x does not exist on node %d.";
			const std::string kMsgNonExistingNode = "Node with id %d does not exist.";
			const std::string kMsgExistingNode = "Node with id %d already exists.";
			const std::string kMsgExistingIndex = "Index %#x already exists on node %d.";
			const std::string kMsgExistingSubIndex = "SubIndex %#x/%#x already exists on node %d.";
			const std::string kMsgNoNodesConfigured = "Configuration does not contain any POWERLINK-Nodes.";
			const std::string kMsgNoControlledNodesConfigured = "Configuration does not contain any POWERLINK-CNs.";
			const std::string kMsgNoManagingNodeConfigured = "Configuration does not contain any POWERLINK-MN.";
			// TODO: Refactor name
			const std::string kMsgObjectDictoryEmpty = "Object dictionary of node %d is empty.";
			const std::string kMsgIndexContainsNoSubIndices = "Index %#x of node %d contains no subIndices.";
			const std::string kMsgNullArgument = "Argument(s) %s must not be NULL.";
			const std::string kMsgAttributeValueInvalid = "Value '%s' invalid for attribute type '%d' of index %#x/%#x, node %d. Expected value: '%s'.";
			const std::string kMsgAttributeValueNotInRange = "Value '%d' invalid for attribute type '%d' of index %#x/%#x, node %d. Expected value in range: [%d-%d].";
			const std::string kMsgExternalSystemCallFailed = "External system call '%s' failed with returncode %d";
			const std::string kMsgTPDOChannelCountExceeded = "No. of TPDO-Channels on node %d exceeded. Allowed: %d, existing: %d.";
			const std::string kMsgNonExistingMappedIndex = "Mapping-Object %#x/%#x: Mapped index %#x does not exist on node %d.";
			const std::string kMsgNonExistingMappedSubIndex = "Mapping-Object %#x/%#x: Mapped subIndex %#x/%#x does not exist on node %d.";
			const std::string kMsgInsufficientMappingObjects = "Insufficient number of mapping objects within index %#x on node %d. Expected: %d, existing %d";

			enum ErrorCode 
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
				INSUFFICIENT_MAPPING_OBJECTS

			};		

			class Result
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