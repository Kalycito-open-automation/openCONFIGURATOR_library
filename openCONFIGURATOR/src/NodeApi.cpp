#include "../Include/NodeApi.h"
#include "../Include/ProjectConfiguration.h"
#include <sstream>

using namespace std;

/*DLLEXPORT ocfmRetCode AddNode(Node const& node, const string path, const string xddFile);*/
DLLEXPORT ocfmRetCode AddNode(const UINT32 nodeId, const NodeType nodeType, const string nodeName, const string xddFile)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		if (IsExistingNode(nodeId))
		{
			return ocfmRetCode(OCFM_ERR_NODE_ALREADY_EXISTS);
		}
		return NewProjectNode(nodeId,
		                      nodeType,
		                      nodeName.c_str(),
		                      (xddFile.empty())
		                      ? NULL
		                      : xddFile.c_str());
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}


DLLEXPORT ocfmRetCode DeleteNode(const UINT32 nodeId)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		NodeType type = (nodeId == 240)
		                ? MN
		                : CN;
		return DeleteNode(nodeId, type);
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

/*DLLEXPORT ocfmRetCode GetNode(const unsigned int nodeId, Node& node);*/

DLLEXPORT ocfmRetCode ReplaceXdd(const UINT32 nodeId, const string path, const string xddFile)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		NodeType type = (nodeId == 240) ? MN : CN;
		ostringstream xddFilePath;
		xddFilePath << path;
		xddFilePath << xddFile;

		ocfmRetCode retValue = ValidateXDDFile(xddFilePath.str().c_str());
		if (retValue.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			retValue = ImportXML(xddFilePath.str().c_str(), nodeId, type);
		}
		return retValue;
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT bool IsExistingNode(const UINT32 nodeId)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		UINT32 nodePos;
		bool isExistingNode = false;
		NodeType type = (nodeId == 240)
		                ? MN
		                : CN;

		ocfmRetCode retValue = IfNodeExists(nodeId, type, &nodePos, isExistingNode);
		if (retValue.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			return isExistingNode;
		}
		return false;
	}
	return false;
}

DLLEXPORT UINT32 GetNodeCount()
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		UINT32 nodeCount = 0;
		ocfmRetCode retValue = GetNodeCount(&nodeCount);

		if (retValue.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			return nodeCount;
		}
		return 0;
	}
	return 0;
}