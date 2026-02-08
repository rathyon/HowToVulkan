#include "context.hpp"

#include <iostream>

#define VULKAN_HPP_DISPATCH_LOADER_DYNAMIC 1  // NOLINT(clang-diagnostic-unused-macros)
#include <vulkan/vulkan.hpp>

VULKAN_HPP_DEFAULT_DISPATCH_LOADER_DYNAMIC_STORAGE;

#include <SDL3/SDL.h>
#include <SDL3/SDL_vulkan.h>

// todo define VULKAN_HPP_NO_EXCEPTIONS to remove exception throwing by VulkanHPP

namespace ralkan
{
    Context::Context()
    {
        
        // Ugly but SDL library is irrelevant for the refactor
        if (SDL_Init(SDL_INIT_VIDEO) == false)
        {
            throw std::runtime_error("SDL Library failed to load");
        }

        if (SDL_Vulkan_LoadLibrary(NULL) == false)
        {
            throw std::runtime_error("SDL Library failed to load");
        }

        //todo Set default dispatcher for VulkanHPP (define VK_NO_PROTOTYPES)
        
        VULKAN_HPP_DEFAULT_DISPATCHER.init();
        create_instance();
        select_physical_device();
        create_logical_device();
    }

    Context::~Context()
    {
        m_instance.destroy();
    }

    void Context::create_instance()
    {
        // ** BEFORE **
        // VkApplicationInfo appInfo{ .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO, .pApplicationName = "How to Vulkan", .apiVersion = VK_API_VERSION_1_3 };
        // uint32_t instanceExtensionsCount{ 0 };
        // char const* const* instanceExtensions{ SDL_Vulkan_GetInstanceExtensions(&instanceExtensionsCount) };
        // VkInstanceCreateInfo instanceCI{
        //     .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        //     .pApplicationInfo = &appInfo,
        //     .enabledExtensionCount = instanceExtensionsCount,
        //     .ppEnabledExtensionNames = instanceExtensions,
        // };
        // chk(vkCreateInstance(&instanceCI, nullptr, &instance));

        // ** AFTER **+
        vk::ApplicationInfo app_info{
            "How to Vulkan HPP",
            vk::makeVersion(1, 0, 0),
            "No engine",
            vk::makeVersion(1, 0, 0),
            vk::ApiVersion13,
            nullptr
        };

        // Acquire mandatory extensions for SDL3
        uint32_t instance_extensions_count{0};
        char const* const* instance_extensions{SDL_Vulkan_GetInstanceExtensions(&instance_extensions_count)};

        // Create the application
        vk::InstanceCreateInfo instance_create_info{
            {}, 
            &app_info, 
            0, nullptr, 
            instance_extensions_count, instance_extensions
        };

        m_instance = vk::createInstance(instance_create_info);
        
        VULKAN_HPP_DEFAULT_DISPATCHER.init(m_instance);
    }

    void Context::select_physical_device()
    {
        // ** BEFORE **
        // uint32_t deviceCount{ 0 };
        // chk(vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr));
        // std::vector<VkPhysicalDevice> devices(deviceCount);
        // chk(vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data()));
        // uint32_t deviceIndex{ 0 };
        // if (argc > 1) {
        //     deviceIndex = std::stoi(argv[1]);
        //     assert(deviceIndex < deviceCount);
        // }
        // VkPhysicalDeviceProperties2 deviceProperties{ .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2 };
        // vkGetPhysicalDeviceProperties2(devices[deviceIndex], &deviceProperties);
        // std::cout << "Selected device: " << deviceProperties.properties.deviceName << "\n";

        // ** AFTER **+
        const std::vector<vk::PhysicalDevice> physical_devices = m_instance.enumeratePhysicalDevices();

        // Listing available GPUs
        std::cout << "Available devices:" << '\n';
        for (const vk::PhysicalDevice& physical_device : physical_devices)
        {
            const auto& stuff = physical_device.getProperties2();
            std::cout << stuff.properties.deviceName << '\n';
        }
        
        // Select the first one (as per the tutorial)
        m_physical_device = physical_devices[0];
    }

    void Context::create_logical_device()
    {
        // VULKAN_HPP_DEFAULT_DISPATCHER.init(m_device);
    }
}
