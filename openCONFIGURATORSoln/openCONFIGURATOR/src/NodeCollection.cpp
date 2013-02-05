///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Source: $
//
// NAME:  NodeCollection.cpp
//
// BASE  CLASSES: none
//  
// PURPOSE:  purpose description
//
// AUTHOR:  Kalycito Powerlink Team
//
//  COPYRIGHT NOTICE:
//
//	****************************************************************************

// (c) Kalycito Infotech Private Limited
//
//  Project:      openCONFIGURATOR 
//
//  Description:  
//
//
//  License:
//
//    Redistribution and use in source and binary forms, with or without
//    modification, are permitted provided that the following conditions
//    are met:
//
//    1. Redistributions of source code must retain the above copyright
//       notice, this list of conditions and the following disclaimer.
//
//    2. Redistributions in binary form must reproduce the above copyright
//       notice, this list of conditions and the following disclaimer in the
//       documentation and/or other materials provided with the distribution.
//
//    3. Neither the name of Kalycito Infotech Private Limited nor the names of 
//       its contributors may be used to endorse or promote products derived
//       from this software without prior written permission. For written
//       permission, please contact info@kalycito.com.
//
//    THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
//    "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
//    LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
//    FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
//    COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
//    INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
//    BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
//    LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
//    CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
//    LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
//    ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
//    POSSIBILITY OF SUCH DAMAGE.
//
//    Severability Clause:
//
//        If a provision of this License is or becomes illegal, invalid or
//        unenforceable in any jurisdiction, that shall not affect:
//        1. the validity or enforceability in that jurisdiction of any other
//           provision of this License; or
//        2. the validity or enforceability in other jurisdictions of that or
//           any other provision of this License.
//
//****************************************************************************/
//
//  REVISION HISTORY:
// $Log:      $
///////////////////////////////////////////////////////////////////////////////////////////////

/****************************************************************************************************
 * Includes
 ****************************************************************************************************/
#include <stdio.h>
#include "../Include/NodeCollection.h"
#include "../Include/Node.h"
//$S Only for testing

/****************************************************************************************************
 * Global Variables
 ****************************************************************************************************/
bool CNodeCollection::instanceFlag = false;
CNodeCollection* CNodeCollection::objNodeCollection = NULL;

/****************************************************************************************************
 * FUNCTION DEFINITIONS
 ****************************************************************************************************/
/****************************************************************************************************
 * Constructor
 ****************************************************************************************************/
CNodeCollection::CNodeCollection(void)
{
	m_NodeCount = collectionObj.Count();
}

/****************************************************************************************************
 * Destructor
 ****************************************************************************************************/
CNodeCollection::~CNodeCollection(void)
{
	instanceFlag = false;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNumberOfNodes
 * Description  : This function returns node count
 * Return value : INT32
 ****************************************************************************************************/
INT32 CNodeCollection::getNumberOfNodes()
{
	return m_NodeCount;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::addNode
 * Description  : This function collects objects under node count
 * Return value : void
 ****************************************************************************************************/
void CNodeCollection::addNode(CNode objNode)
{
	INT32 iItemPosition = collectionObj.Add();

	collectionObj[iItemPosition] = objNode;
	m_NodeCount = collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CNodeCollection::deleteNode
 * Description  : This function deletes the node ids
 * Return value : void
 ****************************************************************************************************/
void CNodeCollection::deleteNode(INT32 iNodeID)
{
	collectionObj.Remove(iNodeID);
	m_NodeCount = collectionObj.Count();
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNodeColObjectPointer
 * Description  : This function collects the location of node
 * Return value : CNodeCollection*
 ****************************************************************************************************/
CNodeCollection* CNodeCollection::getNodeColObjectPointer()
{
	if (!instanceFlag)
	{
		objNodeCollection = new CNodeCollection();
		instanceFlag = true;
	}
	return objNodeCollection;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNodeColObject
 * Description  : This function collects value of node objects
 * Return value : CNodeCollection
 ****************************************************************************************************/
CNodeCollection CNodeCollection::getNodeColObject()
{
	if (!instanceFlag)
	{
		objNodeCollection = new CNodeCollection();
		instanceFlag = true;
	}
	return *objNodeCollection;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNode
 * Description  : This function collects the type of node objects
 * Return value : CNode
 ****************************************************************************************************/
CNode CNodeCollection::getNode(ENodeType enumNodeType, INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	CNode objNode;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.getNodeType() == enumNodeType
				&& objNode.getNodeId() == iNodeID)
		{
			return objNode;
		}
	}
	return objNode;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNodePtr
 * Description  : This function collects the location of node objects type 
 * Return value : CNode*
 ****************************************************************************************************/
CNode* CNodeCollection::getNodePtr(ENodeType enumNodeType, INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	CNode* pobjNode = NULL;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		pobjNode = &(objNodeCollection->collectionObj[iLoopCount]);

		if (pobjNode->getNodeType() == enumNodeType
				&& pobjNode->getNodeId() == iNodeID)
		{
			return pobjNode;
		}
	}
	return NULL;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNode
 * Description:   This function collects the type of node objects
 * Return value: CNode
 ****************************************************************************************************/
CNode CNodeCollection::getNode(INT32 iNodeID)
{
	INT32 iLoopCount = 0;
	CNode objNode;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.getNodeId() == iNodeID)
		{
			return objNode;
		}
	}
	return objNode;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getMNNode
 * Description:  This function collects the type of node objects for MN
 * Return value: CNode
 ****************************************************************************************************/
CNode CNodeCollection::getMNNode()
{
	INT32 iLoopCount;
	CNode objNode;

	for (iLoopCount = 0; iLoopCount < m_NodeCount; iLoopCount++)
	{
		objNode = objNodeCollection->collectionObj[iLoopCount];

		if (objNode.getNodeType() == MN)
		{
			return objNode;
		}

	}
	return objNode;
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNodebyCollectionIndex
 * Description:   This function returns node from the node collection index
 * Return value: CNode
 ****************************************************************************************************/
CNode CNodeCollection::getNodebyCollectionIndex(INT32 iColIndex)
{
	return collectionObj[iColIndex];
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getNodebyColIndex
 * Description:   This function collects the node with reference to the index location 
 * Return value: CNode*
 ****************************************************************************************************/
CNode* CNodeCollection::getNodebyColIndex(INT32 iColIndex)
{
	return &collectionObj[iColIndex];
}

/****************************************************************************************************
 * Function Name: CNodeCollection::getCNNodesCount
 * Description:  This function collects the CN node count 
 * Return value: INT32
 ****************************************************************************************************/
INT32 CNodeCollection::getCNNodesCount()
{
	INT32 iCNNodecount = 0;

	for (INT32 iLoopCount = 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		if (collectionObj[iLoopCount].getNodeType() == CN)
		{
			iCNNodecount++;
		}
	}
	return iCNNodecount;
}
