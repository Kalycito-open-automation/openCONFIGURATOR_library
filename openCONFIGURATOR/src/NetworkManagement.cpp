/**
 ******************************************************************************
 \file		NetworkManagement.cpp

 \brief		This file contains class member definitions to handle the network management features of a Node
 ******************************************************************************
 */

/*
 �) Kalycito Infotech Private Limited

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

#include "../Include/NetworkManagement.h"
#include "../Include/Internal.h"
#include "../Include/Logging.h"
#include "../Include/BoostShared.h"

#include <string>

using namespace std;

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**


 */

NetworkManagement::NetworkManagement(void) :
	FeatureCollection(),
	maxPDOCount(0)
{}

/*************************************************************************/
/* Destructor */

/**


 */

NetworkManagement::~NetworkManagement(void)
{
	//Add destructor code here
}

void NetworkManagement::AddFeature(Feature objFeature)
{
	FeatureCollection.push_back(objFeature);
}

char* NetworkManagement::GetNwMgmtFeatureValue(FeatureType featureType,
        const char* featureName)
{
	UINT32 loopCount = 0;
	char* retString = NULL;
	Feature objFeature;

	for (loopCount = 0; loopCount < FeatureCollection.size(); loopCount++)
	{
		objFeature = FeatureCollection[loopCount];
		if (objFeature.featureType == featureType
		        && (!strcmp(featureName, objFeature.name)))
		{
			retString = new char[strlen(objFeature.value) + STR_ALLOC_BUFFER];
			strcpy(retString, objFeature.value);
			return retString;
		}
	}
	retString = new char[1 + STR_ALLOC_BUFFER];
	strcpy((char*) retString, "");
	return retString;
}

UINT32 NetworkManagement::GetNumberOfFeatures()
{
	return FeatureCollection.size();
}

Feature* NetworkManagement::GetFeature(UINT32 featurePosition)
{
	return &FeatureCollection[featurePosition];
}

void NetworkManagement::DeleteFeatureCollections()
{
	FeatureCollection.clear();

}

INT32 NetworkManagement::GetMaxPDOCount()
{
	return maxPDOCount;
}

void NetworkManagement::CalculateMaxPDOCount()
{
	string featureName = "PDOTPDOChannels";
	string featureValue = GetNwMgmtFeatureValue(MN_FEATURES, featureName.c_str());

	// PDOTPDOChannels is optional in an XDD. If it does not exist, the above function
	// will return an empty string.
	// The previous version of this function used atoi() to convert featureValue into maxPDOCount,
	// atoi() returns value 0 for empty strings.
	// To avoid side-effects, this behaviour will be kept, although PDOTPDOChannels has a default value
	// of 256 if it is not present in an XDD.

	this->maxPDOCount = (featureValue.empty())
	                    ? 0
	                    : boost::lexical_cast<INT32>(featureValue);
	//check is made for validating the value in MN xdd
	//Min value = 0; Maxvalue = 256 (EPSG specification)
	if (this->maxPDOCount > 256)
	{
		ocfmRetCode ex(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
		LOG_FATAL() << ex.getErrorString();
		throw ex;
	}
}
