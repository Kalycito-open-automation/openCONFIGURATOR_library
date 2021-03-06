/************************************************************************
\file	ProjectConfiguration.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	11.10.2013
************************************************************************/

#include "../Include/ProjectConfiguration.h"
#include "../Include/Logging.h"
#include "../Include/Result.h"
#include "../Include/BoostShared.h"
#include "../Include/NodeApi.h"

#include <iostream>
#include <map>
#include <algorithm>
#include <boost/algorithm/string/replace.hpp>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>
#include <libxml/tree.h>
#include <libxml/xpathInternals.h>
#include <libxml/uri.h>

using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::Library::Utilities;
using namespace openCONFIGURATOR::GUI::ProjectFile::ViewType;
using namespace openCONFIGURATOR::GUI::ProjectFile::Setting;


const string PROJECT_XML_ROOT_ELEMENT = "openCONFIGURATORProject";
const string PROJECT_XML_VERSION_ATTRIBUTE = "version";

const string PROJECT_XML_PATH_ELEMENT = "Path";
const string PROJECT_XML_PATH_ID_ATTRIBUTE = "id";
const string PROJECT_XML_PATH_PATH_ATTRIBUTE = "path";
const string PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE = "defaultOutputPath";

const string PROJECT_XML_GENERATOR_ELEMENT = "Generator";
const string PROJECT_XML_GENERATOR_VENDOR_ATTRIBUTE = "vendor";
const string PROJECT_XML_GENERATOR_TOOLNAME_ATTRIBUTE = "toolName";
const string PROJECT_XML_GENERATOR_TOOLVERSION_ATTRIBUTE = "toolVersion";
const string PROJECT_XML_GENERATOR_CREATEDON_ATTRIBUTE = "createdOn";
const string PROJECT_XML_GENERATOR_MODIFIEDON_ATTRIBUTE = "modifiedOn";
const string PROJECT_XML_GENERATOR_MODIFIEDBY_ATTRIBUTE = "modifiedBy";

const string PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT = "ProjectConfiguration";
const string PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE = "activeAutoGenerationSetting";
const string PROJECT_XML_PROJECT_CONFIGURATION_PROJECT_NAME_ATTRIBUTE = "name";

const string PROJECT_XML_PROJECT_PATHSETTINGS_ELEMENT = "PathSettings";
const string PROJECT_XML_IDECONFIGURATION_ELEMENT = "IDEConfiguration";
const string PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_ELEMENT = "ViewSettings";
const string PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_TYPE_ATTRIBUTE = "type";
const string PROJECT_XML_IDECONFIGURATION_ACTIVE_VIEW_SETTING_ATTRIBUTE = "activeViewSetting";

const string PROJECT_XML_SETTING_ELEMENT = "Setting";
const string PROJECT_XML_SETTING_NAME_ATTRIBUTE = "name";
const string PROJECT_XML_SETTING_VALUE_ATTRIBUTE = "value";

const string PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT = "AutoGenerationSettings";
const string PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT = "NetworkConfiguration";
const string PROJECT_XML_NODE_COLLECTION_ELEMENT = "NodeCollection";
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

const string PROJECT_AUTOGEN_SETTING_ALL = "all";
const string PROJECT_AUTOGEN_SETTING_NONE = "none";

const string PROJECT_VIEW_SETTING_BASIC = "BASIC";
const string PROJECT_VIEW_SETTING_ADVANCED = "ADVANCED";

ProjectConfiguration ProjectConfiguration::instance;

ProjectConfiguration::ProjectConfiguration() :
	initialized(false),
	alreadysaved(false),
	projectFile(),
	projectPath(),
	generateMNOBD(true),
	creationDate(GetCurrentDateTime()),
	defaultOutputPath(),
	cycleTime(),
	asyncMTU(),
	multiplexedCycleLength(),
	prescaler(),
	activeViewSettingType(BASIC),
	viewSettings(),
	pathSettings(),
	activeAutogenerationSettingsID(PROJECT_AUTOGEN_SETTING_ALL),
	autogenerationSettings()
{}

void ProjectConfiguration::ResetConfiguration()
{
	initialized = false;
	alreadysaved = false;
	projectFile = "";
	defaultOutputPath.clear();
	projectPath.clear();
	SetCreationDate(GetCurrentDateTime());

	generateMNOBD = true;

	cycleTime = boost::none;
	asyncMTU = boost::none;
	multiplexedCycleLength = boost::none;
	prescaler = boost::none;

	activeViewSettingType = BASIC;
	activeAutogenerationSettingsID = PROJECT_AUTOGEN_SETTING_ALL;

	viewSettings.clear();
	autogenerationSettings.clear();
	pathSettings.clear();

}

ProjectConfiguration::~ProjectConfiguration()
{

}

ProjectConfiguration& ProjectConfiguration::GetInstance()
{
	return ProjectConfiguration::instance;
}

const boost::filesystem::path& ProjectConfiguration::GetDefaultOutputPath() const
{
	return defaultOutputPath;
}

const std::string& ProjectConfiguration::GetCreationDate(void) const
{
	return creationDate;
}

void ProjectConfiguration::SetCreationDate(const std::string& creationDate)
{
	this->creationDate = creationDate;
}

void ProjectConfiguration::SetDefaultOutputPath(const boost::filesystem::path& defaultOutputPath)
{
	this->defaultOutputPath = defaultOutputPath;
}

bool ProjectConfiguration::IsInitialized() const
{
	return initialized;
}

void ProjectConfiguration::SetInitialized(bool initialized)
{
	this->initialized = initialized;
}

bool ProjectConfiguration::IsAlreadySaved(void) const
{
	return alreadysaved;
}
void ProjectConfiguration::SetAlreadySaved(bool saved)
{
	this->alreadysaved = saved;
}

bool ProjectConfiguration::GetGenerateMNOBD(void) const
{
	return generateMNOBD;
}

void ProjectConfiguration::SetGenerateMNOBD(bool generateMNOBD)
{
	this->generateMNOBD = generateMNOBD;
}

const boost::filesystem::path& ProjectConfiguration::GetProjectPath(void) const
{
	return projectPath;
}

void ProjectConfiguration::SetProjectPath(const boost::filesystem::path& projectPath)
{
	this->projectPath = projectPath;
}

const std::string& ProjectConfiguration::GetProjectFile(void) const
{
	return projectFile;
}

void ProjectConfiguration::SetProjectFile(const std::string& projectFile)
{
	this->projectFile = projectFile;
}

const boost::optional<UINT32>& ProjectConfiguration::GetCycleTime() const
{
	return this->cycleTime;
}

void ProjectConfiguration::SetCycleTime(UINT32 cycleTime)
{
	this->cycleTime = cycleTime;
}

