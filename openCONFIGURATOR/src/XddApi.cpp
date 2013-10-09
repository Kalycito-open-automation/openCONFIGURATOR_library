#include "../Include/XddApi.h"
#include "../Include/Node.h"
#include "../Include/NodeCollection.h"
#include "../Include/Enums.h"
#include "../Include/ProjectConfiguration.h"

DLLEXPORT ocfmRetCode GetDataTypeSize(const string name, UINT32& size)
{
	size = GetDataSize(name);
	if (size == 0)
	{
		return ocfmRetCode(OCFM_ERR_DATATYPE_NOT_FOUND);
	}
	return ocfmRetCode(OCFM_ERR_SUCCESS);
}

DLLEXPORT ocfmRetCode GetFeatureValue(const UINT32 nodeId, const FeatureType featureType, const string featureName, string& featureValue)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		bool isExistingNode = false;
		UINT32 nodePos = 0;
		NodeType type = (nodeId == 240)
		                ? MN
		                : CN;

		ocfmRetCode retCode = IfNodeExists(nodeId, type, &nodePos, isExistingNode);
		if ((retCode.getErrorCode() != OCFM_ERR_SUCCESS) || !isExistingNode)
		{
			isExistingNode = false;
			return retCode;
		}

		NodeCollection* nodeCollection = NodeCollection::GetNodeColObjectPointer();
		Node node = nodeCollection->GetNode(nodeId);

		NetworkManagement* nmtObj = node.GetNetworkManagement();
		featureValue = nmtObj->GetNwMgmtFeatureValue(featureType, featureName.c_str());
		return ocfmRetCode(OCFM_ERR_SUCCESS);
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}