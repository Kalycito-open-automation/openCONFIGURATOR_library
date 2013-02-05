%module openConfiguratorWrapper
#define DllExport
/* Windows Platform */
#if defined(_WIN32) && defined(_MSC_VER)
	 #undef DllExport
	 #define DllExport  __declspec(dllexport)
	 
#endif

%{
#include "../openCONFIGURATOR/Include/ApplicationProcess.h"
#include "../openCONFIGURATOR/Include/BaseIndex.h"
#include "../openCONFIGURATOR/Include/ComplexDataType.h"
#include "../openCONFIGURATOR/Include/Error.h"

#include "../openCONFIGURATOR/Include/Exports.h"
#include "../openCONFIGURATOR/Include/DataTypeCollection.h"
#include "../openCONFIGURATOR/Include/Declarations.h"
#include "../openCONFIGURATOR/Include/globals.h"
#include "../openCONFIGURATOR/Include/IndexCollection.h"
#include "../openCONFIGURATOR/Include/Index.h"
#include "../openCONFIGURATOR/Include/NodeCollection.h"
#include "../openCONFIGURATOR/Include/Node.h"
#include "../openCONFIGURATOR/Include/SubIndex.h"


%}

%include cpointer.i
%include cstring.i
%pointer_functions(char, charp)
%pointer_functions(long, longp)
%pointer_functions(int, intp)
%pointer_functions(bool, boolp)
%pointer_functions(EAutoGenerate, EAutoGeneratep)
%pointer_functions(EAutoSave, EAutoSavep)
%pointer_functions(EViewMode, EViewModep)
%pointer_functions(EStationType, EStationTypep)
%cstring_bounded_output(char *Out_AttributeValue, 512);
%cstring_bounded_output(char *Out_IndexID, 100);
%cstring_bounded_output(char *Out_SubIndexID, 100);
%cstring_bounded_output(char *Out_NodeName, 100);
%cstring_bounded_output(char *Out_FeatureValue, 512);
%cstring_bounded_output(char *Out_DataTypes, 1000);
%cstring_bounded_output(char *Out_ForcedCycle, 10);


%include typemaps.i
%include exception.i
%include std_except.i

/* Let's just grab the original header file here */

%include "../openCONFIGURATOR/Include/ApplicationProcess.h"
%include "../openCONFIGURATOR/Include/BaseIndex.h"
%include "../openCONFIGURATOR/Include/ComplexDataType.h"
%include "../openCONFIGURATOR/Include/Error.h"

%include "../openCONFIGURATOR/Include/Exports.h"
%include "../openCONFIGURATOR/Include/DataTypeCollection.h"
%include "../openCONFIGURATOR/Include/Declarations.h"
%include "../openCONFIGURATOR/Include/globals.h"
%include "../openCONFIGURATOR/Include/IndexCollection.h"
%include "../openCONFIGURATOR/Include/Index.h"
%include "../openCONFIGURATOR/Include/NodeCollection.h"
%include "../openCONFIGURATOR/Include/Node.h"
%include "../openCONFIGURATOR/Include/SubIndex.h"


using namespace std; 


