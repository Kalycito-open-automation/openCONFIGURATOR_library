/*//////////////////////////////////////////////////////////////////////////////////////////////
 //
 //  $Source: $
 //
 // NAME:  Utility.cpp
 //
 // BASE  CLASSES: none
 //  
 // PURPOSE:  purpose description
 //
 // AUTHOR:  Kalycito Powerlink Team
 //
 //  COPYRIGHT NOTICE:
 //
 //	****************************************************************************

 // (c) Kalycito Infotech Private Limited
 //
 //  Project:      openCONFIGURATOR 
 //
 //  Description:  
 //
 //
 //  License:
 //
 //    Redistribution and use in source and binary forms, with or without
 //    modification, are permitted provided that the following conditions
 //    are met:
 //
 //    1. Redistributions of source code must retain the above copyright
 //       notice, this list of conditions and the following disclaimer.
 //
 //    2. Redistributions in binary form must reproduce the above copyright
 //       notice, this list of conditions and the following disclaimer in the
 //       documentation and/or other materials provided with the distribution.
 //
 //    3. Neither the name of Kalycito Infotech Private Limited nor the names of 
 //       its contributors may be used to endorse or promote products derived
 //       from this software without prior written permission. For written
 //       permission, please contact info@kalycito.com.
 //
 //    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 //    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 //    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 //    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 //    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 //    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 //    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 //    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 //    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 //    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 //    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 //    POSSIBILITY OF SUCH DAMAGE.
 //
 //    Severability Clause:
 //
 //        If a provision of this License is or becomes illegal, invalid or
 //        unenforceable in any jurisdiction, that shall not affect:
 //        1. the validity or enforceability in that jurisdiction of any other
 //           provision of this License; or
 //        2. the validity or enforceability in other jurisdictions of that or
 //           any other provision of this License.
 //
 ***********************************************************************************************/
//
//  REVISION HISTORY:
// $Log:      $
//////////////////////////////////////////////////////////////////////////////////////////////
/****************************************************************************************************
 * Includes
 ****************************************************************************************************/
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/ProcessImage.h"

using namespace std;

/****************************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************************/

/****************************************************************************************************
 * Function Name: ConvertToUpper
 * Description: This function converts received string to upper case
 * Return value: char*
 ****************************************************************************************************/
char* ConvertToUpper(char* pbStrBuffer)
{
	if (NULL == pbStrBuffer)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	UINT32 uiLoopCount = 0;

	for (uiLoopCount = 0; pbStrBuffer[uiLoopCount]; ++uiLoopCount)
	{
		pbStrBuffer[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);
	}
	return pbStrBuffer;
}

/****************************************************************************************************
 * Function Name: StringToUpper
 * Description: This function checks string length,allocate buffer and covert the string to upper case
 * Return value: char*
 ****************************************************************************************************/
//TODO: To add a parameter for destination to avoid new delete memory leak.
char* StringToUpper(char* pbStrBuffer)
{
	if (NULL == pbStrBuffer)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	UINT32 uiLoopCount;
	UINT32 uiStringLen = strlen(pbStrBuffer);

	char* pbStrUpp = new char[uiStringLen + STR_ALLOC_BUFFER];

	for (uiLoopCount = 0; uiLoopCount < uiStringLen; ++uiLoopCount)
	{
		pbStrUpp[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);
	}

	pbStrUpp[uiLoopCount] = '\0';
	return pbStrUpp;
}

/****************************************************************************************************
 * Function Name: reverse
 * Description: This function reverses the received string
 * Return value: char*
 ****************************************************************************************************/
char* reverse(char* pbStrBuffer)
{
	if (NULL == pbStrBuffer)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char* pbCharLeft = pbStrBuffer;
	char* pbCharRight = pbCharLeft + strlen(pbStrBuffer) - 1;
	char cTemp;

	while (pbCharLeft < pbCharRight)
	{
		cTemp = *pbCharLeft;
		*pbCharLeft++ = *pbCharRight;
		*pbCharRight-- = cTemp;
	}
	return pbStrBuffer;
}

