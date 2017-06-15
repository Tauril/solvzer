#include <SDL2/SDL.h>

namespace display
{
  class Display
  {
    public:
      Display(SDL_Window *window, SDL_Renderer *renderer);
      ~Display();
      void setup_background() const;
      void draw_rubiks(const std::string& rubiks) const;
      void draw_square(const std::pair<int, int>& pos, const char& c) const;

    private:
      SDL_Window *window_;
      SDL_Renderer *renderer_;
  };
}