const boost::optional<UINT32>& ProjectConfiguration::GetAsyncMTU() const
{
	return this->asyncMTU;
}

void ProjectConfiguration::SetAsyncMTU(UINT32 asyncMTU)
{
	this->asyncMTU = asyncMTU;
}

const boost::optional<UINT32>& ProjectConfiguration::GetMultiplexedCycleLength() const
{
	return this->multiplexedCycleLength;
}

void ProjectConfiguration::SetMultiplexedCycleLength(UINT32 multiplexedCycleLength)
{
	this->multiplexedCycleLength = multiplexedCycleLength;
}

const boost::optional<UINT32>& ProjectConfiguration::GetPrescaler() const
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
				xmlTextReaderClose(xmlReader);
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
		xmlTextReaderClose(xmlReader);
		xmlCleanupParser();
		xmlMemoryDump();

		this->OverrideNetworkConfiguration();
	}
	catch (ocfmRetCode& exceptionThrown)
	{
		return exceptionThrown;
	}
	this->initialized = true;
	this->alreadysaved = true;
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
					        && xmlAttributeName == PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE)
					{
						this->activeAutogenerationSettingsID = xmlAttributeValue;
						if(this->activeAutogenerationSettingsID == PROJECT_AUTOGEN_SETTING_ALL)
							SetGenerateMNOBD(true);
					}
				}
			}
		}
		else if (xmlName == PROJECT_XML_GENERATOR_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				ProcessGenerator(xmlReader);
			}
		}
		else if (xmlName == PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				ProcessAutogenerationSettings(xmlReader);
			}
		}
		else if (xmlName == PROJECT_XML_IDECONFIGURATION_ELEMENT)
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
					        && xmlAttributeName == PROJECT_XML_IDECONFIGURATION_ACTIVE_VIEW_SETTING_ATTRIBUTE)
					{
						if(xmlAttributeValue == PROJECT_VIEW_SETTING_BASIC)
							this->activeViewSettingType = BASIC;
						else if(xmlAttributeValue == PROJECT_VIEW_SETTING_ADVANCED)
							this->activeViewSettingType = ADVANCED;
					}
				}
			}
		}
		else if (xmlName == PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_ELEMENT)
		{
			if (xmlTextReaderHasAttributes(xmlReader) == 1)
			{
				ProcessViewSettings(xmlReader);
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
				ProcessProjectNode(xmlReader);
			}
		}
	}
}

void ProjectConfiguration::ProcessAutogenerationSettings(xmlTextReaderPtr xmlReader)
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
		        && xmlAttributeName == PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT)
		{
			currentAutogenerationSetting = xmlAttributeValue;
		}
	}
	//Process the AutogenerationSetting
	while (xmlTextReaderNodeType(xmlReader) != 1)
		xmlTextReaderRead(xmlReader);

	xmlNodePtr node = xmlTextReaderCurrentNode(xmlReader);
	while (node != NULL)
	{
		string autogenerationSettingName;
		string autogenerationSettingValue;
		if (xmlStrEqual(node->content, (xmlChar*) PROJECT_XML_SETTING_ELEMENT.c_str()) == 0)
		{
			xmlAttr* attribute = node->properties;
			while (attribute && attribute->name && attribute->children)
			{
				if(xmlStrEqual(attribute->name, (xmlChar*) PROJECT_XML_SETTING_NAME_ATTRIBUTE.c_str()) == 1)
				{
				xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
				autogenerationSettingName = (char*) value;
				xmlFree(value);
				}
				else if(xmlStrEqual(attribute->name, (xmlChar*) PROJECT_XML_SETTING_VALUE_ATTRIBUTE.c_str()) == 1)
				{
				xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
				autogenerationSettingValue = (char*) value;
				xmlFree(value);
				}
				attribute = attribute->next;
			}
			this->AddAutoGenerationSetting(currentAutogenerationSetting, autogenerationSettingName, autogenerationSettingValue);
		}
		node = xmlNextElementSibling(node);
	}

}

void ProjectConfiguration::ProcessViewSettings(xmlTextReaderPtr xmlReader)
{
	ViewType currentViewSetting;
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
		        && xmlAttributeName == PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_TYPE_ATTRIBUTE)
		{
			if(xmlAttributeValue == PROJECT_VIEW_SETTING_BASIC)
				currentViewSetting = BASIC;
			else if(xmlAttributeValue == PROJECT_VIEW_SETTING_ADVANCED)
				currentViewSetting = ADVANCED;
		}
	}

	//Process the ViewSetting

	while (xmlTextReaderNodeType(xmlReader) != 1)
		xmlTextReaderRead(xmlReader);

	xmlNodePtr node = xmlTextReaderCurrentNode(xmlReader);
	while (node != NULL)
	{
		string settingNameStr;
		string settingValueStr;
		if (xmlStrEqual(node->name, (xmlChar*) PROJECT_XML_SETTING_ELEMENT.c_str()) == 1)
		{
			xmlAttr* attribute = node->properties;
			while (attribute && attribute->name && attribute->children)
			{
				if(xmlStrEqual(attribute->name, (xmlChar*) PROJECT_XML_SETTING_NAME_ATTRIBUTE.c_str()) == 1)
				{
				xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
				settingNameStr = (char*) value;
				xmlFree(value);
				}
				else if(xmlStrEqual(attribute->name, (xmlChar*) PROJECT_XML_SETTING_VALUE_ATTRIBUTE.c_str()) == 1)
				{
				xmlChar* value = xmlNodeListGetString(node->doc, attribute->children, 1);
				settingValueStr = (char*) value;
				xmlFree(value);
				}
				attribute = attribute->next;
			}
			this->AddViewSetting(currentViewSetting, settingNameStr, settingValueStr);
		}
		node = xmlNextElementSibling(node);
	}
}

void ProjectConfiguration::ProcessGenerator(xmlTextReaderPtr xmlReader)
{
	while (xmlTextReaderMoveToNextAttribute(xmlReader))
	{
		const string xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
		                                ? (const char*) xmlTextReaderConstName(xmlReader)
		                                : "";
		const string xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
		                                 ? (const char*) xmlTextReaderConstValue(xmlReader)
		                                 : "";

		if (xmlAttributeName == PROJECT_XML_GENERATOR_TOOLNAME_ATTRIBUTE)
{
			//Process Generator attribute toolName
		}
		else if (xmlAttributeName == PROJECT_XML_GENERATOR_TOOLVERSION_ATTRIBUTE)
		{
			//Process Generator attribute toolVersion
		}
		else if (xmlAttributeName == PROJECT_XML_GENERATOR_CREATEDON_ATTRIBUTE)
		{
			SetCreationDate(xmlAttributeValue);
		}
		else if (xmlAttributeName == PROJECT_XML_GENERATOR_MODIFIEDBY_ATTRIBUTE)
		{
			//Process Generator attribute modifiedBy
		}
		else if (xmlAttributeName == PROJECT_XML_GENERATOR_MODIFIEDON_ATTRIBUTE)
		{
			//Process Generator attribute modifiedOn
		}
	}
}

