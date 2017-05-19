#pragma once

#include <ostream>

namespace cube
{

  enum color
  {
    U,  // 0
    R,  // 1
    F,  // 2
    D,  // 3
    L,  // 4
    B   // 5
  };

  color get_color(int c);

  std::ostream& operator<<(std::ostream& o, const color& c);

} // namespace cube
