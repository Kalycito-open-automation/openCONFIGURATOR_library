/*******************************************************************************
* \file   UpgradeAPI.h
*
* \brief  Header file that includes the interface to the project upgrade library.
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

#ifndef _PROJECT_UPGRADE_UPGRADER_API_H_
#define _PROJECT_UPGRADE_UPGRADER_API_H_

#include <vector>
#include <string>

#include "Namespace.h"
#include "Exports.h"
#include "Result.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

namespace API
{
	/** \addtogroup projectupgrade
	* @{
	*/

	/**
	 * \brief    Returns the nodeIDs of all existing nodes in the project.
	 *
	 * \param[out] nodeIDs of the existing nodes.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result GetNodes(std::vector<unsigned int>& nodeIDs);

	/**
	 * \brief    Returns the name of the node.
	 *
	 * \param[in] nodeID  The ID of the node.
	 * \param[out] name   The name of the node.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result GetNodeName(const unsigned int nodeID, std::string & name);

	/**
	 * \brief   Returns the location of the '.octx' file of the node.
	 *
	 * \param[in] nodeID        The ID of the node.
	 * \param[out] octxFilePath The path of the OCTX file.
	 * \param[in] shortPath     False: Full system path.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result GetOctxPath(const unsigned int nodeID, std::string & octxFilePath, bool shortPath = true);

	/**
	 * \brief   Sets the location of the XDD/XDC file as the path for the input XDD.
	 *
	 * \param[in] nodeID  The ID of the node.
	 * \param[in] xddPath The path of the input XDD (an alternate configuration).
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result SetXddPath(const unsigned int nodeID, const std::string & xddPath);

	/**
	 * \brief   Reads and parses the '.oct' project file.
	 *
	 * \param[in] projectFile path of the project file.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result ImportProjectFile(const std::string & projectFile);

	/**
	 * \brief   Upgrade the pre v1.4.0 '.oct' project file to the new '.xml' project file 
	 *          format and convert all the '.octx' files to the '.xdc' files.
	 *
	 * \param[in] projectPath Path for the new project.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result UpgradeProject(const std::string & projectPath);

	/**
	 * \brief    Returns the new project file path with the name and extension.
	 * \param[out] newProjectFilePath The path of the new project file.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result GetNewProjectFilePath(std::string & newProjectFilePath);
	
	/**
	 * \brief   Resets the project upgrade library to its initial state.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result ResetProjectUpgradeLib();
	
	/**
	 * \brief   Deletes the new project folder and its contents and restores the
	 *          old project to its location.
	 * \return Result.
	 */
	PROJECT_UPGRADE_DLLEXPORT Result RevertUpgradeProject();

	/** @} */
} // API

PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_UPGRADER_API_H_
