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

\file       OdApiTest.cpp

\brief      Implements the OdApiTest for the openCONFIGURATOR unit tests

\author     Christoph Ruecker

\date       23.08.2013

\since      23.08.2013

\version    1.0

\note       openCONFIGURATOR

*******************************************************************************/
#include "../include/OdApiTest.h"
#include "../include/Enums.h"
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>


CPPUNIT_TEST_SUITE_REGISTRATION( OdApiTest );

OdApiTest::OdApiTest(void)
{

}

OdApiTest::~OdApiTest(void)
{

}
void OdApiTest::testAddIndex()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Add Index API : Start test." << endl;
	cout << "TEST: Add Index API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add Index API : Add index (2010) with name 'Index2010' to project without adding node: ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INVALID_INDEXID);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add Index API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add Index API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add Index API : Add index (2010) again to test error handling : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INDEX_ALREADY_EXISTS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add Index API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add Index API : End test." << endl;
#endif
}

void OdApiTest::testSetIndexAttribute()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Set Index Attribute API : Start test." << endl;
	cout << "TEST: Set Index Attribute API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute Name 'Index2010' from Index (2010) : ";
#endif
	string getNameString;
	retCode = GetIndexAttribute(1, 8208, NAME, getNameString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getNameString.compare("Index2010") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute Name 'Index2010New' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, NAME, "Index2010New");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute Name 'Index2010New' from Index (2010) : ";
#endif
	string setNameString;
	retCode = GetIndexAttribute(1, 8208, NAME, setNameString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(setNameString.compare("Index2010New") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute ObjectType 'DEFTYPE' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "DEFTYPE");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute ObjectType 'DEFTYPE' from Index (2010) : ";
#endif
	string getObjecttypeString;
	retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, getObjecttypeString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getObjecttypeString.compare("DEFTYPE") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute ObjectType 'DEFSTRUCT' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "DEFSTRUCT");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute ObjectType 'DEFSTRUCT' from Index (2010) : ";
#endif
	string setObjecttypeString;
	retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(setObjecttypeString.compare("DEFSTRUCT") == 0);
	setObjecttypeString.clear();
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute ObjectType 'VAR' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "VAR");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute ObjectType 'VAR' from Index (2010) : ";
#endif
	retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(setObjecttypeString.compare("VAR") == 0);
	setObjecttypeString.clear();

	//retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "ARRAY");
	//cout << retCode.getError() << endl;
	//CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);

	//retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	//CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	//CPPUNIT_ASSERT(setObjecttypeString.compare("ARRAY") == 0);
	//setObjecttypeString.clear();

	//retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "RECORD");
	//cout << retCode.getErrorCode() <<endl;
	//CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);

	//retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	//CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	//CPPUNIT_ASSERT(setObjecttypeString.compare("RECORD") == 0);

#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute AccessType 'ro' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, ACCESSTYPE, "ro");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute AccessType 'ro' from Index (2010) : ";
#endif
	string getAccesstypeString;
	retCode = GetIndexAttribute(1, 8208, ACCESSTYPE, getAccesstypeString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getAccesstypeString.compare("ro") == 0);

#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute HighLimit '10' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, HIGHLIMIT, "10");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute HighLimit '10' from Index (2010) : ";
#endif
	string getHighLimitString;
	retCode = GetIndexAttribute(1, 8208, HIGHLIMIT, getHighLimitString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getHighLimitString.compare("10") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute LowLimit '5' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, LOWLIMIT, "5");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute LowLimit '5' from Index (2010) : ";
#endif
	string getLowLimitString;
	retCode = GetIndexAttribute(1, 8208, LOWLIMIT, getLowLimitString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getLowLimitString.compare("5") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute LowLimit '11' from Index (2010) to provoke error : ";
#endif
	retCode = SetIndexAttribute(1, 8208, LOWLIMIT, "11");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INVALID_UPPERLOWER_LIMITS);

