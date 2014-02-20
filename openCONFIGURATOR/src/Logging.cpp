/************************************************************************
\file	Logging.cpp
\brief	Function definitions for logging functionality.
\author David Puffer, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
************************************************************************/

#include "../Include/Logging.h"

#ifdef DEBUG

std::ostream& Log(const char* file, const char* func, const int line, std::ostream& streamref)
{
	streamref
	        << file
	        << "(Ln " << line << ")"
	        << ":"
	        << func << "(): ";
	return streamref;
}

#endif // DEBUG