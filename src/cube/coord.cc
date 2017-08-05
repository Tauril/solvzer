/**
 ** \file cube/coord.cc
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Implementation for cube::Coord.
 **/

#include <functional>

#include <cube/coord.hh>

namespace cube
{

  /// Instantiate static attributes.
  constexpr int Coord::TWIST;
  constexpr int Coord::FLIP;
  constexpr int Coord::FR_to_BR;
  constexpr int Coord::URF_to_DLF;
  constexpr int Coord::UR_to_DF;
  constexpr int Coord::UR_to_UL;
  constexpr int Coord::UB_to_DF;

  constexpr std::array<std::array<int, Coord::MOVE>, Coord::PARITY>
    Coord::parity_move_;
  std::array<std::array<int, Coord::MOVE>, Coord::TWIST>
    Coord::twist_move_;
  std::array<std::array<int, Coord::MOVE>, Coord::FLIP>
    Coord::flip_move_;
  std::array<std::array<int, Coord::MOVE>, Coord::FR_to_BR>
    Coord::FR_to_BR_move_;
  std::array<std::array<int, Coord::MOVE>, Coord::URF_to_DLF>
    Coord::URF_to_DLF_move_;
  std::array<std::array<int, Coord::MOVE>, Coord::UR_to_DF>
    Coord::UR_to_DF_move_;
  std::array<std::array<int, Coord::MOVE>, Coord::UR_to_UL>
    Coord::UR_to_UL_move_;
  std::array<std::array<int, Coord::MOVE>, Coord::UB_to_DF>
    Coord::UB_to_DF_move_;
  std::array<std::array<int, 336>, 336>
    Coord::merge_UR_to_UL_and_UB_to_DF_;

  unsigned char
    Coord::slice_URF_to_DLF_parity_prune_[Coord::SLICE2 * Coord::URF_to_DLF];
  unsigned char
    Coord::slice_UR_to_DF_parity_prune_[Coord::SLICE2 * Coord::UR_to_DF];
  unsigned char
    Coord::slice_twist_prune_[Coord::SLICE1 * Coord::TWIST / 2 + 1];
  unsigned char
    Coord::slice_flip_prune_[Coord::SLICE1 * Coord::FLIP / 2];

  namespace
  {

    template <int N>
    void
    init_move(std::array<std::array<int, Coord::MOVE>, N>& move, bool is_edge)
    {
      using namespace std::placeholders;

      Cube c;

      // Workaround to avoid complains about the overloaded method.
      std::function<int()> get_UR_to_DF = [&c]{ return c.get_UR_to_DF(); };

      const std::unordered_map<int, std::pair<std::function<void(int)>,
                                              std::function<int()>>>
        func_mapper =
      {
        { Coord::TWIST,      { std::bind(&Cube::set_twist, &c, _1),
                               std::bind(&Cube::get_twist, &c) } },

        { Coord::FLIP,       { std::bind(&Cube::set_flip, &c, _1),
                               std::bind(&Cube::get_flip, &c) } },

        { Coord::FR_to_BR,   { std::bind(&Cube::set_FR_to_BR, &c, _1),
                               std::bind(&Cube::get_FR_to_BR, &c) } },

        { Coord::UR_to_UL,   { std::bind(&Cube::set_UR_to_UL, &c, _1),
                               std::bind(&Cube::get_UR_to_UL, &c) } },

        { Coord::UB_to_DF,   { std::bind(&Cube::set_UB_to_DF, &c, _1),
                               std::bind(&Cube::get_UB_to_DF, &c) } }
      };

      // UR_to_DF and URF_to_DLF have the same constant value.
      const std::unordered_map<bool, std::pair<std::function<void(int)>,
                                               std::function<int()>>>
        UR_to_DF_or_URF_to_DLF =
      {
        { true,  { std::bind(&Cube::set_UR_to_DF, &c, _1),
                             get_UR_to_DF } },

        { false, { std::bind(&Cube::set_URF_to_DLF, &c, _1),
                             std::bind(&Cube::get_URF_to_DLF, &c) } }
      };

      // Associate the upper bound with the corresponding methods to call.
      const auto func_mapper_l =
        [is_edge, &func_mapper, &UR_to_DF_or_URF_to_DLF]
      {
        // Distinguish which methods to call.
        if (N == Coord:: URF_to_DLF)
          return UR_to_DF_or_URF_to_DLF.at(is_edge);

        return func_mapper.at(N);
      };

      static const std::unordered_map<bool, std::function<void(const Cube&)>>
        multiply =
      {
        { true,  std::bind(&Cube::edge_multiply, &c, _1) },
        { false, std::bind(&Cube::corner_multiply, &c, _1) }
      };

      for (int i = 0; i < N; i++)
      {
        func_mapper_l().first(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            multiply.at(is_edge)(Cube::move_cube_[j]);
            move[i][3 * j + k] = func_mapper_l().second();
          }
          multiply.at(is_edge)(Cube::move_cube_[j]);
        }
      }
    }

