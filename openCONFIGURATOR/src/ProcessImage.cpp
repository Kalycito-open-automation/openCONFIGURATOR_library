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

#include "../Include/Internal.h"
#include "../Include/Exception.h"
#include "../Include/ProcessImage.h"
#include "../Include/NodeCollection.h"
#include "../Include/Logging.h"
#include "../Include/Result.h"
#include "../Include/BoostShared.h"

using namespace openCONFIGURATOR::Library::ErrorHandling;

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

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

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

		for (UINT32 piLoopCount = 0; piLoopCount < objNode->PICollection.size();
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

		for (UINT32 iInLoopCount = 0; iInLoopCount < objNode->NETPIColl.size();
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

		for (UINT32 iInLoopCount = 0;
		        iInLoopCount < objNode->PICollection.size(); iInLoopCount++)
		{
			INT32 uniqNameCnt = 1;
			bool nameMatched = false;

			piObjTemp1 = &objNode->PICollection[iInLoopCount];
			if (piObjTemp1 == NULL)
			{
				LOG_WARN() << "Local variable 'piObjTemp1' must not be NULL for node " << objNode->GetNodeId() << "." ;
				continue;
			}
			//it is possible that the changed var name matching a previous entry
			for (UINT32 iInChkLoopCount = 0;
			        iInChkLoopCount < objNode->PICollection.size();
			        iInChkLoopCount++)
			{
				if (iInChkLoopCount == iInLoopCount)
				{
					//Both the index are same, do not check same object
					continue;
				}

				piObjTemp2 = &objNode->PICollection[iInChkLoopCount];
				if (piObjTemp2 == NULL)
				{
					LOG_WARN() << "Local variable 'piObjTemp2' must not be NULL for node " << objNode->GetNodeId() << "." ;
					continue;
				}
				//check module index, module name, directiontype and variable name
				//if all are same then append count variable to variable name
				if ((0 == strcmp(piObjTemp1->moduleIndex, piObjTemp2->moduleIndex))
				        && (0 == strcmp(piObjTemp1->moduleName, piObjTemp2->moduleName))
				        && (piObjTemp1->directionType == piObjTemp2->directionType)
				        && ((piObjTemp1->varDeclName != NULL) && (NULL != piObjTemp2->varDeclName)))
				{
					if ((0 == strcmp(piObjTemp1->varDeclName, piObjTemp2->varDeclName)))
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

PIDataInfo* GetIECDT(const char* iecDataType, INT32 dataSize)
{
	PIDataInfo* stDataInfo = new PIDataInfo;
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
			//TODO: verify spec
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
			//TODO: verify spec
			stDataInfo->dataSize = dataSize;
			stDataInfo->iecDtVar = WSTRING;
		}
	}
	return stDataInfo;
}

void GenerateXAPHeaderFile(const char* fileName, ProcessImage piInCol[],
                           ProcessImage piOutCol[], INT32 inVar, INT32 outVar)
{
	string xapFileName = fileName;
	xapFileName.append(".h");
	ofstream xapFile;
	xapFile.open(xapFileName.c_str(), ios::out);
	ocfmRetCode ex;

	if (!xapFile.is_open())
	{
		boost::format formatter(kMsgFileReadFailed);
		formatter % xapFileName;
		ex.setErrorCode(OCFM_ERR_CANNOT_OPEN_FILE);
		ex.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw ex;
	}

	//write comment
	stringstream buffer;
	buffer << "/* " << BUILD_COMMENT << GetBuildTime() << " */\n";

	//write include guard for the headerfile
	buffer << INCLUDE_GUARD_CHECK << "\n" << INCLUDE_GUARD_START << "\n\n";
	xapFile << buffer.str();

	/* write Input structure */
	if (0 != inVar)
	{
		WriteXAPHeaderContents(piInCol, inVar, INPUT, xapFile);
	}

	/* write Output structure */
	if (0 != outVar)
	{
		WriteXAPHeaderContents(piOutCol, outVar, OUTPUT, xapFile);
	}

	//close include guard for the XAP header file
	xapFile << "\n" << INCLUDE_GUARD_END;

	if(xapFile.bad())
	{
		boost::format formatter(kMsgFileWriteFailed);
		formatter % xapFileName;
		ex.setErrorCode(OCFM_ERR_FILE_CANNOT_OPEN);
		ex.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		xapFile.close();
		throw ex;
	}

	xapFile.close();
}

void WriteXAPHeaderContents(ProcessImage piObj[], INT32 noOfVars,
                            PIDirectionType directionType, ofstream& xapHeader)
{
	stringstream mainBuffer;
	ocfmRetCode ex;

	if (0 != noOfVars)
	{
		if (INPUT == directionType)
		{
			mainBuffer << "# define COMPUTED_PI_OUT_SIZE ";
		}
		else if (OUTPUT == directionType)
		{
			mainBuffer << "\n\n# define COMPUTED_PI_IN_SIZE ";
		}

		stringstream varSection;
		varSection << "\ntypedef struct \n{\n";
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
					varSection << "\tunsigned PADDING_VAR_" << IntToAscii(holeFilledIdNo, holeid, 10) << ":" << IntToAscii(filledBits, fillBitsStr, 10) << fillBitsStr << ";\n";
					holeFilledIdNo = holeFilledIdNo + 1;
					delete[] fillBitsStr;
				}
			}

			//iNodeId = objProcessImage[iLoopCount].CNNodeID;

			nodeId = IntToAscii(piObj[loopCount].nodeId, nodeId, 10);
			SubString(moduleNo, (const char*) piObj[loopCount].moduleIndex, 2, 2);
			//strcpy(moduleNo, SubString(piObj[loopCount].moduleIndex, 2, 2));
			strcpy(moduleName, piObj[loopCount].moduleName);

			varSection << "\tunsigned CN";

			nodeId = IntToAscii(piObj[loopCount].nodeId, nodeId, 10);
			varSection << nodeId << "_M" << moduleNo << "_" << moduleName;
			if (piObj[loopCount].varDeclName != NULL)
			{
				varSection << "_" << piObj[loopCount].varDeclName;
			}

			varSection << ":";

			char* varTempBuff = new char[50];
			varTempBuff = IntToAscii(dataSize, varTempBuff, 10);
			totalSize = dataSize + totalSize;
			varSection << varTempBuff << ";\n";

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
			varSection << "\tunsigned PADDING_VAR_" << IntToAscii(holeFilledIdNo, holeid, 10) << ":";
			fillBitsStr = IntToAscii(filledBits, fillBitsStr, 10);
			varSection << fillBitsStr << ";\n";
			//iHoleFilledIdNo = iHoleFilledIdNo + 1; Unused operation

			delete[] fillBitsStr;
		}
		varSection << "}";

		char* strSize = new char[50];
		/* write the size in bytes*/
		totalSize = totalSize / 8;
		strSize = IntToAscii(totalSize, strSize, 10);
		mainBuffer << strSize;
		delete[] strSize;

		if (directionType == INPUT)
		{
			varSection << " PI_OUT;";
		}
		else if (directionType == OUTPUT)
		{
			varSection << " PI_IN;\n";
		}
		mainBuffer << varSection.str();
	}
	else
	{
		mainBuffer << "";
	}

	xapHeader << mainBuffer.str();
}

