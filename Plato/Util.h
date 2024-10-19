#pragma once
#include <fstream>
#include <sstream>
#include <string>

namespace Plato
{
    namespace Util
    {
        //! Will read a file to a string
        inline std::string ReadFile(const std::string& filepath)
        {
            std::ifstream ifs;
            ifs.open(filepath);
            if (!ifs.good())
            {
                throw std::runtime_error(std::string("No such file \"") + filepath + "\"");
                std::terminate();
            }
            std::string buf;
            std::stringstream content;
            while (std::getline(ifs, buf))
            {
                content << buf << std::endl;
            }
            return content.str();
        }

        //! Will return the path to the directory of a file
        inline std::string FilePathToDirPath(const std::string& filepath)
        {
            // Delete the last file path segment of filepath
            // Reverse-Advance to the last occurence of /, or index 0
            std::size_t i = filepath.length() - 1;
            for (; filepath[i] != '/' && i >= 0; i--) {
                continue;
            }

            // Cut the string: translate ./test/file.obj to ./test
            return filepath.substr(0, i);
        }
    }
}
