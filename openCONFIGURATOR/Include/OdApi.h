/************************************************************************
\file OdApi.h
\brief Functions to interface with the Object-Dictionary of a POWERLINK-Node.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef OD_API_H_
#define OD_API_H_

#include <string>
#include "Exception.h"
#include "Exports.h"
#include "Node.h"
#include "Result.h";

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddIndex(const UINT32 nodeId, const UINT32 index, const std::string actualValue, const std::string name, ObjectType objectType);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, const std::string attributeValue);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetIndexActualValue(const UINT32 nodeId, const UINT32 index, const std::string actualValue);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, std::string& attributeValue);

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, const std::string actualValue, const std::string name);
			/*openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, const std::string attributeValue);*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexActualValue(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, const std::string actualValue);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, std::string& attributeValue);

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingIndex(const UINT32 nodeId, const UINT32 index);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex);

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndexCount(const UINT32 nodeId, UINT32& indexCount);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexCount(const UINT32 nodeId, const UINT32 index, UINT32& subIndexCount);

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNumberOfEntries(const UINT32 nodeId, const UINT32 index, const bool getDefault, UINT32& nrOfEntries);

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteIndex(const UINT32 nodeId, const UINT32 index);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex);

		}
	}
}

#endif