void ProjectConfiguration::ProcessProjectNode(xmlTextReaderPtr xmlReader)
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
	//Replace encoded whitespaces for XDC paths
	string xdcPathString = xdcPath.generic_string();
	boost::replace_all(xdcPathString, "%20", " ");
	xdcPath = xdcPathString;
	LOG_DEBUG() << "XDC-Path for node " << nodeId.get() << ": " << xdcPath.generic_string<string>();

	// Set a default node name if it was omitted
	if ((!nodeName || nodeName.get().empty()) && nodeId.get() == MN_NODEID)
		nodeName = "MN";
	else if ((!nodeName || nodeName.get().empty()) && nodeId.get() != MN_NODEID)
		nodeName = "CN_" + boost::lexical_cast<string>(nodeId.get());

	// Create node
	exceptionObj = NewProjectNode(nodeId.get(),
	                              (nodeId.get() == MN_NODEID) ? MN : CN,
	                              nodeName.get().c_str(),
	                              xdcPath.generic_string().c_str());
	if (exceptionObj.getErrorCode() != OCFM_ERR_SUCCESS)
		throw exceptionObj;

	//Set XDD / XDC paths
	Node& node = NodeCollection::GetNodeColObjectPointer()->GetNodeRef(nodeId.get());
	NodeType type = (nodeId.get() == MN_NODEID) ? MN : CN;

	//Set the paths for the project nodes
	node.SetXdcPath(xdcPath);
	if (type == CN)
	{
		boost::filesystem::path path(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceImport" + PATH_SEPARATOR + xdcPath.filename().generic_string().substr(0, xdcPath.filename().generic_string().find_last_of("_")));
		boost::filesystem::path xddPath = path.replace_extension("xdd");
		boost::filesystem::path xdcPath = path.replace_extension("xdc");

		if (boost::filesystem::exists(xddPath))
		{
			node.SetXddPath(xddPath);
		}
		else if (boost::filesystem::exists(xdcPath))
		{
			node.SetXddPath(xdcPath);
		}
		else
		{
			node.SetXddPath("");
		}
	}
	else
	{
		boost::filesystem::path path(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceImport" + PATH_SEPARATOR + xdcPath.stem().generic_string());
		boost::filesystem::path xddPath = path.replace_extension("xdd");
		boost::filesystem::path xdcPath = path.replace_extension("xdc");

		if (boost::filesystem::exists(xddPath))
		{
			node.SetXddPath(xddPath);
		}
		else if (boost::filesystem::exists(xdcPath))
		{
			node.SetXddPath(xdcPath);
		}
		else
		{
			node.SetXddPath("");
		}
	}


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
			nodeObj.SetForcedCycle(boost::lexical_cast<string>(forcedMultiplexedCycle.get_value_or(0)).c_str());
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
		string pathId = xmlAttributeValue;

		xmlTextReaderMoveToNextAttribute(xmlReader);
		xmlAttributeName = ((const char*) xmlTextReaderConstName(xmlReader))
		                   ? (const char*) xmlTextReaderConstName(xmlReader)
		                   : "";
		xmlAttributeValue = ((const char*) xmlTextReaderConstValue(xmlReader))
		                    ? (const char*) xmlTextReaderConstValue(xmlReader)
		                    : "";
		if (!xmlAttributeName.empty()
		        && xmlAttributeName == PROJECT_XML_PATH_PATH_ATTRIBUTE
		        && !xmlAttributeValue.empty())
		{
			this->AddPath(pathId, string(xmlAttributeValue));
			if(pathId == PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE)
				this->SetDefaultOutputPath(boost::filesystem::path(xmlAttributeValue));
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
void ProjectConfiguration::SynchronizeMultiplexedCycleLength()
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
void ProjectConfiguration::SynchronizeCycleTime()
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
void ProjectConfiguration::SynchronizeAsyncMtu()
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
void ProjectConfiguration::SynchronizePrescaler()
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
	        << hex
	        << showbase
	        << "Overriding configuration for node " << node.GetNodeId()
	        << ", index " << index << "/" << subIndex << ".";
	boost::optional<T> currActualValue = node.GetActualValue<T>(index, subIndex);
	if (currActualValue)
	{
		// There is an actualValue for the index/subIndex, it
		// overrides the project-config
		string projectConfigValueStr = projectConfigValue
		                               ? boost::lexical_cast<string>(projectConfigValue.get())
		                                    : "NULL";
		LOG_DEBUG()
		        << hex
		        << showbase
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
			string currActualValueStr = currActualValue
			                            ? boost::lexical_cast<string>(currActualValue.get())
			                                 : "NULL";
			LOG_DEBUG()
			        << hex
			        << showbase
			        << "Overwrote actualValue '" << currActualValueStr
			        << "' of node with project-configuration value '"
			        << projectConfigValue.get() << "'.";
		}
		else
		{
			LOG_DEBUG() << "Nothing to override.";
		}
	}


	/*	Index& indexRef = node.GetIndexCollection()->GetIndexRef(index);
	SubIndex* subIndexPtr = indexRef.GetSubIndexPtr(subIndex);
	const char* actualValue = (subIndexPtr == NULL)
		? indexRef.GetActualValue()
		: subIndexPtr->GetActualValue();
		stringstream converter;
	if (actualValue != NULL && strlen(actualValue) != 0)
	{
		// There is an actualValue for the index/subIndex, it
		// overrides the project-config
		string actualValueStr(actualValue);
		if (boost::algorithm::starts_with(actualValueStr, "0x"))
				converter << hex << actualValueStr.substr(2);
		else
				converter << boolalpha << actualValueStr;
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
				converter << boolalpha << projectConfigValue.get();
			if (subIndexPtr != NULL)
				subIndexPtr->SetActualValue(converter.str().c_str());
			else
				indexRef.SetActualValue(converter.str().c_str());
		}
	}
	*/
}

openCONFIGURATOR::Library::ErrorHandling::Result ProjectConfiguration::SaveProject(void)
{
	try
	{
		if (!IsAlreadySaved())
		{
			//Create project folder structure if not existing
			CreateProjectFolder();
		}
		//Write or Update MN XDD/XDC files if existing
		WriteMNXDC();

		//Write or Update CN XDD/XDC files if existing
		WriteCNXDCs();


		WriteProjectFile();

		//	//Update existing valid project file
		Result result = ValidateProjectFile(projectPath.generic_string() +  PATH_SEPARATOR + projectFile);
		if (!result.IsSuccessful())
			throw result;


		//Update XDCs indices with actual values
		UpdateNodeConfigurations();

		SetAlreadySaved(true);

		return Result();
	}
	catch (const Result& result)
	{
		return result;
	}
}

void ProjectConfiguration::CreateProjectFolder(void)
{
	try
	{
		//Create project folder structure
		boost::filesystem::path xdd_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceImport");
		boost::filesystem::path xdc_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceConfiguration");
		if (ProjectConfiguration::GetInstance().GetDefaultOutputPath().empty())
		{
			ProjectConfiguration::GetInstance().SetDefaultOutputPath(boost::filesystem::path(GetProjectPath().generic_string() + PATH_SEPARATOR + "output"));
		}
		boost::filesystem::path output_dir(ProjectConfiguration::GetInstance().GetDefaultOutputPath());

		boost::filesystem::create_directories(xdd_dir);
		boost::filesystem::create_directories(xdc_dir);
		boost::filesystem::create_directories(output_dir);
	}
	catch (exception& ex)
	{
		throw Result(UNHANDLED_EXCEPTION, ex.what());
	}
}

void ProjectConfiguration::WriteMNXDC(void)
{
	try
	{
		NodeCollection* nodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (nodeCollection->GetNodeRef(240).GetXdcPath().empty())
		{
			boost::filesystem::path original_mn_xdd_path = nodeCollection->GetNodeRef(240).GetXddPath();
			//Create project folder and copy MN XDD
			boost::filesystem::path xdd_mn_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceImport" + PATH_SEPARATOR + original_mn_xdd_path.filename().string());
			boost::filesystem::path xdc_mn_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceConfiguration" + PATH_SEPARATOR + original_mn_xdd_path.stem().string() + ".xdc");

			boost::filesystem::copy_file(original_mn_xdd_path, xdd_mn_dir, boost::filesystem::copy_option::overwrite_if_exists);
			boost::filesystem::copy_file(original_mn_xdd_path, xdc_mn_dir, boost::filesystem::copy_option::overwrite_if_exists);

			nodeCollection->GetNodeRef(240).SetXddPath(xdd_mn_dir);
			nodeCollection->GetNodeRef(240).SetXdcPath(xdc_mn_dir);
		}
	}
	catch (exception& ex)
	{
		throw Result(UNHANDLED_EXCEPTION, ex.what());
	}
}

void ProjectConfiguration::WriteCNXDCs(void)
{
	try
	{
		NodeCollection* nodeCollection = NodeCollection::GetNodeColObjectPointer();
		if (ProjectConfiguration::GetInstance().IsAlreadySaved())
		{
			boost::filesystem::path xdd_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceImport");
			boost::filesystem::path xdc_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceConfiguration");

			boost::filesystem::directory_iterator end_iter;

			list<boost::filesystem::path> xddPathVector;
			list<boost::filesystem::path> xdcPathVector;

			//list all existing files
			if (exists(xdd_dir) && is_directory(xdd_dir))
			{
				for (boost::filesystem::directory_iterator dir_iter(xdd_dir) ; dir_iter != end_iter ; ++dir_iter)
				{
					if (boost::filesystem::is_regular_file(dir_iter->status()))
					{
						xddPathVector.push_back(*dir_iter);
					}
				}
			}

			if (exists(xdc_dir) && is_directory(xdc_dir))
			{
				for (boost::filesystem::directory_iterator dir_iter(xdc_dir) ; dir_iter != end_iter ; ++dir_iter)
				{
					if (boost::filesystem::is_regular_file(dir_iter->status()))
					{
						xdcPathVector.push_back(*dir_iter);
					}
				}
			}

			//Determine no longer needed files
			for (int i = 0; i < nodeCollection->GetNumberOfNodes(); i++)
			{
				Node* nodeObj = nodeCollection->GetNodebyColIndex(i);
				bool deleteFlag = false;
				list<boost::filesystem::path>::iterator toDelete;
				//remove files needed from the list of all project files
				for (toDelete = xddPathVector.begin() ; toDelete != xddPathVector.end(); ++toDelete)
				{
					if (boost::filesystem::equivalent(nodeObj->GetXddPath(), *toDelete))
					{
						deleteFlag = true;
						break;
					}
				}
				if (deleteFlag)
					xddPathVector.erase(toDelete);

				deleteFlag = false;
				for (toDelete = xdcPathVector.begin() ; toDelete != xdcPathVector.end(); ++toDelete)
				{
					if (boost::filesystem::equivalent(nodeObj->GetXdcPath(), *toDelete))
					{
						deleteFlag = true;
						break;
					}
				}
				if (deleteFlag)
					xdcPathVector.erase(toDelete);
			}

			//Delete no longer needed configuration files
			for (list<boost::filesystem::path>::iterator it = xddPathVector.begin() ; it != xddPathVector.end(); ++it)
			{
				LOG_INFO() << "Unused XDD within deviceImport folder: " << it->generic_string();
				cout << "Unused XDD within deviceImport folder: " << it->generic_string() << endl;
			}
			for (list<boost::filesystem::path>::iterator it = xdcPathVector.begin() ; it != xdcPathVector.end(); ++it)
			{
				boost::filesystem::remove(*it);
			}
		}
		//Copy CN xdc if not existing
		for (int i = 0; i < nodeCollection->GetNumberOfNodes(); i++)
		{
			Node* nodeObj = nodeCollection->GetNodebyColIndex(i);
			if (nodeObj->GetNodeType() == CN && nodeObj->GetXdcPath().empty())
			{
				boost::filesystem::path original_xdd_file = nodeObj->GetXddPath();

				boost::filesystem::path project_xdd_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceImport" + PATH_SEPARATOR + original_xdd_file.filename().string());
				boost::filesystem::copy_file(original_xdd_file, project_xdd_dir, boost::filesystem::copy_option::overwrite_if_exists);
				nodeObj->SetXddPath(project_xdd_dir);

				stringstream nodeIdStream;
				nodeIdStream << nodeObj->GetNodeId();
				boost::filesystem::path project_xdc_dir(GetProjectPath().generic_string() + PATH_SEPARATOR + "deviceConfiguration" + PATH_SEPARATOR + original_xdd_file.stem().string() + "_" + nodeIdStream.str() + ".xdc");
				boost::filesystem::copy_file(original_xdd_file, project_xdc_dir, boost::filesystem::copy_option::overwrite_if_exists);
				nodeObj->SetXdcPath(project_xdc_dir);

			}
		}
	}
	catch (exception& ex)
	{
		throw Result(UNHANDLED_EXCEPTION, ex.what());
	}
}

void ProjectConfiguration::WriteProjectFile(void)
{
	try
	{
		NodeCollection* nodeCollection = NodeCollection::GetNodeColObjectPointer();
		string projectPathStr = ProjectConfiguration::GetInstance().GetProjectPath().generic_string();

		boost::filesystem::path mnXdcPath = nodeCollection->GetNodeRef(240).GetXdcPath();
		boost::filesystem::path projectFilePath(projectPathStr + PATH_SEPARATOR + GetProjectFile());

		//Strip project path if necessary
		string mnXdcPathString = mnXdcPath.generic_string();
		if(boost::starts_with(mnXdcPath.generic_string(), projectPathStr))
			mnXdcPathString  = mnXdcPath.generic_string().substr(projectPathStr.size() + 1, mnXdcPath.generic_string().size());

		//write of project file
		xmlTextWriterPtr writer = xmlNewTextWriterFilename(projectFilePath.generic_string().c_str(), 0);
		xmlTextWriterSetIndent(writer, 1);
		xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_ROOT_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "http://sourceforge.net/projects/openconf/configuration");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:oc", BAD_CAST "http://sourceforge.net/projects/openconf/configuration");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xsi:schemaLocation", BAD_CAST "http://sourceforge.net/projects/openconf/configuration openCONFIGURATOR.xsd");

		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_GENERATOR_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_VENDOR_ATTRIBUTE.c_str(), BAD_CAST "Kalycito Infotech Private Limited & Bernecker + Rainer Industrie Elektronik Ges.m.b.H.");
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_TOOLNAME_ATTRIBUTE.c_str(), BAD_CAST "openCONFIGURATOR");
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_TOOLVERSION_ATTRIBUTE.c_str(), BAD_CAST "1.4.0");
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_CREATEDON_ATTRIBUTE.c_str(), BAD_CAST GetCreationDate().c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_MODIFIEDON_ATTRIBUTE.c_str(), BAD_CAST GetCurrentDateTime().c_str());
		xmlTextWriterEndElement(writer);
		if (!viewSettings.empty())
		{
			xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_ELEMENT.c_str());
			if (activeViewSettingType == BASIC)
				xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_ACTIVE_VIEW_SETTING_ATTRIBUTE.c_str(), BAD_CAST "BASIC");
			else
				xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_ACTIVE_VIEW_SETTING_ATTRIBUTE.c_str(), BAD_CAST "ADVANCED");
			xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_ELEMENT.c_str());
			xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_TYPE_ATTRIBUTE.c_str(), BAD_CAST "BASIC");
			for (std::vector<ViewSetting>::iterator it = viewSettings.begin(); it != viewSettings.end(); ++it)
			{
				if (it->GetViewType() == BASIC)
				{
					xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_SETTING_ELEMENT.c_str());
					xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_NAME_ATTRIBUTE.c_str(), BAD_CAST it->GetName().c_str());

					xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_VALUE_ATTRIBUTE.c_str(), BAD_CAST it->GetValue().c_str());
					xmlTextWriterEndElement(writer);
				}

			}
			xmlTextWriterEndElement(writer);
			xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_ELEMENT.c_str());
			xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_TYPE_ATTRIBUTE.c_str(), BAD_CAST "ADVANCED");
			for (std::vector<ViewSetting>::iterator it = viewSettings.begin(); it != viewSettings.end(); ++it)
			{
				if (it->GetViewType() == ADVANCED)
				{
					xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_SETTING_ELEMENT.c_str());
					xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_NAME_ATTRIBUTE.c_str(), BAD_CAST it->GetName().c_str());

					xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_VALUE_ATTRIBUTE.c_str(), BAD_CAST it->GetValue().c_str());
					xmlTextWriterEndElement(writer);
				}

			}
			xmlTextWriterEndElement(writer);
			xmlTextWriterEndElement(writer);
		}

		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE.c_str(), BAD_CAST activeAutogenerationSettingsID.c_str());
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_PROJECT_PATHSETTINGS_ELEMENT.c_str());
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_PATH_ELEMENT.c_str());

		//Strip Project path
		string strDefaultOutputPath = this->defaultOutputPath.generic_string();
		if(boost::starts_with(this->defaultOutputPath.generic_string(), projectPathStr))
			strDefaultOutputPath  = this->defaultOutputPath.generic_string().substr(projectPathStr.size() + 1, this->defaultOutputPath.generic_string().size());

		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_ID_ATTRIBUTE.c_str(), BAD_CAST "defaultOutputPath");
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_PATH_ATTRIBUTE.c_str(), BAD_CAST url_encode(strDefaultOutputPath).c_str());
		xmlTextWriterEndElement(writer);

		for (vector<Path>::iterator it = pathSettings.begin(); it != pathSettings.end(); ++it)
		{
			if(it->GetName().compare("defaultOutputPath") != 0)
			{
				xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_PATH_ELEMENT.c_str());
				xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_ID_ATTRIBUTE.c_str(), BAD_CAST it->GetName().c_str());

				xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_PATH_ATTRIBUTE.c_str(), BAD_CAST url_encode(it->GetValue()).c_str());
				xmlTextWriterEndElement(writer);
			}
		}
		xmlTextWriterEndElement(writer);

		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_ID_ATTRIBUTE.c_str(), BAD_CAST "all");
		xmlTextWriterEndElement(writer);
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_ID_ATTRIBUTE.c_str(), BAD_CAST "none");
		xmlTextWriterEndElement(writer);
		xmlTextWriterEndElement(writer);

		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT.c_str());
		if (nodeCollection->GetNodeRef(240).GetActualValue<string>(0x1006, 0))
			xmlTextWriterWriteFormatAttribute(writer, BAD_CAST PROJECT_XML_NETWORK_CYCLE_TIME_ATTRIBUTE.c_str(), "%d", nodeCollection->GetNodeRef(240).GetActualValue<UINT32>(0x1006, 0).get());
		if (nodeCollection->GetNodeRef(240).GetActualValue<string>(0x1F98, 8))
			xmlTextWriterWriteFormatAttribute(writer, BAD_CAST PROJECT_XML_NETWORK_ASYNC_MTU_ATTRIBUTE.c_str(), "%d", nodeCollection->GetNodeRef(240).GetActualValue<UINT32>(0x1F98, 8).get());
		if (nodeCollection->GetNodeRef(240).GetActualValue<string>(0x1F98, 7))
			xmlTextWriterWriteFormatAttribute(writer, BAD_CAST PROJECT_XML_NETWORK_MULTIPLEXED_CYCLE_LENGTH_ATTRIBUTE.c_str(), "%d", nodeCollection->GetNodeRef(240).GetActualValue<UINT32>(0x1F98, 7).get());
		if (nodeCollection->GetNodeRef(240).GetActualValue<string>(0x1F98, 9))
			xmlTextWriterWriteFormatAttribute(writer, BAD_CAST PROJECT_XML_NETWORK_PRESCALER_ATTRIBUTE.c_str(), "%d", nodeCollection->GetNodeRef(240).GetActualValue<UINT32>(0x1F98, 9).get());

		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_NODE_COLLECTION_ELEMENT.c_str());
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_MANAGING_NODE_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_NODEID_ATTRIBUTE.c_str(), BAD_CAST "240");
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_NAME_ATTRIBUTE.c_str(), BAD_CAST nodeCollection->GetNodeRef(240).GetNodeName());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE.c_str(), BAD_CAST url_encode(mnXdcPathString).c_str());

		//if (nodeCollection->GetNodeRef(240).IsAsyncOnly())
		//{
		//	string value = nodeCollection->GetNodeRef(240).IsAsyncOnly().get()
		//		? "true"
		//		: "false";
		//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISASYNCONLY_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
		//}

		//if (nodeCollection->GetNodeRef(240).IsType1Router())
		//{
		//	string value = nodeCollection->GetNodeRef(240).IsType1Router().get()
		//		? "true"
		//		: "false";
		//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE1ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
		//}

		//if (nodeCollection->GetNodeRef(240).IsType2Router())
		//{
		//	string value = nodeCollection->GetNodeRef(240).IsType2Router().get()
		//		? "true"
		//		: "false";
		//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
		//}

		//if (nodeCollection->GetNodeRef(240).TransmitsPRes())
		//{
		//	string value = nodeCollection->GetNodeRef(240).TransmitsPRes().get()
		//		? "true"
		//		: "false";
		//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_MN_TRANSMITSPRES_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
		//}

		//if (nodeCollection->GetNodeRef(240).GetActualValue<string>(0x1F8A, 2))
		//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_MN_ASYNCSLOTTIMEOUT_ATTRIBUTE.c_str(), BAD_CAST nodeCollection->GetNodeRef(240).GetActualValue<UINT32>(0x1F8A, 2).get().c_str());

		//if (nodeCollection->GetNodeRef(240).GetActualValue<string>(0x1F8A, 3))
		//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_MN_ASNDMAXNUMBER_ATTRIBUTE.c_str(), BAD_CAST nodeCollection->GetNodeRef(240).GetActualValue<UINT32>(0x1F8A, 3).get().c_str());


		xmlTextWriterEndElement(writer);

		for (int i = 0; i < nodeCollection->GetNumberOfNodes(); i++)
		{
			Node* nodeObj = nodeCollection->GetNodebyColIndex(i);
			stringstream nodeIdStream;
			nodeIdStream << nodeObj->GetNodeId();

			if (nodeObj->GetNodeType() == CN)
			{
				boost::filesystem::path cnXdcPath = nodeObj->GetXdcPath();
				//Strip project path
				string cnXdcPathString = cnXdcPath.generic_string();
				if(boost::starts_with(cnXdcPath.generic_string(), projectPathStr))
					cnXdcPathString  = cnXdcPath.generic_string().substr(projectPathStr.size() + 1, cnXdcPath.generic_string().size());

				xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_CONTROLLED_NODE_ELEMENT.c_str());
				xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_NODEID_ATTRIBUTE.c_str(), BAD_CAST  nodeIdStream.str().c_str());
				xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_NAME_ATTRIBUTE.c_str(), BAD_CAST nodeObj->GetNodeName());
				xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE.c_str(), BAD_CAST url_encode(cnXdcPathString).c_str());
				if (nodeObj->GetStationType() == CHAINED)
					xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISCHAINED_ATTRIBUTE.c_str(), BAD_CAST PROJECT_XML_VALUE_TRUE.c_str());
				if (nodeObj->GetStationType() == MULTIPLEXED)
				{
					xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISMULTIPLEXED_ATTRIBUTE.c_str(), BAD_CAST PROJECT_XML_VALUE_TRUE.c_str());
					if (nodeObj->GetForceCycleFlag())
					{
						string tempValue = nodeObj->GetForcedCycleValue();
						std::stringstream sstm;

						if(tempValue.substr(0, 2).compare("0x") == 0 || tempValue.substr(0, 2).compare("0X") == 0)
							sstm << HexToInt<unsigned int>(tempValue);
						else
							sstm << tempValue;

						xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_FORCED_MULTIPLEXED_CYCLE_ATTRIBUTE.c_str(), BAD_CAST sstm.str().c_str());
					}

				}

				//if (nodeObj->IsAsyncOnly())
				//{
				//	string value = nodeObj->IsAsyncOnly().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISASYNCONLY_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->IsType1Router())
				//{
				//	string value = nodeObj->IsType1Router().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE1ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->IsType2Router())
				//{
				//	string value = nodeObj->IsType2Router().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->IsMandatory())
				//{
				//	string value = nodeObj->IsMandatory().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetAutostartNode())
				//{
				//	string value = nodeObj->GetAutostartNode().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetResetInOperational())
				//{
				//	string value = nodeObj->GetResetInOperational().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetVerifyAppSwVersion())
				//{
				//	string value = nodeObj->GetVerifyAppSwVersion().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetAutoAppSwUpdateAllowed())
				//{
				//	string value = nodeObj->GetAutoAppSwUpdateAllowed().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetVerifyDeviceType())
				//{
				//	string value = nodeObj->GetVerifyDeviceType().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetVerifyVendorId())
				//{
				//	string value = nodeObj->GetVerifyVendorId().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetVerifyRevisionNumber())
				//{
				//	string value = nodeObj->GetVerifyRevisionNumber().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetVerifyProductCode())
				//{
				//	string value = nodeObj->GetVerifyProductCode().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				//if (nodeObj->GetVerifySerialNumber())
				//{
				//	string value = nodeObj->GetVerifySerialNumber().get()
				//		? "true"
				//		: "false";
				//	xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_NODE_ISTYPE2ROUTER_ATTRIBUTE.c_str(), BAD_CAST value.c_str());
				//}

				xmlTextWriterEndElement(writer);
			}
		}
		xmlTextWriterEndElement(writer);
		xmlTextWriterEndElement(writer);
		xmlTextWriterEndDocument(writer);


		//free memory used by libxml2
		xmlFreeTextWriter(writer);
		xmlCleanupParser();
		xmlMemoryDump();
	}
	catch (ocfmRetCode& ex)
	{
		throw Translate(ex);
	}
}

