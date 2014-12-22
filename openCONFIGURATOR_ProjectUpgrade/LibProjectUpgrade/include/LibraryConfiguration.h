/*******************************************************************************
* \file   LibraryConfiguration.h
*
* \brief  Header file that includes functions used to retrieve configuration 
*         options of the project upgrade library.
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

#ifndef _PROJECT_UPGRADE_LIBRARY_CONFIGURATION_H_
#define _PROJECT_UPGRADE_LIBRARY_CONFIGURATION_H_

#include <string>

#include "Namespace.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

/**
 * \brief
 *
 */
class LibraryConfiguration
{
	public:
		~LibraryConfiguration();

		/**
		 * \brief Get library resource path.
		 *
		 *		The library resource path will be lazily instantiated the first time
		 * 		this method is called and will be determined as follows:<br/>
		 * 		1. If environment variable OPENCONFIGURATOR_LIBRARY_RESOURCEPATH exists,
		 * 		   resource path will be set to its value and returned.
		 * 		2. Otherwise "resources" will be set as resource path and returned.
		 *
		 * \return The library resource path containing all external resources the library could possibly load.
		 */
		static const std::string& GetLibResourcePath();

		/**
		 * \return The path to the XDD XML-Schema file "Powerlink_main.xsd".
		 */
		static const std::string GetXddSchemaFilePath();

		/**
		 * \return The path to the openCONFIGURATOR project file schema "openCONFIGURATOR.xsd".
		 */
		static const std::string GetProjectFileSchemaFilePath();

		/**
		 * \return The path to the XML lint executable.
		 */
		static const std::string GetXmlLintFilePath();

	private:
		LibraryConfiguration();
		LibraryConfiguration(const LibraryConfiguration&);
		void operator=(const LibraryConfiguration&);

		static std::string libResourcePath; /**< Resource directory path */
		static const std::string kDefaultLibResourcePath; /**< Default resource directory path */
		static const std::string kLibResourcePathEnvVar; /**< Resource path environment variable: OPENCONFIGURATOR_LIBRARY_RESOURCEPATH */
		static const std::string kRelXDDSchemaPath; /**< The location of XDD schema path */
		static const std::string kRelProjectFileSchemaPath; /**< The location of project file schema path */
		static const std::string kLibXmlLintPath; /**< Libxml2 XML lint executable path */
};

PROJECT_UPGRADE_CLOSE_NAMESPACE


#endif // _PROJECT_UPGRADE_LIBRARY_CONFIGURATION_H_
