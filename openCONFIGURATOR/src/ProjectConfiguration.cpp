/************************************************************************
\file	ProjectConfiguration.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	11.10.2013
************************************************************************/

#include "../Include/ProjectConfiguration.h"
#include "../Include/Logging.h"
#include "../Include/Result.h"
#include "../Include/BoostShared.h"

using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::Library::Utilities;

const string PROJECT_XML_ROOT_ELEMENT = "openCONFIGURATORProject";
const string PROJECT_XML_VERSION_ATTRIBUTE = "version";

const string PROJECT_XML_PATH_ELEMENT = "Path";
const string PROJECT_XML_PATH_ID_ATTRIBUTE = "id";
const string PROJECT_XML_PATH_PATH_ATTRIBUTE = "path";
const string PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE = "defaultOutputPath";

const string PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT = "ProjectConfiguration";
const string PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE = "activeAutoGenerationSetting";
const string PROJECT_XML_PROJECT_CONFIGURATION_PROJECT_NAME_ATTRIBUTE = "name";

const string PROJECT_XML_SETTING_ELEMENT = "Setting";
const string PROJECT_XML_SETTING_NAME_ATTRIBUTE = "name";
const string PROJECT_XML_SETTING_VALUE_ATTRIBUTE = "value";

const string PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT = "AutoGenerationSettings";
const string PROJECT_XML_AUTOGENERATION_SETTINGS_ID_ATTRIBUTE = "id";
const string PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT = "NetworkConfiguration";
const string PROJECT_XML_MANAGING_NODE_ELEMENT = "MN";
const string PROJECT_XML_CONTROLLED_NODE_ELEMENT = "CN";

const string PROJECT_XML_NETWORK_CYCLE_TIME_ATTRIBUTE = "cycleTime";
const string PROJECT_XML_NETWORK_ASYNC_MTU_ATTRIBUTE = "asyncMTU";
const string PROJECT_XML_NETWORK_MULTIPLEXED_CYCLE_LENGTH_ATTRIBUTE = "multiplexedCycleLength";
const string PROJECT_XML_NETWORK_PRESCALER_ATTRIBUTE = "prescaler";

const string PROJECT_XML_NODE_NAME_ATTRIBUTE = "name";
const string PROJECT_XML_NODE_NODEID_ATTRIBUTE = "nodeID";
const string PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE = "pathToXDC";
const string PROJECT_XML_NODE_FORCED_MULTIPLEXED_CYCLE_ATTRIBUTE = "forcedMultiplexedCycle";
const string PROJECT_XML_NODE_ISMULTIPLEXED_ATTRIBUTE = "isMultiplexed";
const string PROJECT_XML_NODE_ISCHAINED_ATTRIBUTE = "isChained";
const string PROJECT_XML_NODE_ISMANDATORY_ATTRIBUTE = "isMandatory";
const string PROJECT_XML_NODE_AUTOSTARTNODE_ATTRIBUTE = "autostartNode";
const string PROJECT_XML_NODE_RESETINOPERATIONAL_ATTRIBUTE = "resetInOperational";
const string PROJECT_XML_NODE_VERIFYAPPSWVERSION_ATTRIBUTE = "verifyAppSwVersion";
const string PROJECT_XML_NODE_AUTOAPPSWUPDATEALLOWED_ATTRIBUTE = "autoAppSwUpdateAllowed";
const string PROJECT_XML_NODE_VERIFYDEVICETYPE_ATTRIBUTE = "verifyDeviceType";
const string PROJECT_XML_NODE_VERIFYVENDORID_ATTRIBUTE = "verifyVendorId";
const string PROJECT_XML_NODE_VERIFYREVISIONNUMBER_ATTRIBUTE = "verifyRevisionNumber";
const string PROJECT_XML_NODE_VERIFYPRODUCTCODE_ATTRIBUTE = "verifyProductCode";
const string PROJECT_XML_NODE_VERIFYSERIALNUMBER_ATTRIBUTE = "verifySerialNumber";
const string PROJECT_XML_NODE_ISASYNCONLY_ATTRIBUTE = "isAsyncOnly";
const string PROJECT_XML_NODE_ISTYPE1ROUTER_ATTRIBUTE = "isType1Router";
const string PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE = "isType2Router";

const string PROJECT_XML_NODE_MN_ASYNCSLOTTIMEOUT_ATTRIBUTE = "asyncSlotTimeout";
const string PROJECT_XML_NODE_MN_ASNDMAXNUMBER_ATTRIBUTE = "aSndMaxNumber";
const string PROJECT_XML_NODE_MN_TRANSMITSPRES_ATTRIBUTE = "transmitsPRes";

const string PROJECT_XML_VALUE_TRUE = "true";
const string PROJECT_XML_VALUE_FALSE = "false";

ProjectConfiguration ProjectConfiguration::instance;

