/**
 *****************************************************************************************************
 \file			ProcessImage.h

 \brief			This Handles Function declarations for Process Image
 *****************************************************************************************************
 */

/*

 (c) Kalycito Infotech Private Limited

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

#ifndef processImage_h
#define processImage_h

/****************************************************************************************************/
/* Includes */

#include <iostream>
#include <fstream>
#include "Declarations.h"

/************************************************************************************************/
/* Enumerations */

/*! Enum PIDirectionType*/
typedef enum PIDirectionType
{
	INPUT = 0, OUTPUT
} PIDirectionType;

/*! Enum PDODataType*/
typedef enum PDODataType
{
	UNSIGNED8 = 0, INTEGER8, UNSIGNED16, INTEGER16,	UNSIGNED32, INTEGER32, REAL32, INTEGER64, UNSIGNED64
} PDODataType;

/*! Enum PIDataInfo*/
typedef struct PIDataInfo
{
		IEC_Datatype iecDtVar;
		INT32 dataSize;
		char* dtName;
		void Intialize()
		{
			dtName = NULL;
			dataSize = 0;
		}
} PIDataInfo;

/************************************************************************************************/
/* Structures */

/* struct ProcessImage */
struct ProcessImage
{
		char* name;
		char direction[5];
		PIDataInfo dataInfo;
		INT32 byteOffset;
		char* value;	//TODO: to be removed
		char* subindex;	//TODO: to be removed
		char* index;	//TODO: to be removed
		INT32 bitOffset;
		INT32 parameterPos;
		char* valueFrmPDO;	//TODO: to be removed
		char* moduleName;
		INT32 nodeId;
		char* varDeclName;
		char* moduleIndex;
		PIDirectionType directionType;
		void Initialize()
		{
			name = NULL;
			dataInfo.Intialize();
			byteOffset = 0;
			value = NULL;
			subindex = NULL;
			index = NULL;
			bitOffset = 0;
			valueFrmPDO = NULL;
			moduleName = NULL;
			nodeId = 0;
			varDeclName = NULL;
			moduleIndex = NULL;
			parameterPos = 0;
		}
};

/* struct NETProcessImage */
struct NETProcessImage
{
		char* name;
		char* lastName;
		char* moduleName;
		INT32 nodeId;
		INT32 totalDataSize;
		INT32 count;
		PIDataInfo dataInfo;
		PIDirectionType directionType;
		void Initialize()
		{
			count = 0;
			totalDataSize = 0;
			name = NULL;
			lastName = NULL;
			moduleName = NULL;
			dataInfo.Intialize();
		}
};

/* struct piIndexTable */
typedef struct piIndexTable
{
		char addressStr[10];
		PDODataType dtObj;
		PIDirectionType direction;
} PIIndexTable;


/* struct stOffsets */
struct Offsets
{
		INT32 prevOffset;
		INT32 currOffset;
};

/* struct PIObject */
struct PIObject
{
		char* indexId;
		char* sIdxId;
		void Initialize()
		{
			indexId = NULL;
			sIdxId = NULL;
		}

};

/* struct ModuleCol */
struct ModuleCol
{
		INT32 moduleNo;
		char moduleName[50];
};

/************************************************************************************************/
/* Externs */

extern int inVarsGlobal;
extern int outVarsGlobal;

/************************************************************************************************/
/* Constants */

static const int NO_OF_PI_ENTERIES = 18;

/****************************************************************************************************/
/* Function Declarations */

/*****************************************************************************/
/**
 \brief		This function shall be used to assign ProcessImage objects to the given piInCol and piOutCol arrays from the PICollection list based on their directionType
 
 \param		piInCol       Class variable of ProcessImage to hold the IN ProcessImage
 \param		piOutCol      Class variable of ProcessImage to hold the OUT ProcessImage
 
 \return	void
 */
/*****************************************************************************/
void GroupInOutPIVariables(ProcessImage piInCol[], ProcessImage piOutCol[]);
/*****************************************************************************/
/**
 \brief		This function shall be used to return the pointer to structure variable of PIDataInfo whose datatype matches the given parameter
  
 \param		iecDataType		Character pointer to the value of data
 \param		dataSize		Integer to hold the value of data size

 \return		PIDataInfo*
 */
/*****************************************************************************/
PIDataInfo* GetIECDT(char* iecDataType, INT32 dataSize);
/*****************************************************************************/
/**
 \brief		This function shall compute and return the INPUT offset for the process image variables

 \param		dataSize		Integer to hold the data size
 \param		pdoType		Enum of type PDOType to hold PDO type

 \return	INT32
 */
