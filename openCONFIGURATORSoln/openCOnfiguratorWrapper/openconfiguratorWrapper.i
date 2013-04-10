%module openConfiguratorWrapper
#define DLLEXPORT
/* Windows Platform */
#if defined(_WIN32) && defined(_MSC_VER)
	 #undef DLLEXPORT
	 #define DLLEXPORT  __declspec(dllexport)
	 
#endif

%{

#include "../openCONFIGURATOR/Include/Exports.h"
#include "../openCONFIGURATOR/Include/Error.h"
#include "../openCONFIGURATOR/Include/Declarations.h"

%}

%include cpointer.i
%include cstring.i
%pointer_functions(char, charp)
%pointer_functions(long, longp)
%pointer_functions(int, intp)
%pointer_functions(bool, boolp)
%pointer_functions(AutoGenerate, AutoGeneratep)
%pointer_functions(AutoSave, AutoSavep)
%pointer_functions(ViewMode, ViewModep)
%pointer_functions(StationType, StationTypep)
%cstring_bounded_output(char *outAttributeValue, 512);
%cstring_bounded_output(char *outIndexID, 100);
%cstring_bounded_output(char *outSubIndexID, 100);
%cstring_bounded_output(char *outNodeName, 100);
%cstring_bounded_output(char *outFeatureValue, 512);
%cstring_bounded_output(char *outDataTypes, 1000);
%cstring_bounded_output(char *outForcedCycle, 10);


%include typemaps.i
%include exception.i
%include std_except.i

%include "../openCONFIGURATOR/Include/Declarations.h"
%include "../openCONFIGURATOR/Include/Error.h"
%include "../openCONFIGURATOR/Include/Exports.h"

using namespace std; 


%inline %{
 ocfmRetCode ImportXML(char *fileName, INT32 nodeId, NodeType nodeType);
 ocfmRetCode ReImportXML(char* fileName, INT32 nodeId, NodeType nodeType);
 ocfmRetCode GenerateXAP(char* xapFilePath);
 ocfmRetCode GenerateCDC(char* cdcPath);
 ocfmRetCode GenerateNET(char* netFilePath);
 ocfmRetCode CreateNode(INT32 nodeId, NodeType nodeType, char* nodeName);
 ocfmRetCode DeleteNode(INT32 nodeId, NodeType nodeType);
 ocfmRetCode DeleteNodeObjDict(INT32 nodeId, NodeType nodeType);
 ocfmRetCode DeleteIndex(INT32 nodeId, NodeType nodeType, char* indexId);
 ocfmRetCode DeleteSubIndex(INT32 nodeId, NodeType nodeType, char* indexId, char* subIndexID);
 ocfmRetCode AddIndex(INT32 nodeId, NodeType nodeType, char* indexId);
 ocfmRetCode AddSubIndex(INT32 nodeId, NodeType nodeType, char* indexId, char* subIndexId);
 ocfmRetCode SetBasicIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* indexValue, char* indexName, Flag includeInCDC);
 ocfmRetCode SetBasicSubIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId, char* indexValue, char* indexName, Flag includeInCDC);
 ocfmRetCode IfNodeExists(INT32 nodeId, NodeType nodeType, INT32 *nodePos, bool& nodeExist);
 ocfmRetCode IfIndexExists(INT32 nodeId, NodeType nodeType, char* indexId, INT32 *idxPos);
 ocfmRetCode IfSubIndexExists(INT32 nodeId, NodeType nodeType, char* idxId, char* sidxId, INT32* sidxPos, INT32* idxPos);
 ocfmRetCode GetIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, AttributeType attributeType, char* outAttributeValue);
 ocfmRetCode GetSubIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId, AttributeType attributeType, char* outAttributeValue);
 ocfmRetCode GetNodeCount(INT32 nodeId, INT32* outNodeCount);
 ocfmRetCode GetIndexCount(INT32 nodeId, NodeType nodeType, INT32* outIndexCount);
 ocfmRetCode GetSubIndexCount(INT32 nodeId, NodeType nodeType, char* indexId, INT32* outSubIndexCount);
 ocfmRetCode GetNodeAttributesbyNodePos(INT32 nodePos, INT32* outNodeId, char* outNodeName, StationType* outStationType, char* outForcedCycle, bool* outIsForcedCycle);
 ocfmRetCode GetIndexIDbyIndexPos(INT32 nodeId, NodeType nodeType, INT32 indexPos, char* outIndexId);
 ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 nodeId, NodeType nodeType, char* indexId, INT32 subIndexPos, char* outSubIndexID);
 ocfmRetCode GetIndexIDbyPositions(INT32 nodePos, INT32 indexPos, char* outIndexID);
 ocfmRetCode GetSubIndexIDbyPositions(INT32 nodePos, INT32 indexPos, INT32 subIndexPos, char* outSubIndexID);
 ocfmRetCode GetIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos, AttributeType attributeType, char* outAttributeValue);
 ocfmRetCode GetSubIndexAttributesbyPositions(INT32 nodePos, INT32 indexPos, INT32 subIndexPos, AttributeType attributeType, char* outAttributeValue);
 void LoadObjectDictionary(char* xmlFilePath);
 ocfmRetCode SaveNode(const char* fileName, INT32 nodeId, NodeType nodeType);
 ocfmRetCode SaveProject(char* projectPath, char* projectName);
 ocfmRetCode OpenProject(char* projectPath, char* projectFileName);
 ocfmRetCode FreeProjectMemory();
 ocfmRetCode GetProjectSettings(AutoGenerate *autoGenStatus, AutoSave *autoSaveStatus, ViewMode *viewMode, bool* isExpertViewAlreadySet);
 ocfmRetCode SetProjectSettings(AutoGenerate autoGenStatus, AutoSave autoSaveStatus, ViewMode viewMode, bool isExpertViewAlreadySet);
 ocfmRetCode GenerateMNOBD();
 ocfmRetCode SetAllIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* actualValue, char* indexName, char* accessType, char* dataTypeName, char* pdoMappingVal, char* defaultValue, char* highLimitVal, char* lowLimitVal, char* objectType, Flag includeInCDC);
 ocfmRetCode SetAllSubIndexAttributes(INT32 nodeId, NodeType nodeType, char* indexId, char* sidxId, char* actualValue, char* indexName, char* accessType, char* dataTypeName, char* pdoMappingVal, char* defaultValue, char* highLimitVal, char* lowLimitVal, char* objectType, Flag includeInCDC);
 ocfmRetCode GetFeatureValue(INT32 nodeId, NodeType nodeType, FeatureType featureType, char* featureName, char* outFeatureValue);
 ocfmRetCode UpdateNodeParams(INT32 currentNodeId, INT32 newNodeID, NodeType nodeType, char* nodeName, StationType stationType, char* forcedCycleVal, bool isForcedCycle, char* presTimeoutVal);
 ocfmRetCode GetNodeDataTypes(INT32 nodeId, NodeType nodeType, char* outDataTypes);
 ocfmRetCode NewProjectNode(INT32 nodeId, NodeType nodeType, char* nodeName, char* importXmlFile);
 INT32 GetDataSize(char* dataTypeVal);
%}

