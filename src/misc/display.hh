#pragma once
#include <map>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>

#include "misc/state.hh"

namespace display
{
  class Display
  {
    public:
      ~Display();
      Display(const Display& d) = delete;
      Display& operator =(const Display& d) = delete;
      void setup_background() const;
      void setup_ui();
      void draw_rubiks(const std::string& rubiks) const;
      const std::string& is_intersect(int x, int y) const;
      void draw_text(const std::string& text, const std::array<uint8_t, 3> color, int x, int y, int font_size);
      void refresh() const;
      void repaint();
      void window_size_get(std::pair<int, int>& res) const;
      static Display& Instance();
      static Display& Instance(SDL_Window **window, SDL_Renderer **renderer);
      std::map<std::string, SDL_Rect> elements_;


    private:
      Display(SDL_Window **window, SDL_Renderer **renderer);
      void draw_square(const std::pair<int, int>& pos, const char& c) const;
      void setup_ui_for(const std::string& file, int y_offset);
      SDL_Window *window_;
      SDL_Renderer *renderer_;
  };
  static constexpr char scramble_button_[] = "resources/button_scramble.bmp";
  static constexpr char resolve_button_[] = "resources/button_resolve.bmp";
}
