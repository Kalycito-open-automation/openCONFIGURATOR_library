/**
 *****************************************************************************************************
 \file			TCollection.h

 \brief			Handles collections of base items
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
#ifndef TCollection_h
#define TCollection_h

/****************************************************************************************************/
/* Includes */

#include <vector>

using namespace std;

template<class TBase>

/**
 ******************************************************************************************************
 \class			TCollection
 \brief			Collection of Base items
 
 ******************************************************************************************************/
class TCollection
{
	protected:
		/* The Vector container that will hold the collection of Items */
		vector<TBase> Items;
	public:
		int Add(void)
		{
			//Create a new base item
			TBase BaseItem;
			//Add the item to the container
			Items.push_back(BaseItem);
			//Return the position of the item within the container. 
			//Zero Based
			return (Items.size() - 1);
		}
		//Function to return the memory address of a specific Item
		TBase* GetAddress(int ItemKey)
		{
			return (&(Items[ItemKey]));
		}
		//Remove a specific Item from the collection
		void Remove(int ItemKey)
		{
			//Remove the Item using the vectors erase function
			//m_items.erase(GetAddress(ItemKey)); 
			Items.erase(Items.begin() + (ItemKey));
		}
		void Clear(void) //Clear the collection
		{
			Items.clear();
		}
		//Return the number of items in collection
		int Count(void)
		{
			return (Items.size()); //One Based
		}
		//Operator Returning a reference to TBase
		TBase& operator[](int ItemKey)
		{
			return (Items[ItemKey]);
		}

};
#endif // TCollection_h
