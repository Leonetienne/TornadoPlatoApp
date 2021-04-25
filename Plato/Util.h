#pragma once
#include <fstream>
#include <sstream>
#include <string>

inline std::string ReadFile(const std::string& filepath)
{
    std::ifstream ifs;
    ifs.open(filepath);
    if (!ifs.good())
    {
        throw std::exception("No such file");
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