void ProjectConfiguration::UpdateNodeConfigurations(void)
{
	try
	{
		NodeCollection* nodeCollection = NodeCollection::GetNodeColObjectPointer();
		for (int i = 0; i < nodeCollection->GetNumberOfNodes(); i++)
		{
			Node* nodeObj = nodeCollection->GetNodebyColIndex(i);
			//Validate before updating the XDC
 			Result retValue = Translate(ValidateXDDFile(nodeObj->GetXdcPath().generic_string().c_str()));
			if (!retValue.IsSuccessful())
				throw retValue;

			//Load the nodes XDC file
			xmlDocPtr pDoc = xmlParseFile(nodeObj->GetXdcPath().generic_string().c_str());
			if (pDoc == NULL)
			{
				boost::format formatter(kMsgFileReadFailed);
				formatter % nodeObj->GetXdcPath().generic_string();
				LOG_FATAL() << formatter.str();
				xmlCleanupParser();
				xmlMemoryDump();
				throw Result(FILE_READ_FAILED, formatter.str());
			}

			//Delete all existing Object actual values
			string xpathActual = "//plk:Object[@actualValue]";
			xmlXPathObjectPtr pResultingXPathObjectActual(GetNodeSet(pDoc, xpathActual.c_str()));
			if (pResultingXPathObjectActual)
			{
				xmlNodeSetPtr pNodeSet(pResultingXPathObjectActual->nodesetval);
				for (int i = 0; i < pNodeSet->nodeNr; ++i)
				{
					xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
					xmlRemoveProp(xmlHasProp(pIndexNode, BAD_CAST "actualValue"));
				}
			}

			xpathActual = "//plk:SubObject[@actualValue]";
			xmlXPathObjectPtr pResultingXPathObjectActualSubIndex(GetNodeSet(pDoc, xpathActual.c_str()));
			if (pResultingXPathObjectActualSubIndex)
			{
				xmlNodeSetPtr pNodeSet(pResultingXPathObjectActualSubIndex->nodesetval);
				for (int i = 0; i < pNodeSet->nodeNr; ++i)
				{
					xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
					xmlRemoveProp(xmlHasProp(pIndexNode, BAD_CAST "actualValue"));
				}
			}

			//Traverses all node indices
			IndexCollection* indexColl = nodeObj->GetIndexCollection();
			for (unsigned int i = 0; i < indexColl->Size(); i++)
			{
				Index* index = indexColl->GetIndexByPosition(i);
				if (IsDefaultActualNotEqual(index) && index->GetActualValue() != NULL && strcmp(index->GetActualValue(), "") != 0)
				{
					//Select all the user nodes
					string xpath = "//plk:Object[@index='";
					xpath.append(index->GetIndexValue());
					xpath.append("']");

					xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpath.c_str()));
					if (pResultingXPathObject)
					{
						xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
						for (int i = 0; i < pNodeSet->nodeNr; ++i)
						{
							xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
							xmlNewProp(pIndexNode, BAD_CAST "actualValue", BAD_CAST index->GetActualValue());
							LOG_INFO() << "Save Node: " << nodeObj->GetNodeId() << " Index: " << index->GetIndexValue() << " ActualValue: " << index->GetActualValue() << endl;
						}
					}
					else
					{
						cout << "WARNING: Object(0x" << index->GetIndexValue() << " (" << index->GetActualValue() << ")" << ") on Node(" << nodeObj->GetNodeId() << ")" << " was generated and cannot be stored in the node XDC file." << endl;
					}
					xmlXPathFreeObject(pResultingXPathObject);
				}
				if (index->HasSubIndices())
				{
					for (int j = 0; j < index->GetNumberofSubIndexes(); j++)
					{
						SubIndex* subIndex = index->GetSubIndexByPosition(j);
						if (IsDefaultActualNotEqual(subIndex) && subIndex->GetActualValue() != NULL && strcmp(subIndex->GetActualValue(), "") != 0)
						{
							//alter subindex actual value in xdc
							//Select all the user nodes
							string xpath = "//plk:Object[@index='";
							xpath.append(index->GetIndexValue());
							xpath.append("']");
							xpath.append("/plk:SubObject[@subIndex='");
							xpath.append(subIndex->GetIndexValue());
							xpath.append("']");

							xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpath.c_str()));
							if (pResultingXPathObject)
							{
								xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
								for (int i = 0; i < pNodeSet->nodeNr; ++i)
								{
									xmlNodePtr pSubIndexNode(pNodeSet->nodeTab[i]);
									xmlNewProp(pSubIndexNode, BAD_CAST "actualValue", BAD_CAST subIndex->GetActualValue());
									LOG_INFO() << " Save Node: " << nodeObj->GetNodeId() << " Index: " << index->GetIndexValue() << " Subindex: " << subIndex->GetIndexValue() << " ActualValue: " << subIndex->GetActualValue() << endl;
								}
							}
							else
							{
								cout << "WARNING: Object(0x" << index->GetIndexValue() << "/0x" << subIndex->GetIndexValue() << " (" << subIndex->GetActualValue() << ")" << ") on Node(" << nodeObj->GetNodeId() << ")" << " was generated and cannot be stored in the node XDC file." << endl;
							}
							xmlXPathFreeObject(pResultingXPathObject);
						}
					}
				}
			}
			//Save the nodes XDC file
			xmlSaveFormatFileEnc(nodeObj->GetXdcPath().generic_string().c_str(), pDoc, "UTF-8", 1);
			xmlFreeDoc(pDoc);
			xmlCleanupParser();
			xmlMemoryDump();
		}
	}
	catch (exception& ex)
	{
		throw Result(UNHANDLED_EXCEPTION, ex.what());
	}
}

