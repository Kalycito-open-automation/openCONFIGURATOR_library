#ifndef IMPORTEXPORT_API_H_
#define IMPORTEXPORT_API_H_

#include <string>
#include "Exports.h"
#include "Exception.h"

DLLEXPORT ocfmRetCode GenerateProcessImageDescription(const OutputLanguage lang, const string outputPath, const string fileName);
DLLEXPORT ocfmRetCode GenerateStackConfiguration(const string outputPath, const string fileName);

#endif