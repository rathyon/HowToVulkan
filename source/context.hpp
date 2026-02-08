#pragma once

#include <vulkan/vulkan_raii.hpp>

namespace ralkan
{
    class Context
    {
    public:
        Context();
        ~Context();

    private:
        void create_instance();
        void select_physical_device();
        void create_logical_device();
        
        vk::Instance m_instance;
        vk::PhysicalDevice m_physical_device;
        vk::Device m_device;
    };
}
