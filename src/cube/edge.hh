#pragma once

#include <array>

namespace cube
{

  enum class edge
  {
    UR,   // 0
    UF,   // 1
    UL,   // 2
    UB,   // 3
    DR,   // 4
    DF,   // 5
    DL,   // 6
    DB,   // 7
    FR,   // 8
    FL,   // 9
    BL,   // 10
    BR    // 11
  };

  static constexpr std::array<edge, 12> edges =
    { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

} // namespace cube
