#include "../Include/ProjectConfiguration.h"
using namespace std;

const char* PROJECT_XML_ROOT_ELEMENT = "openCONFIGURATORProject";
const char* PROJECT_XML_VERSION_ATTRIBUTE = "version";

const char* PROJECT_XML_PATH_ELEMENT = "Path";
const char* PROJECT_XML_PATH_ID_ATTRIBUTE = "id";
const char* PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE = "defaultOutputPath";
const char* PROJECT_XML_PATH_TXT2CDC_ATTRIBUTE = "txt2CdcPath";

const char* PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT = "ProjectConfiguration";
const char* PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE = "activeAutoGenerationSetting";
const char* PROJECT_XML_PROJECT_CONFIGURATION_PROJECT_NAME_ATTRIBUTE = "name";

const char* PROJECT_XML_SETTING_ELEMENT = "Setting";
const char* PROJECT_XML_SETTING_NAME_ATTRIBUTE = "name";
const char* PROJECT_XML_SETTING_VALUE_ATTRIBUTE = "value";

const char* PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT = "AutoGenerationSettings";
const char* PROJECT_XML_AUTOGENERATION_SETTINGS_ID_ATTRIBUTE = "id";
const char* PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT = "NetworkConfiguration";
const char* PROJECT_XML_MANAGING_NODE_ELEMENT = "MN";
const char* PROJECT_XML_CONTROLLED_NODE_ELEMENT = "CN";

const char* PROJECT_XML_NETWORK_CYCLE_TIME_ATTRIBUTE = "cycleTime";
const char* PROJECT_XML_NETWORK_ASYNC_MTU_ATTRIBUTE = "asyncMTU";
const char* PROJECT_XML_NETWORK_MULTIPLEXED_CYCLE_LENGTH_ATTRIBUTE = "multiplexedCycleLength";
const char* PROJECT_XML_NETWORK_PRESCALER_ATTRIBUTE = "prescaler";

const char* PROJECT_XML_NODE_NAME_ATTRIBUTE = "name";
const char* PROJECT_XML_NODE_NODEID_ATTRIBUTE = "nodeID";
const char* PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE = "pathToXDC";
const char* PROJECT_XML_NODE_FORCED_MULTIPLEXED_CYCLE_ATTRIBUTE = "forcedMultiplexedCycle";
const char* PROJECT_XML_NODE_ISMULTIPLEXED_ATTRIBUTE = "isMultiplexed";
const char* PROJECT_XML_NODE_ISCHAINED_ATTRIBUTE = "isChained";
const char* PROJECT_XML_NODE_ISMANDATORY_ATTRIBUTE = "isMandatory";
const char* PROJECT_XML_NODE_AUTOSTARTNODE_ATTRIBUTE = "autostartNode";
const char* PROJECT_XML_NODE_RESETINOPERATIONAL_ATTRIBUTE = "resetInOperational";
const char* PROJECT_XML_NODE_VERIFYAPPSWVERSION_ATTRIBUTE = "verifyAppSwVersion";
const char* PROJECT_XML_NODE_AUTOAPPSWUPDATEALLOWED_ATTRIBUTE = "autoAppSwUpdateAllowed";
const char* PROJECT_XML_NODE_VERIFYDEVICETYPE_ATTRIBUTE = "verifyDeviceType";
const char* PROJECT_XML_NODE_VERIFYVENDORID_ATTRIBUTE = "verifyVendorId";
const char* PROJECT_XML_NODE_VERIFYREVISIONNUMBER_ATTRIBUTE = "verifyRevisionNumber";
const char* PROJECT_XML_NODE_VERIFYPRODUCTCODE_ATTRIBUTE = "verifyProductCode";
const char* PROJECT_XML_NODE_VERIFYSERIALNUMBER_ATTRIBUTE = "verifySerialNumber";
const char* PROJECT_XML_NODE_ISASYNCONLY_ATTRIBUTE = "isAsyncOnly";
const char* PROJECT_XML_NODE_ISTYPE1ROUTER_ATTRIBUTE = "isType1Router";
const char* PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE = "isType2Router";

