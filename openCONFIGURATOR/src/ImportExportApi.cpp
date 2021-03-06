/************************************************************************
\file ImportExportApi.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/ImportExportApi.h"
#include "../Include/Internal.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/BoostShared.h"
#include "../Include/Logging.h"
#include "../Include/ProjectApi.h"

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
				if (path.empty() && !ProjectConfiguration::GetInstance().GetDefaultOutputPath().generic_string().empty())
				{
					boost::filesystem::path absDefaultOutputPath = ProjectConfiguration::GetInstance().GetDefaultOutputPath();
					if (absDefaultOutputPath.is_relative())
					{
						absDefaultOutputPath = ProjectConfiguration::GetInstance().GetProjectPath();
						absDefaultOutputPath.append(
							ProjectConfiguration::GetInstance().GetDefaultOutputPath().generic_string().begin(),
							ProjectConfiguration::GetInstance().GetDefaultOutputPath().generic_string().end());
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
						{
							boost::format formatter(kMsgEmptyArgument);
							formatter % "outputPath";
							return Result(ARGUMENT_INVALID_EMPTY, formatter.str());
						}

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
					return Result(NO_PROJECT_LOADED, kMsgNoProjectLoaded);
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
						{
							boost::format formatter(kMsgEmptyArgument);
							formatter % "outputPath";
							return Result(ARGUMENT_INVALID_EMPTY, formatter.str());
						}

						string fallbackFileName = (fileName.empty())
						                          ? kDefaultStackConfigFileName
						                          : fileName.substr(0, fileName.find_last_of("."));
						absOutputPath.append(fallbackFileName.begin(), fallbackFileName.end());
						return Translate(GenerateCDC(absOutputPath.generic_string().c_str(), ProjectConfiguration::GetInstance()));
					}
					return Result(NO_PROJECT_LOADED, kMsgNoProjectLoaded);
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

