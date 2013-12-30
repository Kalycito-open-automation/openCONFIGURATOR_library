/************************************************************************
\file	ProjectConfiguration.h
\brief	Classes/Functions to read/provide openCONFIGURATOR project configuration.

		Functionality: Reading project configuration from an XML file and providing
		settings to the application.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	11.10.2013
************************************************************************/

#ifndef PROJECTCONFIGURATION_H_
#define PROJECTCONFIGURATION_H_

#include <string>
#include <iostream>
#include <sstream>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <libxml/xmlreader.h>
#include <libxml/xmlmemory.h>
#include <libxml/parser.h>
#include <libxml/xpath.h>

#include "Exception.h"
#include "NodeCollection.h"
#include "Internal.h"
#include "Validation.h"
#include "Result.h"
#include "Enums.h"
#include "AutoGenSetting.h"
#include "Path.h"
#include "ViewSetting.h"

#include <map>
#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <libxml/xmlreader.h>

#include "BoostShared.h"

using namespace openCONFIGURATOR::Library::ErrorHandling;
using namespace openCONFIGURATOR::GUI::ProjectFile::ViewType;
using namespace openCONFIGURATOR::GUI::ProjectFile::Setting;

class ProjectConfiguration
{
	public:
		static ProjectConfiguration& GetInstance(void);

		const boost::filesystem::path& GetDefaultOutputPath(void) const;
		void SetDefaultOutputPath(const boost::filesystem::path& defaultOutputPath);

		const std::string& GetCreationDate(void) const;
		void SetCreationDate(const std::string& creationDate);

		bool GetGenerateMNOBD(void) const;
		void SetGenerateMNOBD(bool generateMNOBD);

		bool IsInitialized(void) const;
		void SetInitialized(bool initialized);

		bool IsAlreadySaved(void) const;
		void SetAlreadySaved(bool saved);

		const std::string& GetProjectFile(void) const;
		void SetProjectFile(const std::string& projectFile);

		const boost::filesystem::path& GetProjectPath(void) const;
		void SetProjectPath(const boost::filesystem::path& projectPath);

		const boost::optional<UINT32>& GetCycleTime(void) const;
		void SetCycleTime(UINT32 cycleTime);

		const boost::optional<UINT32>& GetAsyncMTU(void) const;
		void SetAsyncMTU(UINT32 asyncMTU);

		const boost::optional<UINT32>& GetMultiplexedCycleLength(void) const;
		void SetMultiplexedCycleLength(UINT32 multiplexedCycleLength);

		const boost::optional<UINT32>& GetPrescaler(void) const;
		void SetPrescaler(UINT32 prescaler);

		Result SaveProject(void);

		ocfmRetCode LoadProject(const std::string& projectFile);
		void ResetConfiguration(void);

		Result AddPath(const std::string& id, const std::string& path);
		Result GetPath(const std::string& id, std::string& pathResult);
		Result DeletePath(const std::string& id);

		Result AddViewSetting(const ViewType& viewType, const std::string& name, const std::string& value);
		Result GetViewSetting(const ViewType& viewType, const std::string& name, std::string& value);
		Result DeleteViewSetting(const ViewType& viewType, const std::string& name);
		Result SetActiveViewSettings(const ViewType& viewType);
		Result GetActiveViewSettings(ViewType& viewType);

		Result AddAutoGenerationSetting(const std::string& type, const std::string& name, const std::string& value);
		Result GetAutoGenerationSetting(const std::string& name, const std::string& type, std::string& settingValue);
		Result DeleteAutoGenerationSetting(const string& id, const string& name);
		Result SetActiveAutoGenerationSetting(const string& id);
		Result GetActiveAutoGenerationSetting(std::string& activeAutoGenSetting);

		~ProjectConfiguration(void);

	private:
		static ProjectConfiguration instance;
		ProjectConfiguration(void);
		ProjectConfiguration(const ProjectConfiguration&);
		void operator=(const ProjectConfiguration&);

		void ProcessProject(xmlTextReaderPtr xmlReader);
		void ProcessProjectNode(xmlTextReaderPtr xmlReader);
		void ProcessPath(xmlTextReaderPtr xmlReader);
		void ProcessNetworkConfiguration(xmlTextReaderPtr xmlReader);
		void ProcessAutogenerationSettings(xmlTextReaderPtr xmlReader);
		void ProcessViewSettings(xmlTextReaderPtr xmlReader);
		void ProcessGenerator(xmlTextReaderPtr xmlReader);

		void CreateProjectFolder(void);
		void WriteProjectFile(void);
		void WriteMNXDC(void);
		void WriteCNXDCs(void);
		void UpdateNodeConfigurations(void);
		const xmlXPathObjectPtr GetNodeSet(const xmlDocPtr doc, const char* xpath);

		/*
		void SynchronizeMultiplexedCycleLength(void);
		void SynchronizeCycleTime(void);
		void SynchronizeAsyncMtu(void);
		void SynchronizePrescaler(void);
		*/

		/************************************************************************
		\brief	Override project configuration values for the network (element "NetworkConfiguration").

				Overrides the attributes "cycleTime", "multiplexedCycleLength", "asyncMTU" and "prescaler"
				of the "NetworkConfiguration" element with corresponding values from the MN's OD, indices
				0x1006, 0x1F98/7-9.
		\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
		************************************************************************/
		void OverrideNetworkConfiguration();

		/************************************************************************
		\brief	Override project configuration values for a Node with values from the Node's XDD.

				This method will:
				1. If the given index/subIndex on the Node has an "actualValue" (!= NULL && len > 0),
				   read and write it to projectConfigValue (precedence of Nodes' OD over project configuration file).
				2. If there is no "actualValue" and projectConfigValue is initialized (read: there is a value
				   in the project configuration file), it will write projectConfigValue to "actualValue".

		\param index 16bit index of the Index.
		\param subIndex	8bit index of the SubIndex.
		\param projectConfigValue Actual value from project configuration file.
		\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
		************************************************************************/
		template<typename T>
		void OverrideNodeConfiguration(Node& node,
		                               const UINT32 index,
		                               const UINT32 subIndex,
		                               boost::optional<T>& projectConfigValue) const;

		//General attributes
		bool initialized;
		bool alreadysaved;

		//Project related attributes
		std::string projectFile;
		std::string creationDate;

		bool generateMNOBD;

		std::string currentAutogenerationSetting;

		//Project paths
		boost::filesystem::path defaultOutputPath;
		boost::filesystem::path projectPath;

		//General network attributes
		boost::optional<UINT32> cycleTime;
		boost::optional<UINT32> asyncMTU;
		boost::optional<UINT32> multiplexedCycleLength;
		boost::optional<UINT32> prescaler;

		//Project Settings
		ViewType activeViewSettingType;
		std::string activeAutogenerationSettingsID;

		std::vector<ViewSetting> viewSettings;
		std::vector<Path> pathSettings;
		std::vector<AutoGenSetting> autogenerationSettings;
};

#endif // PROJECTCONFIGURATION_H_