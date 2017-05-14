#include <algorithm>

#include <cube/cube.hh>
#include <misc/indent.hh>

namespace cube
{

  Cube::Cube(std::array<std::array<int, 9>, 6>& cube)
  {
    for (auto i = 0; i < 6; i++)
        cube_[i] = Face(cube[i]);
  }

  Cube::Cube(cube_t& cube)
  : cube_(cube)
  {}

  std::ostream&
  operator<<(std::ostream& o, const Cube& c)
  {
    auto sep = "`---'---'---`";

    // Up
    for (auto i = 0; i < 3; i++)
    {
      misc::indent(o);

      if (i == 0)
      {
        o << sep
          << std::endl;

        misc::indent(o);
      }

      c.cube_get()[0].dump_row(o, i, 0);
      o << std::endl;

      if (i < 2)
      {
        misc::indent(o);

        o << sep
          << std::endl;
      }
    }

    for (auto i = 0; i < 4; i++)
    {
      if (i == 0)
        o << '`';
      o << "---'---'---`";
    }

    o << std::endl;


    // Left | Right | Back
    for (auto i = 0; i < 3; i++)
    {
      int x = 0;
      for (auto j = 1; j < 5; j++, x++)
      {

        c.cube_get()[j].dump_row(o, i, x);
      }
      o << std::endl;

      for (auto k = 0; k < 4; k++)
      {
        if (k == 0)
          o << '`';
        o << "---'---'---`";
      }

      o << std::endl;
    }


    // Down
    for (auto i = 0; i < 3; i++)
    {
      misc::indent(o);

      c.cube_get()[5].dump_row(o, i, 0);
      o << std::endl;

      misc::indent(o);

      o << sep
        << std::endl;
    }

    return o;
  }

} // namespace cube
