/**
 *****************************************************************************
 \file		Utility.cpp

 \brief		This file contains the definitions that are used as utility functions to support operations such as string manipulation, number to string conversions, etc
 *****************************************************************************
 */

/*
 � Kalycito Infotech Private Limited

 License:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of Kalycito Infotech Private Limited nor the names of
 its contributors may be used to endorse or promote products derived
 from this software without prior written permission. For written
 permission, please contact info@kalycito.com.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 Severability Clause:

 If a provision of this License is or becomes illegal, invalid or
 unenforceable in any jurisdiction, that shall not affect:
 1. the validity or enforceability in that jurisdiction of any other
 provision of this License; or
 2. the validity or enforceability in other jurisdictions of that or
 any other provision of this License.

 ****************************************************************************/

/****************************************************************************/
/* Includes */

#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <sstream>
#include <algorithm>
#include <iomanip>
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/ProcessImage.h"
#include "../Include/Logging.h"
#include "../Include/Result.h"
#include "../Include/BoostShared.h"

using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

char* ConvertToUpper(char* str)
{
	if (!str)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'str'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	UINT32 loopCount = 0;

	for (loopCount = 0; str[loopCount]; ++loopCount)
	{
		str[loopCount] = (char) toupper(str[loopCount]);
	}
	return str;
}

char* Reverse(char* str)
{
	if (!str)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'str'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}
	char* charLeft = str;
	char* charRight = charLeft + strlen(str) - 1;
	char cTemp;

	while (charLeft < charRight)
	{
		cTemp = *charLeft;
		*charLeft++ = *charRight;
		*charRight-- = cTemp;
	}
	return str;
}

char* PadLeft(char* str, char padChar, INT32 padLength)
{
	if (!str)
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'str'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	INT32 iStringLen = strlen(str);

	if (iStringLen < padLength)
	{
		char abTempBuff[50];
		INT32 iLoopCount = 0;
		str = Reverse(str);
		while (iLoopCount < (padLength - iStringLen))
		{
			abTempBuff[iLoopCount] = padChar;
			iLoopCount++;
		}

		abTempBuff[iLoopCount] = '\0';
		strcat(str, abTempBuff);
		str = Reverse(str);
	}
	return str;
}

char* SubString(char* destStr, const char* srcStr, UINT32 startPos, UINT32 len)
{
	if ((NULL != srcStr) && (NULL != destStr))
	{
		if ((startPos + len) > strlen(srcStr))
		{
			LOG_ERROR() << "Invalid function call.";
		}
		else
		{
			strncpy(destStr, (const char*)(srcStr + startPos), len);
			destStr[len] = '\0';
		}
	}
	return destStr;
}

char* UnsignedToAlphaNumeric(unsigned srcValue, char* destStr, INT32 baseValue)
{
	char* alphNum = NULL;

	alphNum = (char*) "0123456789abcdefghijklmnopqrstuvwxyz";
	if (0 == baseValue)
	{
		baseValue = 10;
	}
	if ((NULL == destStr) || (2 > baseValue) || (36 < baseValue))
	{
		return NULL;
	}
	if (srcValue < (unsigned) baseValue)
	{
		destStr[0] = alphNum[srcValue];
		destStr[1] = '\0';
	}
	else
	{
		char* tempBuffer = NULL;
		for (tempBuffer = UnsignedToAlphaNumeric(
		                      srcValue / ((unsigned) baseValue), destStr, baseValue);
		        *tempBuffer; tempBuffer++)
			;
		{
			UnsignedToAlphaNumeric(srcValue % ((unsigned) baseValue),
			                       tempBuffer, baseValue);
		}
	}
	return destStr;

}

char* IntToAscii(LONG srcValue, char* destStr, INT32 baseValue)
{
	char* tempStr = NULL;
	ULONG uiValue;

	tempStr = destStr;
	if (0 == baseValue)
	{
		baseValue = 10;
	}
	if ((NULL == destStr) || (2 > baseValue) || (36 < baseValue))
	{
		return NULL;
	}
	if (srcValue < 0)
	{
		uiValue = -(srcValue);
		//Two's complement of that number
		uiValue = ~uiValue + 1;
	}
	else
	{
		uiValue = srcValue;
	}

	UnsignedToAlphaNumeric(uiValue, tempStr, baseValue);

	return destStr;
}