ProjectConfiguration::ProjectConfiguration(void) : 
	initialized(false),
	projectFile(),
	projectPath(),
	generateMNOBD(true),
	autogenerationSettingID(),
	defaultOutputPath(),
	cycleTime(),
	asyncMTU(),
	multiplexedCycleLength(),
	prescaler()	
{}

void ProjectConfiguration::ResetConfiguration(void)
{
	initialized = false;

	projectFile = "";
	defaultOutputPath = "";
	projectPath = "";
	generateMNOBD = true;

	cycleTime = boost::none;
	asyncMTU = boost::none;
	multiplexedCycleLength = boost::none;
	prescaler = boost::none;

	autogenerationSettingID = "";
}

ProjectConfiguration::~ProjectConfiguration(void)
{

}

ProjectConfiguration& ProjectConfiguration::GetInstance(void)
{
	return ProjectConfiguration::instance;
}

const string& ProjectConfiguration::GetDefaultOutputPath() const
{
	return defaultOutputPath;
}

void ProjectConfiguration::SetDefaultOutputPath(const string& defaultOutputPath)
{
	this->defaultOutputPath = defaultOutputPath;
}

bool ProjectConfiguration::IsInitialized(void) const
{
	return initialized;
}

void ProjectConfiguration::SetInitialized(bool initialized)
{
	this->initialized = initialized;
}

bool ProjectConfiguration::GetGenerateMNOBD(void) const
{
	return generateMNOBD;
}

void ProjectConfiguration::SetGenerateMNOBD(bool generateMNOBD)
{
	this->generateMNOBD = generateMNOBD;
}

const string& ProjectConfiguration::GetProjectPath(void) const
{
	return projectPath;
}

void ProjectConfiguration::SetProjectPath(const string& projectPath)
{
	this->projectPath = projectPath;
}

const string& ProjectConfiguration::GetProjectFile(void) const
{
	return projectFile;
}

void ProjectConfiguration::SetProjectFile(const string& projectFile)
{
	this->projectFile = projectFile;
}

const boost::optional<UINT32>& ProjectConfiguration::GetCycleTime(void) const
{
	return this->cycleTime;
}

void ProjectConfiguration::SetCycleTime(UINT32 cycleTime)
{
	this->cycleTime = cycleTime;
}

const boost::optional<UINT32>& ProjectConfiguration::GetAsyncMTU(void) const
{
	return this->asyncMTU;
}

void ProjectConfiguration::SetAsyncMTU(UINT32 asyncMTU)
{
	this->asyncMTU = asyncMTU;
}

const boost::optional<UINT32>& ProjectConfiguration::GetMultiplexedCycleLength(void) const
{
	return this->multiplexedCycleLength;
}

void ProjectConfiguration::SetMultiplexedCycleLength(UINT32 multiplexedCycleLength)
{
	this->multiplexedCycleLength = multiplexedCycleLength;
}

const boost::optional<UINT32>& ProjectConfiguration::GetPrescaler(void) const
{
	return this->prescaler;
}

void ProjectConfiguration::SetPrescaler(UINT32 prescaler)
{
	this->prescaler = prescaler;
}

ocfmRetCode ProjectConfiguration::LoadProject(const string& projectFile)
{
	LOG_INFO() << "Loading project '" << projectFile << "'.";
	try
	{
		xmlTextReaderPtr xmlReader = xmlReaderForFile(projectFile.c_str(), NULL, 0);

		if (xmlReader != NULL)
		{
			INT32 retVal = xmlTextReaderRead(xmlReader);
			while (retVal == 1)
			{
				ProcessProject(xmlReader);
				retVal = xmlTextReaderRead(xmlReader);
			}
			if (retVal != 0)
			{
				xmlCleanupParser();
				xmlMemoryDump();
				throw ocfmRetCode(OCFM_ERR_PARSE_XML);
			}
		}
		else
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % projectFile;
			ocfmRetCode result(OCFM_ERR_CANNOT_OPEN_FILE);
			result.setErrorString(formatter.str());
			LOG_FATAL() << formatter.str();
			xmlCleanupParser();
			xmlMemoryDump();
			throw result;
		}

		xmlCleanupParser();
		xmlMemoryDump();

		// FIXME: What is this code block good for?? 
		NodeCollection* nodeCollObj = NodeCollection::GetNodeColObjectPointer();

		char* presTimeoutVal = new char[50];
		presTimeoutVal[0] = 0;

		for (INT32 i = 0; i < nodeCollObj->GetNumberOfNodes(); i++)
		{
			Node nodeObj = nodeCollObj->GetNodebyCollectionIndex(i);
			INT32 nodeId = nodeObj.GetNodeId();
			NodeType nodeType = nodeObj.GetNodeType();

			CopyPDODefToAct(nodeId, nodeType);
			//CopyMNPropDefToAct(nodeId, nodeType);

			if ((nodeId != MN_NODEID) && (nodeType != MN))
			{
				char* nodeIdHexStr = new char[SUBINDEX_LEN];
				nodeIdHexStr = IntToAscii(nodeId, nodeIdHexStr, 16);
				nodeIdHexStr = PadLeft(nodeIdHexStr, '0', 2);
				GetSubIndexAttributes(MN_NODEID, MN, "1f92", nodeIdHexStr, ACTUALVALUE, presTimeoutVal);
				LOG_DEBUG() << "Actual Value for PollResponse-Timeout (0x1F92) for node " << nodeId << " set to: " << presTimeoutVal;
				if (((presTimeoutVal == NULL)
					|| (strlen(presTimeoutVal) == 0))
					|| (!(ValidateCNPresTimeout(nodeIdHexStr, presTimeoutVal))))
				{
					CalculateCNPollResponse(nodeId, nodeType);
				}

				delete[] nodeIdHexStr;
			}
		}
		delete[] presTimeoutVal;
		// FIXME end

		this->OverrideNetworkConfiguration();
	}
	catch (ocfmRetCode& exceptionThrown)
	{
		return exceptionThrown;
	}
	this->initialized = true;
	LOG_INFO() << "Project-Load finished.";
	return ocfmRetCode(OCFM_ERR_SUCCESS);
}

