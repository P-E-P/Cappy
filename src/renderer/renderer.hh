#ifndef RENDERER_H
#define RENDERER_H

namespace cappy::renderer
{

struct Version
{
  int major;
  int minor;
  int patch;
};

class Renderer
{
public:
  Renderer () = default;
  Renderer (const Renderer &) = default;
  Renderer (Renderer &&) = delete;
  auto operator= (const Renderer &) -> Renderer & = default;
  auto operator= (Renderer &&) -> Renderer & = delete;
  virtual ~Renderer () = default;
};
}
#endif /* !RENDERER_H */
