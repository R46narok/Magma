//
// Created by Acer on 5.12.2021 г..
//

#include "String.h"
#include <sstream>

namespace Magma
{
    std::string& String::ReplaceFirst(std::string &str, const std::string_view &token, const std::string_view &to)
    {
        const auto startPos = str.find(token);
        if (startPos == std::string::npos)
            return str;

        str.replace(startPos, token.length(), to);
        return str;
    }

    std::vector<std::string>& String::Split(const std::string &str, char sep)
    {
        std::vector<std::string> tokens;
        std::string token;
        std::istringstream tokenStream(str);

        while (std::getline(tokenStream, token, sep))
            tokens.emplace_back(token);
        return tokens;
    }

}


