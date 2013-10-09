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

CPPUNIT_TEST_SUITE_REGISTRATION( NodeApiTest );

NodeApiTest::NodeApiTest(void)
{
}


NodeApiTest::~NodeApiTest(void)
{
}

void NodeApiTest::testAddNode()
{
	ocfmRetCode retCode(OCFM_ERR_UNKNOWN);
	retCode = NewProject("TestProject",".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(12,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(12,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_NODE_ALREADY_EXISTS);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
}
void NodeApiTest::testDeleteNode()
{
	ocfmRetCode retCode(OCFM_ERR_UNKNOWN);
	retCode = NewProject("TestProject",".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(12,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = DeleteNode(12);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = DeleteNode(12);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_NODEID_NOT_FOUND);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
}
void NodeApiTest::testReplaceXdd()
{
	//TODO implement test
}
void NodeApiTest::testIsExistingNode()
{
	ocfmRetCode retCode(OCFM_ERR_UNKNOWN);
	retCode = NewProject("TestProject",".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(14,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(14,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_NODE_ALREADY_EXISTS);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
}
void NodeApiTest::testGetNodeCount()
{
	ocfmRetCode retCode(OCFM_ERR_UNKNOWN);
	retCode = NewProject("TestProject",".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(1,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(2,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(3,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddNode(4,CN,"Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	UINT32 nodeCount = GetNodeCount();
	CPPUNIT_ASSERT(nodeCount == 5);
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
}