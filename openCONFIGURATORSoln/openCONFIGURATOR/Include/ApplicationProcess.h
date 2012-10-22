#ifndef ApplicationProcess_h
#define ApplicationProcess_h
///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
//
// NAME:  Application Process
//
// BASE  CLASSES: none
//  
// PURPOSE:  This class has the collection of all the objects within the Application Process tag 
// xdd/xdc file.
//
// AUTHOR:  Monica B
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
#pragma once
#include <string>

using namespace std;
#include "ComplexDataType.h"
#include "TCollection.h"
class DllExport CApplicationProcess
	{
	public:
		CApplicationProcess(void);
		~CApplicationProcess(void);
	public:
		char* XDDfilename;
		char* ProjectPath;
		TCollection<CComplexDataType> CDTCollection;
		TCollection<Parameter> ParameterCollection;

	public:
		void ParseXDDfile(char* filename);
		int checkFileStatus(char* filename);		
		void addComplexDataType(CComplexDataType complexDT);
		void addParameter(Parameter  parameter);
		int get_ParameterIndexby_UniqueIDRef(char* UniqueIdRef);
		Parameter* get_Parameterby_UniqueIDRef(char* UniqueIdRef);	
		CComplexDataType* getCDTbyUniqueID(char* UniqueId);
		CComplexDataType* getCDTbydt_UniqueRefID(char* UniqueRefId);
		CComplexDataType* getCDTbydtIndex(int Index);
		int get_CDT_UniqueIDRef(char *UniqueIdRef);
		void updatePreviousCDT_UId(char *UniqueID, int Index);
		Parameter get_UniqueIDRef_by_ParameterIndex(int ParameterIndex);
		int getCDTCount();
		CComplexDataType* getCDTbyCount(int count);
	};

#endif // ApplicationProcess_h

