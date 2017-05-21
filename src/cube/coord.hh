#pragma once

#include <array>

#include <cube/cube.hh>

namespace cube
{

  class Coord
  {
    public:
      Coord(const Cube& c);

      static void init_statics();
      static void set_pruning(unsigned char arr[], int index,
                              unsigned char value);
      static unsigned char get_pruning(unsigned char arr[], int index);

      int twist_        = 0;
      int flip_         = 0;
      int parity_       = 0;
      int FR_to_BR_     = 0;
      int URF_to_DLF_   = 0;
      int UR_to_UL_     = 0;
      int UB_to_DF_     = 0;
      int UR_to_DF_     = 0;

      static constexpr int TWIST      = 2187;
      static constexpr int FLIP       = 2048;
      static constexpr int SLICE1     = 495;
      static constexpr int SLICE2     = 24;
      static constexpr int PARITY     = 2;
      static constexpr int URF_to_DLF = 20160;
      static constexpr int FR_to_BR   = 11880;
      static constexpr int UR_to_UL   = 1320;
      static constexpr int UB_to_DF   = 1320;
      static constexpr int UR_to_DF   = 20160;
      static constexpr int URF_to_DLB = 40320;
      static constexpr int UR_to_BR   = 479001600;
      static constexpr int MOVE       = 18;

      static constexpr std::array<std::array<int, 18>, 2> parity_move_ =
      {{
        { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
        { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
      }};
      static std::array<std::array<int, MOVE>, TWIST> twist_move_;
      static std::array<std::array<int, MOVE>, FLIP> flip_move_;
      static std::array<std::array<int, MOVE>, FR_to_BR> FR_to_BR_move_;
      static std::array<std::array<int, MOVE>, URF_to_DLF> URF_to_DLF_move_;
      static std::array<std::array<int, MOVE>, UR_to_DF> UR_to_DF_move_;
      static std::array<std::array<int, MOVE>, UR_to_UL> UR_to_UL_move_;
      static std::array<std::array<int, MOVE>, UB_to_DF> UB_to_DF_move_;
      static std::array<std::array<int, 336>, 336> merge_UR_to_UL_and_UB_to_DF_;

      /// The pruning table entries give a lower estimation for the number
      /// of moves to reach the solved cube.
      static unsigned char
      slice_URF_to_DLF_parity_prune_[SLICE2 * URF_to_DLF * PARITY / 2];
      static unsigned char
      slice_UR_to_DF_parity_prune_[SLICE2 * UR_to_DF * PARITY / 2];

      /// The pruning table entries give a lower estimation for the number
      /// of moves to reach the H-subgroup.
      static unsigned char
      slice_twist_prune_[SLICE1 * TWIST / 2 + 1];
      static unsigned char
      slice_flip_prune_[SLICE1 * FLIP / 2];
  };

} // namespace cube