ULONG HexToInt(const char* hexStr)
{
	ULONG retValue;
	stringstream inputStream;
	inputStream << hex << hexStr;
	inputStream >> retValue;
	return retValue;
}

bool CheckIfNotPDO(const char* indexId)
{
	if ((0 == strncmp(indexId, "14xx", 2)) || (0 == strncmp(indexId, "16xx", 2))
	        || (0 == strncmp(indexId, "18xx", 2))
	        || (0 == strncmp(indexId, "1Axx", 2))
	        || (0 == strncmp(indexId, "1axx", 2)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

bool CheckIfManufactureSpecificObject(const char* indexId)
{
	UINT32 manufactureIndexid;

	manufactureIndexid = HexToInt((char*) "2000");
	if (HexToInt(indexId) >= manufactureIndexid)
	{
		return true;
	}
	else
	{
		return false;
	}
}

bool CheckIfMappingPDO(const char* indexId)
{
	if ((0 == strncmp(indexId, "1A", 2)) || (0 == strncmp(indexId, "16", 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}


string ConvertStringToHex(const string& srcStr)
{
	std::stringstream ss;
	for (unsigned i = 0; i < srcStr.length(); ++i)
	{
		ss << std::hex << unsigned(srcStr.at(i));
	}
	return ss.str();
}

char* ConvertToHexformat(char* hexValue, INT32 padLength, bool doPadding)
{
	char* hexTemp = NULL;

	hexTemp = new char[padLength + 2 + STR_ALLOC_BUFFER];

	strcpy(hexTemp, "0x");

	if (doPadding)
	{
		hexValue = PadLeft(hexValue, '0', padLength);
	}
	strcat(hexTemp, hexValue);

	return hexTemp;
}

bool CheckIfHex(const char* srcStr)
{
	if (NULL == strchr(srcStr, 'x'))
	{
		if (NULL == strchr(srcStr, 'X'))
		{
			return false;
		}
		else
		{
			return true;
		}
	}
	else
	{
		return true;
	}

}

INT32 GetConfigDate(void)
{
	//Time & date are calculated since 1984
	INT32 daysCount = 0;
	INT32 yearsSince = 0;

	time_t rawTime;
	struct tm* timeInfo;

	time(&rawTime);
	timeInfo = localtime(&rawTime);

	yearsSince = ((timeInfo->tm_year) - 84);

	//Except the curent year calculate all the values
	for (INT32 loopCount = 0; loopCount < yearsSince; loopCount++)
	{
		INT32 tempYear = 0;
		INT32 tempDays = 0;
		tempYear = (1984 + loopCount);
		if ((0 == tempYear % 4) && !(0 == tempYear % 100)
		        && (0 != tempYear % 400))
		{
			tempDays = 366;
		}
		else
		{
			tempDays = 365;
		}
		daysCount += tempDays;
	}
	//Calculate the number of days from January 1 of current year
	daysCount += timeInfo->tm_yday;
	// Add one because timeInfo starts with zero
	daysCount++;
	return daysCount;
}

INT32 GetConfigTime(void)
{
	time_t rawtime;
	struct tm* timeInfo;
	INT32 milliSeconds = 0;

	time(&rawtime);
	timeInfo = localtime(&rawtime);

	milliSeconds += (timeInfo->tm_hour) * 3600000;
	milliSeconds += (timeInfo->tm_min) * 60000;
	milliSeconds += (timeInfo->tm_sec) * 1000;

	return milliSeconds;
}


bool CheckAllowedDTForMapping(const char* dataTypeName)
{
	bool retVal = false;
	if (NULL != dataTypeName)
	{
		char* dtNameUpper = new char[strlen(dataTypeName) + STR_ALLOC_BUFFER];
		strcpy(dtNameUpper, dataTypeName);
		dtNameUpper = ConvertToUpper(dtNameUpper);

		if (0 == strcmp(dtNameUpper, "INTEGER8")
		        || 0 == strcmp(dtNameUpper, "INTEGER16")
		        || 0 == strcmp(dtNameUpper, "INTEGER32")
		        || 0 == strcmp(dtNameUpper, "INTEGER64")
		        || 0 == strcmp(dtNameUpper, "UNSIGNED8")
		        || 0 == strcmp(dtNameUpper, "UNSIGNED16")
		        || 0 == strcmp(dtNameUpper, "UNSIGNED32")
		        || 0 == strcmp(dtNameUpper, "UNSIGNED64"))
		{
			retVal = true;
		}
		else
		{
			retVal = false;
		}
		delete[] dtNameUpper;
	}
	else
	{
		retVal = false;
	}
	return retVal;
}

//TODO: Add a parameter to return the value to avoid new delete memory issues
char* GetLastAvailableCycleNumber(void)
{
	char* retForcedCycleValue = new char[20];
	ocfmRetCode retCode;
	INT32 indexPos;
	INT32 subIndexPos;
	retCode = IfSubIndexExists(240, MN, (char*) "1F98", (char*) "07",
	                           &subIndexPos, &indexPos);
	if (OCFM_ERR_SUCCESS != retCode.getErrorCode())
	{
		strcpy(retForcedCycleValue, "");
		return retForcedCycleValue;
	}
	char* actValue = new char[20];
	retCode = GetSubIndexAttributes(240, MN, (char*) "1F98", (char*) "07",
	                                ACTUALVALUE, actValue);
	UINT32 tempCycleNumber = cycleNumberGlobal;
	if (OCFM_ERR_SUCCESS != retCode.getErrorCode())
	{

		UINT32 cycleValue;
		if (0 == strncmp(actValue, "0x", 2) || 0 == strncmp(actValue, "0X", 2))
		{
			char* tempCycleVal = new char[strlen(actValue)];
			SubString(tempCycleVal, (const char*)actValue, 2, strlen(actValue) - 2);
			cycleValue = (UINT32) HexToInt(tempCycleVal);
			delete[] tempCycleVal;
		}
		else
			cycleValue = atoi(actValue);

		if (cycleValue > tempCycleNumber + 1)
		{
			tempCycleNumber += 1;
		}
		else
		{
			tempCycleNumber += 1;
			while (cycleValue < tempCycleNumber)
			{
				tempCycleNumber = tempCycleNumber - cycleValue;
			}

		}

	}
	delete[] actValue;

	UINT32 freeCycleNumber = GetFreeCycleNumber(tempCycleNumber);
	if (freeCycleNumber == tempCycleNumber)
	{
		cycleNumberGlobal = tempCycleNumber;
		retForcedCycleValue = IntToAscii(tempCycleNumber, retForcedCycleValue,
		                                 16);
	}
	else
	{
		retForcedCycleValue = IntToAscii(freeCycleNumber, retForcedCycleValue,
		                                 16);
	}
	return retForcedCycleValue;
}

void CheckAndCorrectName(char* srcStr)
{
	if (NULL == srcStr)
	{
		return;
	}

	UINT32 srcStrLen = strlen(srcStr);
	for (UINT32 loopCount = 0; loopCount < srcStrLen; loopCount++)
	{
		if ((48 <= srcStr[loopCount] && 57 >= srcStr[loopCount])
		        || (65 <= srcStr[loopCount] && 90 >= srcStr[loopCount])
		        || (97 <= srcStr[loopCount] && 122 >= srcStr[loopCount])
		        || 95 == srcStr[loopCount])
		{
		}
		else
		{
			srcStr[loopCount] = '_';
		}
	}
}

bool CheckIfValueZero(const char* srcStr)
{
	if (NULL == srcStr || 0 == strcmp(srcStr, ""))
	{
		return false;
	}

	INT32 srcValue = 0;
	if (0 == strncmp(srcStr, "0x", 2) || 0 == strncmp(srcStr, "0X", 2))
	{
		char* tempVal = new char[strlen(srcStr)];
		SubString(tempVal, (const char*) srcStr, 2, strlen(srcStr) - 2);
		srcValue = HexToInt(tempVal);
		delete[] tempVal;
	}
	else
	{
		srcValue = atoi(srcStr);
	}

	if (0 == srcValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

INT32 GetDecimalValue(const char* srcStr)
{
	if (NULL == srcStr || 0 == strcmp(srcStr, ""))
	{
		return 0;
	}

	INT32 srcValue = 0;
	if (0 == strncmp(srcStr, "0x", 2) || 0 == strncmp(srcStr, "0X", 2))
	{
		char* tempVal = new char[strlen(srcStr)];
		SubString(tempVal, (const char*) srcStr, 2, strlen(srcStr) - 2);
		srcValue = (INT32) HexToInt(tempVal);
		delete[] tempVal;
	}
	else
	{
		srcValue = atoi(srcStr);
	}

	return srcValue;
}

bool CheckAccessTypeForInclude(const char* accessType)
{
	bool retInclude;
	if (NULL == accessType)
	{
		//TODO: To check with the functionality. Return true or false?
		retInclude = true;
	}
	else
	{
		char* tempAccesstype = new char[strlen(accessType) + STR_ALLOC_BUFFER];
		strcpy(tempAccesstype, accessType);
		tempAccesstype = ConvertToUpper(tempAccesstype);
		if ((0 == strcmp(tempAccesstype, "CONST"))
		        || (0 == strcmp(tempAccesstype, "RO")))
		{
			retInclude = false;
		}
		else
		{
			retInclude = true;
		}
		delete[] tempAccesstype;
	}
	return retInclude;
}

bool CheckToolVersion(char* currentToolVersion)
{
	char* tempToolVersion = ConvertToUpper(currentToolVersion);
	if ((0 == strcmp(tempToolVersion, TOOL_VERSION))
	        || (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_1))
	        || (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_2))
	        || (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_3))
	        || (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_4))
	        || (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_5))
	        || (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_6))
	        || (0 == strcmp(tempToolVersion, PREV_TOOL_VERSION_7))
	        || (0 == strcmp(tempToolVersion, LAST_TOOL_VERSION))
	        || (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_1))
	        || (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_2))
	        || (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_3))
	        || (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_4))
	        || (0 == strcmp(tempToolVersion, TOOL_INTER_REL_VERSION_5)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

template <typename T>
string openCONFIGURATOR::Library::Utilities::IntToHex(const T number, const unsigned int padLength, const string& prefix, const string& suffix)
{
	ostringstream hexStream;
	hexStream << std::setfill('0')
	          << std::setw(padLength)
	          << std::hex
	          << std::uppercase
	          << number
	          << suffix;
	return (prefix + hexStream.str());
}

// Explicit instantiation of the template for required types
template string openCONFIGURATOR::Library::Utilities::IntToHex<unsigned short>(const unsigned short number, const unsigned int padLength, const string& prefix, const string& suffix);
template string openCONFIGURATOR::Library::Utilities::IntToHex<unsigned int>(const unsigned int number, const unsigned int padLength, const string& prefix, const string& suffix);
template string openCONFIGURATOR::Library::Utilities::IntToHex<unsigned long>(const unsigned long number, const unsigned int padLength, const string& prefix, const string& suffix);
template string openCONFIGURATOR::Library::Utilities::IntToHex<unsigned long long>(const unsigned long long number, const unsigned int padLength, const string& prefix, const string& suffix);

template <typename T>
T openCONFIGURATOR::Library::Utilities::HexToInt(const string& hexString)
{
	// Strip prefix if necessary
	string valueStr = (hexString.substr(0, 2) == "0x")
	                  ? hexString.substr(2)
	                  : hexString;
	stringstream stream;
	T value = 0;
	stream << std::hex << valueStr;
	stream >> value;
	return value;
}

// Explicit instantiation of the template for required types
template unsigned short openCONFIGURATOR::Library::Utilities::HexToInt<unsigned short>(const string& hexString);
template unsigned int openCONFIGURATOR::Library::Utilities::HexToInt<unsigned int>(const string& hexString);
template unsigned long openCONFIGURATOR::Library::Utilities::HexToInt<unsigned long>(const string& hexString);
template unsigned long long openCONFIGURATOR::Library::Utilities::HexToInt<unsigned long long>(const string& hexString);
const std::string GetCurrentDateTime() {
    time_t     now = time(0);
    struct tm  tstruct;
    char       buf[80];
    tstruct = *localtime(&now);
    strftime(buf, sizeof(buf), "%Y-%m-%dT%X", &tstruct);
    return buf;
}

string openCONFIGURATOR::Library::Utilities::url_encode(const string &value) {
    ostringstream escaped;
    escaped.fill('0');
    escaped << hex;

    for (string::const_iterator i = value.begin(), n = value.end(); i != n; ++i) {
        string::value_type c = (*i);

        if (c == ' ') {
            escaped << '%' << setw(2) << int((unsigned char) c);
            continue;
        }

        escaped << c;
    }

    return escaped.str();
}
