///////////////////////////////////////////////////////////////////////////////////////////////
//
//  $Header: $
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
		m_NodeCount =  collectionObj.Count();
}

/****************************************************************************************************
* Destructor
****************************************************************************************************/
CNodeCollection::~CNodeCollection(void)
{
		instanceFlag =false;
}

/****************************************************************************************************
* Function Name: CNodeCollection::getNumberOfNodes
* Description:
* Return value: INT32
****************************************************************************************************/
INT32 CNodeCollection::getNumberOfNodes()
{
		return m_NodeCount;
}

/****************************************************************************************************
* Function Name: CNodeCollection::addNode
* Description:
* Return value: void
****************************************************************************************************/
void CNodeCollection::addNode(CNode objNode)
{
		INT32 iItemPosition = collectionObj.Add();
		
		collectionObj[iItemPosition] = objNode;
		m_NodeCount= collectionObj.Count();
	
}
	
/****************************************************************************************************
* Function Name: CNodeCollection::deleteNode
* Description:
* Return value: void
****************************************************************************************************/
void CNodeCollection::deleteNode(int iNodeID)
{
		
       collectionObj.Remove(iNodeID);
	   m_NodeCount = collectionObj.Count();
	   //printf("\n\nAfter Delete - Count:%d\n\n", m_NodeCount);

}	

/****************************************************************************************************
* Function Name: CNodeCollection::getNodeColObjectPointer
* Description:
* Return value: CNodeCollection*
****************************************************************************************************/
CNodeCollection* CNodeCollection::getNodeColObjectPointer()
{
		if(!instanceFlag)
		{
				objNodeCollection = new CNodeCollection();
				instanceFlag 	  = true;
		}
		return objNodeCollection;
		
}
	
/****************************************************************************************************
* Function Name: CNodeCollection::getNodeColObject
* Description:
* Return value: CNodeCollection
****************************************************************************************************/
CNodeCollection CNodeCollection::getNodeColObject()
{
		if(!instanceFlag)
		{
				objNodeCollection 	= new CNodeCollection();
				instanceFlag		= true;
		}
		return *objNodeCollection;
		
}
	
/****************************************************************************************************
* Function Name: CNodeCollection::getNode
* Description:
* Return value: CNode
****************************************************************************************************/
CNode CNodeCollection::getNode(ENodeType enumNodeType,int iNodeID)
{
		INT32 iLoopCount;
		CNode objNode;
		
		for(iLoopCount=0; iLoopCount < m_NodeCount ; iLoopCount++)
		{		
				objNode = objNodeCollection->collectionObj[iLoopCount];
				
				if(objNode.getNodeType() == enumNodeType && objNode.getNodeId() == iNodeID)
				{
					return objNode;
				}
				
		}
		return objNode;				
}
	
/****************************************************************************************************
* Function Name: CNodeCollection::getNodePtr
* Description:
* Return value: CNode*
****************************************************************************************************/
CNode* CNodeCollection::getNodePtr(ENodeType enumNodeType,int iNodeID)
{
		INT32 iLoopCount;
		CNode* pobjNode;
		
		for(iLoopCount=0; iLoopCount < m_NodeCount ; iLoopCount++)
		{		
				pobjNode = &(objNodeCollection->collectionObj[iLoopCount]);				
				
				if(pobjNode->getNodeType() == enumNodeType && pobjNode->getNodeId() == iNodeID)
				{
					return pobjNode;
				}
				
		}
		return NULL;				
}
	
/****************************************************************************************************
* Function Name: CNodeCollection::getNode
* Description:
* Return value: CNode
****************************************************************************************************/
CNode CNodeCollection::getNode(int iNodeID)
{
		INT32 iLoopCount;
		CNode objNode;
		
		for(iLoopCount=0; iLoopCount < m_NodeCount ; iLoopCount++)
		{			
				objNode = objNodeCollection->collectionObj[iLoopCount];
				
				if(objNode.getNodeId() == iNodeID)
				{
					return objNode;
				}
				
		}
		return objNode;
		
}
	
/****************************************************************************************************
* Function Name: CNodeCollection::getMNNode
* Description:
* Return value: CNode
****************************************************************************************************/
CNode CNodeCollection::getMNNode()
{
			INT32 iLoopCount;
			CNode objNode;
			
			for(iLoopCount = 0; iLoopCount < m_NodeCount ; iLoopCount++)
			{
				
				objNode = objNodeCollection->collectionObj[iLoopCount];
				
				if(objNode.getNodeType() == MN )
				{
					return objNode;
				}
				
			}
			return objNode;				
}
	
/****************************************************************************************************
* Function Name: CNodeCollection::getNodebyCollectionIndex
* Description:
* Return value: CNode
****************************************************************************************************/
CNode CNodeCollection::getNodebyCollectionIndex(int iColIndex)
{
	//$Scout << "$S." << endl;
	return collectionObj[iColIndex];
}

/****************************************************************************************************
* Function Name: CNodeCollection::getNodebyColIndex
* Description:
* Return value: CNode*
****************************************************************************************************/
CNode* CNodeCollection::getNodebyColIndex(int iColIndex)
{
	return &collectionObj[iColIndex];
}

/****************************************************************************************************
* Function Name: CNodeCollection::getCNNodesCount
* Description:
* Return value: int
****************************************************************************************************/
INT32 CNodeCollection::getCNNodesCount() 
{
	INT32 iCNNodecount = 0;
	
	for(INT32 iLoopCount= 0; iLoopCount < collectionObj.Count(); iLoopCount++)
	{
		if(collectionObj[iLoopCount].getNodeType() == CN)
		{
			iCNNodecount++;
		}
	}
	return iCNNodecount;
}
