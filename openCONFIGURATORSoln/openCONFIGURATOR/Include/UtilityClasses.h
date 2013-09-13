/**
 *****************************************************************************
 \file		UtilityClasses.h

 \brief		This file contains the utility class declarations
 *****************************************************************************
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
#ifndef utilityclasses_h
#define utilityclasses_h

#include <string.h>
#include <stdio.h>
#include "Enums.h"

/** 
* \class AppProcessCommon
* \brief The class represents the common members under the Application process tag
*/
class AppProcessCommon
{
private:
	char* name;					/**< Name of the interface variable or structure component */
	char* uniqueId;				/**< Unique Id of the interface variable or structure component */
	char* dataType;				/**< Nested datatype of the interface variable or structure component */
	char* dataTypeUniqueIDRef;	/**< Datatype Unique Id reference of the interface variable or structure component */

public:
	AppProcessCommon(void)
	{
		this->Initialize();
	}


	~AppProcessCommon(void)
	{

	}

	/**
	\brief		This function is used to get the uniqueId of the interface variable or structure component
	\return		char*	uniqueId of the interface variable or structure component
	*/
	char* GetUniqueID()
	{
		return (uniqueId);
	}

	/**
	\brief		This function is used to set the uniqueId of the interface variable or structure component
	\param[in]	varUniqueID		Character pointer to the uniqueId to be set
	\return		void
	*/
	void SetUniqueID(char* varUniqueID)
	{
		uniqueId = new char[strlen(varUniqueID) + 1];
		strcpy(uniqueId, varUniqueID);
	}

	/**
	\brief		This function is used to get the name of the interface variable or structure component	
	\return		char*	name of the interface variable or structure component
	*/
	char* GetName()
	{
		return (name);
	}

	/**
	\brief		This function is used to set the name of the interface variable or structure component
	\param[in]	attrName	Character pointer to the name to be set
	\return		void
	*/
	void SetName(char* attrName)
	{

		name = new char[strlen(attrName) + 1];
		strcpy(name, attrName);

	}

	/**
	\brief		This function is used to get the dataType of the interface variable or structure component
	\return		char*	dataType of the interface variable or structure component
	*/
	char* GetDataType()
	{
		return (dataType);
	}

	/**
	\brief		This function is used to set the dataType of the interface variable or structure component
	\param[in]	dtStr	Character pointer to the dataType to be set
	\return		void
	*/
	void SetDataType(char* dtStr)
	{

		dataType = new char[strlen(dtStr) + 1];
		strcpy(dataType, dtStr);
	}

	/**
	\brief		This function is used to get the uniqueIdReference of the interface variable or structure component
	\return		char*	dataTypeUniqueIdReference of the interface variable or structure component
	*/
	char* GetDtUniqueRefId()
	{
		return (dataTypeUniqueIDRef);
	}

	/**
	\brief		This function is used to set the uniqueIdReference of the interface variable or structure component
	\param[in]	uniqueRefID		Character pointer to the uniqueId Reference to be set
	\return		void	
	*/
	void SetDtUniqueRefId(char* uniqueRefID)
	{

		dataTypeUniqueIDRef = new char[strlen(uniqueRefID) + 1];
		strcpy(dataTypeUniqueIDRef, uniqueRefID);
	}

	/**
	\brief		This function is used to initialise the members to a default value.
	\return		void
	*/
	void Initialize()
	{
		name = NULL;
		uniqueId = NULL;
		dataType = NULL;
		dataTypeUniqueIDRef = NULL;
	}
};

/** 
* \class VarDeclaration
* \brief The class represents the interface variable or structure component and their attributes
*/
class VarDeclaration
{
public:
	AppProcessCommon* namIdDtAttr;		/**< To hold the common attributes of the interface variable or structure component */
	char size[5];						/**< To hold the size of the interface variable or structure component */
	char* initialValue; 				/**< To hold the initial value of the interface variable or structure component */
	char* structUniqueId;				/**< To hold the unique id of the struct component */

	/**
	\brief		This function is used to initialise the members to a default value.
	\return		void
	*/

