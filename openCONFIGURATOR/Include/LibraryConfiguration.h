/************************************************************************
\file	LibraryConfiguration.h
\brief	Functionality to retrieve library configuration options.
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef LIBRARYCONFIGURATION_H_
#define LIBRARYCONFIGURATION_H_

#include <string>

class LibraryConfiguration
{
	private:
		LibraryConfiguration(void);
		LibraryConfiguration(const LibraryConfiguration&);
		void operator=(const LibraryConfiguration&);
		
		static std::string libResourcePath;
		static const std::string kDefaultLibResourcePath;
		static const std::string kLibResourcePathEnvVar;
		static const std::string kObjectDictionaryFile;
		static const std::string kRelXDDSchemaPath;
		static const std::string kRelProjectFileSchemaPath;
		static const std::string kTxt2CdcFile;

	public:

		/************************************************************************
		\brief	Get library resource path.
				
				The library resource path will be lazily instantiated the first time
				this method is called and will be determined as follows:<br/>
				1. If environment variable OPENCONFIGURATOR_LIBRARY_RESOURCEPATH exists, 
				   resource path will be set to its value and returned.
				2. Otherwise "resources" will be set as resource path and returned.

		\return The library resource path containing all external resources the library could possibly load.
		************************************************************************/
		static const std::string& GetLibResourcePath(void);

		/************************************************************************
		\return The path to the object dictionary template "od.xml".
		************************************************************************/
		static const std::string GetObjectDictinaryFilePath(void);

		/************************************************************************
		\return The path to the XDD XML-Schema file "Powerlink_main.xsd".
		************************************************************************/
		static const std::string GetXddSchemaFilePath(void);

		/************************************************************************
		\return The path to the openCONFIGURATOR project-file schema "openCONFIGURATOR.xsd".
		************************************************************************/
		static const std::string GetProjectFileSchemaFilePath(void);

		/************************************************************************
		\return The path to the openCONFIGURATOR command line utility "txt2cdc".
				Will return including the ".exe" extension if compiled under Windows.
		************************************************************************/
		static const std::string GetTxt2CdcFilePath(void);

		~LibraryConfiguration(void);		
};

#endif // LIBRARYCONFIGURATION_H_