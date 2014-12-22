/*******************************************************************************
* \file   Utility.cpp
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

#include "../include/Utility.h"

#include <iostream>
#include <sstream>
#include <iomanip>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/date_time/c_local_time_adjustor.hpp>

using namespace boost::posix_time;

PROJECT_UPGRADE_OPEN_NAMESPACE

namespace Utility
{

	// Use extended ISO format for time zone offset.
	boost::posix_time::time_duration GetUtcTimeZoneOffset();

	std::string GetUtcTimeZoneOffsetString();

	const std::string GetUtcDateTime(const std::string & format, bool withTimeZone = false);

	const std::string GetXsdDateTime(XSD_DATE_TIME_TYPE::XSD_DATE_TIME_TYPE dateTimeType)
	{
		switch (dateTimeType)
		{
		case XSD_DATE_TIME_TYPE::DURATION:
			// Not implemented.
			break;
		case XSD_DATE_TIME_TYPE::DATE_TIME: // 2014-12-02T12:43:50
			return GetUtcDateTime("%Y-%m-%dT%H:%M:%S");
			break;
		case XSD_DATE_TIME_TYPE::TIME: // 07:07:29+05:30
			return GetUtcDateTime("%H:%M:%S", true);
			break;
		case XSD_DATE_TIME_TYPE::DATE: // 2014-12-03
			return GetUtcDateTime("%Y-%m-%d");
			break;
		case XSD_DATE_TIME_TYPE::G_YEAR_MONTH: // 2014-12
			return GetUtcDateTime("%Y-%m");
			break;
		case XSD_DATE_TIME_TYPE::G_YEAR: // 2014
			return GetUtcDateTime("%Y");
			break;
		case XSD_DATE_TIME_TYPE::G_MONTH_DAY: // MM-DD
			return GetUtcDateTime("%m-%d");
			break;
		case XSD_DATE_TIME_TYPE::G_DAY: // DD
			return GetUtcDateTime("%d");
			break;
		case XSD_DATE_TIME_TYPE::G_MONTH: // MM
			return GetUtcDateTime("%m");
			break;
		default:
			std::cout << __FUNCTION__ << " Default case. value:" << dateTimeType << std::endl;
		}
		return "";
	}

	const std::string EncodeUrl(const std::string & value)
	{
		std::ostringstream escaped;
		escaped.fill('0');
		escaped << std::hex;

		for (std::string::const_iterator i = value.begin(), n = value.end();
			 i != n;
			 ++i)
		{
			std::string::value_type c = (*i);

			if (c == ' ')
			{
				escaped << '%' << std::setw(2) << int((unsigned char) c);
				continue;
			}

			escaped << c;
		}

		return escaped.str();
	}

	const std::string GetUtcDateTime(const std::string & format, bool withTimeZone)
	{
		const ptime now = second_clock::universal_time();
		std::locale loc(std::cout.getloc(),
						new time_facet(format.c_str()));

		std::stringstream ss;
		ss.imbue(loc);
		ss << now;

		if (withTimeZone)
		{
			ss << GetUtcTimeZoneOffsetString();
		}
		return ss.str();
	}

	std::string GetUtcTimeZoneOffsetString() {
		std::stringstream out;

		time_facet* tf = new time_facet();
		tf->time_duration_format("%+%H:%M");
		out.imbue(std::locale(out.getloc(), tf));

		out << GetUtcTimeZoneOffset();

		return out.str();
	}

	boost::posix_time::time_duration GetUtcTimeZoneOffset() {
		// boost::date_time::c_local_adjustor uses the C-API to adjust a
		// moment given in utc to the same moment in the local time zone.
		const ptime utc_now = second_clock::universal_time();
		const ptime now = boost::date_time::c_local_adjustor<ptime>::utc_to_local(utc_now);

		return now - utc_now;
	}

} // Utility

PROJECT_UPGRADE_CLOSE_NAMESPACE
