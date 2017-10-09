#include <cassert>
#include <iostream>
#include <cube/cube.hh>
#include <cube/search.hh>
#include <detect/detector.hh>
#include <detect/cube_detector.hh>
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
  /*
  // RESOLVE BY HAND
  std::cout << "Resolution begins\n";
  std::string str = "FRLRULDDLBFBURULBRFFULFFBLFDDUDDLBUUDBRDLUURRDFLBBBFRR";
  Resolution::Resolve resolve(str);
  resolve.find_solution();
  resolve.resolve_cube();
  std::cout << "Resolution ends\n";
  return 0;
  // RESOLVE BY HAND
  */

  // 2D DEBUG
  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Event event;

  //detect::Displayer displayer("solvzer");
  auto& dis = display::Display::Instance(&window, &renderer);
  auto face = cube::Face(cube::Cube::solved_state_);
  state::State::Instance().face_set(face);
  dis.repaint();
  controller::start_controller(&event);
  return 0;
  // 2D DEBUG

  /* CUBE DETECTION DEBUG (camera par camera) */
  /*
  detect::Displayer displayer("solvzer");

  if (argc < 2)
  {
    std::cout << "usage: ./solvzer [-t | -b] [channel] for top or bottom camera" << std::endl;
    return 1;
  }
  detect::Detector d(displayer, argv[1][1] == 't' ? detect::CameraPosition::TOP
                                                  : detect::CameraPosition::BOTTOM,
                                argv[2][0] - '0');
  while (true)
  {
    d.update();
    displayer.display();
  }
  return 0;
  */

  /* CUBE DETECTION */
  /*
  detect::CubeDetector detector;
  std::string state = detector.detect_cube();
  std::cout << "state: " << state << std::endl;
  return 0;
  */
}
