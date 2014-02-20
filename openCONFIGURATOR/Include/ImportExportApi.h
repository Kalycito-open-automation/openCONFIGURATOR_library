/************************************************************************
\file ImportExportApi.h
\brief	Functions related to input/output functionality of the library.

I.e.: Generation of the stack-configuration and process-image in various
file formats.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef IMPORTEXPORT_API_H_
#define IMPORTEXPORT_API_H_

#include <string>

#include "Exports.h"
#include "Exception.h"
#include "Result.h"

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{ 
			/** \addtogroup imexport
			 * @{
			 */
			/**
			\brief	Generate the process image description for a POWERLINK network.
					
			Generate a process image description file in the programming language specified by the user. 

			\param[in] lang	::OutputLanguage specified for the process image file. 
			\param[in] outputPath Output path for the process image file (default project output path if empty).
			\param[in] fileName	File name for the process image code file (will default to "PI.*" if empty).

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GenerateProcessImageDescription(const OutputLanguage lang, const std::string outputPath, const std::string fileName);

			/**
			\brief	Generate the stack configuration file for a POWERLINK network.

			Generate a POWERLINK stack configuration file in the CDC file format (Concise Device Configuration) in text and binary format.

			\param[in] outputPath	Output path for the stack configuration file (if empty library uses default output path of project).
			\param[in] fileName	File name for the stack configuration file (will default to "mnobd.*" if empty).

			\return openCONFIGURATOR::Library::ErrorHandling::Result
			*/
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GenerateStackConfiguration(const std::string outputPath, const std::string fileName);
			/** @} */
		}
	}
}

#endif