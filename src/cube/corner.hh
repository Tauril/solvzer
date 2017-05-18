#pragma once

#include <array>

namespace cube
{

  enum class corner
  {
    URF,  // 0
    UFL,  // 1
    ULB,  // 2
    UBR,  // 3
    DFR,  // 4
    DLF,  // 5
    DBL,  // 6
    DRB   // 7
  };

  static constexpr std::array<corner, 8> corners =
    { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };

} // namespace cube
