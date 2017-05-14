#include <algorithm>

#include <cube/face.hh>

namespace cube
{

  Face::Face(std::array<int, 9>& face)
  {
    for (auto i = 0; i < 9; i++)
      face_[i] = get_color(face[i]);
  }

  Face::Face(face_t& face)
    : face_(face)
  {}

  std::ostream&
  Face::dump_row(std::ostream& o, int i, int x) const
  {
    // If i = 2, we want to go from (begin + 6) to (begin + 9).
    for (auto it = begin() + (i * 3); it != begin() + ((i + 1) * 3); it++, x++)
    {
      if (x == 0)
        o << '|';

      o << ' ' << *it << " |";
    }

    return o;
  }

  std::ostream&
  operator<<(std::ostream& o, const Face& f)
  {
    int i = 0;
    auto sep = "`---'---'---`";

    o << sep << std::endl;

    std::for_each(f.begin(), f.end(),
                  [&i, &o, sep](const color& c)
                  {
                    if (i == 0)
                      o << '|';

                    o << ' ' << c << " |";

                    if (++i == 3)
                    {
                      o << std::endl
                        << sep << std::endl;
                      i = 0;
                    }
                  });

    return o;
  }

} // namespace cube
