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
				try
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
					return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED));
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
				return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED));
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

			DLLEXPORT bool IsExistingNode(const UINT32 nodeId)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
						return NodeCollection::GetNodeColObjectPointer()->ContainsNode(nodeId);
					return false;
				}
				catch (const ocfmRetCode& ex)
				{
					return false;
				}
				catch (const std::exception& ex)
				{
					return false;
				}
			}
			//FIXME: Pass value by reference to handle occuring errors and return Result object 
			DLLEXPORT UINT32 GetNodeCount()
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
						return NodeCollection::GetNodeColObjectPointer()->GetNumberOfNodes();
					return 0;
				}
				catch (const ocfmRetCode& ex)
				{
					return 0;
					//return Translate(ex);
				}
				catch (const std::exception& ex)
				{
					return 0;
					//return Result(UNHANDLED_EXCEPTION, ex.what());
	}
			}
		}
	}
}