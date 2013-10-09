#ifndef XDD_API_H_
#define XDD_API_H_

#include <string>
#include "Exports.h"
#include "OdApi.h"

DLLEXPORT ocfmRetCode GetDataTypeSize(const string name, UINT32& size);
DLLEXPORT ocfmRetCode GetFeatureValue(const UINT32 nodeId, const FeatureType featureType, const string featureName, string& featureValue);

#endif