#ifndef LIBRARYCONFIGURATION_H
#define LIBRARYCONFIGURATION_H

#include <string>
using namespace std;

class LibraryConfiguration
{
	private:
		static LibraryConfiguration* instance;
		LibraryConfiguration(void);
		LibraryConfiguration(const LibraryConfiguration&);

		void SetLibResourcePath(const string& libResourcePath);
		bool stringCompare(const string & l, const string & r);
		void InitFilePaths();

		string libResourcePath;
		string objectDictinaryFilePath;
		string xddSchemaFilePath;
		string projectFileSchemaFilePath;
		string txt2CdcFilePath;


	public:
		static LibraryConfiguration* GetInstance(void);
		static bool Initialize(const string& libResourcePath);

		const string& GetLibResourcePath(void);
		~LibraryConfiguration(void);

		const string& GetObjectDictinaryFilePath(void);
		const string& GetXddSchemaFilePath(void);
		const string& GetProjectFileSchemaFilePath(void);
		const string& GetTxt2CdcFilePath(void);
};

#endif