#pragma once
#include <string>
#include <vector>

namespace FileSystem
{
	void GetFileNamesWithExtension(const std::string& dirPath, const std::string& ext, std::vector<std::string>& results);
}