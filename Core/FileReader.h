#pragma once

#include <string>
#include <fstream>
#include <cassert>

namespace VenusEngine
{
	class FileReader
	{
	public:
		static std::string readFile(std::string const& path)
		{
			std::ifstream filename(path);
			if (!filename)
			{
				std::cout << "File %s does not exist; exiting\n" << path << std::endl;
				exit(-1);
			}

			return std::string((std::istreambuf_iterator<char>(filename)),
								std::istreambuf_iterator<char>());
		}
	};
}
