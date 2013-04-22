/**
 ************************************************************************************************
 \file			NodeCollection.cpp

 \brief			Collects node id,count,location of all nodes present in the network
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

 ****************************************************************************/

/****************************************************************************************************/
/* Includes */

#include <stdio.h>
#include "../Include/NodeCollection.h"
#include "../Include/Node.h"

/****************************************************************************************************/
/* Global Variables */

bool NodeCollection::instanceFlag = false;
NodeCollection* NodeCollection::objNodeColl = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

NodeCollection::NodeCollection(void)
{
	nodeCount = nodeCollObj.Count();
}

/*************************************************************************/
/* Destructor */

/**
 

 */

NodeCollection::~NodeCollection(void)
{
	instanceFlag = false;
}

INT32 NodeCollection::GetNumberOfNodes()
{
	return nodeCount;
}

void NodeCollection::AddNode(Node nodeObj)
{
	INT32 iItemPosition = nodeCollObj.Add();

	nodeCollObj[iItemPosition] = nodeObj;
	nodeCount = nodeCollObj.Count();
}

void NodeCollection::DeleteNode(INT32 nodePos)
{
	nodeCollObj.Remove(nodePos);
	nodeCount = nodeCollObj.Count();
}

NodeCollection* NodeCollection::GetNodeColObjectPointer()
{
	if (!instanceFlag)
	{
		objNodeColl = new NodeCollection();
		instanceFlag = true;
	}
	return objNodeColl;
}

NodeCollection NodeCollection::GetNodeColObject()
{
	if (!instanceFlag)
	{
		objNodeColl = new NodeCollection();
		instanceFlag = true;
	}
	return *objNodeColl;
}

Node NodeCollection::GetNode(NodeType nodeType, INT32 nodeId)
{
	INT32 nodeLC = 0;
	Node objNode;

	for (nodeLC = 0; nodeLC < nodeCount; nodeLC++)
	{
		objNode = objNodeColl->nodeCollObj[nodeLC];

		if ((objNode.GetNodeType() == nodeType)
				&& (objNode.GetNodeId() == nodeId))
		{
			return objNode;
		}
	}
	return objNode;
}

Node* NodeCollection::GetNodePtr(NodeType nodeType, INT32 nodeId)
{
	INT32 nodeLC = 0;
	Node* objNode = NULL;

	for (nodeLC = 0; nodeLC < nodeCount; nodeLC++)
	{
		objNode = &(objNodeColl->nodeCollObj[nodeLC]);

		if ((objNode->GetNodeType() == nodeType)
				&& (objNode->GetNodeId() == nodeId))
		{
			return objNode;
		}
	}
	return NULL;
}

//TODO: unused function
Node NodeCollection::GetNode(INT32 nodeId)
{
	INT32 nodeLC = 0;
	Node objNode;

	for (nodeLC = 0; nodeLC < nodeCount; nodeLC++)
	{
		objNode = objNodeColl->nodeCollObj[nodeLC];

		if (objNode.GetNodeId() == nodeId)
		{
			return objNode;
		}
	}
	return objNode;
}

Node NodeCollection::GetMNNode()
{
	INT32 nodeLC;
	Node objNode;

	for (nodeLC = 0; nodeLC < nodeCount; nodeLC++)
	{
		objNode = objNodeColl->nodeCollObj[nodeLC];

		if (objNode.GetNodeType() == MN)
		{
			return objNode;
		}

	}
	return objNode;
}

Node NodeCollection::GetNodebyCollectionIndex(INT32 position)
{
	return nodeCollObj[position];
}

Node* NodeCollection::GetNodebyColIndex(INT32 position)
{
	return &nodeCollObj[position];
}

INT32 NodeCollection::GetCNNodesCount()
{
	INT32 cnNodeCount = 0;

	for (INT32 nodeCountLC = 0; nodeCountLC < nodeCollObj.Count();
			nodeCountLC++)
	{
		if (nodeCollObj[nodeCountLC].GetNodeType() == CN)
		{
			cnNodeCount++;
		}
	}
	return cnNodeCount;
}
