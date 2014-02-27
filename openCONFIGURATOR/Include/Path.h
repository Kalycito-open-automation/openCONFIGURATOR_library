/************************************************************************
\file	Path.h
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	28.10.2014
************************************************************************/

#ifndef PATH_H
#define PATH_H

#include <string>
#include "Setting.h"

namespace openCONFIGURATOR
{
	namespace GUI
	{
		namespace ProjectFile
		{
			namespace Setting
			{
				class Path : public Setting
				{
					public:
						Path(std::string name, std::string value) : Setting(name, value)
						{}
				};
			}
		}
	}
}
#endif