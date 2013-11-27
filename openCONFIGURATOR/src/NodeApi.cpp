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
			const string kDefaultCNXDD = "./resources/openPOWERLINK_CN.xdd";

			DLLEXPORT Result AddNode(const UINT32 nodeId, const string nodeName, const string xddFile)
			{
				try
				{
					bool exists = false;
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						NodeType type = (nodeId == MN_NODEID)
							? MN
							: CN;
						Result res = IsExistingNode(nodeId, exists);
						if (!res.IsSuccessful())
							return res;
						if (exists) 
						{
							boost::format formatter(kMsgExistingNode);
							formatter % nodeId;
							return Result(NODE_EXISTS, formatter.str());
						}
						return Translate(NewProjectNode(nodeId,
							type,
							nodeName.c_str(),
							(xddFile.empty())
								? kDefaultCNXDD.c_str()
								: xddFile.c_str()));
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

			DLLEXPORT Result DeleteNode(const UINT32 nodeId)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						NodeType type = (nodeId == MN_NODEID)
							? MN
							: CN;
						return Translate(DeleteNode(nodeId, type));
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

			/*DLLEXPORT ocfmRetCode GetNode(const unsigned int nodeId, Node& node);*/

			DLLEXPORT Result ReplaceXdd(const UINT32 nodeId, const string path, const string xddFile)
			{
				try
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

			DLLEXPORT Result IsExistingNode(const UINT32 nodeId, bool& exists)
			{
				try
				{
					exists = false;
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						exists = NodeCollection::GetNodeColObjectPointer()->ContainsNode(nodeId); 
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

			DLLEXPORT Result GetNodeCount(UINT32& nodeCount)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						nodeCount = NodeCollection::GetNodeColObjectPointer()->GetNumberOfNodes();
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