void GenerateNETHeaderFile(const char* fileName, ProcessImage piInCol[],
                           ProcessImage piOutCol[], INT32 iInVar, INT32 iOutVar)
{
	string netFileName = fileName;
	netFileName.append(".cs");
	std::ofstream netFile;
	netFile.open(netFileName.c_str(), ios::out);
	ocfmRetCode ex;

	/* write Input structure */
	if (!netFile.is_open())
	{
		boost::format formatter(kMsgFileReadFailed);
		formatter % netFileName;
		ex.setErrorCode(OCFM_ERR_CANNOT_OPEN_FILE);
		ex.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw ex;
	}

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
		/*Writing Header of .NET Interface*/
		stringstream mainBuffer;
		mainBuffer << "using System;\nusing System.Runtime.InteropServices;\n/// <summary>\n/// " << BUILD_COMMENT << GetBuildTime() << "\n/// </summary>\n\nnamespace openPOWERLINK\n{\n";
		netFile << mainBuffer.str();
		if (netFile.bad())
		{
			boost::format formatter(kMsgFileWriteFailed);
			formatter % netFileName;
			ex.setErrorCode(OCFM_ERR_FILE_CANNOT_OPEN);
			ex.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			netFile.close();
			throw ex;
		}
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
		netFile << "}\n";
		if (netFile.bad())
		{
			boost::format formatter(kMsgFileWriteFailed);
			formatter % netFileName;
			ex.setErrorCode(OCFM_ERR_FILE_CANNOT_OPEN);
			ex.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			netFile.close();
			throw ex;
		}

	}
	netFile.close();
}

