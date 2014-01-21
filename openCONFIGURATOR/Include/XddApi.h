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
			/** \addtogroup xddapi
			 * @{
			 */
			/**
			\brief	Return size in bytes of a POWERLINK data type.

					See EPSG DS301 v1.1.0, section 6.1

			\param[in] type		openCONFIGURATOR::Library::ObjectDictionary::PlkFeature::PlkFeature to get the size of.
			\param[out] size	Byte size of the data type.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetDataTypeSize(const openCONFIGURATOR::Library::ObjectDictionary::PlkDataType::PlkDataType type, UINT32& size);

			/**
			\brief	Return the string-representation of the value of a device description entry.

					Read the value of a device description entry (also known as "feature" in DS311 v1.1.0) and return its string-representation.<br/>
					See EPSG DS311, v1.0.0 as well as EPSG DS301 v1.1.0 App. 2.

			\param[in] nodeId			Id of the %Node.
			\param[in] feature			openCONFIGURATOR::Library::ObjectDictionary::PlkFeature::PlkFeature to read.
			\param[out] featureValue	String-representation of a feature's value.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetFeatureValue(const UINT32 nodeId, const openCONFIGURATOR::Library::ObjectDictionary::PlkFeature::PlkFeature feature, std::string& featureValue);
			/** @} */
		}
	}
}

#endif
