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
#include "resolve/resolve.hh"


int main(int argc, char** argv)
{
  // RESOLVE BY HAND
  std::cout << "Resolution begins\n";
  std::string str = "FRLRULDDLBFBURULBRFFULFFBLFDDUDDLBUUDBRDLUURRDFLBBBFRR";
  Resolution::Resolve resolve(str);
  resolve.find_solution();
  resolve.resolve_cube();
  std::cout << "Resolution ends\n";
  return 0;
  // RESOLVE BY HAND

  // 2D DEBUG
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Event event;

  detect::Displayer displayer("solvzer");
  auto& dis = display::Display::Instance(&window, &renderer);
  auto face = cube::Face(cube::Cube::solved_state_);
  state::State::Instance().face_set(face);
  dis.repaint();
  controller::start_controller(&event);
  return 0;
  // 2D DEBUG

  // CUBE DETECTION
  detect::Detector d(displayer, detect::CameraPosition::TOP);
  while (true)
  {
    d.update();
    displayer.display();
  }
  return 0;
  // CUBE DETECTION
}
