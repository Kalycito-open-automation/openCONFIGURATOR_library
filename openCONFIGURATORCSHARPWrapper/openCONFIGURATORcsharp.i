%module openCONFIGURATORcsharpWRAPPER
#define DLLEXPORT
/* Windows Platform */
#if defined(_WIN32) && defined(_MSC_VER)
	 #undef DLLEXPORT
	 #define DLLEXPORT  __declspec(dllexport)
	 
#endif

%{
/* Simply include the header definitions for the SWIG */

	#include "../openCONFIGURATOR/Include/Exports.h"
	#include "../openCONFIGURATOR/Include/Enums.h"
	#include "../openCONFIGURATOR/Include/Exception.h"		
%}

/* Include all the required SWIG interfaces */
%include std_except.i
%include std_string.i
%include std_vector.i
%include typemaps.i
%include std_string_ref.i

%apply unsigned int& OUTPUT { unsigned int& size };

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

%include "../openCONFIGURATOR/Include/Exports.h"
%include "../openCONFIGURATOR/Include/Enums.h"

;

class DLLEXPORT ocfmRetCode : public std::exception
{
public:
	ocfmRetCode(void);
	ocfmRetCode(ConfiguratorError currentError);
	ocfmRetCode(const ocfmRetCode& origin);
	~ocfmRetCode(void) throw ();

	ConfiguratorError getErrorCode(void);
	const std::string& getErrorString(void);
};

%inline %{

DLLEXPORT ocfmRetCode NewProject(const std::string projectName, const std::string projectPath, const std::string pathToMNXdd = "");
DLLEXPORT ocfmRetCode SaveProject(void);
DLLEXPORT ocfmRetCode CloseProject(void);
DLLEXPORT ocfmRetCode OpenProject(const std::string projectFile);

/*DLLEXPORT ocfmRetCode AddNode(const Node& node, const std::string path, const std::string xddFile);*/
DLLEXPORT ocfmRetCode AddNode(const unsigned int nodeId, const NodeType nodeType, const std::string nodeName, const std::string xddFile = "");
DLLEXPORT ocfmRetCode DeleteNode(const unsigned int nodeId);
/*DLLEXPORT ocfmRetCode GetNode(const unsigned int nodeId, Node& node);*/
DLLEXPORT ocfmRetCode ReplaceXdd(const unsigned int nodeId, const std::string path, const std::string xddFile);
DLLEXPORT bool IsExistingNode(const unsigned int nodeId);
DLLEXPORT unsigned int GetNodeCount(void);

DLLEXPORT ocfmRetCode AddIndex(const unsigned int nodeId, const unsigned int index, const std::string actualValue, const std::string name, ObjectType objectType);
DLLEXPORT ocfmRetCode SetIndexAttribute(const unsigned int nodeId, const unsigned int index, AttributeType attributeType, const std::string attributeValue);
DLLEXPORT ocfmRetCode SetIndexActualValue(const unsigned int nodeId, const unsigned int index, const std::string actualValue);
DLLEXPORT ocfmRetCode GetIndexAttribute(const unsigned int nodeId, const unsigned int index, AttributeType attributeType, std::string& attributeValue);

DLLEXPORT ocfmRetCode AddSubIndex(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, const std::string actualValue, const std::string name);
DLLEXPORT ocfmRetCode SetSubIndexAttribute(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, AttributeType attributeType, const std::string attributeValue);
DLLEXPORT ocfmRetCode SetSubIndexActualValue(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, const std::string actualValue);
DLLEXPORT ocfmRetCode GetSubIndexAttribute(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex, AttributeType attributeType, std::string& attributeValue);

DLLEXPORT bool IsExistingIndex(const unsigned int nodeId, const unsigned int index);
DLLEXPORT bool IsExistingSubIndex(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex);

DLLEXPORT unsigned int GetIndexCount(const unsigned int nodeId);
DLLEXPORT unsigned int GetSubIndexCount(const unsigned int nodeId, const unsigned int index);

DLLEXPORT unsigned int GetNumberOfEntries(const unsigned int nodeId, const unsigned int index, const bool getDefault);

DLLEXPORT ocfmRetCode DeleteIndex(const unsigned int nodeId, const unsigned int index);
DLLEXPORT ocfmRetCode DeleteSubIndex(const unsigned int nodeId, const unsigned int index, const unsigned int subIndex);

DLLEXPORT ocfmRetCode GenerateProcessImageDescription(const OutputLanguage lang, const std::string outputPath, const std::string fileName);
DLLEXPORT ocfmRetCode GenerateStackConfiguration(const std::string outputPath, const std::string fileName);

DLLEXPORT ocfmRetCode GetDataTypeSize(const std::string name, unsigned int& size);
DLLEXPORT ocfmRetCode GetFeatureValue(const unsigned int nodeId, const FeatureType featureType, const std::string featureName, std::string& featureValue);

DLLEXPORT bool SetLibraryResourcePath(const std::string libraryResourcePath);

%}