void ProjectConfiguration::ProcessProject(xmlTextReaderPtr xmlReader)
{
	const string xmlName = ((const char*) xmlTextReaderConstName(xmlReader))
		? (const char*) xmlTextReaderConstName(xmlReader)
		: "";

	if (xmlTextReaderNodeType(xmlReader) == 1)
	{
		if (xmlName == PROJECT_XML_ROOT_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				//Process XML version not needed checked by XML validator
			}
		}

		else if (xmlName == PROJECT_XML_PATH_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				while (xmlTextReaderMoveToNextAttribute(xmlReader))
				{
					ProcessPath(xmlReader);
				}
			}
		}
		else if (xmlName == PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				while (xmlTextReaderMoveToNextAttribute(xmlReader))
				{
					const string xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
						? (const char*) xmlTextReaderConstName(xmlReader)
						: "";
					const string xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
						? (const char*) xmlTextReaderConstValue(xmlReader)
						: "";

					if (!xmlAttributeName.empty()
						&& !xmlAttributeValue.empty()
						&& xmlAttributeValue == PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE)
					{
						autogenerationSettingID = xmlAttributeValue;
					}
				}
			}
		}
		else if (xmlName == PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				ProcessAutogenerationSettings(xmlReader);
			}
		}
		else if (xmlName == PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				while (xmlTextReaderMoveToNextAttribute(xmlReader))
				{
					ProcessNetworkConfiguration(xmlReader);
				}
			}

		}
		else if (xmlName == PROJECT_XML_MANAGING_NODE_ELEMENT 
			|| xmlName == PROJECT_XML_CONTROLLED_NODE_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				ProcessNode(xmlReader);
			}
		}
	}
}

void ProjectConfiguration::ProcessAutogenerationSettings(xmlTextReaderPtr xmlReader)
{
	string currentAutogenerationSetting;
	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const string xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
			? (const char*) xmlTextReaderConstName(xmlReader)
			: "";
		const string xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
			? (const char*) xmlTextReaderConstValue(xmlReader)
			: "";

		if (!xmlAttributeName.empty()
			&& !xmlAttributeValue.empty()
			&& xmlAttributeName == PROJECT_XML_AUTOGENERATION_SETTINGS_ID_ATTRIBUTE)
		{
			currentAutogenerationSetting = xmlAttributeValue;
		}
	}
	//Process the selected AutogenerationSetting
	if (currentAutogenerationSetting == this->autogenerationSettingID)
	{
		INT32 retVal = xmlTextReaderRead(xmlReader);
		while (retVal == 1)
		{
			const string xmlName = ((const char*) xmlTextReaderConstName(xmlReader))
				? (const char*) xmlTextReaderConstName(xmlReader)
				: "";
			if (xmlName == PROJECT_XML_SETTING_ELEMENT)
			{
				if (xmlTextReaderHasAttributes(xmlReader) == 1)
				{
					ProcessAutogenerationSetting(xmlReader);
				}
				retVal = xmlTextReaderRead(xmlReader);
			}
			else
			{
				retVal = 0;
			}
		}
		if (retVal != 0)
		{
			xmlCleanupParser();
			xmlMemoryDump();
			throw ocfmRetCode(OCFM_ERR_PARSE_XML);
		}
	}
}

