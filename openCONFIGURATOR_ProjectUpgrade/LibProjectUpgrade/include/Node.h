/*******************************************************************************
* \file   Node.h
*
* \brief  Header file that includes functions to handle properties of a Node.
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

#ifndef _PROJECT_UPGRADE_NODE_H_
#define _PROJECT_UPGRADE_NODE_H_

#include <string>
#include <boost/filesystem/path.hpp>
#include <libxml/tree.h>

#include "BasicTypes.h"
#include "Result.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

/**
 *	The station type of the Node.
 */
namespace StationType
{
	enum StationType
	{
		UNDEFINED = 0,
		NORMAL,
		CHAINED,
		MULTIPLEXED
	};
}

/**
 *	The type of the Node.
 */
namespace NodeType
{
	enum NodeType
	{
		UNDEFINED = 0,
		MN,
		CN
	};
}

/**
 * \brief The members of the 'Parameter' section in the XDD/XDC of a Node.
 */
struct Parameter
{
		std::string uniqueID;
		std::string access;
		std::string uniqueIDRef;

		Parameter() :
			uniqueID(),
			access(),
			uniqueIDRef()
		{
		}
};

/**
 * \brief Class that describes the properties of a Node.
 *
 * The properties of a Node and its configuration file.
 */
class Node
{
	public:
		Node();

		const std::string GetName() const;
		void SetName(const std::string & name);

		const boost::filesystem::path GetOctxPath(bool shortPath = true) const;
		void SetOctxPath(const boost::filesystem::path& octxPath);

		const boost::filesystem::path& GetInputXddPath() const;
		void SetInputXddPath(const boost::filesystem::path& inputXddPath);

		const boost::filesystem::path GetXddPath(bool shortPath = true) const;
		void SetXddPath(const boost::filesystem::path& xddPath);

		const boost::filesystem::path GetXdcPath(bool shortPath = true) const;
		void SetXdcPath(const boost::filesystem::path& xdcPath);

		UINT32 GetNodeId() const;
		void SetNodeId(const UINT32 nodeId);

		StationType::StationType GetStationType() const;
		void SetStationType(StationType::StationType stationType);

		void SetNodeType(NodeType::NodeType nodeType);
		NodeType::NodeType GetNodeType() const;

		bool isForcedCycle() const;
		void SetForceCycleFlag(bool flag);

		UINT32 GetForcedCycleValue() const;
		void SetForcedCycleValue(const UINT32 value);

		/**
		 * \brief Updates the octx file with the missing tags and elements.
		 * \return The result.
		 */
		Result UpdateOctxMissingFields() const;

		/**
		 * \brief Updates the XDC present in the deviceConfiguration directory with
		 *			the actual values from octx file.
		 * \return The result.
		 */
		Result UpdateXdcWithValuesFromOctx() const;

	private:
		UINT32 id;
		std::string name;
		StationType::StationType stationType;
		NodeType::NodeType nodeType;
		bool forceCycleFlag;
		UINT32 forcedCycleValue;

		boost::filesystem::path octxPath; ///< Short path of the octx file.

		boost::filesystem::path inputXddPath; ///< The path of the alternative configuration.

		boost::filesystem::path xddPath; ///< The path to the deviceImport file.
		boost::filesystem::path xdcPath; ///< The path to the deviceConfiguration file.

		/**
		 * \brief Adds the missing elements of the 'ISO15745ProfileContainer'
		 *			tag with default values to the specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result
		 */
		Result UpdateISO15745ProfileContainer(xmlDocPtr pDoc) const;

		/**
		 * \brief Fixes the misplaced 'ISO15745Profile' tag in the octx file.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result
		 */
		Result FixISO15745ProfileTag(xmlDocPtr pDoc) const;

		/**
		 * \brief Adds the missing elements of the 'ProfileHeader'
		 *			tag with default values to the specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result
		 */
		Result AddProfileHeader(xmlDocPtr pDoc) const;

		/**
		 * \brief Adds the missing elements of the 'ProfileBody'
		 *			tag with default values to the specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result
		 */
		Result UpdateProfileBodyInformation(xmlDocPtr pDoc) const;

		/**
		 * \brief Fix issues with the 'ApplicationProcess' tag to the specified 
		 *        XML document reference.
		 *
		 * \note If there are no children for dataTypeList,
		 *			parameterList, templateList, parameterGroupList sections
		 *			in the octx, then the 'ApplicationProcesss' tag is removed 
		 *          from the specified XML document reference.
		 *
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result
		 */
		Result FixApplicationProcess(xmlDocPtr pDoc) const;

		/**
		 * \brief Adds the missing elements of the 'parameter' tag with
		 *		default values to the specified XML document reference.
		 *
		 * \note Includes fix for the non-unique parameter list and
		 *			adds the 'description' child with default values.
		 *
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result.
		 */
		Result FixParameterDescription(xmlDocPtr pDoc) const;

		/**
		 * \brief Removes the duplicate 'Datatypelist/Struct' elements from the 
		 *        specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \param[in] uniqueIDRef The reference ID.
		 * \return Result.
		 */
		Result RemoveDuplicateDataTypeListStruct(xmlDocPtr pDoc,
												const std::string & uniqueIDRef) const;

		/**
		 * \brief Corrects the lower/upper case presentation of access type of 
		 *        the parameter.
		 * \param[in] accessType The access type string.
		 * \return The corrected access type string.
		 */
		const std::string CorrectParameterAccessCase(const std::string & accessType) const;

		/**
		 * \brief Adds the missing elements of the 'ApplicationLayer'
		 *			tag with default values to the specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result.
		 */
		Result AddApplicationLayerIdentityInfo(xmlDocPtr pDoc) const;

		/**
		 * \brief Adds the missing elements of the 'TransportLayers'
		 *			tag with default values to the specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result.
		 */
		Result AddTransportLayers(xmlDocPtr pDoc) const;

		/**
		 * \brief Cleans up the attributes and values of the Object/subObject at
		 *        the specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result.
		 */
		Result CleanupObjectAttributes(xmlDocPtr pDoc) const;

		/**
		 * \brief Corrects the lower/upper case presentation of the PDO mapping value.
		 * \param[in] pdoMapping PDO mapping value in string.
		 * \return The fixed PDO mapping string.
		 */
		const std::string FixPdoMappingCase(const std::string & pdoMapping) const;

		/**
		 * \brief Fixes the errors within the NetworkManagement attributes at 
		 *        the specified XML document reference.
		 * \param[in,out] pDoc The reference to the XML document.
		 * \return Result.
		 */
		Result FixNetworkManagemetAttributes(xmlDocPtr pDoc) const;

		/**
		 * \brief Updates the XDC with actual values from the octx/input XDD 
		 *        file at the specified XML document reference.
		 * \param[in] pOctxDoc    The input octx/XDD document reference.
		 * \param[in,out] pXdcDoc The XDC document reference.
		 * \return Result.
		 */
		Result UpdateXdcWithObjectActualValues(xmlDocPtr pOctxDoc, xmlDocPtr pXdcDoc) const;

		/**
		 * \brief Updates the actualValues from the octx/input XDD file to the
		 *        subObjects of the given ObjectID in the XDC at the 
		 *        specified XML document reference.
		 * \param[in] pOctxDoc    The input octx/XDD document reference.
		 * \param[in,out] pXdcDoc The XDC document reference.
		 * \param[in] objectId    The ID of the object.
		 * \return Result
		 */
		Result UpdateXdcWithSubObjectActualValues(xmlDocPtr pOctxDoc, 
												xmlDocPtr pXdcDoc,
												const std::string & objectId) const;
};

PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_NODE_H_
