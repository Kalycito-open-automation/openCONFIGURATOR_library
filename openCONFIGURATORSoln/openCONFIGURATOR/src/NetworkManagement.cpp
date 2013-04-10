/**
 ************************************************************************************************
 \file			NetworkManagement.cpp

 \brief			Handles information of each object node present in the network
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

#include "../Include/NetworkManagement.h"
#include "../Include/Internal.h"

//==========================================================================//
// 				F U N C T I O N  D E F I N I T I O N S  					//
//==========================================================================//

/*************************************************************************/
/* Constructor */

/**
 

 */

NetworkManagement::NetworkManagement(void)
{
	maxPDOCount = 0;
}

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
	INT32 itemPosition = FeatureCollection.Add();
	FeatureCollection[itemPosition] = objFeature;
}

char* NetworkManagement::GetNwMgmtFeatureValue(FeatureType featureType,
		char* featureName)
{
	INT32 loopCount = 0;
	char* retString = NULL;
	Feature objFeature;

	for (loopCount = 0; loopCount < FeatureCollection.Count(); loopCount++)
	{
		objFeature = FeatureCollection[loopCount];
		if (objFeature.featureType == featureType
				&& (!strcmp(featureName, objFeature.name)))
		{
			retString =
					new char[strlen(objFeature.value) + STR_ALLOC_BUFFER];
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
	return FeatureCollection.Count();
}

Feature* NetworkManagement::GetFeature(UINT32 featurePosition)
{
	return &FeatureCollection[featurePosition];
}


void NetworkManagement::DeleteFeatureCollections()
{
	if (0 != FeatureCollection.Count())
	{
		FeatureCollection.Clear();
	}
}


INT32 NetworkManagement::GetMaxPDOCount()
{
	return maxPDOCount;
}


void NetworkManagement::CalculateMaxPDOCount()
{
	char* tpdoChannelValue = new char[5];
	char* featureName = new char[20];
	maxPDOCount = 0;
	if ((NULL == tpdoChannelValue) || (NULL == featureName))
	{
#if defined DEBUG
		cout << "Memory allocation error" << __FUNCTION__ << endl;
#endif

		ocfmException ex;
		ex.OCFMException(OCFM_ERR_MEMORY_ALLOCATION_ERROR);
		throw ex;
	}
	else
	{
		strcpy(featureName, "PDOTPDOChannels");
		strcpy(tpdoChannelValue, GetNwMgmtFeatureValue(MN_FEATURES, featureName));
		maxPDOCount = atoi((char*) tpdoChannelValue);
		//check is made for validating the value in MN xdd
		//Min value = 0; Maxvalue = 256 (EPSG specification)
		if (maxPDOCount > 256)
		{
			ocfmException ex;
			ex.OCFMException(OCFM_ERR_EXCEEDS_MAX_TPDO_CHANNELS);
			throw ex;
		}
	}
	delete[] tpdoChannelValue;
	delete[] featureName;
}
