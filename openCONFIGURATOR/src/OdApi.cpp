#include "../Include/OdApi.h"
#include "../Include/NodeCollection.h"
#include "../Include/Validation.h"
#include "../Include/Internal.h"
#include "../Include/ProjectConfiguration.h"
#include <sstream>
#include <algorithm>

using namespace std;

DLLEXPORT ocfmRetCode AddIndex(const UINT32 nodeId, const UINT32 index, const string actualValue, const string name, ObjectType objectType)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		ostringstream objectTypeString("");
		objectTypeString << objectType;
		string indexString = IntToHex(index, 4, "", "");
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		ocfmRetCode retCode = AddIndex(nodeId, nodeType, indexString.c_str());
		if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
			return retCode;
		retCode = SetBasicIndexAttributes(nodeId, nodeType, indexString.c_str(), actualValue.c_str(), name.c_str(), false);
		if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
			return retCode;

		return SetIndexAttribute(nodeId, index, OBJECTTYPE, objectTypeString.str());
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode SetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, const string attributeValue)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		try
		{
			string indexString = IntToHex(index, 4, "", "");
			NodeType nodeType = (nodeId == 240)
			                    ? MN
			                    : CN;
			INT32 indexPos = 0;
			bool booleanVal = false;

			if (attributeType == FLAGIFINCDC)
			{
				if (attributeValue.compare("0") == 0)
				{
					booleanVal = false;
				}
				else if (attributeValue.compare("1") == 0)
				{
					booleanVal = true;
				}
				else
				{
					string attributeValueLowerCase(attributeValue);
					transform(attributeValueLowerCase.begin(), attributeValueLowerCase.end(), attributeValueLowerCase.begin(), ::tolower);
					if (attributeValueLowerCase.compare("true") == 0 || attributeValueLowerCase.compare("false") == 0)
					{
						istringstream inputBoolString(attributeValueLowerCase);
						inputBoolString >> std::boolalpha >> booleanVal;
					}
					else
					{
						throw ocfmRetCode(OCFM_ERR_INVALID_VALUE);
					}
				}
			}

			ocfmRetCode retCode = IfIndexExists(nodeId, nodeType, indexString.c_str(), &indexPos);
			if (OCFM_ERR_SUCCESS != retCode.getErrorCode())
			{
				retCode.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
				return retCode;
			}

			NodeCollection* nodeCollObj = NULL;
			Node nodeObj;
			IndexCollection* indexCollObj = NULL;
			Index* indexObj = NULL;

			nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			if (NULL == nodeCollObj)
			{
				retCode.setErrorCode(OCFM_ERR_NO_NODES_FOUND);
				return retCode;
			}

			nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

			indexCollObj = nodeObj.GetIndexCollection();
			if (NULL == indexCollObj)
			{
				retCode.setErrorCode(OCFM_ERR_NO_INDEX_FOUND);
				return retCode;
			}

			indexObj = indexCollObj->GetIndex(indexPos);
			if (NULL == indexObj)
			{
				retCode.setErrorCode(OCFM_ERR_INDEXID_NOT_FOUND);
				return retCode;
			}

			if (!attributeValue.empty())
			{
				switch (attributeType)
				{
				case NAME:
					indexObj->SetName(attributeValue.c_str());
					break;
				case OBJECTTYPE:
					indexObj->SetObjectType(attributeValue.c_str());
					if (indexObj->GetEObjectType() == ARRAY)
					{
						indexObj->UpdateArraySubObjects();
					}
					break;
				case DATATYPE:
					if ((CheckIfDataTypeByNameExists(attributeValue.c_str(), indexObj->GetNodeID())) == true)
					{
						indexObj->SetDataTypeName(attributeValue.c_str(), nodeId);
						if (indexObj->GetEObjectType() == ARRAY)
						{
							indexObj->UpdateArraySubObjects();
						}
					}
					else
					{
						retCode.setErrorCode(OCFM_ERR_DATATYPE_NOT_FOUND);
						return retCode;
					}
					break;
				case ACCESSTYPE:
					indexObj->SetAccessType(attributeValue.c_str());
					break;
				case DEFAULTVALUE:
					indexObj->SetDefaultValue(attributeValue.c_str());
					break;
				case ACTUALVALUE:
					if (indexObj->IsIndexValueValid(attributeValue.c_str()))
					{
						indexObj->SetActualValue(attributeValue.c_str());
					}
					else
					{
						retCode.setErrorCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
						return retCode;
					}
					break;
				case PDOMAPPING:
					indexObj->SetPDOMapping(attributeValue.c_str());
					break;
				case LOWLIMIT:
					retCode = CheckUpperAndLowerLimits(attributeValue.c_str(), indexObj->GetHighLimit());
					if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
					{
						indexObj->SetLowLimit(attributeValue.c_str());
					}
					else
					{
						retCode.setErrorCode(OCFM_ERR_INVALID_UPPERLOWER_LIMITS);
						return retCode;
					}
					break;
				case HIGHLIMIT:
					retCode = CheckUpperAndLowerLimits(indexObj->GetLowLimit(), attributeValue.c_str());
					if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
					{
						indexObj->SetHighLimit(attributeValue.c_str());
					}
					else
					{
						retCode.setErrorCode(OCFM_ERR_INVALID_UPPERLOWER_LIMITS);
						return retCode;
					}
					break;
				case FLAGIFINCDC:
					indexObj->SetFlagIfIncludedCdc(booleanVal);
					break;
				default:
					break;
				}
				retCode.setErrorCode(OCFM_ERR_SUCCESS);
			}
			else
			{
				retCode.setErrorCode(OCFM_ERR_INVALID_VALUE);
				return retCode;
			}

			//TODO: Clarify what this does exactly
			if ((ARRAY == indexObj->GetEObjectType())
			        || (RECORD == indexObj->GetEObjectType()))
			{
				//delete the subobject 00 and then add and updates the number of entries
				retCode = DeleteSubIndex(nodeId, nodeType, indexString.c_str(), (char*)"00");
				retCode = AddSubobject(nodeId, nodeType, indexString.c_str());
				UpdateNumberOfEnteriesSIdx(indexObj, nodeType);
			}

			return retCode;
		}
		catch (ocfmRetCode exObj)
		{
			return exObj;
		}
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode SetIndexActualValue(const UINT32 nodeId, const UINT32 index, const string actualValue)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		AttributeType attrType = ACTUALVALUE;
		return SetIndexAttribute(nodeId, index, attrType, actualValue);
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode GetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, string& attributeValue)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		string indexString = IntToHex(index, 4, "", "");
		char* attributeValueTemp = new char[50];
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		ocfmRetCode retCode = GetIndexAttributes(nodeId, nodeType, indexString.c_str(), attributeType, attributeValueTemp);
		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			attributeValue.clear();
			attributeValue.append(attributeValueTemp);
		}
		delete[] attributeValueTemp;
		return retCode;
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode AddSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, const string actualValue, const string name)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		string indexString = IntToHex(index, 4, "", "");
		string subIndexString = IntToHex(subIndexId, 2, "", "");
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		ocfmRetCode retCode = AddSubIndex(nodeId, nodeType, indexString.c_str(), subIndexString.c_str());
		if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
			return retCode;

		return SetBasicSubIndexAttributes(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), actualValue.c_str(), name.c_str(), false);
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode SetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, AttributeType attributeType, const string attributeValue)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		try
		{
			string indexString = IntToHex(index, 4, "", "");
			string subIndexString = IntToHex(subIndexId, 2, "", "");
			NodeType nodeType = (nodeId == 240)
			                    ? MN
			                    : CN;

			INT32 sidxPos = 0;
			INT32 iIndexPos = 0;

			bool booleanVal = false;

			if (attributeType == FLAGIFINCDC)
			{
				if (attributeValue.compare("0") == 0)
				{
					booleanVal = false;
				}
				else if (attributeValue.compare("1") == 0)
				{
					booleanVal = true;
				}
				else
				{
					string attributeValueLowerCase(attributeValue);
					transform(attributeValueLowerCase.begin(), attributeValueLowerCase.end(), attributeValueLowerCase.begin(), ::tolower);
					if (attributeValueLowerCase.compare("true") == 0 || attributeValueLowerCase.compare("false") == 0)
					{
						istringstream inputBoolString(attributeValueLowerCase);
						inputBoolString >> std::boolalpha >> booleanVal;
					}
					else
					{
						throw ocfmRetCode(OCFM_ERR_INVALID_VALUE);
					}
				}
			}

			ocfmRetCode errCodeObj = IfSubIndexExists(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), &sidxPos, &iIndexPos);
			if (errCodeObj.getErrorCode() != OCFM_ERR_SUCCESS)
			{
				return errCodeObj;
			}

			NodeCollection* nodeCollObj = NodeCollection::GetNodeColObjectPointer();
			if (NULL == nodeCollObj)
			{
				throw ocfmRetCode(OCFM_ERR_NO_NODES_FOUND);
			}

			Node nodeObj = nodeCollObj->GetNode(nodeType, nodeId);

			IndexCollection* indexCollObj = nodeObj.GetIndexCollection();
			if (NULL == indexCollObj)
			{
				throw ocfmRetCode(OCFM_ERR_NO_INDEX_FOUND);
			}

			Index* indexObj = indexCollObj->GetIndex(iIndexPos);
			if (NULL == indexObj)
			{
				throw ocfmRetCode(OCFM_ERR_INDEXID_NOT_FOUND);
			}

			SubIndex* sidxObj = indexObj->GetSubIndex(sidxPos);
			if (NULL == sidxObj)
			{
				throw ocfmRetCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
			}

			switch (attributeType)
			{
			case NAME:
				sidxObj->SetName(attributeValue.c_str());
				break;
			case OBJECTTYPE:
				sidxObj->SetObjectType(attributeValue.c_str());
				break;
			case DATATYPE:
				if ((CheckIfDataTypeByNameExists(attributeValue.c_str(), indexObj->GetNodeID())) == true)
				{
					sidxObj->SetDataTypeName(attributeValue.c_str(), nodeId);
				}
				else
				{
					throw ocfmRetCode(OCFM_ERR_DATATYPE_NOT_FOUND);
				}
				break;
			case ACCESSTYPE:
				sidxObj->SetAccessType(attributeValue.c_str());
				break;
			case DEFAULTVALUE:
				sidxObj->SetDefaultValue(attributeValue.c_str());
				break;
			case ACTUALVALUE:
				sidxObj->SetActualValue(attributeValue.c_str());
				break;
			case PDOMAPPING:
				sidxObj->SetPDOMapping(attributeValue.c_str());
				break;
			case LOWLIMIT:
				errCodeObj = CheckUpperAndLowerLimits(attributeValue.c_str(), indexObj->GetHighLimit());
				if (errCodeObj.getErrorCode() == OCFM_ERR_SUCCESS)
				{
					sidxObj->SetLowLimit(attributeValue.c_str());
				}
				else
				{
					throw ocfmRetCode(OCFM_ERR_INVALID_UPPERLOWER_LIMITS);
				}
				break;
			case HIGHLIMIT:
				errCodeObj = CheckUpperAndLowerLimits(indexObj->GetLowLimit(), attributeValue.c_str());
				if (errCodeObj.getErrorCode() == OCFM_ERR_SUCCESS)
				{
					sidxObj->SetHighLimit(attributeValue.c_str());
				}
				else
				{
					throw ocfmRetCode(OCFM_ERR_INVALID_UPPERLOWER_LIMITS);
				}
				break;
			case FLAGIFINCDC:
				indexObj->SetFlagIfIncludedCdc(booleanVal);
				break;
			default:
				throw ocfmRetCode(OCFM_ERR_INVALID_ATTRIBUTETYPE);
			}

			return ocfmRetCode(OCFM_ERR_SUCCESS);
		}
		catch (ocfmRetCode& ex)
		{
			return ex;
		}
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode SetSubIndexActualValue(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, const string actualValue)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		return SetSubIndexAttribute(nodeId, index, subIndexId, ACTUALVALUE, actualValue);
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode GetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, AttributeType attributeType, string& attributeValue)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		string indexString = IntToHex(index, 4, "", "");
		string subIndexString = IntToHex(subIndexId, 2, "", "");
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;
		char* attributeValueTemp = new char [50];

		ocfmRetCode retCode = GetSubIndexAttributes(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), attributeType, attributeValueTemp);
		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			attributeValue.clear();
			attributeValue.append(attributeValueTemp);
		}
		delete[] attributeValueTemp;
		return retCode;
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT bool IsExistingIndex(const UINT32 nodeId, const UINT32 index)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		string indexString = IntToHex(index, 4, "", "");
		INT32 indexPosition = 0;
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		ocfmRetCode retCode = IfIndexExists(nodeId, nodeType, indexString.c_str(), &indexPosition);

		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			return true;
		}
		return false;
	}
	return false;
}

