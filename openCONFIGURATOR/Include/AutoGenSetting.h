/************************************************************************
\file	AutoGenSetting.h
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	28.10.2014
************************************************************************/

#ifndef AUTOGENSETTING_H
#define AUTOGENSETTING_H

#include "Setting.h"
#include <string>
namespace openCONFIGURATOR
{
	namespace GUI
	{
		namespace ProjectFile
		{
			namespace Setting
			{
				class AutoGenSetting : public Setting
				{
					public:
						AutoGenSetting(const std::string id, const std::string name, const std::string value) : Setting(name, value), id(id)
						{}

						const std::string GetAutoGenId()
						{
							return this->id;
						}
					private:
						std::string id;
				};
			}
		}
	}
}
#endif