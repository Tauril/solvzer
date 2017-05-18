#pragma once

#include <ostream>

namespace cube
{

  enum class color
  {
    U,    // 0
    F,    // 1
    R,    // 2
    D,    // 3
    B,    // 4
    L     // 5
  };

  color get_color(int c);

  std::ostream& operator<<(std::ostream& o, const color& c);

} // namespace cube
