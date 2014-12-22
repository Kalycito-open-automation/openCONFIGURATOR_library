/*******************************************************************************
* \file   UpgradeAPI.cpp
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

#include "../include/UpgradeAPI.h"
#include "../include/ProjectConfiguration.h"
#include "../include/BasicTypes.h"
#include "../include/Logging.h"

#include <iostream> // To remove

#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>

PROJECT_UPGRADE_OPEN_NAMESPACE

const std::string kProjectFileExtensionNew = ".xml";
const std::string kProjectFileExtensionOld = ".oct";

namespace API
{
Result GetNodes(std::vector<UINT32>& nodeIds)
{
	try
	{
		if (!ProjectConfiguration::GetInstance().IsInitialized())
		{
			return Result(ErrorCode::NO_PROJECT_LOADED, kMsgNoProjectLoaded);
		}

		nodeIds = NodeCollection::GetInstance().GetNodeIds();
	}
	catch (const std::exception& ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}

	return Result(ErrorCode::SUCCESS);
}

Result GetNodeName(const UINT32 nodeId, std::string & name)
{
	try
	{
		if (!ProjectConfiguration::GetInstance().IsInitialized())
		{
			return Result(ErrorCode::NO_PROJECT_LOADED, kMsgNoProjectLoaded);
		}

		name = NodeCollection::GetInstance().GetNodeRef(nodeId).GetName();
	}
	catch (const std::exception& ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	catch (Result & ex)
	{
		return ex;
	}

	return Result(ErrorCode::SUCCESS);
}

Result GetOctxPath(const UINT32 nodeId, std::string & octxFilePath, bool shortPath)
{
	try
	{
		if (!ProjectConfiguration::GetInstance().IsInitialized())
		{
			return Result(ErrorCode::NO_PROJECT_LOADED, kMsgNoProjectLoaded);
		}

		boost::filesystem::path octxPath = NodeCollection::GetInstance().GetNodeRef(nodeId).GetOctxPath();
		if (shortPath)
		{
			octxFilePath =  octxPath.generic_string();
		}
		else
		{
			// Returns the project path and '.octx' path
			octxFilePath =  (ProjectConfiguration::GetInstance().GetOctProjectPath().generic_string()
							 + kPathSeparator
							 + octxPath.generic_string());
		}
	}
	catch (const std::exception& ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	catch (Result & ex)
	{
		return ex;
	}

	return Result(ErrorCode::SUCCESS);
}

Result SetXddPath(const UINT32 nodeId, const std::string & xddPath)
{
	try
	{
		if (!ProjectConfiguration::GetInstance().IsInitialized())
		{
			return Result(ErrorCode::NO_PROJECT_LOADED, kMsgNoProjectLoaded);
		}

		if (!xddPath.empty())
		{
			//TODO: Check for file name or '.xdd' or '.xdc'
			Result retVal = ProjectConfiguration::GetInstance().ValidateXDDFile(xddPath);
			if (!retVal.IsSuccessful())
			{
				return retVal;
			}
		}

		NodeCollection::GetInstance().GetNodeRef(nodeId).SetInputXddPath(xddPath);
	}
	catch (const std::exception& ex)
	{
		return Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
	}
	catch (Result & ex)
	{
		return ex;
	}

	return Result(ErrorCode::SUCCESS);
}

Result ImportProjectFile(const std::string & projectFile)
{
	try
	{
		// Resets the project configuration data.
		ProjectConfiguration::GetInstance().ResetConfiguration();

		/*
		 *  Validates the project file with its schema.
		 * If successful, there is no need to process the new XML format project file.
		 */
		Result retVal = ProjectConfiguration::GetInstance().ValidateProjectFile(projectFile);
		if (retVal.IsSuccessful())
		{
			LOG_INFO() << "This is the new project file format which passes the XML schema validation." << std::endl;
			//TODO: Return error.
			return retVal;
		}

		/*
		 * Check for the project file extension.
		 * if XML, return the result of the schema validation.
		 * if OCT, proceed with conversion
		 * if other, return with error 'Invalid file'.
		 */
		const boost::filesystem::path newProjectFile(projectFile);
		const std::string projectFileExtension(newProjectFile.extension().generic_string());
		if (boost::iequals(projectFileExtension, kProjectFileExtensionNew))
		{
			/* This might be one of the following:
			 * 1. New project file format which didn't pass schema validation.
			 * 2. '.oct' project file with extension '.xml'.
			 */
			LOG_FATAL() << "Invalid or corrupted new project file format. ErrorCode: " << retVal.GetErrorCode() << "	" << retVal.GetErrorString();
			return retVal; // Returns the result from the ValidateProjectFile
		}
		else if (boost::iequals(projectFileExtension, kProjectFileExtensionOld))
		{
			retVal = ProjectConfiguration::GetInstance().ImportProject(projectFile);
			if (!retVal.IsSuccessful())
			{
				// OCT Project file parse failed with errors.
				LOG_FATAL() << "OCT project file parse failed with errors. ErrorCode: " << retVal.GetErrorCode() << "	" << retVal.GetErrorString();
				return retVal;
			}

			if (!ProjectConfiguration::GetInstance().IsVersionValid())
			{
				// Old project version is !< new project version. So Throw error and reset the project configuration.
				LOG_FATAL() << "Old project version is newer than version 1.3.1." << std::endl;

				ProjectConfiguration::GetInstance().ResetConfiguration();

				boost::format formatter(kMsgProjectVersionInvalid);
				formatter % ProjectConfiguration::GetInstance().GetOldProjectVersion();
				return Result(ErrorCode::PROJECT_VERSION_INVALID, formatter.str());
			}

			// The project has been read successfully and has been initialized.
			ProjectConfiguration::GetInstance().SetInitialized(true);
		}
		else
		{
			// Return error as "invalid project file extension"
			boost::format formatter(kMsgInvalidProjectFileExtension);
			formatter % projectFileExtension % kProjectFileExtensionOld;
			formatter % kProjectFileExtensionNew;
			return Result(ErrorCode::PROJECT_FILE_INVALID_EXTENSION, formatter.str());
		}
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