const char* PROJECT_XML_NODE_MN_ASYNCSLOTTIMEOUT_ATTRIBUTE = "asyncSlotTimeout";
const char* PROJECT_XML_NODE_MN_ASNDMAXNUMBER_ATTRIBUTE = "aSndMaxNumber";
const char* PROJECT_XML_NODE_MN_TRANSMITSPRES_ATTRIBUTE = "transmitsPRes";

const char* PROJECT_XML_VALUE_TRUE = "true";
const char* PROJECT_XML_VALUE_FALSE = "false";

ProjectConfiguration* ProjectConfiguration::instance = NULL;

ProjectConfiguration::ProjectConfiguration(void)
{
	projectLoaded = false;

	projectFile = "";
	defaultOutputPath = "";
	projectPath = "";
	pathTxt2CdcExecutable = "";
	generateMNOBD = true;

	cycleTime = 0;
	asyncMTU = 0;
	multiplexedCycleLength = 0;
	prescaler = 0;

	autogenerationSettingID = "";

	isChosenSetting = false;
}
void ProjectConfiguration::resetConfiguration(void)
{
	projectLoaded = false;

	projectFile = "";
	defaultOutputPath = "";
	projectPath = "";
	pathTxt2CdcExecutable = "";
	generateMNOBD = true;

	cycleTime = 0;
	asyncMTU = 0;
	multiplexedCycleLength = 0;
	prescaler = 0;

	autogenerationSettingID = "";

	isChosenSetting = false;
}

ProjectConfiguration::~ProjectConfiguration(void)
{
	delete ProjectConfiguration::instance;
	ProjectConfiguration::instance = NULL;
}

ProjectConfiguration* ProjectConfiguration::GetInstance(void)
{
	if (ProjectConfiguration::instance == NULL)
	{
		instance = new ProjectConfiguration();
	}
	return instance;
}

const string& ProjectConfiguration::GetDefaultOutputPath()
{
	return defaultOutputPath;
}

void ProjectConfiguration::SetDefaultOutputPath(const string& defaultOutputPath)
{
	this->defaultOutputPath = defaultOutputPath;
}

bool ProjectConfiguration::GetProjectLoaded(void)
{
	return projectLoaded;
}

void ProjectConfiguration::SetProjectLoaded(bool projectLoaded)
{
	this->projectLoaded = projectLoaded;
}

bool ProjectConfiguration::GetGenerateMNOBD(void)
{
	return generateMNOBD;
}

void ProjectConfiguration::SetGenerateMNOBD(bool generateMNOBD)
{
	this->generateMNOBD = generateMNOBD;
}

const string& ProjectConfiguration::GetProjectPath(void)
{
	return projectPath;
}

void ProjectConfiguration::SetProjectPath(const string& projectPath)
{
	this->projectPath = projectPath;
}

const string& ProjectConfiguration::GetPathTxt2CdcExecutable(void)
{
	return pathTxt2CdcExecutable;
}

void ProjectConfiguration::SetPathTxt2CdcExecutable(const string& pathTxt2CdcExecutable)
{
	this->pathTxt2CdcExecutable = pathTxt2CdcExecutable;
}

const string& ProjectConfiguration::GetProjectFile(void)
{
	return projectFile;
}

void ProjectConfiguration::SetProjectFile(const string& projectFile)
{
	this->projectFile = projectFile;
}

UINT32 ProjectConfiguration::GetCycleTime(void)
{
	return this->cycleTime;
}
void ProjectConfiguration::SetCycleTime(UINT32 cycleTime)
{
	this->cycleTime = cycleTime;
}

UINT32 ProjectConfiguration::GetAsyncMTU(void)
{
	return this->asyncMTU;
}

void ProjectConfiguration::SetAsyncMTU(UINT32 asyncMTU)
{
	this->asyncMTU = asyncMTU;
}

