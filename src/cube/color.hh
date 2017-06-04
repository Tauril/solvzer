#pragma once

#include <ostream>

namespace cube
{

  /// We get the locations in the URFDLB order.
  enum color
  {
    U,  // 0
    R,  // 1
    F,  // 2
    D,  // 3
    L,  // 4
    B   // 5
  };

  color get_color(char c);

  std::ostream& operator<<(std::ostream& o, const color& c);

} // namespace cube
