/************************************************************************
\file Enums.cpp
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/Enums.h"

#include <string>
#include <algorithm>

using namespace openCONFIGURATOR::Library::ObjectDictionary;
using namespace std;

AccessType::AccessType openCONFIGURATOR::Library::ObjectDictionary::GetAccessType(const string& accessType)
{
	string accessTypelowerCase(accessType);

	transform(accessTypelowerCase.begin(), accessTypelowerCase.end(), accessTypelowerCase.begin(), ::tolower);
	if (accessTypelowerCase == "wo")
		return AccessType::WO;
	else if (accessTypelowerCase == "wos")
		return AccessType::WOS;
	else if (accessTypelowerCase == "ro")
		return AccessType::RO;
	else if (accessTypelowerCase == "rw")
		return AccessType::RW;
	else if (accessTypelowerCase == "rws")
		return AccessType::RWS;
	else if (accessTypelowerCase == "const")
		return AccessType::CONST;
	else if (accessTypelowerCase == "cond")
		return AccessType::COND;
	else
		return AccessType::INVALID;
}

::ObjectType openCONFIGURATOR::Library::ObjectDictionary::GetObjectType(const std::string& objectType)
{
	string objectTypelowerCase(objectType);

	transform(objectTypelowerCase.begin(), objectTypelowerCase.end(), objectTypelowerCase.begin(), ::tolower);
	if (objectTypelowerCase == "deftype")
		return ::DEFTYPE;
	else if (objectTypelowerCase == "defstruct")
		return ::DEFSTRUCT;
	else if (objectTypelowerCase == "var")
		return ::VAR;
	else if (objectTypelowerCase == "array")
		return ::ARRAY;
	else if (objectTypelowerCase == "record")
		return ::RECORD;
	else
		return ::INVALID;
}