#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute HighLimit '4' from Index (2010) to provoke error : ";
#endif
	retCode = SetIndexAttribute(1, 8208, HIGHLIMIT, "4");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INVALID_UPPERLOWER_LIMITS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute DefaultValue '7' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, DEFAULTVALUE, "7");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute DefaultValue '7' from Index (2010) : ";
#endif
	string getDefaultValueString;
	retCode = GetIndexAttribute(1, 8208, DEFAULTVALUE, getDefaultValueString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getDefaultValueString.compare("7") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute ActualValue '8' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, ACTUALVALUE, "8");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute ActualValue '8' from Index (2010) : ";
#endif
	string getActualValueString;
	retCode = GetIndexAttribute(1, 8208, ACTUALVALUE, getActualValueString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getActualValueString.compare("8") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute ActualValue '16' from Index (2010) to provoke error : ";
#endif
	retCode = SetIndexAttribute(1, 8208, ACTUALVALUE, "0x16");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute PDOMapping 'not_defined' from Index (2010) : ";
#endif
	string getPDOMappingValue;
	retCode = GetIndexAttribute(1, 8208, PDOMAPPING, getPDOMappingValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getPDOMappingValue.compare("NOT_DEFINED") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute PDOMapping 'OPTIONAL' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, PDOMAPPING, "optional");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute PDOMapping 'optional' from Index (2010) : ";
#endif
	retCode = GetIndexAttribute(1, 8208, PDOMAPPING, getPDOMappingValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getPDOMappingValue.compare("OPTIONAL") == 0);

#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute FlagIfInCDC '0' (false) from Index (2010) : ";
#endif
	string getFlagincdcString;
	retCode = GetIndexAttribute(1, 8208, FLAGIFINCDC, getFlagincdcString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getFlagincdcString.compare("0") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute FlagIfInCDC '1' (true) from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, FLAGIFINCDC, "1");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute FlagIfInCDC '1' (true) from Index (2010) : ";
#endif
	string setFlagincdcString;
	retCode = GetIndexAttribute(1, 8208, FLAGIFINCDC, setFlagincdcString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(setFlagincdcString.compare("1") == 0);
	setFlagincdcString.clear();
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute FlagIfInCDC '0' (false) from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, FLAGIFINCDC, "false");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Get and compare attribute FlagIfInCDC '0' (false) from Index (2010) : ";
#endif
	retCode = GetIndexAttribute(1, 8208, FLAGIFINCDC, setFlagincdcString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(setFlagincdcString.compare("0") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Set index attribute ActualValue '16' from Index (2010) to provoke error : ";
#endif
	retCode = SetIndexAttribute(1, 8208, FLAGIFINCDC, "ttrue");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INVALID_VALUE);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Attribute API : End test." << endl;
#endif
}
void OdApiTest::testSetIndexActualValue()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Set Index Actual Value API : Start test." << endl;
	cout << "TEST: Set Index Actual Value API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : Set index attribute HighLimit '10' from Index (2010) : ";
#endif
	retCode = SetIndexAttribute(1, 8208, HIGHLIMIT, "10");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : Set index attribute LowLimit '5' from Index (2010) : ";
#endif
	//Check Low Limit
	retCode = SetIndexAttribute(1, 8208, LOWLIMIT, "5");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : Set index attribute ActualValue '8' from Index (2010) : ";
#endif
	retCode = SetIndexActualValue(1, 8208, "8");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : Get and compare attribute ActualValue '8' from Index (2010) : ";
#endif
	string getActualValueString;
	retCode = GetIndexAttribute(1, 8208, ACTUALVALUE, getActualValueString);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getActualValueString.compare("8") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set Index Actual Value API : End test." << endl;
#endif
}
//void OdApiTest::testGetIndexAttribute()
//{
//
//}
void OdApiTest::testAddSubIndex()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Add SubIndex API : Start test." << endl;
	cout << "TEST: Add SubIndex API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add SubIndex API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add SubIndex API : Add sub index (2010/12) with name 'SubIndex2010/12' to project without adding index first: ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INDEXID_NOT_FOUND);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add SubIndex API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add SubIndex API : Add sub index (2010/12) with name 'SubIndex2010/12' : ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add SubIndex API : Add sub index (2010/12) with name 'SubIndex2010/12' again to provoke error : ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUBINDEX_ALREADY_EXISTS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add SubIndex API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Add SubIndex API : End test." << endl;