/*****************************************************************************/
INT32 ComputeINOffset(INT32 dataSize, PDOType pdoType);
/*****************************************************************************/
/**
 \brief		This function shall compute and return the OUTPUT offset for the process image variables

 \param		dataSize		Integer to hold the data size
 \param		pdoType		Enum of type PDOType to hold PDO type

 \return	INT32
 */
/*****************************************************************************/
INT32 ComputeOUTOffset(INT32 dataSize, PDOType pdoType);
/*****************************************************************************/
/**
 \brief		This function shall be used to set the start bit and byte offsets in the given ProcessImage object
 
 \param		piObj				Class pointer of ProcessImage type
 \param		startByteOffset		Integer pointer to the value of startbyteoffset
 \param		startBitOffset		Integer pointer to the value of startbitoffset

 \return	void
 */
/*****************************************************************************/
void SetPIOffsets(ProcessImage* piObj, INT32& startByteOffset, INT32& startBitOffset);
/*****************************************************************************/
/**
 \brief		This function shall be used to generate the XAP header file
 
 \param		fileName		Character pointer to the file name
 \param		piInCol			Class variable of ProcessImage to hold the IN ProcessImage
 \param		piOutCol		Class variable of ProcessImage to hold the OUT ProcessImage
 \param		inVar			Integer to hold the number of variables to be written in XAP header for the IN ProcessImage
 \param		outVar			Integer to hold the number of variables to be written in XAP header for the OUT ProcessImage
 
 \return	void	
 */
/*****************************************************************************/
void GenerateXAPHeaderFile(char* fileName, ProcessImage piInCol[], ProcessImage piOutCol[], INT32 inVar, INT32 outVar);
/*****************************************************************************/
/**
 \brief		This function shall be used to add PDO index to MN based on the given Index and SubIndex IDs
 
 \param		indexId			Character pointer to the value of Index ID 
 \param		sIdxId			Character pointer to the value of SubIndex ID
 \param		pdoTypeVal		Enum variable of PDOType
 
 \return	void	
 */
/*****************************************************************************/
void AddPDOIndexsToMN(char* indexId, char* sIdxId, PDOType pdoTypeVal);
/*****************************************************************************/
/**
 \brief		This function shall be used to return an PIObject containing the address of the ProcessImage object of given PDODataType, PIDirectionType, offsetVal and dataSizeBits in the PI collection list
 
 \param		dtType			Class variable of PDODataType to hold the datatype
 \param		dirType			Class variable of EPIDirectionType to hold the value of direction type
 \param		offsetVal		Integer to hold value of offset
 \param		dataSizeBits	Integer to hold the value of datasizeBits
 
 \return	PIObject	
 */
/*****************************************************************************/
PIObject GetPIAddress(PDODataType dtType, PIDirectionType dirType, INT32 offsetVal, INT32 dataSizeBits);
/*****************************************************************************/
/**
 \brief		This function shall be used to return the DataType of the ProcessImage Index at given IndexID in the collection list
 
 \param		indexId       Character pointer to the IndexID

 \return	char*	
 */
/*****************************************************************************/
char* GetPIDataTypeName(char* indexId);
/*****************************************************************************/
/**
 \brief		This function shall be used to return the DataType of the ProcessImage Index at given IndexID in the collection list
 
 \param		indexId		Character pointer to the ProcessImage address

 \return	char*	
 */
/*****************************************************************************/
char* GetPIName(char* indexId);
/*****************************************************************************/
/**
 \brief		This function shall be used to check if the given IndexID belongs to the range allocated for ProcessImage
 
 \param		idxId		Character Pointer Variable to hold the value of index
 \return	BOOL

 \retval	TRUE		if given IndexID belongs to the range allocated for ProcessImage
 \retval	FALSE		if given IndexID does not belong to the range allocated for ProcessImage
 */
/*****************************************************************************/
bool CheckIfProcessImageIdx(char* idxId);
/*****************************************************************************/
/**
 \brief		This function shall be used to write the contents of given ProcessImage object to XAP header file
 
 \param		piObj				Class variable of ProcessImage to hold the process image value
 \param		noOfVars			Integer to hold the value of total number of variables to be written to XAP header file
 \param		directionType		Enum variable of  PIDirectionType
 \param		xapHeader			File pointer to the header file path

 \return	void	
 */
/*****************************************************************************/
void WriteXAPHeaderContents(ProcessImage piObj[], INT32 noOfVars, PIDirectionType directionType, FILE* xapHeader);
/*****************************************************************************/
/**
 \brief		This function shall be used to write the contents of given ProcessImage object to .NET header file
 
 \param		fileName         Character pointer to the file name path
 \param		piInCol          Class variable of IN ProcessImage
 \param		piOutCol         Class variable of OUT ProcessImage
 \param		iInVar           Integer to hold input value   
 \param		iOutVar          Integer to hold output value 
 
 \return	void	
 */
