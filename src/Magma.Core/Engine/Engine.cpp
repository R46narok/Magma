//
// Created by Acer on 21.11.2021 г..
//

#include "Magma.Core/Engine/Engine.h"
#include "Magma.Core/Window.h"
#include "Magma.Core/Engine/Log.h"

namespace Magma
{
    Engine::Engine()
    {
        std::vector<ModuleId> created;
        for (;;)
        {
            bool postponed = false;
            for (const auto &[moduleId, moduleTest] : Module::Registry())
            {

                if (std::find (created.begin(), created.end(), moduleId) != created.end())
                    continue;

                bool this_postponed = false;
                for (const auto &requireId : moduleTest.Requires)
                {
                    if (std::find(created.begin(), created.end(), requireId) == created.end())
                    {
                        this_postponed = true;
                        break;
                    }
                }
                if (this_postponed)
                {
                    postponed = true;
                    continue;
                }
                auto &&module = moduleTest.Create();
                _Modules.emplace(Module::StageIndex(moduleTest.Stage, moduleId.hash_code()), std::move(module));
                created.emplace_back(moduleId);
            }
            if (!postponed)
                break;
        }
    }

    void Engine::Run()
    {
        while (Window::Get()->IsAlive())
        {
            UpdateStage(Module::Stage::Pre);
            UpdateStage(Module::Stage::Normal);
            UpdateStage(Module::Stage::Post);
            UpdateStage(Module::Stage::Render);
        }
    }

    void Engine::UpdateStage(Module::Stage stage)
    {
        for (auto &[stageIndex, module] : _Modules)
        {
            if (stageIndex.first == stage)
                module->Update();
        }
    }
}
