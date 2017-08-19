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
  {
    set_window_icon();
    init_textures();
    toggle_enable("scramble", true);
  }

  Display::~Display()
  {
    SDL_DestroyWindow(window_);
    SDL_DestroyRenderer(renderer_);
    TTF_Quit();
    SDL_Quit();
    IMG_Quit();
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
      TTF_Init();
      IMG_Init(IMG_INIT_PNG|IMG_INIT_JPG);
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
  Display::draw_text(const std::string& text, const std::array<uint8_t, 3> color,
                     int x, int y, int font_size)
  {
    TTF_Font* Sans = TTF_OpenFont("resources/Arial-Rounded-Bold.ttf", font_size);
    SDL_Color sdl_color = {color[0], color[1], color[2], 1};
    SDL_Surface* surfaceMessage = TTF_RenderText_Solid(Sans, text.c_str(), sdl_color);
    SDL_Texture* Message = SDL_CreateTextureFromSurface(renderer_, surfaceMessage);

    SDL_Rect Message_rect;
    Message_rect.x = x;
    Message_rect.y = y;
    Message_rect.w = surfaceMessage->w;
    Message_rect.h = surfaceMessage->h;

    SDL_RenderCopy(renderer_, Message, NULL, &Message_rect);
    TTF_CloseFont(Sans);
    SDL_DestroyTexture(Message);
    SDL_FreeSurface(surfaceMessage);
  }

  void
  Display::draw_rubiks(const std::string& rubiks) const
  {
      int index = 0;
      for (char c : rubiks)
        draw_square(get_pos(index++), c);
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
    update_ui();
    state::State::Instance().draw_text_data();
    refresh();
  }

  void
  Display::window_size_get(std::pair<int, int>& res) const
  {
    SDL_GetWindowSize(window_, &res.first, &res.second);
  }

  void
  Display::toggle_enable(const std::string& element, const bool enable)
  {
    for (auto& texture : textures_)
      if (texture.path_.find(element) != std::string::npos)
        texture.enable_ = enable;
  }

  std::string
  Display::is_intersect(int x, int y) const
  {
    for (const auto& el : textures_)
    {
      auto& rect = el.dstrect_;
      if (x >= rect.x && x <= rect.x + rect.w && y >= rect.y && y <= rect.y + rect.h)
        return el.path_;
    }
    return "";
  }

  void
  Display::setup_background() const
  {
      SDL_RenderClear(renderer_);
      static SDL_Surface *surf = IMG_Load("resources/background.jpg");
      SDL_Texture *background = SDL_CreateTextureFromSurface(renderer_, surf);
      SDL_RenderCopy(renderer_, background, nullptr, nullptr);
      SDL_DestroyTexture(background);
  }

  void
  Display::init_textures()
  {
    for (const auto path : resources_)
      textures_.push_back(Texture(path));
  }

  void
  Display::load_texture(Texture& tex)
  {
    SDL_Surface *image = IMG_Load(tex.path_.c_str());
    SDL_Texture *texture = SDL_CreateTextureFromSurface(renderer_, image);
    auto rect = get_rect(texture, Texture::current_offset_++);
    tex.dstrect_ = rect;
    SDL_RenderCopy(renderer_, texture, nullptr, &rect);
    SDL_FreeSurface(image);
    SDL_DestroyTexture(texture);
  }

  void
  Display::update_ui()
  {
    Texture::current_offset_ = 1;
    for (auto& texture : textures_)
      if (texture.enable_)
        load_texture(texture);
      else
        texture.dstrect_ = {0, 0, 0, 0};
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

  SDL_Rect
  Display::get_rect(SDL_Texture *texture, unsigned int offset)
  {
    int sizes[4];
    SDL_QueryTexture(texture, nullptr, nullptr, &sizes[0], &sizes[1]);
    SDL_GetWindowSize(window_, &sizes[2], &sizes[3]);
    SDL_Rect dstrect = {sizes[2] - sizes[0] - 50, 100 * (int)offset, sizes[0], sizes[1]};
    return dstrect;
  }

  void
  Display::set_window_icon() const
  {
    SDL_Surface *icon = IMG_Load("resources/icon.png");
    SDL_SetWindowIcon(window_, icon);
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
} // namespace display
