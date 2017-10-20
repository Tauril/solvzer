#pragma once

#include <detect/detector.hh>
#include <detect/displayer.hh>
#include <cube/move.hh>

namespace detect
{
  class CubeDetector
  {
    public:
      CubeDetector();
      ~CubeDetector() = default;

      std::string detect_cube(bool second_run);

    private:
      Detector t1_, t2_, b1_, b2_;
      Displayer displayer_ = Displayer("solvzer");
  };
}
