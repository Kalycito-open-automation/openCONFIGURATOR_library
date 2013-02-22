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

bool CPjtSettings::instanceFlag = false;
CPjtSettings* CPjtSettings::objPjtSettings = NULL;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 
 @param void
 */

CPjtSettings::CPjtSettings(void)
{
#if defined DEBUG
	cout << "\nPjtSettings Objects Initialized!\n" << endl;
#endif
	m_saveMode = DISCARD_AS;
	m_generateMode = NO_AG;
	m_IP_openPOWERLINK = NULL;
	m_bExpertViewSelected = false;
	m_viewMode = SIMPLE; //TODO: Review initialisation
}

/*************************************************************************/
/* Destructor */

/**
 
 @param void
 */

CPjtSettings::~CPjtSettings(void)
{
	instanceFlag = false;
#if defined DEBUG
	cout << "\n\n\n\nPjtSettings Object Deleted!\n" << endl;
#endif
}

/*****************************************************************************/
/**
 \brief			getPjtSettingsPtr
 
 This is a member function of CPjtSettings creats new Pjtsettings 

 \return		CPjtSettings*
 */
/*****************************************************************************/

CPjtSettings* CPjtSettings::getPjtSettingsPtr()
{
	if (!instanceFlag)
	{
		objPjtSettings = new CPjtSettings();
		instanceFlag = true;
#if defined DEBUG
		cout << "\n\n\n\nNew PjtSettings Object Created!\n" << endl;
#endif
	}
	return objPjtSettings;
}

/*****************************************************************************/
/**
 \brief			getSaveAttr
 
 This is a member function of CPjtSettings saves new Pjtsettings  

 \return		EAutoSave
 */
/*****************************************************************************/

EAutoSave CPjtSettings::getSaveAttr()
{
	return m_saveMode;
}

/*****************************************************************************/
/**
 \brief			setSaveAttr
 
 This is a member function of CPjtSettings sets mode for pjt settings 
 
 \param			enumAutoSaveMode   Enum Variable of EAutoSave to save the attributes
 \return		void
 */
/*****************************************************************************/

void CPjtSettings::setSaveAttr(EAutoSave enumAutoSaveMode)
{
	m_saveMode = enumAutoSaveMode;
}

/*****************************************************************************/
/**
 \brief			getGenerateAttr
 
 This is a member function of CPjtSettings generates mode for pjt settings    

 \return		EAutoGenerate
 */
/*****************************************************************************/

EAutoGenerate CPjtSettings::getGenerateAttr()
{
#if defined DEBUG
	//cout << "\n Returning m_generateMode:-" << m_generateMode <<endl;	
#endif	
	return m_generateMode;
}

/*****************************************************************************/
/**
 \brief			setGenerateAttr
 
 This is a member function of CPjtSettings sets mode for pjt settings  
 
 \param			enumAutoGenerateMode Enum Variable of EAutoGenerate to generate attribute
 \return		void
 */
/*****************************************************************************/

void CPjtSettings::setGenerateAttr(EAutoGenerate enumAutoGenerateMode)
{
	m_generateMode = enumAutoGenerateMode;
}

/*****************************************************************************/
/**
 \brief			setPOWERLINK_IP
 
 This is a member function of CPjtSettings assigns IP address for Powerlink  
 
 \param			pbIPAddr   Character Pointer variable to hold IPaddress
 \return		void
 */
/*****************************************************************************/

void CPjtSettings::setPOWERLINK_IP(char* pbIPAddr)
{
//TODO: review
	if (NULL != m_IP_openPOWERLINK)
	{
		delete[] m_IP_openPOWERLINK;
	}

	m_IP_openPOWERLINK = new char[strlen(pbIPAddr) + STR_ALLOC_BUFFER];
	strcpy((char*) m_IP_openPOWERLINK, pbIPAddr);

}

/*****************************************************************************/
/**
 \brief			getPOWERLINK_IP
 
 This is a member function of CPjtSettings returns Powerlink IP  
 
 \return		char*
 */
/*****************************************************************************/

//TODO: Unused Function
const char* CPjtSettings::getPOWERLINK_IP()
{
	if (NULL != m_IP_openPOWERLINK)
	{
		return m_IP_openPOWERLINK;
	}
	else
	{
		return NULL;
	}
}

/*****************************************************************************/
/**
 \brief			getViewMode
 
 This is a member function of CPjtSettings sets view mode under pjt settings  
 
 \return		EViewMode
 */
/*****************************************************************************/

EViewMode CPjtSettings::getViewMode()
{
	return m_viewMode;
}

/*****************************************************************************/
/**
 \brief			setViewMode
 
 This is a member function of CPjtSettings assigns view mode  
 
 \param			enumViewMode	Enum Variable of EViewMode to hold the value of mode type			
 \return		void
 */
/*****************************************************************************/

void CPjtSettings::setViewMode(EViewMode enumViewMode)
{
	m_viewMode = enumViewMode;
}

/*****************************************************************************/
/**
 \brief			getExpertViewSelectedFlag
 
 This is a member function of CPjtSettings sets flag for type of view selected under pjt settings  
 
 \return		BOOL
 \retval			TRUE			if successful
 \retval			FALSE			if there is already a message pending	
 */
/*****************************************************************************/

bool CPjtSettings::getExpertViewSelectedFlag()
{
	return m_bExpertViewSelected;
}

/*****************************************************************************/
/**
 \brief			setExpertViewSelectedFlag
 
 This is a member function of CPjtSettings assigns type of view selected under pjt settings 
 
 \param			bExpertViewSelected	 Boolean Variable to hold the value of Expertview selected			
 \return		void
 */
/*****************************************************************************/

void CPjtSettings::setExpertViewSelectedFlag(bool bExpertViewSelected)
{
	m_bExpertViewSelected = bExpertViewSelected;
}

