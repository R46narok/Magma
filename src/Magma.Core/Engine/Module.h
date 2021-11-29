//
// Created by Acer on 12.11.2021 г..
//

#ifndef MAGMA_MODULE_H
#define MAGMA_MODULE_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"

#include <map>
#include <functional>
#include <typeindex>
#include <vector>

namespace Magma
{
    using ModuleId = std::type_index;
    template<typename TBase>
    class ModuleFactory
    {
    public:
        class TCreateValue
        {
        public:
            std::function<Scope<TBase>()> Create;
            typename TBase::Stage Stage;
            std::vector<ModuleId> Requires;
        };

        using TRegistryMap = std::unordered_map<ModuleId, TCreateValue>;

        virtual ~ModuleFactory() noexcept = default;

        static TRegistryMap &Registry()
        {
            static TRegistryMap impl;
            return impl;
        }

        template<typename T>
        class ModuleInfo {
        public:
            ModuleInfo() = delete;

            template<typename K,
                    typename = std::enable_if_t<std::is_convertible_v<K *, T *>>>
            static std::type_index GetTypeId() noexcept
            {
                return {typeid(K)};
            }
        };

        template<typename... TArgs>
        class Requires
        {
        public:
            [[nodiscard]] std::vector<ModuleId> Get() const
            {
                std::vector<ModuleId> req;
                (req.emplace_back(ModuleInfo<TBase>::template GetTypeId<TArgs>()), ...);
                return req;
            }
        };

        template<typename T>
        class Registrar : public TBase {
        public:
            static T *Get() { return moduleInstance; }

        protected:
            template<typename ... Args>
            static bool Register(typename TBase::Stage stage, Requires<Args...> &&req = {}) {
                ModuleFactory::Registry()[ModuleInfo<TBase>::template GetTypeId<T>()] = {[]() {
                    moduleInstance = new T();
                    return Scope<TBase>(moduleInstance);
                }, stage, req.Get()};
                return true;
            }

            inline static T *moduleInstance = nullptr;
        };
    };

    class Module : public ModuleFactory<Module>
    {
    public:
        enum class Stage : uint8_t {
            Never, Always, Pre, Normal, Post, Render
        };

        using StageIndex = std::pair<Stage, std::size_t>;

        virtual ~Module() = default;

        virtual void Update() = 0;
    };

}

#endif //MAGMA_MODULE_H
