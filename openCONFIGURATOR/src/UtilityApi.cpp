#include "../Include/UtilityApi.h"
#include "../Include/LibraryConfiguration.h"
#include <string>
using namespace std;

DLLEXPORT bool SetLibraryResourcePath(const string libraryResourcePath)
{
	return LibraryConfiguration::Initialize(libraryResourcePath);
}