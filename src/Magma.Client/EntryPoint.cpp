//
// Created by Acer on 7.11.2021 г..
//

#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Engine/Engine.h"

using namespace Magma;

#include <filesystem>
#include <vulkan/vulkan.hpp>

int main()
{
    InitializeLog();
    //std::vector<Ref<Shader>> shaders{CreateRef<Shader>("vert.spv", ShaderType::Vertex), CreateRef<Shader>("frag.spv", ShaderType::Fragment)};

    auto engine = CreateScope<Engine>();
    engine->Run();

    /*while(window->IsAlive())
    {
        //window->Update();
        //graphics->Update();
    }*/

    return 0;
}