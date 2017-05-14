#pragma once

#include <ostream>

namespace cube
{

  enum class color
  {
    white,    // 0
    blue,     // 1
    orange,   // 2
    green,    // 3
    red,      // 4
    yellow    // 5
  };

  color get_color(int c);

  std::ostream& operator<<(std::ostream& o, const color& c);

} // namespace cube
