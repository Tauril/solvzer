#pragma once

#include <array>
#include <iostream>

#include <cube/corner.hh>
#include <cube/edge.hh>
#include <cube/face.hh>

namespace cube
{

  /*
  ** Cube faces' locations
  **
  **      0   1   2   3   4   5   6   7   8   9   10  11
  **
  **                    `Up`
  **
  **                `---'---'---`
  **  0             | 0 | 1 | 2 |
  **                `---'---'---`
  **  1             | 3 | 4 | 5 |
  **      `Left`    `---'---'---`   `Right` |   `Back`
  **  2             | 6 | 7 | 8 |
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  3 | 36| 37| 38| 18| 19| 20| 9 | 10| 11| 45| 46| 47|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  4 | 39| 40| 41| 21| 22| 23| 12| 13| 14| 48| 49| 50|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  5 | 42| 43| 44| 24| 25| 26| 15| 16| 17| 51| 52| 53|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  6             | 27| 28| 29|
  **                `---'---'---`
  **  7             | 30| 31| 32|
  **                `---'---'---`
  **  8             | 33| 34| 35|
  **                `---'---'---`
  **
  **                    `Down`
  */

  class Cube
  {
    public:
      Cube() = default;
      Cube(const Face& face);
      Cube(const c_perm& corner_perm, const c_ori& corner_ori,
           const e_perm& edge_perm,   const e_ori& edge_ori);

      static void init_statics();

      /// Multiply `*this` and `cube`, restricted to the corners.
      void corner_multiply(const Cube& cube);
      /// Multiply `*this` and `cube`, restricted to the edges.
      void edge_multiply(const Cube& cube);

      int get_twist() const;
      void set_twist(int twist);

      int get_flip() const;
      void set_flip(int flip);

      int corner_parity() const;
      int edge_parity() const;

      int get_FR_to_BR() const;
      void set_FR_to_BR(int id);

      int get_URF_to_DLF() const;
      void set_URF_to_DLF(int id);

      int get_UR_to_DF() const;
      static int get_UR_to_DF(int id1, int id2);
      void set_UR_to_DF(int id);

      int get_UR_to_UL() const;
      void set_UR_to_UL(int id);

      int get_UB_to_DF() const;
      void set_UB_to_DF(int id);

      int get_URF_to_DLB() const;
      void set_URF_to_DLB(int id);

      int get_UR_to_BR() const;
      void set_UR_to_BR(int id);

      std::string verify() const;

      void to_face(Face& face) const;

    public:
      /// 6 basic cube moves.
      static std::array<Cube, 6> move_cube_;


      std::array<corner, 8> corner_perm_ =
        {{ URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB }};

      std::array<unsigned char, 8> corner_ori_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0 }};

      std::array<edge, 12> edge_perm_ =
        {{ UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR }};

      std::array<unsigned char, 12> edge_ori_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};


      static constexpr char solved_state_[] =
        "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
  };

  std::ostream& operator<<(std::ostream& o, const Cube& c);

} // namespace cube

#include <cube/cube.hxx>
