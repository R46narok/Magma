//
// Created by Acer on 8.11.2021 г..
//

#include "Magma.Core/Engine/Log.h"

#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/fmt/ostr.h>
#include <vector>

namespace Magma
{
    static Ref<spdlog::logger> CoreLogger;
    static Ref<spdlog::logger> ClientLogger;

    void InitializeLog()
    {
        std::vector<spdlog::sink_ptr> logSinks;
        logSinks.emplace_back(std::make_shared<spdlog::sinks::stdout_color_sink_mt>());
        logSinks.emplace_back(std::make_shared<spdlog::sinks::basic_file_sink_mt>("Magma.log", true));

        logSinks[0]->set_pattern("%^[%T] %n: %v%$");
        logSinks[1]->set_pattern("[%T] [%l] %n: %v");

        CoreLogger = std::make_shared<spdlog::logger>("CORE", begin(logSinks), end(logSinks));
        spdlog::register_logger(CoreLogger);
        CoreLogger->set_level(spdlog::level::trace);
        CoreLogger->flush_on(spdlog::level::trace);

        ClientLogger = std::make_shared<spdlog::logger>("APP", begin(logSinks), end(logSinks));
        spdlog::register_logger(ClientLogger);
        ClientLogger->set_level(spdlog::level::trace);
        ClientLogger->flush_on(spdlog::level::trace);
    }

    Ref<spdlog::logger> GetCoreLogger()
    {
        return CoreLogger;
    }

    Ref<spdlog::logger> GetClientLogger()
    {
        return ClientLogger;
    }
}