	VarDeclaration(void)
	{
		this->Initialize();
	}


	~VarDeclaration(void)
	{

	}

	void Initialize(void)
	{
		namIdDtAttr = new AppProcessCommon();
		namIdDtAttr->Initialize();
		strcpy(size, "");
		initialValue = NULL;
		structUniqueId = NULL;
	}
};

/** 
* \class DataType
* \brief The class represents the DataTypeList under the Application layer tag
*/
class DataType
{
public:
	char* dataTypeName;			/**< DataType of the interface variable or structure component */
	char* dataTypeValue;		/**< Value for the dataType */
	INT32 dataSize;				/**< DataSize for the dataType */
	IEC_Datatype iecDataType;	/**< To represent the equivalent IEC datatype */

	DataType(void)
	{
		dataTypeName = NULL;
		dataTypeValue = NULL;
		dataSize = 0;
	}


	~DataType(void)
	{

	}

	/**
	\brief		This function is used to get the dataType
	\return		char*
	*/
	char* GetName()
	{
		return (dataTypeName);
	}
	
	/**
	\brief		This function is used to set the dataType	
	\param[in]	tempDataTypeName	Character pointer to the dataType to be set
	\return		void
	*/
	//TODO: Review. setName not called
	void SetName(char* tempDataTypeName)
	{

		dataTypeName = new char[strlen(tempDataTypeName) + 1];
		strcpy(dataTypeName, tempDataTypeName);
	}
	
	/**
	\brief		This function is used to initialise the members to a default value.
	\return		void
	*/
	void Initialize()
	{
		dataTypeName = NULL;
		dataTypeValue = NULL;
		dataSize = 0;
	}

};

/** 
* \class Feature
* \brief The class represents members of the Network management features
*/
class Feature
{
public:
	char* name;					/**< Name of the feature */
	char* value;				/**< Value of the feature */
	FeatureType featureType;	/**< Type of the feature */

	/**
	\brief		This function is used to initialise the members to a default value.
	\return		void
	*/
	Feature(void)
	{
		this->Initialize();
	}


	~Feature(void)
	{
	}


	void Initialize()
	{
		name = NULL;
		value = NULL;
	}
};

/** 
* \class MNPdoVariable
* \brief The class represents the process data objects for the MN, which is used in generating the MN PDO index & subindex.
*/
class MNPdoVariable
{
public:

	char* indexId;			/**< Holds the IndexId of the Index to be created */
	char* subIndexId;		/**< Holds the IndexId of the SubIndex to be created */
	INT32 offsetVal; 		/**< Holds the Offset value of the PDO */
	char* value;			/**< Holds the mapping configuration */	
	INT32 dataSize;			/**< To hold the size of the PDO data */
	PDOType pdoType;		/**< To hold the type of the PDO */

	/**
	\brief		This function is used to initialise the members to a default value.
	\return		void
	*/
	MNPdoVariable(void)
	{
		indexId = NULL;
		subIndexId = NULL;
		offsetVal = 0;
		value = NULL;
		dataSize = 0;
	}


	~MNPdoVariable(void)
	{
	}

	void Initialize()
	{
		indexId = NULL;
		subIndexId = NULL;
		offsetVal = 0;
		value = NULL;
		dataSize = 0;
	}
};

/** 
* \class Parameter
* \brief The class represents the parameter list under the Application process tag
*/
class Parameter
{
public:
	AppProcessCommon* nameIdDtAttr;		/**< To hold the common attributes for a parameter */
	INT32 size;							/**< To hold the nested size of the parameter */
	char* accessStr;					/**< To hold the access for the parameter: */
	//char* dataType;
	//INT32 structIndex;
	//INT32 paraIndex;

	/**
	\brief		This function is used to initialise the members to a default value.
	\return		void
	*/
	Parameter(void)
	{
		nameIdDtAttr = NULL;
		accessStr = NULL;
	}


	~Parameter(void)
	{

	}

	void Initialize()
	{
		nameIdDtAttr = new AppProcessCommon();
		nameIdDtAttr->Initialize();
		accessStr = NULL;
		size = 0;
	}

};

#endif