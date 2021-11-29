//
// Created by Acer on 7.11.2021 г..
//

#ifndef MAGMA_BASE_H
#define MAGMA_BASE_H

#if defined(_Magma_Core)
#define _Magma_Dll __declspec(dllexport)
#else
#define _Magma_Dll __declspec(dllimport)
#endif

#define _Magma_Assert(x, y) if(!(x)) {y; __debugbreak();}
#define _Magma_VkAssert(x, y) if((x) != VK_SUCCESS) {y; __debugbreak();}
#define _Magma_Define_VkHandle(object) typedef struct object##_T* object;

#endif // MAGMA_BASE_H
