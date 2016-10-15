#include <FileSystem.h>
/*#include <tinydir.h>
#include <atlstr.h>
#include <SFML/System/String.hpp>*/
#include <filesystem>

namespace FileSystem
{
	using namespace std::experimental::filesystem::v1;
	void GetFileNamesWithExtension(const std::string& dirPath, const std::string& ext, std::vector<std::string>& results)
	{
		path dir = { dirPath };

		for (const auto& entry : directory_iterator(dir) )
		{
			if (is_regular_file(entry.status()))
			{
				auto extension = entry.path().extension();

				if (extension == ext)
					results.push_back(entry.path().filename().generic_string());
			}
		}

		/*sf::String str{ dirPath };
		sf::String extension{ ext };
		
		tinydir_dir dir;
		if (tinydir_open(&dir, str.toWideString().c_str()) == -1)
		{
			perror("Error opening file");
			tinydir_close(&dir);
			return;
		}

		while(dir.has_next)
		{
			tinydir_file file;
			if (tinydir_readfile(&dir, &file) == -1)
			{
				perror("Error getting file");
				tinydir_close(&dir);
				return;
			}

			if (StrCmpW(file.extension, extension.toWideString().c_str()))
			{
				results.push_back(sf::String{ file.name }.toAnsiString());
			}
		}

		tinydir_close(&dir);*/
	}
}