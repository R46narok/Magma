//
// Created by Acer on 12.11.2021 г..
//

#ifndef MAGMA_FILE_H
#define MAGMA_FILE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

#include <filesystem>
#include <optional>
#include <vector>
#include <string>

namespace Magma
{
    _Magma_Dll std::optional<std::vector<char>> ReadFileBinary(const std::filesystem::path& path);
}

#endif //MAGMA_FILE_H
