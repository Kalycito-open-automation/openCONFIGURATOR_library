#include "../Include/ProjectApi.h"
#include "../Include/NodeCollection.h"
#include "../Include/Internal.h"
#include "../Include/Validation.h"

#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <libxml/xmlreader.h>

const char* DEFAULT_MN_XDD = "./resources/openPOWERLINK_MN.xdd";
const char* DEFAULT_MN_NAME = "openPOWERLINK_MN(240)";

DLLEXPORT ocfmRetCode NewProject(const string projectName, const string projectPath, const string pathToMNXdd)
{
	ocfmRetCode retCode(OCFM_ERR_SUCCESS);
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		ProjectConfiguration::GetInstance()->resetConfiguration();
		retCode = CloseProject();
	}
	if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
	{
		string currentPath(pathToMNXdd);

		if (currentPath.empty())
		{
			currentPath.append(DEFAULT_MN_XDD);
		}

		retCode = ValidateXDDFile(currentPath.c_str());

		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			retCode = NewProjectNode(240, MN, DEFAULT_MN_NAME, currentPath.c_str());
			if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
			{
				ProjectConfiguration::GetInstance()->SetProjectFile(projectName + ".xml");
				ProjectConfiguration::GetInstance()->SetProjectPath(projectPath);
				ProjectConfiguration::GetInstance()->SetProjectLoaded(true);
				return ocfmRetCode(OCFM_ERR_SUCCESS);
			}
		}
	}
	return retCode;
}

DLLEXPORT ocfmRetCode SaveProject(void)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
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
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		ProjectConfiguration::GetInstance()->resetConfiguration();
		retCode = CloseProject();
	}
	if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
	{
		retCode = ValidateProjectFile(projectFile);
		if (retCode.getErrorCode() == OCFM_ERR_SUCCESS)
		{
			ProjectConfiguration::GetInstance()->SetProjectFile(projectFile);
			ProjectConfiguration::GetInstance()->SetProjectPath(projectFile.substr(0,projectFile.find_last_of(PATH_SEPARATOR)).append(PATH_SEPARATOR));
			return ProjectConfiguration::GetInstance()->LoadProject(projectFile);
		}
		else
		{
			string errorString(retCode.getErrorString());
			retCode.setErrorString(errorString + "Error occured in file : " + projectFile);
		}
	}
	return retCode;
}