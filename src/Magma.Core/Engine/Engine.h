//
// Created by Acer on 21.11.2021 г..
//

#ifndef MAGMA_ENGINE_H
#define MAGMA_ENGINE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Engine/Module.h"

#include <map>
#include <functional>
#include <vector>

namespace Magma
{
    /// Responsible for initialization of all modules from all stages.
    /// Manages the lifetime of all modules.
    class MAGMA_API Engine
    {
    public:
        Engine();
        ~Engine();

        /// Updates all stages of the engine if the window is not minimized.
        /// TODO: If the latter is not minimized, ...
        void Run();
        void UpdateStage(Module::Stage stage);
    private:
        std::multimap<Module::StageIndex, Ref<Module>> _Modules;
    };
}

#endif //MAGMA_ENGINE_H
