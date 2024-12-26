#include <chrono>
#include <cstdlib>
#include <iostream>
#include <string_view>
#include <vector>
#include <vulkan/vulkan.hpp>
#include <vulkan/vulkan_structs.hpp>

#include "engine.hh"

using std::chrono::high_resolution_clock;

auto
main (int argc, char *argv[]) -> int
{
  const std::vector<std::string_view> args (argv, argv + argc);

  Engine engine ("Cappy engine");

  long frames = 0;
  double unprocessed_seconds = 0;
  auto previous_time = high_resolution_clock::now ();

  constexpr double SECONDS_PER_TICK = 1.0 / 60.0;
  constexpr int LOGGING_PERIOD = 60;

  int fixed_tick_count = 0;
  bool ticked = false;

  while (engine.is_running ())
    {
      auto current_time = high_resolution_clock::now ();
      auto passed_time = current_time - previous_time;
      previous_time = current_time;

      unprocessed_seconds
          += (double)passed_time.count () / high_resolution_clock::period::den;

      while (unprocessed_seconds > SECONDS_PER_TICK)
        {
          engine.fixed_tick ();
          unprocessed_seconds -= SECONDS_PER_TICK;
          ticked = true;
          fixed_tick_count++;

          if (fixed_tick_count % LOGGING_PERIOD == 0)
            {
              std::cout << "FPS: " << frames << "\n";
              frames = 0;
            }
        }

      if (ticked)
        {
          engine.render ();
          frames++;
        }

      engine.tick ();
    }

  return EXIT_SUCCESS;
}

auto
Engine::create_window (int width, int height,
                       const std::string &name) -> GLFWwindow *
{
  glfwInit ();

  glfwWindowHint (GLFW_CLIENT_API, GLFW_NO_API);
  glfwWindowHint (GLFW_RESIZABLE, GLFW_FALSE);

  return glfwCreateWindow (width, height, name.c_str (), nullptr, nullptr);
}

Engine::Engine (const std::string &name)
    : vkInstance (create_vulkan_instance (name)),
      window (create_window (WINDOW_WIDTH, WINDOW_HEIGHT, name))
{
  create_vulkan_instance (name);
}

Engine::~Engine ()
{
  glfwDestroyWindow (window);
  glfwTerminate ();
}

auto
Engine::fixed_tick () -> void
{
  if (glfwWindowShouldClose (window) != 0)
    {
      running = false;
    }
  level.fixed_tick ();
}

auto
Engine::tick () -> void
{
  level.tick ();
  glfwPollEvents ();
}

auto
Engine::render () -> void
{
  level.render ();
  glfwSwapBuffers (window);
}

auto
Engine::stop () -> void
{
  running = false;
}
