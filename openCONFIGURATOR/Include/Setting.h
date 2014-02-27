/************************************************************************
\file	Setting.h
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	28.10.2014
************************************************************************/

#ifndef SETTING_H
#define SETTING_H

#include <string>
namespace openCONFIGURATOR
{
	namespace GUI
	{
		namespace ProjectFile
		{
			namespace Setting
			{
				class Setting
				{
					public:
						Setting():
							name(""),
							value("")
						{}

						Setting(const std::string name, const std::string value):
							name(name),
							value(value)
						{}

						const std::string GetName()
						{
							return this->name;
						}

						const std::string GetValue()
						{
							return this->value;
						}

					private:
						std::string name;
						std::string value;
				};
			}
		}
	}
}
#endif