//
// Created by Acer on 7.11.2021 г..
//

#ifndef MAGMA_BASE_H
#define MAGMA_BASE_H

#if defined(_Magma_Core)
#define MAGMA_API __declspec(dllexport)
#else
#define MAGMA_API __declspec(dllimport)
#endif

#define _Magma_Assert(x, y) if(!(x)) {_Magma_Core_Error(y); __debugbreak();}
#define _Magma_VkAssert(x, y) if((x) != VK_SUCCESS) {y; __debugbreak();}
#define _Magma_Define_VkHandle(object) typedef struct object##_T* object;

namespace Magma
{

    struct Configuration
    {
#ifdef _DEBUG
        static constexpr bool IsDebug = true;
#else
        static constexpr bool IsDebug = false;
#endif
    };

}

#endif // MAGMA_BASE_H
