/************************************************************************
\file	LibraryConfiguration.cpp
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/LibraryConfiguration.h"
#include "../Include/Enums.h"
#include "../Include/Constants.h"
#include "../Include/Logging.h"

#include <algorithm>
#include <cassert>
#include <cstdlib>
#ifndef NDEBUG
#include <iostream>
#endif

using namespace std;

const string LibraryConfiguration::kDefaultLibResourcePath = "resources";
const string LibraryConfiguration::kLibResourcePathEnvVar = "OPENCONFIGURATOR_LIBRARY_RESOURCEPATH";
const string LibraryConfiguration::kRelXDDSchemaPath = "xddschema/Powerlink_Main.xsd";
const string LibraryConfiguration::kRelProjectFileSchemaPath = "OC_ProjectFile/openCONFIGURATOR.xsd";
const string LibraryConfiguration::kTxt2CdcFile = "txt2cdc";
const string LibraryConfiguration::kObjectDictionaryFile = "od.xml";

string LibraryConfiguration::libResourcePath;

const string& LibraryConfiguration::GetLibResourcePath()
{
	if (LibraryConfiguration::libResourcePath.empty())
	{
		const char* libResourcePathEnvVar = std::getenv(kLibResourcePathEnvVar.c_str());
		if (libResourcePathEnvVar)
			LibraryConfiguration::libResourcePath = libResourcePathEnvVar;
		else
			LibraryConfiguration::libResourcePath = kDefaultLibResourcePath;
#ifndef NDEBUG
		cout << "Library resource path evaluated to: " << LibraryConfiguration::libResourcePath << endl;
#endif
	}
	return LibraryConfiguration::libResourcePath;
}

const string LibraryConfiguration::GetObjectDictinaryFilePath(void)
{
	return string(LibraryConfiguration::GetLibResourcePath()
	              + kPathSeparator
	              + kObjectDictionaryFile);
}

const string LibraryConfiguration::GetXddSchemaFilePath(void)
{
	return string(LibraryConfiguration::GetLibResourcePath()
	              + kPathSeparator
	              + kRelXDDSchemaPath);
}

const string LibraryConfiguration::GetProjectFileSchemaFilePath(void)
{
	return string(LibraryConfiguration::GetLibResourcePath()
	              + kPathSeparator
	              + kRelProjectFileSchemaPath);
}

const string LibraryConfiguration::GetTxt2CdcFilePath(void)
{
	string path(LibraryConfiguration::GetLibResourcePath()
	            + kPathSeparator
	            + kTxt2CdcFile);

#if defined(_WIN32) && defined(_MSC_VER)
	return (path + ".exe");
#else
	return path;
#endif
}