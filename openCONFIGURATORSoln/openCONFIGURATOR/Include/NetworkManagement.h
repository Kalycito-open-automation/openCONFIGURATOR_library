/**
 *****************************************************************************************************
 \file			NetworkManagement.h

 \brief			Handles Feature collection of each node present in the network
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
#ifndef NetworkManagement_h
#define NetworkManagement_h

/****************************************************************************************************/
/* Includes */

#include <string>
#include "ComplexDataType.h"
#include "TCollection.h"

using namespace std;

/**
 ******************************************************************************************************
 \class			NetworkManagement
 \brief			This class handles Feature collection of each node present in the network
 
 ******************************************************************************************************/
class NetworkManagement
{
	public:
		NetworkManagement(void);
		~NetworkManagement(void);

		/*****************************************************************************/
		/**
		 \brief		This function shall be used to calculate the maximum PDO count value from the PDOTPDOChannels parameter in MN xdd and validate if it exceeds the maximum value 256 as in Powerlink Specification Document
		 
		 \return	void
		 */
		/*****************************************************************************/
		void CalculateMaxPDOCount();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to delete all features from the collection list 
		 
		 \return	void
		 */
		/*****************************************************************************/
		void DeleteFeatureCollections();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to add the given Feature object to the collection list
		 
		 \param		objFeature		Structure Variable of Feature to add a feature to collection list
		 
		 \return	void
		 */
		/*****************************************************************************/
		void AddFeature(Feature objFeature);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return max PDO count

		 \return	INT32
		 */
		/*****************************************************************************/
		INT32 GetMaxPDOCount();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the number of features in the collection list
		 
		 \return	UINT32
		 */
		/*****************************************************************************/
		UINT32 GetNumberOfFeatures();
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the Feature object at the given position in the collection list
		 
		 \param		featurePosition  	Unsigned integer to hold count value
		 
		 \return	Feature*
		 */
		/*****************************************************************************/
		Feature* GetFeature(UINT32 featurePosition);
		/*****************************************************************************/
		/**
		 \brief		This function shall be used to return the value of the Feature object in the collection list whose type and name match the given parameters
		 
		 \param		featureType		Enum variable of FeatureType to hold the feature type
		 \param		featureName		Character pointer to the feature name

		 \return	char*
		 */
		/*****************************************************************************/
		char* GetNwMgmtFeatureValue(FeatureType featureType, char* featureName);

		TCollection<Feature> FeatureCollection;

	private:
		INT32 maxPDOCount;
};

#endif // NetworkManagement_h
