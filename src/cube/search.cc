/**
 ** \file cube/search.cc
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Implementation for cube::Search.
 **/

#include <chrono>
#include <iostream>

#include <cube/search.hh>
#include <misc/state.hh>

namespace cube
{

  namespace
  {

    void
    init_statics()
    {
      static bool init = false;
      if (!init)
      {
        init = true;
        Cube::init_statics();
        Coord::init_statics();
      }
    }

  }

  Search::Search()
  {
    std::cout << "Intializing all statics ..." << std::endl;
    init_statics();
    std::cout << " ... intialization is finished." << std::endl;
  }

  std::string
  Search::solution(const Face& face, int max_depth)
  {
    // If the cube is already solved, nothing to do.
    if (face.face_str_get() == Cube::solved_state_)
      return "";

    Cube cube(face);

    if (cube.verify() != "")
      throw std::runtime_error("The cube is ill formed!");

    Coord coord(cube);

    power_[0] = 0;
    axis_[0] = 0;
    flip_[0] = coord.flip_;
    twist_[0] = coord.twist_;
    parity_[0] = coord.parity_;
    slice_[0] = coord.FR_to_BR_ / Coord::SLICE2;
    URF_to_DLF_[0] = coord.URF_to_DLF_;
    FR_to_BR_[0] = coord.FR_to_BR_;
    UR_to_UL_[0] = coord.UR_to_UL_;
    UB_to_DF_[0] = coord.UB_to_DF_;
    min_depth_phase1_[1] = 1;

    int move = 0;
    int n = 0;
    bool busy = false;
    int depth_phase1 = 1;

    auto start = std::chrono::system_clock::now();

    do {
      do {
        if ((depth_phase1 - n > min_depth_phase1_[n + 1]) && !busy)
        {
          if (axis_[n] == 0 || axis_[n] == 3)
            axis_[++n] = 1;
          else
            axis_[++n] = 0;
          power_[n] = 1;
        }
        else if (++power_[n] > 3)
        {
          do {
            if (++axis_[n] > 5)
            {
              if (n == 0)
              {
                if (depth_phase1 >= max_depth)
                  throw std::runtime_error("max depth exceeded!");
                else
                {
                  depth_phase1++;
                  axis_[n] = 0;
                  power_[n] = 1;
                  busy = false;
                  break;
                }
              }
              else
              {
                n--;
                busy = true;
                break;
              }
            }
            else
            {
              power_[n] = 1;
              busy = false;
            }
          } while (n != 0
                   && (axis_[n - 1] == axis_[n]
                       || axis_[n - 1] - 3 == axis_[n]));
        }
        else
          busy = false;
      } while (busy);

      move = 3 * axis_[n] + power_[n] - 1;
      flip_[n + 1] = Coord::flip_move_[flip_[n]][move];
      twist_[n + 1] = Coord::twist_move_[twist_[n]][move];
      slice_[n + 1] = Coord::FR_to_BR_move_[slice_[n] * 24][move] / 24;
      min_depth_phase1_[n + 1] =
        std::max(
          Coord::get_pruning(Coord::slice_flip_prune_,
                             Coord::SLICE1 * flip_[n + 1] + slice_[n + 1]),
          Coord::get_pruning(Coord::slice_twist_prune_,
                             Coord::SLICE1 * twist_[n + 1] + slice_[n + 1])
        );

      if (min_depth_phase1_[n + 1] == 0 && n >= depth_phase1 - 5)
      {
        min_depth_phase1_[n + 1] = 10;
        int depth = 0;

        if (n == depth_phase1 - 1
            && (depth = total_depth(depth_phase1, max_depth)) >= 0)
          if (depth == depth_phase1
              || (axis_[depth_phase1 - 1] != axis_[depth_phase1]
                  && axis_[depth_phase1 - 1] != axis_[depth_phase1] + 3))
          {
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> dt = end - start;

            std::cout << "Computation time: "
                      << dt.count() << "s" << std::endl;
            state::State::Instance().compute_time_set(dt.count());

            return solution_to_string(depth);
          }
      }
    } while (true);

    return 0;
  }

  std::string
  Search::solution(const std::string& facelets, int max_depth)
  {
    Face face(facelets);
    return solution(face, max_depth);
  }

