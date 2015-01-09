/*******************************************************************************
* \file   Result.h
*
* \brief  Header file that includes functions to retrieve the result of a
*         function execution.
*
*    This class is meant to be used as the return type of a function which needs
*    to return error codes and error context information (like an error message).
*    Furthermore, this class shall be used as return type for all API functions
*    that return error codes.
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

#ifndef _PROJECT_UPGRADE_RESULT_H_
#define _PROJECT_UPGRADE_RESULT_H_

#include <string>

#include "Namespace.h"
#include "ErrorCode.h"
#include "Exports.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

const std::string kMsgFileReadFailed = "Failed to read from the file '%s'.";
const std::string kMsgFileWriteFailed = "Failed to write to the file '%s'.";
const std::string kMsgEmptyArgument = "Argument(s) %s must not be empty.";

const std::string kMsgNoProjectLoaded = "No openCONFIGURATOR project loaded in the library.";
const std::string kMsgProjectVersionDoesNotExist = "No project version exists in the openCONFIGUGRATOR project.";
const std::string kMsgProjectVersionInvalid = "Project version '%s' is invalid.";
const std::string kMsgInvalidProjectFileExtension = "The project file extension '%s' is invalid. Possible extensions are '%s' and '%s'.";
const std::string kMsgInvalidProjectFile = "The project file '%s' is invalid.";
const std::string kMsgAutoTagDoesNotExist = "Auto tag (which is the child of Profile) does not exist in the openCONFIGUGRATOR project.";
const std::string kMsgNodeTagDoesNotExist = "The Node tag does not exist in the openCONFIGUGRATOR project.";

const std::string kMsgSchemaNotFound = "XML schema file '%s' cannot be loaded.";
const std::string kMsgSchemaParserContextError = "LibXML2 cannot create a parser context for the XML schema file '%s'.";
const std::string kMsgSchemaValidationContextError = "LibXML2 cannot create a validation context for the XML schema file '%s'.";
const std::string kMsgSchemaInvalid = "XML schema file '%s' is not valid.";

const std::string kMsgXddTagDoesNotExist = "The tag '%s' does not exist in the file '%s'.";

const std::string kMsgNonExistingNode = "Node with id %d does not exist.";

/** \addtogroup projectupgradeapireturn
* @{
*/

/**
 * \brief Result returned by all functions of the ProjectUpgrade API.
 *			Contains an ErrorCode and a descriptive error string.
 */
class PROJECT_UPGRADE_DLLEXPORT Result
{
	private:
		openCONFIGURATOR::ProjectUpgrade::ErrorCode::ErrorCode errorCode;
		std::string errorString;

	public:
		Result(openCONFIGURATOR::ProjectUpgrade::ErrorCode::ErrorCode errorCode, const std::string errorString);
		explicit Result(openCONFIGURATOR::ProjectUpgrade::ErrorCode::ErrorCode errorCode = openCONFIGURATOR::ProjectUpgrade::ErrorCode::SUCCESS);

		openCONFIGURATOR::ProjectUpgrade::ErrorCode::ErrorCode GetErrorCode() const;
		const std::string& GetErrorString() const;
		const char * what() const; // Exception compatibility

		/**
		 * \return <code>True</code> if operation was successful, <code>False</code> otherwise.
		*/
		bool IsSuccessful() const;
};

/** @} */

PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_RESULT_H_
