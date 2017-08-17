#include "controller.hh"
#include "cube/cube.hh"
#include "cube/move.hh"
#include "cube/search.hh"
#include "misc/display.hh"
#include "misc/state.hh"

namespace controller
{
  void
  scramble()
  {
    state::State::Instance().draw_ = false;
    auto& face = state::State::Instance().face_get();
    face.scramble();
    display::Display::Instance().toggle_enable("resolve", true);
    display::Display::Instance().toggle_enable("step_by_step", true);
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
     state.compute_time_set(0);
     display::Display::Instance().toggle_enable("resolve", false);
     display::Display::Instance().toggle_enable("step_by_step", false);
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
            const auto& check_button =
              display::Display::Instance().is_intersect(event->button.x,
                                                        event->button.y);
            if (check_button == "")
              break;
            else if (check_button.find("scramble") != std::string::npos)
              scramble();
            else if (check_button.find("resolve") != std::string::npos)
              resolve();
            break;
        }
      }
      SDL_Delay(5);
    }
  }
} // namespace controller
