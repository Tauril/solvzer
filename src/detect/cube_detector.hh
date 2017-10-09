#pragma once

#include <detect/detector.hh>
#include <detect/displayer.hh>

namespace detect
{
  class CubeDetector
  {
    public:
      CubeDetector();
      ~CubeDetector() = default;

      std::string detect_cube();

    private:
      Detector t1_, t2_, b1_, b2_;
      Displayer displayer_ = Displayer("solvzer");
  };
}
