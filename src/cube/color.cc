#include <cube/color.hh>
#include <misc/contract.hh>

namespace cube
{

  color
  get_color(int c)
  {
    switch (c)
    {
      case 0:
        return color::U;
      case 1:
        return color::R;
      case 2:
        return color::F;
      case 3:
        return color::D;
      case 4:
        return color::L;
      case 5:
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
