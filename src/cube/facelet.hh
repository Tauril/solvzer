#pragma once

#include <ostream>

namespace cube
{

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
    F1,   // 9
    F2,   // 10
    F3,   // 11
    F4,   // 12
    F5,   // 13
    F6,   // 14
    F7,   // 15
    F8,   // 16
    F9,   // 17
    R1,   // 18
    R2,   // 19
    R3,   // 20
    R4,   // 21
    R5,   // 22
    R6,   // 23
    R7,   // 24
    R8,   // 25
    R9,   // 26
    D1,   // 27
    D2,   // 28
    D3,   // 29
    D4,   // 30
    D5,   // 31
    D6,   // 32
    D7,   // 33
    D8,   // 34
    D9,   // 35
    B1,   // 36
    B2,   // 37
    B3,   // 38
    B4,   // 39
    B5,   // 40
    B6,   // 41
    B7,   // 42
    B8,   // 43
    B9,   // 44
    L1,   // 45
    L2,   // 46
    L3,   // 47
    L4,   // 48
    L5,   // 49
    L6,   // 50
    L7,   // 51
    L8,   // 52
    L9    // 53
  };

  facelet get_facelet(int f);

  std::ostream& operator<<(std::ostream& o, const facelet& f);

} // namespace cube
