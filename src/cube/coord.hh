/**
 ** \file cube/coord.hh
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Declaration of cube::Coord.
 **/

#pragma once

#include <array>
#include <unordered_map>

#include <cube/cube.hh>

namespace cube
{

  /**
   ** \class Coord
   ** \brief Permutations and orientations of the edges and the corners
   **        are represented by natural numbers, useful to implement a
   **        fast solving algorithm.
   **/
  class Coord
  {
    public:
      /** \name Ctor.
       ** \{ */
      /// Constructs a Coord from a Cube.
      Coord(const Cube& c);
      /** \} */

      /** \name Static methods.
       ** \{ */
      /// Initializes all the static arrays used in the solving algorithm.
      static void init_statics();

      /// Used to determine the minimum depth in the solving algorithm.
      static void set_pruning(unsigned char arr[], int index,
                              unsigned char value);
      static unsigned char get_pruning(unsigned char arr[], int index);
      /** \} */

      /** \name Attributes.
       ** \{ */
      /// Each coordinate must be equal to 0 for a solved state.
      int twist_      = 0;
      int flip_       = 0;
      int parity_     = 0;
      int FR_to_BR_   = 0;
      int URF_to_DLF_ = 0;
      int UR_to_UL_   = 0;
      int UB_to_DF_   = 0;
      int UR_to_DF_   = 0;

      /// Orientation of the 8 corners (3^7 possible solutions).
      static constexpr int TWIST      = 2187;
      /// Orientation of the 12 edges (2^11 possible solutions).
      static constexpr int FLIP       = 2048;
      /// 12 choose 4 possible positions of FR, FL, BL, BR edges.
      static constexpr int SLICE1     = 495;
      /// 4! possible permutations of FR, FL, BL, BR edges in phase2
      /// (see search.cc).
      static constexpr int SLICE2     = 24;
      /// 2 possible parities.
      static constexpr int PARITY     = 2;
      /// 8!(8-6)! possible permutations of
      /// URF, UFL, ULB, UBR, DFR, DLF corners.
      static constexpr int URF_to_DLF = 20160;
      /// 12!/(12-4)! possible permutations of FR, FL, BL, BR edges.
      static constexpr int FR_to_BR   = 11880;
      /// 12!/(12-3)! possible permutations of UF, UL, UB edges.
      static constexpr int UR_to_UL   = 1320;
      /// 12!/(12-3)! possible permutations of UB, DR, DF edges.
      static constexpr int UB_to_DF   = 1320;
      /// 8!/(8-6)! possible permutations of UR, UF, UL, UB, DR, DF edges.
      static constexpr int UR_to_DF   = 20160;
      /// Number of moves.
      static constexpr int MOVE       = 18;

      /// Parity of the corner permutation.
      static constexpr std::array<std::array<int, MOVE>, PARITY> parity_move_ =
      {{
        { 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1, 1, 0, 1 },
        { 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0, 0, 1, 0 }
      }};
      /// Move tables for the permutations.
      static std::array<std::array<int, MOVE>, TWIST> twist_move_;
      static std::array<std::array<int, MOVE>, FLIP> flip_move_;
      static std::array<std::array<int, MOVE>, URF_to_DLF> URF_to_DLF_move_;
      static std::array<std::array<int, MOVE>, FR_to_BR> FR_to_BR_move_;
      static std::array<std::array<int, MOVE>, UR_to_UL> UR_to_UL_move_;
      static std::array<std::array<int, MOVE>, UB_to_DF> UB_to_DF_move_;
      static std::array<std::array<int, MOVE>, UR_to_DF> UR_to_DF_move_;
      static std::array<std::array<int, 336>, 336> merge_UR_to_UL_and_UB_to_DF_;

      /// The pruning table entries give a lower estimation for the number
      /// of moves to reach the solved cube.
      static unsigned char
      slice_URF_to_DLF_parity_prune_[SLICE2 * URF_to_DLF];
      static unsigned char
      slice_UR_to_DF_parity_prune_[SLICE2 * UR_to_DF];

      /// The pruning table entries give a lower estimation for the number
      /// of moves to reach the H-subgroup.
      static unsigned char
      slice_twist_prune_[SLICE1 * TWIST / 2 + 1];
      static unsigned char
      slice_flip_prune_[SLICE1 * FLIP / 2];
      /** \} */
  };

} // namespace cube
