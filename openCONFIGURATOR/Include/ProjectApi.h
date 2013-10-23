/************************************************************************
\file ProjectApi.h
\brief Functions to interface with openCONFIGURATORs project-management.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef PROJECT_API_H_
#define PROJECT_API_H_

#include <string>
#include "Exports.h"
#include "Exception.h"

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			DLLEXPORT ocfmRetCode NewProject(const std::string projectName, const std::string projectPath, const std::string pathToMNXdd = "");
			DLLEXPORT ocfmRetCode SaveProject(void);
			DLLEXPORT ocfmRetCode CloseProject(void);
			DLLEXPORT ocfmRetCode OpenProject(const std::string projectFile);

		}
	}
}

#endif