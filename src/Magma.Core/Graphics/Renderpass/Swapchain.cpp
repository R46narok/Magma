//
// Created by Acer on 10.11.2021 г..
//

#include "Magma.Core/Engine/Log.h"
#include "Magma.Core/Graphics/Graphics.h"
#include "Magma.Core/Graphics/Renderpass/Swapchain.h"

#include <array>
#include <limits>
#include <algorithm>
#include <GLFW/glfw3.h>

namespace Magma
{
    SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface)
    {
        SwapchainSupportDetails details;

        vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface, &details.Capabilities);

        uint32_t iFormatCount = 0;
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &iFormatCount, nullptr);
        if (iFormatCount != 0)
        {
            details.Formats.resize(iFormatCount);
            vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &iFormatCount, details.Formats.data());
        }

        uint32_t iPresentModeCount = 0;
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &iPresentModeCount, nullptr);
        if (iPresentModeCount != 0)
        {
            details.PresentModes.resize(iPresentModeCount);
            vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &iPresentModeCount, details.PresentModes.data());
        }

        return details;
    }

    Swapchain::Swapchain()
        : _Swapchain(VK_NULL_HANDLE)
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        auto physicalDevice = Graphics::GetPhysicalDevice()->GetVulkanPhysicalDevice();
        auto surface = Graphics::GetSurface()->GetVulkanSurface();
        auto window = Window::Get();

        auto support = QuerySwapchainSupport(physicalDevice, surface);
        VkSurfaceFormatKHR format = ChooseFormat(support.Formats);
        VkPresentModeKHR presentMode = ChoosePresentMode(support.PresentModes);
        VkExtent2D extent = ChooseExtent2D(support.Capabilities, window);

        uint32_t iImageCount = support.Capabilities.minImageCount + 1;
        if (support.Capabilities.minImageCount > 0 && iImageCount > support.Capabilities.minImageCount)
            iImageCount = support.Capabilities.maxImageCount;

        VkSwapchainCreateInfoKHR createInfo{
            .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
            .surface = surface,
            .minImageCount = iImageCount,
            .imageFormat = format.format,
            .imageColorSpace = format.colorSpace,
            .imageExtent = extent,
            .imageArrayLayers = 1,
            .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
            .preTransform = support.Capabilities.currentTransform,
            .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
            .presentMode = presentMode,
            .clipped = true,
            .oldSwapchain = VK_NULL_HANDLE
        };

        auto indices = FindQueueFamilies(physicalDevice, surface);
        std::array<uint32_t, 2> queueFamilyIndices{
            indices.GraphicsFamily.value(),
            indices.PresentFamily.value()
        };

        if (indices.GraphicsFamily != indices.PresentFamily)
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
            createInfo.queueFamilyIndexCount = 2;
            createInfo.pQueueFamilyIndices = queueFamilyIndices.data();
        }
        else
        {
            createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
            createInfo.queueFamilyIndexCount = 0;
            createInfo.pQueueFamilyIndices = nullptr;
        }

        Graphics::CheckVk(vkCreateSwapchainKHR(device, &createInfo, nullptr, &_Swapchain));
        _Magma_Core_Info("Swapchain created");

        vkGetSwapchainImagesKHR(device, _Swapchain, &iImageCount, nullptr);
        _Images.resize(iImageCount);
        vkGetSwapchainImagesKHR(device, _Swapchain, &iImageCount, _Images.data());

        _Extent = extent;
        _Format = format.format;

        CreateImageViews();
    }

    Swapchain::~Swapchain() noexcept
    {
        auto device = Graphics::GetDevice()->GetVulkanDevice();
        for (auto& imageView : _ImageViews)
            vkDestroyImageView(device, imageView, nullptr);
        vkDestroySwapchainKHR(device, _Swapchain, nullptr);
    }

    VkSurfaceFormatKHR Swapchain::ChooseFormat(const std::vector<VkSurfaceFormatKHR> &formats)
    {
        for(const auto& format : formats)
        {
            if (format.format == VK_FORMAT_B8G8R8A8_SRGB &&
                format.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR)
                return format;
        }

        return formats[0];
    }

    // TODO: Prefer VK_PRESENT_MODE_FIFO_KHR on low energy devices
    VkPresentModeKHR Swapchain::ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes)
    {
        for(const auto& presentMode : presentModes)
        {
            if (presentMode == VK_PRESENT_MODE_MAILBOX_KHR)
                return presentMode;
        }

        return VK_PRESENT_MODE_FIFO_KHR;
    }

    VkExtent2D Swapchain::ChooseExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, Window* pWindow)
    {
        if (capabilities.currentExtent.width != std::numeric_limits<uint32_t>::max())
            return capabilities.currentExtent;

        int iWidth, iHeight;
        glfwGetFramebufferSize(pWindow->GetWindow(), &iWidth, &iHeight);

        VkExtent2D extent {
            .width = static_cast<uint32_t>(iWidth),
            .height = static_cast<uint32_t>(iHeight)
        };

        extent.width = std::clamp(extent.width, capabilities.minImageExtent.width, capabilities.maxImageExtent.width);
        extent.height = std::clamp(extent.height, capabilities.minImageExtent.height, capabilities.maxImageExtent.height);

        return extent;
    }

    void Swapchain::CreateImageViews()
    {
        _ImageViews.resize(_Images.size());
        for (int i = 0; i < _Images.size(); ++i)
        {
            VkImageViewCreateInfo createInfo{
                .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
                .image = _Images[i],
                .viewType = VK_IMAGE_VIEW_TYPE_2D,
                .format = _Format,
                .components = {.r = VK_COMPONENT_SWIZZLE_IDENTITY,
                               .g = VK_COMPONENT_SWIZZLE_IDENTITY,
                               .b = VK_COMPONENT_SWIZZLE_IDENTITY,
                               .a = VK_COMPONENT_SWIZZLE_IDENTITY},
                .subresourceRange = {
                        .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
                        .baseMipLevel = 0,
                        .levelCount = 1,
                        .baseArrayLayer = 0,
                        .layerCount = 1}
            };

            auto device = Graphics::GetDevice()->GetVulkanDevice();
            Graphics::CheckVk(vkCreateImageView(device, &createInfo, nullptr, &_ImageViews[i]));
        }
    }
}
