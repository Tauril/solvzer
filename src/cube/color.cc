#include <cube/color.hh>
#include <misc/contract.hh>

namespace cube
{

  color
  get_color(int c)
  {
    switch (c)
    {
      case 85:  // "U"
        return color::U;
      case 82:  // "R"
        return color::R;
      case 70:  // "F"
        return color::F;
      case 68:  // "D"
        return color::D;
      case 76:  // "L"
        return color::L;
      case 66:  // "B"
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