Result UpgradeProject(const std::string & newProjectPath)
{
	try
	{
		if (!ProjectConfiguration::GetInstance().IsInitialized())
		{
			return Result(ErrorCode::NO_PROJECT_LOADED, kMsgNoProjectLoaded);
		}

		Result retVal = ProjectConfiguration::GetInstance().BackupOldProject();
		if (!retVal.IsSuccessful())
		{
			return retVal;
		}

		//TODO: Check for the newProjectPath same as the projectBackup path?
		ProjectConfiguration::GetInstance().SetNewProjectPath(newProjectPath);

		const std::string logFile = newProjectPath + kPathSeparator + kProjectUpgradeLogFile ;

		InitProjectUpgradeLogging(logFile);

		LOG_INFO() << "The old project has been moved to a new location: " << ProjectConfiguration::GetInstance().GetOctProjectPath().generic_string() << std::endl;
		LOG_INFO() << "Creating new project in the location: " << newProjectPath;

		retVal = ProjectConfiguration::GetInstance().CreateNewProjectDirectories();
		if (!retVal.IsSuccessful())
		{
			LOG_FATAL() << "ErrorCode: " << retVal.GetErrorCode() << "	" << retVal.GetErrorString();
			DisableLogging();
			return retVal;
		}

		retVal = ProjectConfiguration::GetInstance().ImportAllXddFiles();
		if (!retVal.IsSuccessful())
		{
			LOG_FATAL() << "ErrorCode: " << retVal.GetErrorCode() << "	" << retVal.GetErrorString();
			DisableLogging();
			return retVal;
		}

		retVal = ProjectConfiguration::GetInstance().ImportAllConfigurationsFromDeviceImportDir();
		if (!retVal.IsSuccessful())
		{
			LOG_FATAL() << "ErrorCode: " << retVal.GetErrorCode() << "	" << retVal.GetErrorString();
			DisableLogging();
			return retVal;
		}

		/*
		 * After converting all the '.octx' files, write the project XML file
		 */
		retVal = ProjectConfiguration::GetInstance().WriteProjectFile(newProjectPath);
		if (!retVal.IsSuccessful())
		{
			LOG_FATAL() << "ErrorCode: " << retVal.GetErrorCode() << "	" << retVal.GetErrorString();
			DisableLogging();
			return retVal;
		}
	}
	catch (const std::exception & ex)
	{
		Result retVal = Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
		LOG_FATAL() << "ErrorCode: " << retVal.GetErrorCode() << "	" << retVal.GetErrorString();
		DisableLogging();
		return retVal;
	}
	catch (Result & ex)
	{
		LOG_FATAL() << "ErrorCode: " << ex.GetErrorCode() << "	" << ex.GetErrorString();
		DisableLogging();
		return ex;
	}

	LOG_INFO() << "The project " << ProjectConfiguration::GetInstance().GetProjectName() << " has been upgraded successfully.";
	DisableLogging();
	return Result(ErrorCode::SUCCESS);
}

