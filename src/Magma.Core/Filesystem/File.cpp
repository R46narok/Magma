//
// Created by Acer on 12.11.2021 г..
//

#include "Magma.Core/Filesystem/File.h"

#include <fstream>
#include <filesystem>

namespace Magma
{
    std::optional<std::vector<char>> ReadFileBinary(const std::filesystem::path& path)
    {
        std::ifstream stream(path, std::ios::ate | std::ios::binary);

        if (!stream.is_open())
            return {};

        auto size = (size_t)stream.tellg();
        std::vector<char> buffer(size);

        stream.seekg(0);
        stream.read(buffer.data(), size);
        stream.close();

        return {buffer};
    }
}
