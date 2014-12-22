/*******************************************************************************
* \file   Logging.h
*
* \brief  Header file that includes MACROS and function headers used for 
*         generation log during project upgrade.
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

#ifndef _PROJECT_UPGRADE_LOGGING_H_
#define _PROJECT_UPGRADE_LOGGING_H_

#ifdef _MSC_VER
#pragma warning(push, 0) // Store current warning state and set global warning level 0
#endif

#include <boost/log/trivial.hpp>

#ifdef _MSC_VER
#pragma warning(pop) // Restore previous warning state
#pragma warning (disable:4503 4714) // Boost-Headers themselves disable this warning, reinstate
#endif

#include <string>

#include "Namespace.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

const std::string kProjectUpgradeLogFile = "upgrade.log";

/**
 * \brief Initializes the 'fileName' of the log file generated during project upgrade.
 *
 * \param[in] fileName The name of the log file.
 */
void InitProjectUpgradeLogging(const std::string & fileName);

/**
 * \brief Disables all the sinks registered for logging.
 */
void DisableLogging();

// Custom macros for severity logging
#define LOG_TRACE()\
	BOOST_LOG_TRIVIAL(trace)

#define LOG_DEBUG()\
	BOOST_LOG_TRIVIAL(debug) << __FUNCTION__ << " line:" << __LINE__ << " "

#define LOG_INFO()\
	BOOST_LOG_TRIVIAL(info)

#define LOG_WARN()\
	BOOST_LOG_TRIVIAL(warning)

#define LOG_ERROR()\
	BOOST_LOG_TRIVIAL(error)

#define LOG_FATAL()\
	BOOST_LOG_TRIVIAL(fatal)


PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_LOGGING_H_
