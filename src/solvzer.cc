#include <cassert>
#include <iostream>
#include <cube/cube.hh>
#include <cube/search.hh>
#include <detect/detector.hh>
#include <detect/cube_detector.hh>
#include <misc/display.hh>
#include <misc/controller.hh>
#include <misc/state.hh>
#include <iostream>
#include <thread>
#include <future>

// temp
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <opencv2/opencv.hpp>
#include "detect/displayer.hh"
#include "resolve/resolve.hh"


namespace
{

  void check_camera(detect::Displayer& displayer, short count)
  {
    using namespace std::literals;

    auto f = std::async(std::launch::async,
                        []{
                            while (std::cin.get() != '\n');
                            return 0;
                          });

    detect::Detector d(displayer,
                       count < 2 ? detect::CameraPosition::TOP
                                 : detect::CameraPosition::BOTTOM,
                       count);

    do {
      d.update();
      displayer.display();
    } while (f.wait_for(1s) != std::future_status::ready);
  }

}

int main(int argc, char** argv)
{

#if 0
  // RESOLVE BY HAND
  std::cout << "Resolution begins\n";
  std::string str = "FRLRULDDLBFBURULBRFFULFFBLFDDUDDLBUUDBRDLUURRDFLBBBFRR";
  Resolution::Resolve resolve(str);
  resolve.resolve_cube();
  std::cout << "Resolution ends\n";
  return 0;
  // RESOLVE BY HAND
#endif

  detect::Displayer displayer("solvZer");
  for (short count = 0; count < 4; count++)
    check_camera(displayer, count);
  displayer.destroy();

  SDL_Window* window = nullptr;
  SDL_Renderer* renderer = nullptr;
  SDL_Event event;

  detect::CubeDetector detector;

  // While we detect wrong colors, keep trying.
  bool second_run = false;

  std::string state;

  while (true)
  {
    try {
      state = detector.detect_cube(second_run);
      auto& dis = display::Display::Instance(&window, &renderer);
      auto face = cube::Face(state); // throw if wrong number of colors
      auto cube = cube::Cube(face);
      cube.verify(); // throw execption if not a regulat cube
      state::State::Instance().face_set(face);
      dis.repaint();
      break;
    } catch (const std::exception& e) {
      if (!second_run)
          second_run = true;
      else
      {
          std::cerr << "Could not detect properly colors." << std::endl;
          std::cout << e.what() << std::endl;
          std::cout << "Actual state " << state << std::endl;
          detect::last_chance_detection(state);
          std::cout << "final state " << state << std::endl;
          auto& dis = display::Display::Instance(&window, &renderer);
          auto face = cube::Face(state); // throw if wrong number of colors
          auto cube = cube::Cube(face);
          cube.verify(); // throw execption if not a regulat cube
          state::State::Instance().face_set(face);
          dis.repaint();
          break;
      }
    }
  }

  controller::start_controller(&event);

  return 0;
}
