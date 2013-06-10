/**
 ************************************************************************************************
 \file		ProcessImage.cpp

 \brief		This file contains the function definitions and Process Image type structure variables to handle operations on the ProcessImage objects in the collection
 ************************************************************************************************
 */

/*
 © Kalycito Infotech Private Limited

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

/****************************************************************************************************/
/* Includes */

#include <iostream>
#include <fstream>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include "../Include/openCONFIGURATOR.h"
#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include "../Include/ProcessImage.h"

/****************************************************************************************************/
/* Defines */

#define HEADER_FILE_BUFFER 5000000
#define TOTAL_MODULES 10000

/****************************************************************************************************/
/* Global Variables */

INT32 inVarsGlobal = 0;
INT32 outVarsGlobal = 0;

static PIIndexTable piIndexTable[NO_OF_PI_ENTERIES] =
{
{ "A000", INTEGER8, OUTPUT },
{ "A040", UNSIGNED8, OUTPUT },
{ "A0C0", INTEGER16, OUTPUT },
{ "A100", UNSIGNED16, OUTPUT },
{ "A1C0", INTEGER32, OUTPUT },
{ "A200", UNSIGNED32, OUTPUT },
{ "A240", REAL32, OUTPUT },
{ "A400", INTEGER64, OUTPUT },
{ "A440", UNSIGNED64, OUTPUT },
{ "A480", INTEGER8, INPUT },
{ "A4C0", UNSIGNED8, INPUT },
{ "A540", INTEGER16, INPUT },
{ "A580", UNSIGNED16, INPUT },
{ "A640", INTEGER32, INPUT },
{ "A680", UNSIGNED32, INPUT },
{ "A6C0", REAL32, INPUT },
{ "A880", INTEGER64, INPUT },
{ "A8C0", UNSIGNED64, INPUT },

};

ModuleCol moduleColl[TOTAL_MODULES];

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

//TODO: Unused function
void SetPIOffsets(ProcessImage* piObj, INT32& startByteOffset,
		INT32& startBitOffset)
{
	if (NULL == piObj)
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}
	if (1 == piObj->dataInfo.dataSize)
	{
		if (-1 == startBitOffset)
		{
			piObj->byteOffset = startByteOffset;
		}
		startBitOffset = startBitOffset + 1;
		piObj->bitOffset = startBitOffset;
		piObj->byteOffset = startByteOffset;

		if (7 == startBitOffset)
		{
			startByteOffset = startByteOffset + 1;
		}
	}
	else
	{
		piObj->byteOffset = startByteOffset;
		startByteOffset = startByteOffset + (piObj->dataInfo.dataSize) / 8;
	}
}

void GroupInOutPIVariables(ProcessImage piInCol[], ProcessImage piOutCol[])
{

	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	inVarsGlobal = 0;
	outVarsGlobal = 0;

	for (INT32 nodeLoopCount = 0;
			nodeLoopCount < objNodeCol->GetNumberOfNodes(); nodeLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(nodeLoopCount);

		for (INT32 piLoopCount = 0; piLoopCount < objNode->PICollection.Count();
				piLoopCount++)
		{
			if (INPUT == objNode->PICollection[piLoopCount].directionType)
			{
				piInCol[inVarsGlobal] = objNode->PICollection[piLoopCount];
				inVarsGlobal++;
			}
			else if (OUTPUT == objNode->PICollection[piLoopCount].directionType)
			{
				piOutCol[outVarsGlobal] = objNode->PICollection[piLoopCount];
				outVarsGlobal++;
			}
			else
			{
				//Nothing other than INPUT & OUTPUT
			}
		}
	}
}

INT32 GroupNETPIVariables(PIDirectionType directionType,
		NETProcessImage netPIObj[])
{
	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	INT32 netPIVarsCount = 0;
	for (INT32 iOutLoopCount = 0;
			iOutLoopCount < objNodeCol->GetNumberOfNodes(); iOutLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(iOutLoopCount);

		for (INT32 iInLoopCount = 0; iInLoopCount < objNode->NETPIColl.Count();
				iInLoopCount++)
		{
			if (objNode->NETPIColl[iInLoopCount].directionType == directionType)
			{
				netPIObj[netPIVarsCount] = objNode->NETPIColl[iInLoopCount];
				netPIVarsCount++;
			}
		}
	}
	return netPIVarsCount;
}

