/**
 ************************************************************************************************
 \file			BaseIndex.h

 \brief			BaseClass for CIndex/CSubIndex
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
 \class			CBaseIndex
 \brief			This is the BaseClass for CIndex/CSubIndex

 ****************************************************************************************************/

class DllExport CBaseIndex
{
	public:
		CBaseIndex(void);
		~CBaseIndex(void);

	protected:
		const char* m_Name;
		const char* m_Index;
		const char* m_LowLimit;
		const char* m_HighLimit;
		EObjectType m_objectType;
		DataType m_dataType;
		const char* m_accessType;
		const char* m_DefaultValue;
		char* m_ActualValue;
		EPDOMapping m_pdoMapping;
		INT32 m_NodeID;
		char* m_UniqueIDRef;
		INT32 m_ParameterIndex;
		char* m_dataTypeValue;/* Only used for ObjectDictionary*/
		EFlag m_IsIncludedInCDC;

	public:

		const char* getName();
		void setName(char* Name);

		const char* getIndexValue();
		void setIndexValue(char* Index);

		const char* getDefaultValue();
		void setDefaultValue(char* Value);

		const char* getActualValue();
		void setActualValue(char* Value);

		const char* getAccessType();
		void setAccessType(char* Access);

		const char* getLowLimit();
		void setLowLimit(char* LowLimit);

		const char* getHighLimit();
		void setHighLimit(char* HighLimit);

		DataType getDataType();
		void setDataType(char* dataTypeValue);
		void setDataType(char* dataTypeName, INT32 NodeID);

		const char* getPDOMapping();
		void setPDOMapping(char* pdoMappingValue);

		const char* getObjectType();
		EObjectType getEObjectType();
		void setObjectType(char* objectType);

		INT32 getNodeID();
		void setNodeID(INT32 NodeID);

		bool IsIndexVaueValid(char* value);
		void SaveChanges(char* Name, char* Value);

		char* getUniqueIDRef();
		void setUniqueIDRef(char* UniqueID);

		char* getDataTypeValue();
		void setDataTypeValue(char* value);

		EFlag getFlagIfIncludedCdc();
		void setFlagIfIncludedCdc(EFlag flag);

		void setDataTypeST(DataType dt);
};
#endif //BaseIndex_h
