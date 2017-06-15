#include <assert.h>
#include <iostream>
#include <utility>

#include "display.hh"

#define SQUARE_SIZE 50
#define BORDER_SIZE 5

namespace display
{
  Display::Display(SDL_Window *window, SDL_Renderer *renderer)
  : window_(window)
  , renderer_(renderer)
  {}

  Display::~Display()
  {
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    SDL_Quit();
  }

  void
  Display::setup_background() const
  {
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
    return (((index / 3) * SQUARE_SIZE + (index / 3) * BORDER_SIZE) * offset) + (BORDER_SIZE * 2);
  }

  inline
  int
  get_generic_x_pos(const int& index, const int offset)
  {
    return ((3 * SQUARE_SIZE + 3 * BORDER_SIZE + (index % 3) * SQUARE_SIZE + \
          (index % 3) * BORDER_SIZE) * offset) + (BORDER_SIZE * 2);
  }

  const std::pair<int, int>
  get_pos(const int& index)
  {
    // Up face
    if (index >= 0 && index <= 8)
    {
      return std::pair<int, int>(get_generic_x_pos(index, 2), get_generic_y_pos(index, 2));
    }
    else
      return std::pair<int, int>(500, 500);
  }

  void
  Display::draw_rubiks(const std::string& rubiks) const
  {
    SDL_RenderClear(renderer_);
    setup_background();
    int index = 0;
    for (char c : rubiks)
    {
      auto res = get_pos(index++);
      draw_square(res, c);
    }
    SDL_RenderPresent(renderer_);
  }

  void
  Display::draw_square(const std::pair<int, int>& pos, const char& c) const
  {
    SDL_Rect r;
    r.x = pos.first;
    r.y = pos.second;
    r.w = SQUARE_SIZE;
    r.h = SQUARE_SIZE;
    SDL_SetRenderDrawColor(renderer_, 0, 0, 200, 255);
    SDL_RenderFillRect(renderer_, &r);
  }
}