Result GetNewProjectFilePath(std::string & newProjectFilePath)
{
	try
	{
		if (!ProjectConfiguration::GetInstance().IsInitialized())
		{
			return Result(ErrorCode::NO_PROJECT_LOADED, kMsgNoProjectLoaded);
		}
		newProjectFilePath = ProjectConfiguration::GetInstance().GetNewProjectPath().generic_string()
								+ kPathSeparator
								+ ProjectConfiguration::GetInstance().GetNewProjectFile();
	}
	catch (boost::filesystem::filesystem_error & ex)
	{
		return Result(ErrorCode::FILE_SYSTEM_ERROR, ex.what());
	}
	catch (const std::exception & ex)
	{
		Result retVal = Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
		return retVal;
	}
	catch (Result & ex)
	{
		return ex;
	}
	return Result(ErrorCode::SUCCESS);
}

Result ResetProjectUpgradeLib()
{
	try
	{
		// Resets the project configuration data.
		ProjectConfiguration::GetInstance().ResetConfiguration();
	}
	catch (const std::exception & ex)
	{
		Result retVal = Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
		return retVal;
	}
	catch (Result & ex)
	{
		return ex;
	}
	return Result(ErrorCode::SUCCESS);
}

Result RevertUpgradeProject()
{
	try
	{
		if (!ProjectConfiguration::GetInstance().IsInitialized())
		{
			return Result(ErrorCode::NO_PROJECT_LOADED, kMsgNoProjectLoaded);
		}

		if (ProjectConfiguration::GetInstance().GetNewProjectPath().empty())
		{
			return Result(ErrorCode::SUCCESS, "There is no change in the project.");
		}

		// Remove the new project folder and contents.
		const boost::filesystem::path newProjectPath(ProjectConfiguration::GetInstance().GetNewProjectPath());
		if (boost::filesystem::exists(newProjectPath))
		{
			boost::filesystem::remove_all(newProjectPath);
		}

		// Move the '.oct' project contents to its original directory.
		boost::filesystem::path octPjtPath = ProjectConfiguration::GetInstance().GetOctProjectPath();

		std::string foldername = octPjtPath.filename().generic_string();
		const std::string pathToRemove("_" + ProjectConfiguration::GetInstance().GetOldProjectVersion());

		const std::string::size_type n = pathToRemove.length();

		for (std::string::size_type i = foldername.find(pathToRemove);
			 i != std::string::npos;
			 i = foldername.find(pathToRemove))
		{
			foldername.erase(i, n);
		}

		boost::filesystem::path newPath = octPjtPath.parent_path().generic_string() + kPathSeparator + foldername;

		boost::filesystem::rename(octPjtPath, newPath);

		// Reverted successfully. Update the project path.

		ProjectConfiguration::GetInstance().SetOctProjectPath(newPath);

		ProjectConfiguration::GetInstance().SetNewProjectPath("");
	}
	catch (boost::filesystem::filesystem_error & ex)
	{
		return Result(ErrorCode::FILE_SYSTEM_ERROR, ex.what());
	}
	catch (const std::exception & ex)
	{
		Result retVal = Result(ErrorCode::UNHANDLED_EXCEPTION, ex.what());
		return retVal;
	}
	catch (Result & ex)
	{
		return ex;
	}

	return Result(ErrorCode::SUCCESS);
}

} // API

PROJECT_UPGRADE_CLOSE_NAMESPACE

