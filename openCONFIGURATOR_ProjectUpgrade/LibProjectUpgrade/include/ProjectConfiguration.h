/*******************************************************************************
* \file   ProjectConfiguration.h
*
* \brief  Header file that includes functions to retrieve project configuration 
*         from the project file.
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

#ifndef _PROJECT_UPGRADE_PROJECT_CONFIGURATION_H_
#define _PROJECT_UPGRADE_PROJECT_CONFIGURATION_H_

#include <vector>
#include <boost/filesystem.hpp>

#include "NodeCollection.h"
#include "Constants.h"
#include "Result.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

/**
 *	Auto generate options.
 */
namespace AutoGenerate
{
	enum AutoGenerate
	{
		UNDEFINED = 0,
		YES,
		NO
	};
}

/**
 *	Save options.
 */
namespace AutoSave
{
	enum AutoSave
	{
		UNDEFINED = 0,
		AUTO_SAVE,
		PROMPT,
		DISCARD
	};
}

/**
 *	GUI view types.
 */
namespace ViewMode
{
	enum ViewMode
	{
		UNDEFINED = 0,
		SIMPLE,
		ADVANCED
	};
}

/**
 * \brief This class provides the interface for the configurations in 
 *        the openCONFIGURATOR project.
 */
class ProjectConfiguration
{
	public:
		static ProjectConfiguration& GetInstance();

		const std::string& GetOctProjectFile() const;
		void SetOctProjectFile(const std::string& octProjectFile);

		const std::string& GetNewProjectFile() const;
		void SetNewProjectFile(const std::string& newProjectFile);

		const boost::filesystem::path& GetOctProjectPath() const;
		void SetOctProjectPath(const boost::filesystem::path& octProjectPath);

		const boost::filesystem::path& GetNewProjectPath() const;
		void SetNewProjectPath(const boost::filesystem::path& newProjectPath);

		const std::string GetProjectName() const;

		const std::string& GetOldProjectVersion() const;
		void SetOldProjectVersion(const std::string& version);

		bool IsVersionValid() const;

		bool IsInitialized() const;
		void SetInitialized(bool initialized);

		void ResetConfiguration();

		/**
		 * \brief Moves the old project to a location with suffix '_oldProjectVersion'.
		 * \return Result.
		 */
		Result BackupOldProject();

		/**
		 * \brief Creates directories for the new project.
		 * \return Result.
		 */
		Result CreateNewProjectDirectories() const;

		/**
		 * \brief Imports the '.oct' project file.
		 * \param[in] octProjectFile Path of the oct project file.
		 * \return Result.
		 */
		Result ImportProject(const std::string & octProjectFile);

		/**
		 * \brief Writes the upgraded format of the project file to the given path.
		 * \param[in] projectPath Path for the new project file.
		 * \return Result.
		 */
		Result WriteProjectFile(const std::string & projectPath);

		/**
		 * \brief Validate the project file with the project schema.
		 * \param[in] fileName Path of the project file.
		 * \return Result.
		 */
		Result ValidateProjectFile(const std::string & fileName) const;

		/**
		 * \brief Imports all the configuration files from the old project to the deviceImport folder.
		 * Converts the 'octx' format to XDC format by adding the missing fields/tags to the 'octx' file.
		 * \return Result.
		 */
		Result ImportAllXddFiles();

		/**
		 * \brief Copies all the XDD/XDC files to the deviceConfiguration folder.
		 *        Updates the 'actualValue' field in XDC with 'actualValue' field from the 'octx' file.
		 * \return Result.
		 */
		Result ImportAllConfigurationsFromDeviceImportDir();

		/**
		 * \brief Validates the XDD file according to the schema.
		 * \param[in] fileName Path to the XDD/XDC file.
		 * \return Result.
		 */
		Result ValidateXDDFile(const std::string & fileName) const;

	private:
		static ProjectConfiguration instance;
		ProjectConfiguration();
		ProjectConfiguration(const ProjectConfiguration&);
		void operator=(const ProjectConfiguration&);

		bool initialized;  ///< Initialise once the project configuration has been read successfully.

		/// OCT project file configuration parameters.
		boost::filesystem::path octProjectPath;
		std::string octProjectFile;
		std::string octProjectVersion;
		AutoGenerate::AutoGenerate autoGenerate;
		AutoSave::AutoSave autoSave;
		ViewMode::ViewMode viewMode;

		/// XML (new) project file configuration parameters.
		boost::filesystem::path newProjectPath;
		std::string newProjectFile;
};

PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_PROJECT_CONFIGURATION_H_
