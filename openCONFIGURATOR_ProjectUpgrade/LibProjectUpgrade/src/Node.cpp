/*******************************************************************************
* \file   Node.cpp
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

#include "../include/Node.h"
#include "../include/XmlOperations.h"
#include "../include/BasicTypes.h"
#include "../include/Utility.h"
#include "../include/ProjectConfiguration.h"
#include "../include/Logging.h"

#include <map>

#include <boost/format.hpp>
#include <boost/algorithm/string.hpp>

#include <libxml/parser.h>
#include <libxml/xpath.h>


PROJECT_UPGRADE_OPEN_NAMESPACE

Node::Node() :
	id(0),
	name(""),
	stationType(StationType::UNDEFINED),
	nodeType(NodeType::UNDEFINED),
	forceCycleFlag(false),
	forcedCycleValue(0),
	octxPath(),
	inputXddPath(),
	xddPath(),
	xdcPath()

{

}

UINT32 Node::GetNodeId() const
{
	return this->id;
}

void Node::SetNodeId(const UINT32 nodeId)
{
	this->id = nodeId;
}

const boost::filesystem::path Node::GetOctxPath(bool shortPath) const
{
	if (shortPath)
	{
		return this->octxPath;
	}
	else
	{
		return boost::filesystem::path(ProjectConfiguration::GetInstance().GetOctProjectPath().generic_string()
									   + kPathSeparator
									   + this->octxPath.generic_string());
	}
}

void Node::SetOctxPath(const boost::filesystem::path& octxPath)
{
	this->octxPath = octxPath;
}

const std::string Node::GetName() const
{
	return this->name;
}

void Node::SetName(const std::string & name)
{
	this->name = name;
}

StationType::StationType Node::GetStationType() const
{
	return this->stationType;
}

void Node::SetStationType(StationType::StationType stationType)
{
	this->stationType = stationType;
}

NodeType::NodeType Node::GetNodeType() const
{
	return this->nodeType;
}

void Node::SetNodeType(NodeType::NodeType nodeType)
{
	this->nodeType = nodeType;
}

bool Node::isForcedCycle() const
{
	return this->forceCycleFlag;
}

void Node::SetForceCycleFlag(bool flag)
{
	this->forceCycleFlag = flag;
}

UINT32 Node::GetForcedCycleValue() const
{
	return this->forcedCycleValue;
}

void Node::SetForcedCycleValue(const UINT32 value)
{
	this->forcedCycleValue = value;
}

const boost::filesystem::path& Node::GetInputXddPath() const
{
	return this->inputXddPath;
}

void Node::SetInputXddPath(const boost::filesystem::path& inputXddPath)
{
	this->inputXddPath = inputXddPath;
}

const boost::filesystem::path Node::GetXddPath(bool shortPath) const
{
	if (shortPath)
	{
		return this->xddPath;
	}
	else
	{
		return boost::filesystem::path(ProjectConfiguration::GetInstance().GetNewProjectPath().generic_string()
									   + kPathSeparator
									   + this->xddPath.generic_string());
	}
}

void Node::SetXddPath(const boost::filesystem::path& xddPath)
{
	this->xddPath = xddPath;
}

const boost::filesystem::path Node::GetXdcPath(bool shortPath) const
{
	if (shortPath)
	{
		return this->xdcPath;
	}
	else
	{
		return boost::filesystem::path(ProjectConfiguration::GetInstance().GetNewProjectPath().generic_string()
									   + kPathSeparator
									   + this->xdcPath.generic_string());
	}
}

void Node::SetXdcPath(const boost::filesystem::path& xdcPath)
{
	this->xdcPath = xdcPath;
}

// '.octx' file present inside the deviceImportFolder is updated with the missing fields.
Result Node::UpdateOctxMissingFields() const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string deviceImportXddFile = this->GetXddPath(false).generic_string();
		LOG_INFO() << "Upgrading the configuration of Node ID '" << this->id << "': " << deviceImportXddFile;

		xmlInitParser();
		xmlDocPtr pDoc = xmlParseFile(deviceImportXddFile.c_str());
		if (pDoc == NULL)
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % deviceImportXddFile;
			xmlCleanupParser();
			xmlMemoryDump();
			LOG_DEBUG() << "ERROR: " << formatter.str();
			return Result(ErrorCode::FILE_READ_FAILED, formatter.str());
		}

		Result retVal = UpdateISO15745ProfileContainer(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = FixISO15745ProfileTag(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = AddProfileHeader(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = UpdateProfileBodyInformation(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = FixApplicationProcess(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = FixParameterDescription(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = AddApplicationLayerIdentityInfo(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = AddTransportLayers(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = CleanupObjectAttributes(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		retVal = FixNetworkManagemetAttributes(pDoc);
		if (!retVal.IsSuccessful())
		{
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		// Save the file with all updated information.
		xmlSaveFormatFileEnc(deviceImportXddFile.c_str(), pDoc, "UTF-8", 1);
		xmlFreeDoc(pDoc);
	}
	catch (const std::exception & ex)
	{
		xmlCleanupParser();
		xmlMemoryDump();
		LOG_DEBUG() << "ERROR: " << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	xmlCleanupParser();
	xmlMemoryDump();
	return Result(ErrorCode::SUCCESS);
}

//
Result Node::UpdateXdcWithValuesFromOctx() const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string octxFile = this->GetOctxPath(false).generic_string();
		const std::string xdcFile = this->GetXdcPath(false).generic_string();

		xmlDocPtr pOctxDoc = xmlParseFile(octxFile.c_str());
		if (pOctxDoc == NULL)
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % octxFile;
			xmlCleanupParser();
			xmlMemoryDump();
			LOG_DEBUG() << " ERROR:" << formatter.str();
			return Result(ErrorCode::FILE_READ_FAILED, formatter.str());
		}

		xmlDocPtr pXdcDoc = xmlParseFile(xdcFile.c_str());
		if (pXdcDoc == NULL)
		{
			boost::format formatter(kMsgFileReadFailed);
			formatter % xdcFile;
			xmlFreeDoc(pOctxDoc);
			xmlCleanupParser();
			xmlMemoryDump();
			LOG_DEBUG() << " ERROR:" << formatter.str();
			return Result(ErrorCode::FILE_READ_FAILED, formatter.str());
		}

		Result retVal = UpdateXdcWithObjectActualValues(pOctxDoc, pXdcDoc);
		if (!retVal.IsSuccessful())
		{
			xmlFreeDoc(pOctxDoc);
			xmlFreeDoc(pXdcDoc);
			xmlCleanupParser();
			xmlMemoryDump();
			return retVal;
		}

		xmlFreeDoc(pOctxDoc);
		//Save the XDC file
		xmlSaveFormatFileEnc(xdcFile.c_str(), pXdcDoc, "UTF-8", 1);
		xmlFreeDoc(pXdcDoc);
	}
	catch (const std::exception & ex)
	{
		xmlCleanupParser();
		xmlMemoryDump();
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	xmlCleanupParser();
	xmlMemoryDump();
	return Result(ErrorCode::SUCCESS);
}

Result Node::UpdateISO15745ProfileContainer(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string xpathActual = "/ISO15745ProfileContainer";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlNewProp(pIndexNode, BAD_CAST "xmlns", BAD_CAST "http://www.ethernet-powerlink.org");
				xmlNewProp(pIndexNode, BAD_CAST "xmlns:xsi", BAD_CAST "http://www.w3.org/2001/XMLSchema-instance");
				xmlNewProp(pIndexNode, BAD_CAST "xsi:schemaLocation", BAD_CAST "http://www.ethernet-powerlink.org Powerlink_Main.xsd");
			}
		}
		else
		{
			boost::format formatter(kMsgXddTagDoesNotExist);
			formatter % xpathActual % pDoc->name;
			xmlXPathFreeObject(pResultingXPathObject);
			LOG_DEBUG() << " ERROR:" << formatter.str();
			return Result(ErrorCode::XDD_TAG_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::FixISO15745ProfileTag(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		std::string xpathActual = "//ISO15745ProfileContainer/ISO15745Profile";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			if (xmlXPathNodeSetGetLength(pNodeSet) >= 2)
			{
				LOG_WARN() << "This file " << pDoc->name << " has more that two ISO15745Profile tags. Please check your OCTX file.";
				//TODO: Return appropriate error.
				return Result(ErrorCode::SUCCESS);
			}

			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);

				// Add the ISO15745Profile under ISO15745ProfileContainer
				xmlNodePtr isoContainer = pIndexNode->parent;
				// ISO15745Profile
				xmlNodePtr iso15745Profile = xmlNewNode( 0, BAD_CAST "ISO15745Profile" );

				xpathActual = "//ISO15745ProfileContainer/ISO15745Profile/ProfileBody";
				xmlXPathObjectPtr pResultingProfileBody(GetNodeSet(pDoc, xpathActual));
				if (pResultingProfileBody)
				{
					xmlNodeSetPtr pProfileBodySet(pResultingProfileBody->nodesetval);
					for (int j = 0; j < xmlXPathNodeSetGetLength(pProfileBodySet); ++j)
					{
						if (j == 1)
						{
							LOG_INFO() << "Moving the ProfileBody tag to the new ISO15745Profile.";
							xmlNodePtr pProfileBody(pProfileBodySet->nodeTab[j]);
							xmlUnlinkNode(pProfileBody);
							xmlAddChild(iso15745Profile, pProfileBody);
						}
					}
				}
				else
				{
					LOG_FATAL() << "The " << xpathActual << " tag is not found.";
				}
				xmlXPathFreeObject(pResultingProfileBody);

				xmlAddChild(isoContainer, iso15745Profile);
			}
		}
		else
		{
			xpathActual = "//ISO15745ProfileContainer";
			xmlXPathObjectPtr pResultingXPathContainer(GetNodeSet(pDoc, xpathActual));
			if (pResultingXPathContainer)
			{
				xmlNodeSetPtr pNodeSet(pResultingXPathContainer->nodesetval);

				for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
				{
					xmlNodePtr isoContainer(pNodeSet->nodeTab[i]);

					// ISO15745Profile - 1
					xmlNodePtr iso15745Profile_1 = xmlNewNode( 0, BAD_CAST "ISO15745Profile" );

					// ISO15745Profile - 2
					xmlNodePtr iso15745Profile_2 = xmlNewNode( 0, BAD_CAST "ISO15745Profile" );

					xpathActual = "//ISO15745ProfileContainer/ProfileBody";
					xmlXPathObjectPtr pResultingProfileBody(GetNodeSet(pDoc, xpathActual));
					if (pResultingProfileBody)
					{
						xmlNodeSetPtr pProfileBodySet(pResultingProfileBody->nodesetval);
						if (xmlXPathNodeSetGetLength(pNodeSet) > 2)
						{
							// Return error since ISO15745ProfileContainer shall only have 2 ProfileBody tags
							return Result(ErrorCode::XDD_TAG_DOES_NOT_EXIST, "Any ISO15745ProfileContainer shall only have 2 ProfileBody tags");
						}

						for (int j = 0; j < xmlXPathNodeSetGetLength(pProfileBodySet); ++j)
						{
							xmlNodePtr pProfileBody(pProfileBodySet->nodeTab[j]);
							xmlUnlinkNode(pProfileBody);
							if (j == 0)
							{
								xmlAddChild(iso15745Profile_1, pProfileBody);
							}
							else if (j == 1)
							{
								xmlAddChild(iso15745Profile_2, pProfileBody);
							}
						}
					}
					else
					{
						LOG_FATAL() << "The " << xpathActual << " tag is not found." << std::endl;
					}
					xmlXPathFreeObject(pResultingProfileBody);

					xmlAddChild(isoContainer, iso15745Profile_1);
					xmlAddChild(isoContainer, iso15745Profile_2);

				}
			}
			else
			{
				LOG_FATAL() << "The " << xpathActual << " tag is not found";
			}
			xmlXPathFreeObject(pResultingXPathContainer);
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::AddProfileHeader(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string xpathActual = "//ISO15745Profile/ProfileBody";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);

				LOG_INFO() << "Adding new ProfileHeader tag.";
				if (!pIndexNode)
				{
					continue;
				}

				xmlNodePtr profileHeader = xmlNewNode( 0, BAD_CAST "ProfileHeader" );

				// profileIdentification node
				xmlNodePtr profileIdentification = xmlNewNode(0, BAD_CAST "ProfileIdentification");
				xmlNodePtr profileIdentificationChild = xmlNewText( BAD_CAST "openPOWERLINK Device Profile name" );
				xmlAddChild(profileIdentification, profileIdentificationChild);

				xmlAddChild(profileHeader, profileIdentification);

				// ProfileRevision node
				xmlNodePtr profileRevision = xmlNewNode(0, BAD_CAST "ProfileRevision");
				xmlNodePtr profileRevisionChild = xmlNewText( BAD_CAST "1" );
				xmlAddChild(profileRevision, profileRevisionChild);

				xmlAddChild(profileHeader, profileRevision);


				// ProfileName node
				xmlNodePtr profileName = xmlNewNode(0, BAD_CAST "ProfileName");
				xmlNodePtr profileNameChild = xmlNewText( BAD_CAST "openPOWERLINK Device Profile name" );
				xmlAddChild(profileName, profileNameChild);

				xmlAddChild(profileHeader, profileName);


				//ProfileSource node
				xmlNodePtr profileSource = xmlNewNode(0, BAD_CAST "ProfileSource");

				xmlAddChild(profileHeader, profileSource);


				//ProfileClassID node
				xmlNodePtr profileClassID = xmlNewNode(0, BAD_CAST "ProfileClassID");
				if (i == 0)
				{
					xmlNodePtr profileClassIdChild = xmlNewText( BAD_CAST "Device" );
					xmlAddChild(profileClassID, profileClassIdChild);
				}
				else if (i == 1)
				{
					xmlNodePtr profileClassIdChild = xmlNewText( BAD_CAST "CommunicationNetwork" );
					xmlAddChild(profileClassID, profileClassIdChild);
				}
				xmlAddChild(profileHeader, profileClassID);

				// ISO15745Reference
				xmlNodePtr iso15745Reference = xmlNewNode( 0, BAD_CAST "ISO15745Reference" );
				//ISO15745Part
				xmlNodePtr iso15745Part = xmlNewNode( 0, BAD_CAST "ISO15745Part" );
				xmlNodePtr iso15745PartChild = xmlNewText( BAD_CAST "4" );
				xmlAddChild(iso15745Part, iso15745PartChild);

				xmlAddChild(iso15745Reference, iso15745Part);

				// ISO15745Edition
				xmlNodePtr iso15745Edition = xmlNewNode( 0, BAD_CAST "ISO15745Edition" );
				xmlNodePtr iso15745EditionChild = xmlNewText( BAD_CAST "1" );
				xmlAddChild(iso15745Edition, iso15745EditionChild);

				xmlAddChild(iso15745Reference, iso15745Edition);

				//ProfileTechnology
				xmlNodePtr profileTechnology = xmlNewNode( 0, BAD_CAST "ProfileTechnology" );
				xmlNodePtr profileTechnologyChild = xmlNewText( BAD_CAST "Powerlink" );
				xmlAddChild(profileTechnology, profileTechnologyChild);

				xmlAddChild(iso15745Reference, profileTechnology);

				xmlAddChild(profileHeader, iso15745Reference);

				// Add the Profile header previous to profile body.
				xmlAddPrevSibling(pIndexNode, profileHeader);
			}
		}
		else
		{
			boost::format formatter(kMsgXddTagDoesNotExist);
			formatter % xpathActual % pDoc->name;
			xmlXPathFreeObject(pResultingXPathObject);
			LOG_DEBUG() << "ERROR: " << formatter.str();
			return Result(ErrorCode::XDD_TAG_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::UpdateProfileBodyInformation(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string fileName(this->GetXddPath().stem().generic_string() + ".xdc");

		std::string xpathActual = "/ISO15745ProfileContainer/ISO15745Profile/ProfileBody";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				if (i == 0)
				{
					xmlNewProp(pIndexNode, BAD_CAST "xsi:type", BAD_CAST "ProfileBody_Device_Powerlink");
				}
				else if (i == 1)
				{
					xmlNewProp(pIndexNode, BAD_CAST "xsi:type", BAD_CAST "ProfileBody_CommunicationNetwork_Powerlink");
				}

				xmlNewProp(pIndexNode, BAD_CAST "fileName", BAD_CAST fileName.c_str());
				xmlNewProp(pIndexNode, BAD_CAST "fileCreator", BAD_CAST "openCONFIGURATOR project upgrade library");
				xmlNewProp(pIndexNode, BAD_CAST "fileCreationDate", BAD_CAST Utility::GetXsdDateTime(XSD_DATE_TIME_TYPE::DATE).c_str());
				xmlNewProp(pIndexNode, BAD_CAST "fileCreationTime", BAD_CAST Utility::GetXsdDateTime(XSD_DATE_TIME_TYPE::TIME).c_str());
				xmlNewProp(pIndexNode, BAD_CAST "fileModificationDate", BAD_CAST Utility::GetXsdDateTime(XSD_DATE_TIME_TYPE::DATE).c_str());
				xmlNewProp(pIndexNode, BAD_CAST "fileModificationTime", BAD_CAST Utility::GetXsdDateTime(XSD_DATE_TIME_TYPE::TIME).c_str());
				xmlNewProp(pIndexNode, BAD_CAST "fileModifiedBy", BAD_CAST "openCONFIGURATOR project upgrade library");
				xmlNewProp(pIndexNode, BAD_CAST "fileVersion", BAD_CAST "01.00");
				xmlNewProp(pIndexNode, BAD_CAST "supportedLanguages", BAD_CAST "en");

				// Add the DeviceIdentity and DeviceFunctions for the 1st profile body.
				if (i == 0)
				{
					// ------------------DeviceIdentity-------------------------
					xmlNodePtr deviceIdentity = xmlNewNode( 0, BAD_CAST "DeviceIdentity" );

					// vendorName
					xmlNodePtr vendorName = xmlNewNode( 0, BAD_CAST "vendorName" );
					xmlNodePtr vendorNameChild = xmlNewText( BAD_CAST "Unknown vendor" );
					xmlAddChild(vendorName, vendorNameChild);

					xmlAddChild(deviceIdentity, vendorName);

					// vendorID
					xmlNodePtr vendorID = xmlNewNode( 0, BAD_CAST "vendorID" );
					xmlNodePtr vendorIDChild = xmlNewText( BAD_CAST "0x00000000" );
					xmlAddChild(vendorID, vendorIDChild);

					xmlAddChild(deviceIdentity, vendorID);

					// productName
					xmlNodePtr productName = xmlNewNode( 0, BAD_CAST "productName" );
					xmlNodePtr productNameChild = xmlNewText( BAD_CAST "openPOWERLINK device" );
					xmlAddChild(productName, productNameChild);

					xmlAddChild(deviceIdentity, productName);


					if (pIndexNode->children)
						xmlAddPrevSibling(pIndexNode->children, deviceIdentity);
					else
						xmlAddChild(pIndexNode, deviceIdentity);


					// ------------------DeviceFunction-------------------------
					xmlNodePtr deviceFunction = xmlNewNode( 0, BAD_CAST "DeviceFunction" );

					// capabilities
					xmlNodePtr capabilities = xmlNewNode( 0, BAD_CAST "capabilities" );

					// characteristicsList
					xmlNodePtr characteristicsList = xmlNewNode( 0, BAD_CAST "characteristicsList" );

					// characteristic
					xmlNodePtr characteristic = xmlNewNode( 0, BAD_CAST "characteristic" );


					// characteristicName
					xmlNodePtr characteristicName = xmlNewNode( 0, BAD_CAST "characteristicName" );
					// label
					xmlNodePtr label = xmlNewNode( 0, BAD_CAST "label" );
					xmlNodePtr labelText = xmlNewText( BAD_CAST "Transfer rate" );
					xmlAddChild(label, labelText);
					xmlNewProp(label, BAD_CAST "lang", BAD_CAST "en");
					xmlAddChild(characteristicName, label);

					xmlAddChild(characteristic, characteristicName);

					// characteristicContent
					xmlNodePtr characteristicContent = xmlNewNode( 0, BAD_CAST "characteristicContent" );
					// label
					xmlNodePtr label_1 = xmlNewNode( 0, BAD_CAST "label" );
					xmlNodePtr labelText_1 = xmlNewText( BAD_CAST "100 MBit/s" );
					xmlAddChild(label_1, labelText_1);
					xmlNewProp(label_1, BAD_CAST "lang", BAD_CAST "en");
					xmlAddChild(characteristicContent, label_1);

					xmlAddChild(characteristic, characteristicContent);


					xmlAddChild(characteristicsList, characteristic);

					xmlAddChild(capabilities, characteristicsList);


					xmlAddChild(deviceFunction, capabilities);

					xmlAddNextSibling(deviceIdentity, deviceFunction);
				}
			}
		}
		else
		{
			boost::format formatter(kMsgXddTagDoesNotExist);
			formatter % xpathActual % pDoc->name;
			xmlXPathFreeObject(pResultingXPathObject);
			LOG_DEBUG() << "ERROR: " << formatter.str();
			return Result(ErrorCode::XDD_TAG_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::AddApplicationLayerIdentityInfo(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		std::string xpathActual = "//ProfileBody/ApplicationLayers/DataTypeList";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlNodePtr identity = xmlNewNode( 0, BAD_CAST "identity" );


				// vendorID
				xmlNodePtr vendorId = xmlNewNode( 0, BAD_CAST "vendorID" );
				xmlNodePtr vendorIdChild = xmlNewText( BAD_CAST "0x00000000" );
				xmlAddChild(vendorId, vendorIdChild);

				xmlAddChild(identity, vendorId);

				// productID
				xmlNodePtr productId = xmlNewNode( 0, BAD_CAST "productID" );
				xmlNodePtr productIdChild = xmlNewText( BAD_CAST "0" );
				xmlAddChild(productId, productIdChild);

				xmlAddChild(identity, productId);

				xmlAddPrevSibling(pIndexNode, identity);
			}
		}
		else
		{
			boost::format formatter(kMsgXddTagDoesNotExist);
			formatter % xpathActual % pDoc->name;
			xmlXPathFreeObject(pResultingXPathObject);
			LOG_DEBUG() << "ERROR: " << formatter.str();
			return Result(ErrorCode::XDD_TAG_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::AddTransportLayers(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		std::string xpathActual = "//ProfileBody/NetworkManagement";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlNodePtr transportLayers = xmlNewNode( 0, BAD_CAST "TransportLayers" );

				xmlAddPrevSibling(pIndexNode, transportLayers);
			}
		}
		else
		{
			boost::format formatter(kMsgXddTagDoesNotExist);
			formatter % xpathActual % pDoc->name;
			xmlXPathFreeObject(pResultingXPathObject);
			LOG_DEBUG() << "ERROR: " << formatter.str();
			return Result(ErrorCode::XDD_TAG_DOES_NOT_EXIST, formatter.str());
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::CleanupObjectAttributes(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		std::string xpathActual = "//Object[@index and (@PDOmapping or @CDCFlag)]";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;

				while (attribute && attribute->name && attribute->children)
				{
					if (boost::iequals((char*) attribute->name, "PDOmapping"))
					{
						xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
						xmlSetProp(pIndexNode, BAD_CAST "PDOmapping", BAD_CAST FixPdoMappingCase((const char*) value).c_str());
						xmlFree(value);
					}
					else if (boost::iequals((char*) attribute->name, "CDCFlag"))
					{
						xmlRemoveProp(xmlHasProp(pIndexNode, BAD_CAST "CDCFlag"));
					}
					attribute = attribute->next;
				}
			}
		}
		else
		{
			LOG_WARN() << "There is no object present in this file: " << pDoc->name;
		}
		xmlXPathFreeObject(pResultingXPathObject);

		xpathActual = "//SubObject[@subIndex and (@PDOmapping or @CDCFlag)]";
		xmlXPathObjectPtr pResultingXPathObject_1(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject_1)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject_1->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;

				while (attribute && attribute->name && attribute->children)
				{
					if (boost::iequals((char*) attribute->name, "PDOmapping"))
					{
						xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
						xmlSetProp(pIndexNode, BAD_CAST "PDOmapping", BAD_CAST FixPdoMappingCase((const char*) value).c_str());
						xmlFree(value);
					}
					else if (boost::iequals((char*) attribute->name, "CDCFlag"))
					{
						xmlRemoveProp(xmlHasProp(pIndexNode, BAD_CAST "CDCFlag"));
					}

					attribute = attribute->next;
				}
			}
		}
		else
		{
			LOG_WARN() << "There is no SubObject present in this file: " << pDoc->name;
		}
		xmlXPathFreeObject(pResultingXPathObject_1);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

const std::string Node::FixPdoMappingCase(const std::string & pdoMapping) const
{
	if (boost::iequals(pdoMapping, "no"))
	{
		return "no";
	}
	else if (boost::iequals(pdoMapping, "TPDO"))
	{
		return "TPDO";
	}
	else if (boost::iequals(pdoMapping, "RPDO"))
	{
		return "RPDO";
	}
	else if (boost::iequals(pdoMapping, "default"))
	{
		return "default";
	}
	else if (boost::iequals(pdoMapping, "optional"))
	{
		return "optional";
	}

	return pdoMapping;
}

Result Node::FixNetworkManagemetAttributes(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string xpathActual = "//NetworkManagement/CNFeatures[@DLLCNFeatureChaining]";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;

				while (attribute && attribute->name && attribute->children)
				{
					// Change the attribute DLLCNFeatureChaining to "DLLCNPResChaining"
					if (boost::iequals((char*) attribute->name, "DLLCNFeatureChaining"))
					{
						xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
						xmlRemoveProp(attribute);
						xmlNewProp(pIndexNode, BAD_CAST "DLLCNPResChaining", BAD_CAST value);
						// xmlSetProp(pIndexNode, BAD_CAST "DLLCNPResChaining", value);
						xmlFree(value);
						LOG_INFO() << "The attribute DLLCNFeatureChaining has been changed to DLLCNPResChaining.";
					}

					attribute = attribute->next;
				}
			}
		}
		else
		{
			// CNFeatures not found and it is not mandatory. So ignore this error.
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::FixApplicationProcess(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string xpathActual = "//ProfileBody/ApplicationProcess";
		xmlXPathObjectPtr pResultingXPathApplicationProcess(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathApplicationProcess)
		{
			xmlNodeSetPtr pAppNodeSet(pResultingXPathApplicationProcess->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pAppNodeSet); ++i)
			{
				xmlNodePtr pAppIndexNode(pAppNodeSet->nodeTab[i]);
				if (pAppIndexNode)
				{
					xmlNodePtr pAppChildNode = pAppIndexNode->children;
					bool dataTypeList = false;
					bool parameterList = false;
					bool templateList = false;
					bool parameterGroupList = false;

					while (pAppChildNode && pAppChildNode->name)
					{
						if (boost::iequals((const char*) pAppChildNode->name, "dataTypeList"))
						{
							if (xmlChildElementCount(pAppChildNode) != 0)
							{
								dataTypeList = true;
							}
						}
						else if (boost::iequals((const char*) pAppChildNode->name, "parameterList"))
						{
							if (xmlChildElementCount(pAppChildNode) != 0)
							{
								parameterList = true;
							}
						}
						else if (boost::iequals((const char*) pAppChildNode->name, "templateList"))
						{
							if (xmlChildElementCount(pAppChildNode) != 0)
							{
								templateList = true;
							}
						}
						else if (boost::iequals((const char*) pAppChildNode->name, "parameterGroupList"))
						{
							if (xmlChildElementCount(pAppChildNode) != 0)
							{
								parameterGroupList = true;
							}
						}
						else if (!boost::iequals((const char*) pAppChildNode->name, "text"))
						{
							LOG_WARN() << "Unhandled tag found. " << (const char*) pAppChildNode->name;
						}

						pAppChildNode = pAppChildNode->next;
					}

					// Remove ApplicationProcess tag if templateList, dataTypeList, parameterGroupList and parameterList has no children.
					// TODO: add acc. to specification. But OCTX can only contain the below.
					if (!dataTypeList && !parameterList && !templateList && !parameterGroupList)
					{
						LOG_WARN() << "Removing ApplicationProcess node from the XML tree.";
						xmlUnlinkNode(pAppIndexNode);
					}
				}
			}
		}
		else
		{
			// No ApplicationProcess tag is found so we can ignore this error.
		}
		xmlXPathFreeObject(pResultingXPathApplicationProcess);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::FixParameterDescription(xmlDocPtr pDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string xpathActual = "//ApplicationProcess/parameterList/parameter";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			std::map<std::string, Parameter> parameterMap;
			xmlNodePtr parameterParent = NULL;

			// Remove all parameters from the XML document pointer and prepare the unique set of parameters.
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr parameterNode(pNodeSet->nodeTab[i]);
				parameterParent = parameterNode->parent;
				xmlAttrPtr attribute = parameterNode->properties;
				Parameter parameter;
				while (attribute && attribute->name && attribute->children)
				{
					xmlChar* value = xmlNodeListGetString(parameterNode->doc, attribute->children, 1);
					if (boost::iequals((char*) attribute->name, "uniqueID"))
					{
						parameter.uniqueID = (const char*) value;
					}
					else if (boost::iequals((char*) attribute->name, "access"))
					{
						parameter.access = CorrectParameterAccessCase((const char*) value);
					}

					xmlFree(value);
					attribute = attribute->next;
				}

				xmlNodePtr dataTypeIdRef = parameterNode->children->next;
				xmlAttrPtr dataTypeIdRefAttribute = dataTypeIdRef->properties;
				while (dataTypeIdRefAttribute && dataTypeIdRefAttribute->name && dataTypeIdRefAttribute->children)
				{
					if (boost::iequals((char*) dataTypeIdRefAttribute->name, "uniqueIDRef"))
					{
						xmlChar* value = xmlNodeListGetString(dataTypeIdRef->doc, dataTypeIdRefAttribute->children, 1);
						parameter.uniqueIDRef = (const char*) value;
						xmlFree(value);

					}
					dataTypeIdRefAttribute = dataTypeIdRefAttribute->next;
				}

				LOG_DEBUG() << "Inserting Parameter " << parameter.uniqueID << " " << parameter.access << " " << parameter.uniqueIDRef;
				parameterMap.insert(std::pair<std::string, Parameter>(parameter.uniqueID, parameter));

				xmlUnlinkNode(parameterNode);
			}

			// Add the unique set of parameters to the XML document
			if (parameterParent)
			{
				for( std::map<std::string, Parameter>::iterator it = parameterMap.begin();
					 it != parameterMap.end();
					 ++it)
				{
					Parameter parameter = (*it).second;
					xmlNodePtr parameterNode = xmlNewNode(0, BAD_CAST "parameter");
					xmlNewProp(parameterNode, BAD_CAST "uniqueID", BAD_CAST parameter.uniqueID.c_str());
					xmlNewProp(parameterNode, BAD_CAST "access", BAD_CAST parameter.access.c_str());

					xmlNodePtr description = xmlNewNode(0, BAD_CAST "description");
					xmlNodePtr descriptionText = xmlNewText( BAD_CAST parameter.uniqueID.c_str() );
					xmlAddChild(description, descriptionText);
					xmlNewProp(description, BAD_CAST "lang", BAD_CAST "en");

					xmlAddChild(parameterNode, description);

					xmlNodePtr dataTypeIDRef = xmlNewNode(0, BAD_CAST "dataTypeIDRef");
					xmlNewProp(dataTypeIDRef, BAD_CAST "uniqueIDRef", BAD_CAST parameter.uniqueIDRef.c_str());
					xmlAddChild(parameterNode, dataTypeIDRef);

					xmlAddChild(parameterParent, parameterNode);
					LOG_DEBUG() << "Writing Parameter " << parameter.uniqueID << " " << parameter.access << " " << parameter.uniqueIDRef;

					//Fix multiple struct with same uniqueID
					Result retVal = RemoveDuplicateDataTypeListStruct(pDoc, parameter.uniqueIDRef);
					if (!retVal.IsSuccessful())
					{
						return retVal;
					}
				}
			}
		}
		else
		{
			// Parameter is not present
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result Node::RemoveDuplicateDataTypeListStruct(xmlDocPtr pDoc, const std::string & uniqueIDRef) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		std::string xpathActual = "//dataTypeList/struct[@uniqueID='";
		xpathActual.append(uniqueIDRef);
		xpathActual.append("']");
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);

			// If not the last struct then we remove the struct itself
			for (int i = 0; i < (xmlXPathNodeSetGetLength(pNodeSet) - 1) ; ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				LOG_WARN() << "Removing the struct with uniqueId: " << uniqueIDRef;
				xmlUnlinkNode(pIndexNode);
			}
		}
		else
		{
			LOG_WARN() << "No tag found: " << xpathActual;
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << " ERROR:" << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

const std::string Node::CorrectParameterAccessCase(const std::string & accessType) const
{
	if (boost::iequals(accessType, "const"))
	{
		return "const";
	}
	else if (boost::iequals(accessType, "read"))
	{
		return "read";
	}
	else if (boost::iequals(accessType, "write"))
	{
		return "write";
	}
	else if (boost::iequals(accessType, "readWrite"))
	{
		return "readWrite";
	}
	else if (boost::iequals(accessType, "readWriteInput"))
	{
		return "readWriteInput";
	}
	else if (boost::iequals(accessType, "readWriteOutput"))
	{
		return "readWriteOutput";
	}
	else if (boost::iequals(accessType, "noAccess"))
	{
		return "noAccess";
	}

	return accessType;
}

Result Node::UpdateXdcWithObjectActualValues(xmlDocPtr pOctxDoc, xmlDocPtr pXdcDoc) const
{
	LOG_TRACE() << __FUNCTION__ ;
	try
	{
		const std::string xpathActual = "//ObjectList/Object[@index]";
		xmlXPathObjectPtr pResultingXPathObject(GetNodeSet(pOctxDoc, xpathActual));
		if (pResultingXPathObject)
		{
			xmlNodeSetPtr pNodeSet(pResultingXPathObject->nodesetval);
			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;
				std::string indexId;
				std::string actualValue;
				std::string defaultValue; // Not used. Do we need to check?
				while (attribute && attribute->name && attribute->children)
				{
					if (boost::iequals((char*) attribute->name, "index"))
					{
						xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
						indexId = (const char*) value;
						xmlFree(value);
					}
					else if (boost::iequals((char*) attribute->name, "actualValue"))
					{
						xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
						actualValue = (const char*) value;
						xmlFree(value);
					}
					else
					{
						// Other attributes are ignored.
					}

					attribute = attribute->next;
				}
				xmlFreePropList(attribute);

				//TODO: do we need to check the actual and default values from the XDC?
				/**
				  * Update the 'actualvalue' field fetched from '.octx' file to '.xdc' file only when
				  * the fetched 'indexId' and 'actualValue' are not empty.
				  */
				if (!indexId.empty())
				{
					if (!actualValue.empty())
					{
						std::string xpath = "//plk:Object[@index='";
						xpath.append(indexId);
						xpath.append("']");

						xmlXPathObjectPtr pXPathObject(GetXdcNodeSet(pXdcDoc, xpath));
						if (pXPathObject)
						{
							xmlNodeSetPtr pObjectList(pXPathObject->nodesetval);
							for (int j = 0; j < xmlXPathNodeSetGetLength(pObjectList); ++j)
							{
								xmlNodePtr pObjectNodePtr(pObjectList->nodeTab[j]);
								xmlAttrPtr actualValueProp = xmlHasProp(pObjectNodePtr, BAD_CAST "actualValue");
								if (actualValueProp)
									xmlRemoveProp(actualValueProp);
								xmlNewProp(pObjectNodePtr, BAD_CAST "actualValue", BAD_CAST actualValue.c_str());

								// xmlFreeNode(pObjectNodePtr);
							}
							// xmlXPathFreeNodeSet(pObjectList);
						}
						else
						{
							LOG_WARN() << "NodeID:" << this->id << " Object(0x" << indexId << ") ActualValue(" << actualValue << ")" << " not updated.";
						}
						xmlXPathFreeObject(pXPathObject);
					}

					/**
					  * Update the corresponding subObject actualValue
					  */
					Result retVal = UpdateXdcWithSubObjectActualValues(pOctxDoc, pXdcDoc, indexId);
					if (!retVal.IsSuccessful())
					{
						return retVal;
					}
				}
			}
			// xmlXPathFreeNodeSet(pNodeSet);
		}
		else
		{
			// NO Object is present - so we can ignore this error.
		}
		xmlXPathFreeObject(pResultingXPathObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << "ERROR: " << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

Result Node::UpdateXdcWithSubObjectActualValues(xmlDocPtr pOctxDoc, xmlDocPtr pXdcDoc, const std::string & objectId) const
{
	LOG_TRACE() << __FUNCTION__ << " ObjectID:" << objectId;
	try
	{
		std::string xpath = "//Object[@index='";
		xpath.append(objectId);
		xpath.append("']");
		xpath.append("/SubObject[@actualValue]");
		xmlXPathObjectPtr pOctxXPathSubObject(GetNodeSet(pOctxDoc, xpath));
		if (pOctxXPathSubObject)
		{
			xmlNodeSetPtr pNodeSet(pOctxXPathSubObject->nodesetval);

			for (int i = 0; i < xmlXPathNodeSetGetLength(pNodeSet); ++i)
			{
				xmlNodePtr pIndexNode(pNodeSet->nodeTab[i]);
				xmlAttrPtr attribute = pIndexNode->properties;
				std::string subObjectId;
				std::string actualValue;
				std::string defaultValue; // Not used. Do we need to check?
				while (attribute && attribute->name && attribute->children)
				{
					if (boost::iequals((char*) attribute->name, "subIndex"))
					{
						xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
						subObjectId = (const char*) value;
						xmlFree(value);
					}
					else if (boost::iequals((char*) attribute->name, "actualValue"))
					{
						xmlChar* value = xmlNodeListGetString(pIndexNode->doc, attribute->children, 1);
						actualValue = (const char*) value;
						xmlFree(value);
					}
					else
					{
						// Other attributes are ignored.
					}

					attribute = attribute->next;
				}
				xmlFreePropList(attribute);

				//TODO: do we need to check the actual and default value from the XDC?
				/**
				  * Update the 'actualvalue' fetched from '.octx' to '.xdc' only when
				  * the fetched 'subObjectId' and 'actualValue' are not empty.
				  */
				if (!subObjectId.empty() && !actualValue.empty())
				{
					xpath = "//plk:Object[@index='";
					xpath.append(objectId);
					xpath.append("']");
					xpath.append("/plk:SubObject[@subIndex='");
					xpath.append(subObjectId);
					xpath.append("']");

					xmlXPathObjectPtr pXdcXPathSubObject(GetXdcNodeSet(pXdcDoc, xpath));
					if (pXdcXPathSubObject)
					{
						xmlNodeSetPtr pSubObjectList(pXdcXPathSubObject->nodesetval);
						for (int j = 0; j < xmlXPathNodeSetGetLength(pSubObjectList); ++j)
						{
							xmlNodePtr pSubObjectNodePtr(pSubObjectList->nodeTab[j]);
							xmlAttrPtr actualValueProp = xmlHasProp(pSubObjectNodePtr, BAD_CAST "actualValue");
							if (actualValueProp)
								xmlRemoveProp(actualValueProp);
							xmlNewProp(pSubObjectNodePtr, BAD_CAST "actualValue", BAD_CAST actualValue.c_str());

							// xmlFreeNode(pSubObjectNodePtr);
						}
						// xmlXPathFreeNodeSet(pSubObjectList);
					}
					else
					{
						LOG_WARN() << "NodeID:" << this->id << " Object(0x" << objectId << ") SubObject(0x" << subObjectId << ") ActualValue(" << actualValue << ")" << " not updated.";
					}
					xmlXPathFreeObject(pXdcXPathSubObject);
				}
			}
			// xmlXPathFreeNodeSet(pNodeSet);
		}
		else
		{
			//  No subObject is present - so we can ignore this error.
		}
		xmlXPathFreeObject(pOctxXPathSubObject);
	}
	catch (const std::exception & ex)
	{
		LOG_DEBUG() << "ERROR: " << ex.what();
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	return Result(ErrorCode::SUCCESS);
}

PROJECT_UPGRADE_CLOSE_NAMESPACE
