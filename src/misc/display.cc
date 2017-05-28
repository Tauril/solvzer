#include <utility>
#include "display.hh"

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

  Display&
  init_sdl()
  {
    // Create the main window
    SDL_Window *window = SDL_CreateWindow("SolvZer", SDL_WINDOWPOS_CENTERED,
                              SDL_WINDOWPOS_CENTERED,
                              1280, 680, SDL_WINDOW_SHOWN);
    SDL_Renderer *renderer =  SDL_CreateRenderer(window, -1,
                                                 SDL_RENDERER_ACCELERATED);

    // Will be use for event catching
    SDL_Event evenements;

    // Set the background
    SDL_Surface *Loading_Surf = SDL_LoadBMP("resources/background.bmp");
    SDL_Texture *Background_Tx = SDL_CreateTextureFromSurface(renderer,
                                                              Loading_Surf);
    SDL_RenderCopy(renderer, Background_Tx, NULL, NULL);

    // Freeing stuff
    SDL_FreeSurface(Loading_Surf);

    Display d(window, renderer);
    //return std::move(d);
    return d;
  }
}

/*int main(int argc, char **argv)
{
  SDL_Rect r;
 r.x = 50;
 r.y = 50;
 r.w = 50;
 r.h = 50;
 SDL_SetRenderDrawColor( renderer, 0, 0, 255, 255 );
 SDL_RenderFillRect( renderer, &r );
 SDL_RenderPresent(renderer);
 while(true)
 {
   SDL_WaitEvent(&evenements);

   if(evenements.window.event == SDL_WINDOWEVENT_CLOSE)
     break;
 }
  return 0;
}*/
