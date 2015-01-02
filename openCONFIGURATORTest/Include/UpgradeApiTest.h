/*******************************************************************************
* \file   UpgradeApiTest.h
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

#pragma once
#include <cppUnit/TestCase.h>
#include <cppUnit/extensions/HelperMacros.h>
#include "UpgradeAPI.h"


class UpgradeAPITest : public CppUnit::TestFixture
{
	CPPUNIT_TEST_SUITE(UpgradeAPITest);
	CPPUNIT_TEST(testGetNodes);
	CPPUNIT_TEST(testGetNodeName);
	CPPUNIT_TEST(testGetOctxPath);
	CPPUNIT_TEST(testSetXddPath);
	CPPUNIT_TEST(testImportProjectFile);
	/*
	CPPUNIT_TEST(testUpgradeProject);
	CPPUNIT_TEST(testRevertUpgradeProject);
	*/
	CPPUNIT_TEST_SUITE_END();

public:
	UpgradeAPITest();
	~UpgradeAPITest();

	void setUp();
	void tearDown();

	void testGetNodes();
	void testGetNodeName();
	void testGetOctxPath();
	void testSetXddPath();
	void testImportProjectFile();
	/*
	void testUpgradeProject();
	void testRevertUpgradeProject();
	*/

private:
		openCONFIGURATOR::ProjectUpgrade::Result retCode;

};
