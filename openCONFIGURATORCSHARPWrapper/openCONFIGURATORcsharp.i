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
%}

/* Include all the required SWIG interfaces */
%include std_except.i
%include std_string.i
%include std_vector.i
%include typemaps.i
%include std_string_ref.i

%apply unsigned int& OUTPUT { unsigned int& size };

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
%ignore FeatureType;
%ignore IEC_Datatype;
%ignore NodeType;
%ignore ObjectType;
%ignore PDOMapping;
%ignore PDOType;
%ignore ParameterAccess;
%ignore ProjectSettings;
%ignore StationType;
%ignore ViewMode;
%ignore AccessType;
%ignore ConfiguratorError;
%ignore GetAccessType(const std::string& accessType);
%ignore GetObjectType(const std::string& objectType);

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
				MAX_PI_SIZE_EXCEEDED
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

				/*DLLEXPORT Result AddNode(const Node& node, const std::string path, const std::string xddFile);*/
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const unsigned int nodeId, const NodeType nodeType, const std::string nodeName, const std::string xddFile = "");
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteNode(const unsigned int nodeId);
				/*DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNode(const unsigned int nodeId, Node& node);*/
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result ReplaceXdd(const unsigned int nodeId, const std::string path, const std::string xddFile);
				DLLEXPORT bool IsExistingNode(const unsigned int nodeId);
				DLLEXPORT unsigned int GetNodeCount(void);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddIndex(const unsigned int nodeId, const unsigned int index, const std::string actualValue, const std::string name, ObjectType objectType);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetIndexAttribute(const unsigned int nodeId, const unsigned int index, AttributeType attributeType, const std::string attributeValue);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetIndexActualValue(const unsigned int nodeId, const unsigned int index, const std::string actualValue);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndexAttribute(const unsigned int nodeId, const unsigned int index, AttributeType attributeType, std::string& attributeValue);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddSubIndex(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, const std::string actualValue, const std::string name);
				/*DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexAttribute(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, AttributeType attributeType, const std::string attributeValue);*/
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexActualValue(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, const std::string actualValue);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexAttribute(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, AttributeType attributeType, std::string& attributeValue);

				DLLEXPORT bool IsExistingIndex(const unsigned int nodeId, const unsigned int index);
				DLLEXPORT bool IsExistingSubIndex(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex);

				DLLEXPORT unsigned int GetIndexCount(const unsigned int nodeId);
				DLLEXPORT unsigned int GetSubIndexCount(const unsigned int nodeId, const unsigned int index);

				DLLEXPORT unsigned int GetNumberOfEntries(const unsigned int nodeId, const unsigned int index, const bool getDefault);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteIndex(const unsigned int nodeId, const unsigned int index);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteSubIndex(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GenerateProcessImageDescription(const OutputLanguage lang, const std::string outputPath, const std::string fileName);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GenerateStackConfiguration(const std::string outputPath, const std::string fileName);

				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetDataTypeSize(const std::string name, unsigned int& size);
				DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetFeatureValue(const unsigned int nodeId, const FeatureType featureType, const std::string featureName, std::string& featureValue);
		}
	}
}
%}