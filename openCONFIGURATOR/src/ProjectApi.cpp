/************************************************************************
\file ProjectApi.cpp
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/ProjectApi.h"
#include "../Include/NodeCollection.h"
#include "../Include/Internal.h"
#include "../Include/Validation.h"
#include "../Include/ProjectConfiguration.h"
#include "../Include/Result.h"
#include "../Include/Logging.h"

#include <string>

using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace std;

namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace API
		{
			const string kDefaultMNXDD = "./resources/openPOWERLINK_MN.xdd";
			const string kDefaultMNName = "openPOWERLINK_MN(240)";

			DLLEXPORT Result NewProject(const string projectName, const string projectPath, const string pathToMNXdd)
			{
				try
				{
					Result result;
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						ProjectConfiguration::GetInstance().ResetConfiguration();
						result = CloseProject();
					}
					if (result.IsSuccessful())
					{
						string fallbackPath = pathToMNXdd.empty()
						                      ? kDefaultMNXDD
						                      : pathToMNXdd;

						result = Translate(ValidateXDDFile(fallbackPath.c_str()));
						if (result.IsSuccessful())
						{
							result = Translate(NewProjectNode(MN_NODEID, MN, kDefaultMNName.c_str(), fallbackPath.c_str()));
							if (result.IsSuccessful())
							{
								ProjectConfiguration::GetInstance().SetProjectFile(projectName + ".xml");
								ProjectConfiguration::GetInstance().SetProjectPath(boost::filesystem::path(projectPath));
								ProjectConfiguration::GetInstance().SetInitialized(true);

								Node* node = NodeCollection::GetNodeColObjectPointer()->GetNodePtr(240);
								node->SetXddPath(fallbackPath);
								node->SetXdcPath("");

								return Result();
							}
						}
					}
					return result;
				}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}

			DLLEXPORT Result SaveProject(void)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						return ProjectConfiguration::GetInstance().SaveProject();
					}
					return Result(NO_PROJECT_LOADED, kMsgNoProjectLoaded);
				}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}

			DLLEXPORT Result CloseProject(void)
			{
				try
				{
					ProjectConfiguration::GetInstance().ResetConfiguration();
					return Translate(FreeProjectMemory());
				}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}

			DLLEXPORT Result OpenProject(const string projectFile)
			{
				try
				{
					Result result;
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						ProjectConfiguration::GetInstance().ResetConfiguration();
						result = CloseProject();
					}
					if (result.IsSuccessful())
					{
						result = ValidateProjectFile(projectFile);
						if (result.IsSuccessful())
						{
							boost::filesystem::path pPath(projectFile);
							ProjectConfiguration::GetInstance().SetProjectFile(pPath.filename().generic_string());
							ProjectConfiguration::GetInstance().SetProjectPath(pPath.parent_path());
							return Translate(ProjectConfiguration::GetInstance().LoadProject(projectFile));
						}
					}
					return result;
				}
				catch (const ocfmRetCode& ex)
				{
					return Translate(ex);
				}
				catch (const exception& ex)
				{
					return Result(UNHANDLED_EXCEPTION, ex.what());
				}
			}

			DLLEXPORT Result AddPath(const std::string id, const std::string path)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						if(id.empty() || path.empty())
							return Result(ARGUMENT_INVALID_EMPTY, "ID or Path argument must not be empty.");
						return ProjectConfiguration::GetInstance().AddPath(id, path);
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
			DLLEXPORT Result GetPath(const std::string id, std::string& path)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						if(id.empty())
							return Result(ARGUMENT_INVALID_EMPTY, "ID argument must not be empty.");
						return ProjectConfiguration::GetInstance().GetPath(id, path);
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

			DLLEXPORT Result DeletePath(const std::string id)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						if(id.empty())
							return Result(ARGUMENT_INVALID_EMPTY, "ID argument must not be empty.");
						return ProjectConfiguration::GetInstance().DeletePath(id);
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

			DLLEXPORT Result SetActiveAutoCalculationConfig(const std::string id)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						if(id.empty())
							return Result(ARGUMENT_INVALID_EMPTY, "ID argument must not be empty.");
						return ProjectConfiguration::GetInstance().SetActiveAutoGenerationSetting(id);
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

			DLLEXPORT Result GetActiveAutoCalculationConfig(std::string& id)
			{
				try
				{
					if (ProjectConfiguration::GetInstance().IsInitialized())
					{
						return ProjectConfiguration::GetInstance().GetActiveAutoGenerationSetting(id);
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