  int
  Search::total_depth(int depth_phase1, int max_depth)
  {
    int move = 0;
    int depth1 = 0;
    int depth2 = 0;
    int max_depth_phase2 = std::min(10, max_depth - depth_phase1);

    for (int i = 0; i < depth_phase1; i++)
    {
      move = 3 * axis_[i] + power_[i] - 1;
      URF_to_DLF_[i + 1] = Coord::URF_to_DLF_move_[URF_to_DLF_[i]][move];
      FR_to_BR_[i + 1] = Coord::FR_to_BR_move_[FR_to_BR_[i]][move];
      parity_[i + 1] = Coord::parity_move_[parity_[i]][move];
    }

    if ((depth1 = Coord::get_pruning(Coord::slice_URF_to_DLF_parity_prune_,
        (Coord::SLICE2 * URF_to_DLF_[depth_phase1] + FR_to_BR_[depth_phase1])
        * 2 + parity_[depth_phase1])) > max_depth_phase2)
      return -1;

    for (int i = 0; i < depth_phase1; i++)
    {
      move = 3 * axis_[i] + power_[i] - 1;
      UR_to_UL_[i + 1] = Coord::UR_to_UL_move_[UR_to_UL_[i]][move];
      UB_to_DF_[i + 1] = Coord::UB_to_DF_move_[UB_to_DF_[i]][move];
    }

    UR_to_DF_[depth_phase1] =
      Coord::merge_UR_to_UL_and_UB_to_DF_[UR_to_UL_[depth_phase1]]
                                         [UB_to_DF_[depth_phase1]];

    if ((depth2 = Coord::get_pruning(Coord::slice_UR_to_DF_parity_prune_,
        (Coord::SLICE2 * UR_to_DF_[depth_phase1] + FR_to_BR_[depth_phase1])
        * 2 + parity_[depth_phase1])) > max_depth_phase2)
      return -1;

    if ((min_depth_phase2_[depth_phase1] = std::max(depth1, depth2)) == 0)
      return depth_phase1;

    // Set up search.

    int depth_phase2 = 1;
    int n = depth_phase1;
    bool busy = false;
    power_[depth_phase1] = 0;
    axis_[depth_phase1] = 0;
    min_depth_phase2_[n + 1] = 1;

    do {
      do {
        if ((depth_phase1 + depth_phase2 - n > min_depth_phase2_[n + 1])
            && !busy)
        {
          if (axis_[n] == 0 || axis_[n] == 3)
          {
            axis_[++n] = 1;
            power_[n] = 2;
          }
          else
          {
            axis_[++n] = 0;
            power_[n] = 1;
          }
        }
        else if ((axis_[n] == 0 || axis_[n] == 3)
            ? (++power_[n] > 3)
            : ((power_[n] = power_[n] + 2) > 3))
        {
          do {
            if (++axis_[n] > 5)
            {
              if (n == depth_phase1)
              {
                if (depth_phase2 >= max_depth_phase2)
                  return -1;
                else
                {
                  depth_phase2++;
                  axis_[n] = 0;
                  power_[n] = 1;
                  busy = false;
                  break;
                }
              }
              else
              {
                n--;
                busy = true;
                break;
              }
            }
            else
            {
              if (axis_[n] == 0 || axis_[n] == 3)
                power_[n] = 1;
              else
                power_[n] = 2;
              busy = false;
            }
          } while (n != depth_phase1
                   && (axis_[n - 1] == axis_[n]
                       || axis_[n - 1] - 3 == axis_[n]));
        }
        else
          busy = false;
      } while (busy);

      move = 3 * axis_[n] + power_[n] - 1;
      URF_to_DLF_[n + 1] = Coord::URF_to_DLF_move_[URF_to_DLF_[n]][move];
      FR_to_BR_[n + 1] = Coord::FR_to_BR_move_[FR_to_BR_[n]][move];
      parity_[n + 1] = Coord::parity_move_[parity_[n]][move];
      UR_to_DF_[n + 1] = Coord::UR_to_DF_move_[UR_to_DF_[n]][move];

      min_depth_phase2_[n + 1] =
        std::max(
          Coord::get_pruning(Coord::slice_UR_to_DF_parity_prune_,
                            (Coord::SLICE2
                             * UR_to_DF_[n + 1] + FR_to_BR_[n + 1])
                             * 2 + parity_[n + 1]),
          Coord::get_pruning(Coord::slice_URF_to_DLF_parity_prune_,
                            (Coord::SLICE2
                             * URF_to_DLF_[n + 1] + FR_to_BR_[n + 1])
                             * 2 + parity_[n + 1]));
    } while (min_depth_phase2_[n + 1] != 0);

    return depth_phase1 + depth_phase2;
  }

  std::string
  Search::solution_to_string(int length)
  {
    std::string ret = "";

    static const std::unordered_map<int, std::string> axes =
    {
      { 0, "U" }, { 1, "R" }, { 2, "F" }, { 3, "D" }, { 4, "L" }, { 5, "B" }
    };
    static const std::unordered_map<int, std::string> powers =
    {
      { 1, "" }, { 2, "2" }, { 3, "'" }
    };

    for (int i = 0; i < length; i++)
    {
      ret += axes.at(axis_[i]);
      ret += powers.at(power_[i]);

      if (i < length - 1)
        ret += " ";
    }

    std::cout << "Solution: " << ret << std::endl;

    return ret;
  }

  bool
  Search::ack_solution(const std::string& state, const std::string& solution)
  {
    return move::make_moves(state, solution) == Cube::solved_state_;
  }

  bool
  Search::ack_solution(const Face& face, const std::string& solution)
  {
    return ack_solution(face.face_str_get(), solution);
  }

} // namespace cube