    void
    init_merge_UR_to_UL_and_UB_to_DF()
    {
      for (int i = 0; i < 336; i++)
        for (int j = 0; j < 336; j++)
          Coord::merge_UR_to_UL_and_UB_to_DF_[i][j] = Cube::get_UR_to_DF(i, j);
    }

    void
    init_slice_URF_to_DLF_parity_prune()
    {
      for (int i = 0; i < Coord::SLICE2 * Coord::URF_to_DLF; i++)
        Coord::slice_URF_to_DLF_parity_prune_[i] = -1;

      Coord::set_pruning(Coord::slice_URF_to_DLF_parity_prune_, 0,
                         static_cast<unsigned char>(0));

      int depth = 0;
      int done = 1;
      while (done != Coord::SLICE2 * Coord::URF_to_DLF * Coord::PARITY)
      {
        for (int i = 0;
             i < Coord::SLICE2 * Coord::URF_to_DLF * Coord::PARITY; i++)
        {
          if (Coord::get_pruning(Coord::slice_URF_to_DLF_parity_prune_, i)
              == depth)
          {
            int parity = i % 2;
            int URF_to_DLF = (i / 2) / Coord::SLICE2;
            int slice = (i / 2) % Coord::SLICE2;

            for (int j = 0; j < Coord::MOVE; j++)
            {
              switch (j)
              {
                case 3:  case 5:  case 6:  case 8:
                case 12: case 14: case 15: case 17:
                  continue;
                default:
                  int new_slice = Coord::FR_to_BR_move_[slice][j];
                  int new_URF_to_DLF =
                    Coord::URF_to_DLF_move_[URF_to_DLF][j];
                  int new_parity = Coord::parity_move_[parity][j];

                  if (Coord::get_pruning(Coord::slice_URF_to_DLF_parity_prune_,
                                         (Coord::SLICE2 * new_URF_to_DLF
                                          + new_slice) * 2 + new_parity)
                      == 0x0f)
                  {
                    Coord::set_pruning(Coord::slice_URF_to_DLF_parity_prune_,
                                       (Coord::SLICE2 * new_URF_to_DLF
                                        + new_slice) * 2 + new_parity,
                                       static_cast<unsigned char>(depth + 1));
                    done++;
                  }
              }
            }
          }
        }
        depth++;
      }
    }

    void
    init_slice_UR_to_DF_parity_prune()
    {
      for (int i = 0; i < Coord::SLICE2 * Coord::UR_to_DF; i++)
        Coord::slice_UR_to_DF_parity_prune_[i] = -1;

      Coord::set_pruning(Coord::slice_UR_to_DF_parity_prune_, 0,
                         static_cast<unsigned char>(0));

      int depth = 0;
      int done = 1;
      while (done != Coord::SLICE2 * Coord::UR_to_DF * Coord::PARITY)
      {
        for (int i = 0;
             i < Coord::SLICE2 * Coord::UR_to_DF * Coord::PARITY; i++)
        {
          if (Coord::get_pruning(Coord::slice_UR_to_DF_parity_prune_, i)
              == depth)
          {
            int parity = i % 2;
            int UR_to_DF = (i / 2) / Coord::SLICE2;
            int slice = (i / 2) % Coord::SLICE2;

            for (int j = 0; j < Coord::MOVE; j++)
            {
              switch (j)
              {
                case 3:  case 5:  case 6:  case 8:
                case 12: case 14: case 15: case 17:
                  continue;
                default:
                  int new_slice = Coord::FR_to_BR_move_[slice][j];
                  int new_UR_to_DF =
                    Coord::UR_to_DF_move_[UR_to_DF][j];
                  int new_parity = Coord::parity_move_[parity][j];

                  if (Coord::get_pruning(Coord::slice_UR_to_DF_parity_prune_,
                                         (Coord::SLICE2 * new_UR_to_DF
                                          + new_slice) * 2 + new_parity)
                      == 0x0f)
                  {
                    Coord::set_pruning(Coord::slice_UR_to_DF_parity_prune_,
                                       (Coord::SLICE2 * new_UR_to_DF
                                        + new_slice) * 2 + new_parity,
                                       static_cast<unsigned char>(depth + 1));
                    done++;
                  }
              }
            }
          }
        }
        depth++;
      }
    }

