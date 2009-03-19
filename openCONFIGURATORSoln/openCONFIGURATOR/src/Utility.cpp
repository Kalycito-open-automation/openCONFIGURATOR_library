///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  ClassNAME
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:  
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
//****************************************************************************/
//
//  REVISION HISTORY:
// $Log:      $
///////////////////////////////////////////////////////////////////////////////////////////////

#include "../Include/Declarations.h"
#include "../Include/Internal.h"
#include "../Include/ProcessImage.h"

#include <iostream>
#include <stdlib.h>
#include <time.h>
using namespace std;
char* ConvertToUpper(char* pbStrBuffer)
	{
		UINT32 uiLoopCount;

		for(uiLoopCount = 0; pbStrBuffer[uiLoopCount]; ++uiLoopCount)
		{
			pbStrBuffer[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);
		}
		return pbStrBuffer;
		//int t;
		//int len = strlen(str);
		//char* str1;
		//str1 = new char[len];
		//for(t=0; t<strlen(str); ++t)
		//{
		//	str1[t] = toupper(str[t]);
		//}		
		//str1[t] = '\0';
		//return str1;
	}
	char* StringToUpper(char* pbStrBuffer)
	{
		UINT32 uiLoopCount;
		UINT32 uiStringLen = strlen(pbStrBuffer);
		char* strUpp = new char[uiStringLen + ALLOC_BUFFER];

		for(uiLoopCount = 0; uiLoopCount < uiStringLen; ++uiLoopCount)
		{
			strUpp[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);
		}
		strUpp[uiLoopCount] = '\0';
		return strUpp;
	
	}
char* reverse(char* pbStrBuffer)
{
    char* pbCharLeft  = pbStrBuffer;
    char* pbCharRight = pbCharLeft + strlen(pbStrBuffer) - 1;
    char  cTemp;
    while (pbCharLeft < pbCharRight) {
        cTemp	= *pbCharLeft;
        *pbCharLeft++  = *pbCharRight;
        *pbCharRight-- = cTemp;
    }
    return pbStrBuffer;
}
char* padLeft(char* pbStrBuffer, char cPadChar, int iPadLength)
{
			char abTemp[50];
			int iLoopCount = 0;
			int iStringLen = strlen(pbStrBuffer);
			
			if (iStringLen < iPadLength)
			{
				//temp = new char[iPadLength-iStringLen+1];		
				pbStrBuffer = reverse(pbStrBuffer);
				while(iLoopCount < (iPadLength-iStringLen))
				{
					abTemp[iLoopCount] = cPadChar;
					iLoopCount++;
				}
				abTemp[iLoopCount] = '\0';
				strcat(pbStrBuffer, abTemp);				
				pbStrBuffer = reverse(pbStrBuffer);
			}
			return pbStrBuffer;
	}
char* subString(char* pbStrBuffer, int iStartPos, int iStringLen)
{
		char* pbSubStrBuffer;
	
		pbSubStrBuffer = new char[iStringLen + 1];
		strncpy(pbSubStrBuffer,(const char*)(pbStrBuffer + iStartPos),iStringLen);
		pbSubStrBuffer[iStringLen]='\0';
		return pbSubStrBuffer;
}
char *utoa(unsigned uValue, char *pbDigits, int iBaseValue)
{
    char *pbAlphNum, *pbTempBuffer;
	
    pbAlphNum = (char *) "0123456789abcdefghijklmnopqrstuvwxyz"; 
    if (iBaseValue == 0)
        iBaseValue = 10;
    if (pbDigits == NULL || iBaseValue < 2 || iBaseValue > 36)
        return NULL;
    if (uValue < (unsigned) iBaseValue) {
        pbDigits[0] = pbAlphNum[uValue];
        pbDigits[1] = '\0';
    } else {
        for (pbTempBuffer = utoa(uValue / ((unsigned)iBaseValue), pbDigits, iBaseValue);
             *pbTempBuffer;
             pbTempBuffer++);
        utoa( uValue % ((unsigned)iBaseValue), pbTempBuffer, iBaseValue);
    }
    return pbDigits;
}

