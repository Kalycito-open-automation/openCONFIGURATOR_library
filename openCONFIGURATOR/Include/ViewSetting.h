/************************************************************************
\file	ViewSetting.h
\author Christoph Ruecker, Bernecker + Rainer Industrie Elektronik Ges.m.b.H.
\date	28.10.2014
************************************************************************/

#ifndef VIEWSETTING_H
#define VIEWSETTING_H

#include "Setting.h"
#include "Enums.h"
#include <string>

namespace openCONFIGURATOR
{
	namespace GUI
	{
		namespace ProjectFile
		{
			namespace Setting
			{
				class ViewSetting : public Setting
				{
					public:
						ViewSetting(const openCONFIGURATOR::GUI::ProjectFile::ViewType::ViewType type, const std::string name, const std::string value) : Setting(name, value), viewType(type)
						{}

						openCONFIGURATOR::GUI::ProjectFile::ViewType::ViewType GetViewType()
						{
							return this->viewType;
						}

					private:
						openCONFIGURATOR::GUI::ProjectFile::ViewType::ViewType viewType;
				};
			}
		}
	}
}
#endif