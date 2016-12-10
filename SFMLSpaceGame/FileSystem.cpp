// This is an independent project of an individual developer. Dear PVS-Studio, please check it.
// PVS-Studio Static Code Analyzer for C, C++ and C#: http://www.viva64.com
#include "stdafx.h"
#include <FileSystem.h>
#include <filesystem>

namespace FileSystem
{
	// Sure, the <filesystem> include is experimental, but it is working for me now.
	// Might need to revist this if I go to make things cross platform
	using namespace std::experimental::filesystem::v1;
	void GetFileNamesWithExtension(const std::string& dirPath, const std::string& ext, std::vector<std::string>& results)
	{
		path dir = { dirPath };

		for (const auto& entry : directory_iterator(dir) )
		{
			if (is_regular_file(entry.status()))
			{
				auto& extension = entry.path().extension();

				if (extension == ext)
					results.push_back(entry.path().filename().generic_string());
			}
		}
	}

	void GetFileNamesWithExtensions(const std::string& dirPath, 
									std::initializer_list<std::string> extensions,
									std::vector<std::string>& results)
	{
		path dir = { dirPath };

		for (const auto& entry : directory_iterator(dir))
		{
			if (is_regular_file(entry.status()))
			{
				auto& extension = entry.path().extension();

				for (auto& ext : extensions)
				{
					if (extension == ext)
					{
						results.push_back(entry.path().filename().generic_string());
						break;
					}
				}
			}
		}
	}

	bool DoesFileExist(const std::string& name)
	{
		path p = { name };
		
		return is_regular_file(p);
	}
}