char *_IntToAscii(int iValue, char *pbDigits, int iBaseValue)
{
    char *pbDigit;
    unsigned uValue; /* assume unsigned is big enough to hold all the
                 * unsigned values -x could possibly be -- don't
                 * know how well this assumption holds on the
                 * DeathStation 9000, so beware of nasal demons
                 */

    pbDigit = pbDigits;
    if (iBaseValue == 0)
        iBaseValue = 10;
    if (pbDigits == NULL || iBaseValue < 2 || iBaseValue > 36)
        return NULL;
    if (iValue < 0) {
        *pbDigit++ = '-';
        uValue = -((unsigned)iValue);
    } else
        uValue = iValue;
    utoa(uValue, pbDigit, iBaseValue);

    return pbDigits;
}


/**************************************************************************************************
* Function Name: _IntToAscii
* Description: Perform C++ style "_IntToAscii"
***************************************************************************************************/

//char* _IntToAscii( int value, char* result, int base ) 
//{
//	// check that the base if valid
//
//	if (base < 2 || base > 16) 
//	{ 
//		*result = 0; return result; 
//	}
//	char* out = result;
//	int quotient = value;
//	do 
//	{
//		*out = "0123456789abcdef"[ abs( quotient % base ) ];
//		++out;
//		quotient /= base;
//	} while ( quotient );
//
//	// Only apply negative sign for base 10
//	
//	if ( value < 0 && base == 10) *out++ = '-';
//	result = reverse(out);
//	printf("\nResult%s",result);
//	*out = 0;
//	return result;
//}
unsigned long hex2int(char *pbStrBuffer)
{
    UINT32 uiLoopCount;
    UINT32 uiValue = 0;
    UINT32 uiStrLen = strlen(pbStrBuffer);
				
    for(uiLoopCount = 0; uiLoopCount < uiStrLen; uiLoopCount++)
    {
	if(IsAscii(pbStrBuffer[uiLoopCount]))
		pbStrBuffer[uiLoopCount] = toupper(pbStrBuffer[uiLoopCount]);	
       if(pbStrBuffer[uiLoopCount] <= 57)
        uiValue += (pbStrBuffer[uiLoopCount] - 48)*(1<<(4*(uiStrLen-1-uiLoopCount)));
       else
        uiValue += (pbStrBuffer[uiLoopCount] - 55)*(1<<(4*(uiStrLen-1-uiLoopCount)));
    }
    return uiValue;
}
bool IsAscii(char cArg)
{
	UINT32 uiCount;
	uiCount = cArg;
	if (uiCount >= 48 && uiCount <= 57)
		return false;
	else 
		return true;
}
bool CheckIfNotPDO(char* pbIndex)
	{
		if(strncmp(pbIndex, "14",2) == 0 || strncmp(pbIndex, "16",2) == 0 || strncmp(pbIndex, "18",2) == 0 ||
				strncmp(pbIndex,"1A",2) == 0)
			return false;
		else return true;
	}
