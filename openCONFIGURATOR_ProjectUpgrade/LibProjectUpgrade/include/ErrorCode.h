/*******************************************************************************
* \file   ErrorCode.h
*
* \brief  Header file that includes the ERROR codes used in this project.
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

#ifndef _PROJECT_UPGRADE_ERRORCODE_H_
#define _PROJECT_UPGRADE_ERRORCODE_H_

#include "Exports.h"
#include "Namespace.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

/** \addtogroup projectupgradeapireturn
* @{
*/
namespace ErrorCode
{
	/**
	 * \brief Error codes used in the openCONFIGURATOR project upgrade library
	 *
	 */
	enum  ErrorCode
	{
		SUCCESS = 0, /**< Operation completed successfully. */
		UNHANDLED_EXCEPTION, /**< Unhandled exception occurred during operation. */
		INVALID_PARAMETER,

		FILE_READ_FAILED, /**< Cannot read from file. */
		FILE_WRITE_FAILED, /**< Cannot write from file. */

		NO_PROJECT_LOADED, /**< No project is loaded into the memory. */
		PROJECT_VERSION_DOES_NOT_EXIST, /**< Project version does not exist. */
		PROJECT_VERSION_INVALID, /**< Invalid project version. */
		AUTO_TAG_DOES_NOT_EXIST, /**< Auto tag (which is the children of Profile) does not exist. */
		NODE_TAG_DOES_NOT_EXIST, /**< The Node tag does not exist. */
		PROJECT_FILE_INVALID_EXTENSION, /**< The project file has an invalid extension. */
		PROJECT_FILE_INVALID, /**< Invalid project file. */

		/**< XDD schema related error codes */
		XDD_SCHEMA_NOT_FOUND,
		XDD_SCHEMA_PARSER_CONTEXT_ERROR,
		XDD_SCHEMA_VALIDATION_CONTEXT_ERROR,
		XDD_SCHEMA_NOT_VALID,
		XDD_SCHEMA_VALIDATION_FAILED,
		XML_PARSING_ERROR,
		XML_FORMATING_FAILED,

		FILE_SYSTEM_ERROR, /**< File system error. */

		/**< Logging errors */
		INIT_LOGGING_ERROR,
		DISABLE_LOGGING_ERROR,

		XDD_TAG_DOES_NOT_EXIST, /**< Specified tag does not exist in the XML */

		NODE_ID_DOES_NOT_EXIST /**< Specified node ID does not exist in the project. */
	};
}

/** @} */

PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_ERRORCODE_H_
