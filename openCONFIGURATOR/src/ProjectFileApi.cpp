/************************************************************************
\file ProjectFileApi.cpp
\author Christoph R&uumml;cker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/


#include "../Include/ProjectFileApi.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/Result.h"
#include "../Include/Logging.h"

#include <string>

using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::GUI::ProjectFile::ViewType;
using namespace std;

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace API
		{
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result AddViewSetting(const ViewType viewType, const std::string name, const std::string value)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						if(name.empty())
							return Result(ARGUMENT_INVALID_EMPTY, "Name argument must not be empty.");
						return ProjectConfiguration::GetInstance().AddViewSetting(viewType, name, value);
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
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetActiveView(ViewType& viewType)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						return ProjectConfiguration::GetInstance().GetActiveViewSettings(viewType);
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
			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result SetActiveView(const ViewType viewType)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						return ProjectConfiguration::GetInstance().SetActiveViewSettings(viewType);
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

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result GetViewSetting(const ViewType viewType, const std::string name, std::string& value)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						if(name.empty())
							return Result(ARGUMENT_INVALID_EMPTY, "Name argument must not be empty.");
						return ProjectConfiguration::GetInstance().GetViewSetting(viewType, name, value);
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

			DLLEXPORT openCONFIGURATOR::Library::ErrorHandling::Result DeleteViewSetting(const ViewType viewType, const std::string name)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						if(name.empty())
							return Result(ARGUMENT_INVALID_EMPTY, "Name argument must not be empty.");
						return ProjectConfiguration::GetInstance().DeleteViewSetting(viewType, name);
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