/*******************************************************************************
* \file   LibraryConfiguration.cpp
*
* \author Ramakrishnan Periyakaruppan, Kalycito Infotech Private Limited.
*
* \copyright (c) 2014, Kalycito Infotech Private Limited
*                    All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of the copyright holders nor the
*     names of its contributors may be used to endorse or promote products
*     derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "../include/LibraryConfiguration.h"
#include "../include/Constants.h"

#include <cstdlib>
#ifdef DEBUG
#include <iostream>
#endif

PROJECT_UPGRADE_OPEN_NAMESPACE

const std::string LibraryConfiguration::kDefaultLibResourcePath = "resources";
const std::string LibraryConfiguration::kLibResourcePathEnvVar = "OPENCONFIGURATOR_LIBRARY_RESOURCEPATH";
const std::string LibraryConfiguration::kRelXDDSchemaPath = "xddschema/Powerlink_Main.xsd";
const std::string LibraryConfiguration::kRelProjectFileSchemaPath = "OC_ProjectFile/openCONFIGURATOR.xsd";
const std::string LibraryConfiguration::kLibXmlLintPath = "xmllint";

std::string LibraryConfiguration::libResourcePath;

const std::string& LibraryConfiguration::GetLibResourcePath()
{
	if (LibraryConfiguration::libResourcePath.empty())
	{
		const char* libResourcePathEnvVar = std::getenv(kLibResourcePathEnvVar.c_str());
		if (libResourcePathEnvVar)
			LibraryConfiguration::libResourcePath = libResourcePathEnvVar;
		else
			LibraryConfiguration::libResourcePath = kDefaultLibResourcePath;
#ifdef DEBUG
		std::cout << "Library resource path evaluated to: " << LibraryConfiguration::libResourcePath << std::endl;
#endif
	}
	return LibraryConfiguration::libResourcePath;
}

const std::string LibraryConfiguration::GetXddSchemaFilePath()
{
	return std::string(LibraryConfiguration::GetLibResourcePath()
					   + kPathSeparator
					   + kRelXDDSchemaPath);
}

const std::string LibraryConfiguration::GetProjectFileSchemaFilePath()
{
	return std::string(LibraryConfiguration::GetLibResourcePath()
					   + kPathSeparator
					   + kRelProjectFileSchemaPath);
}

const std::string LibraryConfiguration::GetXmlLintFilePath()
{
	std::string path;
#if defined(_WIN32) && defined(_MSC_VER)
	path = (LibraryConfiguration::GetLibResourcePath()
			+ kPathSeparator
			+ "bin"
			+ kPathSeparator);
#endif

	path += kLibXmlLintPath;

#if defined(_WIN32) && defined(_MSC_VER)
	return (path + ".exe");
#else
	return path;
#endif
}

PROJECT_UPGRADE_CLOSE_NAMESPACE
