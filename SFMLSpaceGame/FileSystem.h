#pragma once
#include <string>
#include <vector>

#ifdef RELEASE

#ifndef DATA_PATH
#define DATA_PATH "Data\\"
#endif
#ifndef IMAGE_PATH
#define IMAGE_PATH "Images\\"
#endif

#else

#ifndef DATA_PATH
#define DATA_PATH "..\\Data\\"
#endif
#ifndef IMAGE_PATH
#define IMAGE_PATH "Images\\"
#endif

#endif

namespace FileSystem
{
	void GetFileNamesWithExtension(const std::string& dirPath, 
									const std::string& ext, 
									std::vector<std::string>& results);

	void GetFileNamesWithExtensions(const std::string& dirPath, 
									std::initializer_list<std::string> extensions,
									std::vector<std::string>& results);

	bool DoesFileExist(const std::string& name);
}