%inline %{

ocfmRetCode ImportXML(char* fileName, INT32 NodeID, ENodeType NodeType);
ocfmRetCode ReImportXML(char* fileName, INT32 NodeID, ENodeType NodeType);

ocfmRetCode GenerateXAP(char* fileName);
ocfmRetCode GenerateCDC(char* fileName);

ocfmRetCode CreateNode(INT32 NodeID, ENodeType NodeType, char* NodeName);
ocfmRetCode parseFile(char* filename, INT32 NodeID, ENodeType  NodeType);
ocfmRetCode DeleteNode(INT32 NodeID, ENodeType NodeType);
ocfmRetCode DeleteNodeObjDict(INT32 NodeID, ENodeType NodeType);

ocfmRetCode DeleteIndex(INT32 NodeID, ENodeType NodeType, char* IndexID);
ocfmRetCode DeleteSubIndex(INT32 NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
ocfmRetCode AddIndex(INT32 NodeID, ENodeType NodeType, char* IndexID);
ocfmRetCode AddSubIndex(INT32 NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
ocfmRetCode SetIndexAttributes(INT32 NodeID, ENodeType NodeType, char* IndexID, char* IndexValue, char* IndexName, EFlag flagIfInCdc);
//ocfmRetCode SetSubIndexAttributes(INT32 NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName);
ocfmRetCode SetSubIndexAttributes(INT32 NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName, EFlag flagIfInCdc);

ocfmRetCode IfNodeExists(INT32 NodeID, ENodeType NodeType, INT32* NodePos, bool& ExistfFlag);
ocfmRetCode IfIndexExists(INT32 NodeID, ENodeType NodeType, char* IndexID, INT32* IndexPos);
ocfmRetCode IfSubIndexExists(INT32 NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, INT32* SubIndexPos, INT32* IndexPos);

ocfmRetCode GetIndexAttributes(INT32 NodeID, ENodeType NodeType, char* IndexID, EAttributeType AttributeType, char* Out_AttributeValue);
ocfmRetCode GetSubIndexAttributes(INT32 NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, EAttributeType AttributeType,char* Out_AttributeValue);
ocfmRetCode GetNodeCount(INT32 MNID, INT32* Out_NodeCount);
ocfmRetCode GetIndexCount(INT32 NodeID, ENodeType NodeType, INT32* Out_IndexCount);
ocfmRetCode GetSubIndexCount(INT32 NodeID, ENodeType NodeType, char* IndexID, INT32* Out_SubIndexCount);

ocfmRetCode GetNodeAttributesbyNodePos(INT32 NodePos, INT32* Out_NodeID, char* Out_NodeName, EStationType* Out_eStationType, char* Out_ForcedCycle, bool* bForcedCycleFlag);
ocfmRetCode GetIndexIDbyIndexPos(INT32 NodeID, ENodeType NodeType, INT32 IndexPos, char* Out_IndexID);
ocfmRetCode GetSubIndexIDbySubIndexPos(INT32 NodeID, ENodeType NodeType, char* IndexID, INT32 SubIndexPos, char* Out_SubIndexID);

ocfmRetCode GetIndexIDbyPositions(INT32 NodePos, INT32 IndexPos, char* Out_IndexID);
ocfmRetCode GetSubIndexIDbyPositions(INT32 NodePos, INT32 IndexPos, INT32 SubIndexPos, char* Out_SubIndexID);
ocfmRetCode GetIndexAttributesbyPositions(INT32 NodePos, INT32 IndexPos, EAttributeType AttributeType, char* Out_AttributeValue);
ocfmRetCode GetSubIndexAttributesbyPositions(INT32 NodePos, INT32 IndexPos, INT32 SubIndexPos, EAttributeType AttributeType, char* Out_AttributeValue);

void LoadObjectDictionary(char* fileName);
ocfmRetCode GenerateMNOBD();

ocfmRetCode SetAllIndexAttributes(INT32 NodeID, ENodeType NodeType, 
char* IndexID, char* ActualValue,
char* IndexName, char* Access, char* dataTypeValue,
char* pdoMappingVal, char* defaultValue, char* highLimit,
char* lowLimit, char* objType, EFlag flagIfIncludedInCdc);

ocfmRetCode SetAllSubIndexAttributes(INT32 NodeID, ENodeType NodeType, 
char* IndexID, char* SubIndexID, char* ActualValue,
char* IndexName, char* Access, char* dataTypeValue,
char* pdoMappingVal, char* defaultValue, char* highLimit,
char* lowLimit, char* objType, EFlag flagIfIncludedInCdc);

ocfmRetCode GetProjectSettings(EAutoGenerate* autoGen, EAutoSave* autoSave, EViewMode* viewMode, bool* bExpertViewAlreadySet);
ocfmRetCode SetProjectSettings(EAutoGenerate autoGen, EAutoSave autoSave, EViewMode viewMode, bool bExpertViewAlreadySet);

ocfmRetCode FreeProjectMemory();
ocfmRetCode GetFeatureValue(INT32 iNodeId, ENodeType eNodeType, EFeatureType eFeatureType, char* FeatureName, char* Out_FeatureValue);
ocfmRetCode UpdateNodeParams(INT32 iCurrNodeId, INT32 iNewNodeID, ENodeType eNodeType, char* NodeName, EStationType eStationType, char* ForcedCycle , bool ForcedCycleFlag, char* PollResponseTimeout);
ocfmRetCode GetNodeDataTypes(INT32 iNodeId, ENodeType eNodeType, char* Out_DataTypes);
ocfmRetCode NewProjectNode(INT32 iNodeID, ENodeType enumNodeType, char* pbNodeName, char * pbImportXmlFile);
%}

