/*******************************************************************************
*
* License Agreement
*
* Copyright � 2013 BERNECKER + RAINER, AUSTRIA, 5142 EGGELSBERG, B&R STRASSE 1
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

\file       TestTemplate.cpp

\brief      Implements the Test Template for the openCONFIGURATOR unit tests

\author     Christoph Ruecker

\date       11.06.2013

\since      10.06.2013

\version    1.0

\note       openCONFIGURATOR

*******************************************************************************/
#include "../include/TestTemplate.h"
#include <libxml/xmlreader.h>
#include <libxml/parser.h>
#include <libxml/xmlschemas.h>

CPPUNIT_TEST_SUITE_REGISTRATION(TestTemplate);

TestTemplate::TestTemplate(void)
{
}

TestTemplate::~TestTemplate(void)
{
}

void TestTemplate::setUp() {}

void TestTemplate::tearDown() {}

void TestTemplate::executeTest(void)
{
	int x = 5;
	CPPUNIT_ASSERT(x == 5);
}