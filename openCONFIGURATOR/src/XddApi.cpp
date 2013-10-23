/************************************************************************
\file XddApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/XddApi.h"
#include "../Include/Node.h"
#include "../Include/NodeCollection.h"
#include "../Include/Enums.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/Result.h"
#include "../Include/Logging.h"
#include "../Include/BoostShared.h"

using namespace openCONFIGURATOR::Library::ErrorHandling;

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			DLLEXPORT ocfmRetCode GetDataTypeSize(const string name, UINT32& size)
			{
				size = GetDataSize(name);
				if (size == 0)
					return ocfmRetCode(OCFM_ERR_DATATYPE_NOT_FOUND);
				return ocfmRetCode(OCFM_ERR_SUCCESS);
			}

			DLLEXPORT ocfmRetCode GetFeatureValue(const UINT32 nodeId, const FeatureType featureType, const string featureName, string& featureValue)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					try
					{
						NodeCollection* nodeCollectionPtr = NodeCollection::GetNodeColObjectPointer();
						assert(nodeCollectionPtr);
						Node* node = nodeCollectionPtr->GetNodePtr(nodeId);

						if (!node)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter % nodeId;
							ocfmRetCode result(OCFM_ERR_NODEID_NOT_FOUND);
							result.setErrorString(formatter.str());
							throw result;
						}

						NetworkManagement* nmtPtr = node->GetNetworkManagement();
						featureValue = nmtPtr->GetNwMgmtFeatureValue(featureType, featureName.c_str());
						return ocfmRetCode(OCFM_ERR_SUCCESS);
					} catch (ocfmRetCode& ex)
					{
						LOG_FATAL() << ex.getErrorString();
						return ex;
					}
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

		}
	}
}