void ProjectConfiguration::ProcessAutogenerationSetting(xmlTextReaderPtr xmlReader)
{
	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const string xmlName = ((const char*) xmlTextReaderConstName(xmlReader))
			? (const char*) xmlTextReaderConstName(xmlReader)
			: "";
		//const xmlChar* xmlValue = xmlTextReaderConstValue(xmlReader);

		if (xmlName == PROJECT_XML_SETTING_NAME_ATTRIBUTE)
		{
			//Process Setting attribute Name
		}
		else if (xmlName == PROJECT_XML_SETTING_VALUE_ATTRIBUTE)
		{
			//Process Setting attribute Value
		}
	}
}

// TODO: ProcessProjectNode?
void ProjectConfiguration::ProcessNode(xmlTextReaderPtr xmlReader)
{	
	// MN, CN properties
	boost::optional<string> nodeName;
	boost::optional<string> pathToXDC;
	boost::optional<bool> isAsyncOnly;
	boost::optional<bool> isType1Router;
	boost::optional<bool> isType2Router;
	boost::optional<UINT32> nodeId;

	// MN properties
	boost::optional<bool> transmitsPRes;
	boost::optional<UINT32> asyncSlotTimeout;
	boost::optional<UINT32> aSndMaxNumber;

	// CN properties
	boost::optional<UINT32> forcedMultiplexedCycle;
	boost::optional<bool> autostartNode;
	boost::optional<bool> resetInOperational;
	boost::optional<bool> verifyAppSwVersion;
	boost::optional<bool> autoAppSwUpdateAllowed;
	boost::optional<bool> verifyDeviceType;
	boost::optional<bool> verifyVendorId;
	boost::optional<bool> verifyRevisionNumber;
	boost::optional<bool> verifyProductCode;
	boost::optional<bool> verifySerialNumber;
	boost::optional<bool> isMandatory;
	boost::optional<bool> isChained;
	boost::optional<bool> isMultiplexed;
	
	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	// Extract all node attributes for CN- and MN-Elements
	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const string xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
			? (const char*) xmlTextReaderConstName(xmlReader)
			: "";
		const string xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
			? (const char*) xmlTextReaderConstValue(xmlReader)
			: "";
		
		if (!xmlAttributeName.empty() && !xmlAttributeValue.empty())
		{
			if (xmlAttributeName == PROJECT_XML_NODE_NAME_ATTRIBUTE)
			{
				nodeName = xmlAttributeValue;
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_NODEID_ATTRIBUTE)
			{
				nodeId = boost::lexical_cast<UINT32>(xmlAttributeValue);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE)
			{
				pathToXDC = xmlAttributeValue;
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_FORCED_MULTIPLEXED_CYCLE_ATTRIBUTE)
			{
				forcedMultiplexedCycle = boost::lexical_cast<UINT32>(xmlAttributeValue);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_ISMULTIPLEXED_ATTRIBUTE)
			{
				isMultiplexed = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_ISCHAINED_ATTRIBUTE)
			{
				isChained = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_ISMANDATORY_ATTRIBUTE)
			{
				isMandatory = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_ISASYNCONLY_ATTRIBUTE)
			{
				isAsyncOnly = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_AUTOSTARTNODE_ATTRIBUTE)
			{
				autostartNode = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_RESETINOPERATIONAL_ATTRIBUTE)
			{
				resetInOperational = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_VERIFYAPPSWVERSION_ATTRIBUTE)
			{
				verifyAppSwVersion = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_AUTOAPPSWUPDATEALLOWED_ATTRIBUTE)
			{
				autoAppSwUpdateAllowed = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_VERIFYDEVICETYPE_ATTRIBUTE)
			{
				verifyDeviceType = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_VERIFYVENDORID_ATTRIBUTE)
			{
				verifyVendorId = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_VERIFYSERIALNUMBER_ATTRIBUTE)
			{
				verifySerialNumber = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_VERIFYPRODUCTCODE_ATTRIBUTE)
			{
				verifyProductCode = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_VERIFYREVISIONNUMBER_ATTRIBUTE)
			{
				verifyRevisionNumber = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_ISTYPE1ROUTER_ATTRIBUTE)
			{
				isType1Router = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE)
			{
				isType2Router = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_MN_ASYNCSLOTTIMEOUT_ATTRIBUTE)
			{
				asyncSlotTimeout = boost::lexical_cast<UINT32>(xmlAttributeValue);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_MN_ASNDMAXNUMBER_ATTRIBUTE)
			{
				aSndMaxNumber = boost::lexical_cast<UINT32>(xmlAttributeValue);
			}
			else if (xmlAttributeName == PROJECT_XML_NODE_MN_TRANSMITSPRES_ATTRIBUTE)
			{
				transmitsPRes = (xmlAttributeValue == PROJECT_XML_VALUE_TRUE);
			}
		}
	} //while

	// Should have been caught by XML-Schema validation
	assert(nodeId);
	assert(pathToXDC);

	// Invalid setup. Concurrent Chaining and Multiplexing is not specified.
	if (isChained.get_value_or(false) && isMultiplexed.get_value_or(false))
	{
		ostringstream errorString;
		errorString << "Configuration error node " << nodeId << ": Simultanous Multiplexing and PRes-Chaining not supported.";
		ocfmRetCode result(OCFM_ERR_CHAINED_AND_MULTIPLEXED);
		result.setErrorString(errorString.str());
		throw result;
	}

	// Extract path to XDC and convert to absolute if necessary
	boost::filesystem::path xdcPath = pathToXDC.get();
	if (xdcPath.is_relative())
	{
		xdcPath = this->GetProjectPath(); // trailing (back)slash
		xdcPath.append(pathToXDC.get().begin(), pathToXDC.get().end()); // might result in // or \\ which should be ignored by the OS
	}
	LOG_DEBUG() << "XDC-Path for node " << nodeId.get() << ": " << xdcPath.generic_string<string>();

	// Set a default node name if it was omitted
	if ((!nodeName || nodeName.get().empty()) && nodeId.get() == MN_NODEID)
		nodeName = "MN";
	else if ((!nodeName || nodeName.get().empty()) && nodeId.get() != MN_NODEID) 
		nodeName = "CN_" + boost::lexical_cast<std::string>(nodeId.get());
	
	// Create node
	exceptionObj = NewProjectNode(nodeId.get(),
		(nodeId.get() == MN_NODEID) ? MN : CN,
		nodeName.get().c_str(), 
		xdcPath.generic_string().c_str());
	if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		throw exceptionObj;

	// Override node configuration for certain properties.
	// Excluded: Bits in 0x1F81, NodeAssignment. Bits in 0x1F80, StartUp
	Node& nodeObj = NodeCollection::GetNodeColObjectPointer()->GetNodeRef(nodeId.get());
	if (nodeId.get() == MN_NODEID)
	{
		if (nodeObj.GetIndexCollection()->ContainsIndex(0x1F8A, 2))
			this->OverrideNodeConfiguration(nodeObj, 0x1F8A, 2, asyncSlotTimeout);
		if (nodeObj.GetIndexCollection()->ContainsIndex(0x1F8A, 3))
			this->OverrideNodeConfiguration(nodeObj, 0x1F8A, 3, aSndMaxNumber);		
	}
	else
	{
		// 0x1F9B should exist on MN and CN if Multiplexing is supported, but typically the
		// Multiplexed-Cycle assignments will be entered on the MN to be distributed to all CNs.
		if (isMultiplexed.get_value_or(false))
		{
			// NOTE: This only works if MN is defined before all CNs in the openCONFIGURATOR project-config!
			Node& managingNode = NodeCollection::GetNodeColObjectPointer()->GetNodeRef(MN_NODEID);
			if (managingNode.GetIndexCollection()->ContainsIndex(0x1F9B, nodeId.get()))
				this->OverrideNodeConfiguration(managingNode, 0x1F9B, nodeId.get(), forcedMultiplexedCycle);
		}
	}
	
	StationType stationType = NORMAL;
	if (isChained.get_value_or(false))
		stationType = CHAINED;
	else if (isMultiplexed.get_value_or(false))
		stationType = MULTIPLEXED;
	nodeObj.SetStationType(stationType);

	// Properties which were not synchronized to the OD of the respective Node
	// are being set as Node-Properties.
	if (nodeId.get() != MN_NODEID)
	{
		// CN properties
		if (stationType == MULTIPLEXED)
		{		
			nodeObj.SetForceCycleFlag(forcedMultiplexedCycle.get_value_or(0) != 0);
			nodeObj.SetForcedCycle(boost::lexical_cast<std::string>(forcedMultiplexedCycle.get_value_or(0)).c_str());
		}
		else
		{		
			nodeObj.SetForceCycleFlag(false);
			nodeObj.SetForcedCycle("0");
		}
		if (isMandatory)
			nodeObj.SetIsMandatory(isMandatory.get());
		if (autostartNode)
			nodeObj.SetAutostartNode(autostartNode.get());
		if (resetInOperational)
			nodeObj.SetResetInOperational(resetInOperational.get());
		if (autoAppSwUpdateAllowed)
			nodeObj.SetAutoAppSwUpdateAllowed(autoAppSwUpdateAllowed.get());
		if (verifyAppSwVersion)
			nodeObj.SetVerifyAppSwVersion(verifyAppSwVersion.get());
		if (verifyDeviceType)
			nodeObj.SetVerifyDeviceType(verifyDeviceType.get());
		if (verifyProductCode)
			nodeObj.SetVerifyProductCode(verifyProductCode.get());
		if (verifyRevisionNumber)
			nodeObj.SetVerifyRevisionNumber(verifyRevisionNumber.get());
		if (verifySerialNumber)
			nodeObj.SetVerifySerialNumber(verifySerialNumber.get());
		if (verifyVendorId)
			nodeObj.SetVerifyVendorId(verifyVendorId.get());
	}	
	else
	{
		// MN properties
		if (transmitsPRes)
			nodeObj.SetTransmitsPRes(transmitsPRes.get());
	}
	// Common properties	
	if (isAsyncOnly)
		nodeObj.SetIsAsyncOnly(isAsyncOnly.get());
	if (isType1Router)
		nodeObj.SetIsType1Router(isType1Router.get());
	if (isType2Router)
		nodeObj.SetIsType2Router(isType2Router.get());	
}