UINT32 ProjectConfiguration::GetMultiplexedCycleLength(void)
{
	return this->multiplexedCycleLength;
}

void ProjectConfiguration::SetMultiplexedCycleLength(UINT32 multiplexedCycleLength)
{
	this->multiplexedCycleLength = multiplexedCycleLength;
}

UINT32 ProjectConfiguration::GetPrescaler(void)
{
	return this->prescaler;
}
void ProjectConfiguration::SetPrescaler(UINT32 prescaler)
{
	this->prescaler = prescaler;
}

ocfmRetCode ProjectConfiguration::LoadProject(const string& projectFile)
{
	try
	{
		xmlTextReaderPtr xmlReader = xmlReaderForFile(projectFile.c_str(), NULL, 0);

		if (xmlReader != NULL)
		{
			INT32 retVal = xmlTextReaderRead(xmlReader);
			while (retVal == 1)
			{
				ProcessProjectElement(xmlReader);
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
			xmlCleanupParser();
			xmlMemoryDump();
			throw ocfmRetCode(OCFM_ERR_CANNOT_OPEN_FILE);
		}

		xmlCleanupParser();
		xmlMemoryDump();

		NodeCollection* nodeCollObj = NodeCollection::GetNodeColObjectPointer();
		nodeCollObj = NodeCollection::GetNodeColObjectPointer();

		char* presTimeoutVal = new char[50];
		presTimeoutVal[0] = 0;

		for (INT32 nodeLC = 0; nodeLC < nodeCollObj->GetNumberOfNodes();
			nodeLC++)
		{
			Node nodeObj;
			INT32 nodeId;
			NodeType nodeType;
			nodeObj = nodeCollObj->GetNodebyCollectionIndex(nodeLC);

			nodeType = nodeObj.GetNodeType();
			nodeId = nodeObj.GetNodeId();
			CopyPDODefToAct(nodeId, nodeType);
			CopyMNPropDefToAct(nodeId, nodeType);

			if ((nodeId != MN_NODEID) && (MN != nodeType))
			{
				char* value = NULL;
				value = new char[SUBINDEX_LEN];
				value = IntToAscii(nodeId, value, 16);
				value = PadLeft(value, '0', 2);
				GetSubIndexAttributes(MN_NODEID, MN, (char*) "1f92", value,
					ACTUALVALUE, presTimeoutVal);
#if defined DEBUG
				cout << "Actual Value" << nodeId << presTimeoutVal << endl;
#endif
				if (((NULL == presTimeoutVal)
					|| (strcmp(presTimeoutVal, "") == 0))
					|| (!(ValidateCNPresTimeout(value, presTimeoutVal))))
				{
					CalculateCNPollResponse(nodeId, nodeType);
				}

				delete[] value;
			}
		}
		delete[] presTimeoutVal;

		SynchronizeMultiplexingCycleLength();
		SynchronizeCycleTime();
		SynchronizeAsyncMtu();
		SynchronizePrescaler();
	}
	catch (ocfmRetCode& exceptionThrown)
	{
		return exceptionThrown;
	}

	projectLoaded = true;

	return ocfmRetCode(OCFM_ERR_SUCCESS);
}

void ProjectConfiguration::ProcessProjectElement(xmlTextReaderPtr xmlReader)
{
	const xmlChar* xmlName = xmlTextReaderConstName(xmlReader);

	if (xmlTextReaderNodeType(xmlReader) == 1)
	{
		if (strcmp(((const char*) xmlName), PROJECT_XML_ROOT_ELEMENT) == 0)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				//Process XML version not needed checked by XML validator
			}
		}

		else if (strcmp(((const char*) xmlName), PROJECT_XML_PATH_ELEMENT) == 0)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				while (xmlTextReaderMoveToNextAttribute(xmlReader))
				{
					ProcessPath(xmlReader);
				}
			}
		}
		else if (strcmp(((const char*) xmlName), PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT) == 0)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				while (xmlTextReaderMoveToNextAttribute(xmlReader))
				{
					const xmlChar *xmlAttributeName = xmlTextReaderConstName(xmlReader);
					const xmlChar *xmlAttributeValue = xmlTextReaderConstValue(xmlReader);

					if (strcmp(((char*) xmlAttributeName), PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE) == 0)
					{
						if ((char*) xmlAttributeValue != NULL)
						{
							autogenerationSettingID = string((const char*) xmlAttributeValue);
						}
					}
				}
			}
		}
		else if (strcmp(((const char*) xmlName), PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT) == 0)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				ProcessAutogenerationSettings(xmlReader);
			}
		}

		else if (strcmp(((const char*) xmlName), PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT) == 0)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				while (xmlTextReaderMoveToNextAttribute(xmlReader))
				{
					ProcessNetworkConfiguration(xmlReader);
				}
			}

		}
		else if (strcmp(((const char*) xmlName), PROJECT_XML_MANAGING_NODE_ELEMENT) == 0 || strcmp(((const char*) xmlName), PROJECT_XML_CONTROLLED_NODE_ELEMENT) == 0)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				ProcessProjectNode(xmlReader);
			}
		}
	}
}

