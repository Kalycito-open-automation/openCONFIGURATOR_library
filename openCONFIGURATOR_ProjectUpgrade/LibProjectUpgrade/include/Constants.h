/************************************************************************
\file Constants.h
\brief	Constants ("real" constants or #defines) used throughout the code base.
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef _PROJECT_UPGRADE_CONSTANTS_H_
#define _PROJECT_UPGRADE_CONSTANTS_H_

#include "Namespace.h"

PROJECT_UPGRADE_OPEN_NAMESPACE

const char kPathSeparator =
		#ifdef _WIN32
		'\\';
#else
		'/';
#endif

PROJECT_UPGRADE_CLOSE_NAMESPACE

#endif // _PROJECT_UPGRADE_CONSTANTS_H_
