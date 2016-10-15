#pragma once
#include <string>
#include <vector>

#ifdef RELEASE
#ifndef DATA_PATH
#define DATA_PATH "Data\\"
#endif
#else
#ifndef DATA_PATH
#define DATA_PATH "..\\Data\\"
#endif
#endif

namespace FileSystem
{
	void GetFileNamesWithExtension(const std::string& dirPath, const std::string& ext, std::vector<std::string>& results);
}