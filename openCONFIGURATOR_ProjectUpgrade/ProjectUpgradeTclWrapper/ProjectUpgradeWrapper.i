/*******************************************************************************
* \file   ProjectUpgradeWrapper.h
*
* \brief  TCL wrapper interface configuration for the openCONFIGURATOR project 
*         upgrade library
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

%module ProjectUpgradeWrapper

%{
/* Simply include the header definitions. */
#include "../LibProjectUpgrade/include/Namespace.h"
#include "../LibProjectUpgrade/include/Exports.h"
#include "../LibProjectUpgrade/include/Result.h"
#include "../LibProjectUpgrade/include/ErrorCode.h"
#include "../LibProjectUpgrade/include/UpgradeAPI.h"
%}

/* Include all the required SWIG interfaces. */
%include stl.i
%include std_string.i
%include cpointer.i
%include std_vector.i

%rename(ProjectUpgrade_ErrorCode) openCONFIGURATOR::ProjectUpgrade::ErrorCode::ErrorCode;
%rename(ProjectUpgrade_Result) openCONFIGURATOR::ProjectUpgrade::Result;

%rename(ProjectUpgrade_GetNodes) openCONFIGURATOR::ProjectUpgrade::API::GetNodes;
%rename(ProjectUpgrade_GetNodeName) openCONFIGURATOR::ProjectUpgrade::API::GetNodeName;
%rename(ProjectUpgrade_GetOctxPath) openCONFIGURATOR::ProjectUpgrade::API::GetOctxPath;
%rename(ProjectUpgrade_SetXddPath) openCONFIGURATOR::ProjectUpgrade::API::SetXddPath;
%rename(ProjectUpgrade_ImportProjectFile) openCONFIGURATOR::ProjectUpgrade::API::ImportProjectFile;
%rename(ProjectUpgrade_UpgradeProject) openCONFIGURATOR::ProjectUpgrade::API::UpgradeProject;
%rename(ProjectUpgrade_GetNewProjectFilePath) openCONFIGURATOR::ProjectUpgrade::API::GetNewProjectFilePath;
%rename(ProjectUpgrade_ResetProjectUpgradeLib) openCONFIGURATOR::ProjectUpgrade::API::ResetProjectUpgradeLib;
%rename(ProjectUpgrade_RevertUpgradeProject) openCONFIGURATOR::ProjectUpgrade::API::RevertUpgradeProject;

%include "../LibProjectUpgrade/include/Namespace.h"
%include "../LibProjectUpgrade/include/Exports.h"
%include "../LibProjectUpgrade/include/Result.h"
%include "../LibProjectUpgrade/include/ErrorCode.h"
%include "../LibProjectUpgrade/include/UpgradeAPI.h"

%inline 
%{

namespace openCONFIGURATOR
{
	namespace ProjectUpgrade
	{
		namespace API
		{
			Result GetNodes(std::vector<unsigned int>& nodeIds);
			Result GetNodeName(const unsigned int nodeId, std::string & name);
			Result GetOctxPath(const unsigned int nodeId, std::string & octxFilePath, bool shortPath);
			Result SetXddPath(const unsigned int nodeId, const std::string & xddPath);
			Result ImportProjectFile(const std::string & projectFile);
			Result UpgradeProject(const std::string & projectPath);
			Result GetNewProjectFilePath(std::string & newProjectFilePath);
			Result ResetProjectUpgradeLib();
			Result RevertUpgradeProject();
		}
	}
}
%}

%pointer_functions(std::string, ProjectUpgradeStringpointer)

namespace std
{
	%template(ProjectUpgradeUIntVector)    vector<unsigned int>;
}
