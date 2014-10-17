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

\file       ImportExportApiTest.cpp

\brief      Implements the Import Export Api Test for the openCONFIGURATOR unit tests

\author     Christoph Ruecker

\date       23.08.2013

\since      23.08.2013

\version    1.0

\note       openCONFIGURATOR

*******************************************************************************/
#include "..\Include\ImportExportApiTest.h"
using namespace openCONFIGURATOR::Library::API;

CPPUNIT_TEST_SUITE_REGISTRATION(ImportExportApiTest);

ImportExportApiTest::ImportExportApiTest(void) {}

ImportExportApiTest::~ImportExportApiTest(void) {}

void ImportExportApiTest::setUp() {}

void ImportExportApiTest::tearDown()
{
	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void ImportExportApiTest::testGenerateProcessImageDescription()
{
	this->retCode = OpenProject(".\\testProject\\testProject.xml");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	//this->retCode = GenerateStackConfiguration(".\\testProject\\output", "mnobd");
	//CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GenerateProcessImageDescription(C, ".\\testProject\\output", "XAP");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GenerateProcessImageDescription(CSHARP, ".\\testProject\\output", "PI");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void ImportExportApiTest::testGenerateStackConfiguration()
{
	//Test already conducted within testGenerateProcessImageDescription()
}