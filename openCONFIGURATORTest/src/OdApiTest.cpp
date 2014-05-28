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

using namespace openCONFIGURATOR::Library::API;


CPPUNIT_TEST_SUITE_REGISTRATION(OdApiTest);

OdApiTest::OdApiTest(void) {}

OdApiTest::~OdApiTest(void) {}

void OdApiTest::setUp() {}

void OdApiTest::tearDown()
{
	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
}

void OdApiTest::testAddIndex()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == INDEX_INVALID);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == INDEX_EXISTS);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testSetIndexAttribute()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	string getNameString;
	this->retCode = GetIndexAttribute(1, 8208, NAME, getNameString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getNameString.compare("Index2010") == 0);

	this->retCode = SetIndexAttribute(1, 8208, NAME, "Index2010New");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string setNameString;
	this->retCode = GetIndexAttribute(1, 8208, NAME, setNameString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(setNameString.compare("Index2010New") == 0);

	this->retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "DEFTYPE");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getObjecttypeString;
	this->retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, getObjecttypeString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getObjecttypeString.compare("DEFTYPE") == 0);

	this->retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "DEFSTRUCT");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string setObjecttypeString;
	this->retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(setObjecttypeString.compare("DEFSTRUCT") == 0);
	setObjecttypeString.clear();

	this->retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "VAR");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(setObjecttypeString.compare("VAR") == 0);
	setObjecttypeString.clear();

	//this->retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "ARRAY");
	//cout << this->retCode.getError() << endl;
	//CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	//this->retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	//CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	//CPPUNIT_ASSERT(setObjecttypeString.compare("ARRAY") == 0);
	//setObjecttypeString.clear();

	//this->retCode = SetIndexAttribute(1, 8208, OBJECTTYPE, "RECORD");
	//cout << this->retCode.getErrorCode() <<endl;
	//CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	//this->retCode = GetIndexAttribute(1, 8208, OBJECTTYPE, setObjecttypeString);
	//CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	//CPPUNIT_ASSERT(setObjecttypeString.compare("RECORD") == 0);


	this->retCode = SetIndexAttribute(1, 8208, ACCESSTYPE, "ro");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getAccesstypeString;
	this->retCode = GetIndexAttribute(1, 8208, ACCESSTYPE, getAccesstypeString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getAccesstypeString.compare("ro") == 0);


	this->retCode = SetIndexAttribute(1, 8208, HIGHLIMIT, "10");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getHighLimitString;
	this->retCode = GetIndexAttribute(1, 8208, HIGHLIMIT, getHighLimitString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getHighLimitString.compare("10") == 0);

	this->retCode = SetIndexAttribute(1, 8208, LOWLIMIT, "5");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getLowLimitString;
	this->retCode = GetIndexAttribute(1, 8208, LOWLIMIT, getLowLimitString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getLowLimitString.compare("5") == 0);

	this->retCode = SetIndexAttribute(1, 8208, LOWLIMIT, "11");
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == OBJECT_LIMITS_INVALID);


	this->retCode = SetIndexAttribute(1, 8208, HIGHLIMIT, "4");
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == OBJECT_LIMITS_INVALID);

	this->retCode = SetIndexAttribute(1, 8208, DEFAULTVALUE, "7");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getDefaultValueString;
	this->retCode = GetIndexAttribute(1, 8208, DEFAULTVALUE, getDefaultValueString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getDefaultValueString.compare("7") == 0);

	this->retCode = SetIndexAttribute(1, 8208, ACTUALVALUE, "8");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getActualValueString;
	this->retCode = GetIndexAttribute(1, 8208, ACTUALVALUE, getActualValueString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getActualValueString.compare("8") == 0);

	this->retCode = SetIndexAttribute(1, 8208, ACTUALVALUE, "0x16");
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == VALUE_NOT_WITHIN_RANGE);

	string getPDOMappingValue;
	this->retCode = GetIndexAttribute(1, 8208, PDOMAPPING, getPDOMappingValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getPDOMappingValue.compare("NOT_DEFINED") == 0);

	this->retCode = SetIndexAttribute(1, 8208, PDOMAPPING, "optional");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetIndexAttribute(1, 8208, PDOMAPPING, getPDOMappingValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getPDOMappingValue.compare("OPTIONAL") == 0);


	string getFlagincdcString;
	this->retCode = GetIndexAttribute(1, 8208, FORCETOCDC, getFlagincdcString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getFlagincdcString.compare("0") == 0);

	this->retCode = SetIndexAttribute(1, 8208, FORCETOCDC, "1");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string setFlagincdcString;
	this->retCode = GetIndexAttribute(1, 8208, FORCETOCDC, setFlagincdcString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(setFlagincdcString.compare("1") == 0);
	setFlagincdcString.clear();

	this->retCode = SetIndexAttribute(1, 8208, FORCETOCDC, "false");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetIndexAttribute(1, 8208, FORCETOCDC, setFlagincdcString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(setFlagincdcString.compare("0") == 0);

	this->retCode = SetIndexAttribute(1, 8208, FORCETOCDC, "ttrue");
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == ATTRIBUTEVALUE_INVALID);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testSetIndexActualValue()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = SetIndexAttribute(1, 8208, HIGHLIMIT, "10");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	//Check Low Limit
	this->retCode = SetIndexAttribute(1, 8208, LOWLIMIT, "5");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = SetIndexActualValue(1, 8208, "8");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getActualValueString;
	this->retCode = GetIndexAttribute(1, 8208, ACTUALVALUE, getActualValueString);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getActualValueString.compare("8") == 0);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testGetIndexAttribute()
{


}

