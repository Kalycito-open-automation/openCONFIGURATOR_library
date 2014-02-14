/************************************************************************
\file Constants.h
\brief	Constants ("real" constants or #defines) used throughout the codebase.
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#ifndef CONSTANTS_H_
#define CONSTANTS_H_

const char kPathSeparator =
#ifdef _WIN32
	'\\';
#else
	'/';
#endif

#endif // CONSTANTS_H_