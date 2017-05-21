#include <cube/color.hh>
#include <misc/contract.hh>

namespace cube
{

  color
  get_color(int c)
  {
    switch (c)
    {
      case 'U':
        return color::U;
      case 'R':
        return color::R;
      case 'F':
        return color::F;
      case 'D':
        return color::D;
      case 'L':
        return color::L;
      case 'B':
        return color::B;
      default:
        die("Can't match an integer to a color");
    }
  }

  std::ostream&
  operator<<(std::ostream& o, const color& c)
  {
    return o << static_cast<int>(c);
  }

} // namespace cube
