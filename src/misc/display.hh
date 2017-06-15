#include <SDL2/SDL.h>

namespace display
{
  class Display
  {
    public:
      ~Display();
      void setup_background() const;
      void draw_rubiks(const std::string& rubiks) const;
      static const Display& Instance();
      static const Display& Instance(SDL_Window **window, SDL_Renderer **renderer);

    private:
      Display(SDL_Window **window, SDL_Renderer **renderer);
      void draw_square(const std::pair<int, int>& pos, const char& c) const;
      SDL_Window *window_;
      SDL_Renderer *renderer_;
     };
}
