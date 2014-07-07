/**
 *****************************************************************************
 \file		NodeCollection.h

 \brief		This file contains the class declaration to manage the inclusion and retrieval of Nodes in the collection list
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
#ifndef NodeCollection_h
#define NodeCollection_h

/****************************************************************************/
/* Includes */

#include "Declarations.h"
#include "Node.h"
#include <vector>

using namespace std;

/**
 *****************************************************************************
 \class		NodeCollection
 \brief		Handles collection of each node's information on id,name,type present inside the network

 *****************************************************************************/
class NodeCollection
{
	public:
		~NodeCollection(void);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the count of Node objects in the collection list

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetNumberOfNodes();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add a Node object to a collection list and update the count

		 \param		nodeObj       Class Variable of Node to hold the Node object

		 \return	void
		 */
		/*****************************************************************************/
		void AddNode(Node nodeObj);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete the Node objects at given position in the collection list

		 \param		nodePos    Integer to hold the position of Node object to be deleted

		 \return	void
		 */
		/*****************************************************************************/
		void DeleteNode(INT32 nodePos);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the pointer to NodeCollection object to be used as the collection list

		 \return	NodeCollection*
		 */
		/*****************************************************************************/
		static NodeCollection* GetNodeColObjectPointer();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Node object of given type and ID from the collection list

		 \param		nodeType  	Enum variable of NodeType to hold the value of Node type
		 \param		nodeId		Integer to hold the value of Node id

		 \return	Node
		 */
		/*****************************************************************************/
		Node GetNode(NodeType nodeType, INT32 nodeId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Node object of ID from the collection list

		 \param		nodeId		Integer to hold the value of Node ID

		 \return	Node
		 */
		/*****************************************************************************/
		Node GetNode(INT32 nodeId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Node object at the given position from the collection list

		 \param		position    Integer to hold value of column index

		 \return	Node
		 */
		/*****************************************************************************/
		Node GetNodebyCollectionIndex(INT32 position);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the MN Node object from the collection list

		 \return	Node
		 */
		/*****************************************************************************/
		Node GetMNNode();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return pointer to the Node object of given type and ID from the collection list

		 \param		nodeType  	Enum variable of NodeType to hold the value of Node type
		 \param		nodeId		Integer to hold the value of Node id

		 \return	Node* / NULL
		 \note The returned pointer will possibly be invalidated by adding/removing Index-Objects after obtaining it.
		 \deprecated Use NodeCollection::GetNodePtr(const UINT32 nodeId) instead.
		 */
		/*****************************************************************************/
		Node* GetNodePtr(NodeType nodeType, INT32 nodeId);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return pointer to the Node object at the given position from the collection list

		 \param		position   Integer to hold value of column index

		 \return	Node*
		 \note The returned pointer will possibly be invalidated by adding/removing Index-Objects after obtaining it.
		 \deprecated Use NodeCollection::GetNodePtr(const UINT32 nodeId) instead.
		 */
		/*****************************************************************************/
		Node* GetNodebyColIndex(INT32 position);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the count of CN objects in the collection list

		 This is a member function of CNodeCollection collects the CN node count

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetCNNodesCount();

		/************************************************************************
		\brief Query if this collection contains a Node with the given nodeId.

		\param nodeId   POWERLINK-NodeId of the Node.

		\return true if Node exists, false otherwise.
		\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
		************************************************************************/
		bool ContainsNode(const UINT32 nodeId);

		/************************************************************************
		\brief Gets a pointer to a Node by its nodeId.

		\param nodeId POWERLINK-NodeId of the Node.

		\return Pointer to Node-Object or NULL if not found.
		\note The returned pointer will possibly be invalidated by adding/removing Node-Objects after obtaining it.
		\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
		************************************************************************/
		Node* GetNodePtr(const UINT32 nodeId);

		/************************************************************************
		\brief Gets a reference to a Node by its nodeId.

		\param nodeId POWERLINK-NodeId of the Node.

		\return Reference to Node-Object.
		\throws ocfmRetCode ConfiguratorError OCFM_ERR_NODEID_NOT_FOUND if nodeId is not in the collection.
		\note The returned reference will possibly be invalidated by adding/removing Node-Objects after obtaining it.
		\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
		************************************************************************/
		Node& GetNodeRef(const UINT32 nodeId);

		/************************************************************************
		\brief Returns the existing nodes.

		\return NodeID vector.

		\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
		************************************************************************/
		const std::vector<UINT32> GetNodeIds();
	private:
		NodeCollection(void);
		NodeCollection(const NodeCollection&);

		static NodeCollection* objNodeColl;
		vector<Node> nodeCollObj;
};
#endif // NodeCollection_h
