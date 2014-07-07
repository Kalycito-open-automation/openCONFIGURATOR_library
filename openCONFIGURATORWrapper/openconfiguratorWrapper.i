/**
********************************************************************************
\file   openconfiguratorWrapper.h

\brief  TCL wrapper interface configuration for the openCONFIGURATOR library

\author Ramakrishnan Periyakaruppan

\copyright (c) 2014, Kalycito Infotech Private Limited
					 All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holders nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

%module openConfiguratorWrapper
/* Include all the required SWIG interfaces */
%include stl.i
%include std_string.i
%include cpointer.i

%inline 
%{

enum AttributeType
{
	NAME = 0,		/**< Name attribute. */
	OBJECTTYPE,		/**< Object Type attribute. */
	DATATYPE,		/**< Data Type attribute. */
	ACCESSTYPE,		/**< Access Type attribute. */
	DEFAULTVALUE,	/**< Default value attribute. */
	ACTUALVALUE,	/**< Actual value attribute. */
	PDOMAPPING,		/**< PDO Mapping attribute. */
	LOWLIMIT,		/**< Low limit attribute. */
	HIGHLIMIT,		/**< High limit attribute. */
	FORCETOCDC		/**< Force to CDC. */
};

enum OutputLanguage
{
	C, /**< ANSI C*/
	CSHARP, /**< Microsoft C Sharp*/
	/*JAVA,*/
	XML /**< Generic XML representation (needed for ANSI C)*/
};

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace ObjectDictionary
		{
			namespace PlkDataType
			{
				enum PlkDataType
				{
					BOOLEAN = 0x0001,
					INTEGER8 = 0x0002,
					INTEGER16 = 0x0003,
					INTEGER32 = 0x0004,
					UNSIGNED8 = 0x0005,
					UNSIGNED16 = 0x0006,
					UNSIGNED32 = 0x0007,
					REAL32 = 0x0008,
					VISIBLE_STRING = 0x0009,
					OCTET_STRING = 0x000A,
					UNICODE_STRING = 0x000B,
					TIME_OF_DAY = 0x000C,
					TIME_DIFF = 0x000D,
					Domain = 0x000F,
					INTEGER24 = 0x0010,
					REAL64 = 0x0011,
					INTEGER40 = 0x0012,
					INTEGER48 = 0x0013,
					INTEGER56 = 0x0014,
					INTEGER64 = 0x0015,
					UNSIGNED24 = 0x0016,
					UNSIGNED40 = 0x0018,
					UNSIGNED48 = 0x0019,
					UNSIGNED56 = 0x001A,
					UNSIGNED64 = 0x001B,
					MAC_ADDRESS = 0x0401,
					IP_ADDRESS = 0x0402,
					NETTIME = 0x0403
				};
			}
			namespace PlkFeature
			{
				enum PlkFeature
				{
					//POWERLINK general features
					CFMConfigManager = 0,
					DLLErrBadPhysMode,
					DLLErrMacBuffer,
					DLLFeatureCN,
					DLLFeatureMN,
					NMTBootTimeNotActive,
					NMTCycleTimeGranularity,
					NMTCycleTimeMax,
					NMTCycleTimeMin,
					NMTMinRedCycleTime,
					NMTEmergencyQueueSize,
					NMTErrorEntries,
					NMTFlushArpEntry,
					NMTNetHostNameSet,
					NMTMaxCNNodeID,
					NMTMaxCNNumber,
					NMTMaxHeartbeats,
					NMTNodeIDByHW,
					NMTProductCode,
					NMTPublishActiveNodes,
					NMTPublishConfigNodes,
					NMTPublishEmergencyNew,
					NMTPublishNodeState,
					NMTPublishOperational,
					NMTPublishPreOp1,
					NMTPublishPreOp2,
					NMTPublishReadyToOp,
					NMTPublishStopped,
					NMTPublishTime,
					NMTRevisionNo,
					NWLForward,
					NWLICMPSupport,
					NWLIPSupport,
					PDOGranularity,
					PDOMaxDescrMem,
					PDORPDOChannelObjects,
					PDORPDOChannels,
					PDORPDOCycleDataLim,
					PDORPDOOverallObjects,
					PDOSelfReceipt,
					PDOTPDOChannelObjects,
					PDOTPDOCycleDataLim,
					PDOTPDOOverallObjects,
					PHYExtEPLPorts,
					PHYHubDelay,
					PHYHubIntegrated,
					PHYHubJitter,
					RT1RT1SecuritySupport,
					RT1RT1Support,
					RT2RT2Support,
					SDOClient,
					SDOCmdFileRead,
					SDOCmdFileWrite,
					SDOCmdLinkName,
					SDOCmdReadAllByIndex,
					SDOCmdReadByName,
					SDOCmdReadMultParam,
					SDOCmdWriteAllByIndex,
					SDOCmdWriteByName,
					SDOCmdWriteMultParam,
					SDOMaxConnections,
					SDOMaxParallelConnections,
					SDOSeqLayerTxHistorySize,
					SDOServer,
					//POWERLINK MN features >= 64
					DLLErrMNMultipleMN,
					DLLMNFeatureMultiplex,
					DLLMNPResChaining,
					DLLMNFeaturePResTx,
					NMTMNASnd2SoC,
					NMTMNBasicEthernet,
					NMTMNMultiplCycMax,
					NMTMNPRes2PReq,
					NMTMNPRes2PRes,
					NMTMNPResRx2SoA,
					NMTMNPResTx2SoA,
					NMTMNSoA2ASndTx,
					NMTMNSoC2PReq,
					NMTNetTime,
					NMTNetTimeIsRealTime,
					NMTRelativeTime,
					NMTSimpleBoot,
					PDOTPDOChannels,
					//POWERLINK CN features >= 82
					DLLCNFeatureMultiplex,
					DLLCNPResChaining,
					NMTCNSoC2PReq
				};
				
			}
		}
		namespace NodeParameter
		{
			enum NodeParameter
			{
				NODENAME = 0, /**< Name of the node */
				NODEID, /**< Id of the node */
				STATIONTYPE, /**< Station type of the node (Normal, Chained or Multiplexed)*/
				FORCEDMULTIPLEXEDCYCLE, /**< Force a destinct cycle for a multiplexed node*/
				TRANSMITSPRES, 
				ASYNCSLOTTIMEOUT, 
				ASNDMAXNUMBER,
				ISASYNCONLY, 
				ISTYPE1ROUTER,
				ISTYPE2ROUTER,
				ISMANDATORY,
				AUTOSTARTNODE, 
				RESETINOPERATIONAL,
				VERIFYAPPSWVERSION,
				AUTOAPPSWUPDATEALLOWED,
				VERIFYDEVICETYPE,
				VERIFYVENDORID,
				VERIFYREVISIONNUMBER,
				VERIFYPRODUCTCODE,
				VERIFYSERIALNUMBER
			};
		}
		namespace ProjectFile
		{
			namespace ViewType
			{
				enum ViewType
				{
					BASIC = 0, /**< Basic View Mode */
					ADVANCED /**< Advanced View Mode */
				};
			}

		}
		namespace ErrorHandling
		{
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
				ARGUMENT_INVALID_NULL,
				ARGUMENT_INVALID_EMPTY,
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
				SCHEMA_VALIDATION_CONTEXT_ERROR,
				NO_PROJECT_LOADED,
				MAPPING_INVALID,
				PARAMETER_VALUE_NOT_SET,
				PARAMETER_VALUE_INVALID,
				PATH_EXISTS, 
				PATH_DOES_NOT_EXIST,
				VIEW_SETTING_EXISTS,
				VIEW_SETTING_DOES_NOT_EXIST,
				VIEW_SETTINGS_DOES_NOT_EXIST,
				AUTO_GEN_SETTING_EXISTS,
				AUTO_GEN_SETTING_DOES_NOT_EXIST,
				AUTO_GEN_SETTINGS_DOES_NOT_EXIST
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

		}// ErrorHandling

		namespace API
		{
			/* NodeApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const unsigned int, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result DeleteNode(const unsigned int);
			openCONFIGURATOR::Library::ErrorHandling::Result ReplaceXdd(const unsigned int, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result IsExistingNode(const unsigned int, bool&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetNodeCount(unsigned int&);
			openCONFIGURATOR::Library::ErrorHandling::Result SetNodeParameter(const unsigned int, const openCONFIGURATOR::Library::NodeParameter::NodeParameter, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetNodeParameter(const unsigned int, const openCONFIGURATOR::Library::NodeParameter::NodeParameter, std::string& );


			/* XddApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result GetDataTypeSize(const openCONFIGURATOR::Library::ObjectDictionary::PlkDataType::PlkDataType, unsigned int&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetFeatureValue(const unsigned int, const openCONFIGURATOR::Library::ObjectDictionary::PlkFeature::PlkFeature, std::string&);


			/* ProjectApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result NewProject(const std::string, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result SaveProject();
			openCONFIGURATOR::Library::ErrorHandling::Result CloseProject();
			openCONFIGURATOR::Library::ErrorHandling::Result OpenProject(const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result AddPath(const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetPath(const std::string, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result DeletePath(const std::string );
			openCONFIGURATOR::Library::ErrorHandling::Result SetActiveAutoCalculationConfig(const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetActiveAutoCalculationConfig(std::string&);

			/* ProjectFileApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result AddViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType, const std::string, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result DeleteViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result SetActiveView(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType);
			openCONFIGURATOR::Library::ErrorHandling::Result GetActiveView(openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType&);

			/* ImportExportApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result GenerateProcessImageDescription(const OutputLanguage, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GenerateStackConfiguration(const std::string, const std::string);

			/* OdApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result SetIndexActualValue(const unsigned int, const unsigned int, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexActualValue(const unsigned int, const unsigned int, const unsigned int, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetIndexAttribute(const unsigned int, const unsigned int, AttributeType, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexAttribute(const unsigned int, const unsigned int, const unsigned int, AttributeType, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result IsExistingIndex(const unsigned int, const unsigned int, bool& exists);
			openCONFIGURATOR::Library::ErrorHandling::Result IsExistingSubIndex(const unsigned int, const unsigned int, const unsigned int, bool&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetIndexCount(const unsigned int, unsigned int&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexCount(const unsigned int, const unsigned int, unsigned int&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetNumberOfEntries(const unsigned int, const unsigned int, const bool, unsigned int&);
		}
	}
}
%}


%pointer_functions(openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType, ViewTypep)
%pointer_functions(std::string, stringpointer)
%pointer_functions(bool, boolpointer)
%pointer_functions(unsigned int, uintpointer)