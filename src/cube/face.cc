#include <cube/face.hh>
#include <misc/contract.hh>
#include <misc/indent.hh>

namespace cube
{

  constexpr facelet Face::corner_facelet_[8][3];
  constexpr facelet Face::edge_facelet_[12][2];
  constexpr color Face::corner_color_[8][3];
  constexpr color Face::edge_color_[12][2];

  Face::Face(const std::string& cube)
  {
    precondition(cube.size() == 54);

    for (unsigned i = 0; i < cube.size(); i++)
      face_[i] = get_color(cube[i]);
  }

  std::ostream&
  Face::dump_row(std::ostream& o, int i, int j, int x) const
  {
    // If i == 2, we want to go from (begin + 6) to (begin + 9).
    // If j == 0, we want the firt color.
    for (auto it = face_.begin() + (j * 9) + (i * 3);
         it != face_.begin() + (j * 9) + ((i + 1) * 3); it++, x++)
    {
      if (x == 0)
        o << '|';

      o << ' ' << clr_to_chr(*it) << " |";
    }

    return o;
  }

  std::ostream&
  operator<<(std::ostream& o, const Face& f)
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

      f.dump_row(o, i, 0, 0);
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


    // Left | Face | Right | Back
    for (auto i = 0; i < 3; i++)
    {
      int x = 0;
      for (auto j = 4; x < 4; j = j == 4 ? 2 : j == 2 ? 1 : 5, x++)
        f.dump_row(o, i, j, x);
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

      f.dump_row(o, i, 3, 0);
      o << std::endl;

      misc::indent(o);

      o << sep << std::endl;
    }

    return o;
  }

  char
  clr_to_chr(color c)
  {
    return "URFDLB"[c];
  }

} // namespace cube
