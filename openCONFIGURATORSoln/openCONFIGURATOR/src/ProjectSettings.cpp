/**
 ************************************************************************************************
 \file			ProjectSettings.cpp

 \brief			Handles project settings such as view mode,new settings,generate,ip address.
 ************************************************************************************************
 */

/*
 (c) Kalycito Infotech Private Limited

 License:

 Redistribution and use in source and binary forms, with or without
 modification, are permitted provided that the following conditions
 are met:

 1. Redistributions of source code must retain the above copyright
 notice, this list of conditions and the following disclaimer.

 2. Redistributions in binary form must reproduce the above copyright
 notice, this list of conditions and the following disclaimer in the
 documentation and/or other materials provided with the distribution.

 3. Neither the name of Kalycito Infotech Private Limited nor the names of 
 its contributors may be used to endorse or promote products derived
 from this software without prior written permission. For written
 permission, please contact info@kalycito.com.

 THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS
 FOR A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE
 COPYRIGHT HOLDERS OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT,
 INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING,
 BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
 LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN
 ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.

 Severability Clause:

 If a provision of this License is or becomes illegal, invalid or
 unenforceable in any jurisdiction, that shall not affect:
 1. the validity or enforceability in that jurisdiction of any other
 provision of this License; or
 2. the validity or enforceability in other jurisdictions of that or
 any other provision of this License.

 ****************************************************************************/

/****************************************************************************************************/
/* Includes */

#include <iostream>
#include "../Include/ProjectSettings.h"
#include "../Include/Internal.h"

using namespace std;

/****************************************************************************************************/
/* Global Variables */

bool PjtSettings::instanceFlag = false;
PjtSettings* PjtSettings::pjtSettingsObj = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

PjtSettings::PjtSettings(void)
{
#if defined DEBUG
	cout << "\nPjtSettings Objects Initialized!\n" << endl;
#endif
	saveMode = DISCARD_AS;
	generateMode = NO_AG;
	ipAddress = NULL;
	expertView = false;
	viewMode = SIMPLE; //TODO: Review initialisation
}

/*************************************************************************/
/* Destructor */

/**
 

 */

PjtSettings::~PjtSettings(void)
{
	instanceFlag = false;
#if defined DEBUG
	cout << "\n\n\n\nPjtSettings Object Deleted!\n" << endl;
#endif
}


PjtSettings* PjtSettings::GetPjtSettingsPtr()
{
	if (!instanceFlag)
	{
		pjtSettingsObj = new PjtSettings();
		instanceFlag = true;
#if defined DEBUG
		cout << "\n\n\nNew PjtSettings Object Created!\n" << endl;
#endif
	}
	return pjtSettingsObj;
}


AutoSave PjtSettings::GetSaveAttr()
{
	return saveMode;
}

void PjtSettings::SetSaveAttr(AutoSave autoSaveMode)
{
	saveMode = autoSaveMode;
}


AutoGenerate PjtSettings::GetGenerateAttr()
{
#if defined DEBUG
	//cout << "\n Returning AutoGenerate mode:-" << generateMode <<endl;
#endif	
	return generateMode;
}

void PjtSettings::SetGenerateAttr(AutoGenerate autoGenerateMode)
{
	generateMode = autoGenerateMode;
}

void PjtSettings::SetIP(char* ipAddr)
{
//TODO: review
	if (NULL != ipAddress)
	{
		delete[] ipAddress;
	}

	ipAddress = new char[strlen(ipAddr) + STR_ALLOC_BUFFER];
	strcpy((char*) ipAddress, ipAddr);

}

//TODO: Unused Function
const char* PjtSettings::GetIP()
{
	if (NULL != ipAddress)
	{
		return ipAddress;
	}
	else
	{
		return NULL;
	}
}


ViewMode PjtSettings::GetViewMode()
{
	return viewMode;
}

void PjtSettings::SetViewMode(ViewMode viewModeTemp)
{
	viewMode = viewModeTemp;
}


bool PjtSettings::GetExpertViewSelectedFlag()
{
	return expertView;
}

void PjtSettings::SetExpertViewSelectedFlag(bool expertViewTemp)
{
	expertView = expertViewTemp;
}