void SetUniquePIVarName()
{
	NodeCollection* objNodeCol = NULL;
	Node* objNode = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	inVarsGlobal = 0;
	outVarsGlobal = 0;
	ProcessImage* piObjTemp1 = NULL;
	ProcessImage* piObjTemp2 = NULL;
	const INT32 uniqNameLen = 2;

	for (INT32 nodeLoopCount = 0;
			nodeLoopCount < objNodeCol->GetNumberOfNodes(); nodeLoopCount++)
	{
		objNode = objNodeCol->GetNodebyColIndex(nodeLoopCount);

		for (INT32 iInLoopCount = 0;
				iInLoopCount < objNode->PICollection.Count(); iInLoopCount++)
		{
			INT32 uniqNameCnt = 1;
			bool nameMatched = false;

			piObjTemp1 = &objNode->PICollection[iInLoopCount];
			if( piObjTemp1 == NULL)
			{
				cout<<"PICollection Null object. Node ID:"<<objNode->GetNodeId()<<" iInLoopCount:"<<iInLoopCount<<endl;
				continue;
			}
			//it is possible that the changed var name matching a previous entry
			for (INT32 iInChkLoopCount = 0;
					iInChkLoopCount < objNode->PICollection.Count();
					iInChkLoopCount++)
			{
				if (iInChkLoopCount == iInLoopCount)
				{
					//Both the index are same, do not check same object
					continue;
				}

				piObjTemp2 = &objNode->PICollection[iInChkLoopCount];
				if( piObjTemp2 == NULL)
				{
					cout<<"PICollection Null object. Node ID:"<<objNode->GetNodeId()<<" InLC:"<<iInLoopCount<<" OutLC:"<<iInChkLoopCount<<endl;
					continue;
				}
				//check module index, module name, directiontype and variable name
				//if all are same then append count variable to variable name
				if ((0 == strcmp(piObjTemp1->moduleIndex, piObjTemp2->moduleIndex))
						&& (0 == strcmp(piObjTemp1->moduleName, piObjTemp2->moduleName))
						&& (piObjTemp1->directionType == piObjTemp2->directionType)
						&& ((piObjTemp1->varDeclName != NULL) && (NULL != piObjTemp2->varDeclName)))
				{
					if((0 == strcmp(piObjTemp1->varDeclName, piObjTemp2->varDeclName)))
					{
						//change the name of VarName
						uniqNameCnt++; //1 is reserved for first matched entry
						if (NULL != piObjTemp2->varDeclName)
						{
							delete[] piObjTemp2->varDeclName;
						}
						piObjTemp2->varDeclName = new char[strlen(piObjTemp1->varDeclName) + uniqNameLen + ALLOC_BUFFER];
						sprintf(piObjTemp2->varDeclName, "%s%02d", piObjTemp1->varDeclName, uniqNameCnt);
						nameMatched = true;

						//change the name of Name
						if (NULL != piObjTemp2->name)
							delete[] piObjTemp2->name;

						piObjTemp2->name = new char[strlen(piObjTemp2->varDeclName) + strlen(piObjTemp2->moduleName) + 6 + ALLOC_BUFFER];
						strcpy(piObjTemp2->name, GetPIName(objNode->GetNodeId()));
						strcat(piObjTemp2->name, piObjTemp2->moduleName);
						strcat(piObjTemp2->name, ".");
						strcat(piObjTemp2->name, piObjTemp2->varDeclName);
					}

				}
			}

			if (true == nameMatched)
			{
				char* tempVarName = NULL;
				tempVarName = new char[strlen(piObjTemp1->varDeclName) + ALLOC_BUFFER];
				strcpy(tempVarName, piObjTemp1->varDeclName);
				if (NULL != piObjTemp1->varDeclName)
				{
					delete[] piObjTemp1->varDeclName;
				}
				piObjTemp1->varDeclName = new char[strlen(tempVarName) + uniqNameLen + ALLOC_BUFFER];

				sprintf(piObjTemp1->varDeclName, "%s%02d", tempVarName, 1);
				delete[] tempVarName;

				//change the name of Name
				if (NULL != piObjTemp1->name)
				{
					delete[] piObjTemp1->name;
				}
				piObjTemp1->name = new char[strlen(piObjTemp1->varDeclName) + strlen(piObjTemp1->moduleName) + 6 + ALLOC_BUFFER];

				strcpy(piObjTemp1->name, GetPIName(objNode->GetNodeId()));
				strcat(piObjTemp1->name, piObjTemp1->moduleName);
				strcat(piObjTemp1->name, ".");
				strcat(piObjTemp1->name, piObjTemp1->varDeclName);
			}
		}
	}
}

