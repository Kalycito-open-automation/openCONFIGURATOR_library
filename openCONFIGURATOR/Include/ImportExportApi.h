/************************************************************************
\file ImportExportApi.h
\brief	Functions related to input/output functionality of the library.

		I.e.: Generation of the stack-configuration and processs-image in various
		file formats.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef IMPORTEXPORT_API_H_
#define IMPORTEXPORT_API_H_

#include <string>

#include "Exports.h"
#include "Exception.h"

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			DLLEXPORT ocfmRetCode GenerateProcessImageDescription(const OutputLanguage lang, const std::string outputPath, const std::string fileName);
			DLLEXPORT ocfmRetCode GenerateStackConfiguration(const std::string outputPath, const std::string fileName);

		}
	}
}

#endif