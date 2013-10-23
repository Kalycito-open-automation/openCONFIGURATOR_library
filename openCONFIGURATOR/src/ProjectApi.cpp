/************************************************************************
\file ProjectApi.cpp
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/ProjectApi.h"
#include "../Include/NodeCollection.h"
#include "../Include/Internal.h"
#include "../Include/Validation.h"
#include "../Include/ProjectConfiguration.h"

#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <libxml/xmlreader.h>

#include <string>

using namespace std;

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			const string kDefaultMNXDD = "./resources/openPOWERLINK_MN.xdd";
			const string kDefaultMNName = "openPOWERLINK_MN(240)";

			DLLEXPORT ocfmRetCode NewProject(const string projectName, const string projectPath, const string pathToMNXdd)
			{
				ocfmRetCode retCode(OCFM_ERR_SUCCESS);
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					ProjectConfiguration::GetInstance().ResetConfiguration();
					retCode = CloseProject();
				}
				if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
				{
					string fallbackPath = pathToMNXdd.empty() 
						? kDefaultMNXDD 
						: pathToMNXdd;

					retCode = ValidateXDDFile(fallbackPath.c_str());
					if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
					{
						retCode = NewProjectNode(MN_NODEID, MN, kDefaultMNName.c_str(), fallbackPath.c_str());
						if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
						{
							ProjectConfiguration::GetInstance().SetProjectFile(projectName + ".xml");
							ProjectConfiguration::GetInstance().SetProjectPath(projectPath);
							ProjectConfiguration::GetInstance().SetInitialized(true);
							return ocfmRetCode(OCFM_ERR_SUCCESS);
						}
					}
				}
				return retCode;
			}

			DLLEXPORT ocfmRetCode SaveProject(void)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					return ocfmRetCode(OCFM_ERR_UNKNOWN);
					//return SaveProject(path.c_str(), projectName.c_str());
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode CloseProject(void)
			{
				return FreeProjectMemory();
			}

			DLLEXPORT ocfmRetCode OpenProject(const string projectFile)
			{
				ocfmRetCode retCode(OCFM_ERR_SUCCESS);
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					ProjectConfiguration::GetInstance().ResetConfiguration();
					retCode = CloseProject();
				}
				if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
				{
					retCode = ValidateProjectFile(projectFile);
					if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
					{
						ProjectConfiguration::GetInstance().SetProjectFile(projectFile);
						ProjectConfiguration::GetInstance().SetProjectPath(projectFile.substr(0, projectFile.find_last_of(PATH_SEPARATOR)).append(PATH_SEPARATOR));
						return ProjectConfiguration::GetInstance().LoadProject(projectFile);
					}
					else
					{
						string errorString(retCode.getErrorString());
						retCode.setErrorString(errorString + "Error occurred in file: " + projectFile);
					}
				}
				return retCode;
			}

		}
	}
}
