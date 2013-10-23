/************************************************************************
\file ImportExportApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/ImportExportApi.h"
#include "../Include/Internal.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/BoostShared.h"

#include <sstream>

using namespace std;

namespace openCONFIGURATOR 
{
	namespace Library
	{
		namespace API
		{

			const string kDefaultProcessImageFileName = "PI";
			const string kDefaultStackConfigFileName = "mnobd";

			static boost::filesystem::path getAbsOutputPath(string path)
			{
				boost::filesystem::path absOutputPath(path);
				if (path.empty() && !ProjectConfiguration::GetInstance().GetDefaultOutputPath().empty())
				{
					boost::filesystem::path absDefaultOutputPath(ProjectConfiguration::GetInstance().GetDefaultOutputPath());
					if (absDefaultOutputPath.is_relative())
					{
						absDefaultOutputPath = ProjectConfiguration::GetInstance().GetProjectPath();
						absDefaultOutputPath.append(
							ProjectConfiguration::GetInstance().GetDefaultOutputPath().begin(),
							ProjectConfiguration::GetInstance().GetDefaultOutputPath().end());
					}
					absOutputPath = absDefaultOutputPath;
				}
				return absOutputPath;
			}

			DLLEXPORT ocfmRetCode GenerateProcessImageDescription(const OutputLanguage lang, const string outputPath, const string fileName)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					boost::filesystem::path absOutputPath = getAbsOutputPath(outputPath);
					if (absOutputPath.empty())		
						return ocfmRetCode(OCFM_ERR_EMPTY_PATH);

					string fallbackFileName = (fileName.empty())
						? kDefaultProcessImageFileName
						: fileName.substr(0, fileName.find_last_of("."));
					absOutputPath.append(fallbackFileName.begin(), fallbackFileName.end());

					switch (lang)
					{
						case C:
							return GenerateXAP(absOutputPath.generic_string().c_str());
						case CSHARP:
							return GenerateNET(absOutputPath.generic_string().c_str());
						case XML:
							return GenerateXAP(absOutputPath.generic_string().c_str());
						default:
							return ocfmRetCode(OCFM_ERR_UNKNOWN);
							break;
					}
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

			DLLEXPORT ocfmRetCode GenerateStackConfiguration(const string outputPath, const string fileName)
			{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					boost::filesystem::path absOutputPath = getAbsOutputPath(outputPath);
					if (absOutputPath.empty())		
						return ocfmRetCode(OCFM_ERR_EMPTY_PATH);

					string fallbackFileName = (fileName.empty())
						? kDefaultStackConfigFileName
						: fileName.substr(0, fileName.find_last_of("."));
					absOutputPath.append(fallbackFileName.begin(), fallbackFileName.end());
					return GenerateCDC(absOutputPath.generic_string().c_str(), ProjectConfiguration::GetInstance());
				}
				return ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED);
			}

		}
	}
}

