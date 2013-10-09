#include "../Include/ImportExportApi.h"
#include "../Include/Internal.h"
#include "../Include/ProjectConfiguration.h"
#include <sstream>
using namespace std;

DLLEXPORT ocfmRetCode GenerateProcessImageDescription(const OutputLanguage lang, const string outputPath, const string fileName)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		ostringstream outputFile;
		//Prepare output path
		if (!outputPath.empty())
		{
			//Path not empty
			if (outputPath.find_first_of(".") == 1)
			{
				//Path relative
				outputFile << ProjectConfiguration::GetInstance()->GetProjectPath();
				outputFile << PATH_SEPARATOR;
				outputFile << outputPath;
			}
			else
			{
				//Path absolute
				outputFile << outputPath;
			}
		}
		else if (!ProjectConfiguration::GetInstance()->GetDefaultOutputPath().empty())
		{
			//Default output path not empty
			if (ProjectConfiguration::GetInstance()->GetDefaultOutputPath().find_first_of(".") == 1)
			{
				//Path relative
				outputFile << ProjectConfiguration::GetInstance()->GetProjectPath();
				outputFile << PATH_SEPARATOR;
				outputFile << ProjectConfiguration::GetInstance()->GetDefaultOutputPath();
			}
			else
			{
				//Path absolute
				outputFile << ProjectConfiguration::GetInstance()->GetDefaultOutputPath();
			}
		}
		else
		{
			return ocfmRetCode(OCFM_ERR_EMPTY_PATH);
		}

		//Prepare output file name
		if (!fileName.empty())
		{
			outputFile << PATH_SEPARATOR;
			outputFile << fileName.substr(0, fileName.find_last_of("."));
		}
		else if (!ProjectConfiguration::GetInstance()->GetDefaultOutputPath().empty())
		{
			outputFile << PATH_SEPARATOR;
			outputFile << "PI";
		}

		switch (lang)
		{
		case C:
			return GenerateXAP(outputFile.str().c_str());
			break;
		case CSHARP:
			return GenerateNET(outputFile.str().c_str());
			break;
		case XML:
			return GenerateXAP(outputFile.str().c_str());
			break;
		default:
			return ocfmRetCode(OCFM_ERR_UNKNOWN);
			break;
		}
	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

DLLEXPORT ocfmRetCode GenerateStackConfiguration(const string outputPath, const string fileName)
{
	if (ProjectConfiguration::GetInstance()->GetProjectLoaded())
	{
		ocfmRetCode retValue(OCFM_ERR_SUCCESS);
		ostringstream outputFile;
		//Prepare output path
		if (!outputPath.empty())
		{
			//Path not empty
			if (outputPath.find_first_of(".") == 1)
			{
				//Path relative
				outputFile << ProjectConfiguration::GetInstance()->GetProjectPath();
				outputFile << PATH_SEPARATOR;
				outputFile << outputPath;
			}
			else
			{
				//Path absolute
				outputFile << outputPath;
			}
		}
		else if (!ProjectConfiguration::GetInstance()->GetDefaultOutputPath().empty())
		{
			//Default output path not empty
			if (ProjectConfiguration::GetInstance()->GetDefaultOutputPath().find_first_of(".") == 1)
			{
				//Path relative
				outputFile << ProjectConfiguration::GetInstance()->GetProjectPath();
				outputFile << PATH_SEPARATOR;
				outputFile << ProjectConfiguration::GetInstance()->GetDefaultOutputPath();
			}
			else
			{
				//Path absolute
				outputFile << ProjectConfiguration::GetInstance()->GetDefaultOutputPath();
			}
		}
		else
		{
			return ocfmRetCode(OCFM_ERR_EMPTY_PATH);
		}

		if (!fileName.empty())
		{
			outputFile << PATH_SEPARATOR;
			outputFile << fileName.substr(0, fileName.find_last_of("."));
		}
		else if (!ProjectConfiguration::GetInstance()->GetDefaultOutputPath().empty())
		{
			outputFile << PATH_SEPARATOR;
			outputFile << "mnobd";
		}
		return GenerateCDC(outputFile.str().c_str(), *ProjectConfiguration::GetInstance());

	}
	return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
}