const xmlXPathObjectPtr ProjectConfiguration::GetNodeSet(const xmlDocPtr doc, const char* xpath)
{
	xmlXPathContextPtr context = xmlXPathNewContext(doc);
	if (context == NULL)
		return NULL;

	//add namespaces to the xpath expression
	if (xmlXPathRegisterNs(context,  BAD_CAST "plk", BAD_CAST "http://www.ethernet-powerlink.org") != 0)
		return NULL;

	if (xmlXPathRegisterNs(context,  BAD_CAST "co", BAD_CAST "http://sourceforge.net/projects/openconf/configuration") != 0)
		return NULL;

	//evaluate xpath expression
	xmlXPathObjectPtr result = xmlXPathEvalExpression(BAD_CAST xpath, context);
	xmlXPathFreeContext(context);
	if (result == NULL)
		return NULL;

	//no nodes found by the xpath expression
	if (xmlXPathNodeSetIsEmpty(result->nodesetval))
	{
		xmlXPathFreeObject(result);
		return NULL;
	}

	return result;
}

//Path setting manipulation
Result ProjectConfiguration::AddPath(const string& id, const string& path)
{
	for (std::vector<Path>::iterator it = this->pathSettings.begin(); it != this->pathSettings.end(); ++it)
	{
		if (it->GetName() == id)
		{
			boost::format formatter(kMsgPathExists);
			formatter % id;
			return Result(PATH_EXISTS, formatter.str());
		}
	}
	this->pathSettings.push_back(Path(id, path));
	return Result();
}

