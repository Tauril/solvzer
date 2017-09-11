#include <cassert>
#include <iostream>
#include <cube/cube.hh>
#include <cube/search.hh>
#include <detect/detector.hh>
#include <misc/display.hh>
#include <misc/controller.hh>
#include <misc/state.hh>

// temp
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "detect/displayer.hh"

detect::Displayer displayer("solvzer");

int main(int argc, char** argv)
{
  /* 2D DEBUG */
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Event event;

  auto& dis = display::Display::Instance(&window, &renderer);
  auto face = cube::Face(cube::Cube::solved_state_);
  state::State::Instance().face_set(face);
  dis.repaint();
  controller::start_controller(&event);
  return 0;
  /* 2D DEBUG */

  /* CUBE DETECTION */
  detect::Detector d(displayer, detect::CameraPosition::TOP);
  while (true)
  {
    d.update();
    displayer.display();
  }
  /* CUBE DETECTION */

  return 0;
}
