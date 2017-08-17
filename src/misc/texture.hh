#pragma once
#include <string>
#include <SDL2/SDL.h>

namespace display
{
  // Wrapper to manage on-screen textures easily
  class Texture
  {
    public:
      Texture(const std::string& path, SDL_Rect dstrect = {0, 0, 0, 0},
              bool enable = false);
      ~Texture();

    public:
      const std::string path_;
      SDL_Rect dstrect_;
      bool enable_;
      static unsigned int current_offset_;
  };
} // namespace display