    void
    init_slice_twist_prune()
    {
      for (int i = 0; i < Coord::SLICE1 * Coord::TWIST / 2 + 1; i++)
        Coord::slice_twist_prune_[i] = -1;

      Coord::set_pruning(Coord::slice_twist_prune_, 0,
                         static_cast<unsigned char>(0));

      int depth = 0;
      int done = 1;
      while (done != Coord::SLICE1 * Coord::TWIST)
      {
        for (int i = 0; i < Coord::SLICE1 * Coord::TWIST; i++)
        {
          if (Coord::get_pruning(Coord::slice_twist_prune_, i) == depth)
          {
            int twist = i / Coord::SLICE1;
            int slice = i % Coord::SLICE1;

            for (int j = 0; j < Coord::MOVE; j++)
            {
              int new_slice =
                Coord::FR_to_BR_move_[slice * Coord::SLICE2][j] / Coord::SLICE2;
              int new_twist = Coord::twist_move_[twist][j];

              if (Coord::get_pruning(Coord::slice_twist_prune_,
                              Coord::SLICE1 * new_twist + new_slice) == 0x0f)
              {
                Coord::set_pruning(Coord::slice_twist_prune_,
                                   Coord::SLICE1 * new_twist + new_slice,
                                   static_cast<unsigned char>(depth + 1));
                done++;
              }
            }
          }
        }
        depth++;
      }
    }

    void
    init_slice_flip_prune()
    {
      for (int i = 0; i < Coord::SLICE1 * Coord::FLIP / 2; i++)
        Coord::slice_flip_prune_[i] = -1;

      Coord::set_pruning(Coord::slice_flip_prune_, 0,
                         static_cast<unsigned char>(0));

      int depth = 0;
      int done = 1;
      while (done != Coord::SLICE1 * Coord::FLIP)
      {
        for (int i = 0; i < Coord::SLICE1 * Coord::FLIP; i++)
        {
          if (Coord::get_pruning(Coord::slice_flip_prune_, i) == depth)
          {
            int flip = i / Coord::SLICE1;
            int slice = i % Coord::SLICE1;

            for (int j = 0; j < Coord::MOVE; j++)
            {
              int new_slice =
                Coord::FR_to_BR_move_[slice * Coord::SLICE2][j] / Coord::SLICE2;
              int new_flip = Coord::flip_move_[flip][j];

              if (Coord::get_pruning(Coord::slice_flip_prune_,
                                     Coord::SLICE1 * new_flip + new_slice)
                  == 0x0f)
              {
                Coord::set_pruning(Coord::slice_flip_prune_,
                                   Coord::SLICE1 * new_flip + new_slice,
                                   static_cast<unsigned char>(depth + 1));
                done++;
              }
            }
          }
        }
        depth++;
      }
    }

  }

  Coord::Coord(const Cube& c)
    : twist_(c.get_twist())
    , flip_(c.get_flip())
    , parity_(c.corner_parity())
    , FR_to_BR_(c.get_FR_to_BR())
    , URF_to_DLF_(c.get_URF_to_DLF())
    , UR_to_UL_(c.get_UR_to_UL())
    , UB_to_DF_(c.get_UB_to_DF())
    , UR_to_DF_(c.get_UR_to_DF())
  {}

  void
  Coord::init_statics()
  {
    init_move<Coord::TWIST>(Coord::twist_move_, false);
    init_move<Coord::FLIP>(Coord::flip_move_, true);
    init_move<Coord::FR_to_BR>(Coord::FR_to_BR_move_, true);
    init_move<Coord::URF_to_DLF>(Coord::URF_to_DLF_move_, false);
    init_move<Coord::UR_to_DF>(Coord::UR_to_DF_move_, true);
    init_move<Coord::UR_to_UL>(Coord::UR_to_UL_move_, true);
    init_move<Coord::UB_to_DF>(Coord::UB_to_DF_move_, true);

    init_merge_UR_to_UL_and_UB_to_DF();
    init_slice_URF_to_DLF_parity_prune();
    init_slice_UR_to_DF_parity_prune();
    init_slice_twist_prune();
    init_slice_flip_prune();
  }

  void
  Coord::set_pruning(unsigned char arr[], int index, unsigned char value)
  {
    if ((index & 1) == 0)
      arr[index / 2] &= 0xf0 | value;
    else
      arr[index / 2] &= 0x0f | (value << 4);
  }

  unsigned char
  Coord::get_pruning(unsigned char arr[], int index)
  {
    if ((index & 1) == 0)
      return static_cast<unsigned char>(arr[index / 2] & 0x0f);
    else
      return static_cast<unsigned char>((arr[index / 2] & 0xf0) >> 4);
  }

} // namespace cube
