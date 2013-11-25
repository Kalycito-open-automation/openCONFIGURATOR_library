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

			DLLEXPORT Result GetDataTypeSize(const string name, UINT32& size)
			{
				try
				{
					size = GetDataSize(name);
					if (size == 0)
						return Translate(ocfmRetCode(OCFM_ERR_DATATYPE_NOT_FOUND));//FIXME: Change when correct new error code is implemented
					return Result();
				}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const std::exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}

			DLLEXPORT Result GetFeatureValue(const UINT32 nodeId, const FeatureType featureType, const string featureName, string& featureValue)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						NodeCollection* nodeCollectionPtr = NodeCollection::GetNodeColObjectPointer();
						assert(nodeCollectionPtr);
						Node* node = nodeCollectionPtr->GetNodePtr(nodeId);

						if (!node)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter % nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}

						NetworkManagement* nmtPtr = node->GetNetworkManagement();
						featureValue = nmtPtr->GetNwMgmtFeatureValue(featureType, featureName.c_str());
						return Result();

					}
					return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED)); //FIXME: Change when correct new error code is implemented
				}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const std::exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}
		}
	}
}