#endif
}
void OdApiTest::testSetSubIndexAttribute()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Set SubIndex Attribute Value API : Start test." << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Add sub index (2010/12) with name 'SubIndex2010/12' : ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "0x12", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Get and compare actual value from 2010/12 '0x12' : ";
#endif
	string getSubIndexValue;
	retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("0x12") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Get and compare name value from 2010/12 'SubIndex2010/12' : ";
#endif
	retCode = SetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, "0x24");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Set actual value from 2010/12 '0x24' : ";
#endif
	retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("0x24") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Get and compare actual value from 2010/12 '0x24' : ";
#endif
	retCode = GetSubIndexAttribute(1, 8208, 12, NAME, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("SubIndex2010/12") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Get and compare PDOMAPPING value from 2010/12 'not_defined' : ";
#endif
	retCode = GetSubIndexAttribute(1, 8208, 12, PDOMAPPING, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("NOT_DEFINED") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Get and compare DEFAULTVALUE value from 2010/12 '' : ";
#endif
	retCode = GetSubIndexAttribute(1, 8208, 12, DEFAULTVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Get and compare FLAGIFINCDC value from 2010/12 'true' : ";
#endif
	retCode = GetSubIndexAttribute(1, 8208, 12, FLAGIFINCDC, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("0") == 0);

//TODO extend this test with more set / get operations

#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Attribute Value API : End test." << endl;
#endif
}

void OdApiTest::testSetSubIndexActualValue()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Set SubIndex Actual Value API : Start test." << endl;
	cout << "TEST: Set SubIndex Actual Value API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : Add sub index (2010/12) with name 'SubIndex2010/12' : ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "10000", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : Get and compare actual value from 2010/12 '10000' : ";
#endif
	string getSubIndexValue;
	retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("10000") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : Set actual value from 2010/12 '15000' : ";
#endif
	retCode = SetSubIndexActualValue(1, 8208, 12, "15000");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : Get and compare actual value from 2010/12 '15000' : ";
#endif
	retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	CPPUNIT_ASSERT(getSubIndexValue.compare("15000") == 0);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Set SubIndex Actual Value API : End test." << endl;
#endif
}

//void OdApiTest::testGetSubIndexAttribute()
//{
//}

void OdApiTest::testIsExistingIndex()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Is Existing Index API : Start test." << endl;
	cout << "TEST: Is Existing Index API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing Index API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing Index API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing Index API : Check if index 2010 exists : ";
#endif
	bool isExisting = IsExistingIndex(1, 8208);
	CPPUNIT_ASSERT(isExisting == true);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing Index API : Delete index 2010 : ";
#endif
	retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing Index API : Check if non existing index 2010 exists to provoke error: ";
#endif
	isExisting = IsExistingIndex(1, 8208);
	CPPUNIT_ASSERT(isExisting == false);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing Index API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing Index API : End test." << endl;
#endif
}

void OdApiTest::testIsExistingSubIndex()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Is Existing SubIndex API : Start test." << endl;
	cout << "TEST: Is Existing SubIndex API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : Add index (2010) with name 'Index2010' to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : Add sub index (2010/12) with name 'SubIndex2010/12' : ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : Check if sub index 2010/12 exists : ";
#endif
	bool isExisting = IsExistingSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(isExisting == true);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : Delete sub index 2010/12 : ";
#endif
	retCode = DeleteSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : Check if non existing sub index 2010/12 exists to provoke error: ";
#endif
	isExisting = IsExistingSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(isExisting == false);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Is Existing SubIndex API : End test." << endl;
#endif
}

