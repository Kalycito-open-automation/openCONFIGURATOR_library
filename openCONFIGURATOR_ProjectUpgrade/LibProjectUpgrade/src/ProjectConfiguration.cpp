/*******************************************************************************
* \file   ProjectConfiguration.cpp
*
* \author Ramakrishnan Periyakaruppan, Kalycito Infotech Private Limited.
*
* \copyright (c) 2014, Kalycito Infotech Private Limited
*                    All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*   * Redistributions of source code must retain the above copyright
*     notice, this list of conditions and the following disclaimer.
*   * Redistributions in binary form must reproduce the above copyright
*     notice, this list of conditions and the following disclaimer in the
*     documentation and/or other materials provided with the distribution.
*   * Neither the name of the copyright holders nor the
*     names of its contributors may be used to endorse or promote products
*     derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
* ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

#include <iostream>
#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/format.hpp>

#include <libxml/parser.h>
#include <libxml/xmlwriter.h>
#include <libxml/xpath.h>

#include "../include/ProjectConfiguration.h"
#include "../include/Utility.h"
#include "../include/BasicTypes.h"
#include "../include/LibraryConfiguration.h"
#include "../include/XmlOperations.h"
#include "../include/Result.h"
#include "../include/Logging.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

const std::string PROJECT_XML_ROOT_ELEMENT = "openCONFIGURATORProject";

const std::string PROJECT_XML_PATH_ELEMENT = "Path";
const std::string PROJECT_XML_PATH_ID_ATTRIBUTE = "id";
const std::string PROJECT_XML_PATH_PATH_ATTRIBUTE = "path";
const std::string PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE = "defaultOutputPath";

const std::string PROJECT_XML_GENERATOR_ELEMENT = "Generator";
const std::string PROJECT_XML_GENERATOR_VENDOR_ATTRIBUTE = "vendor";
const std::string PROJECT_XML_GENERATOR_TOOLNAME_ATTRIBUTE = "toolName";
const std::string PROJECT_XML_GENERATOR_TOOLVERSION_ATTRIBUTE = "toolVersion";
const std::string PROJECT_XML_GENERATOR_CREATEDON_ATTRIBUTE = "createdOn";
const std::string PROJECT_XML_GENERATOR_MODIFIEDON_ATTRIBUTE = "modifiedOn";

const std::string PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT = "ProjectConfiguration";
const std::string PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE = "activeAutoGenerationSetting";

const std::string PROJECT_XML_PROJECT_PATHSETTINGS_ELEMENT = "PathSettings";
const std::string PROJECT_XML_IDECONFIGURATION_ELEMENT = "IDEConfiguration";
const std::string PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_ELEMENT = "ViewSettings";
const std::string PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_TYPE_ATTRIBUTE = "type";
const std::string PROJECT_XML_IDECONFIGURATION_ACTIVE_VIEW_SETTING_ATTRIBUTE = "activeViewSetting";

const std::string PROJECT_XML_SETTING_ELEMENT = "Setting";
const std::string PROJECT_XML_SETTING_NAME_ATTRIBUTE = "name";
const std::string PROJECT_XML_SETTING_VALUE_ATTRIBUTE = "value";

const std::string PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT = "AutoGenerationSettings";
const std::string PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT = "NetworkConfiguration";
const std::string PROJECT_XML_NODE_COLLECTION_ELEMENT = "NodeCollection";
const std::string PROJECT_XML_MANAGING_NODE_ELEMENT = "MN";
const std::string PROJECT_XML_CONTROLLED_NODE_ELEMENT = "CN";

const std::string PROJECT_XML_NODE_NAME_ATTRIBUTE = "name";
const std::string PROJECT_XML_NODE_NODEID_ATTRIBUTE = "nodeID";
const std::string PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE = "pathToXDC";
const std::string PROJECT_XML_NODE_FORCED_MULTIPLEXED_CYCLE_ATTRIBUTE = "forcedMultiplexedCycle";
const std::string PROJECT_XML_NODE_ISMULTIPLEXED_ATTRIBUTE = "isMultiplexed";
const std::string PROJECT_XML_NODE_ISCHAINED_ATTRIBUTE = "isChained";

const std::string PROJECT_XML_VALUE_TRUE = "true";
const std::string PROJECT_XML_VALUE_FALSE = "false";

const std::string PROJECT_AUTOGEN_SETTING_ALL = "all";
const std::string PROJECT_AUTOGEN_SETTING_NONE = "none";

const std::string PROJECT_VIEW_SETTING_BASIC = "BASIC";
const std::string PROJECT_VIEW_SETTING_ADVANCED = "ADVANCED";

const UINT32 kMnNodeId = 240;
const std::string kNewProjectVersion = "1.4.0";
const std::string kDefaultMNXDD = "./resources/openPOWERLINK_MN.xdd";
const std::string kNewProjectDeviceImportDir = "deviceImport";
const std::string kNewProjectDeviceConfigurationDir = "deviceConfiguration";

ProjectConfiguration ProjectConfiguration::instance;

ProjectConfiguration::ProjectConfiguration() :
	initialized(false),
	octProjectPath(),
	octProjectFile(),
	octProjectVersion("0.0.0"),
	autoGenerate(AutoGenerate::UNDEFINED),
	autoSave(AutoSave::UNDEFINED),
	viewMode(ViewMode::UNDEFINED),
	newProjectPath(),
	newProjectFile()
{

}

ProjectConfiguration& ProjectConfiguration::GetInstance()
{
	return ProjectConfiguration::instance;
}

bool ProjectConfiguration::IsVersionValid() const
{
	// TODO: Do we need to check with versions of openCONFIGURATOR library macros?
	return std::lexicographical_compare(this->octProjectVersion.begin(),
										this->octProjectVersion.end(),
										kNewProjectVersion.begin(),
										kNewProjectVersion.end());
}

const std::string& ProjectConfiguration::GetOctProjectFile() const
{
	return octProjectFile;
}

void ProjectConfiguration::SetOctProjectFile(const std::string & projectFile)
{
	this->octProjectFile = projectFile;
}

const std::string& ProjectConfiguration::GetNewProjectFile() const
{
	return newProjectFile;
}

void ProjectConfiguration::SetNewProjectFile(const std::string& newProjectFile)
{
	this->newProjectFile = newProjectFile;
}

const std::string ProjectConfiguration::GetProjectName() const
{
	// Project Name is the file name of the project file.
	boost::filesystem::path projectFileName(this->octProjectFile);
	return projectFileName.stem().generic_string();
}

const boost::filesystem::path& ProjectConfiguration::GetOctProjectPath() const
{
	return octProjectPath;
}

void ProjectConfiguration::SetOctProjectPath(const boost::filesystem::path& projectPath)
{
	this->octProjectPath = projectPath;
}

const std::string& ProjectConfiguration::GetOldProjectVersion() const
{
	return octProjectVersion;
}

void ProjectConfiguration::SetOldProjectVersion(const std::string& version)
{
	this->octProjectVersion = version;
}

Result ProjectConfiguration::ImportProject(const std::string & projectFile)
{
	try
	{
		boost::filesystem::path pPath(projectFile);
		ProjectConfiguration::GetInstance().SetOctProjectFile(pPath.filename().generic_string());
		ProjectConfiguration::GetInstance().SetOctProjectPath(pPath.parent_path());

		xmlDocPtr pDoc = xmlParseFile(projectFile.c_str());
		if (pDoc == NULL)
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % projectFile;
			xmlCleanupParser();
			xmlMemoryDump();
			return Result(ErrorCode::FILE_READ_FAILED, formatter.str());
		}

		std::string xpathActual = "/openCONFIGURATOR[@Version]";
		xmlXPathObjectPtr pResultingXPathObjectActual(GetNodeSet(pDoc, xpathActual));

		if (pResultingXPathObjectActual)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObjectActual->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;
				while (attribute && attribute->name && attribute->children)
				{
					xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
					if (boost::iequals((const char*) attribute->name, "Version"))
					{
						this->SetOldProjectVersion((char*) value);
					}
					else
					{
						std::cout << "Unhandled attribute detected. Attribute: " << (const char*) attribute->name << " Value: " << (char*) value << std::endl;
					}
					xmlFree(value);
					attribute = attribute->next;
				}
			}
		}
		else
		{
			boost::format formatter(kMsgProjectVersionDoesNotExist);
			xmlXPathFreeObject(pResultingXPathObjectActual);
			xmlCleanupParser();
			xmlMemoryDump();
			return Result(ErrorCode::PROJECT_VERSION_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pResultingXPathObjectActual);

		xpathActual = "/openCONFIGURATOR/profile/Auto";
		xmlXPathObjectPtr pProfileAutoPtr(GetNodeSet(pDoc, xpathActual));

		if (pProfileAutoPtr)
		{
			xmlNodeSetPtr pNodeSet(pProfileAutoPtr->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;
				while (attribute && attribute->name && attribute->children)
				{
					xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
					const std::string valueStr = (char*) value;

					if (boost::iequals((const char*) attribute->name, "Generate"))
					{
						if (boost::iequals(valueStr, "YES"))
							this->autoGenerate = AutoGenerate::YES;
						else if (boost::iequals(valueStr, "NO"))
							this->autoGenerate = AutoGenerate::NO;
						else
							this->autoGenerate = AutoGenerate::UNDEFINED;
					}
					else if (boost::iequals((const char*) attribute->name, "Save"))
					{
						if (boost::iequals(valueStr, "YES"))
							this->autoSave = AutoSave::AUTO_SAVE;
						else if (boost::iequals(valueStr, "PROMPT"))
							this->autoSave = AutoSave::PROMPT;
						else if (boost::iequals(valueStr, "DISCARD"))
							this->autoSave = AutoSave::DISCARD;
						else
							this->autoSave = AutoSave::UNDEFINED;
					}
					else if (boost::iequals((const char*) attribute->name, "View"))
					{
						if (boost::iequals(valueStr, "General"))
							this->viewMode = ViewMode::SIMPLE;
						else if (boost::iequals(valueStr, "Advanced"))
							this->viewMode = ViewMode::ADVANCED;
						else
							this->viewMode = ViewMode::UNDEFINED;
					}
					else if (boost::iequals((const char*) attribute->name, "ExpertViewSelected"))
					{
						// This attribute is ignored.
					}
					else
					{
						std::cout << "Unhandled attribute detected. Attribute: " << (const char*) attribute->name << " Value: " << (char*) value << std::endl;
					}

					xmlFree(value);
					attribute = attribute->next;
				}
			}
		}
		else
		{
			boost::format formatter(kMsgAutoTagDoesNotExist);
			xmlXPathFreeObject(pProfileAutoPtr);
			xmlCleanupParser();
			xmlMemoryDump();
			return Result(ErrorCode::AUTO_TAG_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pProfileAutoPtr);

		xpathActual = "/openCONFIGURATOR/NodeCollection/Node";
		xmlXPathObjectPtr pNodeCollectionPtr(GetNodeSet(pDoc, xpathActual));

		if (pNodeCollectionPtr)
		{
			xmlNodeSetPtr pNodeSet(pNodeCollectionPtr->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;

				Node nodeObj;
				while (attribute && attribute->name && attribute->children)
				{
					xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);

					const std::string valueStr = (char*) value;
					if (boost::iequals((char*) attribute->name, "name"))
					{
						nodeObj.SetName(valueStr);
					}
					else if (boost::iequals((char*) attribute->name, "NodeId"))
					{
						nodeObj.SetNodeId(boost::lexical_cast<UINT32>(valueStr));
					}
					else if (boost::iequals((char*) attribute->name, "xdc"))
					{
						boost::filesystem::path xdcPath(valueStr);
						nodeObj.SetOctxPath(xdcPath);
					}
					else if (boost::iequals((char*) attribute->name, "xddPath"))
					{
						boost::filesystem::path xddPath(valueStr);
						nodeObj.SetInputXddPath(xddPath);
					}
					else if (boost::iequals((char*) attribute->name, "StationType"))
					{
						if (boost::iequals(valueStr, "NORMAL"))
							nodeObj.SetStationType(StationType::NORMAL);
						else if (boost::iequals(valueStr, "CHAINED"))
							nodeObj.SetStationType(StationType::CHAINED);
						else if (boost::iequals(valueStr, "MULTIPLEXED"))
							nodeObj.SetStationType(StationType::MULTIPLEXED);
						else
							nodeObj.SetStationType(StationType::UNDEFINED);
					}
					else if (boost::iequals((char*) attribute->name, "NodeType"))
					{
						if (boost::iequals(valueStr, "MN"))
							nodeObj.SetNodeType(NodeType::MN);
						else if (boost::iequals(valueStr, "CN"))
							nodeObj.SetNodeType(NodeType::CN);
						else
						{
							nodeObj.SetNodeType(NodeType::CN);
						}
					}
					else if (boost::iequals((char*) attribute->name, "ForceCycleFlag"))
					{
						if (boost::iequals(valueStr, "true"))
							nodeObj.SetForceCycleFlag(true);
						else
							nodeObj.SetForceCycleFlag(false);
					}
					else if (boost::iequals((char*) attribute->name, "ForceCycle"))
					{
						if (!boost::iequals(valueStr, ""))
							nodeObj.SetForcedCycleValue(boost::lexical_cast<UINT32>(valueStr));
						else
							nodeObj.SetForcedCycleValue(0);
					}
					else
					{
						std::cout << "FieldName=" << (char*) attribute->name << " Value=" << valueStr << std::endl;
					}
					xmlFree(value);
					attribute = attribute->next;
				}
				NodeCollection::GetInstance().Add(nodeObj);
			}

			// TODO: Need to check whether MN configuration is present or not?
		}
		else
		{
			boost::format formatter(kMsgNodeTagDoesNotExist);
			xmlXPathFreeObject(pNodeCollectionPtr);
			xmlCleanupParser();
			xmlMemoryDump();
			return Result(ErrorCode::NODE_TAG_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pNodeCollectionPtr);

		xmlFree(pDoc);
	}
	catch (const std::exception &ex)
	{
		xmlCleanupParser();
		xmlMemoryDump();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	xmlCleanupParser();
	xmlMemoryDump();
	return Result(ErrorCode::SUCCESS);
}

bool ProjectConfiguration::IsInitialized() const
{
	return this->initialized;
}

void ProjectConfiguration::SetInitialized(bool initialized)
{
	this->initialized = initialized;
}

void ProjectConfiguration::ResetConfiguration()
{
	initialized = false;
	octProjectFile.clear();
	octProjectPath.clear();
	octProjectVersion = "0.0.0";
	autoGenerate = AutoGenerate::UNDEFINED;
	autoSave = AutoSave::UNDEFINED;
	viewMode = ViewMode::UNDEFINED;
	newProjectPath.clear();
	NodeCollection::GetInstance().Clear();
}

Result ProjectConfiguration::BackupOldProject()
{
	try
	{
		//TODO: Check if the new folder is already existing or not. If yes, try choosing a different name.
		/*
		 * The old project and its contents will be moved to a directory oldDirectory_<version>.
		 * i.e. <version> will be appended to the project path.
		 */
		const boost::filesystem::path oldProjectBackupDir(ProjectConfiguration::GetInstance().GetOctProjectPath().generic_string()
														  + "_"
														  + ProjectConfiguration::GetInstance().GetOldProjectVersion());
		boost::filesystem::rename(ProjectConfiguration::GetInstance().GetOctProjectPath(), oldProjectBackupDir);

		// Renamed successfully. Update the project path.
		ProjectConfiguration::GetInstance().SetOctProjectPath(oldProjectBackupDir);
	}
	catch (boost::filesystem::filesystem_error & ex)
	{
		return Result(ErrorCode::FILE_SYSTEM_ERROR, ex.what());
	}
	catch (const std::exception & ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

Result ProjectConfiguration::WriteProjectFile(const std::string & projectPath)
{
	try
	{
		const std::string newProjectFileName(ProjectConfiguration::GetInstance().GetProjectName() + ".xml");
		ProjectConfiguration::GetInstance().SetNewProjectFile(newProjectFileName);

		boost::filesystem::path projectFilePath(projectPath + kPathSeparator + newProjectFileName);

		//Write the project file
		xmlTextWriterPtr writer = xmlNewTextWriterFilename(projectFilePath.generic_string().c_str(), 0);
		xmlTextWriterSetIndent(writer, 1);
		//1 - StartDocument
		xmlTextWriterStartDocument(writer, NULL, "UTF-8", NULL);
		//2 - StartElement
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_ROOT_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns", BAD_CAST "http://sourceforge.net/projects/openconf/configuration");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:oc", BAD_CAST "http://sourceforge.net/projects/openconf/configuration");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
		xmlTextWriterWriteAttribute(writer, BAD_CAST "xsi:schemaLocation", BAD_CAST "http://sourceforge.net/projects/openconf/configuration openCONFIGURATOR.xsd");

		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_GENERATOR_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_VENDOR_ATTRIBUTE.c_str(), BAD_CAST "Kalycito Infotech Private Limited & Bernecker + Rainer Industrie Elektronik Ges.m.b.H.");
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_TOOLNAME_ATTRIBUTE.c_str(), BAD_CAST "openCONFIGURATOR");
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_TOOLVERSION_ATTRIBUTE.c_str(), BAD_CAST kNewProjectVersion.c_str());

		std::string createdBy("openCONFIGURATOR project upgrade library from version ");
		createdBy.append(this->octProjectVersion);
		xmlTextWriterWriteAttribute(writer, BAD_CAST "createdBy", BAD_CAST createdBy.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_CREATEDON_ATTRIBUTE.c_str(), BAD_CAST Utility::GetXsdDateTime(XSD_DATE_TIME_TYPE::DATE_TIME).c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_GENERATOR_MODIFIEDON_ATTRIBUTE.c_str(), BAD_CAST Utility::GetXsdDateTime(XSD_DATE_TIME_TYPE::DATE_TIME).c_str());
		xmlTextWriterEndElement(writer);

		//IDEConfiguration
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_ELEMENT.c_str());

		// The active view settings is set to SIMPLE view regardless of the value in the oct project.
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_ACTIVE_VIEW_SETTING_ATTRIBUTE.c_str(), BAD_CAST "BASIC");

		//BasicViewSettings
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer,
									BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_TYPE_ATTRIBUTE.c_str(),
									BAD_CAST "BASIC");


		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_SETTING_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_NAME_ATTRIBUTE.c_str(), BAD_CAST "default");

		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_VALUE_ATTRIBUTE.c_str(), BAD_CAST "SIMPLE");
		xmlTextWriterEndElement(writer);

		xmlTextWriterEndElement(writer);
		//End BasicViewSettings

		//AdvancedViewSettings
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer,
									BAD_CAST PROJECT_XML_IDECONFIGURATION_VIEW_SETTINGS_TYPE_ATTRIBUTE.c_str(),
									BAD_CAST "ADVANCED");

		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_SETTING_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_NAME_ATTRIBUTE.c_str(), BAD_CAST "default");

		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_SETTING_VALUE_ATTRIBUTE.c_str(), BAD_CAST "EXPERT");
		xmlTextWriterEndElement(writer);

		xmlTextWriterEndElement(writer);
		//End AdvancedViewSettings

		xmlTextWriterEndElement(writer);
		//End IDEConfiguration

		//ProjectConfiguration
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_PROJECT_CONFIGURATION_ELEMENT.c_str());
		switch (autoGenerate)
		{
			case AutoGenerate::YES:
				xmlTextWriterWriteAttribute(writer,
											BAD_CAST PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE.c_str(),
											BAD_CAST "all");
				break;
			case AutoGenerate::NO:
			default:
				xmlTextWriterWriteAttribute(writer,
											BAD_CAST PROJECT_XML_PROJECT_CONFIGURATION_ACTIVE_AUTOGEN_SETTING_ATTRIBUTE.c_str(),
											BAD_CAST "none");
				break;
		}

		//PathSettings
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_PROJECT_PATHSETTINGS_ELEMENT.c_str());
		//Path
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_PATH_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer,
									BAD_CAST PROJECT_XML_PATH_ID_ATTRIBUTE.c_str(),
									BAD_CAST PROJECT_XML_PATH_DEFAULT_OUTPUT_ATTRIBUTE.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_PATH_ATTRIBUTE.c_str(),
									BAD_CAST "output");
		xmlTextWriterEndElement(writer);
		//End Path

		xmlTextWriterEndElement(writer);
		//End PathSettings

		// AutoGenerationSettings
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_ID_ATTRIBUTE.c_str(), BAD_CAST "all");
		xmlTextWriterEndElement(writer);
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_AUTOGENERATION_SETTINGS_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer, BAD_CAST PROJECT_XML_PATH_ID_ATTRIBUTE.c_str(), BAD_CAST "none");
		xmlTextWriterEndElement(writer);

		xmlTextWriterEndElement(writer);
		//End ProjectConfiguration

		//NetworkConfiguration
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_NETWORK_CONFIGURATION_ELEMENT.c_str());

		//NodeCollection
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_NODE_COLLECTION_ELEMENT.c_str());

		//MN
		const Node mnNode = NodeCollection::GetInstance().GetNodeRef(kMnNodeId);
		boost::filesystem::path mnXdcPath = mnNode.GetXdcPath();
		xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_MANAGING_NODE_ELEMENT.c_str());
		xmlTextWriterWriteAttribute(writer,
									BAD_CAST PROJECT_XML_NODE_NODEID_ATTRIBUTE.c_str(),
									BAD_CAST boost::lexical_cast<std::string>(kMnNodeId).c_str());
		xmlTextWriterWriteAttribute(writer,
									BAD_CAST PROJECT_XML_NODE_NAME_ATTRIBUTE.c_str(),
									BAD_CAST mnNode.GetName().c_str());
		xmlTextWriterWriteAttribute(writer,
									BAD_CAST PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE.c_str(),
									BAD_CAST Utility::EncodeUrl(mnXdcPath.generic_string()).c_str());

		xmlTextWriterEndElement(writer);
		//End MN

		for (UINT32 i = 0; i < NodeCollection::GetInstance().GetNumberOfNodes(); ++i)
		{
			Node *nodeObj = NodeCollection::GetInstance().GetNodebyColIndex(i);
			std::stringstream nodeIdStream;
			nodeIdStream << nodeObj->GetNodeId();

			if (nodeObj->GetNodeType() == NodeType::CN)
			{
				boost::filesystem::path cnXdcPath = nodeObj->GetXdcPath();

				xmlTextWriterStartElement(writer, BAD_CAST PROJECT_XML_CONTROLLED_NODE_ELEMENT.c_str());
				xmlTextWriterWriteAttribute(writer,
											BAD_CAST PROJECT_XML_NODE_NODEID_ATTRIBUTE.c_str(),
											BAD_CAST  nodeIdStream.str().c_str());
				xmlTextWriterWriteAttribute(writer,
											BAD_CAST PROJECT_XML_NODE_NAME_ATTRIBUTE.c_str(),
											BAD_CAST nodeObj->GetName().c_str());
				xmlTextWriterWriteAttribute(writer,
											BAD_CAST PROJECT_XML_NODE_PATHTOXDC_ATTRIBUTE.c_str(),
											BAD_CAST Utility::EncodeUrl(cnXdcPath.generic_string()).c_str());
				if (nodeObj->GetStationType() == StationType::CHAINED)
				{
					xmlTextWriterWriteAttribute(writer,
												BAD_CAST PROJECT_XML_NODE_ISCHAINED_ATTRIBUTE.c_str(),
												BAD_CAST PROJECT_XML_VALUE_TRUE.c_str());
				}
				else if (nodeObj->GetStationType() == StationType::MULTIPLEXED)
				{
					xmlTextWriterWriteAttribute(writer,
												BAD_CAST PROJECT_XML_NODE_ISMULTIPLEXED_ATTRIBUTE.c_str(),
												BAD_CAST PROJECT_XML_VALUE_TRUE.c_str());
					if (nodeObj->isForcedCycle())
					{
						const UINT32 tempValue = nodeObj->GetForcedCycleValue();

						xmlTextWriterWriteAttribute(writer,
													BAD_CAST PROJECT_XML_NODE_FORCED_MULTIPLEXED_CYCLE_ATTRIBUTE.c_str(),
													BAD_CAST boost::lexical_cast<std::string>(tempValue).c_str());
					}
				}
				else
				{
					// Other types are not written to the XML.
				}

				xmlTextWriterEndElement(writer);
			}
		}

		xmlTextWriterEndElement(writer);
		//End NodeCollection
		xmlTextWriterEndElement(writer);
		//End NetworkConfiguration

		xmlTextWriterEndElement(writer);
		xmlTextWriterEndDocument(writer);

		//Free memory used by libxml2
		xmlFreeTextWriter(writer);
		xmlCleanupParser();
		xmlMemoryDump();
	}
	catch (boost::filesystem::filesystem_error & ex)
	{
		return Result(ErrorCode::FILE_SYSTEM_ERROR, ex.what());
	}
	catch (const std::exception & ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	catch (Result & ex)
	{
		return ex;
	}
	return Result(ErrorCode::SUCCESS);
}

