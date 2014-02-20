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
#include "Result.h"

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{
			/** \addtogroup odapi
			 * @{
			 */
			/**
			\brief	Add an index to the object dictionary of a node.

			\param[in] nodeId		Id of the %Node to add the index to.
			\param[in] index		Index to add.
			\param[in] actualValue	Actual value of the index.
			\param[in] name			Name of the index.
			\param[in] objectType	::ObjectType of the index.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddIndex(const UINT32 nodeId, const UINT32 index, const std::string actualValue, const std::string name, ObjectType objectType);

			/**
			\brief	Set an attribute of a node's %Index to the given value.

			\param[in] nodeId			Id of the %Node.
			\param[in] index			%Index to manipulate.
			\param[in] attributeType	::AttributeType to set.
			\param[in] attributeValue	Value to set.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, const std::string attributeValue);

			/**
			\brief	Set the actual value of an %Index of a %Node.

			\param[in] nodeId		Id of the %Node.
			\param[in] index		%Index to manipulate.
			\param[in] actualValue	Actual value to set.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetIndexActualValue(const UINT32 nodeId, const UINT32 index, const std::string actualValue);

			/**
			\brief	Return the value of an attribute of a node's index.

			\param[in] nodeId			Id of the %Node.
			\param[in] index			%Index to get attribute from.
			\param[in] attributeType	::AttributeType to get.
			\param[out] attributeValue	Attribute value.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, std::string& attributeValue);

			/**
			\brief	Add a %SubIndex to an %Index of a %Node.

			\param[in] nodeId		Id of the %Node.
			\param[in] index		%Index to add the %SubIndex to.
			\param[in] subIndex		%SubIndex to add.
			\param[in] actualValue	Actual value of the %SubIndex.
			\param[in] name			Name of the index.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, const std::string actualValue, const std::string name);
			
			/**
			\brief	Set the actual value of a %SubIndex of an %Index.

			\param[in] nodeId		Id of the %Node.
			\param[in] index		%Index that holds the %SubIndex.
			\param[in] subIndex		%SubIndex to manipulate.
			\param[in] actualValue	Actual value to set.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexActualValue(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, const std::string actualValue);

			/**
			\brief	Get the attribute value of a %SubIndex of a %Node.

			\param[in] nodeId			Id of the %Node.
			\param[in] index			%Index that holds the %SubIndex.
			\param[in] subIndex			Subindex to manipulate.
			\param[in] attributeType	::AttributeType to get.
			\param[out] attributeValue	Attribute value.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, std::string& attributeValue);

			/**
			\brief	Check if an %Index exists in the object dictionary of a %Node.

			\param[in] nodeId	Id of the %Node.
			\param[in] index	%Index to check for. 
			\param[out] exists	<code>True</code> if the %Index exists, <code>false</code> otherwise.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingIndex(const UINT32 nodeId, const UINT32 index, bool& exists);

			/**
			\brief	Check if a subindex exists in the object dictionary of a node.

			\param[in] nodeId	Id of the %Node.
			\param[in] index	%Index that holds the %SubIndex.
			\param[in] subIndex	%SubIndex to check for.
			\param[out] exists	<code>True</code> if the %SubIndex exists, <code>false</code> otherwise.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result IsExistingSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, bool& exists);

			/**
			\brief	Return the no. of indices on a %Node.

			\param[in] nodeId		Id of the %Node.
			\param[out] indexCount	No. of indices on the %Node.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetIndexCount(const UINT32 nodeId, UINT32& indexCount);

			/**
			\brief	Return the no. of subIndices within an %Index on a %Node.

			\param[in] nodeId			Id of the %Node.
			\param[in] index			%Index that holds the subIndices.
			\param[out] subIndexCount	No. of subIndices in the %Index.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexCount(const UINT32 nodeId, const UINT32 index, UINT32& subIndexCount);

			/**
			\brief	Return the actualValue or defaultValue of subIndex 0x00 (NrOfEntries) of an %Index.

			\param[in] nodeId		Id of the %Node.
			\param[in] index		%Index that holds the %SubIndex.
			\param[in] getDefault	<code>True</code> to return the defaultValue, <code>false</code> to return the actualValue.
			\param[out] nrOfEntries	Actual or default value of %SubIndex 0x00 of the given %Index.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetNumberOfEntries(const UINT32 nodeId, const UINT32 index, const bool getDefault, UINT32& nrOfEntries);

			/**
			\brief	Delete an %Index of the object dictionary of a %Node.

			\param[in] nodeId	Id of the %Node.
			\param[in] index	%Index to delete.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteIndex(const UINT32 nodeId, const UINT32 index);

			/**
			\brief	Delete a %SubIndex of an %Index.

			\param[in] nodeId	Id of the %Node.
			\param[in] index	%Index that holds the %SubIndex.
			\param[in] subIndex	%SubIndex to delete.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex);
			/** @} */
			openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, const std::string attributeValue);
		}
	}
}

#endif