void OdApiTest::testGetIndexCount()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Get Index Count API : Start test." << endl;
	cout << "TEST: Get Index Count API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Index Count API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Index Count API : Add indices 2010 - 2015 to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddIndex(1, 8209, "30", "Index2011", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddIndex(1, 8210, "30", "Index2012", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddIndex(1, 8211, "30", "Index2013", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddIndex(1, 8212, "30", "Index2014", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddIndex(1, 8213, "30", "Index2015", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Index Count API : Get and compare index count '6' : ";
#endif
	UINT32 indexCount = GetIndexCount(1);
	CPPUNIT_ASSERT(indexCount == 6);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Index Count API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Index Count API : End test." << endl;
#endif
}

void OdApiTest::testGetSubIndexCount()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Get SubIndex Count API : Start test." << endl;
	cout << "TEST: Get SubIndex Count API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get SubIndex Count API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get SubIndex Count API : Add indices 2010 to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get SubIndex Count API : Add sub indices 12 - 16 to project : ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddSubIndex(1, 8208, 13, "30", "SubIndex2010/13");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddSubIndex(1, 8208, 14, "30", "SubIndex2010/14");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddSubIndex(1, 8208, 15, "30", "SubIndex2010/15");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
	retCode = AddSubIndex(1, 8208, 16, "30", "SubIndex2010/16");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get SubIndex Count API : Get and compare sub index count '5' : ";
#endif
	UINT32 indexCount = GetSubIndexCount(1, 8208);
	CPPUNIT_ASSERT(indexCount == 5);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get SubIndex Count API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get SubIndex Count API : End test." << endl;
#endif
}

void OdApiTest::testGetNumberOfEntries()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Get Number Of Entries API : Start test." << endl;
	cout << "TEST: Get Number Of Entries API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Number Of Entries API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Number Of Entries API : Add indices 2010 to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Number Of Entries API : Add subindex 2010/0 with actual value '4' : ";
#endif
	retCode = AddSubIndex(1, 8208, 0, "4", "NrOfEntries");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Number Of Entries API : Get and compare Nr of Entries '4' : ";
#endif
	UINT32 nrOfEntries = GetNumberOfEntries(1, 8208, false);
	CPPUNIT_ASSERT(nrOfEntries == 4);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Number Of Entries API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Get Number Of Entries API : End test." << endl;
#endif
}

void OdApiTest::testDeleteIndex()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Delete Index API : Start test." << endl;
	cout << "TEST: Delete Index API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : Delete not existing index 2010 to provoke error : ";
#endif
	retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INDEXID_NOT_FOUND);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : Add index 2010 to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : Delete index 2010 from project : ";
#endif
	retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : Check if index 2010 was deleted : ";
#endif
	bool existIndex = IsExistingIndex(1, 8208);
	CPPUNIT_ASSERT(existIndex == false);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : Delete not existing index 2010 again to provoke error : ";
#endif
	retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_INDEXID_NOT_FOUND);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete Index API : End test." << endl;
#endif
}

void OdApiTest::testDeleteSubIndex()
{
#ifdef CPPUNIT_DEBUG
	cout << endl << "TEST: Delete SubIndex API : Start test." << endl;
	cout << "TEST: Delete SubIndex API : Create new project : ";
#endif
	retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : Add node to project : ";
#endif
	retCode = AddNode(1, CN, "Node");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : Add indices 2010 to project : ";
#endif
	retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : Delete not existing subindex 2010 to provoke error : ";
#endif
	retCode = DeleteSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUBINDEXID_NOT_FOUND);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : Add sub indix 2010/12 to project : ";
#endif
	retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : Delete subindex 2010 from project : ";
#endif
	retCode = DeleteSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : Check if subindex 2010/12 was deleted : ";
#endif
	bool existIndex = IsExistingSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(existIndex == false);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : Close project to free memory and end test : ";
#endif
	retCode = CloseProject();
	CPPUNIT_ASSERT(retCode.getErrorCode() == OCFM_ERR_SUCCESS);
#ifdef CPPUNIT_DEBUG
	cout << "SUCCESS" << endl;
	cout << "TEST: Delete SubIndex API : End test." << endl;
#endif
}
