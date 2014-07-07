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

			DLLEXPORT Result AddIndex(const UINT32 nodeId, const UINT32 index, const string actualValue, const string name, ObjectType objectType)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						ostringstream objectTypeString;
						objectTypeString << objectType;
						string indexString = IntToHex(index, 4, "", "");
						NodeType nodeType = (nodeId == MN_NODEID)
						                    ? MN
						                    : CN;

						Result result = Translate(AddIndex(nodeId, nodeType, indexString.c_str()));
						if (!result.IsSuccessful())
							return result;
						result = Translate(SetBasicIndexAttributes(nodeId, nodeType, indexString.c_str(), actualValue.c_str(), name.c_str(), false));
						if (!result.IsSuccessful())
							return result;

						return SetIndexAttribute(nodeId, index, OBJECTTYPE, objectTypeString.str());
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

			DLLEXPORT Result SetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, const string attributeValue)
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

						IndexCollection* indexCollectionPtr = node->GetIndexCollection();
						assert(indexCollectionPtr);

						Index* indexPtr = indexCollectionPtr->GetIndexPtr(index);
						if (!indexPtr)
						{
							boost::format formatter(kMsgNonExistingIndex);
							formatter % index % nodeId;
							return Result(INDEX_DOES_NOT_EXIST, formatter.str());
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
										return Result(ATTRIBUTEVALUE_INVALID, formatter.str());
									}
								}
							case OBJECTTYPE:
								indexPtr->SetObjectType(GetObjectType(attributeValue));
								break;
							case NAME:
							case DATATYPE:
							case ACCESSTYPE:
							case DEFAULTVALUE:
							case PDOMAPPING:
							case LOWLIMIT:
							case HIGHLIMIT:
							default:
								{
									boost::format formatter(kMsgUnsupportedAttributeType);
									formatter % attributeType;
									return Result(UNSUPPORTED_ATTRIBUTETYPE, formatter.str());
								}
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

			Result SetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, AttributeType attributeType, const string attributeValue)
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

						IndexCollection* indexCollectionPtr = node->GetIndexCollection();
						assert(indexCollectionPtr);

						Index* indexPtr = indexCollectionPtr->GetIndexPtr(index);
						if (!indexPtr)
						{
							boost::format formatter(kMsgNonExistingIndex);
							formatter % index % nodeId;
							return Result(INDEX_DOES_NOT_EXIST, formatter.str());
						}

						SubIndex* subIndexPtr = indexPtr->GetSubIndexPtr(subIndex);
						if (!subIndexPtr)
						{
							boost::format formatter(kMsgNonExistingSubIndex);
							formatter % index % subIndex % nodeId;
							return Result(SUBINDEX_DOES_NOT_EXIST, formatter.str());
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
										return Result(ATTRIBUTEVALUE_INVALID, formatter.str());
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
									formatter % attributeType;
									return Result(UNSUPPORTED_ATTRIBUTETYPE, formatter.str());
								}
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

			DLLEXPORT Result SetIndexActualValue(const UINT32 nodeId, const UINT32 index, const string actualValue)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
						return SetIndexAttribute(nodeId, index, ACTUALVALUE, actualValue);
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

			DLLEXPORT Result GetIndexAttribute(const UINT32 nodeId, const UINT32 index, AttributeType attributeType, string& attributeValue)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						string indexString = IntToHex(index, 4, "", "");
						char* attributeValueTemp = new char[50];
						NodeType nodeType = (nodeId == MN_NODEID)
						                    ? MN
						                    : CN;

						Result result = Translate(GetIndexAttributes(nodeId, nodeType, indexString.c_str(), attributeType, attributeValueTemp));
						if (result.IsSuccessful())
						{
							attributeValue.clear();
							attributeValue.append(attributeValueTemp);
						}
						delete[] attributeValueTemp;
						return result;
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

			DLLEXPORT Result AddSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, const string actualValue, const string name)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						string indexString = IntToHex(index, 4, "", "");
						string subIndexString = IntToHex(subIndexId, 2, "", "");
						NodeType nodeType = (nodeId == MN_NODEID)
						                    ? MN
						                    : CN;

						Result result = Translate(AddSubIndex(nodeId, nodeType, indexString.c_str(), subIndexString.c_str()));
						if (!result.IsSuccessful())
							return result;

						return Translate(SetBasicSubIndexAttributes(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), actualValue.c_str(), name.c_str(), false));
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

			DLLEXPORT Result SetSubIndexActualValue(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, const string actualValue)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
						return SetSubIndexAttribute(nodeId, index, subIndexId, ACTUALVALUE, actualValue);
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

			DLLEXPORT Result GetSubIndexAttribute(const UINT32 nodeId, const UINT32 index, const UINT32 subIndexId, AttributeType attributeType, string& attributeValue)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						string indexString = IntToHex(index, 4, "", "");
						string subIndexString = IntToHex(subIndexId, 2, "", "");
						NodeType nodeType = (nodeId == MN_NODEID)
						                    ? MN
						                    : CN;
						char* attributeValueTemp = new char [50];

						Result result = Translate(GetSubIndexAttributes(nodeId, nodeType, indexString.c_str(), subIndexString.c_str(), attributeType, attributeValueTemp));
						if (result.IsSuccessful())
						{
							attributeValue.clear();
							attributeValue.append(attributeValueTemp);
						}
						delete[] attributeValueTemp;
						return result;
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

			DLLEXPORT Result IsExistingIndex(const UINT32 nodeId, const UINT32 index, bool& exists)
			{
				try
				{
					exists = false;
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
						if (!node)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter
							% nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}
						else
						{
							if (node->GetIndexCollection()->ContainsIndex(index, 0))
								exists = true;
							return Result();
						}
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

			DLLEXPORT Result IsExistingSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex, bool& exists)
			{
				try
				{
					exists = false;
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
						if (!node)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter
							% nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}
						Index* indexPtr = node->GetIndexCollection()->GetIndexPtr(index);
						if (!indexPtr)
						{
							boost::format formatter(kMsgExistingIndex);
							formatter
							% index
							% nodeId;
							return Result(INDEX_DOES_NOT_EXIST, formatter.str());
						}

						if (indexPtr->ContainsSubIndex(subIndex))
							exists = true;
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

			DLLEXPORT Result GetIndexCount(const UINT32 nodeId, UINT32& indexCount)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
						if (!node)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter
							% nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}
						indexCount = node->GetIndexCollection()->Size();
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

			DLLEXPORT Result GetSubIndexCount(const UINT32 nodeId, const UINT32 index, UINT32& subIndexCount)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
						if (!node)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter
							% nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}
						Index* indexPtr = node->GetIndexCollection()->GetIndexPtr(index);
						if (!indexPtr)
						{
							boost::format formatter(kMsgExistingIndex);
							formatter
							% index
							% nodeId;
							return Result(INDEX_DOES_NOT_EXIST, formatter.str());
						}
						subIndexCount = indexPtr->GetNumberofSubIndexes();
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

			DLLEXPORT Result GetNumberOfEntries(const UINT32 nodeId, const UINT32 index, const bool getDefault, UINT32& nrOfEntries)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(nodeId);
						if (!node)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter
							% nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}
						Index* indexPtr = node->GetIndexCollection()->GetIndexPtr(index);
						if (!indexPtr)
						{
							boost::format formatter(kMsgExistingIndex);
							formatter
							% index
							% nodeId;
							return Result(INDEX_DOES_NOT_EXIST, formatter.str());
						}
						SubIndex* subIndexPtr = indexPtr->GetSubIndexPtr(0);
						if (!subIndexPtr)
						{
							boost::format formatter(kMsgExistingSubIndex);
							formatter
							% index
							% 0
							% nodeId;
							return Result(SUBINDEX_DOES_NOT_EXIST, formatter.str());
						}
						const char* value = (getDefault)
						                    ? subIndexPtr->GetDefaultValue()
						                    : subIndexPtr->GetActualValue();
						if (!value)
						{
							boost::format formatter(kMsgNoActualOrDefaultValue);
							formatter
							% index
							% 0
							% nodeId;
							return Result(NO_DEFAULT_OR_ACTUAL_VALUE, formatter.str());
						}
						string valueStr = value;
						if (valueStr.substr(0, 2) == "0x")
							nrOfEntries = HexToInt<UINT32>(valueStr);
						else
							nrOfEntries =  boost::lexical_cast<UINT32>(value);
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
			DLLEXPORT Result DeleteIndex(const UINT32 nodeId, const UINT32 index)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						string indexString = IntToHex(index, 4, "", "");
						NodeType nodeType = (nodeId == MN_NODEID)
						                    ? MN
						                    : CN;

						return Translate(DeleteIndex(nodeId, nodeType, indexString.c_str()));
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

			DLLEXPORT Result DeleteSubIndex(const UINT32 nodeId, const UINT32 index, const UINT32 subIndex)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						string indexString = IntToHex(index, 4, "", "");
						string subIndexString = IntToHex(subIndex, 2, "", "");
						NodeType nodeType = (nodeId == MN_NODEID)
						                    ? MN
						                    : CN;

						return Translate(DeleteSubIndex(nodeId, nodeType, indexString.c_str(), subIndexString.c_str()));
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

			DLLEXPORT Result GetIndices(const UINT32 nodeId, std::vector<UINT32>& indices)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						bool nodeExists = false;
						if (IsExistingNode(nodeId, nodeExists).IsSuccessful() && nodeExists)
						{
							indices =  NodeCollection::GetNodeColObjectPointer()->GetNode(nodeId).GetIndexCollection()->GetIndices();
						}
						else
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter
							% nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
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

			DLLEXPORT Result GetSubIndices(const UINT32 nodeId, const UINT32 index, std::vector<UINT32>& subIndices)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						bool nodeExists = false;
						if (IsExistingNode(nodeId, nodeExists).IsSuccessful() && nodeExists)
						{
							bool indexExists = false;
							if (IsExistingIndex(nodeId, index, indexExists).IsSuccessful() && indexExists)
							{
								subIndices =  NodeCollection::GetNodeColObjectPointer()->GetNode(nodeId).GetIndexCollection()->GetIndexRef(index).GetSubindices();
							}
							else
							{
								boost::format formatter(kMsgNonExistingIndex);
								formatter
								% index
								% nodeId;
								return Result(INDEX_DOES_NOT_EXIST, formatter.str());
							}

						}
						else
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter
							% nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
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