void ProjectConfiguration::ProcessAutogenerationSettings(xmlTextReaderPtr xmlReader)
{
	string currentAutogenerationSetting("");
	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const xmlChar *xmlName = xmlTextReaderConstName(xmlReader);
		const xmlChar *xmlValue = xmlTextReaderConstValue(xmlReader);

		if (strcmp(((const char*) xmlName), PROJECT_XML_AUTOGENERATION_SETTINGS_ID_ATTRIBUTE) == 0)
		{
			if ((const char*) xmlValue != NULL)
			{
				currentAutogenerationSetting = string((const char*) xmlValue);
			}
		}
	}
	//Process the chosen AutogenerationSetting
	if (autogenerationSettingID.compare(currentAutogenerationSetting) == 0)
	{
		INT32 retVal = xmlTextReaderRead(xmlReader);
		while (retVal == 1)
		{
			const xmlChar* xmlName = xmlTextReaderConstName(xmlReader);
			if (strcmp(((const char*) xmlName), PROJECT_XML_SETTING_ELEMENT) == 0)
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
		const xmlChar *xmlName = xmlTextReaderConstName(xmlReader);
		//const xmlChar *xmlValue = xmlTextReaderConstValue(xmlReader);

		if (strcmp(((const char*) xmlName), PROJECT_XML_SETTING_NAME_ATTRIBUTE) == 0)
		{
			//Process Setting attribute Name
		}
		else if (strcmp(((const char*) xmlName), PROJECT_XML_SETTING_VALUE_ATTRIBUTE) == 0)
		{
			//Process Setting attribute Value
		}
	}
}

