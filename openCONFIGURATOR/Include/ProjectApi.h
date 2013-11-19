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

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result NewProject(const std::string projectName, const std::string projectPath, const std::string pathToMNXdd = "");
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SaveProject(void);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result CloseProject(void);
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result OpenProject(const std::string projectFile);

		}
	}
}

#endif