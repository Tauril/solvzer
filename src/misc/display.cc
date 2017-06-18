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

  Display&
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

  Display&
  Display::Instance()
  {
    return Instance(nullptr, nullptr);
  }

  void
  Display::setup_ui_for(const std::string& file, int y_offset)
  {
    int sizes[4];
    SDL_Surface *image = SDL_LoadBMP(file.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, image);
    SDL_QueryTexture(texture, nullptr, nullptr, &sizes[0], &sizes[1]);
    SDL_GetWindowSize(window_, &sizes[2], &sizes[3]);
    SDL_Rect dstrect = {sizes[2] - sizes[0] - 50, 100 * y_offset, sizes[0], sizes[1]};
    SDL_RenderCopy(renderer_, texture, NULL, &dstrect);
    elements_[file] = dstrect;
  }

  void
  Display::setup_ui()
  {
    elements_.clear();
    setup_ui_for(scramble_button_, 1);
    setup_ui_for(resolve_button_, 2);
  }

  const std::string&
  Display::is_intersect(int x, int y) const
  {
    for (const auto& el : elements_)
    {
      auto& rect = el.second;
      if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
        return el.first;
    }
    return "";
  }

  void
  Display::setup_background() const
  {
      SDL_RenderClear(renderer_);
      static SDL_Surface *surf = SDL_LoadBMP("resources/background.bmp");
      static SDL_Texture *background = SDL_CreateTextureFromSurface(renderer_, surf);
      SDL_RenderCopy(renderer_, background, nullptr, nullptr);
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
      // White
      case 'U':
        color[0] = 255;
        color[1] = 255;
        color[2] = 255;
        break;
      // Blue
      case 'R':
        color[0] = 0;
        color[1] = 0;
        color[2] = 255;
        break;
      // Yellow
      case 'D':
        color[0] = 255;
        color[1] = 255;
        color[2] = 0;
        break;
      // Red
      case 'F':
        color[0] = 255;
        color[1] = 0;
        color[2] = 0;
        break;
      // Green
      case 'L':
        color[0] = 0;
        color[1] = 255;
        color[2] = 0;
        break;
      // Orange
      case 'B':
        color[0] = 255;
        color[1] = 165;
        color[2] = 0;
        break;
    }
  }

  void
  Display::draw_text(const std::string& text, const std::array<uint8_t, 3> color, int x, int y)
  {
    TTF_Font* Sans = TTF_OpenFont("Sans.ttf", 24);

    SDL_Color sdl_color = {color[0], color[1], color[2]};

    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), sdl_color);

    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer_, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = 100;
    Message_rect.h = 100;

    SDL_RenderCopy(renderer_, Message, NULL, &Message_rect);
  }

  void
  Display::draw_rubiks(const std::string& rubiks) const
  {
      int index = 0;
      for (char c : rubiks)
        draw_square(get_pos(index++), c);
      SDL_Delay(500);
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

  void
  Display::refresh() const
  {
    SDL_RenderPresent(renderer_);
  }

  void
  Display::repaint()
  {
    setup_background();
    draw_rubiks(state::State::Instance().face_str_get());
    setup_ui();
    refresh();
  }
}