void ProjectConfiguration::ProcessPath(xmlTextReaderPtr xmlReader)
{
	string xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
			? (const char*) xmlTextReaderConstName(xmlReader)
			: "";
	string xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
			? (const char*) xmlTextReaderConstValue(xmlReader)
			: "";

	if (!xmlAttributeName.empty()
		&& !xmlAttributeValue.empty()
		&& xmlAttributeName == PROJECT_XML_PATH_ID_ATTRIBUTE)
	{
		if (xmlAttributeValue == PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE)
		{
			xmlTextReaderMoveToNextAttribute(xmlReader);			
			xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
				? (const char*) xmlTextReaderConstName(xmlReader)
				: "";
			xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
				? (const char*) xmlTextReaderConstValue(xmlReader)
				: "";
			if (!xmlAttributeName.empty()
				&& !xmlAttributeValue.empty()
				&& xmlAttributeName == PROJECT_XML_PATH_PATH_ATTRIBUTE)
			{
				this->SetDefaultOutputPath(xmlAttributeValue);
			}
		}
	}
}

// TODO: Where are the properties set in this method ever used?
// If these settings are not defined within an MN-XDD, but just the project file, will
// they be evaluated during the CDC-Generation??
void ProjectConfiguration::ProcessNetworkConfiguration(xmlTextReaderPtr xmlReader)
{
	string xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
		? (const char*) xmlTextReaderConstName(xmlReader)
		: "";
	string xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
		? (const char*) xmlTextReaderConstValue(xmlReader)
		: "";

	if (!xmlAttributeName.empty() && !xmlAttributeValue.empty())
	{
		if (xmlAttributeName == PROJECT_XML_NETWORK_CYCLE_TIME_ATTRIBUTE)
		{
			//0x1006
			this->SetCycleTime(boost::lexical_cast<UINT32>(xmlAttributeValue));		
		}
		else if (xmlAttributeName == PROJECT_XML_NETWORK_ASYNC_MTU_ATTRIBUTE)
		{
			//0x1F98/0x8
			this->SetAsyncMTU(boost::lexical_cast<UINT32>(xmlAttributeValue));		
		}
		else if (xmlAttributeName == PROJECT_XML_NETWORK_MULTIPLEXED_CYCLE_LENGTH_ATTRIBUTE)
		{
			//0x1F98/0x7
			this->SetMultiplexedCycleLength(boost::lexical_cast<UINT32>(xmlAttributeValue));		
		}
		else if (xmlAttributeName == PROJECT_XML_NETWORK_PRESCALER_ATTRIBUTE)
		{
			//0x1F98/0x9
			this->SetPrescaler(boost::lexical_cast<UINT32>(xmlAttributeValue));		
		}
	}
}

