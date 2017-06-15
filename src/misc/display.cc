#include <array>
#include <iostream>
#include <utility>

#include "display.hh"

#define SQUARE_SIZE 50
#define BORDER_SIZE 5
#define SIDE_SIZE SQUARE_SIZE * 3 + BORDER_SIZE * 3

namespace display
{
  Display::Display(SDL_Window **window, SDL_Renderer **renderer)
    : window_(*window)
    , renderer_(*renderer)
  {}

  Display::~Display()
  {
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
  }

  const Display&
  Display::Instance(SDL_Window **window, SDL_Renderer **renderer)
  {
    static bool isInit = false;
    if (!isInit)
    {
      isInit = true;
      *window = SDL_CreateWindow("SolvZer", SDL_WINDOWPOS_CENTERED,
          SDL_WINDOWPOS_CENTERED,
          1280, 680, SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
      *renderer =  SDL_CreateRenderer(*window, -1, SDL_RENDERER_ACCELERATED);
    }
    static Display d(window, renderer);
    return d;
  }

  const Display&
  Display::Instance()
  {
    return Instance(nullptr, nullptr);
  }

  void
  Display::setup_background() const
  {
    std::cout << "rend " << &renderer_ << std::endl;
      // Set the background
      static SDL_Surface *Loading_Surf = SDL_LoadBMP("resources/background.bmp");
      static SDL_Texture *Background_Tx = SDL_CreateTextureFromSurface(renderer_,
          Loading_Surf);
      SDL_RenderCopy(renderer_, Background_Tx, NULL, NULL);
      //SDL_RenderPresent(renderer_);

      // Freeing stuff
      //SDL_FreeSurface(Loading_Surf);
  }

  inline
  int
  get_generic_y_pos(const int& index, const int offset)
  {
    return (offset * SIDE_SIZE + ((index / 3) % 3) * (SQUARE_SIZE + \
            BORDER_SIZE)) + (BORDER_SIZE * 2) + BORDER_SIZE * 3 * offset;
  }

  inline
  int
  get_generic_x_pos(const int& index, const int offset)
  {
    return (offset * SIDE_SIZE + (index % 3) * (SQUARE_SIZE + BORDER_SIZE)) + \
           (BORDER_SIZE * 2) + BORDER_SIZE * 3 * offset;
  }

  const std::pair<int, int>
  get_pos(const int& index)
  {
    // Up face
    if (index >= 0 && index <= 8)
      return std::pair<int, int>(get_generic_x_pos(index, 1), \
          get_generic_y_pos(index, 0));
    // Right face
    else if (index >= 9 && index <= 17)
      return std::pair<int, int>(get_generic_x_pos(index, 2), \
          get_generic_y_pos(index, 1));
    // Front face
    else if (index >= 18 && index <= 26)
      return std::pair<int, int>(get_generic_x_pos(index, 1), \
          get_generic_y_pos(index, 1));
    // Down face
    else if (index >= 27 && index <= 35)
      return std::pair<int, int>(get_generic_x_pos(index, 1), \
          get_generic_y_pos(index, 2));
    // Left face
    else if (index >= 36 && index <= 44)
      return std::pair<int, int>(get_generic_x_pos(index, 0), \
          get_generic_y_pos(index, 1));
     // Back face
     else
      return std::pair<int, int>(get_generic_x_pos(index, 3), \
          get_generic_y_pos(index, 1));
  }
  void
  get_color(std::array<int, 3>& color, const char& c)
  {
    switch (c)
    {
      case 'U':
        color[0] = 255;
        color[1] = 0;
        color[2] = 0;
        break;
      case 'R':
        color[0] = 0;
        color[1] = 255;
        color[2] = 0;
        break;
      case 'F':
        color[0] = 255;
        color[1] = 255;
        color[2] = 0;
        break;
      case 'D':
        color[0] = 255;
        color[1] = 128;
        color[2] = 0;
        break;
      case 'L':
        color[0] = 255;
        color[1] = 255;
        color[2] = 255;
        break;
      case 'B':
        color[0] = 0;
        color[1] = 0;
        color[2] = 255;
        break;
    }
  }

  void
  Display::draw_rubiks(const std::string& rubiks) const
  {
      std::cout << &renderer_ << std::endl;
      SDL_RenderClear(renderer_);
      setup_background();
      int index = 0;
      std::cout << "HERE" << std::endl;
      for (char c : rubiks)
        draw_square(get_pos(index++), c);
      SDL_RenderPresent(renderer_);
      SDL_Delay(2000);
  }

  void
  Display::draw_square(const std::pair<int, int>& pos, const char& c) const
  {
      SDL_Rect r;
      r.x = pos.first;
      r.y = pos.second;
      r.w = SQUARE_SIZE;
      r.h = SQUARE_SIZE;
      std::array<int, 3> color;
      get_color(color, c);
      SDL_SetRenderDrawColor(renderer_, color[0], color[1], color[2], 255);
      SDL_RenderFillRect(renderer_, &r);
  }
}
