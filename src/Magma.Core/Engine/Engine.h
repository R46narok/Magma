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
    class _Magma_Dll Engine
    {
    public:
        Engine();

        void Run();
        void UpdateStage(Module::Stage stage);
    private:
        std::multimap<Module::StageIndex, Ref<Module>> _Modules;
    };
}

#endif //MAGMA_ENGINE_H