/*
// Synchronize 1F98/7 between MN and all CNs
// FIXME: Why is this done at project-load? If a user changes the cycle-length later via interface,
// will it still be distributed to all CNs?
// Shouldn't a cycleLength defined in the MN-XDD just overwrite the setting in the project-settings?
// TODO: If a setting within the MN-XDD overwrites a setting from the project-file, there should be an INFO log-entry.
// FIXME: What happens with MN and CN-Settings defined in the project-file AND the respective XDD?
void ProjectConfiguration::SynchronizeMultiplexedCycleLength(void)
{
	stringstream convertString;
	INT32 indexPos = 0;

	//Check if index 1F98 exists on MN (is mandatory)
	NodeCollection::GetNodeColObjectPointer()->GetMNNode().GetIndexCollection()->ContainsIndex(0x1F98);

	ocfmRetCode retCode = IfIndexExists(MN_NODEID, MN, "1F98", &indexPos);
	if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
	{
		throw retCode;
	}
	NodeCollection* nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	if (!nodeCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_NODES_FOUND);
	}

	Node nodeObj = nodeCollObj->GetNode(MN, MN_NODEID);
	IndexCollection* indexCollObj = nodeObj.GetIndexCollection();
	if (NULL == indexCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_INDEX_FOUND);
	}

	Index* indexObj = indexCollObj->GetIndexByPosition(indexPos);
	if (NULL == indexObj)
	{
		throw ocfmRetCode(OCFM_ERR_INDEXID_NOT_FOUND);
	}

	SubIndex* subIndexObj = indexObj->GetSubIndexbyIndexValue("07");
	if (NULL == indexObj)
	{
		throw ocfmRetCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
	}
	const char* actualValue = subIndexObj->GetActualValue();
	if (actualValue != NULL)
	{
		string actualValue(actualValue);
		if (!actualValue.empty())
		{
			//Set ProjectConfiguration multiplexedCycleLength
			convertString << actualValue;
			convertString >> this->multiplexedCycleLength;

			//Set all Nodes multiplexedCycleLength
			for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes(); nodeLC++)
			{
				nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);
				IndexCollection* indexCollObj = nodeObj.GetIndexCollection();
				if (indexCollObj == NULL)
				{
					throw ocfmRetCode(OCFM_ERR_NO_INDEX_FOUND);
				}
				retCode = IfIndexExists(nodeObj.GetNodeId(), nodeObj.GetNodeType(), "1F98", &indexPos);
				if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
				{
					throw ocfmRetCode(OCFM_ERR_INDEXID_NOT_FOUND);
				}
				indexObj = indexCollObj->GetIndexByPosition(indexPos);
				if (indexObj == NULL)
				{
					throw ocfmRetCode(OCFM_ERR_INDEXID_NOT_FOUND);
				}
				subIndexObj = indexObj->GetSubIndexbyIndexValue("07");
				if (indexObj == NULL)
				{
					throw ocfmRetCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
				}
				subIndexObj->SetActualValue(actualValue.c_str());
				//If actual value is "0" assure that multiplexing is off
				if (actualValue.compare("0") == 0)
				{
					nodeObj.SetForceCycleFlag(false);
					nodeObj.SetForcedCycle((char*) "0");
					nodeObj.SetForcedMultiplexedCycle(0);
				}
			}
		}
	}
	else
	{
		throw ocfmRetCode(OCFM_ERR_INVALID_VALUE);
	}
}

// FIXME: Why??
void ProjectConfiguration::SynchronizeCycleTime(void)
{
	stringstream convertString;
	INT32 indexPos = 0;

	//Check if index 1006 exists on MN (is mandatory)
	ocfmRetCode retCode = IfIndexExists(MN_NODEID, MN, "1006", &indexPos);
	if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
	{
		throw retCode;
	}
	NodeCollection* nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	if (NULL == nodeCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_NODES_FOUND);
	}

	Node nodeObj = nodeCollObj->GetNode(MN, MN_NODEID);
	IndexCollection* indexCollObj = nodeObj.GetIndexCollection();
	if (NULL == indexCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_INDEX_FOUND);
	}

	Index* indexObj = indexCollObj->GetIndexByPosition(indexPos);
	if (NULL == indexObj)
	{
		throw ocfmRetCode(OCFM_ERR_INDEXID_NOT_FOUND);
	}

	const char* actualValue = indexObj->GetActualValue();
	if (actualValue != NULL)
	{
		string actualValue(actualValue);
		if (!actualValue.empty())
		{
			//Set ProjectConfiguration CycleLength
			convertString << actualValue;
			convertString >> this->cycleTime;
		}
	}
}

// FIXME: Why??
void ProjectConfiguration::SynchronizeAsyncMtu(void)
{
	stringstream convertString;
	INT32 indexPos = 0;

	//Check if index 1F98 exists on MN (is mandatory)
	ocfmRetCode retCode = IfIndexExists(MN_NODEID, MN, "1F98", &indexPos);
	if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
	{
		throw retCode;
	}
	NodeCollection* nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	if (NULL == nodeCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_NODES_FOUND);
	}

	Node nodeObj = nodeCollObj->GetNode(MN, MN_NODEID);
	IndexCollection* indexCollObj = nodeObj.GetIndexCollection();
	if (NULL == indexCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_INDEX_FOUND);
	}

	Index* indexObj = indexCollObj->GetIndexByPosition(indexPos);
	if (NULL == indexObj)
	{
		throw ocfmRetCode(OCFM_ERR_INDEXID_NOT_FOUND);
	}

	SubIndex* subIndexObj = indexObj->GetSubIndexbyIndexValue("08");
	if (NULL == indexObj)
	{
		throw ocfmRetCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
	}
	const char* actualValue = subIndexObj->GetActualValue();
	if (actualValue != NULL)
	{
		string actualValue(actualValue);
		if (!actualValue.empty())
		{
			//Set ProjectConfiguration multiplexedCycleLength
			convertString << actualValue;
			convertString >> this->asyncMTU;
		}
	}
}

// FIXME: Why??
void ProjectConfiguration::SynchronizePrescaler(void)
{
	stringstream convertString;
	INT32 indexPos = 0;

	//Check if index 1F98 exists on MN (is mandatory)
	ocfmRetCode retCode = IfIndexExists(MN_NODEID, MN, "1F98", &indexPos);
	if (retCode.getErrorCode() != OCFM_ERR_SUCCESS)
	{
		throw retCode;
	}
	NodeCollection* nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	if (NULL == nodeCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_NODES_FOUND);
	}

	Node nodeObj = nodeCollObj->GetNode(MN, MN_NODEID);
	IndexCollection* indexCollObj = nodeObj.GetIndexCollection();
	if (NULL == indexCollObj)
	{
		throw ocfmRetCode(OCFM_ERR_NO_INDEX_FOUND);
	}

	Index* indexObj = indexCollObj->GetIndexByPosition(indexPos);
	if (NULL == indexObj)
	{
		throw ocfmRetCode(OCFM_ERR_INDEXID_NOT_FOUND);
	}

	SubIndex* subIndexObj = indexObj->GetSubIndexbyIndexValue("09");
	if (NULL == indexObj)
	{
		throw ocfmRetCode(OCFM_ERR_SUBINDEXID_NOT_FOUND);
	}
	const char* actualValue = subIndexObj->GetActualValue();
	if (actualValue != NULL)
	{
		string actualValue(actualValue);
		if (!actualValue.empty())
		{
			convertString << actualValue;
			convertString >> this->prescaler;
		}
	}
}
*/

