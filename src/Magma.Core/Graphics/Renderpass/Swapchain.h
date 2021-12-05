//
// Created by Acer on 10.11.2021 г..
//

#ifndef MAGMA_SWAPCHAIN_H
#define MAGMA_SWAPCHAIN_H

#include "Magma.Core/Base.h"
#include "Magma.Core/Types.h"
#include "Magma.Core/Window.h"
#include "Magma.Core/Devices/Instance.h"
#include "Magma.Core/Devices/LogicalDevice.h"
#include "Magma.Core/Devices/Surface.h"

#include <vector>
#include <vulkan/vulkan.h>
#include <array>

namespace Magma
{
    struct SwapchainSupportDetails
    {
        VkSurfaceCapabilitiesKHR Capabilities;
        std::vector<VkSurfaceFormatKHR> Formats;
        std::vector<VkPresentModeKHR> PresentModes;
    };

    SwapchainSupportDetails QuerySwapchainSupport(VkPhysicalDevice device, VkSurfaceKHR surface);

    class MAGMA_API Swapchain
    {
    public:
        explicit Swapchain();
        ~Swapchain() noexcept;

        [[nodiscard]] const VkSwapchainKHR& GetSwapchain() const noexcept { return _Swapchain; }
        [[nodiscard]] const VkExtent2D& GetExtent2D() const noexcept { return _Extent; }
        [[nodiscard]] const VkFormat& GetFormat() const noexcept { return _Format; }
        [[nodiscard]] const std::vector<VkImageView>& GetImageViews() const noexcept { return _ImageViews; }
        [[nodiscard]] uint32_t GetImageCount() const noexcept { return _ImageViews.size(); }
    private:
        void CreateImageViews();
        static VkSurfaceFormatKHR ChooseFormat(const std::vector<VkSurfaceFormatKHR>& formats);
        static VkPresentModeKHR ChoosePresentMode(const std::vector<VkPresentModeKHR>& presentModes);
        static VkExtent2D ChooseExtent2D(const VkSurfaceCapabilitiesKHR& capabilities, Window* pWindow);
    private:
        VkSwapchainKHR _Swapchain;
        VkFormat _Format;
        VkExtent2D _Extent;

        std::vector<VkImage> _Images;
        std::vector<VkImageView> _ImageViews;
    };
}

#endif //MAGMA_SWAPCHAIN_H