Result ProjectConfiguration::GetPath(const string& id, std::string& pathResult)
{
	for (std::vector<Path>::iterator it = this->pathSettings.begin(); it != this->pathSettings.end(); ++it)
	{
		if (it->GetName() == id)
		{
			pathResult = it->GetValue();
			return Result();
		}
	}
	boost::format formatter(kMsgPathDoesNotExist);
	formatter % id;
	return Result(PATH_DOES_NOT_EXIST, formatter.str());
}

Result ProjectConfiguration::DeletePath(const string& id)
{
	std::vector<Path>::iterator toDelete;
	bool found = false;
	for (toDelete = this->pathSettings.begin(); toDelete != this->pathSettings.end(); ++toDelete)
	{
		if (toDelete->GetName() == id)
		{
			found = true;
			break;
		}
	}
	if(!found)
	{
		boost::format formatter(kMsgPathDoesNotExist);
		formatter % id;
		return Result(PATH_DOES_NOT_EXIST, formatter.str());
	}

	this->pathSettings.erase(toDelete);
	return Result();
}

//View Setting manipulation
Result ProjectConfiguration::AddViewSetting(const ViewType& viewType, const string& name, const string& value)
{
	for (std::vector<ViewSetting>::iterator it = this->viewSettings.begin(); it != this->viewSettings.end(); ++it)
	{
		if (it->GetName() == name && it->GetViewType() == viewType)
		{
			boost::format formatter(kMsgViewSettingExists);
				formatter % name,
				formatter % viewType;
				return Result(VIEW_SETTING_EXISTS, formatter.str());
		}
	}
	this->viewSettings.push_back(ViewSetting(viewType, name, value));
	return Result();
}

