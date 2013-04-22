/**
 *****************************************************************************************************
 \file			ProjectSettings.h

 \brief
 *****************************************************************************************************
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
#ifndef ProjectSettings_h
#define ProjectSettings_h

/****************************************************************************************************/
/* Includes */

#include "Declarations.h"

/**
 ******************************************************************************************************
 \class			PjtSettings
 \brief			This handles Powerlink IP settings details	
 
 ******************************************************************************************************/

class PjtSettings
{
	public:
		PjtSettings(void);
		~PjtSettings(void);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to check for and return an existing PjtSettings object. If not available, it will return a new PjtSettings object
		 
		 \return	PjtSettings*
		 */
		/*****************************************************************************/
		static PjtSettings* GetPjtSettingsPtr();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to assign IP address for Powerlink MN
		 
		 \param		ipAddr		Character pointer to the IPaddress string
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetIP(char* ipAddr);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the saveMode attribute in the PjtSettings object
		 
		 \param		autoSaveMode   	Enum variable of AutoSave to hold the saveMode attributes to be set
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetSaveAttr(AutoSave autoSaveMode);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the viewMode attribute in the PjtSettings object
		 
		 \param		viewModeTemp		Enum variable of ViewMode to hold the viewMode attributes to be set
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetViewMode(ViewMode viewModeTemp);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the generateMode attribute in the PjtSettings object
		 
		 \param		autoGenerateMode	 Enum variable of AutoGenerate to hold the generateMode attributes to be set
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetGenerateAttr(AutoGenerate autoGenerateMode);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to set the expertView flag in the PjtSettings object	
		 
		 \param		expertViewTemp	 	Boolean to hold the enable/disable value of expertView flag
		 
		 \return	void
		 */
		/*****************************************************************************/
		void SetExpertViewSelectedFlag(bool expertViewTemp);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the IP address assigned for Powerlink MN
		 
		 \return	const char* or NULL
		 */
		/*****************************************************************************/
		const char* GetIP();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the saveMode attribute in the PjtSettings object
		 
		 \return	AutoSave
		 */
		/*****************************************************************************/
		AutoSave GetSaveAttr();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the viewMode attribute in the PjtSettings object
		 
		 \return	ViewMode
		 */
		/*****************************************************************************/
		ViewMode GetViewMode();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the generateMode attribute in the PjtSettings project
		 
		 \return	AutoGenerate
		 */
		/*****************************************************************************/
		AutoGenerate GetGenerateAttr();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the expertView flag in the PjtSettings object
		 
		 \return	BOOL
		 \retval	TRUE		if expertView is enabled
		 \retval	FALSE		if expertView is disabled	
		 */
		/*****************************************************************************/
		bool GetExpertViewSelectedFlag();

	private:
		static bool instanceFlag;
		static PjtSettings *pjtSettingsObj;
		bool expertView;
		char* ipAddress;
		AutoSave saveMode;
		ViewMode viewMode;
		AutoGenerate generateMode;

};
#endif // ProjectSettings_h
