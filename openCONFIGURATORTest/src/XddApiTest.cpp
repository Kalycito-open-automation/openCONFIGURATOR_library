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

\file       XddApiTest.cpp

\brief      Implements the XDD API Tests for the openCONFIGURATOR unit tests

\author     Christoph Ruecker

\date       23.08.2013

\since      23.08.2013

\version    1.0

\note       openCONFIGURATOR

*******************************************************************************/
#include "../include/XddApiTest.h"
#include "Exception.h"

using namespace openCONFIGURATOR::Library::API;
using namespace openCONFIGURATOR::Library::ObjectDictionary::PlkDataType;

CPPUNIT_TEST_SUITE_REGISTRATION(XddApiTest);

XddApiTest::XddApiTest() {}

XddApiTest::~XddApiTest() {}

void XddApiTest::setUp() {}

void XddApiTest::tearDown()
{
	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void XddApiTest::testGetFeatureValue()
{
	this-> retCode = NewProject("TestProject", ".", "./resources/openPOWERLINK_MN.xdd");
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	string featureValue;
	retCode = GetFeatureValue(240, MN_FEATURES, "DLLMNPResChaining", featureValue);
	CPPUNIT_ASSERT(featureValue.compare("true") == 0);
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);

	featureValue.clear();
	retCode = GetFeatureValue(240, GENERAL_FEATURES , "NMTBootTimeNotActive", featureValue);
	CPPUNIT_ASSERT(featureValue.compare("3000000") == 0);
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);

	featureValue.clear();
	retCode = GetFeatureValue(240, CN_FEATURES , "DLLCNFeatureMultiplex", featureValue);
	CPPUNIT_ASSERT(featureValue.compare("true") == 0);
	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);

	featureValue.clear();
	retCode = GetFeatureValue(240, CN_FEATURES , "notexisting", featureValue);
	CPPUNIT_ASSERT(featureValue.compare("") == 0);
	CPPUNIT_ASSERT(retCode.GetErrorCode() == FEATURE_VALUE_NOT_FOUND);
}
void XddApiTest::testGetDataTypeSize()
{
	UINT32 value = 0;

	//TEST UNSIGNED8
	Result retCode = GetDataTypeSize(PlkDataType::UNSIGNED8, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 1);

	//TEST BOOLEAN
	retCode = GetDataTypeSize(PlkDataType::BOOLEAN, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 1);

	//TEST INTEGER8
	retCode = GetDataTypeSize(PlkDataType::INTEGER8, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 1);

	//TEST UNSIGNED16
	retCode = GetDataTypeSize(PlkDataType::UNSIGNED16, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 2);

	//TEST INTEGER16
	retCode = GetDataTypeSize(PlkDataType::INTEGER16, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 2);

	//TEST INTEGER24
	retCode = GetDataTypeSize(PlkDataType::INTEGER24, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 3);

	//TEST UNSIGNED24
	retCode = GetDataTypeSize(PlkDataType::UNSIGNED24, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 3);

	//TEST UNSIGNED32
	retCode = GetDataTypeSize(PlkDataType::UNSIGNED32, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 4);

	//TEST INTEGER32
	retCode = GetDataTypeSize(PlkDataType::INTEGER32, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 4);

	//TEST REAL32
	retCode = GetDataTypeSize(PlkDataType::REAL32, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 4);

	//TEST IP_ADDRESS
	retCode = GetDataTypeSize(PlkDataType::IP_ADDRESS, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 4);

	//TEST INTEGER40
	retCode = GetDataTypeSize(PlkDataType::INTEGER40, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 5);

	//TEST UNSIGNED40
	retCode = GetDataTypeSize(PlkDataType::UNSIGNED40, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 5);

	//TEST INTEGER48
	retCode = GetDataTypeSize(PlkDataType::INTEGER48, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 6);

	//TEST UNSIGNED48
	retCode = GetDataTypeSize(PlkDataType::UNSIGNED48, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 6);

	//TEST MAC_ADDRESS
	retCode = GetDataTypeSize(PlkDataType::MAC_ADDRESS, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 6);

	//TEST TIME_DIFF
	retCode = GetDataTypeSize(PlkDataType::TIME_DIFF, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 6);

	//TEST TIME_OF_DAY
	retCode = GetDataTypeSize(PlkDataType::TIME_OF_DAY, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 6);

	//TEST INTEGER56
	retCode = GetDataTypeSize(PlkDataType::INTEGER56, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 7);

	//TEST UNSIGNED56
	retCode = GetDataTypeSize(PlkDataType::UNSIGNED56, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 7);

	//TEST UNSIGNED64
	retCode = GetDataTypeSize(PlkDataType::UNSIGNED64, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 8);

	//TEST INTEGER64
	retCode = GetDataTypeSize(PlkDataType::INTEGER64, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 8);

	//TEST INTEGER64
	retCode = GetDataTypeSize(PlkDataType::REAL64, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 8);

	//TEST NETTIME
	retCode = GetDataTypeSize(PlkDataType::NETTIME, value);

	CPPUNIT_ASSERT(retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(value == 8);
}