void ProjectConfiguration::ProcessProjectNode(xmlTextReaderPtr xmlReader)
{
	char* nodeName = NULL;
	char* xdcFileName = NULL;
	char* forceCycleValue = NULL;

	//MN additional properties
	bool transmitsPRes = false;
	UINT32 asyncSlotTimeout = 0;
	UINT32 aSndMaxNumber = 0;

	//CN additional properties
	UINT32 forcedMultiplexedCycle = 0;
	bool autostartNode = true;
	bool resetInOperational = true;
	bool verifyAppSwVersion = false;
	bool autoAppSwUpdateAllowed = false;
	bool verifyDeviceType = false;
	bool verifyVendorId = false;
	bool verifyRevisionNumber = false;
	bool verifyProductCode = false;
	bool verifySerialNumber = false;

	//Stationtype bools
	bool isChained = false;
	bool isMultiplexed = false;

	//Both MN & CN
	bool isMandatory = false;
	bool isAsyncOnly = false;
	bool isType1Router = false;
	bool isType2Router = false;

	INT32 nodeId = 0;
	NodeType nodeType = CN;

	StationType stationType = NORMAL;

	ocfmRetCode exceptionObj(OCFM_ERR_UNKNOWN);

	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const xmlChar *xmlName = xmlTextReaderConstName(xmlReader);
		const xmlChar *xmlValue = xmlTextReaderConstValue(xmlReader);

		//Process Name attribute
		if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_NAME_ATTRIBUTE) == 0)
		{
			if ((const char*) xmlValue != NULL)
			{
				nodeName = new char[strlen((const char*) xmlValue) + ALLOC_BUFFER];
				strcpy(nodeName, (const char*) xmlValue);
			}
		}
		//Process NodeID attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_NODEID_ATTRIBUTE) == 0)
		{
			if ((const char*) xmlValue != NULL)
			{
				nodeId = atoi((char*) xmlValue);

				nodeType = (nodeId == 240)
					? MN
					: CN;
			}
		}
		//Process PathToXDC attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE) == 0)
		{
			xdcFileName = new char[strlen((const char*) xmlValue) + 1];
			strcpy(xdcFileName, (const char*) xmlValue);
		}
		//Process forcedMultiplexedCycle attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_FORCED_MULTIPLEXED_CYCLE_ATTRIBUTE) == 0)
		{
			forceCycleValue = new char[strlen((const char*) xmlValue) + ALLOC_BUFFER];
			strcpy(forceCycleValue, (const char*) xmlValue);

			istringstream multiCycleString(forceCycleValue);
			multiCycleString >> forcedMultiplexedCycle;
		}
		//Process isMultiplexed attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_ISMULTIPLEXED_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				isMultiplexed = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				isMultiplexed = false;
			}
		}
		//Process isChained attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_ISCHAINED_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				isChained = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				isChained = false;
			}
		}
		//Process isMandatory attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_ISMANDATORY_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				isMandatory = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				isMandatory = false;
			}
		}
		//Process isAsyncOnly attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_ISASYNCONLY_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				isAsyncOnly = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				isAsyncOnly = false;
			}
		}
		//Process autostartNode attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_AUTOSTARTNODE_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				autostartNode = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				autostartNode = false;
			}
		}
		//Process resetInOperational attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_RESETINOPERATIONAL_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				resetInOperational = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				resetInOperational = false;
			}
		}
		//Process verifyAppSwVersion attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_VERIFYAPPSWVERSION_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				verifyAppSwVersion = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				verifyAppSwVersion = false;
			}
		}
		//Process autoAppSwUpdateAllowed attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_AUTOAPPSWUPDATEALLOWED_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				autoAppSwUpdateAllowed = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				autoAppSwUpdateAllowed = false;
			}
		}
		//Process verifyDeviceType attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_VERIFYDEVICETYPE_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				verifyDeviceType = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				verifyDeviceType = false;
			}
		}
		//Process verifyVendorId attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_VERIFYVENDORID_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				verifyVendorId = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				verifyVendorId = false;
			}
		}
		//Process verifySerialNumber attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_VERIFYSERIALNUMBER_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				verifySerialNumber = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				verifySerialNumber = false;
			}
		}
		//Process verifyProductCode attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_VERIFYPRODUCTCODE_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				verifyProductCode = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				verifyProductCode = false;
			}
		}
		//Process verifyRevisionNumber attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_VERIFYREVISIONNUMBER_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				verifyRevisionNumber = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				verifyRevisionNumber = false;
			}
		}
		//Process isType1Router attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_ISTYPE1ROUTER_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				isType1Router = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				isType1Router = false;
			}
		}
		//Process isType2Router attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				isType2Router = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				isType2Router = false;
			}
		}
		//Process asyncSlotTimeout attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_MN_ASYNCSLOTTIMEOUT_ATTRIBUTE) == 0)
		{

			istringstream asyncSlotTimeoutString((const char*) xmlValue);
			asyncSlotTimeoutString >> asyncSlotTimeout;
		}
		//Process aSndMaxNumber attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_MN_ASNDMAXNUMBER_ATTRIBUTE) == 0)
		{

			istringstream aSndMaxNumberString((const char*) xmlValue);
			aSndMaxNumberString >> aSndMaxNumber;
		}
		//Process transmitsPRes attribute
		else if (strcmp(((const char*) xmlName), PROJECT_XML_NODE_MN_TRANSMITSPRES_ATTRIBUTE) == 0)
		{
			if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_TRUE) == 0)
			{
				transmitsPRes = true;
			}
			else if (strcmp(((const char*) xmlValue), PROJECT_XML_VALUE_FALSE) == 0)
			{
				transmitsPRes = false;
			}
		}
	}

	if (isChained && isMultiplexed)
	{
		throw ocfmRetCode(OCFM_ERR_CHAINED_AND_MULTIPLEXED);
	}
	else if (isChained && !isMultiplexed)
	{
		stationType = CHAINED;
	}
	else if (!isChained && isMultiplexed)
	{
		stationType = MULTIPLEXED;
	}

	string xdcFileString(xdcFileName);
	string tempPath("");
	//Check if path is relative or absolut
	if(xdcFileString.find_first_of(".") == 0)
	{
		tempPath.append(this->GetProjectPath());
		tempPath.append(xdcFileString.substr(xdcFileString.find_first_of(".") + 1, xdcFileString.length()));
	}
	else
	{
		tempPath.append(xdcFileString);
	}

	if (nodeType == MN)
	{
		if (nodeName == NULL)
		{
			nodeName = new char[strlen((char*) "MN") + ALLOC_BUFFER];
			strcpy((char*) nodeName, (char*) "MN");
			exceptionObj = NewProjectNode(nodeId, MN, nodeName, tempPath.c_str());
		}
		else
		{
			exceptionObj = NewProjectNode(nodeId, MN, nodeName, tempPath.c_str());
		}
	}
	else
	{
		if (nodeName == NULL)
		{
			nodeName = new char[strlen((char*) "CN") + ALLOC_BUFFER];
			strcpy((char*) nodeName, (char*) "CN");
			exceptionObj = NewProjectNode(nodeId, CN, nodeName, tempPath.c_str());
		}
		else
		{
			exceptionObj = NewProjectNode(nodeId, CN, nodeName, tempPath.c_str());
		}
	}

	if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
	{
		delete[] forceCycleValue;
		delete[] xdcFileName;
		delete[] nodeName;
		throw exceptionObj;
	}


	NodeCollection *nodeCollObj = NodeCollection::GetNodeColObjectPointer();
	Node* nodeObj = nodeCollObj->GetNodePtr(nodeType, nodeId);

	nodeObj->SetStationType(stationType);

	//If Node is multiplexed
	if (nodeType == CN && forcedMultiplexedCycle != 0 && stationType == MULTIPLEXED)
	{
		nodeObj->SetForcedCycle(forceCycleValue);
		nodeObj->SetForceCycleFlag(true);
		nodeObj->SetForcedMultiplexedCycle(forcedMultiplexedCycle);
	}
	else if (nodeType == CN && forcedMultiplexedCycle == 0 || stationType == NORMAL || stationType == CHAINED)
	{
		nodeObj->SetForcedCycle("0");
		nodeObj->SetForceCycleFlag(false);
		nodeObj->SetForcedMultiplexedCycle(0);
	}
	if (nodeType == CN)
	{

		nodeObj->SetAutostartNode(autostartNode);
		nodeObj->SetResetInOperational(resetInOperational);
		nodeObj->SetAutoAppSwUpdateAllowed(autoAppSwUpdateAllowed);
		nodeObj->SetVerifyAppSwVersion(verifyAppSwVersion);
		nodeObj->SetVerifyDeviceType(verifyDeviceType);
		nodeObj->SetVerifyProductCode(verifyProductCode);
		nodeObj->SetVerifyRevisionNumber(verifyRevisionNumber);
		nodeObj->SetVerifySerialNumber(verifySerialNumber);
		nodeObj->SetVerifyVendorId(verifyVendorId);
	}
	else
	{
		nodeObj->SetTransmitsPRes(transmitsPRes);
		nodeObj->SetAsyncSlotTimeout(asyncSlotTimeout);
		nodeObj->SetASndMaxNumber(aSndMaxNumber);
	}
	nodeObj->SetIsMandatory(isMandatory);
	nodeObj->SetIsAsyncOnly(isAsyncOnly);
	nodeObj->SetIsType1Router(isType1Router);
	nodeObj->SetIsType2Router(isType2Router);

	delete[] forceCycleValue;
	delete[] xdcFileName;
	delete[] nodeName;
}