DLLEXPORT bool IsExistingSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		string indexString = IntToHex(index, 4, "", "");
		string subIndexString = IntToHex(subIndex, 2, "", "");
		INT32 indexPosition;
		INT32 subindexPosition;
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		ocfmRetCode retCode = IfSubIndexExists(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), &indexPosition, &subindexPosition);
		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			return true;
		}
		return false;
	}
	return false;
}

DLLEXPORT UINT32 GetIndexCount(const UINT32 nodeId)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		UINT32 nrOfIndices;
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		ocfmRetCode retCode = GetIndexCount(nodeId, nodeType, &nrOfIndices);
		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			return nrOfIndices;
		}
		return 0;
	}
	return 0;
}

DLLEXPORT UINT32 GetSubIndexCount(const UINT32 nodeId, const UINT32 index)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		string indexString = IntToHex(index, 4, "", "");
		UINT32 nrOfSubIndices;
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		ocfmRetCode retCode = GetSubIndexCount(nodeId, nodeType, indexString.c_str(), &nrOfSubIndices);
		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			return nrOfSubIndices;
		}
		return 0;
	}
	return 0;
}

DLLEXPORT UINT32 GetNumberOfEntries(const UINT32 nodeId, const UINT32 index, const bool getDefault)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		ocfmRetCode retCode(OCFM_ERR_UNKNOWN);
		string indexString = IntToHex(index, 4, "", "");
		UINT32 returnValue = 0;
		stringstream strValue;
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		char* outValue = new char[50];

		if (getDefault == true)
		{
			retCode = GetSubIndexAttributes(nodeId, nodeType, indexString.c_str(), (char*) "00", DEFAULTVALUE, outValue);
		}
		else
		{
			retCode = GetSubIndexAttributes(nodeId, nodeType, indexString.c_str(), (char*) "00", ACTUALVALUE, outValue);
		}
		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			strValue << outValue;
			strValue >> returnValue;
		}
		delete[] outValue;
		return returnValue;
	}
	return 0;
}

DLLEXPORT ocfmRetCode DeleteIndex(const UINT32 nodeId, const UINT32 index)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		ocfmRetCode retCode(OCFM_ERR_UNKNOWN);
		string indexString = IntToHex(index, 4, "", "");
		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		retCode = DeleteIndex(nodeId, nodeType, indexString.c_str());
		return retCode;
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode DeleteSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		string indexString = IntToHex(index, 4, "", "");
		string subIndexString = IntToHex(subIndex, 2, "", "");

		NodeType nodeType = (nodeId == 240)
		                    ? MN
		                    : CN;

		return DeleteSubIndex(nodeId, nodeType, indexString.c_str(), subIndexString.c_str());
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}