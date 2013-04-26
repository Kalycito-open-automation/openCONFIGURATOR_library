/**
 *****************************************************************************
 \file		ObjectDictionary.h

 \brief		This file contains the class declaration to handle operations on the object dictionary template od.xml which is used as a template for creating objects for all nodes
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
#ifndef ObjectDictionary_h
#define ObjectDictionary_h

/****************************************************************************/
/* Includes */

#include <libxml/xmlreader.h>
#include "Index.h"
#include "DataTypeCollection.h"
#include "IndexCollection.h"
#include "Declarations.h"
#include "Node.h"
#include "TCollection.h"
#include "Internal.h"

/**
 *****************************************************************************
 \class			ObjectDictionary
 \brief			Handles the object dictionary for each index	
 
 *****************************************************************************/
class ObjectDictionary
{
	public:
		ObjectDictionary(void);
		~ObjectDictionary(void);

	private:
		static bool instanceFlag;
		INT32 varsattrIdxSIdx;

		enum ObjectType
		{
			INDEX = 0, SUBINDEX
		};

		typedef struct sattrIdxSIdx
		{
				char* Idx;
				char* startIndex;
				char* endIndex;
				ObjectType objectType;
		} sattrIdxSIdx;

		TCollection<sattrIdxSIdx> attribCollObj;
	public:

		static Node* objDictNode;
		static ObjectDictionary* objectDictionary;

		void CheckIfSameIndex(char* Index);
		Index* GetIndexDictAttribues(char* Index);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to process the Object Dictionary file and generate the DataTypeCollection, IndexCollection and SubIndexCollection lists
		 
		 \param		reader		xml Variable of type xmlTextReaderPtr 
		 
		 \return	void
		 */
		/*****************************************************************************/

		void ProcessObjectDictionary(xmlTextReaderPtr reader);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return pointer to ObjectDictionary object
		 
		 \return	ObjectDictionary*
		 */
		/*****************************************************************************/
		static ObjectDictionary* GetObjDictPtr();
		/*****************************************************************************/
		/**
		 \brief		This function shall adds the an attribute(subindex) to the attribute collection(Index)
		 
		 \param		object	Structure Variable of sattrIdxSIdx to hold the attribute Object

		 \return	void
		 */
		/*****************************************************************************/
		void AddSameAttributesObjects(sattrIdxSIdx object);
		/*****************************************************************************/
		/**
		 \brief		This function shall create an objects and attributes of the type and indexId
		 
		 \param		value		Character pointer to the value of the object
		 \param		objType		Enum variable of ObjectType to hold the value of Object type
		 \param		idxId		Character pointer to the value of Index
		 
		 \return	void
		 */
		/*****************************************************************************/
		void CreateSameattrObject(char* value, ObjectType objType, char* idxId);
		/*****************************************************************************/
		/**
		 \brief		This is a member function of CObjectDictionary returns index value of the object dictionary
		 
		 \param		indexId            Character Pointer Variable to hold the value of Index
		 
		 \return	Index*
		 */
		/*****************************************************************************/
		Index* GetObjectDictIndex(char* indexId);
		/*****************************************************************************/
		/**
		 \brief		This is a member function of CObjectDictionary returns the object dictonary for sub index
		 
		 \param		idxId       Character pointer variable to hold the value of Index
		 \param		sIdxId      Character pointer variable to hold the value of Sub-Index
		 
		 \return	SubIndex*
		 */
		/*****************************************************************************/
		SubIndex* GetObjectDictSubIndex(char* idxId, char* sIdxId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to check if an Object Dictionary Index exists at the given Index ID in the collection list
		 
		 \param		idxId		Character pointer to the value of index

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 IfObjectDictIndexExists(char* idxId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to check if an Object Dictionary SubIndex exists at the given SubIndex ID in the collection list
		 
		 \param		idxId      Character pointer to the value of Index
		 \param		sIdxId	   Character pointer to the value of SubIndex
		 
		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 IfObjectDictSubIndexExists(char* idxId, char* sIdxId);
		/*****************************************************************************/
		/**
		 \brief		This is a member function of CObjectDictionary returns the range of the object dictionary ids
		 
		 \param		idxId		Character Pointer Variable to hold the value of Index
		 \param		startIdx	Character Pointer Variable to hold the value of Start Index 
		 \param		endIdx      Character Pointer Variable to hold the value of End Index
		 
		 \return	BOOL
		 \retval	TRUE		to check
		 \retval	FALSE		to check	
		 */
		/*****************************************************************************/
		bool CheckInTheRange(char* idxId, char* startIdx, char* endIdx);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the name of the Index given the Object Name and Index ID
		 
		 \param		idxId			Character pointer to the value of Object index
		 \param		objectName		Character pointer to the value of Object name
		 
		 \return	char*
		 */
		/*****************************************************************************/
		char* GetIndexName(char* idxId, char* objectName);
};
#endif // ObjectDictionary_h
