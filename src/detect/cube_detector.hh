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
      bool ack_nb_faces();
      bool pass_sanity_duplicates();
      bool pass_unknown_colors();
      bool no_unknown_colors(Detector& d) const;

      Detector t1_, t2_, b1_, b2_;
      Displayer displayer_ = Displayer("solvzer");
  };
}
