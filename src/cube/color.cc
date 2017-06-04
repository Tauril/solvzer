#include <map>

#include <cube/color.hh>
#include <misc/contract.hh>

namespace cube
{

  color
  get_color(char c)
  {
    std::map<char, color> colors =
    {
      {'U', color::U}, {'R', color::R}, {'F', color::F},
      {'D', color::D}, {'L', color::L}, {'B', color::B}
    };

    auto clr = colors.find(c);
    if (clr == colors.end())
      die("Can't match an integer to a color.");

    return clr->second;
  }

  std::ostream&
  operator<<(std::ostream& o, const color& c)
  {
    return o << static_cast<int>(c);
  }

} // namespace cube