/****************************************************************************************************
 * Function Name: padLeft
 * Description: This function pads left of the string with 0's
 * Return value: char*
 ****************************************************************************************************/
char* padLeft(char* pbStrBuffer, char cPadChar, INT32 iPadLength)
{
	if (NULL == pbStrBuffer)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	INT32 iStringLen = strlen(pbStrBuffer);

	if (iStringLen < iPadLength)
	{
		char abTempBuff[50];
		INT32 iLoopCount = 0;
		pbStrBuffer = reverse(pbStrBuffer);
		while (iLoopCount < (iPadLength - iStringLen))
		{
			abTempBuff[iLoopCount] = cPadChar;
			iLoopCount++;
		}

		abTempBuff[iLoopCount] = '\0';
		strcat(pbStrBuffer, abTempBuff);
		pbStrBuffer = reverse(pbStrBuffer);
	}
	return pbStrBuffer;
}

/****************************************************************************************************
 * Function Name: subString
 * Description:   This function returns substring
 * Return value: char*
 ****************************************************************************************************/
//TODO: Another parameter to be added for substring to avoid new delete memory issues.
char* subString(char* pbStrBuffer, INT32 iStartPos, INT32 iStringLen)
{
	if (NULL == pbStrBuffer)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	char* pbSubStrBuffer = NULL;

	pbSubStrBuffer = new char[iStringLen + 1];
	strncpy(pbSubStrBuffer, (const char*) (pbStrBuffer + iStartPos),
			iStringLen);
	pbSubStrBuffer[iStringLen] = '\0';
	return pbSubStrBuffer;
}

/****************************************************************************************************
 * Function Name: utoa
 * Description: This function converts unsigned values to alphanumeric values
 * Return value: char * / NULL
 ****************************************************************************************************/
char *utoa(unsigned uValue, char *pbDigits, INT32 iBaseValue)
{
	char *pbAlphNum = NULL;

	pbAlphNum = (char *) "0123456789abcdefghijklmnopqrstuvwxyz";
	if (0 == iBaseValue)
	{
		iBaseValue = 10;
	}
	if ((NULL == pbDigits) || (2 > iBaseValue) || (36 < iBaseValue))
	{
		return NULL;
	}
	if (uValue < (unsigned) iBaseValue)
	{
		pbDigits[0] = pbAlphNum[uValue];
		pbDigits[1] = '\0';
	}
	else
	{
		char *pbTempBuffer = NULL;
		for (pbTempBuffer = utoa(uValue / ((unsigned) iBaseValue), pbDigits,
				iBaseValue); *pbTempBuffer; pbTempBuffer++)
			;
		{
			utoa(uValue % ((unsigned) iBaseValue), pbTempBuffer, iBaseValue);
		}
	}
	return pbDigits;

}

/****************************************************************************************************
 * Function Name: _IntToAscii
 * Description: This function converts INT values to ASCII value
 * Return value: char * / NULL
 ****************************************************************************************************/
char* _IntToAscii(LONG iValue, char *pbDigits, INT32 iBaseValue)
{
	char *pbDigit = NULL;
	ULONG uiValue; /* assume unsigned is big enough to hold all the
	 * unsigned values -x could possibly be -- don't
	 * know how well this assumption holds on the
	 * DeathStation 9000, so beware of nasal demons
	 */

	pbDigit = pbDigits;
	if (0 == iBaseValue)
	{
		iBaseValue = 10;
	}
	if ((NULL == pbDigits) || (2 > iBaseValue) || (36 < iBaseValue))
	{
		return NULL;
	}
	if (iValue < 0)
	{
		uiValue = -(iValue);
		uiValue = ~uiValue + 1; // two's complement
	}
	else
	{
		uiValue = iValue;
	}

	utoa(uiValue, pbDigit, iBaseValue);

	return pbDigits;
}

/**************************************************************************************************
 * Function Name: hex2int
 * Description: This function converts hex values to int values
 * Return value: unsigned long
 ***************************************************************************************************/