void OdApiTest::testAddSubIndex()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == INDEX_DOES_NOT_EXIST);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == SUBINDEX_EXISTS);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testSetSubIndexAttribute()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 12, "0x12", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getSubIndexValue;
	this->retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("0x12") == 0);

	this->retCode = SetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, "0x24");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("0x24") == 0);

	this->retCode = GetSubIndexAttribute(1, 8208, 12, NAME, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("SubIndex2010/12") == 0);

	this->retCode = GetSubIndexAttribute(1, 8208, 12, PDOMAPPING, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("NOT_DEFINED") == 0);

	this->retCode = GetSubIndexAttribute(1, 8208, 12, DEFAULTVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("") == 0);

	this->retCode = GetSubIndexAttribute(1, 8208, 12, FORCETOCDC, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("0") == 0);

	//TODO extend this test with more set / get operations

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testSetSubIndexActualValue()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 12, "10000", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	string getSubIndexValue;
	this->retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("10000") == 0);

	this->retCode = SetSubIndexActualValue(1, 8208, 12, "15000");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = GetSubIndexAttribute(1, 8208, 12, ACTUALVALUE, getSubIndexValue);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	CPPUNIT_ASSERT(getSubIndexValue.compare("15000") == 0);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testGetSubIndexAttribute()
{

}

void OdApiTest::testIsExistingIndex()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	bool exists = false;
	this->retCode = IsExistingIndex(1, 8208, exists);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true && exists);

	this->retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	retCode = IsExistingIndex(1, 8208, exists);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() && exists == false);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testIsExistingSubIndex()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	bool exists = false;
	this->retCode = IsExistingSubIndex(1, 8208, 12, exists);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true && exists);

	this->retCode = DeleteSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = IsExistingSubIndex(1, 8208, 12, exists);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() && exists == false);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testGetIndexCount()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddIndex(1, 8209, "30", "Index2011", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddIndex(1, 8210, "30", "Index2012", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddIndex(1, 8211, "30", "Index2013", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddIndex(1, 8212, "30", "Index2014", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddIndex(1, 8213, "30", "Index2015", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	UINT32 indexCount = 0;
	this->retCode = GetIndexCount(1, indexCount);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true && indexCount == 38);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testGetSubIndexCount()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddSubIndex(1, 8208, 13, "30", "SubIndex2010/13");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddSubIndex(1, 8208, 14, "30", "SubIndex2010/14");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddSubIndex(1, 8208, 15, "30", "SubIndex2010/15");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);
	this->retCode = AddSubIndex(1, 8208, 16, "30", "SubIndex2010/16");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	UINT32 indexCount = 0;
	this->retCode = GetSubIndexCount(1, 8208, indexCount);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true && indexCount == 5);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testGetNumberOfEntries()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddSubIndex(1, 8208, 0, "4", "NrOfEntries");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	UINT32 nrOfEntries = 0;
	this->retCode = GetNumberOfEntries(1, 8208, false, nrOfEntries);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true && nrOfEntries == 4);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testDeleteIndex()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == INDEX_DOES_NOT_EXIST);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	bool exists = false;
	this->retCode = IsExistingIndex(1, 8208, exists);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() && exists == false);

	this->retCode = DeleteIndex(1, 8208);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == INDEX_DOES_NOT_EXIST);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}

void OdApiTest::testDeleteSubIndex()
{

	this->retCode = NewProject("TestProject", ".");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddNode(1, "Node");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = AddIndex(1, 8208, "30", "Index2010", VAR);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = DeleteSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(this->retCode.GetErrorCode() == INDEX_CONTAINS_NO_SUBINDICES);

	this->retCode = AddSubIndex(1, 8208, 12, "30", "SubIndex2010/12");
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	this->retCode = DeleteSubIndex(1, 8208, 12);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

	bool exists = false;
	this->retCode = IsExistingSubIndex(1, 8208, 12, exists);
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() && exists == false);

	this->retCode = CloseProject();
	CPPUNIT_ASSERT(this->retCode.IsSuccessful() == true);

}
