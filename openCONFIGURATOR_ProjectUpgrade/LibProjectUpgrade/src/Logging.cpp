/*******************************************************************************
* \file   Logging.cpp
*
* \brief  Function definitions for logging functionality.
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

#include "../include/Logging.h"

#ifdef _MSC_VER
#pragma warning(push, 0) // Store current warning state and set global warning level 0
#endif

#include <boost/log/core.hpp>
#include <boost/log/expressions.hpp>
#include <boost/log/utility/setup/common_attributes.hpp>
#include <boost/log/attributes/clock.hpp>

#ifdef _MSC_VER
#pragma warning(pop) // Restore previous warning state
#pragma warning (disable:4503 4714) // Boost-Headers themselves disable this warning, reinstate
#endif

// using namespace boost::log;

PROJECT_UPGRADE_OPEN_NAMESPACE

ProjectUpgradeLogging ProjectUpgradeLogging::instance;

ProjectUpgradeLogging::ProjectUpgradeLogging() : oldStateOfBoostLogger(false)
{

}

ProjectUpgradeLogging& ProjectUpgradeLogging::GetInstance()
{
	return ProjectUpgradeLogging::instance;
}

Result ProjectUpgradeLogging::InitProjectUpgradeLogging(const std::string & fileName)
{
	try
	{
		ProjectUpgradeLogging::GetInstance().oldStateOfBoostLogger = boost::log::core::get()->get_logging_enabled();

		boost::log::core::get()->set_logging_enabled(true);

		boost::log::register_simple_formatter_factory< boost::log::trivial::severity_level, char >("Severity");
		ProjectUpgradeLogging::GetInstance().logFileSink = boost::log::add_file_log(
																boost::log::keywords::file_name = fileName,
																boost::log::keywords::format = "[%TimeStamp%] [%Severity%]	%Message%",
																boost::log::keywords::auto_flush = true,
																boost::log::keywords::open_mode = std::ios_base::ate | std::ios::out
																);
	#ifndef DEBUG
		boost::log::core::get()->set_filter(boost::log::trivial::severity >= boost::log::trivial::info);
	#endif

		// boost::log::add_common_attributes();

		boost::log::core::get()->add_global_attribute(
					boost::log::aux::default_attribute_names::timestamp(),
					boost::log::attributes::local_clock());
	}
	catch (const std::exception& ex)
	{
		return Result(ErrorCode::INIT_LOGGING_ERROR, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

Result ProjectUpgradeLogging::DisableProjectUpgradeLogging()
{
	try
	{
		boost::log::core::get()->flush();

		// Remove the project upgrade log sink from the Logging core.
		boost::log::core::get()->remove_sink(ProjectUpgradeLogging::GetInstance().logFileSink);

		// Reset the logger to its original state.
		boost::log::core::get()->set_logging_enabled(ProjectUpgradeLogging::GetInstance().oldStateOfBoostLogger);
	}
	catch (const std::exception& ex)
	{
		return Result(ErrorCode::DISABLE_LOGGING_ERROR, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

PROJECT_UPGRADE_CLOSE_NAMESPACE