PIDataInfo* GetIECDT(char* iecDataType, INT32 dataSize)
{
	PIDataInfo *stDataInfo = new PIDataInfo;
	stDataInfo->dtName = new char[strlen(iecDataType) + 1];
	strcpy(stDataInfo->dtName, iecDataType);

	if (NULL != iecDataType)
	{
		if (!strcmp(iecDataType, "BITSTRING"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = BITSTRING;
		}
		else if (!strcmp(iecDataType, "BOOL"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = BOOL;
		}
		else if (!strcmp(iecDataType, "_CHAR"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = _CHAR;
		}
		else if (!strcmp(iecDataType, "WORD"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = BYTE;
		}
		else if (!strcmp(iecDataType, "DWORD"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = DWORD;
		}
		else if (!strcmp(iecDataType, "LWORD"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = LWORD;
		}
		else if (!strcmp(iecDataType, "SINT"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = SINT;
		}
		else if (!strcmp(iecDataType, "INT"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = INT;
		}
		else if (!strcmp(iecDataType, "DINT"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = DINT;
		}
		else if (!strcmp(iecDataType, "LINT"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = LINT;
		}
		else if (!strcmp(iecDataType, "USINT"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = USINT;
		}
		else if (!strcmp(iecDataType, "UINT"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = UINT;
		}
		else if (strcmp(iecDataType, "UDINT") == 0)
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = UDINT;
		}
		else if (!strcmp(iecDataType, "ULINT"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = ULINT;
		}
		else if (!strcmp(iecDataType, "REAL"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = REAL;
		}
		else if (!strcmp(iecDataType, "LREAL"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = LREAL;
		}
		else if (!strcmp(iecDataType, "STRING"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = STRING;
		}
		else if (!strcmp(iecDataType, "WSTRING"))
		{
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = WSTRING;
		}
		else
		{
			//TODO: else added. operation to be specified
		}
	}
	return stDataInfo;
}

//TODO: Unused function
bool CheckIfModuleExists(char* moduleName, INT32 &moduleNo, INT32 noOfModules,
		ModuleCol modCollObj[])
{
	for (INT32 moduleLC = 0; moduleLC <= noOfModules; moduleLC++)
	{
		if (!strcmp(moduleName, moduleColl[moduleLC].moduleName))
		{
			moduleNo = modCollObj[moduleLC].moduleNo;
			return true;
		}
	}
	return false;
}

void GenerateXAPHeaderFile(char* fileName, ProcessImage piInCol[],
		ProcessImage piOutCol[], INT32 inVar, INT32 outVar)
{
	char* xapFileName = new char[strlen(fileName) + ALLOC_BUFFER];
	FILE* xapFile = new FILE();

	strcpy(xapFileName, fileName);
	strcat(xapFileName, ".h");

	if (NULL == (xapFile = fopen(xapFileName, "w+ ")))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
		delete[] xapFileName;
		throw ex;
	}

	//write comment
	char* varComment = new char[strlen(BUILD_COMMENT) + BUILDTIME_BUF_LEN
			+ STR_ALLOC_BUFFER + 8]; // 8 is for comment lines
	strcpy(varComment, "/* ");
	strcat(varComment, BUILD_COMMENT);
	strcat(varComment, GetBuildTime());
	strcat(varComment, " */\n");
	UINT32 uiStrLength = strlen(varComment);
	if ((uiStrLength != fwrite(varComment, sizeof(char), uiStrLength, xapFile)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] varComment;
		delete[] xapFileName;
		fclose(xapFile);
		throw ex;
	}
	delete[] varComment;
	//write include guard for the headerfile
	char* xapHeaderIncludeGuard = new char[strlen(INCLUDE_GUARD_CHECK)
			+ strlen(INCLUDE_GUARD_START) + ALLOC_BUFFER];
	if (NULL == xapHeaderIncludeGuard)
	{
#if defined DEBUG
		cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

		ocfmException ex;
		ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		delete[] xapFileName;
		fclose(xapFile);
		throw ex;
	}
	strcpy(xapHeaderIncludeGuard, INCLUDE_GUARD_CHECK);
	strcat(xapHeaderIncludeGuard, "\n");
	strcat(xapHeaderIncludeGuard, INCLUDE_GUARD_START);
	strcat(xapHeaderIncludeGuard, "\n\n");
	uiStrLength = strlen(xapHeaderIncludeGuard);

	if ((uiStrLength
			!= fwrite(xapHeaderIncludeGuard, sizeof(char), uiStrLength, xapFile)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] xapHeaderIncludeGuard;
		delete[] xapFileName;
		fclose(xapFile);
		throw ex;
	}

	delete[] xapHeaderIncludeGuard;
	/* write Input structure */
	if (0 != inVar)
	{
		WriteXAPHeaderContents(piInCol, inVar, INPUT, xapFile);
	}

	fclose(xapFile);

	/* write Output structure */
	if (NULL == (xapFile = fopen(xapFileName, "a+")))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
		delete[] xapFileName;
		throw ex;
	}
	delete[] xapFileName;
	if (0 != outVar)
	{
		WriteXAPHeaderContents(piOutCol, outVar, OUTPUT, xapFile);
	}

	//close include guard for the XAP header file
	xapHeaderIncludeGuard = new char[strlen(INCLUDE_GUARD_END) + ALLOC_BUFFER];

	strcpy(xapHeaderIncludeGuard, "\n");
	strcat(xapHeaderIncludeGuard, INCLUDE_GUARD_END);
	uiStrLength = strlen(xapHeaderIncludeGuard);
	if ((uiStrLength
			!= fwrite(xapHeaderIncludeGuard, sizeof(char), uiStrLength, xapFile)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] xapHeaderIncludeGuard;
		fclose(xapFile);
		throw ex;
	}

	fclose(xapFile);
	delete[] xapHeaderIncludeGuard;
}

void WriteXAPHeaderContents(ProcessImage piObj[], INT32 noOfVars,
		PIDirectionType directionType, FILE* xapHeader)
{
	char* mainBuffer = new char[HEADER_FILE_BUFFER];
	char* tempBuffer = new char[200];

	if (0 != noOfVars)
	{
		strcpy(mainBuffer, "\ntypedef struct \n{\n");
		INT32 holeFilledIdNo = 1;
		INT32 totalSize = 0;
		for (INT32 loopCount = 0; loopCount < noOfVars; loopCount++)
		{
			//INT32 iNodeId;
			INT32 dataSize = 0;
			char* nodeId = new char[50];
			char* moduleName = new char[50];
			char* moduleNo = new char[20];

			dataSize = piObj[loopCount].dataInfo.dataSize;
			/* Check if 8, 16, 32 bit aligned*/
			if ((0 == dataSize % 32) || (0 == dataSize % 16)
					|| (0 == dataSize % 8))
			{
				if (0 != totalSize % dataSize)
				{
					char holeid[20];
					char* fillBitsStr = new char[2 + ALLOC_BUFFER];
					INT32 filledBits = dataSize - (totalSize % dataSize);

					totalSize = totalSize + filledBits;
					strcat(mainBuffer, "\tunsigned");
					strcat(mainBuffer, " ");
					strcat(mainBuffer, "PADDING_VAR_");
					strcat(mainBuffer, IntToAscii(holeFilledIdNo, holeid, 10));
					strcat(mainBuffer, ":");
					fillBitsStr = IntToAscii(filledBits, fillBitsStr, 10);
					strcat(mainBuffer, fillBitsStr);
					strcat(mainBuffer, ";\n");
					holeFilledIdNo = holeFilledIdNo + 1;
					delete[] fillBitsStr;
				}
			}

			//iNodeId = objProcessImage[iLoopCount].CNNodeID;

			nodeId = IntToAscii(piObj[loopCount].nodeId, nodeId, 10);
			strcpy(moduleNo, SubString(piObj[loopCount].moduleIndex, 2, 2));
			strcpy(moduleName, piObj[loopCount].moduleName);

			strcat(mainBuffer, "\tunsigned");
			strcat(mainBuffer, " ");
			char* tempName = new char[100];
			strcpy(tempName, "CN");
			nodeId = IntToAscii(piObj[loopCount].nodeId, nodeId, 10);
			strcat(tempName, nodeId);
			strcat(tempName, "_");

			/* Add Mod NO*/
			strcat(tempName, "M");
			strcat(tempName, moduleNo);
			strcat(tempName, "_");

			strcat(tempName, moduleName);
			if (piObj[loopCount].varDeclName != NULL)
			{
				strcat(tempName, "_");
				strcat(tempName, piObj[loopCount].varDeclName);
			}

			strcat(mainBuffer, tempName);
			strcat(mainBuffer, ":");

			char* varTempBuff = new char[50];
			varTempBuff = IntToAscii(dataSize, varTempBuff, 10);
			totalSize = dataSize + totalSize;
			strcat(mainBuffer, varTempBuff);
			strcat(mainBuffer, ";");

			strcat(mainBuffer, "\n");
			delete[] tempName;
			delete[] moduleName;
			delete[] moduleNo;
			delete[] nodeId;
		}
		/* Check if the whole struct is 32 bit aligned*/
		if (0 != totalSize % 32)
		{
			char holeid[20];
			char* fillBitsStr = new char[2 + ALLOC_BUFFER];
			INT32 filledBits = 32 - (totalSize % 32);

			totalSize = totalSize + filledBits;
			strcat(mainBuffer, "\tunsigned");
			strcat(mainBuffer, " ");
			strcat(mainBuffer, "PADDING_VAR_");
			strcat(mainBuffer, IntToAscii(holeFilledIdNo, holeid, 10));
			strcat(mainBuffer, ":");
			fillBitsStr = IntToAscii(filledBits, fillBitsStr, 10);
			strcat(mainBuffer, fillBitsStr);
			strcat(mainBuffer, ";\n");
			//iHoleFilledIdNo = iHoleFilledIdNo + 1; Unused operation

			delete[] fillBitsStr;
		}
		strcat(mainBuffer, "}");

		if (INPUT == directionType)
		{
			strcpy(tempBuffer, "# define COMPUTED_PI_OUT_SIZE ");
			strcat(mainBuffer, " PI_OUT;");
		}
		else if (OUTPUT == directionType)
		{

			strcpy(tempBuffer, "\n\n# define COMPUTED_PI_IN_SIZE ");
			strcat(mainBuffer, " PI_IN;");
			strcat(mainBuffer, "\n");
		}
		else
		{
			//Nothing to do
		}

		char* strSize = new char[50];

		/* write the size in bytes*/
		totalSize = totalSize / 8;
		strSize = IntToAscii(totalSize, strSize, 10);
		strcat(tempBuffer, strSize);
		delete[] strSize;
	}
	else
	{
		strcpy(tempBuffer, "");
	}

	UINT32 tempBufferLen = strlen(tempBuffer);

	if ((tempBufferLen
			!= fwrite(tempBuffer, sizeof(char), tempBufferLen, xapHeader)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] mainBuffer;
		delete[] tempBuffer;
		throw ex;
	}
	delete[] tempBuffer;

	tempBufferLen = strlen(mainBuffer);

	if ((tempBufferLen
			!= fwrite(mainBuffer, sizeof(char), tempBufferLen, xapHeader)))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] mainBuffer;
		throw ex;
	}
	delete[] mainBuffer;
}

void GenerateNETHeaderFile(char* fileName, ProcessImage piInCol[],
		ProcessImage piOutCol[], INT32 iInVar, INT32 iOutVar)
{
	char* netFileName = new char[strlen(fileName) + ALLOC_BUFFER];
	FILE* netFile = new FILE();
	ocfmException ex;

	strcpy(netFileName, fileName);
	strcat(netFileName, ".cs");
	/* write Input structure */
	if (NULL == (netFile = fopen(netFileName, "w+")))
	{
		ocfmException ex;
		ex.OCFMException(OCFM_ERR_CANNOT_OPEN_FILE);
		delete[] netFileName;
		throw ex;
	}
	delete[] netFileName;

	NodeCollection* objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();

	for (INT32 nodeLoopCount = 0;
			nodeLoopCount < objNodeCol->GetNumberOfNodes(); nodeLoopCount++)
	{
		Node* objNode = NULL;
		objNode = objNodeCol->GetNodebyColIndex(nodeLoopCount);
		objNode->DeleteCollectionsForNETPI();
	}
	if ((0 != iInVar) || (0 != iOutVar))
	{
		/*Writng Header of .NET Interface*/
		char* mainBuffer = new char[HEADER_FILE_BUFFER];
		strcpy(mainBuffer, "using System;\n");
		strcat(mainBuffer, "using System.Runtime.InteropServices;\n");
		//writing comments
		strcat(mainBuffer, "/// <summary>\n");
		strcat(mainBuffer, "/// ");
		strcat(mainBuffer, BUILD_COMMENT);
		strcat(mainBuffer, GetBuildTime());
		strcat(mainBuffer, "\n");
		strcat(mainBuffer, "/// </summary>\n");

		strcat(mainBuffer, "\nnamespace openPOWERLINK\n");
		strcat(mainBuffer, "{\n");
		UINT32 mainBufLen = strlen(mainBuffer);

		if ((mainBufLen != fwrite(mainBuffer, sizeof(char), mainBufLen, netFile)))
		{
			ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
			fclose(netFile);
			delete[] mainBuffer;
			throw ex;
		}
		delete[] mainBuffer;
	}
	if (0 != iInVar)
	{
		WriteNETHeaderContents(piInCol, iInVar, INPUT, netFile);
	}

	/* write Output structure */
	if (0 != iOutVar)
	{
		WriteNETHeaderContents(piOutCol, iOutVar, OUTPUT, netFile);
	}
	if ((0 != iInVar) || (0 != iOutVar))
	{
		char* tempBuffer = new char[10];
		strcpy(tempBuffer, "}\n");
		UINT32 tempBufLen = strlen(tempBuffer);

		if ((tempBufLen != fwrite(tempBuffer, sizeof(char), tempBufLen, netFile)))
		{
			ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
			fclose(netFile);
			delete[] tempBuffer;
			throw ex;
		}
		delete[] tempBuffer;
	}
	fclose(netFile);
}

void WriteNETHeaderContents(ProcessImage piObj[], INT32 noOfVars,
		PIDirectionType dirType, FILE* netHeader)
{

	char* mainBuffer = new char[HEADER_FILE_BUFFER];
	mainBuffer[0] = 0;
	char* tempBuffer1 = new char[200];
	char* tempBuffer2 = new char[500];
	char offsetStr[10];
	INT32 offsetVal = 0;
	NETProcessImage *objNetPiCol = NULL;
	INT32 totalSizeVal = GroupNETHeaderContents(piObj, noOfVars, dirType,
			netHeader);

	objNetPiCol = new NETProcessImage[PI_VAR_COUNT];
	INT32 netPIVarsCount = GroupNETPIVariables(dirType, objNetPiCol);
	for (INT32 loopCount = 0; loopCount < netPIVarsCount; loopCount++)
	{
		strcat(mainBuffer, "\t\t[FieldOffset(");
		memset(offsetStr, 0, 10);
		IntToAscii(offsetVal, offsetStr, 10);
		strcat(mainBuffer, offsetStr);
		strcat(mainBuffer, ")]\n");
		strcat(mainBuffer, "\t\tpublic ");
		strcat(mainBuffer,
				GetDatatypeNETPI(objNetPiCol[loopCount].dataInfo.iecDtVar));
		offsetVal += GetDatasizeNETPI(objNetPiCol[loopCount].dataInfo.iecDtVar);
		strcat(mainBuffer, " ");
		strcat(mainBuffer, objNetPiCol[loopCount].moduleName);
		if(objNetPiCol[loopCount].name != NULL)
		{
			strcat(mainBuffer, "_");
			strcat(mainBuffer, objNetPiCol[loopCount].name);
			if ((objNetPiCol[loopCount].count > 0) && (objNetPiCol[loopCount].lastName != NULL))
			{
				strcat(mainBuffer, "_to_");
				strcat(mainBuffer, objNetPiCol[loopCount].lastName);
			}
		}
		strcat(mainBuffer, ";\n");
	}
	delete[] objNetPiCol;
	char* totalSize = new char[20];
	IntToAscii(totalSizeVal, totalSize, 10);

	if (INPUT == dirType)
	{
		strcpy(tempBuffer1, "\n\t/// <summary>\n");
		strcat(tempBuffer1, "\t/// Struct : ProcessImage Out\n");
		strcat(tempBuffer1, "\t/// </summary>\n");
		strcat(tempBuffer1,
				"\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = ");
		strcat(tempBuffer1, totalSize);
		strcat(tempBuffer1, ")]\n");
		strcat(tempBuffer1, "\tpublic struct AppProcessImageOut\n");
		strcat(tempBuffer1, "\t{\n");

		strcpy(tempBuffer2, "\t}\n");
	}
	else if (OUTPUT == dirType)
	{

		strcpy(tempBuffer1, "\n\t/// <summary>\n");
		strcat(tempBuffer1, "\t/// Struct : ProcessImage In\n");
		strcat(tempBuffer1, "\t/// </summary>\n");
		strcat(tempBuffer1,
				"\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = ");
		strcat(tempBuffer1, totalSize);
		strcat(tempBuffer1, ")]\n");
		strcat(tempBuffer1, "\tpublic struct AppProcessImageIn\n");
		strcat(tempBuffer1, "\t{\n");

		strcpy(tempBuffer2, "\t}\n");
	}
	else
	{
		//TODO: Else added. Nothing to do
	}
	delete[] totalSize;

	ocfmException ex;
	UINT32 tempBuf1Len = strlen(tempBuffer1);

	if ((tempBuf1Len
			!= fwrite(tempBuffer1, sizeof(char), tempBuf1Len, netHeader)))
	{
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] mainBuffer;
		delete[] tempBuffer1;
		delete[] tempBuffer2;
		throw ex;
	}
	delete[] tempBuffer1;

	tempBuf1Len = strlen(mainBuffer);

	if ((tempBuf1Len != fwrite(mainBuffer, sizeof(char), tempBuf1Len, netHeader)))
	{
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] tempBuffer2;
		delete[] mainBuffer;
		throw ex;
	}
	delete[] mainBuffer;

	tempBuf1Len = strlen(tempBuffer2);

	if ((tempBuf1Len
			!= fwrite(tempBuffer2, sizeof(char), tempBuf1Len, netHeader)))
	{
		ex.OCFMException(OCFM_ERR_FILE_CANNOT_OPEN);
		delete[] tempBuffer2;
		throw ex;
	}
	delete[] tempBuffer2;

}

INT32 GroupNETHeaderContents(ProcessImage piObject[], INT32 noOfVars,
		PIDirectionType dirType, FILE* netHeader)
{

	INT32 totalSize = 0;

	if (0 != noOfVars)
	{
		//iModuleNo = 0;
		INT32 holeFilledIdNo = 1;
		NodeCollection* objNodeCol = NULL;
		Node* objNode = NULL;
		objNodeCol = NodeCollection::GetNodeColObjectPointer();

		for (INT32 noVarsLC = 0; noVarsLC < noOfVars; noVarsLC++)
		{
			objNode = objNodeCol->GetNodePtr(CN, piObject[noVarsLC].nodeId);
			//INT32 iNodeId;
			INT32 dataSize = 0;
			char* nodeIdStr = new char[50];
			char* moduleName = new char[50];
			char* moduleNo = new char[20];

			dataSize = piObject[noVarsLC].dataInfo.dataSize;
			/* Check if 8, 16, 32 bit aligned*/
			if ((0 == dataSize % 32) || (0 == dataSize % 16)
					|| (0 == dataSize % 8))
			{
				if (0 != totalSize % dataSize)
				{
					char* holeid = new char[20];
					char* filledBitsStr = new char[2 + ALLOC_BUFFER];
					INT32 filledBits = dataSize - (totalSize % dataSize);

					totalSize = totalSize + filledBits;
					for (INT32 fillBitLoopCount = 1;
							fillBitLoopCount <= (filledBits / 8);
							fillBitLoopCount++)
					{
						strcpy(holeid, "");
						strcpy(filledBitsStr, "");
						NETProcessImage objNETProcessImage;
						objNETProcessImage.Initialize();
						holeid = IntToAscii(holeFilledIdNo, holeid, 10);
						objNETProcessImage.name = new char[strlen(holeid)
								+ ALLOC_BUFFER];
						strcpy(objNETProcessImage.name, holeid);
						objNETProcessImage.moduleName = new char[strlen(
								"PADDING_VAR") + ALLOC_BUFFER];
						strcpy(objNETProcessImage.moduleName, "PADDING_VAR");

						objNETProcessImage.count = 0;
						objNETProcessImage.dataInfo.iecDtVar = BYTE;
						objNETProcessImage.dataInfo.dataSize = 8;
						objNETProcessImage.dataInfo.dtName = new char[strlen(
								"byte") + ALLOC_BUFFER];
						strcpy(objNETProcessImage.dataInfo.dtName, "byte");
						objNETProcessImage.totalDataSize = 8;
						objNETProcessImage.directionType = dirType;
						objNode->AddNETProcessImage(objNETProcessImage);
						holeFilledIdNo = holeFilledIdNo + 1;
					}
					delete[] filledBitsStr;
					delete[] holeid;
				}
			}

			nodeIdStr = IntToAscii(piObject[noVarsLC].nodeId, nodeIdStr, 10);
			strcpy(moduleNo, SubString(piObject[noVarsLC].moduleIndex, 2, 2));
			strcpy(moduleName, piObject[noVarsLC].moduleName);

			char* varName = new char[100];
			strcpy(varName, "CN");
			nodeIdStr = IntToAscii(piObject[noVarsLC].nodeId, nodeIdStr, 10);
			strcat(varName, nodeIdStr);
			strcat(varName, "_");

			/* Add Mod NO*/
			strcat(varName, "M");
			strcat(varName, moduleNo);
			strcat(varName, "_");
			strcat(varName, moduleName);

			if (piObject[noVarsLC].dataInfo.iecDtVar == BITSTRING)
			{
				INT32 piPos = 0;
				bool netPICreated = false;
				do
				{
					INT32 pos = 0;
					pos = SearchModuleNameNETProcessImageCollection(
							piObject[noVarsLC].nodeId, piPos, varName);
					if (-1 == pos)
					{
						//create new
						netPICreated = true;
						NETProcessImage objNETProcessImage;
						objNETProcessImage.Initialize();
						CopyPItoNETPICollection(piObject[noVarsLC],
								objNETProcessImage, varName);

					}
					else if (8 > objNode->NETPIColl[pos].totalDataSize)
					{
						netPICreated = true;
						objNode->NETPIColl[pos].totalDataSize =
								objNode->NETPIColl[pos].totalDataSize
										+ dataSize;
						objNode->NETPIColl[pos].count++;
						objNode->NETPIColl[pos].lastName = new char[strlen(
								piObject[noVarsLC].varDeclName) + ALLOC_BUFFER];
						strcpy(objNode->NETPIColl[pos].lastName,
								piObject[noVarsLC].varDeclName);
						if (8 == objNode->NETPIColl[pos].totalDataSize)
						{
							objNode->NETPIColl[pos].dataInfo.iecDtVar = BYTE;
							objNode->NETPIColl[pos].dataInfo.dtName =
									new char[strlen("byte") + ALLOC_BUFFER];
							strcpy(objNode->NETPIColl[pos].dataInfo.dtName,
									"byte");
						}
						else if (16 == objNode->NETPIColl[pos].totalDataSize)
						{
							objNode->NETPIColl[pos].dataInfo.iecDtVar = UINT;
							objNode->NETPIColl[pos].dataInfo.dtName =
									new char[strlen("UInt16") + ALLOC_BUFFER];
							strcpy(objNode->NETPIColl[pos].dataInfo.dtName,
									"UInt16");
						}
						else if (32 == objNode->NETPIColl[pos].totalDataSize)
						{
							objNode->NETPIColl[pos].dataInfo.iecDtVar = UDINT;
							objNode->NETPIColl[pos].dataInfo.dtName =
									new char[strlen("UInt32") + ALLOC_BUFFER];
							strcpy(objNode->NETPIColl[pos].dataInfo.dtName,
									"UInt32");
						}
						else
						{
							//TODO: else added. Operation to be specified
						}
					}
					else
					{
						piPos = pos + 1;
					}
				} while (false == netPICreated);
			}
			else
			{
				//copy as it is
				NETProcessImage objNETPI;
				objNETPI.Initialize();
				CopyPItoNETPICollection(piObject[noVarsLC], objNETPI, varName);
			}

			totalSize = dataSize + totalSize;

			delete[] varName;
			delete[] moduleName;
			delete[] moduleNo;
			delete[] nodeIdStr;
		}
		if (0 != totalSize % 32)
		{
			char* holeid = new char[20];
			char* filledBitStr = new char[2 + ALLOC_BUFFER];
			INT32 filledBitsVal = 32 - (totalSize % 32);

			totalSize = totalSize + filledBitsVal;
			for (INT32 fillBitsLC = 1; fillBitsLC <= (filledBitsVal / 8);
					fillBitsLC++)
			{
				strcpy(holeid, "");
				strcpy(filledBitStr, "");
				NETProcessImage objNETPI;
				holeid = IntToAscii(holeFilledIdNo, holeid, 10);
				objNETPI.name = new char[strlen(holeid) + ALLOC_BUFFER];
				strcpy(objNETPI.name, holeid);
				objNETPI.moduleName = new char[strlen("PADDING_VAR")
						+ ALLOC_BUFFER];
				strcpy(objNETPI.moduleName, "PADDING_VAR");

				objNETPI.count = 0;
				objNETPI.dataInfo.iecDtVar = BYTE;
				objNETPI.dataInfo.dataSize = 8;
				objNETPI.dataInfo.dtName = new char[strlen("byte")
						+ ALLOC_BUFFER];
				strcpy(objNETPI.dataInfo.dtName, "byte");
				objNETPI.totalDataSize = 8;
				objNETPI.directionType = dirType;
				objNode->AddNETProcessImage(objNETPI);
				holeFilledIdNo = holeFilledIdNo + 1;
			}
			delete[] filledBitStr;
			delete[] holeid;
		}
	}

	totalSize = totalSize / 8;

	return totalSize;
}

void SetSIdxDataType(DataType *objDataType, char* idxId, char* sIdxId)
{
	if ((NULL == objDataType) || (NULL == idxId) || (NULL == sIdxId))
	{
		ocfmException objException;
		objException.OCFMException(OCFM_ERR_INVALID_PARAMETER);
		cout << "INVALID_PARAMETER" << __FUNCTION__ << __LINE__ << endl;
		throw objException;
	}

	NodeCollection *nodeCollObj = NULL;
	IndexCollection *idxCollObj = NULL;
	Index *idxObj = NULL;
	SubIndex *sIdxObj = NULL;
	Node nodeObj;

	nodeCollObj = NodeCollection::GetNodeColObjectPointer();

	nodeObj = nodeCollObj->GetMNNode();
	idxCollObj = nodeObj.GetIndexCollection();

	idxObj = idxCollObj->GetIndexbyIndexValue(idxId);
	sIdxObj = idxObj->GetSubIndexbyIndexValue(sIdxId);

	sIdxObj->SetDataTypeST(*objDataType);
}

void AddPDOIndexsToMN(char* indexId, char* sIdxId, PDOType pdoTypeVal)
{
	ocfmRetCode stRetCode;
	Node objMNNode;
	Index *objIndex = NULL;
	NodeCollection *objNodeCol = NULL;
	IndexCollection *objIdxCol = NULL;
	DataTypeCollection *objDTCol = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objMNNode = objNodeCol->GetNode(MN, MN_NODEID);
	objDTCol = objMNNode.GetDataTypeCollection();

	stRetCode = AddIndex(MN_NODEID, MN, indexId);
	objIdxCol = objMNNode.GetIndexCollection();
	if (NULL != objIdxCol)
	{
		objIndex = objIdxCol->GetIndexbyIndexValue(indexId);
		if ((OCFM_ERR_INDEX_ALREADY_EXISTS != stRetCode.code)
				&& (NULL != objIndex))
		{
			char objName[14 + ALLOC_BUFFER];
			objIndex->SetObjectType((char*) "ARRAY");
			//name from network point of view
			if (PDO_RPDO == pdoTypeVal)
			{
				strcpy(objName, "PI_INPUTS_A");
				strcat(objName, GetPIName(indexId));
				objIndex->SetName(objName);
			}
			else if (PDO_TPDO == pdoTypeVal)
			{
				strcpy(objName, "PI_OUTPUTS_A");
				strcat(objName, GetPIName(indexId));
				objIndex->SetName(objName);
			}
			else
			{
				cout << "enumPdoType is not TPDO or RPDO _1\n" << endl;
			}
			/* Add subindex 00 */
			stRetCode = AddSubIndex(MN_NODEID, MN, indexId, (char*) "00");
			if (OCFM_ERR_SUCCESS != stRetCode.code)
			{
				ocfmException objException;
				objException.OCFMException(stRetCode.code);
				throw objException;
			}
		}
	}

	stRetCode = AddSubIndex(MN_NODEID, MN, indexId, sIdxId);
	if (OCFM_ERR_SUCCESS != stRetCode.code)
	{
		cout << __LINE__ << " AddSubIndex error in line" << endl;
		ocfmException objException;
		objException.OCFMException(stRetCode.code);
		throw objException;
	}

	if (NULL != objIndex)
	{
		SubIndex *objSIdx = NULL;
		objSIdx = objIndex->GetSubIndexbyIndexValue(sIdxId);
		if (NULL != objSIdx)
		{
			char objectName[14 + ALLOC_BUFFER];
			char *pdoMap = new char[4 + ALLOC_BUFFER];
			objSIdx->SetObjectType((char*) "VAR");
			/* Its reversed because CN's RPDO is MN's TPDO*/
			if (PDO_RPDO == pdoTypeVal)
			{
				strcpy(pdoMap, "TPDO");
				strcpy(objectName, "PI_INPUTS_");
				strcat(objectName, GetPIName(indexId));
				objSIdx->SetAccessType((char*) "ro");

				objSIdx->SetName(objectName);
				objSIdx->SetPDOMapping(pdoMap);
			}
			else if (PDO_TPDO == pdoTypeVal)
			{
				strcpy(pdoMap, "RPDO");
				strcpy(objectName, "PI_OUTPUTS_");
				strcat(objectName, GetPIName(indexId));
				objSIdx->SetAccessType((char*) "rw");

				objSIdx->SetName(objectName);
				objSIdx->SetPDOMapping(pdoMap);
			}
			else
			{
				cout << "enumPdoType is not TPDO or RPDO _2\n" << endl;
			}
			delete[] pdoMap;
		}
	}

	char *tempDtName = NULL;
	DataType *objDataType = NULL;

	tempDtName = GetPIDataTypeName(indexId);

	if (NULL != tempDtName)
	{
		objDataType = objDTCol->GetDataTypeByName(tempDtName);
	}

	if ((NULL != objDataType) && (NULL != indexId) && (NULL != sIdxId))
	{
		SetSIdxDataType(objDataType, indexId, sIdxId);
	}

	if (NULL != objIndex)
	{
		UpdateNumberOfEnteriesSIdx(objIndex, MN);
	}
}

PIObject GetPIAddress(PDODataType dtType, PIDirectionType dirType,
		INT32 offsetVal, INT32 dataSizeBits)
{
	PIObject stPIObject;
	stPIObject.Initialize();
#if defined DEBUG
	cout << __FUNCTION__ << ":: dtType:" << dtType << " dirType:" << dirType << " offsetVal:" << offsetVal << " dataSizeBits:" << dataSizeBits << endl;
#endif
	stPIObject.indexId = new char[INDEX_LEN];
	stPIObject.sIdxId = new char[SUBINDEX_LEN];

	for (INT32 idx = 0; idx < NO_OF_PI_ENTERIES; idx++)
	{
		if ((piIndexTable[idx].dtObj == dtType)
				&& (piIndexTable[idx].direction == dirType))
		{
			INT32 subIndex;
			INT32 dataSizeBytes = dataSizeBits / 8;
			subIndex = (offsetVal / dataSizeBytes) + 1;
			if (subIndex > 254)
			{
				subIndex--;
				INT32 div = subIndex / 254;
				INT32 mod = (subIndex % 254) + 1;

				INT32 addressVal;
				addressVal = HexToInt(piIndexTable[idx].addressStr);
				addressVal = addressVal + div;
				stPIObject.indexId = IntToAscii(addressVal, stPIObject.indexId, 16);
				stPIObject.sIdxId = IntToAscii(mod, stPIObject.sIdxId, 16);
				stPIObject.sIdxId = PadLeft(stPIObject.sIdxId, '0', 2);

				#if defined DEBUG
				cout << "indexId: "<<stPIObject.indexId<<" sIdxId:"<<stPIObject.sIdxId<<endl;
				#endif

			}
			else
			{
				strcpy(stPIObject.indexId, piIndexTable[idx].addressStr);
				stPIObject.sIdxId = IntToAscii(subIndex, stPIObject.sIdxId, 16);
				stPIObject.sIdxId = PadLeft(stPIObject.sIdxId, '0', 2);

			}
		}
	}
	//Handled error case and returned dummy value to avoid warning
	return stPIObject;
}

char* GetPIDataTypeName(char* indexId)
{
	char *retDataType = NULL;
	PDODataType tempDataType;
	for (INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{
		if (0 == strcmp(piIndexTable[iLoopCount].addressStr, indexId))
		{
			tempDataType = piIndexTable[iLoopCount].dtObj;
			break;
		}
		else if (0 < strcmp(piIndexTable[iLoopCount].addressStr, indexId))
		{
			tempDataType =
					(0 < iLoopCount) ?
							piIndexTable[iLoopCount - 1].dtObj :
							static_cast<PDODataType>(-1);
			break;
		}
		else
		{
			//TODO: else added. operation to be specified
		}
	}
	switch (tempDataType)
	{
	case UNSIGNED8:
	{
		retDataType = (char*) "Unsigned8";
		break;
	}
	case INTEGER8:
	{
		retDataType = (char*) "Integer8";
		break;
	}
	case UNSIGNED16:
	{
		retDataType = (char*) "Unsigned16";
		break;
	}
	case INTEGER16:
	{
		retDataType = (char*) "Integer8";
		break;
	}
	case UNSIGNED32:
	{
		retDataType = (char*) "Unsigned32";
		break;
	}
	case INTEGER32:
	{
		retDataType = (char*) "Integer32";
		break;
	}
	case INTEGER64:
	{
		retDataType = (char*) "Integer64";
		break;
	}
	case UNSIGNED64:
	{
		retDataType = (char*) "Unsigned64";
		break;
	}
	default:
	{
		//Handled error case and returned dummy value to avoid warning
		retDataType = (char*) "Error";
		break;
	}
	}

	return retDataType;
}

char* GetPIName(char* indexId)
{
	char *retPiName = NULL;
	PDODataType tempDataType;
	for (INT32 iLoopCount = 0; iLoopCount < NO_OF_PI_ENTERIES; iLoopCount++)
	{
		if (0 == strcmp(piIndexTable[iLoopCount].addressStr, indexId))
		{
			tempDataType = piIndexTable[iLoopCount].dtObj;
			break;
		}
		else if (0 < strcmp(piIndexTable[iLoopCount].addressStr, indexId))
		{
			tempDataType =
					(0 < iLoopCount) ?
							piIndexTable[iLoopCount - 1].dtObj :
							static_cast<PDODataType>(-1);
			break;
		}
		else
		{
			//TOOD: else added, operation to be specified
		}
	}
	switch (tempDataType)
	{
	case UNSIGNED8:
	{
		retPiName = (char*) "U8";
		break;
	}
	case INTEGER8:
	{
		retPiName = (char*) "I8";
		break;
	}
	case UNSIGNED16:
	{
		retPiName = (char*) "U16";
		break;
	}
	case INTEGER16:
	{
		retPiName = (char*) "I16";
		break;
	}
	case UNSIGNED32:
	{
		retPiName = (char*) "U32";
		break;
	}
	case INTEGER32:
	{
		retPiName = (char*) "I32";
		break;
	}
	case UNSIGNED64:
	{
		retPiName = (char*) "U64";
		break;
	}
	case INTEGER64:
	{
		retPiName = (char*) "I64";
		break;
	}

	default:
	{
		//Handled error case and returned dummy value to avoid warning
		retPiName = (char*) "Err";
		break;
	}
	}

	return retPiName;
}

bool CheckIfProcessImageIdx(char* idxId)
{
	return ((0 <= strcmp(idxId, "A000")) && (0 >= strcmp(idxId, "AFFF")));
}

INT32 SearchModuleNameNETProcessImageCollection(INT32 nodeId, INT32 itemNo,
		char* moduleName)
{

	NodeCollection* objNodeCol = NULL;
	Node* objNode;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNodePtr(CN, nodeId);

	INT32 itemTotal = objNode->NETPIColl.Count();
	for (; itemNo < itemTotal; itemNo++)
	{
		if (NULL == objNode->NETPIColl[itemNo].moduleName)
		{
			continue;
		}

		if (0 == strcmp(objNode->NETPIColl[itemNo].moduleName, moduleName))
		{
			return itemNo;
		}

	}
	return -1;
}

void CopyPItoNETPICollection(ProcessImage piObj, NETProcessImage netPIObj,
		char* moduleName)
{
	NodeCollection* objNodeCol = NULL;
	Node* objNode;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objNode = objNodeCol->GetNodePtr(CN, piObj.nodeId);

	if (NULL != piObj.varDeclName)
	{
		netPIObj.name = new char[strlen(piObj.varDeclName) + ALLOC_BUFFER];
		strcpy(netPIObj.name, piObj.varDeclName);
	}
	netPIObj.moduleName = new char[strlen(moduleName) + ALLOC_BUFFER];
	strcpy(netPIObj.moduleName, moduleName);
	netPIObj.dataInfo.iecDtVar = piObj.dataInfo.iecDtVar;
	netPIObj.dataInfo.dataSize = piObj.dataInfo.dataSize;
	if (NULL != piObj.dataInfo.dtName)
	{
		netPIObj.dataInfo.dtName = new char[strlen(piObj.dataInfo.dtName)
				+ ALLOC_BUFFER];
		strcpy(netPIObj.dataInfo.dtName, piObj.dataInfo.dtName);
	}
	netPIObj.nodeId = piObj.nodeId;
	netPIObj.directionType = piObj.directionType;
	netPIObj.count = 0;
	netPIObj.totalDataSize = piObj.dataInfo.dataSize;

	objNode->AddNETProcessImage(netPIObj);
}

char* GetDatatypeNETPI(IEC_Datatype dtIEC)
{
	char* retIECdt;
	switch (dtIEC)
	{
	case BITSTRING:
		retIECdt = (char*) "";
		break;
	case BOOL:
		retIECdt = (char*) "byte";
		break;
	case BYTE:
		retIECdt = (char*) "byte";
		break;
	case _CHAR:
		retIECdt = (char*) "byte";
		break;
	case DWORD:
		retIECdt = (char*) "UInt32";
		break;
	case LWORD:
		retIECdt = (char*) "UInt64";
		break;
	case SINT:
		retIECdt = (char*) "sbyte";
		break;
	case INT:
		retIECdt = (char*) "Int16";
		break;
	case DINT:
		retIECdt = (char*) "Int32";
		break;
	case LINT:
		retIECdt = (char*) "Int64";
		break;
	case USINT:
		retIECdt = (char*) "byte";
		break;
	case UINT:
		retIECdt = (char*) "UInt16";
		break;
	case UDINT:
		retIECdt = (char*) "UInt32";
		break;
	case ULINT:
		retIECdt = (char*) "UInt64";
		break;
	case REAL:
		retIECdt = (char*) "";
		break;
	case LREAL:
		retIECdt = (char*) "";
		break;
	case STRING:
		retIECdt = (char*) "";
		break;
	case WSTRING:
		retIECdt = (char*) "";
		break;
	default:
		retIECdt = (char*) "";
		break;
	}
	return retIECdt;
}

INT32 GetDatasizeNETPI(IEC_Datatype dtIEC)
{
	INT32 retIECdtVal = 0;
	switch (dtIEC)
	{
	case BOOL:
	case BYTE:
	case _CHAR:
	case SINT:
	case USINT:
		retIECdtVal = 1;
		break;
	case INT:
	case UINT:
		retIECdtVal = 2;
		break;
	case DINT:
	case DWORD:
	case UDINT:
		retIECdtVal = 4;
		break;
	case LINT:
	case LWORD:
	case ULINT:
		retIECdtVal = 8;
		break;
	default:
		break;
	}
	return retIECdtVal;
}
