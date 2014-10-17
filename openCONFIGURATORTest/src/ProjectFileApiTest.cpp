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

\file       ProjectFileApiTest.cpp

\brief      Implements the Project Api Test for the openCONFIGURATOR unit tests

\author     Christoph Ruecker

\date       03.11.2014

\since      03.11.2014

\version    1.0

\note       openCONFIGURATOR

*******************************************************************************/
#include "../Include/ProjectFileApiTest.h"

using namespace openCONFIGURATOR::Library::API;
using namespace openCONFIGURATOR::GUI::ProjectFile;

CPPUNIT_TEST_SUITE_REGISTRATION(ProjectFileApiTest);

ProjectFileApiTest::ProjectFileApiTest() {}

ProjectFileApiTest::~ProjectFileApiTest() {}

void ProjectFileApiTest::setUp() {}

void ProjectFileApiTest::tearDown() {}

void ProjectFileApiTest::testAddViewSetting()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::BASIC, "BasicSettingName", "BasicValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::ADVANCED, "AdvancedSettingName", "AdvancedValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	std::string value;
	this->retCode = GetViewSetting(ViewType::BASIC, "BasicSettingName", value);
	CPPUNIT_ASSERT(value == "BasicValue" && this->retCode.IsSuccessful() == true);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}
void ProjectFileApiTest::testDeleteViewSetting()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::BASIC, "BasicSettingName", "BasicValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::ADVANCED, "AdvancedSettingName", "AdvancedValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	std::string value;
	this->retCode = DeleteViewSetting(ViewType::BASIC, "BasicSettingName");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetViewSetting(ViewType::BASIC, "BasicSettingName", value);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == VIEW_SETTING_DOES_NOT_EXIST && this->retCode.IsSuccessful() == false);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void ProjectFileApiTest::testGetActiveView()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::BASIC, "BasicSettingName", "BasicValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::ADVANCED, "AdvancedSettingName", "AdvancedValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	ViewType::ViewType activeViewType;
	this->retCode = GetActiveView(activeViewType);
	CPPUNIT_ASSERT(activeViewType == 0 && this->retCode.IsSuccessful() == true);

	this->retCode = SetActiveView(ViewType::ADVANCED);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetActiveView(activeViewType);
	CPPUNIT_ASSERT(activeViewType == 1 && this->retCode.IsSuccessful() == true);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void ProjectFileApiTest::testGetViewSetting()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::BASIC, "BasicSettingName", "BasicValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::ADVANCED, "AdvancedSettingName", "AdvancedValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	std::string value;
	this->retCode = DeleteViewSetting(ViewType::BASIC, "BasicSettingName");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetViewSetting(ViewType::BASIC, "BasicSettingName", value);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == VIEW_SETTING_DOES_NOT_EXIST && this->retCode.IsSuccessful() == false);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void ProjectFileApiTest::testSetActiveView()
{
	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::BASIC, "BasicSettingName", "BasicValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddViewSetting(ViewType::ADVANCED, "AdvancedSettingName", "AdvancedValue");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	ViewType::ViewType activeViewType;
	this->retCode = GetActiveView(activeViewType);
	CPPUNIT_ASSERT(activeViewType == 0 && this->retCode.IsSuccessful() == true);

	this->retCode = SetActiveView(ViewType::ADVANCED);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetActiveView(activeViewType);
	CPPUNIT_ASSERT(activeViewType == 1 && this->retCode.IsSuccessful() == true);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}
