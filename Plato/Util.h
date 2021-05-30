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
                throw std::runtime_error("No such file");
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
    }
}
