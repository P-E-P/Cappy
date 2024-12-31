#ifndef VULKAN_H
#define VULKAN_H

#include <string>
#include <vulkan/vulkan.hpp>

#include "renderer.hh"

namespace cappy::renderer::vulkan
{

#ifdef NDEBUG
constexpr bool ENABLE_VALIDATION_LAYERS = false;
#else
constexpr bool ENABLE_VALIDATION_LAYERS = true;
#endif

auto create_vulkan_instance (const std::string &name) -> vk::Instance;

auto check_validation_layer_support () -> bool;

auto get_required_extensions () -> std::vector<const char *>;

auto CreateDebugUtilsMessengerEXT (
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) -> VkResult;

class VulkanRenderer : public Renderer
{
  vk::Instance instance;

public:
  VulkanRenderer (const std::string &application_name, Version version);

  VulkanRenderer (const VulkanRenderer &) = default;
  VulkanRenderer (VulkanRenderer &&) = delete;
  auto operator= (const VulkanRenderer &) -> VulkanRenderer & = default;
  auto operator= (VulkanRenderer &&) -> VulkanRenderer & = delete;
  explicit VulkanRenderer (vk::Instance instance) : instance (instance) {}
  ~VulkanRenderer () override;
};

}

#endif
