/************************************************************************
\file OdApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/OdApi.h"
#include "../Include/NodeCollection.h"
#include "../Include/Validation.h"
#include "../Include/Internal.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/Result.h"
#include "../Include/Logging.h"
#include "../Include/NodeApi.h"
#include "../Include/BoostShared.h"

#include <sstream>
#include <algorithm>
#include <cassert>

using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::Library::ObjectDictionary;
using namespace openCONFIGURATOR::Library::Utilities;

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			DLLEXPORT ocfmRetCode AddIndex(const UINT32 nodeId, const UINT32 index, const string actualValue, const string name, ObjectType objectType)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					ostringstream objectTypeString;
					objectTypeString << objectType;
					string indexString = IntToHex(index, 4, "", "");
					NodeType nodeType = (nodeId == MN_NODEID)
						? MN
						: CN;

					ocfmRetCode result = AddIndex(nodeId, nodeType, indexString.c_str());
					if (result.getErrorCode() != OCFM_ERR_SUCCESS)
						return result;
					result = SetBasicIndexAttributes(nodeId, nodeType, indexString.c_str(), actualValue.c_str(), name.c_str(), false);
					if (result.getErrorCode() != OCFM_ERR_SUCCESS)
						return result;

					return SetIndexAttribute(nodeId, index, OBJECTTYPE, objectTypeString.str());
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode SetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, const string attributeValue)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					try
					{
						string indexStr = IntToHex(index, 4, "", "");

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

						IndexCollection* indexCollectionPtr = node->GetIndexCollection();
						assert(indexCollectionPtr);

						Index* indexPtr = indexCollectionPtr->GetIndexPtr(index);
						if (!indexPtr)
						{
							boost::format formatter(kMsgNonExistingIndex);
							formatter % index % nodeId;
							ocfmRetCode result(OCFM_ERR_INDEXID_NOT_FOUND);
							result.setErrorString(formatter.str());
							throw result;
						}

						switch (attributeType)
						{
							case ACTUALVALUE:
								if (indexPtr->IsIndexValueValid(attributeValue.c_str()))
									indexPtr->SetActualValue(attributeValue.c_str());
								// FIXME: Dead code - IsIndexValueValid() always returns true
								//else
								//	throw ocfmRetCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
								break;
							case FLAGIFINCDC:
							{
								// TODO: Remove as soon as flag is being removed
								string attributeValueLowerCase(attributeValue);
								transform(attributeValueLowerCase.begin(), attributeValueLowerCase.end(), attributeValueLowerCase.begin(), ::tolower);
								if (attributeValueLowerCase == "true" || attributeValueLowerCase == "1")
									indexPtr->SetFlagIfIncludedCdc(true);
								else if (attributeValueLowerCase == "false" || attributeValueLowerCase == "0")
									indexPtr->SetFlagIfIncludedCdc(false);
								else
								{
									boost::format formatter(kMsgAttributeValueInvalid);
									formatter % attributeValue
										% attributeType
										% index
										% 0
										% nodeId
										% "true or false";
									ocfmRetCode result(OCFM_ERR_INVALID_VALUE);
									result.setErrorString(formatter.str());
									LOG_FATAL() << formatter.str();
									throw result;
								}								
							}
							case NAME:
							case OBJECTTYPE:
							case DATATYPE:
							case ACCESSTYPE:
							case DEFAULTVALUE:
							case PDOMAPPING:
							case LOWLIMIT:
							case HIGHLIMIT:
							default:
							{
								boost::format formatter(kMsgUnsupportedAttributeType);
								formatter % ((int) attributeType);
								ocfmRetCode result(OCFM_ERR_INVALID_ATTRIBUTETYPE);
								result.setErrorString(formatter.str());					
								throw result;
							}
						}
						return ocfmRetCode(OCFM_ERR_SUCCESS);
					}
					catch (ocfmRetCode& ex)
					{
						LOG_FATAL() << ex.getErrorString();
						return ex;
					}
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			ocfmRetCode SetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, const string attributeValue)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					try
					{
						string indexStr = IntToHex(index, 4, "", "");
						string subIndexStr = IntToHex(subIndex, 2, "", "");

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

						IndexCollection* indexCollectionPtr = node->GetIndexCollection();
						assert(indexCollectionPtr);

						Index* indexPtr = indexCollectionPtr->GetIndexPtr(index);
						if (!indexPtr)
						{
							boost::format formatter(kMsgNonExistingIndex);
							formatter % index % nodeId;
							ocfmRetCode result(OCFM_ERR_INDEXID_NOT_FOUND);
							result.setErrorString(formatter.str());
							throw result;
						}

						SubIndex* subIndexPtr = indexPtr->GetSubIndexPtr(subIndex);
						if (!subIndexPtr)
						{
							boost::format formatter(kMsgNonExistingSubIndex);
							formatter % index % subIndex % nodeId;
							ocfmRetCode result(OCFM_ERR_SUBINDEXID_NOT_FOUND);
							result.setErrorString(formatter.str());
							throw result;
						}

						switch (attributeType)
						{
				
							case ACTUALVALUE:
								if (subIndexPtr->IsIndexValueValid(attributeValue.c_str()))
									subIndexPtr->SetActualValue(attributeValue.c_str());
								// FIXME: Dead code - IsIndexValueValid() always returns true
								//else
								//	throw ocfmRetCode(OCFM_ERR_VALUE_NOT_WITHIN_RANGE);
								break;
							case FLAGIFINCDC:
							{
								// TODO: Remove as soon as flag is being removed
								string attributeValueLowerCase(attributeValue);
								transform(attributeValueLowerCase.begin(), attributeValueLowerCase.end(), attributeValueLowerCase.begin(), ::tolower);
								if (attributeValueLowerCase == "true" || attributeValueLowerCase == "1")
									subIndexPtr->SetFlagIfIncludedCdc(true);
								else if (attributeValueLowerCase == "false" || attributeValueLowerCase == "0")
									subIndexPtr->SetFlagIfIncludedCdc(false);
								else
								{
									boost::format formatter(kMsgAttributeValueInvalid);
									formatter % attributeValue
										% attributeType
										% index
										% subIndex
										% nodeId
										% "true or false";
									ocfmRetCode result(OCFM_ERR_INVALID_VALUE);
									result.setErrorString(formatter.str());
									LOG_FATAL() << formatter.str();
									throw result;
								}
								break;
							}
							case NAME:
							case OBJECTTYPE:
							case DATATYPE:
							case ACCESSTYPE:
							case DEFAULTVALUE:
							case PDOMAPPING:
							case LOWLIMIT:
							case HIGHLIMIT:
							default:
							{
								boost::format formatter(kMsgUnsupportedAttributeType);
								formatter % ((int) attributeType);
								ocfmRetCode result(OCFM_ERR_INVALID_ATTRIBUTETYPE);
								result.setErrorString(formatter.str());
								throw result;
							}	
						}
						return ocfmRetCode(OCFM_ERR_SUCCESS);
					}
					catch (ocfmRetCode& ex)
					{
						LOG_FATAL() << ex.getErrorString();
						return ex;
					}
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode SetIndexActualValue(const UINT32 nodeId, const UINT32 index, const string actualValue)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
					return SetIndexAttribute(nodeId, index, ACTUALVALUE, actualValue);
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode GetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, string& attributeValue)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					string indexString = IntToHex(index, 4, "", "");
					char* attributeValueTemp = new char[50];
					NodeType nodeType = (nodeId == MN_NODEID)
						? MN
						: CN;

					ocfmRetCode result = GetIndexAttributes(nodeId, nodeType, indexString.c_str(), attributeType, attributeValueTemp);
					if (result.getErrorCode() == OCFM_ERR_SUCCESS)
					{
						attributeValue.clear();
						attributeValue.append(attributeValueTemp);
					}
					delete[] attributeValueTemp;
					return result;
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode AddSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, const string actualValue, const string name)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					string indexString = IntToHex(index, 4, "", "");
					string subIndexString = IntToHex(subIndexId, 2, "", "");
					NodeType nodeType = (nodeId == MN_NODEID)
						? MN
						: CN;

					ocfmRetCode result = AddSubIndex(nodeId, nodeType, indexString.c_str(), subIndexString.c_str());
					if (result.getErrorCode() != OCFM_ERR_SUCCESS)
						return result;

					return SetBasicSubIndexAttributes(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), actualValue.c_str(), name.c_str(), false);
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode SetSubIndexActualValue(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, const string actualValue)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
					return SetSubIndexAttribute(nodeId, index, subIndexId, ACTUALVALUE, actualValue);
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode GetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, AttributeType attributeType, string& attributeValue)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					string indexString = IntToHex(index, 4, "", "");
					string subIndexString = IntToHex(subIndexId, 2, "", "");
					NodeType nodeType = (nodeId == MN_NODEID)
						? MN
						: CN;
					char* attributeValueTemp = new char [50];

					ocfmRetCode result = GetSubIndexAttributes(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), attributeType, attributeValueTemp);
					if (result.getErrorCode() == OCFM_ERR_SUCCESS)
					{
						attributeValue.clear();
						attributeValue.append(attributeValueTemp);
					}
					delete[] attributeValueTemp;
					return result;
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			// FIXME: Must handle non-existing nodeId and index -> must return ocfmRetcode!
			DLLEXPORT bool IsExistingIndex(const UINT32 nodeId, const UINT32 index)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
					if (!node)
						return false;
					else
						return node->GetIndexCollection()->ContainsIndex(index, 0);
				}
				return false;
			}

			// FIXME: Must handle non-existing nodeId and index -> must return ocfmRetcode!
			DLLEXPORT bool IsExistingSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
					if (!node)
						return false;
					Index* indexPtr = node->GetIndexCollection()->GetIndexPtr(index);
					if (!indexPtr)
						return false;
					return indexPtr->ContainsSubIndex(subIndex);
				}
				return false;
			}

			// FIXME: Must handle non-existing nodeId and index -> must return ocfmRetcode!
			DLLEXPORT UINT32 GetIndexCount(const UINT32 nodeId)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
					if (!node)
						return 0;
					return node->GetIndexCollection()->Size();
				}
				return 0;
			}

			// FIXME: Must handle non-existing nodeId and index -> must return ocfmRetcode!
			DLLEXPORT UINT32 GetSubIndexCount(const UINT32 nodeId, const UINT32 index)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
					if (!node)
						return 0;
					Index* indexPtr = node->GetIndexCollection()->GetIndexPtr(index);
					if (!indexPtr)
						return 0;
					return indexPtr->GetNumberofSubIndexes();
				}
				return 0;
			}

			// FIXME: Must handle non-existing nodeId and index -> must return ocfmRetcode!
			DLLEXPORT UINT32 GetNumberOfEntries(const UINT32 nodeId, const UINT32 index, const bool getDefault)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
					if (!node)
						return 0;
					Index* indexPtr = node->GetIndexCollection()->GetIndexPtr(index);
					if (!indexPtr)
						return 0;
					SubIndex* subIndexPtr = indexPtr->GetSubIndexPtr(0);
					if (!subIndexPtr)
						return 0;
					const char* value = (getDefault) 
						? subIndexPtr->GetDefaultValue()
						: subIndexPtr->GetActualValue();
					if (!value)
						return 0;
					string valueStr = value;
					if (valueStr.substr(0, 2) == "0x")
						return HexToInt<UINT32>(valueStr);
					else
						return boost::lexical_cast<UINT32>(value);
				}
				return 0;
			}

			DLLEXPORT ocfmRetCode DeleteIndex(const UINT32 nodeId, const UINT32 index)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					string indexString = IntToHex(index, 4, "", "");
					NodeType nodeType = (nodeId == MN_NODEID)
						? MN
						: CN;

					return DeleteIndex(nodeId, nodeType, indexString.c_str());
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode DeleteSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					string indexString = IntToHex(index, 4, "", "");
					string subIndexString = IntToHex(subIndex, 2, "", "");
					NodeType nodeType = (nodeId == MN_NODEID)
						? MN
						: CN;

					return DeleteSubIndex(nodeId, nodeType, indexString.c_str(), subIndexString.c_str());
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

		}
	}
}