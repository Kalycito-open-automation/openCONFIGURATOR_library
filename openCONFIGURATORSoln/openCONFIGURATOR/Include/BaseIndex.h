/**
 ************************************************************************************************
 \file		BaseIndex.h

 \brief		This file contains the class declaration that is used for handling the attributes of an Index/SubIndex object 
 ************************************************************************************************
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

 ***********************************************************************************************/
#ifndef BaseIndex_h
#define BaseIndex_h

/***************************************************************************************************/
/*  Includes  */

#include "TCollection.h"
#include "Declarations.h"

/**
 *****************************************************************************************************
 \class			BaseIndex
 \brief			This is the BaseClass for CIndex/CSubIndex

 ****************************************************************************************************/

class BaseIndex
{
	public:
		BaseIndex(void);
		~BaseIndex(void);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the name of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetName();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the name of the Index

		 \param		nameStr		Character pointer to the name of the Index

		 \return	void
		 */
		/*****************************************************************************/
		void SetName(char* nameStr);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Index ID
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetIndexValue();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the Index ID
		 
		 \param		idxId	 Character pointer to the Index value
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetIndexValue(char* idxId);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Default Value of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetDefaultValue();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the Default Value of the Index
		 
		 \param		value		Character pointer to the Default value
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetDefaultValue(char* value);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Actual Value of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetActualValue();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the Actual Value of the Index
		 
		 \param		value		Character pointer to the Actual value

		 \return	void
		 */
		/*****************************************************************************/
		void SetActualValue(char* value);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Access Type of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetAccessType();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the Access Type of the Index
		 
		 \param		accessStr 		Character pointer to the Access type of the Index
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetAccessType(char* accessStr);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the LowLimit value of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetLowLimit();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the LowLimit of the Index
		 
		 \param		lowLimitStr		Character pointer to LowLimit value
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetLowLimit(char* lowLimitStr);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the HighLimit value of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetHighLimit();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the HighLimit of the Index
		 
		 \param		highLimitStr		Character pointer to the HighLimit value
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetHighLimit(char* highLimitStr);

		/******************************************************************************/
		/**
		 \brief		This function shall be used to return the DataType of the Index
		 
		 \return	DataType
		 */
		/*****************************************************************************/
		DataType GetDataType();
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the datatype of the Index
		 
		 \param		dtObj		Class Variable of DataType to hold value of object datatype
		 \return	void
		 */
		/*****************************************************************************/
		void SetDataTypeST(DataType dtObj);
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the DataType of the Index
		 
		 \param		dataTypeName	Character pointer to the datatype name

		 \return	void
		 */
		/*****************************************************************************/
		void SetDataType(char* dataTypeName);
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the DataType name of the Index for a given Node ID
		 
		 \param		dataTypeName 	Character pointer to the datatype name
		 \param		nodeIdVal	 	Integer to hold Node ID of each node

		 \return	void
		 */
		/*****************************************************************************/
		void SetDataTypeName(char* dataTypeName, INT32 nodeIdVal);

		/******************************************************************************/
		/**
		 \brief		This function shall be used to return the PDOMapping of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetPDOMapping();
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the PDOMapping of the Index
		 
		 \param		pdoMappingStr		Character pointer to the PDOMapping of the Index
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetPDOMapping(char* pdoMappingStr);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Object Type of the Index
		 
		 \return	const char*
		 */
		/*****************************************************************************/
		const char* GetObjectType();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Object Type of the Index
		 
		 \return	ObjectType
		 */
		/*****************************************************************************/
		ObjectType GetEObjectType();
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the Object Type of the Index
		 
		 \param		objTypeStr		Character pointer to the object type
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetObjectType(char* objTypeStr);

		/******************************************************************************/
		/**
		 \brief		This function shall be used to return the NodeID of the Index
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetNodeID();
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the Node ID of the Index
		 
		 \param		nodeIdVal		Integer to hold the Node ID of the objects

		 \return	void
		 */
		/*****************************************************************************/
		void SetNodeID(INT32 nodeIdVal);

		/******************************************************************************/
		/**
		 \brief		This function shall be used to check the validity of index value
		 
		 \param		hexValue	Character pointer to the hex value
		 
		 \return	BOOL
		 
		 \retval	TRUE		if index value is valid
		 \retval	FALSE		if index value is invalid
		 */
		/*****************************************************************************/
		bool IsIndexValueValid(char* hexValue);
		/******************************************************************************/
		/**
		 \brief		This function shall be used to save the changes of the Index properties
		 
		 \param		idxIdStr	Character pointer to the Index value
		 \param		nameStr	 	Character pointer to the Index name

		 \return	void
		 */
		/*****************************************************************************/
		void SaveChanges(char* idxIdStr, char* nameStr);

		/******************************************************************************/
		/**
		 \brief		This function shall be used to return the Unique ID Reference of the Index 
		 
		 \return	char*
		 */
		/*****************************************************************************/
		char* GetUniqueIDRef();
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the Unique ID Reference of the Index
		 
		 \param     uniqueIdStr  	Character pointer to the Unique ID of the objects
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetUniqueIDRef(char* uniqueIdStr);

		/******************************************************************************/
		/**
		 \brief		This function shall be used to return datatype of the Index
		 
		 \return	char*
		 */
		/*****************************************************************************/
		char* GetDataTypeValue();
		/******************************************************************************/
		/**
		 \brief		This function shall be used to set the datatype value of the Index
		 
		 \param		dataTypeStr
		 \return	void
		 */
		/*****************************************************************************/
		void SetDataTypeValue(char* dataTypeStr);

		/******************************************************************************/
		/**
		 \brief		This function shall be used to check if the Index's inclusion in CDC generation is enabled/disabled
		 
		 \return	Flag			
		 */
		/*****************************************************************************/
		Flag GetFlagIfIncludedCdc();
		/******************************************************************************/
		/**
		 \brief		This function shall be used to enable/disable the Index's inclusion in CDC generation
		 
		 \param		flagVal  	Enum of type Flag to hold value of flagIfInCdc
		 
		 \return	void			
		 */
		/*****************************************************************************/
		void SetFlagIfIncludedCdc(Flag flagVal);

	protected:
		const char* name;
		const char* indexId;
		const char* lowLimit;
		const char* highLimit;
		const char* accessType;
		const char* defaultValue;
		char* uniqueIdRef;
		char* actualValue;
		char* dataTypeValue; /* Only used for ObjectDictionary*/
		ObjectType objectType;
		DataType dataType;
		PDOMapping pdoMapping;
		INT32 nodeId;
		INT32 parameterIndex;
		Flag includeInCDC;
};
#endif //BaseIndex_h
