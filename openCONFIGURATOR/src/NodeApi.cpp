/************************************************************************
\file NodeApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/NodeApi.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/Result.h"
#include "../Include/Logging.h"
#include "../Include/BoostShared.h"

#include <sstream>

using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			/*DLLEXPORT ocfmRetCode AddNode(Node const& node, const string path, const string xddFile);*/
			DLLEXPORT Result AddNode(const UINT32 nodeId, const NodeType nodeType, const string nodeName, const string xddFile)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					if (IsExistingNode(nodeId)) 
					{
						boost::format formatter(kMsgExistingNode);
						formatter % nodeId;
						return Result (NODE_EXISTS, formatter.str());
					}
					return Translate(NewProjectNode(nodeId,
						nodeType,
						nodeName.c_str(),
						(xddFile.empty())
							? NULL
							: xddFile.c_str()));
				}
				return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED));
			}


			DLLEXPORT Result DeleteNode(const UINT32 nodeId)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					NodeType type = (nodeId == MN_NODEID)
						? MN
						: CN;
					return Translate(DeleteNode(nodeId, type));
				}
				return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED));
			}

			/*DLLEXPORT ocfmRetCode GetNode(const unsigned int nodeId, Node& node);*/

			DLLEXPORT Result ReplaceXdd(const UINT32 nodeId, const string path, const string xddFile)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					NodeType type = (nodeId == MN_NODEID) ? MN : CN;
					boost::filesystem::path fullPath(path);
					fullPath.append(xddFile.begin(), xddFile.end());

					Result retValue = Translate(ValidateXDDFile(fullPath.generic_string().c_str()));
					if (retValue.IsSuccessful())
						return Translate(ImportXML(fullPath.generic_string().c_str(), nodeId, type));
					else
						return retValue;
				}
				return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED));
			}

			DLLEXPORT bool IsExistingNode(const UINT32 nodeId)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
					return NodeCollection::GetNodeColObjectPointer()->ContainsNode(nodeId);
				return false;
			}

			DLLEXPORT UINT32 GetNodeCount()
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
					return NodeCollection::GetNodeColObjectPointer()->GetNumberOfNodes();
				return 0;
			}

		}
	}
}