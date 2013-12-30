/************************************************************************
\file ProjectApi.h
\brief Functions to interface with openCONFIGURATORs project-management.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef PROJECT_API_H_
#define PROJECT_API_H_

#include <string>
#include "Exports.h"
#include "Result.h"

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace API
		{
			/** \addtogroup projectapi
			 * @{
			 */
			/**
			\brief	Create a new %openCONFIGURATOR project.

			\param[in] projectName	Name of the new project.
			\param[in] projectPath	Path of the new project.
			\param[in] pathToMNXdd	path to MN-XDD (if empty, a default MN-XDD will be used).

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result NewProject(const std::string projectName, const std::string projectPath, const std::string pathToMNXdd = "");

			/**
			\brief	Save %openCONFIGURATOR project.

			Updates the project file with the current configuration and persists all imported XDDs as XDCs or alters all existing XDCs with the objects actual values.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SaveProject();

			/**
			\brief	Save %openCONFIGURATOR project in a new folder.

			Updates the project file with the current configuration and persists all imported XDDs as XDCs or alters all existing XDCs with the objects actual values in a new folder.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SaveProjectAs(const string projectName, const string projectPath);

			/**
			\brief	Close project and free allocated resources.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result CloseProject();

			/**
			\brief	Open existing %openCONFIGURATOR project.

			\param[in] projectFile Full path to project file to open.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result OpenProject(const std::string projectFile);

			/**
			\brief	Add a path setting to the %openCONFIGURATOR project.

			This setting will be stored in the PathSettings section in  the %openCONFIGURATOR project file.

			\param[in] id of the path.
			\param[in] path itself.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddPath(const std::string id, const std::string path);

			/**
			\brief	Get a path setting from the %openCONFIGURATOR project.

			\param[in] id of the path.
			\param[out] path to be retrieved.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetPath(const std::string id, std::string& path);

			/**
			\brief	Delete a path setting from the %openCONFIGURATOR project.

			\param[in] id of the path.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeletePath(const std::string id);

			/**
			\brief	Set the active auto calculation configuration.

			Currently the %openCONFIGURATOR library supports generating the MN mapping and node assignements (ID: "all") or generating nothing (ID: "none").

			\param[in] id of the configuration.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetActiveAutoCalculationConfig(const std::string activeAutoGenerationSetting);


			/**
			\brief	Get the active auto calculation configuration.

			\param[out] id of the configuration to be retrieved.

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetActiveAutoCalculationConfig(std::string& activeAutoGenerationSetting);
			/** @} */
		}
	}
}

#endif