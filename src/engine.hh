#ifndef ENGINE_H
#define ENGINE_H

#include <string>
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "level.hh"
#include "renderer/renderer.hh"

namespace cappy
{

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
  renderer::Renderer &renderer;
  GLFWwindow *window;
  bool running = true;
  Level level;

  static auto create_window (int width, int height,
                             const std::string &name) -> GLFWwindow *;

public:
  auto operator= (const Engine &) -> Engine & = delete;
  auto operator= (Engine &&) -> Engine & = delete;
  Engine (const Engine &) = delete;
  Engine (Engine &&) = delete;
  explicit Engine (const std::string &name, renderer::Renderer &renderer);

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
}

#endif /* !ENGINE_H */
