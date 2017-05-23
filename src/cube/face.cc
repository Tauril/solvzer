#include <cstdlib>
#include <ctime>

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
    std::string moves = "";

    std::srand(std::time(0));
    for (int i = 0; i < 30; i++)
    {
      auto n = std::rand() % 18;

      switch (n)
      {
        case 0:
          moves += "U";
          break;
        case 1:
          moves += "U2";
          break;
        case 2:
          moves += "U'";
          break;
        case 3:
          moves += "R";
          break;
        case 4:
          moves += "R2";
          break;
        case 5:
          moves += "R'";
          break;
        case 6:
          moves += "F";
          break;
        case 7:
          moves += "F2";
          break;
        case 8:
          moves += "F'";
          break;
        case 9:
          moves += "D";
          break;
        case 10:
          moves +="D2";
          break;
        case 11:
          moves += "D'";
          break;
        case 12:
          moves += "L";
          break;
        case 13:
          moves += "L2";
          break;
        case 14:
          moves += "L'";
          break;
        case 15:
          moves += "B";
          break;
        case 16:
          moves += "B2";
          break;
        case 17:
          moves += "B'";
          break;
      }

      if (i < 29)
        moves += ' ';
    }

    std::string state = move::make_moves(face_str_, moves);

    face_set(state);

    std::cout << "Scramble: " << moves << std::endl;

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
