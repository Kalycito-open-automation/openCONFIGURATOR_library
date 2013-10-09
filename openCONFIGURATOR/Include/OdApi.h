#ifndef OD_API_H_
#define OD_API_H_

#include <string>
#include "Exception.h"
#include "Exports.h"
#include "Node.h"

DLLEXPORT ocfmRetCode AddIndex(const UINT32 nodeId, const UINT32 index, const string actualValue, const string name, ObjectType objectType);
DLLEXPORT ocfmRetCode SetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, const string attributeValue);
DLLEXPORT ocfmRetCode SetIndexActualValue(const UINT32 nodeId, const UINT32 index, const string actualValue);
DLLEXPORT ocfmRetCode GetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, string& attributeValue);

DLLEXPORT ocfmRetCode AddSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, const string actualValue, const string name);
DLLEXPORT ocfmRetCode SetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, const string attributeValue);
DLLEXPORT ocfmRetCode SetSubIndexActualValue(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, const string actualValue);
DLLEXPORT ocfmRetCode GetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, string& attributeValue);

DLLEXPORT bool IsExistingIndex(const UINT32 nodeId, const UINT32 index);
DLLEXPORT bool IsExistingSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex);

DLLEXPORT UINT32 GetIndexCount(const UINT32 nodeId);
DLLEXPORT UINT32 GetSubIndexCount(const UINT32 nodeId, const UINT32 index);

DLLEXPORT UINT32 GetNumberOfEntries(const UINT32 nodeId, const UINT32 index, const bool getDefault);

DLLEXPORT ocfmRetCode DeleteIndex(const UINT32 nodeId, const UINT32 index);
DLLEXPORT ocfmRetCode DeleteSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex);

#endif