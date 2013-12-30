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
			/** @} */
		}
	}
}

#endif