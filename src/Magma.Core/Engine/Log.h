//
// Created by Acer on 8.11.2021 г..
//

#ifndef MAGMA_LOG_H
#define MAGMA_LOG_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

#include <spdlog/spdlog.h>

namespace Magma
{
    MAGMA_API void InitializeLog();
    MAGMA_API Ref<spdlog::logger> GetCoreLogger();
    MAGMA_API Ref<spdlog::logger> GetClientLogger();
}

#define _Magma_Core_Trace(...)    ::Magma::GetCoreLogger()->trace(__VA_ARGS__)
#define _Magma_Core_Info(...)     ::Magma::GetCoreLogger()->info(__VA_ARGS__)
#define _Magma_Core_Warn(...)     ::Magma::GetCoreLogger()->warn(__VA_ARGS__)
#define _Magma_Core_Error(...)    ::Magma::GetCoreLogger()->error(__VA_ARGS__)
#define _Magma_Core_Critical(...) ::Magma::GetCoreLogger()->critical(__VA_ARGS__)

// Client log macros
#define _Magma_Trace(...)         ::Magma::GetClientLogger()->trace(__VA_ARGS__)
#define _Magma_Info(...)          ::Magma::GetClientLogger()->info(__VA_ARGS__)
#define _Magma_Warn(...)          ::Magma::GetClientLogger()->warn(__VA_ARGS__)
#define _Magma_Error(...)         ::Magma::GetClientLogger()->error(__VA_ARGS__)
#define _Magma_Critical(...)      ::Magma::GetClientLogger()->critical(__VA_ARGS__)

#endif //MAGMA_LOG_H
