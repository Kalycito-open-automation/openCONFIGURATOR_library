#include "../Include/LibraryConfiguration.h"
#include "../Include/Enums.h"
#include <algorithm>

using namespace std;

#define OBJECT_DICTIONARY_FILE "od.xml"
#define XDD_SCHEMA_FILE "xddschema/Powerlink_Main.xsd"
#define PROJECT_SCHEMA_FILE "OC_ProjectFile/openCONFIGURATOR.xsd"
#define TXT2CDC_FILE "txt2cdc"

LibraryConfiguration* LibraryConfiguration::instance = NULL;

LibraryConfiguration::LibraryConfiguration(void)
{
	libResourcePath = "./resources";
	this->InitFilePaths();
}

LibraryConfiguration::~LibraryConfiguration(void)
{
	delete LibraryConfiguration::instance;
	LibraryConfiguration::instance = NULL;
}

LibraryConfiguration* LibraryConfiguration::GetInstance(void)
{
	if (LibraryConfiguration::instance == NULL)
	{
		instance = new LibraryConfiguration();
	}
	return instance;
}

const string& LibraryConfiguration::GetLibResourcePath()
{
	return this->libResourcePath;
}

void LibraryConfiguration::SetLibResourcePath(const string& libResourcePath)
{
	this->libResourcePath = libResourcePath;
}

bool LibraryConfiguration::Initialize(const string& libResourcePath)
{
	GetInstance()->SetLibResourcePath(libResourcePath);
	GetInstance()->InitFilePaths();

	return true;

}

const string& LibraryConfiguration::GetObjectDictinaryFilePath(void)
{
	return this->objectDictinaryFilePath;
}

const string& LibraryConfiguration::GetXddSchemaFilePath(void)
{
	return this->xddSchemaFilePath;
}

const string& LibraryConfiguration::GetProjectFileSchemaFilePath(void)
{
	return this->projectFileSchemaFilePath;
}

const string& LibraryConfiguration::GetTxt2CdcFilePath(void)
{
	return this->txt2CdcFilePath;
}

bool LibraryConfiguration::stringCompare(const string & l, const string & r)                                                                                   
{                                                                                                                                    
   return (l==r);                                                                                                                         
}

void LibraryConfiguration::InitFilePaths(void)
{
	this->objectDictinaryFilePath = libResourcePath + PATH_SEPARATOR + OBJECT_DICTIONARY_FILE;
	this->xddSchemaFilePath = libResourcePath + PATH_SEPARATOR + XDD_SCHEMA_FILE;
	this->projectFileSchemaFilePath = libResourcePath + PATH_SEPARATOR + PROJECT_SCHEMA_FILE;
	this->txt2CdcFilePath = libResourcePath + PATH_SEPARATOR + TXT2CDC_FILE;
}