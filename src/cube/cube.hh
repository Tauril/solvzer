/**
 ** \file cube/cube.hh
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Declaration of cube::Cube.
 **/

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

  /** \class Cube
   ** \brief Cube representation.
   **/
  class Cube
  {
  public:
    /** \name Ctor.
     ** \{ */
    /// Constructs a Cube.
    Cube() = default;
    /// Constructs a Cube from a Face.
    Cube(const Face& face);
    /// Constructs a Cube from corner and edge positions.
    Cube(const c_perm& corner_perm, const c_ori& corner_ori,
         const e_perm& edge_perm,   const e_ori& edge_ori);
    /** \} */

    /** \name Methods.
     ** \{ */
    static void init_statics();

    /// Multiply `*this` and `cube`, restricted to the corners.
    void corner_multiply(const Cube& cube);
    /// Multiply `*this` and `cube`, restricted to the edges.
    void edge_multiply(const Cube& cube);

    /// A corner is said to be twisted if the cubies belonging to
    /// the corner are in their home position and are not correctly
    /// oriented.
    int get_twist() const;
    void set_twist(int twist);

    /// An edge is said to be flipped if the cubies belonging to
    /// the edge are in their home position and are not correctly
    /// oriented.
    int get_flip() const;
    void set_flip(int flip);

    /**
     ** The parity of a permutation refers to whether that permutation
     ** can be represented by an even or an odd number of swaps.
     **
     ** Parity of the corners and edges must be the same if the cube
     ** is solvable.
     **/
    int corner_parity() const;
    int edge_parity() const;

    /// Permutations of the UD-slice edges (FR, FL, BL, BR).
    int get_FR_to_BR() const;
    void set_FR_to_BR(int id);

    /// Permutations of all corners except DBL and DRB.
    int get_URF_to_DLF() const;
    void set_URF_to_DLF(int id);

    /// Permutations of UR, UF, UL, UB, DR and DF.
    int get_UR_to_DF() const;
    static int get_UR_to_DF(int id1, int id2);
    void set_UR_to_DF(int id);

    /// Permutations of UR, UF and UL.
    int get_UR_to_UL() const;
    void set_UR_to_UL(int id);

    /// Permutations of UB, DR and DF.
    int get_UB_to_DF() const;
    void set_UB_to_DF(int id);

    /// Checks the cube is not ill-formed.
    std::string verify() const;

    void to_face(Face& face) const;
    /** \} */

    /** \name Attrributes.
     ** \{ */
    /// 6 basic cube moves.
    static std::array<Cube, 6> move_cube_;

    /// The rearrangement of the facelets after a move is called a
    /// `permutation`.

    /// The corners we permute.
    std::array<corner, 8> corner_perm_ =
      {{ URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB }};

    /// The edges we permute.
    std::array<edge, 12> edge_perm_ =
      {{ UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR }};

    /// For each corner, represents its orientation after a twist.
    /// 0: twist does not change.
    /// 1: clockwise twist.
    /// 2: anti-clockwise twist.
    std::array<unsigned char, 8> corner_ori_ =
      {{ 0, 0, 0, 0, 0, 0, 0, 0 }};

    /// For each edge, represents its orientation after a twist.
    /// 0: unflipped edge.
    /// 1: flipped edge.
    std::array<unsigned char, 12> edge_ori_ =
      {{ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 }};

    static constexpr char solved_state_[] =
      "UUUUUUUUURRRRRRRRRFFFFFFFFFDDDDDDDDDLLLLLLLLLBBBBBBBBB";
    /** \} */
  };

  /// Reports \a c on \a o for debugging purpose.
  std::ostream& operator<<(std::ostream& o, const Cube& c);

} // namespace cube

#include <cube/cube.hxx>
