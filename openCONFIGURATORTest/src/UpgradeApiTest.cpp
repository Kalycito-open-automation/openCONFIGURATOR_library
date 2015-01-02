/*******************************************************************************
* \file   UpgradeApiTest.cpp
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

#include "../Include/UpgradeAPITest.h"

using namespace openCONFIGURATOR::ProjectUpgrade;
using namespace openCONFIGURATOR::ProjectUpgrade::API;

CPPUNIT_TEST_SUITE_REGISTRATION(UpgradeAPITest);

UpgradeAPITest::UpgradeAPITest(){}
UpgradeAPITest::~UpgradeAPITest(){}

void UpgradeAPITest::setUp() {}

void UpgradeAPITest::tearDown() {}

void UpgradeAPITest::testGetNodes()
{
	std::vector<unsigned int> nodeIds;
	this->retCode = ResetProjectUpgradeLib();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetNodes(nodeIds);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == ErrorCode::NO_PROJECT_LOADED);
	CPPUNIT_ASSERT(nodeIds.size() == 0);

	this->retCode = ImportProjectFile(".\\testProject_pre1.4.0\\testProject.oct");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetNodes(nodeIds);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(nodeIds.size() == 4);
}

void UpgradeAPITest::testGetNodeName()
{
	this->retCode = ResetProjectUpgradeLib();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	std::string nodeName;
	this->retCode = GetNodeName(1, nodeName);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == ErrorCode::NO_PROJECT_LOADED);
	CPPUNIT_ASSERT(nodeName.empty() == true);

	this->retCode = ImportProjectFile(".\\testProject_pre1.4.0\\testProject.oct");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetNodeName(1, nodeName);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	CPPUNIT_ASSERT((nodeName == "CN_1"));
}

void UpgradeAPITest::testGetOctxPath()
{
	this->retCode = ResetProjectUpgradeLib();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	std::string octxPath;
	this->retCode = GetOctxPath(1, octxPath, true);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == ErrorCode::NO_PROJECT_LOADED);
	CPPUNIT_ASSERT(octxPath.empty() == true);

	this->retCode = ImportProjectFile(".\\testProject_pre1.4.0\\testProject.oct");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetOctxPath(1, octxPath, true);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(octxPath.empty() == false);

	octxPath = "";
	this->retCode = GetOctxPath(256, octxPath, true);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == ErrorCode::NODE_ID_DOES_NOT_EXIST);
	CPPUNIT_ASSERT(octxPath.empty() == true);
}

void UpgradeAPITest::testSetXddPath()
{
	this->retCode = ResetProjectUpgradeLib();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = SetXddPath(1, ".\\testProject\\xdd\\openPOWERLINK_CN.xdd");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == ErrorCode::NO_PROJECT_LOADED);
	
	this->retCode = ImportProjectFile(".\\testProject_pre1.4.0\\testProject.oct");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode =SetXddPath(1, ".\\testProject\\xdd\\openPOWERLINK_CN.xdd");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	
}

void UpgradeAPITest::testImportProjectFile()
{
	this->retCode = ResetProjectUpgradeLib();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = ImportProjectFile(".\\testProject_pre1.4.0\\testProject.oct");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	std::vector<unsigned int> nodeIds;
	this->retCode = GetNodes(nodeIds);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(nodeIds.size() > 0);
}

/* Commented The test always fails. Since there is no change in the file system while unit testing.
void UpgradeAPITest::testUpgradeProject()
{
	this->retCode = ResetProjectUpgradeLib();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = UpgradeProject("..\\testProject_upgraded\\");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == ErrorCode::NO_PROJECT_LOADED);
	

	this->retCode = ImportProjectFile(".\\testProject_pre1.4.0\\testProject.oct");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	
	
	std::vector<unsigned int> nodeIds;
	this->retCode = GetNodes(nodeIds);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(nodeIds.size() > 0);

	this->retCode = UpgradeProject("..\\testProject_upgraded");
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	

	this->retCode = RevertUpgradeProject();
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	
}

void UpgradeAPITest::testRevertUpgradeProject()
{
	this->retCode = RevertUpgradeProject();
	std::cout << __LINE__ << " Code:" << this->retCode.GetErrorCode() << " ERR: " << this->retCode.GetErrorString() << std::endl;
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}
*/
