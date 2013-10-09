/************************************************************************
\file Constants.h
\brief	Constants ("real" constants or #defines) used throughout the codebase.
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

const char kPathSeparator =
#ifdef _WIN32
	'\\';
#else
	'/';
#endif