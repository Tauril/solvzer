#pragma once

#include <array>

namespace cube
{

  enum edge
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

  using e_perm = std::array<edge, 12>;
  using e_ori = std::array<unsigned char, 12>;

  static constexpr e_perm edges =
    { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

} // namespace cube
