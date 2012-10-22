#ifndef Internal_h
#define Internal_h
///////////////////////////////////////////////////////////////////////////////////////////////
//
// $Header: $
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
#include <string>
#include "Exports.h"
#include "Index.h"
#include <libxml/xmlwriter.h>
static const int g_simple_arr_size = 19;
static const char *g_Simple[] = {
																										"BOOL",
																										"BITSTRING",
																										"BYTE",
																										"CHAR",
																										"WORD",
																										"DWORD",
																										"LWORD",
																										"SINT",
																										"INT",
																										"DINT",
																										"LINT",
																										"USINT",
																										"UINT",
																										"UDINT",
																										"ULINT",
																										"REAL",
																										"LREAL",
																										"STRING",
																										"WSTRING"
																										};

/****************************************************************************************/
/* Function Declarations
/*****************************************************************************************/
void LoadObjectDictionary();
char* ConvertToUpper(char* str);
char* itoa( int value, char* result, int base );
bool CheckIfNotPDO(char* Index);
void CreateTree();
bool CheckIfNotPDO(char* Index);
void ProcessUniqueIDRefs();
char* subString(char* str, int startpos, int len);
char* reverse(char* str);
void ProcessPDONodes(int NodeID);
bool CheckIfManufactureSpecificObject(char* Index);
bool IsAscii(char c);
int lenOfCNBuffer(char* Buffer);
char* padLeft(char* str, char padChar, int padLength);
unsigned long hex2int(char *a);
bool CheckIfMappingPDO(char* Index);
CIndex* getMNIndexValues(char* Index);
bool CheckAllowedCNIndexes(char* IndexValue);
void ConvertCdcToBinary(char* fileName,char* tempFile);
void WriteXAPElements(ProcessImage pi, xmlTextWriterPtr& writer);
void StartXAPxml(xmlTextWriterPtr& writer,  xmlDocPtr& doc);
void EndWrtitingXAP( xmlTextWriterPtr& writer, char* fileName, xmlDocPtr& doc);



#endif // declarations_h