void ProjectConfiguration::OverrideNetworkConfiguration()
{
	LOG_INFO() << "Overriding network-configuration...";
	Node& node = NodeCollection::GetNodeColObjectPointer()->GetNodeRef(MN_NODEID);
	this->OverrideNodeConfiguration(node, 0x1006, 0, this->cycleTime);
	this->OverrideNodeConfiguration(node, 0x1F98, 7, this->multiplexedCycleLength);
	this->OverrideNodeConfiguration(node, 0x1F98, 8, this->asyncMTU);	
	this->OverrideNodeConfiguration(node, 0x1F98, 9, this->prescaler);
	LOG_INFO() << "Overriding network-configuration done.";
}

template<typename T>
void ProjectConfiguration::OverrideNodeConfiguration(Node& node, 
	const UINT32 index, 
	const UINT32 subIndex, 
	boost::optional<T>& projectConfigValue) const
{
	LOG_DEBUG() 
		<< std::hex
		<< std::showbase
		<< "Overriding configuration for node " << node.GetNodeId()
		<< ", index " << index << "/" << subIndex << ".";
	boost::optional<T> currActualValue = node.GetActualValue<T>(index, subIndex);
	if (currActualValue)
	{
		// There is an actualValue for the index/subIndex, it
		// overrides the project-config
		std::string projectConfigValueStr = projectConfigValue
			? boost::lexical_cast<std::string>(projectConfigValue.get())
			: "NULL";
		LOG_DEBUG()
			<< std::hex
			<< std::showbase
			<< "Overwrote project-configuration value '"
			<< projectConfigValueStr << "'"
			<< " with actualValue '" << currActualValue.get() << "'.";
		projectConfigValue = currActualValue;
	}
	else
	{
		// There is no actualValue, set the project-config value
		// as actual value if it is valid
		if (projectConfigValue)
		{
			node.SetActualValue(index, subIndex, projectConfigValue.get());
			std::string currActualValueStr = currActualValue
				? boost::lexical_cast<std::string>(currActualValue.get())
				: "NULL";
			LOG_DEBUG() 
				<< std::hex
				<< std::showbase
				<< "Overwrote actualValue '" << currActualValueStr
				<< "' of node with project-configuration value '"
				<< projectConfigValue.get() << "'.";
		}
		else
		{
			LOG_DEBUG() << "Nothing to override.";
		}
	}

	/*
	Index& indexRef = node.GetIndexCollection()->GetIndexRef(index);
	SubIndex* subIndexPtr = indexRef.GetSubIndexPtr(subIndex);
	const char* actualValue = (subIndexPtr == NULL)
		? indexRef.GetActualValue()
		: subIndexPtr->GetActualValue();
	std::stringstream converter;
	if (actualValue != NULL && strlen(actualValue) != 0)
	{
		// There is an actualValue for the index/subIndex, it
		// overrides the project-config
		string actualValueStr(actualValue);
		if (boost::algorithm::starts_with(actualValueStr, "0x")) 
			converter << std::hex << actualValueStr.substr(2);
		else
			converter << std::boolalpha << actualValueStr;
		T temporary;
		converter >> temporary;
		projectConfigValue = temporary;
	}
	else
	{
		// There is no actualValue, set the project-config value
		// as actual value if there is a valid one
		if (projectConfigValue)
		{
			converter << std::boolalpha << projectConfigValue.get();
			if (subIndexPtr != NULL)
				subIndexPtr->SetActualValue(converter.str().c_str());
			else
				indexRef.SetActualValue(converter.str().c_str());
		}
	}
	*/
}
