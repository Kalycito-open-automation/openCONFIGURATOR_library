/**
 *****************************************************************************************************
 \file			Index.h

 \brief			Handles definitions of Index collection,PDO type for all datatypes	
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
#ifndef Index_h
#define Index_h

/****************************************************************************************************/
/* Includes */

#include "TCollection.h"
#include "Declarations.h"
#include "BaseIndex.h"
#include "SubIndex.h"

/**
 ******************************************************************************************************
 \class			Index
 \brief			This function has the definitions of Index collection,PDO type for all datatypes			  
 
 ******************************************************************************************************/

class Index: public BaseIndex
{
	public:
		Index(void);
		~Index(void);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add the given SubIndex to the collection list
		 
		 \param		objSubIndex		Class variable of CSubIndex for object subindex		
		 
		 \return	void
		 */
		/*****************************************************************************/
		void AddSubIndex(SubIndex objSubIndex);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the SubIndex at the given position in the collection list
		 
		 \param		subIndexPosition		Integer to hold the value of subindex id
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteSubIndex(INT32 subIndexPosition);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the objects in the SubIndex collection list
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteSubIndexCollection();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to assign PDO type to the private member pdoType
		 
		 \param		varPDOType		Enum Variable of PDOType to hold the value of PDO type
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetPDOType(PDOType varPDOType);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to swap the position of two SubIndex objects in the collection list 
		  
		 \param		fromPosition	Integer to hold the from SubIndex object position
		 \param		toPosition		Integer to hold the to SubIndex object position

		 \return	void
		 */
		/*****************************************************************************/
		void SwapSubObjects(INT32 fromPosition, INT32 toPosition);
		/*****************************************************************************/
		/**
		 \brief			This function shall be used to update the DataType structure variable for SubIndex objects in the collection list
		 
		 \return		void
		 */
		/*****************************************************************************/
		void UpdateArraySubObjects();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the number of objects in the SubIndex collection list
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetNumberofSubIndexes();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the private member pdoType
		 
		 \return	PDOType
		 */
		/*****************************************************************************/		
		PDOType GetPDOType();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the SubIndex object at the given position in the collection list
		 
		 \param		subIndexPosition 	Integer  to hold the value of subindex id
		 
		 \return	SubIndex*
		 */
		/*****************************************************************************/
		SubIndex* GetSubIndex(INT32 subIndexPosition);
		/*****************************************************************************/
		/**
		 \brief			This function shall be used to return the SubIndex object(whose subIndexId value matches the given parameter) from the collection list
		 
		 \param			subIndexId		Character pointer to the subIndexId value
		 
		 \return		SubIndex*
		 */
		/*****************************************************************************/
		SubIndex* GetSubIndexbyIndexValue(char* subIndexId);

	private:
		INT32 sidxCount;
		PDOType pdoType;
		TCollection<SubIndex> subIndexCollection;
};
#endif // Index_h