Result ProjectConfiguration::GetViewSetting(const ViewType& viewType, const string& name, string& value){
	for (std::vector<ViewSetting>::iterator it = this->viewSettings.begin(); it != this->viewSettings.end(); ++it)
	{
		if (it->GetName() == name && it->GetViewType() == viewType)
		{
			value = it->GetValue();
			return Result();
		}
	}
	boost::format formatter(kMsgViewSettingDoesNotExist);
	formatter % name,
	formatter % viewType;
	return Result(VIEW_SETTING_DOES_NOT_EXIST, formatter.str());
}

Result ProjectConfiguration::DeleteViewSetting(const ViewType& viewType, const std::string& name){
	std::vector<ViewSetting>::iterator toDelete;
	bool found = false;
	for (toDelete = this->viewSettings.begin(); toDelete != this->viewSettings.end(); ++toDelete)
	{
		if (toDelete->GetViewType() == viewType && toDelete->GetName() == name)
		{
			found = true;
			break;
		}
	}
	if(!found)
	{
		boost::format formatter(kMsgViewSettingDoesNotExist);
		formatter % name,
		formatter % viewType;
		return Result(VIEW_SETTING_DOES_NOT_EXIST, formatter.str());
	}

	this->viewSettings.erase(toDelete);
	return Result();
}

