#include "cube/cube.hh"
#include "cube/search.hh"
#include "misc/display.hh"
#include "controller.hh"

namespace controller
{
  cube::Search search;
  cube::Face face;

  void
  scramble()
  {
    #undef DRAW
    face = cube::Face(cube::Cube::solved_state_);
    face.scramble();
    #define DRAW
    auto& dis = display::Display::Instance();
    dis.setup_background();
    dis.draw_rubiks(face.face_str_get());
    dis.setup_ui();
    dis.refresh();
  }

  void
  resolve()
  {
     //auto sol = search.solution(face, DEPTH);
     //assert(search.ack_solution(face, sol));
  }

  void
  start_controller(SDL_Event *event)
  {
    bool done = false;
    while (!done)
    {
      if(SDL_PollEvent(event))
      {
        if (event->type != SDL_MOUSEMOTION)
        {
          switch(event->type)
          {
            case SDL_QUIT:
              done = true;
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
      }
    }
  }
}
