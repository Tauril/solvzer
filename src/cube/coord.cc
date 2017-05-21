#include <cube/coord.hh>

namespace cube
{

  constexpr std::array<std::array<int, 18>, 2> Coord::parity_move_;

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
    Coord::slice_URF_to_DLF_parity_prune_[
      Coord::SLICE2 * Coord::URF_to_DLF * Coord::PARITY / 2
    ];
  unsigned char
    Coord::slice_UR_to_DF_parity_prune_[
      Coord::SLICE2 * Coord::UR_to_DF * Coord::PARITY / 2
    ];
  unsigned char
    Coord::slice_twist_prune_[Coord::SLICE1 * Coord::TWIST / 2 + 1];
  unsigned char
    Coord::slice_flip_prune_[Coord::SLICE1 * Coord::FLIP / 2];

  namespace
  {

    void
    init_twist_move()
    {
      Cube c;
      for (int i = 0; i < Coord::TWIST; i++)
      {
        c.set_twist(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            c.corner_multiply(Cube::move_cube_[j]);
            Coord::twist_move_[i][3 * j + k] = c.get_twist();
          }
          c.corner_multiply(Cube::move_cube_[j]);
        }
      }
    }

    void
    init_flip_move()
    {
      Cube c;
      for (int i = 0; i < Coord::FLIP; i++)
      {
        c.set_flip(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            c.edge_multiply(Cube::move_cube_[j]);
            Coord::flip_move_[i][3 * j + k] = c.get_flip();
          }
          c.edge_multiply(Cube::move_cube_[j]);
        }
      }
    }

    void
    init_FR_to_BR_move()
    {
      Cube c;
      for (int i = 0; i < Coord::FR_to_BR; i++)
      {
        c.set_FR_to_BR(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            c.edge_multiply(Cube::move_cube_[j]);
            Coord::FR_to_BR_move_[i][3 * j + k] = c.get_FR_to_BR();
          }
          c.edge_multiply(Cube::move_cube_[j]);
        }
      }
    }

    void
    init_URF_to_DLF_move()
    {
      Cube c;
      for (int i = 0; i < Coord::URF_to_DLF; i++)
      {
        c.set_URF_to_DLF(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            c.corner_multiply(Cube::move_cube_[j]);
            Coord::URF_to_DLF_move_[i][3 * j + k] = c.get_URF_to_DLF();
          }
          c.corner_multiply(Cube::move_cube_[j]);
        }
      }
    }

    void
    init_UR_to_DF_move()
    {
      Cube c;
      for (int i = 0; i < Coord::UR_to_DF; i++)
      {
        c.set_UR_to_DF(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            c.edge_multiply(Cube::move_cube_[j]);
            Coord::UR_to_DF_move_[i][3 * j + k] = (short)c.get_UR_to_DF();
          }
          c.edge_multiply(Cube::move_cube_[j]);
        }
      }
    }

    void
    init_UR_to_UL_move()
    {
      Cube c;
      for (int i = 0; i < Coord::UR_to_UL; i++)
      {
        c.set_UR_to_UL(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            c.edge_multiply(Cube::move_cube_[j]);
            Coord::UR_to_UL_move_[i][3 * j + k] = c.get_UR_to_UL();
          }
          c.edge_multiply(Cube::move_cube_[j]);
        }
      }
    }

    void
    init_UB_to_DF_move()
    {
      Cube c;
      for (int i = 0; i < Coord::UB_to_DF; i++)
      {
        c.set_UB_to_DF(i);
        for (int j = 0; j < 6; j++)
        {
          for (int k = 0; k < 3; k++)
          {
            c.edge_multiply(Cube::move_cube_[j]);
            Coord::UB_to_DF_move_[i][3 * j + k] = c.get_UB_to_DF();
          }
          c.edge_multiply(Cube::move_cube_[j]);
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
      for (int i = 0;
           i < Coord::SLICE2 * Coord::URF_to_DLF * Coord::PARITY / 2; i++)
        Coord::slice_URF_to_DLF_parity_prune_[i] = -1;

      int depth = 0;
      Coord::set_pruning(Coord::slice_URF_to_DLF_parity_prune_, 0,
                         static_cast<unsigned char>(0));

      int done = 1;
      while (done != Coord::SLICE2 * Coord::URF_to_DLF * Coord::PARITY)
      {
        for (int i = 0;
             i < Coord::SLICE2 * Coord::URF_to_DLF * Coord::PARITY; i++)
        {
          int parity = i % 2;
          int URF_to_DLF = (i / 2) / Coord::SLICE2;
          int slice = (i / 2) % Coord::SLICE2;

          if (Coord::get_pruning(Coord::slice_URF_to_DLF_parity_prune_, i)
              == depth)
          {
            for (int j = 0; j < 18; j++)
            {
              switch (j)
              {
                case 3:
                case 5:
                case 6:
                case 8:
                case 12:
                case 14:
                case 15:
                case 17:
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
      for (int i = 0;
           i < Coord::SLICE2 * Coord::UR_to_DF * Coord::PARITY / 2; i++)
        Coord::slice_UR_to_DF_parity_prune_[i] = -1;

      int depth = 0;
      Coord::set_pruning(Coord::slice_UR_to_DF_parity_prune_, 0,
                         static_cast<unsigned char>(0));

      int done = 1;
      while (done != Coord::SLICE2 * Coord::UR_to_DF * Coord::PARITY)
      {
        for (int i = 0;
             i < Coord::SLICE2 * Coord::UR_to_DF * Coord::PARITY; i++)
        {
          int parity = i % 2;
          int UR_to_DF = (i / 2) / Coord::SLICE2;
          int slice = (i / 2) % Coord::SLICE2;

          if (Coord::get_pruning(Coord::slice_UR_to_DF_parity_prune_, i)
              == depth)
          {
            for (int j = 0; j < 18; j++)
            {
              switch (j)
              {
                case 3:
                case 5:
                case 6:
                case 8:
                case 12:
                case 14:
                case 15:
                case 17:
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

      int depth = 0;
      Coord::set_pruning(Coord::slice_twist_prune_, 0,
                         static_cast<unsigned char>(0));

      int done = 1;
      while (done != Coord::SLICE1 * Coord::TWIST)
      {
        for (int i = 0; i < Coord::SLICE1 * Coord::TWIST; i++)
        {
          int twist = i / Coord::SLICE1;
          int slice = i % Coord::SLICE1;

          if (Coord::get_pruning(Coord::slice_twist_prune_, i) == depth)
          {
            for (int j = 0; j < 18; j++)
            {
              int new_slice = Coord::FR_to_BR_move_[slice * 24][j] / 24;
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

      int depth = 0;
      Coord::set_pruning(Coord::slice_flip_prune_, 0,
                         static_cast<unsigned char>(0));

      int done = 1;
      while (done != Coord::SLICE1 * Coord::FLIP)
      {
        for (int i = 0; i < Coord::SLICE1 * Coord::FLIP; i++)
        {
          int flip = i / Coord::SLICE1;
          int slice = i % Coord::SLICE1;

          if (Coord::get_pruning(Coord::slice_flip_prune_, i) == depth)
          {
            for (int j = 0; j < 18; j++)
            {
              int new_slice = Coord::FR_to_BR_move_[slice * 24][j] / 24;
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
    init_twist_move();
    init_flip_move();
    init_FR_to_BR_move();
    init_URF_to_DLF_move();
    init_UR_to_DF_move();
    init_UR_to_UL_move();
    init_UB_to_DF_move();
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
