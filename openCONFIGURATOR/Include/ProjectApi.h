#ifndef PROJECT_API_H_
#define PROJECT_API_H_

#include <string>
#include "Exports.h"
#include "Enums.h"
#include "Exception.h"
#include "ProjectConfiguration.h"

DLLEXPORT ocfmRetCode NewProject(const string projectName, const string projectPath, const string pathToMNXdd = "");
DLLEXPORT ocfmRetCode SaveProject(void);
DLLEXPORT ocfmRetCode CloseProject(void);
DLLEXPORT ocfmRetCode OpenProject(const string projectFile);

#endif