Result ProjectConfiguration::ValidateProjectFile(const std::string & fileName) const
{
	Result retval;
	if (!fileName.empty())
	{
		//Parse XML
		xmlDocPtr xdd_file_ptr = xmlParseFile(fileName.c_str());

		if (xdd_file_ptr != NULL)
		{
			//Call Validation Function with the parse file
			retval = ValidateXMLFile(xdd_file_ptr, LibraryConfiguration::GetProjectFileSchemaFilePath());
		}
		else
		{
			retval = Result(ErrorCode::XML_PARSING_ERROR, "Project file parsing failed due to wrong XML syntax.");
		}

		xmlFreeDoc(xdd_file_ptr);
		//CleanUp XML Parser
		xmlCleanupParser();
		xmlMemoryDump();
	}
	else
	{
		boost::format formatter(kMsgEmptyArgument);
		formatter % "'fileName'";
		retval = Result(ErrorCode::INVALID_PARAMETER, formatter.str());
	}

	return retval;
}

const boost::filesystem::path& ProjectConfiguration::GetNewProjectPath() const
{
	return this->newProjectPath;
}

void ProjectConfiguration::SetNewProjectPath(const boost::filesystem::path& newProjectPath)
{
	this->newProjectPath = newProjectPath;
}

Result ProjectConfiguration::CreateNewProjectDirectories() const
{
	try
	{
		//Create project folder structure
		LOG_INFO() << "New project folders are created in the path: " << GetNewProjectPath().generic_string();
		//TODO: Need to save in the class.
		boost::filesystem::path import_dir(GetNewProjectPath().generic_string() + kPathSeparator + kNewProjectDeviceImportDir);
		boost::filesystem::path configuration_dir(GetNewProjectPath().generic_string() + kPathSeparator + kNewProjectDeviceConfigurationDir);
		boost::filesystem::path output_dir(GetNewProjectPath().generic_string() + kPathSeparator + "output");

		boost::filesystem::create_directories(import_dir);
		boost::filesystem::create_directories(configuration_dir);
		boost::filesystem::create_directories(output_dir);
	}
	catch (boost::filesystem::filesystem_error & ex)
	{
		return Result(ErrorCode::FILE_SYSTEM_ERROR, ex.what());
	}
	catch (const std::exception & ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

Result ProjectConfiguration::ImportAllXddFiles()
{
	try
	{
		LOG_INFO() << "Importing all provided XDD/XDC files.";
		std::vector<UINT32> nodeIds = NodeCollection::GetInstance().GetNodeIds();

		if (nodeIds.size())
		{
			for (std::vector<UINT32>::iterator it = nodeIds.begin(); it != nodeIds.end(); ++it)
			{
				UINT32 nodeId = *it;
				Node &nodeObj = NodeCollection::GetInstance().GetNodeRef(nodeId);

				boost::filesystem::path inputXddPath = nodeObj.GetInputXddPath().generic_string();

				bool octxIsUsedAsXdd = false;
				if (inputXddPath.generic_string().empty())
				{
					if (nodeObj.GetNodeType() == NodeType::MN)
					{
						// Set the input XDD to default MN XDD.
						nodeObj.SetInputXddPath(kDefaultMNXDD);
						inputXddPath = boost::filesystem::path(kDefaultMNXDD);
					}
					else
					{
						/*
						 * OCTX is used as the input XDD.
						 * (Note: Default CN XDD is not used. Manufacturer specific objects will be missing in default CN XDD.)
						 */
						inputXddPath = boost::filesystem::path(GetOctProjectPath().generic_string() + kPathSeparator + nodeObj.GetOctxPath().generic_string());
						octxIsUsedAsXdd = true;
					}
				}

				LOG_INFO() << "NodeID: " << nodeId << " Name: " << nodeObj.GetName() << " Input XDD Path: " << inputXddPath.generic_string() << std::endl;

				/*
				 * If XDD path is not empty, copy the XDD/XDC file to the deviceImport folder.
				 */
				if (!inputXddPath.generic_string().empty())
				{
					std::string fileName = inputXddPath.stem().string();
					if (octxIsUsedAsXdd)
					{
						if (!nodeObj.GetName().empty())
						{
							fileName = nodeObj.GetName() + "_" + boost::lexical_cast<std::string>(nodeId);
						}
					}

					boost::filesystem::path localPathXddFile(kNewProjectDeviceImportDir
															 + kPathSeparator
															 + fileName
															 + inputXddPath.extension().string());

					boost::filesystem::path xdd_file(GetNewProjectPath().generic_string()
													 + kPathSeparator
													 + localPathXddFile.generic_string());

					if (boost::filesystem::exists(xdd_file))
					{
						LOG_WARN() << "Node ID:" << nodeObj.GetNodeId()
								   << ". The node configuration:" << inputXddPath.generic_string()
								   << " will be overwritten on the already available configuration."
								   << xdd_file.generic_string();
					}

					boost::filesystem::copy_file(inputXddPath, xdd_file, boost::filesystem::copy_option::overwrite_if_exists);

					nodeObj.SetXddPath(localPathXddFile);

					if (xdd_file.extension().string().compare(".octx") == 0)
					{
						Result retVal = nodeObj.UpdateOctxMissingFields();
						if (!retVal.IsSuccessful())
						{
							return retVal;
						}

						// TODO: Delete all the actual values. Is it necessary?

						boost::filesystem::path octxFileWithXdcExtension = xdd_file;
						octxFileWithXdcExtension.replace_extension(".xdc");

						std::string xmlLintCommand(LibraryConfiguration::GetXmlLintFilePath());
						xmlLintCommand.append(" --format");
						xmlLintCommand.append(" \"" + xdd_file.generic_string() + "\" >");
						xmlLintCommand.append(" \"" + octxFileWithXdcExtension.generic_string() + "\"");
						LOG_INFO() << "Formatting the XML document " << xmlLintCommand;
						INT32 returnFromXmlLint = std::system(xmlLintCommand.c_str());
						if (returnFromXmlLint)
						{
							return Result(ErrorCode::XML_FORMATING_FAILED, xmlLintCommand);
						}

						// boost::filesystem::rename(xdd_file, octxFileWithXdcExtension);
						boost::filesystem::remove(xdd_file);

						xdd_file.replace_extension(".xdc");
						localPathXddFile.replace_extension(".xdc");
						nodeObj.SetXddPath(localPathXddFile);
					}

					Result retVal = ValidateXDDFile(xdd_file.generic_string());
					if (!retVal.IsSuccessful())
					{
						return retVal;
					}
				}
			}
		}
		else
		{
			LOG_WARN() << "There are no nodes present in the project configuration.";
		}
	}
	catch (boost::filesystem::filesystem_error & ex)
	{
		return Result(ErrorCode::FILE_SYSTEM_ERROR, ex.what());
	}
	catch (const std::exception & ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

Result ProjectConfiguration::ImportAllConfigurationsFromDeviceImportDir()
{
	try
	{
		std::vector<UINT32> nodeIds = NodeCollection::GetInstance().GetNodeIds();

		if (nodeIds.size())
		{
			for (std::vector<UINT32>::iterator it = nodeIds.begin(); it != nodeIds.end(); ++it)
			{
				UINT32 nodeId = *it;
				Node &nodeObj = NodeCollection::GetInstance().GetNodeRef(nodeId);

				boost::filesystem::path importedXdd(nodeObj.GetXddPath(false).generic_string());

				boost::filesystem::path xdcFileLocalPath(kNewProjectDeviceConfigurationDir
														 + kPathSeparator
														 + importedXdd.stem().string()
														 + "_" + boost::lexical_cast<std::string>(nodeId)
														 + ".xdc");
				boost::filesystem::path xdc_file(GetNewProjectPath().generic_string()
												 + kPathSeparator
												 + xdcFileLocalPath.generic_string());

				LOG_INFO() << "NodeID:" << nodeObj.GetNodeId() << " Copying file: " << importedXdd.generic_string() << " as XDC file: " << xdc_file.generic_string();

				boost::filesystem::copy_file(importedXdd, xdc_file, boost::filesystem::copy_option::overwrite_if_exists);

				nodeObj.SetXdcPath(xdcFileLocalPath);

				Result retVal = nodeObj.UpdateXdcWithValuesFromOctx();
				if (!retVal.IsSuccessful())
				{
					return retVal;
				}
			}
		}
	}
	catch (boost::filesystem::filesystem_error & ex)
	{
		return Result(ErrorCode::FILE_SYSTEM_ERROR, ex.what());
	}
	catch (const std::exception & ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

Result ProjectConfiguration::ValidateXDDFile(const std::string & fileName) const
{
	//TODO: Try catch block needed
	Result retval;
	if (!fileName.empty())
	{
		//Parse XML
		xmlDocPtr xdd_file_ptr = xmlParseFile(fileName.c_str());

		if (xdd_file_ptr != NULL)
		{
			//Call validation function with the parse file.
			retval = ValidateXMLFile(xdd_file_ptr, LibraryConfiguration::GetXddSchemaFilePath());
		}
		else
		{
			//TODO: add file name.
			retval = Result(ErrorCode::XML_PARSING_ERROR, "Node configuration file parsing failed due to wrong XML syntax.");
		}

		xmlFreeDoc(xdd_file_ptr);
		//CleanUp XML Parser
		xmlCleanupParser();
		xmlMemoryDump();
	}
	else
	{
		boost::format formatter(kMsgEmptyArgument);
		formatter % "'fileName'";
		retval = Result(ErrorCode::INVALID_PARAMETER, formatter.str());
	}
	return retval;
}

PROJECT_UPGRADE_CLOSE_NAMESPACE
