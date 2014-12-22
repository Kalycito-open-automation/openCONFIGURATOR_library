/*******************************************************************************
* \file   NodeCollection.cpp
*
* \author Ramakrishnan Periyakaruppan, Kalycito Infotech Private Limited.
*
* \copyright (c) 2014, Kalycito Infotech Private Limited
*                    All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of the copyright holders nor the
*     names of its contributors may be used to endorse or promote products
*     derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include "../include/NodeCollection.h"
#include "../include/Result.h"

#include <boost/format.hpp>

PROJECT_UPGRADE_OPEN_NAMESPACE

NodeCollection NodeCollection::instance;

NodeCollection::NodeCollection()
	: nodeCollection()
{

}

NodeCollection& NodeCollection::GetInstance()
{
	return NodeCollection::instance;
}

void NodeCollection::Add(Node nodeObj)
{
	this->nodeCollection.push_back(nodeObj);
}

void NodeCollection::Clear()
{
	this->nodeCollection.clear();
}

UINT32 NodeCollection::GetNumberOfNodes() const
{
	return this->nodeCollection.size();
}

bool NodeCollection::ContainsNode(const UINT32 nodeId)
{
	return (this->GetNodePtr(nodeId) != NULL);
}

Node* NodeCollection::GetNodePtr(const UINT32 nodeId)
{
	std::vector<Node>::iterator it = this->nodeCollection.begin();
	for (;
		 it != this->nodeCollection.end() && ((it->GetNodeId()) != nodeId);
		 ++it)
	{}
	if (it == this->nodeCollection.end())
		return NULL;
	else
		return &(*it);
}

Node& NodeCollection::GetNodeRef(const UINT32 nodeId)
{
	Node* node = this->GetNodePtr(nodeId);
	if (node)
		return *node;
	else
	{
		boost::format formatter(kMsgNonExistingNode);
		formatter % nodeId;
		throw Result(ErrorCode::NODE_ID_DOES_NOT_EXIST, formatter.str());
	}
}

Node* NodeCollection::GetNodebyColIndex(const UINT32 position)
{
	return &nodeCollection[position];
}

const std::vector<UINT32> NodeCollection::GetNodeIds() const
{
	std::vector<UINT32> returnIDs;
	for (std::vector<Node>::const_iterator it = this->nodeCollection.begin();
		 it != this->nodeCollection.end();
		 ++it)
	{
		returnIDs.push_back(it->GetNodeId());
	}
	return returnIDs;
}

PROJECT_UPGRADE_CLOSE_NAMESPACE