void WriteNETHeaderContents(ProcessImage piObj[], INT32 noOfVars,
                            PIDirectionType dirType, ofstream& netHeader)
{
	stringstream mainBuffer;

	char offsetStr[10] = { 0 };
	UINT32 offsetVal = 0;
	NETProcessImage* objNetPiCol = NULL;
	INT32 totalSizeVal = GroupNETHeaderContents(piObj, noOfVars, dirType);
	NodeCollection* objNodeCol = NULL;
	objNodeCol = NodeCollection::GetNodeColObjectPointer();

	objNetPiCol = new NETProcessImage[(objNodeCol->GetCNNodesCount() * PI_VAR_COUNT)];
	INT32 netPIVarsCount = GroupNETPIVariables(dirType, objNetPiCol);
	for (INT32 loopCount = 0; loopCount < netPIVarsCount; loopCount++)
	{
		mainBuffer << "\t\t[FieldOffset(";
		IntToAscii(offsetVal, offsetStr, 10);
		mainBuffer << offsetStr << ")]\n\t\tpublic " << GetDatatypeNETPI(objNetPiCol[loopCount].dataInfo.iecDtVar);
		offsetVal += GetDatasizeNETPI(objNetPiCol[loopCount].dataInfo.iecDtVar);
		mainBuffer << " " << objNetPiCol[loopCount].moduleName;
		if (objNetPiCol[loopCount].name != NULL)
		{
			mainBuffer << "_" << objNetPiCol[loopCount].name;
			if ((objNetPiCol[loopCount].count > 0) && (objNetPiCol[loopCount].lastName != NULL))
			{
				mainBuffer << "_to_" << objNetPiCol[loopCount].lastName;
			}
		}
		mainBuffer << ";\n";
	}
	delete[] objNetPiCol;
	char* totalSize = new char[20];
	IntToAscii(totalSizeVal, totalSize, 10);
	stringstream header;

	if (INPUT == dirType)
	{
		header << "\n\t/// <summary>\n\t/// Struct : ProcessImage Out\n\t/// </summary>\n\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = " 
			<< totalSize << ")]\n\tpublic struct AppProcessImageOut\n\t{\n";
	}
	else if (OUTPUT == dirType)
	{
		header << "\n\t/// <summary>\n\t/// Struct : ProcessImage In\n\t/// </summary>\n\t[StructLayout(LayoutKind.Explicit, Pack = 1, Size = " 
			<< totalSize << ")]\n\tpublic struct AppProcessImageIn\n\t{\n";
	}

	header << mainBuffer.str() << "\t}\n";

	delete[] totalSize;

	netHeader << header.str();
	if (netHeader.bad())
	{
		boost::format formatter(kMsgFileWriteFailed);
		formatter % ".NET Header";
		ocfmRetCode ex;
		ex.setErrorCode(OCFM_ERR_FILE_CANNOT_OPEN);
		ex.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		netHeader.close();
		throw ex;
	}
}

