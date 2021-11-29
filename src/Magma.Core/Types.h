//
// Created by Acer on 7.11.2021 г..
//

#ifndef MAGMA_TYPES_H
#define MAGMA_TYPES_H

#include <memory>
#include <typeindex>
#include <unordered_map>

namespace Magma
{
    template<typename T>
    using Scope = std::unique_ptr<T>;

    template<typename T, typename ...TArgs>
    constexpr Scope<T> CreateScope(TArgs... args)
    {
        return std::make_unique<T>(std::forward<TArgs>(args)...);
    }

    template<typename T>
    using Ref = std::shared_ptr<T>;

    template<typename T, typename ...TArgs>
    constexpr Ref<T> CreateRef(TArgs... args)
    {
        return std::make_shared<T>(std::forward<TArgs>(args)...);
    }

    template<typename T>
    using Weak = std::weak_ptr<T>;

}

#endif //MAGMA_TYPES_H
