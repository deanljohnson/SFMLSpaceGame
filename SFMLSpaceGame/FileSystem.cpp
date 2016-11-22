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
}