bool CheckIfManufactureSpecificObject(char* pbIndex)
{
		UINT32 _Device_Index;
		_Device_Index = hex2int( (char *) "2000");
		if(hex2int(pbIndex) >= _Device_Index )
			return true;
		else return false;
}
bool CheckIfMappingPDO(char* pbIndex)
{
		if(strncmp(pbIndex, "1A",2) == 0 || strncmp(pbIndex, "16",2) == 0)
		return true;
		else return false;
}
bool CheckAllowedCNIndexes(char* pbIndexValue)
{
	if((CheckIfNotPDO((char*)pbIndexValue) == false)  || CheckIfManufactureSpecificObject((char*)pbIndexValue) ||
					strcmp(pbIndexValue,"1F98") == 0 
					||	strcmp(pbIndexValue,"1020") == 0
					||	strcmp(pbIndexValue,"1006") == 0)
	return true;
	else return false;							
}
char* ConvertToHexformat(char* pbHexValue, int iPadLength, bool bDoPadding)
{
	char* pbHexTemp;

	pbHexTemp = (char*)malloc(iPadLength + 3);
	strcpy(pbHexTemp, "0x");
	if(bDoPadding)
	pbHexValue = padLeft(pbHexValue, '0', iPadLength);
	strcat(pbHexTemp, pbHexValue);
	
	return pbHexTemp;
}
bool CheckIfHex(char* pbValue)
{
	if(strchr(pbValue,'x') == NULL)
	{
		if(strchr(pbValue,'X') == NULL)
		return false;
		else return true;
	}
	else return true;
		
}
//char* ReplaceChars(char ch, int num, char* str)
//{
//		int t;
//		char st[2];
//		for(t=0; str[t] ; ++t)
//		{
//			for(int i =0; i<num ;i++)
//			{
//				strcpy(st[i], 
//			}
//			if(str[t] == ch)
//			
//		}
//		return str;
//}
int getConfigDate()
{
  // to have the total number of days Since 1984
  UINT32 uiDays = 0;
  // to have the total number of years Since 1984  
  UINT32 uiYearSince = 0;
  // to have the current Year
  UINT32 uiCurrentYear = 0;
  time_t rawtime;
  struct tm * pstTimeInfo;

  time ( &rawtime );
  pstTimeInfo = localtime ( &rawtime );
  printf ( "Current local time and date: %s", asctime (pstTimeInfo) );

  cout << "\ntm_yday:" << pstTimeInfo->tm_yday << endl;
  cout << "\ntm_year Since 1900:" << pstTimeInfo->tm_year << endl;
  uiYearSince = ((pstTimeInfo->tm_year)- 84);
  cout << "\ntm_year Since 1984:" << uiYearSince << endl;
  uiCurrentYear = ((pstTimeInfo->tm_year) + 1900);
  cout << "\nCurrent Year:" << uiCurrentYear << endl;
  
  // Loops thru all the years except the curent year
  for(UINT32 uiLoopCount = 0; uiLoopCount < uiYearSince; uiLoopCount++)
  {
  	UINT32 uiTmpYear = 0;
  	UINT32 uiTmpDays = 0;
  	uiTmpYear = (1984 + uiLoopCount);
  	//cout << "\nYear:" << uiTmpYear << endl;
  	if (uiTmpYear%4 == 0 && !(uiTmpYear%100 == 0 && uiTmpYear%400 != 0))
  		uiTmpDays = 366;
  	else
  		uiTmpDays = 365;

  	uiDays += uiTmpDays;
  }
  // To calculate number of days since Jan 1 of current year
  uiDays += pstTimeInfo->tm_yday;
  cout << "days:" << uiDays << endl;
  return uiDays;
}
int getConfigTime()
{
  time_t rawtime;
  struct tm * pstTimeInfo;
  UINT32 uiMSeconds = 0;

  time ( &rawtime );
  pstTimeInfo = localtime ( &rawtime );
  
  uiMSeconds += (pstTimeInfo->tm_hour) * 3600000;
  uiMSeconds += (pstTimeInfo->tm_min) * 60000;
  uiMSeconds += (pstTimeInfo->tm_sec) * 1000;
  return uiMSeconds;
}
CHAR toupper (CHAR cArg)
{
	if (cArg >= 97 && cArg <= 122)
	{
		cArg = cArg - 32;
	}
	return cArg;
}
CHAR tobin(CHAR cArg)
{
	if(cArg >= 65 && cArg <= 70)
	{
		cArg = cArg - 55;
	}
	else if ( cArg >=48 && cArg <= 57)
	{
		cArg = cArg - 48;
	}
	return cArg;
}
int reversedata(UINT8 *pbArg1, UINT8 *pbArg2, UINT32 uiSize)
{
	UINT32 uiLoopCount;
	if (pbArg1 == NULL || pbArg2 == NULL)
		return -1;
	for (uiLoopCount = 0; uiLoopCount < uiSize; uiLoopCount++)
		*(pbArg1 + uiLoopCount) = *(pbArg2 + uiSize - 1 - uiLoopCount);
	return 0;
}