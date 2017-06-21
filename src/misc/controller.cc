#include "cube/cube.hh"
#include "cube/move.hh"
#include "cube/search.hh"
#include "misc/display.hh"
#include "misc/state.hh"
#include "controller.hh"

namespace controller
{
  void
  scramble()
  {
    state::State::Instance().draw_ = false;
    auto& face = state::State::Instance().face_get();
    face.scramble();
    display::Display::Instance().repaint();
  }

  void
  resolve()
  {
     auto& state = state::State::Instance();
     state.draw_ = true;
     auto sol = state.search_get().solution(state.face_get(), cube::Search::DEPTH);
     cube::move::make_moves(state.face_str_get(), sol);
     state.face_str_set(cube::Cube::solved_state_);
     state.clear_vector();
     display::Display::Instance().repaint();
  }


  void
  start_controller(SDL_Event *event)
  {
    bool done = false;
    while (!done)
    {
      if(SDL_PollEvent(event))
      {
        switch(event->type)
        {
          case SDL_QUIT:
            done = true;
            break;
          case SDL_WINDOWEVENT:
            if(event->window.event == SDL_WINDOWEVENT_RESIZED)
              display::Display::Instance().repaint();
            break;
          case SDL_MOUSEBUTTONUP:
            auto& check_button = display::Display::Instance().is_intersect(event->button.x, event->button.y);
            if (check_button == display::scramble_button_)
              scramble();
            if (check_button == display::resolve_button_)
              resolve();
            break;
        }
      }
      SDL_Delay(5);
    }
  }
}
