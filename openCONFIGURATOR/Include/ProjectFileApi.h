/************************************************************************
\file ProjectFileApi.h
\brief Functions to interface with openCONFIGURATORs project file management.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef PROJECT_FILE_API_H_
#define PROJECT_FILE_API_H_

#include <string>
#include "Exports.h"
#include "Result.h"

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace API
		{
			/** \addtogroup projectfileapi
			 * @{
			 */
			/**
			\brief	Add a view setting to the %openCONFIGURATOR project configuration.

			\param[in] viewType to add the setting to.
			\param[in] name of the setting.
			\param[in] value of the setting.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType viewType, const std::string name, const std::string value);
			
			/**
			\brief	Get a view setting value from the %openCONFIGURATOR project configuration.

			\param[in] viewType to add the setting to.
			\param[in] name of the setting.
			\param[out] value to be retrieved.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType viewType, const std::string name, std::string& value);
			
			/**
			\brief	Delete a view setting from the %openCONFIGURATOR project configuration.

			\param[in] viewType to add the setting to.
			\param[in] name of the setting.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType viewType, const std::string name);
			
			/**
			\brief	Set the active view type in the %openCONFIGURATOR project configuration.

			\param[in] viewType active View Type.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetActiveView(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType viewType);
			
			/**
			\brief	Get the active view type in the %openCONFIGURATOR project configuration.

			\param[out] viewType active View Type to be retrieved.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetActiveView(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType& viewType);
			/** @} */

		}
	}
}

#endif