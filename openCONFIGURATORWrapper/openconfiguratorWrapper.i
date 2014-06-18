/**
********************************************************************************
\file   openconfiguratorWrapper.h

\brief  TCL wrapper interface configuration for the openCONFIGURATOR library

\author Ramakrishnan Periyakaruppan

\copyright (c) 2014, Kalycito Infotech Private Limited
					 All rights reserved.

Redistribution and use in source and binary forms, with or without
modification, are permitted provided that the following conditions are met:
	* Redistributions of source code must retain the above copyright
	  notice, this list of conditions and the following disclaimer.
	* Redistributions in binary form must reproduce the above copyright
	  notice, this list of conditions and the following disclaimer in the
	  documentation and/or other materials provided with the distribution.
	* Neither the name of the copyright holders nor the
	  names of its contributors may be used to endorse or promote products
	  derived from this software without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL COPYRIGHT HOLDERS BE LIABLE FOR ANY
DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*******************************************************************************/

%module openConfiguratorWrapper
#define DLLEXPORT
/* Windows Platform */
#if defined(_WIN32) && defined(_MSC_VER)
//	 #undef DLLEXPORT
	 #define DLLEXPORT  __declspec(dllexport)
#endif

#define SWIGRUNTIME_DEBUG

%{
/* Simply include the header definitions */
#include "../openCONFIGURATOR/Include/Exports.h"
#include "../openCONFIGURATOR/Include/Result.h"
#include "../openCONFIGURATOR/Include/ProjectFileApi.h"
%}

/* Include all the required SWIG interfaces */
%include typemaps.i
%include exception.i
%include std_except.i
%include std_string.i


/** Ignore all the unwanted defines, enums and functions using %ignore **/
%ignore MN_NODEID;
%ignore BROADCAST_NODEID;
%ignore PI_VAR_COUNT;
%ignore PRES_DEFAULT_PAYLOAD;
%ignore PREQ_DEFAULT_PAYLOAD;
%ignore OFFSET_NUM_ENTRIES_MN;
%ignore OFFSET_NUM_ENTRIES_CN;
%ignore OFFSET_INDEX;
%ignore OFFSET_SUBINDEX;
%ignore OFFSET_SIZE;
%ignore OFFSET_DATA;
%ignore INDEX_SIZE;
%ignore SUBINDEX_SIZE;
%ignore NODE_ID;
%ignore ERR_STRING_LEN;
%ignore OFFSET_FIRST_INDEX_IN_MN_OBD;
%ignore OFFSET_FIRST_INDEX_IN_1F22;
%ignore MAX_CN_CROSS_TRAFFIC_STN;
%ignore MULTIPL_CYCLE_ASSIGN_OBJECT;
%ignore MNCN_POLLRESPONSE_TIMEOUT_OBJECT;
%ignore PATH_SEPARATOR;
%ignore DateTime;
%ignore IEC_Datatype;
%ignore NodeType;
%ignore PDOMapping;
%ignore PDOType;
%ignore ParameterAccess;
%ignore StationType;
%ignore AccessType;
%ignore ConfiguratorError;
%ignore FeatureType;
%ignore PlkFeatureStrings;

%ignore GetAccessType(const std::string& accessType);
%ignore GetObjectType(const std::string& objectType);
%ignore GetPlkFeature(const int enumVal);
%ignore openCONFIGURATOR::Library::ErrorHandling::Translate(const ocfmRetCode& legacyError);

/**
* Rename the CONST enum member to CONSTANT. Because the AccessType member CONST is 
* referred as const (which is a keyword) by the SWIG interpreter.
**/
%rename ("CONSTANT") "CONST";


%include "../openCONFIGURATOR/Include/Exports.h"
%include "../openCONFIGURATOR/Include/Result.h"
%include "../openCONFIGURATOR/Include/ProjectFileApi.h"

%inline 
%{

/* Copy all the API declarations here */
namespace openCONFIGURATOR
{
	namespace Library
	{
		namespace API
		{
			/* NodeApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result AddNode(const UINT32, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result DeleteNode(const UINT32);
			openCONFIGURATOR::Library::ErrorHandling::Result ReplaceXdd(const UINT32, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result IsExistingNode(const UINT32, bool&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetNodeCount(UINT32&);
			openCONFIGURATOR::Library::ErrorHandling::Result SetNodeParameter(const UINT32, const openCONFIGURATOR::Library::NodeParameter::NodeParameter, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetNodeParameter(const UINT32, const openCONFIGURATOR::Library::NodeParameter::NodeParameter, std::string&);


			/* XddApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result GetDataTypeSize(const openCONFIGURATOR::Library::ObjectDictionary::PlkDataType::PlkDataType, UINT32&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetFeatureValue(const UINT32, const openCONFIGURATOR::Library::ObjectDictionary::PlkFeature::PlkFeature, std::string&);


			/* ProjectApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result NewProject(const std::string, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result SaveProject();
			openCONFIGURATOR::Library::ErrorHandling::Result CloseProject();
			openCONFIGURATOR::Library::ErrorHandling::Result OpenProject(const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result AddPath(const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetPath(const std::string, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result DeletePath(const std::string );
			openCONFIGURATOR::Library::ErrorHandling::Result SetActiveAutoCalculationConfig(const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetActiveAutoCalculationConfig(std::string&);

			/* ProjectFileApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result AddViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType, const std::string, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result DeleteViewSetting(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result SetActiveView(const openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType);
			openCONFIGURATOR::Library::ErrorHandling::Result GetActiveView(openCONFIGURATOR::Library::ProjectFile::ViewType::ViewType&);

			/* ImportExportApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result GenerateProcessImageDescription(const OutputLanguage, const std::string, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GenerateStackConfiguration(const std::string, const std::string);

			/* OdApi.h */
			openCONFIGURATOR::Library::ErrorHandling::Result SetIndexActualValue(const UINT32, const UINT32, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result SetSubIndexActualValue(const UINT32, const UINT32, const UINT32, const std::string);
			openCONFIGURATOR::Library::ErrorHandling::Result GetIndexAttribute(const UINT32, const UINT32, AttributeType, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexAttribute(const UINT32, const UINT32, const UINT32, AttributeType, std::string&);
			openCONFIGURATOR::Library::ErrorHandling::Result IsExistingIndex(const UINT32, const UINT32, bool& exists);
			openCONFIGURATOR::Library::ErrorHandling::Result IsExistingSubIndex(const UINT32, const UINT32, const UINT32, bool&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetIndexCount(const UINT32, UINT32&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetSubIndexCount(const UINT32, const UINT32, UINT32&);
			openCONFIGURATOR::Library::ErrorHandling::Result GetNumberOfEntries(const UINT32, const UINT32, const bool, UINT32&);
		}
	}
}

%}