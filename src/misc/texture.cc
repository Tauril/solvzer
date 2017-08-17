#include "display.hh"
#include "texture.hh"

namespace display
{
  Texture::Texture(const std::string& path, SDL_Rect dstrect, bool enable)
  : path_(path)
  , dstrect_(dstrect)
  , enable_(enable)
  {}

  unsigned int Texture::current_offset_ = 1;

  Texture::~Texture()
  {}
}
