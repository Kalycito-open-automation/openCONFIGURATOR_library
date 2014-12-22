/*******************************************************************************
* \file   main.cpp
*
* \brief  Main file for the console application that is used to upgrade pre 
*         v1.4.0 openCONFIGURATOR projects to v1.4.0
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
#include <string>
#include <boost/algorithm/string.hpp>
#include <boost/filesystem.hpp>

#include "UpgradeAPI.h"
#include "Result.h"

using namespace openCONFIGURATOR::ProjectUpgrade;
using namespace openCONFIGURATOR::ProjectUpgrade::API;

const std::string kVersion = "1.1.0";

/**
 *	Opens the log file that is generated during upgrade of the openCONFIGURATOR project.
 */
void ShowUpgradeLog(const std::string & newProjectPath);

void PrintHelpMessage(const std::string & applicationFileName);

int main(int argc, char* argv[])
{
	std::cout << " --------------------------------------------------------------------------------" << std::endl;
	std::cout << "             openCONFIGURATOR project upgrade utility - v" << kVersion << "      " << std::endl;
	std::cout << "                        Kalycito Infotech Private Limited                        " << std::endl;
	std::cout << " --------------------------------------------------------------------------------" << std::endl;

	std::cout << "\nUpgrades pre v1.4.0 openCONFIGURATOR projects to v1.4.0" << std::endl;

	boost::filesystem::path applicationPath = argv[0];
	std::string applicationFileName = applicationPath.stem().string();
#if defined(_WIN32)
	applicationFileName += ".exe";
#endif

	PrintHelpMessage(applicationFileName);

	if (argc <= 1)
	{
		std::getchar();
		return 0;
	}

	std::string arguments(argv[1]);

	if (boost::iequals(arguments, "--i") || boost::iequals(arguments, "-i"))
	{
		if (argc < 3)
		{
			std::cout << "Enter the path of the project file." << std::endl;
			std::getchar();
			return 0;
		}

		boost::filesystem::path projectFile = std::string(argv[2]);

		Result ret = ImportProjectFile(projectFile.generic_string());
		if (ret.IsSuccessful())
		{
			std::vector<unsigned int> nodeIds;
			ret = API::GetNodes(nodeIds);
			if (!ret.IsSuccessful())
			{
				std::cout << "ERROR: " << ret.GetErrorString() << std::endl;
			}

			if (nodeIds.size())
			{
				std::cout << "\nEnter the XDD/XDC path for the following nodes if available..." << std::endl;

				for (std::vector<unsigned int>::iterator it = nodeIds.begin(); it != nodeIds.end(); ++it)
				{
					unsigned int nodeId = *it;
					std::string xddPath;
					std::string nodeName;
					std::string octxPath;

					ret = GetOctxPath(nodeId, octxPath);
					if (!ret.IsSuccessful())
					{
						std::cout << "ERROR: " << ret.GetErrorString() << std::endl;
					}

					ret = GetNodeName(nodeId, nodeName);
					if (!ret.IsSuccessful())
					{
						std::cout << "ERROR: " << ret.GetErrorString() << std::endl;
					}

					std::cout << "NodeID:" << nodeId << " Name:" << nodeName << " Octx:" << octxPath << " xddPath:";
					std::getline(std::cin, xddPath);

					if (!xddPath.empty())
					{
						ret = SetXddPath(nodeId, xddPath);
						if (!ret.IsSuccessful())
						{
							std::cout << "ERROR: " << ret.GetErrorString() << std::endl;
							continue;
						}
					}
				}

				boost::filesystem::path newProjectFile = projectFile;
				newProjectFile.replace_extension(".xml");

				ret = UpgradeProject(newProjectFile.parent_path().generic_string());
				if (!ret.IsSuccessful())
				{
					std::cout << "Project upgrade failed. ERROR: " << ret.GetErrorString() << std::endl;

					ShowUpgradeLog(newProjectFile.parent_path().generic_string());

					std::cout << "\nDo you want to revert the upgrade(y/n):";
					std::string input;
					std::getline(std::cin, input);
					if (boost::iequals(input, "y"))
					{
						ret = RevertUpgradeProject();
						if (!ret.IsSuccessful())
						{
							std::cout << "ERROR: " << ret.GetErrorString() << std::endl;
						}
					}
				}
				else
				{
					std::cout << "\nThe project has been upgraded successfully. ";
					std::string newProjectFilePath;
					ret = GetNewProjectFilePath(newProjectFilePath);
					if (ret.IsSuccessful())
					{
						std::cout << "The new project file is located at " << newProjectFilePath << std::endl;
					}
					ShowUpgradeLog(newProjectFile.parent_path().generic_string());
				}
			}
		}
		else
		{
			std::cout << "ERROR: " << ret.GetErrorString() << std::endl;
		}
	}
	else if (boost::iequals(arguments, "--h") || boost::iequals(arguments, "-h"))
	{
		PrintHelpMessage(applicationFileName);
	}
	else
	{
		std::cout << "The given arguments are not supported." << std::endl;
	}

	return 0;
}

void PrintHelpMessage(const std::string & applicationFileName)
{
	std::cout << "\nUsage: " << applicationFileName << " -i input_project_file" << std::endl;
}

void ShowUpgradeLog(const std::string & newProjectPath)
{
	std::string upgradeLog = newProjectPath + "/upgrade.log";
	boost::filesystem::path upgradeLogPath(upgradeLog);
	if (boost::filesystem::exists(upgradeLogPath))
	{
		std::cout << "\nDo you want to have a look at the upgrade log(y/n):";
		std::string input;
		std::getline(std::cin, input);
		if (boost::iequals(input, "y"))
		{
			int sysRet = std::system(upgradeLog.c_str());
		}
	}
}