ULONG hex2int(char *pbStrBuffer)
{
	UINT32 uiLoopCount = 0;
	UINT32 uiValue = 0;
	UINT32 uiStrLen = strlen(pbStrBuffer);

	for (uiLoopCount = 0; uiLoopCount < uiStrLen; uiLoopCount++)
	{
		if (IsAscii(pbStrBuffer[uiLoopCount]))
		{
			pbStrBuffer[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);
		}
		if (pbStrBuffer[uiLoopCount] <= 57)
		{
			uiValue += (pbStrBuffer[uiLoopCount] - 48)
					* (1 << (4 * ((uiStrLen - 1) - uiLoopCount)));
		}
		else
		{
			uiValue += (pbStrBuffer[uiLoopCount] - 55)
					* (1 << (4 * ((uiStrLen - 1) - uiLoopCount)));
		}
	}
	return uiValue;
}

/**************************************************************************************************
 * Function Name: IsAscii
 * Description: This function checks whether the given character is ascii
 * Return value: bool
 ***************************************************************************************************/
bool IsAscii(char cArg)
{
	UINT32 uiCount;

	uiCount = cArg;

	if (48 <= uiCount && 57 >= uiCount)
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**************************************************************************************************
 * Function Name: CheckIfNotPDO
 * Description: This function checks whether the given index value is PDO or not
 * Return value: bool
 ***************************************************************************************************/
bool CheckIfNotPDO(char* pbIndex)
{
	if ((0 == strncmp(pbIndex, "14xx", 2)) || (0 == strncmp(pbIndex, "16xx", 2))
			|| (0 == strncmp(pbIndex, "18xx", 2))
			|| (0 == strncmp(pbIndex, "1Axx", 2)))
	{
		return false;
	}
	else
	{
		return true;
	}
}

/**************************************************************************************************
 * Function Name: CheckIfManufactureSpecificObject
 * Description: This function checks whether the device index is manufacturer spcific or not
 * Return value: bool
 ***************************************************************************************************/
bool CheckIfManufactureSpecificObject(char* pbIndex)
{
	UINT32 uiDeviceIndex;

	uiDeviceIndex = hex2int((char *) "2000");
	if (hex2int(pbIndex) >= uiDeviceIndex)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
 * Function Name: CheckIfMappingPDO
 * Description: This function checks index for PDO mapping
 * Return value: bool
 ***************************************************************************************************/
bool CheckIfMappingPDO(char* pbIndex)
{
	if ((0 == strncmp(pbIndex, "1A", 2)) || (0 == strncmp(pbIndex, "16", 2)))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
 * Function Name: CheckAllowedCNIndexes
 * Description: This function checks whether received CN index falls in the allowed CN indexs
 * Return value: bool
 ***************************************************************************************************/
//TODO: Function is not used.
bool CheckAllowedCNIndexes(char* pbIndexValue)
{
	if ((false == CheckIfNotPDO((char*) pbIndexValue))
			|| CheckIfManufactureSpecificObject((char*) pbIndexValue)
			|| (0 == strcmp(pbIndexValue, "1F98"))
			|| (0 == strcmp(pbIndexValue, "1020"))
			|| (0 == strcmp(pbIndexValue, "1F9B"))
			|| (0 == strcmp(pbIndexValue, "1F81"))
			|| (0 == strcmp(pbIndexValue, "1006")))
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
 * Function Name: CheckBlockedMNIndexes
 * Description  : This function checks whether received MN index is blocked or not
 * Return value: bool
 ***************************************************************************************************/
//TODO: function is not used.
bool CheckBlockedMNIndexes(char* pbIndexValue)
{
	if (0 == strcmp(pbIndexValue, "1F81"))
	{
		return true;
	}
	else
	{
		return false;
	}
}
/**************************************************************************************************
 * Function Name: ConvertStringToHex
 * Description: Converts the string to hexadecimal for strings of string Datatype
 * Return value: char*
 ***************************************************************************************************/
char* ConvertStringToHex(char* actString)
{
	INT32 ilen = 0;

	if (NULL == actString)
	{
		ocfmException objException;
		objException.ocfm_Excpetion(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER:" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	ilen = strlen(actString);
	char* actStringBuffer = new char[ilen + 1];
	char* tempbufHex = new char[(ilen * 2) + 1];
	if ((NULL == actStringBuffer) || (NULL == tempbufHex))
	{
#if defined DEBUG
		cout << "Memory allocation error" << endl;
#endif

		ocfmException ex;
		ex.ocfm_Excpetion(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		throw ex;
	}
	strcpy(actStringBuffer, (char*) actString);
	strcpy(actString, "");
	for (INT32 iloopCntr = 0; iloopCntr < ilen; iloopCntr++)
	{
		sprintf(tempbufHex, "%X", actStringBuffer[iloopCntr]);
		strcat(actString, (char*) tempbufHex);
	}
	delete[] actStringBuffer;
	delete[] tempbufHex;
	return actString;
}
/**************************************************************************************************
 * Function Name: ConvertToHexformat
 * Description:   This function converts to hex value
 * Return value: char*
 ***************************************************************************************************/
char* ConvertToHexformat(char* pbHexValue, INT32 iPadLength, bool bDoPadding)
{
	char* pbHexTemp = NULL;

	pbHexTemp = new char[iPadLength + 2 + STR_ALLOC_BUFFER];

	strcpy(pbHexTemp, "0x");

	if (bDoPadding)
	{
		pbHexValue = padLeft(pbHexValue, '0', iPadLength);
	}
	strcat(pbHexTemp, pbHexValue);

	return pbHexTemp;
}

/**************************************************************************************************
 * Function Name: CheckIfHex
 * Description: This function checks whether hex or not
 * Return value: bool
 ***************************************************************************************************/
bool CheckIfHex(char* pbValue)
{
	if (NULL == strchr(pbValue, 'x'))
	{
		if (NULL == strchr(pbValue, 'X'))
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

/**************************************************************************************************
 * Function Name: getConfigDate
 * Description: This function returns configuration date
 * Return value: INT32
 ***************************************************************************************************/
INT32 getConfigDate()
{
	// to have the total number of days Since 1984
	INT32 iDays = 0;
	// to have the total number of years Since 1984  
	INT32 iYearSince = 0;
	// to have the current Year
	//  INT32 iCurrentYear = 0;
	time_t rawtime;
	struct tm *pstTimeInfo;

	time(&rawtime);
	pstTimeInfo = localtime(&rawtime);

	iYearSince = ((pstTimeInfo->tm_year) - 84);
	//  iCurrentYear = ((pstTimeInfo->tm_year) + 1900);

	// Loops thru all the years except the curent year
	for (INT32 iLoopCount = 0; iLoopCount < iYearSince; iLoopCount++)
	{
		INT32 iTmpYear = 0;
		INT32 iTmpDays = 0;
		iTmpYear = (1984 + iLoopCount);
		if ((0 == iTmpYear % 4) && !(0 == iTmpYear % 100)
				&& (0 != iTmpYear % 400))
		{
			iTmpDays = 366;
		}
		else
		{
			iTmpDays = 365;
		}
		iDays += iTmpDays;
	}
	// To calculate number of days since Jan 1 of current year
	iDays += pstTimeInfo->tm_yday;
	return iDays;
}

/**************************************************************************************************
 * Function Name: getConfigTime
 * Description: This function returns configuration time
 * Return value: INT32
 ***************************************************************************************************/
INT32 getConfigTime()
{
	time_t rawtime;
	struct tm *pstTimeInfo;
	INT32 iMSeconds = 0;

	time(&rawtime);
	pstTimeInfo = localtime(&rawtime);

	iMSeconds += (pstTimeInfo->tm_hour) * 3600000;
	iMSeconds += (pstTimeInfo->tm_min) * 60000;
	iMSeconds += (pstTimeInfo->tm_sec) * 1000;

	return iMSeconds;
}

/**************************************************************************************************
 * Function Name: reversedata
 * Description:  This function reverses the data 
 * Return value: INT32
 ***************************************************************************************************/
//TODO: function is not used.
INT32 reversedata(UINT8 *pbArg1, UINT8 *pbArg2, UINT32 uiSize)
{
	UINT32 uiLoopCount;

	if (NULL == pbArg1 || NULL == pbArg2)
	{
		return -1;
	}

	for (uiLoopCount = 0; uiLoopCount < uiSize; uiLoopCount++)
	{
		*(pbArg1 + uiLoopCount) = *(pbArg2 + (uiSize - 1) - uiLoopCount);
	}

	return 0;
}

/**************************************************************************************************
 * Function Name: CheckAllowedDTForMapping
 * Description: This function checks for the datatype allowed for mapping
 * Return value: bool
 ***************************************************************************************************/
bool CheckAllowedDTForMapping(char* dtName)
{
	if (0 == strcmp(StringToUpper(dtName), "INTEGER8")
			|| 0 == strcmp(StringToUpper(dtName), "INTEGER16")
			|| 0 == strcmp(StringToUpper(dtName), "INTEGER32")
			|| 0 == strcmp(StringToUpper(dtName), "UNSIGNED8")
			|| 0 == strcmp(StringToUpper(dtName), "UNSIGNED16")
			|| 0 == strcmp(StringToUpper(dtName), "UNSIGNED32"))
	{
		return true;

	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
 * Function Name: getLastAvailableCycleNumber
 * Description: This function returns last available cycle number
 * Return value: char*
 ***************************************************************************************************/
//TODO: Add a parameter to return the value to avoid new delete memory issues
char* getLastAvailableCycleNumber()
{
	char* ForcedCycle = new char[20];
	ocfmRetCode Ret;
	INT32 IndexPos;
	INT32 subIndexPos;
	Ret = IfSubIndexExists(240, MN, (char*) "1F98", (char*) "07", &subIndexPos,
			&IndexPos);
	if (OCFM_ERR_SUCCESS != Ret.code)
	{
		strcpy(ForcedCycle, "");
		return ForcedCycle;
	}
	char* actValue = new char[20];
	Ret = GetSubIndexAttributes(240, MN, (char*) "1F98", (char*) "07",
			ACTUALVALUE, actValue);
	UINT32 uiTempCycleNumber = uiCycleNumber;
	if (OCFM_ERR_SUCCESS != Ret.code)
	{

		UINT32 iCycleValue;
		if (0 == strncmp(actValue, "0x", 2) || 0 == strncmp(actValue, "0X", 2))
			iCycleValue = hex2int(subString(actValue, 2, strlen(actValue) - 2));
		else
			iCycleValue = atoi(actValue);

		if (iCycleValue > uiTempCycleNumber + 1)
		{
			uiTempCycleNumber += 1;
		}
		else
		{
			uiTempCycleNumber += 1;
			while (iCycleValue < uiTempCycleNumber)
			{
				uiTempCycleNumber = uiTempCycleNumber - iCycleValue;
			}

		}

	}
	delete[] actValue;

	UINT32 uiFreeCycleNumber = getFreeCycleNumber(uiTempCycleNumber);
	if (uiFreeCycleNumber == uiTempCycleNumber)
	{
		uiCycleNumber = uiTempCycleNumber;
		ForcedCycle = _IntToAscii(uiTempCycleNumber, ForcedCycle, 16);
	}
	else
	{
		ForcedCycle = _IntToAscii(uiFreeCycleNumber, ForcedCycle, 16);
	}
	return ForcedCycle;
}

/**************************************************************************************************
 * Function Name: checkAndCorrectName
 * Description  : This function corrects the name by adding '_'
 * Return value : void
 ***************************************************************************************************/
void checkAndCorrectName(char* checkName)
{
	if (NULL == checkName)
	{
		return;
	}

	UINT32 uiStrLen = strlen(checkName);
	for (UINT32 uiLoopCount = 0; uiLoopCount < uiStrLen; uiLoopCount++)
	{
		if ((48 <= checkName[uiLoopCount] && 57 >= checkName[uiLoopCount])
				|| (65 <= checkName[uiLoopCount] && 90 >= checkName[uiLoopCount])
				|| (97 <= checkName[uiLoopCount]
						&& 122 >= checkName[uiLoopCount])
				|| 95 == checkName[uiLoopCount])
		{
		}
		else
		{
			checkName[uiLoopCount] = '_';
		}
	}
}

/**************************************************************************************************
 * Function Name: checkIfValueZero
 * Description:  This function checks for zero value
 * Return value: bool
 ***************************************************************************************************/
bool checkIfValueZero(char* pcValue)
{
	if (NULL == pcValue || 0 == strcmp(pcValue, ""))
	{
		return false;
	}

	INT32 iValue = 0;
	if (0 == strncmp(pcValue, "0x", 2) || 0 == strncmp(pcValue, "0X", 2))
	{
		iValue = hex2int(subString(pcValue, 2, strlen(pcValue) - 2));
	}
	else
	{
		iValue = atoi(pcValue);
	}

	if (0 == iValue)
	{
		return true;
	}
	else
	{
		return false;
	}
}

/**************************************************************************************************
 * Function Name: GetDecimalValue
 * Description:   This function returns decimal value
 * Return value: INT32
 ***************************************************************************************************/
INT32 GetDecimalValue(char* pcValue)
{
	if (NULL == pcValue || 0 == strcmp(pcValue, ""))
	{
		return 0;
	}

	INT32 iValue = 0;
	if (0 == strncmp(pcValue, "0x", 2) || 0 == strncmp(pcValue, "0X", 2))
	{
		iValue = hex2int(subString(pcValue, 2, strlen(pcValue) - 2));
	}
	else
	{
		iValue = atoi(pcValue);
	}

	return iValue;
}

/**************************************************************************************************
 * Function Name: CheckAccessTypeForInclude
 * Description: This function checks for access type
 * Return value: bool
 ***************************************************************************************************/
bool CheckAccessTypeForInclude(char* pcAccesstype)
{
	if (NULL == pcAccesstype)
	{
		return true;
	}
	bool bInclude;
	char* pcUpperAccesstype = StringToUpper(pcAccesstype);
	if ((0 == strcmp(pcUpperAccesstype, "CONST"))
			|| (0 == strcmp(pcUpperAccesstype, "RO")))
	{
		bInclude = false;
	}
	else
	{
		bInclude = true;
	}
	delete[] pcUpperAccesstype;
	return bInclude;
}

/**************************************************************************************************
 * Function Name: CheckToolVersion
 * Description: This function checks for tool version
 * Return value: bool
 ***************************************************************************************************/
bool CheckToolVersion(char* pcCurrentToolVersion)
{
	char* pcUpperCurrentToolVersion = ConvertToUpper(pcCurrentToolVersion);
	if ((FALSE == strcmp(pcUpperCurrentToolVersion, TOOL_VERSION))
			|| (FALSE == strcmp(pcUpperCurrentToolVersion, PREV_TOOL_VERSION_1))
			|| (FALSE == strcmp(pcUpperCurrentToolVersion, PREV_TOOL_VERSION_2))
			|| (FALSE == strcmp(pcUpperCurrentToolVersion, PREV_TOOL_VERSION_3))
			|| (FALSE == strcmp(pcUpperCurrentToolVersion, PREV_TOOL_VERSION_4))
			|| (FALSE == strcmp(pcUpperCurrentToolVersion, PREV_TOOL_VERSION_5))
			|| (FALSE == strcmp(pcUpperCurrentToolVersion, PREV_TOOL_VERSION_6))
			|| (FALSE == strcmp(pcUpperCurrentToolVersion, LAST_TOOL_VERSION))
			|| (FALSE
					== strcmp(pcUpperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_1))
			|| (FALSE
					== strcmp(pcUpperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_2))
			|| (FALSE
					== strcmp(pcUpperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_3))
			|| (FALSE
					== strcmp(pcUpperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_4))
			|| (FALSE
					== strcmp(pcUpperCurrentToolVersion,
							TOOL_INTER_REL_VERSION_5)))
	{
		return true;
	}
	else
	{
		return false;
	}
}
