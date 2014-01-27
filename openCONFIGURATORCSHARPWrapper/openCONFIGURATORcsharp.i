%module openCONFIGURATORcsharpWRAPPER
#define DLLEXPORT
/* Windows Platform */
#if defined(_WIN32) && defined(_MSC_VER)
	 #undef DLLEXPORT
	 #define DLLEXPORT  __declspec(dllexport)
	 
#endif

%{
	#include "../openCONFIGURATOR/Include/Exports.h"
	#include "../openCONFIGURATOR/Include/Enums.h"	
	#include "../openCONFIGURATOR/Include/Result.h"	
	#include <vector>		
%}

/* Include all the required SWIG interfaces */
%include std_except.i
%include std_string.i
%include std_vector.i
%include typemaps.i
%include std_string_ref.i

%apply unsigned int& OUTPUT { unsigned int& size };
%apply unsigned int& OUTPUT { unsigned int& nodeCount };
%apply unsigned int& OUTPUT { unsigned int& indexCount };
%apply unsigned int& OUTPUT { unsigned int& subIndexCount };
%apply unsigned int& OUTPUT { unsigned int& nrOfEntries };
%apply bool& OUTPUT { bool& exists };

%template(VectorWrapper) std::vector<unsigned int>;

%ignore MN_NODEID;
%ignore BROADCAST_NODEID;
%ignore PI_VAR_COUNT;
%ignore PRES_DEFAULT_PAYLOAD;
%ignore PREQ_DEFAULT_PAYLOAD;
%ignore OFFSET_NUM_ENTRIES_MN;
%ignore OFFSET_NUM_ENTRIES_CN;
%ignore OFFSET_INDEX;
%ignore OFFSET_SUBINDEX;
%ignore OFFSET_SIZE;
%ignore OFFSET_DATA;
%ignore INDEX_SIZE;
%ignore SUBINDEX_SIZE;
%ignore NODE_ID;
%ignore ERR_STRING_LEN;
%ignore OFFSET_FIRST_INDEX_IN_MN_OBD;
%ignore OFFSET_FIRST_INDEX_IN_1F22;
%ignore MAX_CN_CROSS_TRAFFIC_STN;
%ignore MULTIPL_CYCLE_ASSIGN_OBJECT;
%ignore MNCN_POLLRESPONSE_TIMEOUT_OBJECT;
%ignore PATH_SEPARATOR;
%ignore AutoGenerate;
%ignore AutoSave;
%ignore DateTime;
%ignore IEC_Datatype;
%ignore NodeType;
%ignore PDOMapping;
%ignore PDOType;
%ignore ParameterAccess;
%ignore ProjectSettings;
%ignore StationType;
%ignore ViewMode;
%ignore AccessType;
%ignore ConfiguratorError;
%ignore FeatureType;
%ignore GetAccessType(const std::string& accessType);
%ignore GetObjectType(const std::string& objectType);
%ignore GetPlkFeature(const int enumVal);
%ignore PlkFeatureStrings;

%include "../openCONFIGURATOR/Include/Exports.h"
%include "../openCONFIGURATOR/Include/Enums.h"	

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace ErrorHandling
		{
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
				PARAMETER_VALUE_INVALID 
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

		}// ErrorHandling
	}// Library
}

%inline
%{

namespace openCONFIGURATOR{
	namespace Library{
		namespace API{
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result NewProject(const std::string projectName, const std::string projectPath, const std::string pathToMNXdd = "");
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SaveProject(void);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result CloseProject(void);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result OpenProject(const std::string projectFile);

				/*DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const Node& node, const std::string path, const std::string xddFile);*/
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const unsigned int nodeId, const std::string nodeName, const std::string xddFile = "");
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteNode(const unsigned int nodeId);
				/*DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNode(const unsigned int nodeId, Node& node);*/
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result ReplaceXdd(const unsigned int nodeId, const std::string path, const std::string xddFile);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingNode(const unsigned int nodeId, bool& exists);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNodeCount(unsigned int& nodeCount);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNodes(std::vector<unsigned int>& nodeIds); 

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetNodeParameter(UINT32 nodeId, const openCONFIGURATOR::Library::NodeParameter::NodeParameter param, const std::string value);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNodeParameter(UINT32 nodeId, const openCONFIGURATOR::Library::NodeParameter::NodeParameter param, std::string& value);


				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetIndexActualValue(const unsigned int nodeId, const unsigned int index, const std::string actualValue);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndexAttribute(const unsigned int nodeId, const unsigned int index, AttributeType attributeType, std::string& attributeValue);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexActualValue(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, const std::string actualValue);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexAttribute(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, AttributeType attributeType, std::string& attributeValue);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingIndex(const unsigned int nodeId, const unsigned int index, bool& exists);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingSubIndex(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, bool& exists);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndices(const unsigned int nodeId, std::vector<unsigned int>& indices); 
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndices(const unsigned int nodeId, const unsigned int index, std::vector<unsigned int>& subIndices); 

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndexCount(const unsigned int nodeId, unsigned int& indexCount);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexCount(const unsigned int nodeId, const unsigned int index, unsigned int& subIndexCount);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNumberOfEntries(const unsigned int nodeId, const unsigned int index, const bool getDefault, unsigned int& nrOfEntries);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GenerateProcessImageDescription(const OutputLanguage lang, const std::string outputPath, const std::string fileName);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GenerateStackConfiguration(const std::string outputPath, const std::string fileName);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetDataTypeSize(const openCONFIGURATOR::Library::ObjectDictionary::PlkDataType::PlkDataType type, unsigned int& size);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetFeatureValue(const unsigned int nodeId, const openCONFIGURATOR::Library::ObjectDictionary::PlkFeature::PlkFeature feature, std::string& featureValue);
		}
	}
}
%}
