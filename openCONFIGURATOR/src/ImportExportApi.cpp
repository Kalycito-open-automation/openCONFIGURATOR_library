/************************************************************************
\file ImportExportApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/ImportExportApi.h"
#include "../Include/Internal.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/BoostShared.h"
#include "../Include/Logging.h"

#include <sstream>

using namespace std;
using namespace openCONFIGURATOR::Library::ErrorHandling;

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

			DLLEXPORT Result GenerateProcessImageDescription(const OutputLanguage lang, const string outputPath, const string fileName)
			{
				try
				{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					boost::filesystem::path absOutputPath = getAbsOutputPath(outputPath);
					if (absOutputPath.empty())		
						return Translate(ocfmRetCode(OCFM_ERR_EMPTY_PATH));

					string fallbackFileName = (fileName.empty())
						? kDefaultProcessImageFileName
						: fileName.substr(0, fileName.find_last_of("."));
					absOutputPath.append(fallbackFileName.begin(), fallbackFileName.end());

					switch (lang)
					{
						case C:
							return Translate(GenerateXAP(absOutputPath.generic_string().c_str()));
						case CSHARP:
							return Translate(GenerateNET(absOutputPath.generic_string().c_str()));
						case XML:
							return Translate(GenerateXAP(absOutputPath.generic_string().c_str()));
						default:
							boost::format formatter(kMsgUnsupportedPiLanguage);
							formatter % lang;
							return Result(UNSUPPORTED_PI_GEN_LANGUAGE, formatter.str());
					}
				}
				return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED));
			}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const std::exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}

			DLLEXPORT Result GenerateStackConfiguration(const string outputPath, const string fileName)
			{
				try
				{
				if (ProjectConfiguration::GetInstance().IsInitialized())
				{
					boost::filesystem::path absOutputPath = getAbsOutputPath(outputPath);
					if (absOutputPath.empty())		
						return Translate(ocfmRetCode(OCFM_ERR_EMPTY_PATH));

					string fallbackFileName = (fileName.empty())
						? kDefaultStackConfigFileName
						: fileName.substr(0, fileName.find_last_of("."));
					absOutputPath.append(fallbackFileName.begin(), fallbackFileName.end());
					return Translate(GenerateCDC(absOutputPath.generic_string().c_str(), ProjectConfiguration::GetInstance()));
				}
				return Translate(ocfmRetCode(OCFM_ERR_NO_PROJECT_LOADED));
			}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
			}
				catch (const std::exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}
		}
	}
}

