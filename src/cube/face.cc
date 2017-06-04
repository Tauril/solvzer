#include <cstdlib>
#include <ctime>
#include <map>

#include <cube/face.hh>
#include <cube/move.hh>
#include <misc/indent.hh>

namespace cube
{

  constexpr facelet Face::corner_facelet_[8][3];
  constexpr facelet Face::edge_facelet_[12][2];
  constexpr color Face::corner_color_[8][3];
  constexpr color Face::edge_color_[12][2];

  Face::Face(const std::string& cube)
  {
    face_set(cube);
  }

  std::string
  Face::scramble()
  {
    std::string moves_str = "";
    std::map<int, std::string> moves =
    {
      {0,  "U"}, {1,  "U2"}, {2,  "U'"}, {3,  "R"}, {4,  "R2"}, {5,  "R'"},
      {6,  "F"}, {7,  "F2"}, {8,  "F'"}, {9,  "D"}, {10, "D2"}, {11, "D'"},
      {12, "L"}, {13, "L2"}, {14, "L'"}, {15, "B"}, {16, "B2"}, {17, "B'"}
    };

    std::srand(std::time(0));
    int scramble_length = 30;
    for (int i = 0; i < scramble_length; i++)
    {
      auto n = std::rand() % moves.size();

      moves_str += moves.at(n);

      if (i < scramble_length - 1)
        moves_str += ' ';
    }

    std::string state = move::make_moves(face_str_, moves_str);

    face_set(state);

    std::cout << "Scramble: " << moves_str << std::endl;

    return state;
  }

  std::ostream&
  Face::dump_row(std::ostream& o, int i, int j, int x) const
  {
    // If i == 2, we want to go from (begin + 6) to (begin + 9).
    // If j == 0, we want the firt color.
    for (auto it = face_str_.begin() + (j * 9) + (i * 3);
         it != face_str_.begin() + (j * 9) + ((i + 1) * 3); it++, x++)
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

} // namespace cube
