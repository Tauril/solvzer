#pragma once

#include <array>
#include <iostream>

#include <cube/corner.hh>
#include <cube/edge.hh>

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
      Cube(c_perm& corner_perm, c_ori& corner_ori,
           e_perm& edge_perm,   e_ori& edge_ori);

      static void init_move_cube();

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

    private:
      /// Up moves.
      static constexpr std::array<corner, 8> corner_perm_U_ =
        {{ UBR, URF, UFL, ULB, DFR, DLF, DBL, DRB }};

      static constexpr std::array<unsigned char, 8> corner_ori_U_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0 }};

      static constexpr std::array<edge, 12> edge_perm_U_ =
        {{ UB, UR, UF, UL, DR, DF, DL, DB, FR, FL, BL, BR }};

      static constexpr std::array<unsigned char, 12> edge_ori_U_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};


      /// Right moves.
      static constexpr std::array<corner, 8> corner_perm_R_ =
        {{ DFR, UFL, ULB, URF, DRB, DLF, DBL, UBR }};

      static constexpr std::array<unsigned char, 8> corner_ori_R_ =
        {{ 2, 0, 0, 1, 1, 0, 0, 2 }};

      static constexpr std::array<edge, 12> edge_perm_R_ =
        {{ FR, UF, UL, UB, BR, DF, DL, DB, DR, FL, BL, UR }};

      static constexpr std::array<unsigned char, 12> edge_ori_R_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};


      /// Face moves.
      static constexpr std::array<corner, 8> corner_perm_F_ =
        {{ UFL, DLF, ULB, UBR, URF, DFR, DBL, DRB }};

      static constexpr std::array<unsigned char, 8> corner_ori_F_ =
        {{ 1, 2, 0, 0, 2, 1, 0, 0 }};

      static constexpr std::array<edge, 12> edge_perm_F_ =
        {{ UR, FL, UL, UB, DR, FR, DL, DB, UF, DF, BL, BR }};

      static constexpr std::array<unsigned char, 12> edge_ori_F_ =
        {{ 0, 1, 0, 0, 0, 1, 0, 0, 1, 1, 0, 0 }};


      /// Down moves.
      static constexpr std::array<corner, 8> corner_perm_D_ =
        {{ URF, UFL, ULB, UBR, DLF, DBL, DRB, DFR }};

      static constexpr std::array<unsigned char, 8> corner_ori_D_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0 }};

      static constexpr std::array<edge, 12> edge_perm_D_ =
        {{ UR, UF, UL, UB, DF, DL, DB, DR, FR, FL, BL, BR }};

      static constexpr std::array<unsigned char, 12> edge_ori_D_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};


      /// Left moves.
      static constexpr std::array<corner, 8> corner_perm_L_ =
        {{ URF, ULB, DBL, UBR, DFR, UFL, DLF, DRB }};

      static constexpr std::array<unsigned char, 8> corner_ori_L_ =
        {{ 0, 1, 2, 0, 0, 2, 1, 0 }};

      static constexpr std::array<edge, 12> edge_perm_L_ =
        {{ UR, UF, BL, UB, DR, DF, FL, DB, FR, UL, DL, BR }};

      static constexpr std::array<unsigned char, 12> edge_ori_L_ =
        {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};


      /// Back moves.
      static constexpr std::array<corner, 8> corner_perm_B_ =
        {{ URF, UFL, UBR, DRB, DFR, DLF, ULB, DBL }};

      static constexpr std::array<unsigned char, 8> corner_ori_B_ =
        {{ 0, 0, 1, 2, 0, 0, 2, 1 }};

      static constexpr std::array<edge, 12> edge_perm_B_ =
        {{ UR, UF, UL, BR, DR, DF, DL, BL, FR, FL, UB, DB }};

      static constexpr std::array<unsigned char, 12> edge_ori_B_ =
        {{ 0, 0, 0, 1, 0, 0, 0, 1, 0, 0, 1, 1 }};
  };

  std::ostream& operator<<(std::ostream& o, const Cube& c);

} // namespace cube
