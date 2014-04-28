/************************************************************************
\file BoostShared.h
\brief	Master-Include file for Boost-Headers to be used in the library.

		This header file shall include Boost-Headers which issue warnings at
		warning level 4 ("/W4") of Microsoft Visual Studio's C++ compiler.
		Any implementation file using any of these Boost-Headers shall include
		this Master-Include file instead of directly including those Boost-Headers.
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef BOOSTSHARED_H_
#define BOOSTSHARED_H_

#ifdef _MSC_VER
#pragma warning(push, 0) // Store current warning state and set global warning level 0
#endif

#include <boost/optional.hpp>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/algorithm/string.hpp>
#include <boost/format.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/filesystem.hpp>

#ifdef _MSC_VER
#pragma warning(pop) // Restore previous warning state
#pragma warning (disable:4503 4714) // Boost-Headers themselves disable this warning, reinstate
#endif

#endif