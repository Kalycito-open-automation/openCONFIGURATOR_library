/*******************************************************************************
*
* License Agreement
*
* Copyright © 2013 BERNECKER + RAINER, AUSTRIA, 5142 EGGELSBERG, B&R STRASSE 1
* All rights reserved.
*
* Redistribution and use in source and binary forms,
* with or without modification,
* are permitted provided that the following conditions are met:
*
*   * Redistributions of source code must retain the above copyright notice,
*     this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright notice,
*     this list of conditions and the following disclaimer
*     in the documentation and/or other materials provided with the
*     distribution.
*   * Neither the name of the B&R nor the names of its contributors
*     may be used to endorse or promote products derived from this software
*     without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
* AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
* THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
* A PARTICULAR PURPOSE ARE DISCLAIMED.
* IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE FOR
* ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
* THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*
*******************************************************************************/
/**
********************************************************************************

\file       NodeApiTest.cpp

\brief      Implements the NodeApiTest for the openCONFIGURATOR unit tests

\author     Christoph Ruecker

\date       23.08.2013

\since      23.08.2013

\version    1.0

\note       openCONFIGURATOR

*******************************************************************************/
#include "..\Include\NodeApiTest.h"
using namespace openCONFIGURATOR::Library::API;
using namespace openCONFIGURATOR::Library::NodeParameter;

CPPUNIT_TEST_SUITE_REGISTRATION(NodeApiTest);

NodeApiTest::NodeApiTest() {}

NodeApiTest::~NodeApiTest() {}

void NodeApiTest::setUp() {}

void NodeApiTest::tearDown()
{
	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void NodeApiTest::testAddNode()
{
	this->retCode = NewProject("TestProject", ".", "./resources/openPOWERLINK_MN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(12, "Node", "");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(12, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.GetErrorCode() == NODE_EXISTS);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
}
void NodeApiTest::testDeleteNode()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(12, "Node");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = DeleteNode(12);
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = DeleteNode(12);
	CPPUNIT_ASSERT(retCode.GetErrorCode() == NODE_DOES_NOT_EXIST);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
}
void NodeApiTest::testReplaceXdd()
{
	//TODO implement test
}
void NodeApiTest::testIsExistingNode()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(12, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(12, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.GetErrorCode() == NODE_EXISTS);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
}
void NodeApiTest::testGetNodeCount()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(1, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(2, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(3, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(4, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	UINT32 nodeCount = 0;
	retCode = openCONFIGURATOR::Library::API::GetNodeCount(nodeCount);
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(nodeCount == 5);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
}
void NodeApiTest::testGetNodeParameter()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(1, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	std::string value;
	retCode = GetNodeParameter(1, NODENAME, value);
	CPPUNIT_ASSERT(value == "Node" && retCode.IsSuccessful() == true);
	value = "";
	retCode = GetNodeParameter(1, STATIONTYPE, value);
	CPPUNIT_ASSERT(value == "0" && retCode.IsSuccessful() == true);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
}
void NodeApiTest::testSetNodeParameter()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(1, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	std::string value;
	retCode = SetNodeParameter(1, NODENAME, "TestCN");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = GetNodeParameter(1, NODENAME, value);
	CPPUNIT_ASSERT(value == "TestCN" && retCode.IsSuccessful() == true);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
}
void NodeApiTest::testGetNodes()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(1, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(2, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(3, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	retCode = AddNode(4, "Node", "./resources/openPOWERLINK_CN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	std::vector<UINT32> nodes;
	retCode = GetNodes(nodes);
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(nodes.size() == 5);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);

}