Result ProjectConfiguration::SetActiveViewSettings(const ViewType& viewType){
	for (std::vector<ViewSetting>::iterator it = this->viewSettings.begin(); it != this->viewSettings.end(); ++it)
	{
		if (it->GetViewType() == viewType)
		{
			this->activeViewSettingType = viewType;
			return Result();
		}
	}

	boost::format formatter(kMsgViewSettingsDoesNotExist);
	formatter % viewType;
	return Result(VIEW_SETTINGS_DOES_NOT_EXIST, formatter.str());
}

Result ProjectConfiguration::GetActiveViewSettings(ViewType& viewType)
{
	viewType = this->activeViewSettingType;
	return Result();
}

//Autogeneration Setting manipulation
Result ProjectConfiguration::AddAutoGenerationSetting(const string& id, const string& name, const string& value)
{
	for (std::vector<AutoGenSetting>::iterator it = this->autogenerationSettings.begin(); it != this->autogenerationSettings.end(); ++it)
	{
		if (it->GetName() == name && it->GetAutoGenId() == id)
		{
			boost::format formatter(kMsgAutoGenSettingExists);
			formatter % name,
			formatter % id;
			return Result(AUTO_GEN_SETTING_EXISTS, formatter.str());
		}
	}
	this->autogenerationSettings.push_back(AutoGenSetting(id, name, value));
	return Result();
}
Result ProjectConfiguration::GetAutoGenerationSetting(const string& id, const string& name, std::string& settingValue){
	for (std::vector<AutoGenSetting>::iterator it = this->autogenerationSettings.begin(); it != this->autogenerationSettings.end(); ++it)
	{
		if (it->GetName() == name && it->GetAutoGenId() == id)
		{
			settingValue = it->GetValue();
			return Result();
		}

	}
	boost::format formatter(kMsgAutoGenSettingDoesNotExist);
	formatter % name,
	formatter % id;
	return Result(AUTO_GEN_SETTING_DOES_NOT_EXIST, formatter.str());
}

Result ProjectConfiguration::DeleteAutoGenerationSetting(const string& id, const string& name)
{
	std::vector<AutoGenSetting>::iterator toDelete;
	bool found = false;
	for (toDelete = this->autogenerationSettings.begin(); toDelete != this->autogenerationSettings.end(); ++toDelete)
	{
		if (toDelete->GetName() == name && toDelete->GetAutoGenId() == id)
		{
			found = true;
			break;
		}
	}
	if(!found)
	{
		boost::format formatter(kMsgAutoGenSettingDoesNotExist);
		formatter % name,
		formatter % id;
		return Result(AUTO_GEN_SETTING_DOES_NOT_EXIST, formatter.str());
	}

	this->autogenerationSettings.erase(toDelete);
	return Result();

}

Result ProjectConfiguration::SetActiveAutoGenerationSetting(const string& id)
{
	//for (std::vector<AutoGenSetting>::iterator it = this->autogenerationSettings.begin(); it != this->autogenerationSettings.end(); ++it)
	//{
	//	if (it->GetAutoGenId() == id)
	//	{
			this->activeAutogenerationSettingsID = id;
			if(this->activeAutogenerationSettingsID == PROJECT_AUTOGEN_SETTING_ALL)
				SetGenerateMNOBD(true);
			else if(this->activeAutogenerationSettingsID == PROJECT_AUTOGEN_SETTING_NONE)
				SetGenerateMNOBD(false);
			return Result();
	//	}
	//}
	//boost::format formatter(kMsgAutoGenSettingsDoesNotExist);
	//formatter % id;
	//return Result(AUTO_GEN_SETTINGS_DOES_NOT_EXIST, formatter.str());
}

Result ProjectConfiguration::GetActiveAutoGenerationSetting(std::string& activeAutoGenSetting)
{
	activeAutoGenSetting = this->activeAutogenerationSettingsID;
	return Result();
}
