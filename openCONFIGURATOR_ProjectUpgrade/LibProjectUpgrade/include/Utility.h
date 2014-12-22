/*******************************************************************************
* \file   Utility.h
*
* \brief  Header file that includes the utility functions of project upgrade library.
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

#ifndef _PROJECT_UPGRADE_UTILITY_H_
#define _PROJECT_UPGRADE_UTILITY_H_

#include <string>

#include "Namespace.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

/**
 * List of date time formats as specified in the XML standards.
 * Refer to www.w3.org
 */
namespace XSD_DATE_TIME_TYPE
{
	enum XSD_DATE_TIME_TYPE
	{
		DURATION,
		DATE_TIME,
		TIME,
		DATE,
		G_YEAR_MONTH,
		G_YEAR,
		G_MONTH_DAY,
		G_DAY,
		G_MONTH
	};
}

namespace Utility
{

/**
 * \param dateTimeType Type of the date time.
 * \return The date time string based on the dateTimeType.
 */
const std::string GetXsdDateTime(XSD_DATE_TIME_TYPE::XSD_DATE_TIME_TYPE dateTimeType);

/**
 * \param value The value to be encoded.
 * \return The encoded value.
 */
const std::string EncodeUrl(const std::string & value);

} //Utility

PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_UTILITY_H_
