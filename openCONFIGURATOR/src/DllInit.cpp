/************************************************************************
\file DllInit.cpp
\brief	Contains initialization in the DLL entry-point function.

		Initialization of the Boost.Log logging-library, Windows-only.
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifdef _MSC_VER

#include <windows.h>
#include <ShlObj.h>
#include <fstream>
#include <string>
#include <iostream>

#pragma warning(push, 0) // Store current warning state and set global warning level 0

#include "boost/log/utility/setup/from_stream.hpp"
#include "boost/log/utility/setup/from_settings.hpp"
#include "boost/log/utility/setup/settings.hpp"
#include "boost/log/utility/setup/settings_parser.hpp"
#include "boost/log/utility/setup/common_attributes.hpp"
#include "boost/log/utility/setup/formatter_parser.hpp"
#include "boost/log/utility/setup/filter_parser.hpp"
#include "boost/log/attributes/named_scope.hpp"
#include "boost/log/trivial.hpp"
#include "boost/bind.hpp"
#include "boost/optional.hpp"

#pragma warning(pop) // Restore previous warning state
#pragma warning (disable:4503 4714) // Boost-Headers themselves disable this warning, reinstate

#include "../Include/Constants.h"
#include "../Include/LibraryConfiguration.h"

namespace logging = boost::log;

/************************************************************************
\brief	Custom formatter for the boost.log "Scope" attribute.

		Implementation of a custom formatter to output file- and line number
		information for boost.log's "Scope" attribute.
************************************************************************/
struct scope_list_formatter
{
	typedef void result_type;
	typedef logging::attributes::named_scope::value_type scope_stack;

	explicit scope_list_formatter(const logging::attribute_name& name) : name(name)
	{
	}

	void operator()(const logging::record_view& rec, logging::formatting_ostream& strm) const
	{
		// We need to acquire the attribute value from the log record
		logging::visit<scope_stack>(
			name,
			rec.attribute_values(),
			boost::bind(&scope_list_formatter::format, _1, boost::ref(strm))
		);
	}

	private:
		logging::attribute_name name;

		// This is where our custom formatting takes place
		static void format(const scope_stack& scopes, logging::formatting_ostream& strm)
		{
			if (scopes.empty()) {
				strm << "(scope empty)";
			}
			else
			{
				logging::attributes::named_scope_list::const_reference scope_list = scopes.back();
				std::string file(scope_list.file_name.str());
				size_t pos = file.find_last_of(kPathSeparator);

				strm << file.substr(pos + 1)
					<< ":"
					<< scopes.back().line
					<< " ("
					<< scopes.back().scope_name
					<< ")";
			}			
		}
};

/************************************************************************
\brief	A custom Scope formatter factory.
		
		Custom formatter factory for a Scope attribute, using the custom
		formatter scope_list_formatter.
************************************************************************/
class scope_formatter_factory : public logging::formatter_factory<char>
{
	public:
		/* 
		 * This function creates a formatter for the Scopes attribute.
		 * It effectively associates the attribute with the scope_list_formatter class
		 */
		formatter_type create_formatter(
			logging::attribute_name const& attr_name, args_map const& args)
		{
			return formatter_type(scope_list_formatter(attr_name));
		}
};

/************************************************************************
\brief Determine path to Windows' "Local AppData" folder.

\param [in,out] directory	Path to Windows' local AppData folder. 
							Unchanged (possibly empty) if return value != 0.

\return	The result of the SHGetFolderPath() system call.
\retval	0x0	Successful
\retval 0x1	Folder does not exist
\retval	0x80070057L	CSIDL passed to SHGetFolderPath() is invalid
\note http://msdn.microsoft.com/en-us/library/windows/desktop/bb762181%28v=vs.85%29.aspx
************************************************************************/
static HRESULT getAppDataDirectory(std::string& directory)
{
	TCHAR localAppData[MAX_PATH] = { 0 };

	HRESULT result = SHGetFolderPath(NULL,
		CSIDL_LOCAL_APPDATA,
		NULL,
		SHGFP_TYPE_CURRENT,
		localAppData);
	if (result != S_OK)
		return result;
	directory = localAppData;
	return result;
}

/************************************************************************
\brief	DLL entry-point function.
\note http://msdn.microsoft.com/en-us/library/windows/desktop/ms682583%28v=vs.85%29.aspx
************************************************************************/
BOOL WINAPI DllMain(HINSTANCE hinstDLL,	DWORD fdwReason, LPVOID lpReserved)
{
	switch (fdwReason)
	{
		case DLL_PROCESS_ATTACH:
		{	
			// Determine the directory for our log file and exit if unsuccessful
			std::string dir;
			HRESULT result = getAppDataDirectory(dir);
			if (result != S_OK)
			{
				std::cerr << "Error trying to determine local AppData directory. Error code returned from SHGetFolderPath(): 0x" 
					<< std::hex 
					<< result 
					<< std::endl;
				return false;
			}

			// Read settings from settings file into file object
			std::string libResourcePath = LibraryConfiguration::GetLibResourcePath() + kPathSeparator + "boost_log_settings.ini";
			std::ifstream file(libResourcePath);
			if (file.fail())
			{
				std::cerr << "Error loading Boost.Log settings file '" << libResourcePath << "'." << std::endl;
				return false;
			}

			// Register our custom formatter factory for the Scope attribute
			logging::register_formatter_factory("Scope", boost::make_shared<scope_formatter_factory>());	

			// Register the Severity-Attribute formatter and filter factories for Severity-Logger (usually registered on creation of the logger).
			// Must happen before initializing from configuration file due to a bug in boost.log
			// See https://svn.boost.org/trac/boost/ticket/8840
			boost::log::register_simple_formatter_factory<logging::trivial::severity_level, char>("Severity");
			logging::register_simple_filter_factory<logging::trivial::severity_level, char>("Severity");

			// Register Scope-Attribute to maintain scope information
			logging::core::get()->add_global_attribute("Scope", logging::attributes::named_scope());

			// Registers common attribute like PID/TID and TimeStamp
			logging::add_common_attributes();
			
			// Parse settings from file into settings object
			logging::basic_settings<char> settings(logging::parse_settings(file));
			settings["Sinks.File"]["Target"] = (dir + kPathSeparator + "openCONFIGURATOR/logs");

			//logging::init_from_stream(file);
			logging::init_from_settings(settings);
			break;
		}
		case DLL_THREAD_ATTACH:
			// Do thread-specific initialization.
			break;

		case DLL_THREAD_DETACH:
			// Do thread-specific cleanup.
			break;

		case DLL_PROCESS_DETACH:
			// Perform any necessary cleanup.
			break;
	}
	return true;  // Successful DLL_PROCESS_ATTACH.
}// DllMain

#elif defined (__GNUG__)

	//TODO: Boost.Log initialization for Unix
	/* 
	 * 1. Compile *.so's for Unix
	 * 2. Include Boost.Log header files for Unix
	 * 3. Setup include/linker paths
	 * 4. Define Unix-Macros in Logging.h
	 * 5. Init Logging in *.so entry point function.
	 */

#endif // _MSC_VER