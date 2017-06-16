/**
 ** \file cube/facelet.hh
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Declaration of cube::facelet.
 **/

#pragma once

#include <ostream>

namespace cube
{

  /** \enum  facelet
   ** \brief Each facelet represents a position in the cube. On a solved
   **        state, the facelets are positioned as follows:
   **
   **      0   1   2   3   4   5   6   7   8   9   10  11
   **
   **                    `Up`
   **
   **                `---'---'---`
   **  0             | U1| U2| U3|
   **                `---'---'---`
   **  1             | U4| U5| U6|
   **      `Left`    `---'---'---`   `Right` |   `Back`
   **  2             | U7| U8| U9|
   **    `---'---'---'---'---'---'---'---'---'---'---'---`
   **  3 | L1| L2| L3| F1| F2| F3| R1| R2| R3| B1| B2| B3|
   **    `---'---'---'---'---'---'---'---'---'---'---'---`
   **  4 | L4| L5| L6| F4| F5| F6| R4| R5| R6| B4| B5| B6|
   **    `---'---'---'---'---'---'---'---'---'---'---'---`
   **  5 | L7| L8| L9| F7| F8| F9| R7| R8| R9| B7| B8| B9|
   **    `---'---'---'---'---'---'---'---'---'---'---'---`
   **  6             | D1| D2| D3|
   **                `---'---'---`
   **  7             | D4| D5| D6|
   **                `---'---'---`
   **  8             | D7| D8| D9|
   **                `---'---'---`
   **
   **                    `Down`
   **
   **/
  enum facelet
  {
    U1,   // 0
    U2,   // 1
    U3,   // 2
    U4,   // 3
    U5,   // 4
    U6,   // 5
    U7,   // 6
    U8,   // 7
    U9,   // 8
    R1,   // 9
    R2,   // 10
    R3,   // 11
    R4,   // 12
    R5,   // 13
    R6,   // 14
    R7,   // 15
    R8,   // 16
    R9,   // 17
    F1,   // 18
    F2,   // 19
    F3,   // 20
    F4,   // 21
    F5,   // 22
    F6,   // 23
    F7,   // 24
    F8,   // 25
    F9,   // 26
    D1,   // 27
    D2,   // 28
    D3,   // 29
    D4,   // 30
    D5,   // 31
    D6,   // 32
    D7,   // 33
    D8,   // 34
    D9,   // 35
    L1,   // 36
    L2,   // 37
    L3,   // 38
    L4,   // 39
    L5,   // 40
    L6,   // 41
    L7,   // 42
    L8,   // 43
    L9,   // 44
    B1,   // 45
    B2,   // 46
    B3,   // 47
    B4,   // 48
    B5,   // 49
    B6,   // 50
    B7,   // 51
    B8,   // 52
    B9    // 53
  };

  /// \brief      Returns a facelet from its enum value.
  /// \param c    the enum value.
  /// \return     The facelet.
  facelet get_facelet(int f);

  /// Reports \a f on \a o for debugging purpose.
  std::ostream& operator<<(std::ostream& o, const facelet& f);

} // namespace cube
