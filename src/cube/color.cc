/**
 ** \file cube/color.cc
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Implementation for cube/color.hh
 **/

#include <map>

#include <cube/color.hh>
#include <misc/contract.hh>

namespace cube
{

  color
  get_color(char c)
  {
    /// Associate the ASCII representation with its enum representation.
    static const std::map<char, color> colors =
    {
      { 'U', color::U }, { 'R', color::R }, { 'F', color::F },
      { 'D', color::D }, { 'L', color::L }, { 'B', color::B }
    };

    auto clr = colors.find(c);
    if (clr == colors.end())
      die("Can't match an integer to a color.");

    return clr->second;
  }

  char
  get_char_from_color(color c)
  {
    /// Associate the ASCII representation with its enum representation.
    static const std::map<color, char> colors =
    {
      { color::U, 'U' }, { color::R, 'R' }, { color::F, 'F' },
      { color::D, 'D' }, { color::L, 'L' }, { color::B, 'B' }
    };

    auto clr = colors.find(c);
    if (clr == colors.end())
      die("Can't match an integer to a color.");

    return clr->second;
  }

  std::string
  color_to_str(enum color c)
  {
    static const std::map<enum color, std::string> colors =
    {
      { color::U, "white" }, { color::R, "blue" }, { color::F, "red" },
      { color::D, "yellow"}, { color::L, "green" }, { color::B, "black" }
    };

    auto clr = colors.find(c);
    if (clr == colors.end())
      return "unknown";

    return clr->second;
  }

  std::ostream&
  operator<<(std::ostream& o, const color& c)
  {
    return o << static_cast<int>(c);
  }

} // namespace cube
