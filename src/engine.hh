#include <vulkan/vulkan_core.h>
#include <vulkan/vulkan_enums.hpp>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vulkan/vulkan.hpp>

#include "level.hh"

constexpr auto WINDOW_WIDTH = 800;
constexpr auto WINDOW_HEIGHT = 600;

namespace version
{
constexpr auto MAJOR = 0;
constexpr auto MINOR = 1;
constexpr auto PATCH = 0;
}

class Engine
{
  vk::Instance vkInstance;
  GLFWwindow *window;
  bool running = true;
  Level level;

  static auto create_window (int width, int height,
                             const std::string &name) -> GLFWwindow *;

  static auto
  create_vulkan_instance (const std::string &name) -> vk::Instance
  {
    vk::ApplicationInfo infos{
      name.c_str (),
      vk::makeApiVersion (0, version::MAJOR, version::MINOR, version::PATCH),
      "Cappy",
      vk::makeApiVersion (0, version::MAJOR, version::MINOR, version::PATCH),
      vk::ApiVersion10
    };

    vk::InstanceCreateInfo instanceCreateInfo ({}, &infos);

    vk::InstanceCreateInfo create_infos;
    create_infos.setPApplicationInfo (&infos);

    uint32_t glfw_extension_count = 0;
    const char **glfw_extensions
        = glfwGetRequiredInstanceExtensions (&glfw_extension_count);

    create_infos.enabledLayerCount = 0;

    std::vector<const char *> required_extensions;

    for (uint32_t i = 0; i < glfw_extension_count; i++)
      {
        required_extensions.emplace_back (glfw_extensions[i]);
      }

    required_extensions.emplace_back (
        VK_KHR_PORTABILITY_ENUMERATION_EXTENSION_NAME);

    vk::InstanceCreateFlags flag;
    create_infos.setFlags (flag);

    create_infos.enabledExtensionCount = (uint32_t)required_extensions.size ();
    create_infos.ppEnabledExtensionNames = required_extensions.data ();

    vk::Instance instance = vk::createInstance (create_infos);
    return instance;
  }

public:
  explicit Engine (const std::string &name);

  ~Engine ();

  [[nodiscard]]
  auto
  is_running () const -> bool
  {
    return running;
  }

  auto fixed_tick () -> void;

  auto tick () -> void;

  auto render () -> void;

  auto stop () -> void;
};