INT32 GroupNETHeaderContents(ProcessImage piObject[], INT32 noOfVars,
                             PIDirectionType dirType)
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
			SubString(moduleNo, (const char*) piObject[noVarsLC].moduleIndex, 2, 2);
			//strcpy(moduleNo, SubString(piObject[noVarsLC].moduleIndex, 2, 2));
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
					}
					else
					{
						piPos = pos + 1;
					}
				}
				while (false == netPICreated);
			}
			else
			{
				//copy as it is
				NETProcessImage objNETPI;
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

void SetSIdxDataType(DataType* objDataType, char* idxId, char* sIdxId)
{
	if ((NULL == objDataType) || (NULL == idxId) || (NULL == sIdxId))
	{
		boost::format formatter(kMsgNullArgument);
		formatter % "'objDataType', 'idxId', 'sIdxId'";
		ocfmRetCode exceptionObj(OCFM_ERR_INVALID_PARAMETER);
		exceptionObj.setErrorString(formatter.str());
		LOG_FATAL() << formatter.str();
		throw exceptionObj;
	}

	NodeCollection* nodeCollObj = NULL;
	IndexCollection* idxCollObj = NULL;
	Index* idxObj = NULL;
	SubIndex* sIdxObj = NULL;
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
	Index* objIndex = NULL;
	NodeCollection* objNodeCol = NULL;
	IndexCollection* objIdxCol = NULL;
	DataTypeCollection* objDTCol = NULL;

	objNodeCol = NodeCollection::GetNodeColObjectPointer();
	objMNNode = objNodeCol->GetNode(MN, MN_NODEID);
	objDTCol = objMNNode.GetDataTypeCollection();

	stRetCode = AddIndex(MN_NODEID, MN, indexId);
	objIdxCol = objMNNode.GetIndexCollection();
	if (NULL != objIdxCol)
	{
		objIndex = objIdxCol->GetIndexbyIndexValue(indexId);
		if ((OCFM_ERR_INDEX_ALREADY_EXISTS != stRetCode.getErrorCode())
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
				LOG_ERROR() << "Local variable 'pdoTypeVal' is neither TPDO nor RPDO.";
			}
			/* Add subindex 00 */
			stRetCode = AddSubIndex(MN_NODEID, MN, indexId, (char*) "00");
			if (OCFM_ERR_SUCCESS != stRetCode.getErrorCode())
			{
				LOG_FATAL() << stRetCode.getErrorString();
				throw stRetCode;
			}
		}
	}

	stRetCode = AddSubIndex(MN_NODEID, MN, indexId, sIdxId);
	if (OCFM_ERR_SUCCESS != stRetCode.getErrorCode())
	{
		LOG_FATAL() << "AddSubIndex() returned error code " << stRetCode.getErrorCode() << ".";
		throw stRetCode;
	}

	if (NULL != objIndex)
	{
		SubIndex* objSIdx = NULL;
		objSIdx = objIndex->GetSubIndexbyIndexValue(sIdxId);
		if (NULL != objSIdx)
		{
			char objectName[14 + ALLOC_BUFFER];
			char* pdoMap = new char[4 + ALLOC_BUFFER];
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
				LOG_ERROR() << "Local variable 'pdoTypeVal' is neither TPDO nor RPDO.";
			}
			delete[] pdoMap;
		}
	}

	char* tempDtName = NULL;
	DataType* objDataType = NULL;

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
		UpdateNumberOfEnteriesSIdx(objIndex);
	}
}

PIObject GetPIAddress(PDODataType dtType, PIDirectionType dirType,
                      INT32 offsetVal, INT32 dataSizeBits)
{
	PIObject stPIObject;
	stPIObject.indexId = new char[INDEX_LEN];
	stPIObject.sIdxId = new char[SUBINDEX_LEN];

	for (INT32 idx = 0; idx < NO_OF_PI_ENTERIES; idx++)
	{
		if ((piIndexTable[idx].dtObj == dtType)
		        && (piIndexTable[idx].direction == dirType))
		{
			INT32 dataSizeBytes = dataSizeBits / 8;
			// offsetVal (offset in the process image) divided by size in bytes equals
			// expected subIndex. Since subIndex 0x00 (NrOfEntries) is reserved, increment by 1.
			INT32 subIndex = (offsetVal / dataSizeBytes) + 1;
			// If calculated subIndex > 252, calculate the correct next index
			if (subIndex > 252)
			{
				subIndex--;
				// An index has 252 subIndices to use, div equals the no. of indices
				// to increment.
				INT32 div = subIndex / 252;
				// mod equals the subIndex-offset within the resulting index, again increment
				// by 1 to skip subIndex 0x00
				INT32 mod = (subIndex % 252) + 1;

				// Calc. the new index
				INT32 addressVal = (INT32) HexToInt(piIndexTable[idx].addressStr);
				addressVal = addressVal + div;
				// Index is expected to be in hex-uppercase!
				stPIObject.indexId = ConvertToUpper(IntToAscii(addressVal, stPIObject.indexId, 16));
				stPIObject.sIdxId = IntToAscii(mod, stPIObject.sIdxId, 16);
				stPIObject.sIdxId = PadLeft(stPIObject.sIdxId, '0', 2);
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
	char* retDataType = NULL;
	PDODataType tempDataType = UNDEF;
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
	char* retPiName = NULL;
	PDODataType tempDataType = UNDEF;
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

bool CheckIfProcessImageIdx(const char* idxId)
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

	INT32 itemTotal = objNode->NETPIColl.size();
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
