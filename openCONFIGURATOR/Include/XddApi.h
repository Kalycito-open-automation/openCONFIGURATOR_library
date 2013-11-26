/************************************************************************
\file XddApi.h
\brief Functions to interface directly with imported XDD's.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef XDD_API_H_
#define XDD_API_H_

#include <string>
#include "Exports.h"
#include "OdApi.h"
#include "Result.h"

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetDataTypeSize(const openCONFIGURATOR::Library::ObjectDictionary::PlkDataType::PlkDataType type, UINT32& size);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetFeatureValue(const UINT32 nodeId, const FeatureType featureType, const std::string featureName, std::string& featureValue);

		}
	}
}

#endif