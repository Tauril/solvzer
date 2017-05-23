#include <map>

#include <cube/color.hh>
#include <misc/contract.hh>

namespace cube
{

  color
  get_color(int c)
  {
    std::map<int, color> colors =
    {
      {'U', color::U}, {'R', color::R}, {'F', color::F},
      {'D', color::D}, {'L', color::L}, {'B', color::B}
    };

    if (colors.find(c) == colors.end())
      die("Can't match an integer to a color.");
    else
      return colors.at(c);
  }

  std::ostream&
  operator<<(std::ostream& o, const color& c)
  {
    return o << static_cast<int>(c);
  }

} // namespace cube
