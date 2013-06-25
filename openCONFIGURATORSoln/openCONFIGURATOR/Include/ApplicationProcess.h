/**
 ******************************************************************************
 \file		ApplicationProcess.h

 \brief		This file contains the class declaration that is used for handling the objects within the 'Application Process' tag in the XML configuration file(XDD/XDC) 
 ******************************************************************************
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

 ***********************************************************************************************/

#ifndef ApplicationProcess_h
#define ApplicationProcess_h

/**********************************************************************************************/
/* Includes */

#include <string>
#include "ComplexDataType.h"
#include "TCollection.h"

using namespace std;

/**
 *******************************************************************************************************
 \class		ApplicationProcess
 \brief		The collection of all the objects within the Application Process tag xdd/xdc file

 ********************************************************************************************************/
class ApplicationProcess
{
	public:
		ApplicationProcess(void);
		~ApplicationProcess(void);

		char *xddFileName;			/**< Usage to be documented */
		char *projectPath;			/**< Usage to be documented */
		TCollection<ComplexDataType> CDTCollection;		/**< Usage to be documented */
		TCollection<Parameter> ParameterCollection;		/**< Usage to be documented */

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the objects in ComplexDataType collection list
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteComplexDataTypeCollection(void);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the objects in Parameter collection list
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteParameterCollection(void);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add an object of ComplexDataType to a collection list
		 
		 \param		objectCdT	 Class variable of ComplexDataType for data type

		 \return	void
		 */
		/*****************************************************************************/
		void AddComplexDataType(ComplexDataType objectCdT);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add an object of Parameter to a collection list
		 
		 \param		objectParameter		Structure Variable of addParameter to add parameter to the collection list

		 \return	void
		 */
		/*****************************************************************************/
		void AddParameter(Parameter objectParameter);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to update the prevUniqueId member of the ComplexDataType object of given Index position with the given uniqueID

		 \param		uniqueID 		Character pointer to the unique id of CDT collection
		 \param		cDtPosition		Integer to hold the Index of CDT collection
		 \return	void
		 */
		/*****************************************************************************/
		void UpdatePreviousCDTUId(char* uniqueID, INT32 cDtPosition);
		/*****************************************************************************/
		/**
		 \brief		This function shall return the ComplexDataType collection count

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetCDTCount();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to check if the file is open and return this status 
		 
		 \param		fileName	Character pointer to the path of the file
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 CheckFileStatus(char* fileName);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the parameter index with unique reference ID

		 \param		uniqueIdRef		Character pointer to the value of unique reference ID

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetParameterIndexbyUniqueIDRef(char *uniqueIdRef);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the position of ComplexDataType object with given unique reference ID in the collection list

		 \param		uniqueIdRef   Character pointer to the Unique reference ID

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetCDTUniqueIDRef(char *uniqueIdRef);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the pointer to the ComplexDataType object with given Index position in the collection list

		 \param		cDtPosition		Integer to hold the Index of CDT collection

		 \return	ComplexDataType*
		 */
		/*****************************************************************************/
		ComplexDataType* GetCDTbyCount(INT32 cDtPosition);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the pointer to the ComplexDataType object with given unique reference ID in the collection list

		 \param		uniqueId	Character pointer to the unique reference ID

		 \return	ComplexDataType*
		 */
		/*****************************************************************************/
		ComplexDataType* GetCDTbyUniqueID(char *uniqueId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the pointer to the ComplexDataType object with given dataTypeStr string in the collection list

		 \param		dataTypeStr		Character pointer to the dataTypeStr string		

		 \return	ComplexDataType*
		 */
		/*****************************************************************************/
		ComplexDataType* GetCDTByDtUniqueRefID(char *dataTypeStr);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the pointer to the ComplexDataType object with given Index position in the collection list
		 
		 \param		cDtPosition		Integer to hold the Index of CDT collection

		 \return	ComplexDataType*
		 */
		/*****************************************************************************/
		ComplexDataType* GetCDTByDtIndex(INT32 cDtPosition);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the parameter with unique reference ID

		 \param		uniqueIdRef		Character pointer to the unique reference ID

		 \return	Parameter*
		 */
		/*****************************************************************************/
		Parameter* GetParameterbyUniqueIDRef(char *uniqueIdRef);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to get the Parameter object given its Index position in the collection list
		 
		 \param		parameterPosition		Integer to hold the Parameter Index position

		 \return	Parameter
		 */
		/*****************************************************************************/
		Parameter GetUniqueIDRefbyParameterIndex(INT32 parameterPosition);

};

#endif // ApplicationProcess_h
