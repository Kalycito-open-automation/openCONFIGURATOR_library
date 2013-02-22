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
 \class			CIndex
 \brief			This function has the definitions of Index collection,PDO type for all datatypes			  
 
 ******************************************************************************************************/

class DllExport CIndex: public CBaseIndex
{
	public:
		CIndex(void);
		~CIndex(void);
	private:
		INT32 m_SubIndexCount;
		EPDOType m_PDOType;
	private:
		TCollection<CSubIndex> collectionObj;
	public:
		void addSubIndex(CSubIndex objSubIndex);
		void deleteSubIndex(INT32 SubIndexID);
		void deleteSubIndexCollection();
		INT32 getNumberofSubIndexes();
		CSubIndex* getSubIndex(INT32 SubIndexId);
		EPDOType getPDOType();
		void setPDOType(EPDOType PDOType);
		CSubIndex* getSubIndexbyIndexValue(char* Index);
		void SwapSubObjects(INT32 pos1, INT32 pos2);
		void UpdateArraySubObjects();
};
#endif // Index_h
