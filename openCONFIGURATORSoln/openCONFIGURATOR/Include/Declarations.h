#ifndef declarations_h
#define declarations_h
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
#include <string.h>
#include <stdlib.h>
#include "Exports.h"
#include "Error.h"

//using namespace std;

/************************************************************************************************
Constants
************************************************************************************************/
const int MN_NODEID=240;

/************************************************************************************************
Enumerations
************************************************************************************************/

typedef enum 
{	
				DEFTYPE				= 5,
				DEFSTRUCT		= 6,
				VAR								= 7,
				ARRAY						= 8,
				RECORD					= 9	
}EObjectType;

DllExport typedef enum 
{
				NO,
				DEFAULT,
				OPTIONAL,
				RPDO,
				TPDO									
	}EPDOMapping;
typedef EPDOMapping *e_PDOMapping;
typedef enum 
{
				MN					= 0,
				CN					= 1
}ENodeType;

typedef enum
{
				PDO_TPDO				= 1,
				PDO_RPDO				= 2
}EPDOType;
typedef enum
{
	
			 constant,
			 ro,			 /* read access only (default value)*/
			 wr,			 /* write access only */
			 rw,	 /* both read and write access*/
			 readWriteInput, /*both read and write access, but represents process input data*/
			 readWriteOutput, /*both read and write, access, but represents process output data*/
			 noAccess				 /*access denied				*/
}EParameterAccess;
struct DataType
{
		char*		Name;
		char*		DataTypeValue;
		char*		DataSize;
		char* getName(){return Name;}
		void setName(char* DataTypeName)
		{
			Name = new char[sizeof(DataTypeName)];
			strcpy(Name,DataTypeName);	
		}
};
typedef struct ProcessImage
{
		char*					Name;
		char*					Direction;
		DataType		dataType;
		int							DataSize;
		int							ByteOffset;
		char*					Value;
		char*					subindex;
		char*					Index;
		char*					BitOffset;
	
}ProcessImage;
typedef struct Parameter
{
		char*						Name;
		char*						UniqueID;
		char*						dataType;
			
}Parameter;

typedef struct varDeclaration
{

		char*						Name;
		char*						UniqueID;
		int								size;
		char*						InitialValue;
		char*						dataType;
		char*						dataTypeIDRef;
	
}varDeclaration;


/****************************************************************************************/
/* Function Declarations
/*****************************************************************************************/
DllExport ocfmRetCode ImportXML(char* fileName, int NodeID, ENodeType NodeType);
DllExport ocfmRetCode ImportObjDictXML(char* fileName);
DllExport void GenerateCDC(char* fileName);
//void GenerateMNOBD();
DllExport ocfmRetCode CreateNode(int NodeID, ENodeType NodeType);
DllExport void parseFile(char* filename, int NodeID, ENodeType  NodeType);
DllExport ocfmRetCode DeleteNode(int NodeID, ENodeType NodeType);
//void DeleteNode(int NodePos);
DllExport ocfmRetCode DeleteIndex(int NodeID, ENodeType NodeType, char* IndexID);
DllExport ocfmRetCode DeleteSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
DllExport ocfmRetCode AddIndex(int NodeID, ENodeType NodeType, char* IndexID);
DllExport ocfmRetCode AddSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID);
DllExport ocfmRetCode EditIndex(int NodeID, ENodeType NodeType, char* IndexID, char* IndexValue, char* IndexName);
DllExport ocfmRetCode EditSubIndex(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, char* IndexValue, char* IndexName);
DllExport void DisplayNodeTree(void);
DllExport ocfmRetCode IfNodeExists(int NodeID, ENodeType NodeType, int* NodePos);
DllExport int ocfmRetValError_getErrorCode(ocfmRetValError RetValError);
DllExport ocfmRetCode IfIndexExists(int NodeID, ENodeType NodeType, char* IndexID, int* IndexPos);
DllExport ocfmRetCode IfSubIndexExists(int NodeID, ENodeType NodeType, char* IndexID, char* SubIndexID, int* SubIndexPos, int* IndexPos);
DllExport ocfmRetCode ReImportXML(char* fileName, int NodeID, ENodeType NodeType);
DllExport int ocfmRetValError_getRetValue(ocfmRetValError RetValError);

#endif // declarations_h
