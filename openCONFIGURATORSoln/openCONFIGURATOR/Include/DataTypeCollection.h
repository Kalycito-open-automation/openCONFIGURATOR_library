/**
 *****************************************************************************
 \file		DataTypeCollection.h

 \brief		This file contains the class declaration that is used for handling complex datatype collection in XML configuration file(XDD/XDC)
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
#ifndef DataTypeCollection_h
#define DataTypeCollection_h

/****************************************************************************/
/* Includes */

#include "Declarations.h"
#include "TCollection.h"

/**
 ****************************************************************************
 \class			DataTypeCollection
 \brief			This class is to handle complex datatype collection in xdd/xdc file
 
 ****************************************************************************/
class DataTypeCollection
{
	public:
		DataTypeCollection(void);
		~DataTypeCollection(void);

		/*****************************************************************************/
		/**
		 \brief			This function shall be used to add an object of DataType to a collection list and update the count
		 
		 \param			objDataType		Class variable of DataType to include in data type collection	
		 
		 \return		void
		 */
		/*****************************************************************************/
		void AddDataType(DataType objDataType);
		/*****************************************************************************/
		/**
		 \brief			This function shall be used to delete the objects of DataType collection list and update the count
		 
		 \return		void
		 */
		/*****************************************************************************/
		void DeleteDataTypeCollection();
		/*****************************************************************************/
		/**
		 \brief			This function shall be used to return number of DataType objects in the DataType collection list
		 
		 \return		INT32
		 */
		/*****************************************************************************/
		INT32 GetNumberOfDataTypes();
		/*****************************************************************************/
		/**
		 \brief			This function shall be used to return the DataType object at the given position in the collection list
		 
		 \param			dataTypePosition	Integer to hold the datatype id
		 
		 \return		DataType
		 */
		/*****************************************************************************/
		DataType* GetDataTypeElement(INT32 dataTypePosition);
		/*****************************************************************************/
		/**
		 \brief			This function shall be used to return the DataType object(whose dataTypeValue matches the given parameter) from the collection list
		 
		 \param			dataTypeValue		Character pointer to the datatype value

		 \return		DataType*
		 */
		/*****************************************************************************/
		DataType* GetDataType(char* dataTypeValue);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the DataType object whose dataTypeValue matches the given parameter
		 
		 \param		dataTypeValue		Character pointer to the datatype value
		 
		 \return	DataType*
		 */
		/*****************************************************************************/
		DataType* GetDataTypeByName(char* dataTypeValue);

	private:
		INT32 dataTypeCollectionCount;
		TCollection<DataType> dataTypeCollectionObj;
};
#endif // DataTypeCollection_h
