@echo off
%VULKAN_SDK%/Bin32/glslc.exe ../res/shader.vert -o vert.spv
%VULKAN_SDK%/Bin32/glslc.exe ../res/shader.frag -o frag.spv