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

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			DLLEXPORT ocfmRetCode GetDataTypeSize(const std::string name, UINT32& size);
			DLLEXPORT ocfmRetCode GetFeatureValue(const UINT32 nodeId, const FeatureType featureType, const std::string featureName, std::string& featureValue);

		}
	}
}

#endif