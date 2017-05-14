#include <cstdlib>

#include <cube/color.hh>

namespace cube
{

  color
  get_color(int c)
  {
    switch (c)
    {
      case 0:
        return color::white;
      case 1:
        return color::blue;
      case 2:
        return color::orange;
      case 3:
        return color::green;
      case 4:
        return color::red;
      case 5:
        return color::yellow;
      default:
        std::abort();
    }
  }

} // namespace cube