void ProjectConfiguration::ProcessPath(xmlTextReaderPtr xmlReader)
{
	const xmlChar *name = xmlTextReaderConstName(xmlReader);
	const xmlChar *value = xmlTextReaderConstValue(xmlReader);

	if (0 == strcmp((char*) name, PROJECT_XML_PATH_ID_ATTRIBUTE))
	{
		string id ((const char*) value);
		if (id.compare(PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE) == 0)
		{
			xmlTextReaderMoveToNextAttribute(xmlReader);
			value = xmlTextReaderConstValue(xmlReader);
			name = xmlTextReaderConstName(xmlReader);

			string pathValue((const char*) value);
			this->SetDefaultOutputPath(pathValue);

		}
		else if (id.compare(PROJECT_XML_PATH_TXT2CDC_ATTRIBUTE) == 0)
		{
			xmlTextReaderMoveToNextAttribute(xmlReader);
			value = xmlTextReaderConstValue(xmlReader);
			name = xmlTextReaderConstName(xmlReader);

			string pathValue((const char*) value);
			this->SetPathTxt2CdcExecutable(pathValue);
		}
	}
}

void ProjectConfiguration::ProcessNetworkConfiguration(xmlTextReaderPtr xmlReader)
{
	const xmlChar *name = xmlTextReaderConstName(xmlReader);
	const xmlChar *value = xmlTextReaderConstValue(xmlReader);

	stringstream convert;
	UINT32 tempVal = 0;

	if (0 == strcmp((char*) name, PROJECT_XML_NETWORK_CYCLE_TIME_ATTRIBUTE))
	{
		convert << value;
		convert >> tempVal;
		this->SetCycleTime(tempVal);
		//0x1006
	}
	else if (0 == strcmp((char*) name, PROJECT_XML_NETWORK_ASYNC_MTU_ATTRIBUTE))
	{
		convert << value;
		convert >> tempVal;
		this->SetAsyncMTU(tempVal);
		//0x1F98/0x8
	}
	else if (0 == strcmp((char*) name, PROJECT_XML_NETWORK_MULTIPLEXED_CYCLE_LENGTH_ATTRIBUTE))
	{
		convert << value;
		convert >> tempVal;
		this->SetMultiplexedCycleLength(tempVal);
		//0x1F98/0x7
	}
	else if (0 == strcmp((char*) name, PROJECT_XML_NETWORK_PRESCALER_ATTRIBUTE))
	{
		convert << value;
		convert >> tempVal;
		this->SetPrescaler(tempVal);
		//0x1F98/0x9
	}
}

//Synchronize 1F98/7 between MN and all CNs
void ProjectConfiguration::SynchronizeMultiplexingCycleLength(void)
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

	Index* indexObj = indexCollObj->GetIndex(indexPos);
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
				indexObj = indexCollObj->GetIndex(indexPos);
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

	Index* indexObj = indexCollObj->GetIndex(indexPos);
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

	Index* indexObj = indexCollObj->GetIndex(indexPos);
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

	Index* indexObj = indexCollObj->GetIndex(indexPos);
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
			//Set ProjectConfiguration multiplexedCycleLength
			convertString << actualValue;
			convertString >> this->asyncMTU;
		}
	}
}