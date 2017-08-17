#pragma once
#include <map>
#include <stack>
#include <SDL2/SDL.h>
#include <SDL2/SDL_ttf.h>
#include <SDL2/SDL_image.h>

#include "misc/state.hh"
#include "misc/texture.hh"

namespace display
{
  class Display
  {
    public:
      ~Display();

      // Delete copy constructor and assignement constructor because we
      // are using Singleton Design Pattern
      Display(const Display& d) = delete;
      Display& operator =(const Display& d) = delete;

      // Get the Singleton
      static Display& Instance();
      static Display& Instance(SDL_Window **window, SDL_Renderer **renderer);

      // Draw things on the screen
      void draw_rubiks(const std::string& rubiks) const;
      void draw_text(const std::string& text, const std::array<uint8_t, 3> color,
                     int x, int y, int font_size);

      // Refresh the screen
      void refresh() const;

      // Re-draw all screen
      void repaint();

      // Get the current window size
      void window_size_get(std::pair<int, int>& res) const;

      // Enable/Disable a button on the screen
      void toggle_enable(const std::string& element, const bool enable);

      // Check for click in buttons
      std::string is_intersect(int x, int y) const;

      // Draw the background
      void setup_background() const;

    private:
      // Making default constructor private to ensure the Singleton
      Display(SDL_Window **window, SDL_Renderer **renderer);

      // Init the textures vector with all resources
      void init_textures();

      // Display a texture on the screen
      void load_texture(Texture& tex);

      // Update the screen with current config (ie: after enabling or disabling
      // buttons)
      void update_ui();

      // Wrapper to draw a square with its position and color
      void draw_square(const std::pair<int, int>& pos, const char& c) const;

      // Dynamically returns the rectangle the next button should be set to
      SDL_Rect get_rect(SDL_Texture *texture, unsigned int offset);

      // Set the program icon to our
      void set_window_icon() const;

    private:

      // SDL-mandatory variable to load and display things on the screen
      SDL_Window *window_;
      SDL_Renderer *renderer_;

      // Contain every textures the program is using
      std::vector<Texture> textures_;
  };

  // Static array of all resources user-interactionable
  static std::string resources_[] =
    {
      "resources/button_scramble.png",
      "resources/button_resolve.png",
      "resources/button_step_by_step.png",
      "resources/button_previous.png",
      "resources/button_next.png"
    };

  // Return the color given the face
  void get_color(std::array<int, 3>& color, const char& c);

  // Return the position of the square to be drawed given its index in the cube
  const std::pair<int, int> get_pos(const int& index);

} // namespace display
