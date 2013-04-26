/**
 *****************************************************************************
 \file		IndexCollection.h

 \brief		This file contains the class declaration to handle the properties of objects of the Index Collection
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
#ifndef IndexCollection_h
#define IndexCollection_h

/****************************************************************************************************/
/* Includes */

#include "Index.h"

using namespace std;

/**
 ******************************************************************************************************
 \class			IndexCollection
 \brief			This class has definition of Index collection for all datatypes		
 
 ******************************************************************************************************/
class IndexCollection
{
	public:
		IndexCollection(void);
		~IndexCollection(void);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the 0x14XX, 0x16XX, 0x18XX, 0x1AXX Index objects in the collection list and update the count
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeletePDOs();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the PIO Index objects in the collection list and update the count
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeletePIObjects();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete all the Index objects in the collection list and update the count

		 \return	void
		 */
		/*****************************************************************************/
		void DeleteIndexCollection();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add an Index object to a collection list and update the count
		 
		 \param		objIndex	Class variable of Index for object index
		 
		 \return	void
		 */
		/*****************************************************************************/
		void AddIndex(Index objIndex);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the Index object at given position in the collection list and update the count
		 
		 \param		indexPosition	Integer to hold index id
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteIndex(INT32 indexPosition);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the number of indices in the collection list
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetNumberofIndexes();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Index object at the given position in the collection list
		 
		 \param		indexPosition    Integer to hold index count
		 
		 \return	Index*
		 */
		/*****************************************************************************/
		Index* GetIndex(INT32 indexPosition);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Index object whose indexId matches the given parameter in the collection list
		 
		 \param		indexId    Character pointer to the indexId value
		 
		 \return	Index*
		 */
		/*****************************************************************************/
		Index* GetIndexbyIndexValue(char* indexId);

	private:
		INT32 indexCount;
		TCollection<Index> indexCollection;
};
#endif // IndexCollection_h