/*****************************************************************************/
void GenerateNETHeaderFile(char* fileName, ProcessImage piInCol[], ProcessImage piOutCol[], INT32 iInVar, INT32 iOutVar);
/*****************************************************************************/
/**
 \brief		This function shall be used to write the contents of given ProcessImage object to .NET header file
 
 \param		piObj			Class variable of ProcessImage
 \param		noOfVars		Integer to hold the value of number of variables
 \param		dirType			Enum variable of PIDirectionType
 \param		netHeader		File pointer to the header file

 \return	void	
 */
/*****************************************************************************/
void WriteNETHeaderContents(ProcessImage piObj[], INT32 noOfVars, PIDirectionType dirType, FILE* netHeader);
/*****************************************************************************/
/**
 \brief		This function shall be used to return the item number at which the NET ProcessImage object has the same module number as the given parameter in the Node collection list
 
 \param		nodeId         	Integer to hold the node id
 \param		itemNo   		Integer to hold the value of loop count
 \param		moduleName    	Character pointer to the name of the module

 \return	INT32	
 */
/*****************************************************************************/
INT32 SearchModuleNameNETProcessImageCollection(INT32 nodeId, INT32 itemNo, char* moduleName);
/*****************************************************************************/
/**
 \brief		This function shall be used to assign NETProcessImage objects to the given netPIObj array from the NETPICollection list based directionType matching the given parameter
 
 \param		directionType   	Class variable of PIDirectionType to hold the direction type
 \param		netPIObj       		Class variable of NETProcessImage to hold the objects
 
 \return	void
 */
/*****************************************************************************/
INT32 GroupNETPIVariables(PIDirectionType directionType, NETProcessImage netPIObj[]);
/*****************************************************************************/
/**
 \brief		This function shall be used to assign unique variable name to ProcessImage objects in the collection list
 
 \return	void
 */
/*****************************************************************************/
void SetUniquePIVarName();
/*****************************************************************************/
/**
 \brief		This function shall be used to copy the ProcessImage object to NETProcessImage object and assign the given module name to it
 
 \param		piObj			Class variable of ProcessImage
 \param		netPIObj        Class variable of NETProcessImage
 \param		moduleName      Character pointer to the module name
 
 \return	void	
 */
/*****************************************************************************/
void CopyPItoNETPICollection(ProcessImage piObj, NETProcessImage netPIObj, char* moduleName);
/*****************************************************************************/
/**
 \brief		This function shall be used to group the contents of ProcessImage object for .NET header file generation
  
 \param		piObject		Class variable of ProcessImage
 \param		noOfVars        Integer to hold the value of number of variables to be written to header
 \param		dirType         Enum variable of PIDirectionType
 \param		netHeader       File pointer to the header
 
 \return	INT32	
 */
/*****************************************************************************/
INT32 GroupNETHeaderContents(ProcessImage piObject[], INT32 noOfVars, PIDirectionType dirType, FILE* netHeader);
/*****************************************************************************/
/**
 \brief		This function shall be used to return the Datatype string corresponding to the given IEC_Dataype
 
 \param		dtIEC       Enum variable of IEC_Datatype to hold the datatype of process image
 
 \return	char*	
 */
/*****************************************************************************/
char* GetDatatypeNETPI(IEC_Datatype dtIEC);
/*****************************************************************************/
/**
 \brief		This function shall be used to return the size of the Datatype corresponding to the given IEC_Dataype
 
 \param		dtIEC		Enum variable of IEC_Datatype to hold the datatype of process image
 
 \return	INT32	
 */
/*****************************************************************************/
INT32 GetDatasizeNETPI(IEC_Datatype dtIEC);
/*****************************************************************************/
/**
 \brief		This function shall be used to check if the given module name is present in the collection list. If available it updates the module number in the calling function
 
 \param		moduleName       Character pointer to the value of Module Name
 \param		moduleNo         Integer to hold the value of Module number  
 \param		noOfModules      Integer to hold the value of Number of modules
 \param		modCollObj       Structure Class variable of ModuleCol
 
 \return	BOOL
 \retval	TRUE			if the module is present in the collection
 \retval	FALSE			if the module is not present in the collection	
 */
/*****************************************************************************/
bool CheckIfModuleExists(char* moduleName, INT32 &moduleNo, INT32 noOfModules, ModuleCol modCollObj[]);
/*****************************************************************************/
/**
 \brief		This function shall be used to set the DataType of a SubIndex belonging to the Index according to the given parameters
 
 \param		objDataType	     Class pointer of DataType
 \param		idxId            Character pointer to the Index ID
 \param		sIdxId           Character pointer to the SubIndex ID
 
 \return	void	
 */
/*****************************************************************************/
void SetSIdxDataType(DataType *objDataType, char* idxId, char* sIdxId);
#endif // processImage_h
