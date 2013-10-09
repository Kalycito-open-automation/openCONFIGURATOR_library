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

CPPUNIT_TEST_SUITE_REGISTRATION( XddApiTest );

XddApiTest::XddApiTest(void)
{
}


XddApiTest::~XddApiTest(void)
{
}


void XddApiTest::testGetFeatureValue(){
	//TO BE EXTENDED
	string featureValue;
	ocfmRetCode retCode = GetFeatureValue(1,CN_FEATURES,"PResChaining",featureValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_NO_NODES_FOUND);
}
void XddApiTest::testGetDataTypeSize(){
	UINT32 value = 0;
	ocfmRetCode retCode(OCFM_ERR_UNKNOWN);
	
	//TEST UNSIGNED8
	retCode = GetDataTypeSize("unSigneD8", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 1);

	//TEST BOOLEAN
	retCode = GetDataTypeSize("bOOlEAn", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 1);

	//TEST INTEGER8
	retCode = GetDataTypeSize("INTeger8", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 1);

	//TEST UNSIGNED16
	retCode = GetDataTypeSize("UNSIGNED16", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 2);

	//TEST INTEGER16
	retCode = GetDataTypeSize("INTEGER16", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 2);

	//TEST INTEGER24
	retCode = GetDataTypeSize("INTEGER24", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 3);

	//TEST UNSIGNED24
	retCode = GetDataTypeSize("UNSIGNED24", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 3);

	//TEST UNSIGNED32
	retCode = GetDataTypeSize("UNSIGNED32", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 4);

	//TEST INTEGER32
	retCode = GetDataTypeSize("INTEGER32", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 4);

	//TEST REAL32
	retCode = GetDataTypeSize("REAL32", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 4);

	//TEST IP_ADDRESS
	retCode = GetDataTypeSize("IP_ADDRESS", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 4);

	//TEST INTEGER40
	retCode = GetDataTypeSize("INTEGER40", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 5);

	//TEST UNSIGNED40
	retCode = GetDataTypeSize("UNSIGNED40", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 5);

	//TEST INTEGER48
	retCode = GetDataTypeSize("INTEGER48", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 6);

	//TEST UNSIGNED48
	retCode = GetDataTypeSize("UNSIGNED48", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 6);

	//TEST MAC_ADDRESS
	retCode = GetDataTypeSize("MAC_ADDRESS", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 6);

	//TEST TIME_DIFF
	retCode = GetDataTypeSize("TIME_DIFF", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 6);

	//TEST TIME_OF_DAY
	retCode = GetDataTypeSize("TIME_OF_DAY", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 6);

	//TEST INTEGER56
	retCode = GetDataTypeSize("INTEGER56", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 7);

	//TEST UNSIGNED56
	retCode = GetDataTypeSize("UNSIGNED56", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 7);

	//TEST UNSIGNED64
	retCode = GetDataTypeSize("UNSIGNED64", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 8);

	//TEST INTEGER64
	retCode = GetDataTypeSize("INTEGER64", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 8);

	//TEST INTEGER64
	retCode = GetDataTypeSize("REAL64", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 8);

	//TEST NETTIME
	retCode = GetDataTypeSize("NETTIME", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(value == 8);

	//TEST ERROR HANDLING

	retCode = GetDataTypeSize("NOT_EXISTING", value);

	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_DATATYPE_NOT_FOUND);
	CPPUNIT_ASSERT(value == 0);
}