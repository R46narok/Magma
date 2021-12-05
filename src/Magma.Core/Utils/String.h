//
// Created by Acer on 5.12.2021 г..
//

#ifndef MAGMA_STRING_H
#define MAGMA_STRING_H

#include "Magma.Core/Base.h"

#include <string>
#include <vector>

namespace Magma
{
    class MAGMA_API String
    {
    public:
        static std::vector<std::string>& Split(const std::string &str, char sep);
        static std::string& ReplaceFirst(std::string& str, const std::string_view& token, const std::string_view& to);
    };
}

#endif //MAGMA_STRING_H
