#pragma once

#include <array>

namespace cube
{

  enum corner
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

  using c_perm = std::array<corner, 8>;
  using c_ori = std::array<unsigned char, 8>;

  static constexpr c_perm corners =
    { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };

} // namespace cube
