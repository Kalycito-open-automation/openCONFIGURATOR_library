/************************************************************************
\file NodeApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/NodeApi.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/Result.h"
#include "../Include/Logging.h"
#include "../Include/BoostShared.h"
#include "../Include/OdApi.h"

#include <sstream>
#include <iostream>
#include <boost/lexical_cast.hpp>

using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::Library::NodeParameter;
using namespace openCONFIGURATOR::Library::Utilities;

namespace boost
{
	template<>
	bool lexical_cast<bool, string>(const string& arg)
	{
		try
		{
			istringstream ss(arg);
			ss.exceptions(std::ios::failbit);
			bool b;
			ss >> boolalpha >> b;
			return b;
		}
		catch (const std::istream::failure)
		{
			throw boost::bad_lexical_cast();
		}
	}

	template<>
	string lexical_cast<string, bool>(const bool& b)
	{
		try
		{
			ostringstream ss;
			ss.exceptions(std::ios::failbit);
			ss << boolalpha << b;
			return ss.str();
		}
		catch (const std::istream::failure)
		{
			throw boost::bad_lexical_cast();
		}
	}
}

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
							return Translate(ReImportXML(fullPath.generic_string().c_str(), nodeId, type));
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
			DLLEXPORT Result GetNodes(std::vector<UINT32>& nodeIds)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						nodeIds = NodeCollection::GetNodeColObjectPointer()->GetNodeIds();
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
			DLLEXPORT Result SetNodeParameter(const UINT32 nodeId, const ::NodeParameter param, const string value)
			{
				try
				{
					//Check if project loaded
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						//Check if value is empty
						if (value.empty())
						{
							boost::format formatter(kMsgEmptyArgument);
							formatter % "value";
							return Result(ARGUMENT_INVALID_EMPTY, formatter.str());
						}

						//Check if node exists
						bool existsNodeId = false;
						Result existResNodeId = IsExistingNode(nodeId, existsNodeId);
						if (existResNodeId.IsSuccessful() && !existsNodeId)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter % nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}

						Node& nodeObj = NodeCollection::GetNodeColObjectPointer()->GetNodeRef(nodeId);
						//Set for MN or CN
						switch (param)
						{
							//Set for CN and MN
							case NODENAME:
								{
									nodeObj.SetNodeName(value.c_str());
									break;
								}
							//Set for MN and CN
							case IS_ASYNC_ONLY:
								{
									bool boolValue = boost::lexical_cast<bool>(value);
									nodeObj.SetIsAsyncOnly(boolValue);
									break;
								}
							//Set for MN and CN
							case IS_TYPE_1ROUTER:
								{
									bool boolValue = boost::lexical_cast<bool>(value);
									nodeObj.SetIsType1Router(boolValue);
									break;
								}
							//Set for MN and CN
							case IS_TYPE_2ROUTER:
								{
									bool boolValue = boost::lexical_cast<bool>(value);
									nodeObj.SetIsType2Router(boolValue);
									break;
								}
						}
						//Set parameter for MN
						if (nodeId == 240)
						{
							switch (param)
							{
								//Set for MN
								case TRANSMIT_SPRES:
									{

										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetTransmitsPRes(boolValue);

										break;
									}
								//Set for MN
								case ASYNC_SLOT_TIMEOUT:
									{
										UINT32 asyncSlotTimeOut = 0;
										if(boost::istarts_with("0x", value))
											asyncSlotTimeOut = HexToInt<unsigned int>(value);
										else
											asyncSlotTimeOut = boost::lexical_cast<unsigned int>(value);
										bool exists = false;
										Result res = IsExistingSubIndex(nodeObj.GetNodeId(), 0x1F8A, 2, exists);
										if (res.IsSuccessful() && exists)
											nodeObj.SetActualValue(0x1F8A, 2, asyncSlotTimeOut);
										break;
									}
								//Set for MN
								case ASND_MAX_NUMBER:
									{
										UINT32 aSndMaxNumber = 0;
										if(boost::istarts_with("0x", value))
											aSndMaxNumber = HexToInt<unsigned int>(value);
										else
											aSndMaxNumber = boost::lexical_cast<unsigned int>(value);

										bool exists = false;
										Result res = IsExistingSubIndex(nodeObj.GetNodeId(), 0x1F8A, 2, exists);
										if (res.IsSuccessful() && exists)
											nodeObj.SetActualValue(0x1F8A, 3, aSndMaxNumber);
										break;
									}
							}
						}
						//Set Parameter for CN
						else
						{
							switch (param)
							{
								//Set only for CN
								case NODEID:
									{
										UINT32 newNodeId = 0;
										if(boost::istarts_with("0x", value))
											newNodeId = HexToInt<unsigned int>(value);
										else
											newNodeId = boost::lexical_cast<unsigned int>(value);

										if (nodeId == newNodeId)
											return Result();
										nodeObj.SetNodeId(newNodeId);
										CopyOldNodeIdAssignmentObject(&nodeObj, nodeId);
										if (nodeObj.GetStationType()  == MULTIPLEXED)
										{
											if (nodeObj.GetForceCycleFlag())
											{
												//Update the Forced Cycle with the new NodeID
												nodeObj.SetForcedCycle(nodeObj.GetForcedCycleValue());
											}
										}
										break;
									}
								//Set only for CN
								case STATIONTYPE:
									{
										INT32 stationType = -1;
										if(boost::istarts_with("0x", value))
											stationType = HexToInt<unsigned int>(value);
										else
											stationType = boost::lexical_cast<unsigned int>(value);

										if(stationType < 0 || stationType > 2)
										{
											boost::format formatter(kMsgParameterValueInvalid);
											formatter % nodeId
											% param
											% value;
											return Result(PARAMETER_VALUE_INVALID, formatter.str());
										}
										if(nodeObj.GetStationType() == stationType)
											return Result();
										StationType prevStationType = nodeObj.GetStationType();
										nodeObj.SetStationType((StationType) stationType);

										if (prevStationType == MULTIPLEXED)
										{
											nodeObj.ResetForcedCycleValue();
											nodeObj.SetForcedCycle("0");
										}

										if (stationType == MULTIPLEXED)
										{
											if (nodeObj.GetForcedCycleValue() != NULL && nodeObj.GetForceCycleFlag())
											{
												ocfmRetCode exceptionObj = nodeObj.SetForcedCycle(nodeObj.GetForcedCycleValue());
												if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
												{
													return Translate(exceptionObj);
												}
											}
											else
												ocfmRetCode exceptionObj = nodeObj.SetForcedCycle("0");
										}

										break;
									}
								//Set only for CN
								case FORCED_MULTIPLEXED_CYCLE:
									{
										if (nodeObj.GetStationType() == MULTIPLEXED)
										{
											if(value == "0")
												nodeObj.SetForceCycleFlag(false);
											else
												nodeObj.SetForceCycleFlag(true);

											ocfmRetCode exceptionObj = nodeObj.SetForcedCycle(value.c_str());
											if (OCFM_ERR_SUCCESS != exceptionObj.getErrorCode())
											{
												return Translate(exceptionObj);
											}
										}
										break;
									}

								//Set for CN
								case IS_MANDATORY:
									{

										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetIsMandatory(boolValue);

										break;
									}
								//Set for CN
								case AUTOSTART_NODE:
									{

										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetAutostartNode(boolValue);

										break;
									}
								//Set for CN
								case RESET_IN_OPERATIONAL:
									{

										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetResetInOperational(boolValue);

										break;
									}
								//Set for CN
								case VERIFY_APP_SW_VERSION:
									{
										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetVerifyAppSwVersion(boolValue);
										break;
									}
								//Set for CN
								case AUTO_APP_SW_UPDATE_ALLOWED:
									{
										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetAutoAppSwUpdateAllowed(boolValue);

										break;
									}
								//Set for CN
								case VERIFY_DEVICETYPE:
									{
										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetVerifyDeviceType(boolValue);

										break;
									}
								//Set for CN
								case VERIFY_VENDORID:
									{
										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetVerifyVendorId(boolValue);
										break;
									}
								//Set for CN
								case VERIFY_REVISIONNUMBER:
									{
										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetVerifyRevisionNumber(boolValue);

										break;
									}
								//Set for CN
								case VERIFY_PRODUCTCODE:
									{
										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetVerifyProductCode(boolValue);

										break;
									}
								//Set for CN
								case VERIFY_SERIALNUMBER:
									{
										bool boolValue = boost::lexical_cast<bool>(value);
										nodeObj.SetVerifySerialNumber(boolValue);

										break;
									}

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
				catch (const boost::bad_lexical_cast)
				{
					boost::format formatter(kMsgParameterValueInvalid);
					formatter % nodeId
					% param
					% value;
					return Result(PARAMETER_VALUE_INVALID, formatter.str());
				}
				catch (const exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}

			DLLEXPORT Result GetNodeParameter(const UINT32 nodeId, const ::NodeParameter param, string& value)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						//Check if node exists
						bool nodeExists = false;
						Result existResNodeId = IsExistingNode(nodeId, nodeExists);
						if (existResNodeId.IsSuccessful() && !nodeExists)
						{
							boost::format formatter(kMsgNonExistingNode);
							formatter % nodeId;
							return Result(NODE_DOES_NOT_EXIST, formatter.str());
						}

						ostringstream convertStr;
						Node nodeObj =  NodeCollection::GetNodeColObjectPointer()->GetNodeRef(nodeId);
						bool parameterSet = true;

						switch (param)
						{
							case NODENAME:
								{
									value = string(nodeObj.GetNodeName());
									break;
								}
							case NODEID:
								{
									convertStr << nodeObj.GetNodeId();
									value = convertStr.str();
									break;
								}
							case STATIONTYPE:
								{
									convertStr << nodeObj.GetStationType();
									value = convertStr.str();
									break;
								}
							case IS_ASYNC_ONLY:
								{
									if (nodeObj.IsAsyncOnly())
										value = (nodeObj.IsAsyncOnly().get() == true) ? "true" : "false";
									else
										parameterSet = false;
									break;
								}
							case IS_TYPE_1ROUTER:
								{
									if (nodeObj.IsType1Router())
										value = (nodeObj.IsType1Router().get() == true) ? "true" : "false";
									else
										parameterSet = false;
									break;
								}
							case IS_TYPE_2ROUTER:
								{
									if (nodeObj.IsType2Router())
										value = (nodeObj.IsType2Router().get() == true) ? "true" : "false";
									else
										parameterSet = false;
									break;
								}
						}
						if (nodeId == 240)
						{
							switch (param)
							{
								case TRANSMIT_SPRES:
									{

										if (nodeObj.TransmitsPRes())
											value = (nodeObj.TransmitsPRes().get() == true) ? "true" : "false";
										else
											parameterSet = false;

										break;
									}
								case ASYNC_SLOT_TIMEOUT:
									{
										bool exists = false;
										Result res = IsExistingSubIndex(nodeObj.GetNodeId(), 0x1F8A, 2, exists);
										if (res.IsSuccessful() && exists)
											value = nodeObj.GetActualValue<string>(0x1F8A, 2).get();
										else
											parameterSet = true;
										break;
									}
								case ASND_MAX_NUMBER:
									{
										bool exists = false;
										Result res = IsExistingSubIndex(nodeObj.GetNodeId(), 0x1F8A, 3, exists);
										if (res.IsSuccessful() && exists)
											value = nodeObj.GetActualValue<string>(0x1F8A, 3).get();
										else
											parameterSet = false;

										break;
									}
							}
						}
						else
						{
							switch (param)
							{
								case FORCED_MULTIPLEXED_CYCLE:
									{
										if (nodeObj.GetStationType() == MULTIPLEXED && nodeObj.GetForceCycleFlag() == true)
											value = string(nodeObj.GetForcedCycleValue());
										else
											parameterSet = false;
										break;
									}
								case IS_MANDATORY:
									{
										if (nodeObj.IsMandatory())
											value = (nodeObj.IsMandatory().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case AUTOSTART_NODE:
									{
										if (nodeObj.GetAutostartNode())
											value = (nodeObj.GetAutostartNode().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case RESET_IN_OPERATIONAL:
									{
										if (nodeObj.GetResetInOperational())
											value = (nodeObj.GetResetInOperational().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case VERIFY_APP_SW_VERSION:
									{
										if (nodeObj.GetVerifyAppSwVersion())
											value = (nodeObj.GetVerifyAppSwVersion().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case AUTO_APP_SW_UPDATE_ALLOWED:
									{
										if (nodeObj.GetAutoAppSwUpdateAllowed())
											value = (nodeObj.GetAutoAppSwUpdateAllowed().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case VERIFY_DEVICETYPE:
									{
										if (nodeObj.GetVerifyDeviceType())
											value = (nodeObj.GetVerifyDeviceType().get() == true) ? "true" : "false";
										else
											parameterSet = false;

										break;
									}
								case VERIFY_VENDORID:
									{
										if (nodeObj.GetVerifyVendorId())
											value = (nodeObj.GetVerifyVendorId().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case VERIFY_REVISIONNUMBER:
									{
										if (nodeObj.GetVerifyRevisionNumber())
											value = (nodeObj.GetVerifyRevisionNumber().get() ==  true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case VERIFY_PRODUCTCODE:
									{
										if (nodeObj.GetVerifyRevisionNumber())
											value = (nodeObj.GetVerifyRevisionNumber().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
								case VERIFY_SERIALNUMBER:
									{
										if (nodeObj.GetVerifySerialNumber())
											value = (nodeObj.GetVerifySerialNumber().get() == true) ? "true" : "false";
										else
											parameterSet = false;
										break;
									}
							}
						}
						if (!parameterSet)
						{
							boost::format formatter(kMsgParameterValueNotSet);
							formatter % nodeId
							% param;
							return Result(PARAMETER_VALUE_NOT_SET, formatter.str());
						}
						return Result();
					}
					return Result(NO_PROJECT_LOADED, kMsgNoProjectLoaded);

				}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}
		}
	}
}
