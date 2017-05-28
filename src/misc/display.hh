#include <SDL2/SDL.h>

namespace display
{
  class Display
  {
    public:
      Display(SDL_Window *window, SDL_Renderer *renderer);
      ~Display();

    private:
      SDL_Window *window_;
      SDL_Renderer *renderer_;
  };

  Display& init_sdl();
}
