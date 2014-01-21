/************************************************************************
\file XddApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/XddApi.h"
#include "../Include/Node.h"
#include "../Include/NodeApi.h"
#include "../Include/NodeCollection.h"
#include "../Include/Enums.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/Result.h"
#include "../Include/Logging.h"
#include "../Include/BoostShared.h"

using namespace openCONFIGURATOR::Library::API;
using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::Library::ObjectDictionary;

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace API
		{

			DLLEXPORT Result GetDataTypeSize(const PlkDataType::PlkDataType type, UINT32& size)
			{
				try
				{
					size = 0;
					switch (type)
					{
						case PlkDataType::BOOLEAN:
						case PlkDataType::INTEGER8:
						case PlkDataType::UNSIGNED8:
							size = 1;
							break;
						case PlkDataType::INTEGER16:
						case PlkDataType::UNSIGNED16:
							size = 2;
							break;
						case PlkDataType::INTEGER24:
						case PlkDataType::UNSIGNED24:
							size = 3;
							break;
						case PlkDataType::INTEGER32:
						case PlkDataType::UNSIGNED32:
						case PlkDataType::REAL32:
						case PlkDataType::IP_ADDRESS:
							size = 4;
							break;
						case PlkDataType::INTEGER40:
						case PlkDataType::UNSIGNED40:
							size = 5;
							break;
						case PlkDataType::INTEGER48:
						case PlkDataType::UNSIGNED48:
						case PlkDataType::MAC_ADDRESS:
						case PlkDataType::TIME_DIFF:
						case PlkDataType::TIME_OF_DAY:
							size = 6;
							break;
						case PlkDataType::INTEGER56:
						case PlkDataType::UNSIGNED56:
							size = 7;
							break;
						case PlkDataType::INTEGER64:
						case PlkDataType::UNSIGNED64:
						case PlkDataType::REAL64:
						case PlkDataType::NETTIME:
							size = 8;
							break;
						default:
							boost::format formatter(kMsgPowerlinkDatatypeSizeNotDefined);
							formatter % type;
							return Result(PLKDATATYPE_SIZE_UNDEFINED, formatter.str());
					}
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

			DLLEXPORT Result GetFeatureValue(const UINT32 nodeId, const PlkFeature::PlkFeature feature, string& featureValue)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						bool nodeExists = false;
						Result res = IsExistingNode(nodeId, nodeExists);
						if (res.IsSuccessful() && !nodeExists)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter % nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}

						Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
						FeatureType featureType = GENERAL_FEATURES;
						if(feature >= 82)
							featureType = CN_FEATURES;
						else if(feature >= 64)
							featureType = MN_FEATURES;

						NetworkManagement* nmtPtr = node->GetNetworkManagement();
						featureValue = nmtPtr->GetNwMgmtFeatureValue(featureType, GetPlkFeature(feature).c_str());

						//use default value if existing
						if (featureValue.empty())
						{
							featureValue.append(GetPlkFeatureDefaultValue(feature));
						}

						if (featureValue.empty())
						{
							boost::format formatter(kMsgFeatureValueNotFound);
							string featureTypeStr = (featureType == MN_FEATURES)
							                        ? "MN"
							                        : ((featureType == CN_FEATURES) ? "CN" : "General");
							formatter % featureTypeStr
							% GetPlkFeature(feature)
							% nodeId;
							return Result(FEATURE_VALUE_NOT_FOUND, formatter.str());
						}
						return Result();

					}
					return Result(NO_PROJECT_LOADED, kMsgNoProjectLoaded);
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
