#ifndef PROJECTCONFIGURATION_H
#define PROJECTCONFIGURATION_H

#include <string>
#include <iostream>
#include <sstream>
#include "Exception.h"
#include "NodeCollection.h"
#include "Internal.h"
#include "Validation.h"

#include <libxml/xmlwriter.h>
#include <libxml/encoding.h>
#include <libxml/xmlreader.h>

using namespace std;

class ProjectConfiguration
{
public:
	static ProjectConfiguration* GetInstance(void);

	const string& GetDefaultOutputPath(void);
	void SetDefaultOutputPath(const string& defaultOutputPath);

	const string& GetPathTxt2CdcExecutable(void);
	void SetPathTxt2CdcExecutable(const string& pathTxt2CdcExecutable);

	bool GetGenerateMNOBD(void);
	void SetGenerateMNOBD(bool generateMNOBD);

	bool GetProjectLoaded(void);
	void SetProjectLoaded(bool projectLoaded);

	const string& GetProjectFile(void);
	void SetProjectFile(const string& projectFile);

	const string& GetProjectPath(void);
	void SetProjectPath(const string& projectPath);

	UINT32 GetCycleTime(void);
	void SetCycleTime(UINT32 cycleTime);

	UINT32 GetAsyncMTU(void);
	void SetAsyncMTU(UINT32 asyncMTU);

	UINT32 GetMultiplexedCycleLength(void);
	void SetMultiplexedCycleLength(UINT32 multiplexedCycleLength);

	UINT32 GetPrescaler(void);
	void SetPrescaler(UINT32 prescaler);

	ocfmRetCode LoadProject(const string& projectFile);
	void resetConfiguration(void);

	~ProjectConfiguration(void);

private:
	static ProjectConfiguration* instance;
	ProjectConfiguration(void);
	ProjectConfiguration(const ProjectConfiguration&);
	

	void ProcessProjectElement(xmlTextReaderPtr xmlReader);
	void ProcessProjectNode(xmlTextReaderPtr xmlReader);
	void ProcessPath(xmlTextReaderPtr xmlReader);
	void ProcessNetworkConfiguration(xmlTextReaderPtr xmlReader);
	void ProcessAutogenerationSettings(xmlTextReaderPtr xmlReader);
	void ProcessAutogenerationSetting(xmlTextReaderPtr xmlReader);

	void SynchronizeMultiplexingCycleLength(void);
	void SynchronizeCycleTime(void);
	void SynchronizeAsyncMtu(void);
	void SynchronizePrescaler(void);
	
	//General attributes
	bool projectLoaded;

	//Project related attributes
	string projectFile;
	string projectPath;
	bool generateMNOBD;
	string autogenerationSettingID;

	//Project paths
	string defaultOutputPath;
	string pathTxt2CdcExecutable;

	//General network attributes
	UINT32 cycleTime;
	UINT32 asyncMTU;
	UINT32 multiplexedCycleLength;
	UINT32 prescaler;
	
	//temp variable
	bool isChosenSetting;
};

#endif