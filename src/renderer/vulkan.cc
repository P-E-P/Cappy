#include <iostream>
#include <stdexcept>
#include <vulkan/vk_platform.h>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#include <vulkan/vulkan_structs.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "vulkan.hh"

namespace cappy::renderer::vulkan
{

const std::vector<const char *> VALIDATION_LAYERS
    = { "VK_LAYER_KHRONOS_validation" };

auto
check_validation_layer_support () -> bool
{

  auto properties = vk::enumerateInstanceLayerProperties ();

  for (const auto *layer_name : VALIDATION_LAYERS)
    {
      bool found = false;
      for (auto &property : properties)
        {
          if (strcmp (property.layerName, layer_name) == 0)
            {
              found = true;
            }
        }

      if (!found)
        {
          return false;
        }
    }
  return true;
}

auto
create_vulkan_instance (const std::string &application_name,
                        Version version) -> vk::Instance
{

  if (ENABLE_VALIDATION_LAYERS && !check_validation_layer_support ())
    {
      throw std::runtime_error ("Validation layers unavailable");
    }

  vk::ApplicationInfo infos{
    application_name.c_str (),
    vk::makeApiVersion (0, version.major, version.minor, version.patch),
    "Cappy",
    vk::makeApiVersion (0, version.major, version.minor, version.patch),
    vk::ApiVersion10
  };

  vk::InstanceCreateInfo instanceCreateInfo ({}, &infos);

  vk::InstanceCreateInfo create_infos;
  create_infos.setPApplicationInfo (&infos);

  create_infos.setFlags (vk::InstanceCreateFlags{
      VK_INSTANCE_CREATE_ENUMERATE_PORTABILITY_BIT_KHR });

  auto extensions = get_required_extensions ();
  create_infos.enabledExtensionCount = (uint32_t)extensions.size ();
  create_infos.ppEnabledExtensionNames = extensions.data ();

  if (ENABLE_VALIDATION_LAYERS)
    {
      create_infos.enabledLayerCount = VALIDATION_LAYERS.size ();
      create_infos.ppEnabledLayerNames = VALIDATION_LAYERS.data ();
    }
  else
    {
      create_infos.enabledLayerCount = 0;
    }

  vk::Instance instance = vk::createInstance (create_infos);

  // if (CreateDebugUtilsMessengerEXT (instance, create_infos, nullptr, ))

  return instance;
}

static VKAPI_ATTR auto VKAPI_CALL
debug_callback (VkDebugUtilsMessageSeverityFlagBitsEXT message_severity,
                unsigned int message_type,
                const VkDebugUtilsMessengerCallbackDataEXT *callback_data,
                void *user_data) -> VkBool32
{

  std::ignore = user_data;
  std::ignore = message_type;

  if (message_severity >= VK_DEBUG_UTILS_MESSAGE_SEVERITY_WARNING_BIT_EXT)
    {
    }

  std::cerr << "Validation layer: " << callback_data->pMessage << "\n";
  return VK_FALSE;
}

void
setup_debug_messenger ()
{
  if (!ENABLE_VALIDATION_LAYERS)
    {
      return;
    }

  vk::DebugUtilsMessengerCreateInfoEXT create_infos{
    {},
    vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning
        | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError,
    vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral
        | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance
        | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation,
    debug_callback,
    {}
  };
}

auto
CreateDebugUtilsMessengerEXT (
    VkInstance instance, const VkDebugUtilsMessengerCreateInfoEXT *pCreateInfo,
    const VkAllocationCallbacks *pAllocator,
    VkDebugUtilsMessengerEXT *pDebugMessenger) -> VkResult
{
  auto func = (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr (
      instance, "vkCreateDebugUtilsMessengerEXT");

  if (func != nullptr)
    {
      return func (instance, pCreateInfo, pAllocator, pDebugMessenger);
    }
  return VK_ERROR_EXTENSION_NOT_PRESENT;
}

auto
get_required_extensions () -> std::vector<const char *>
{
  uint32_t glfw_extension_count = 0;
  const char **glfw_extensions = nullptr;
  glfw_extensions = glfwGetRequiredInstanceExtensions (&glfw_extension_count);

  std::vector<const char *> extensions (
      glfw_extensions, glfw_extensions + glfw_extension_count);

  extensions.emplace_back (VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

  if (ENABLE_VALIDATION_LAYERS)
    {
      extensions.push_back (VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    }

  return extensions;
}

VulkanRenderer::VulkanRenderer (const std::string &name, Version version)
    : instance (create_vulkan_instance (name, version))
{
}

VulkanRenderer::~VulkanRenderer () { instance.destroy (); }

}
