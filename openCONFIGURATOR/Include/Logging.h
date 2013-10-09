/************************************************************************
\file	Logging.h
\brief	Macros and function headers for logging functionality.

		Boost.Log macros for Windows only.
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef LOGGING_H_
#define LOGGING_H_

#ifdef _MSC_VER

/************************************************************************/
/*						Boost logging                                   */
/************************************************************************/
#include <boost/log/core/core.hpp>
#include "boost/log/sources/severity_logger.hpp"
#include "boost/log/sources/global_logger_storage.hpp"
#include "boost/log/trivial.hpp"
#include "boost/log/attributes/named_scope.hpp"

// Create global severity logger with multi threading support
BOOST_LOG_INLINE_GLOBAL_LOGGER_DEFAULT(sev_logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>);

// Custom macros for severity/scope logging
#define LOG_TRACE() \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::trace)

#define LOG_DEBUG() \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::debug)

#define LOG_INFO() \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::info)

#define LOG_WARN() \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::warning)

#define LOG_ERROR() \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::error)

#define LOG_FATAL() \
	BOOST_LOG_FUNCTION(); \
	BOOST_LOG_SEV(sev_logger::get(), boost::log::trivial::fatal)
/************************************************************************/

#elif defined (__GNUG__)

#include <iostream>

// No Boost.Log under Linux - Use custom LOG macros
#define LOG_TRACE() ELOG(std::cerr)
#define LOG_DEBUG() ELOG(std::cerr)
#define LOG_INFO() ELOG(std::cerr)
#define LOG_WARN() ELOG(std::cerr)
#define LOG_ERROR() ELOG(std::cerr)
#define LOG_FATAL() ELOG(std::cerr)

#endif

/************************************************************************/
/*						Custom logging-functions						*/
/************************************************************************/
#ifdef DEBUG

#include <iostream>

#ifdef __GNUG__
// Older versions of GCC support __FUNCTION__, newer ones the
// standardized ANSI C99 __func__
#ifndef __func__
#define __func__ __FUNCTION__
#endif
#elif defined (_MSC_VER)
#define __func__ __FUNCTION__
#else
#define __func__ <unknown>
#endif // __GNUG__

/************************************************************************
\brief	Write logging output.

		Will write the parameters in the format '<file>(<line>):<function>: ' to streamref
		and return streamref. This function will not append an "endl".

\param [in] file	The filename to output.
\param [in] func	The function name to output.
\param [in] line	The line number to output.
\param [in,out] streamref The stream to write to, i.e. std::cout, std::cerr etc.

\return A reference to the passed streamref, which clients can use to pass in a message.
************************************************************************/
std::ostream& Log(const char* file, const char* func, const int line, std::ostream& streamref);

// This macro will not append an "endl" to the message!
#define ELOG(streamref) if (false) {} else Log(__FILE__, __func__, __LINE__, streamref)

// If this macro is being used in if-blocks, always enclose in braces to avoid "dangling else" issues!!
#define LOG(message) std::cerr << __FILE__ << "(Ln " << __LINE__ << ")" << ":" << __func__ << "(): " << message << std::endl;

#else // RELEASE

#define ELOG(streamref) if (true) {} else cerr
#define LOG(message) ((void)0); 

#endif // DEBUG
/************************************************************************/

#endif // LOGGING_H_