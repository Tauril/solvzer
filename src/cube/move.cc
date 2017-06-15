/**
 ** \file cube/move.cc
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Implementation for cube/move.hh
 **/

#include <array>

#include <cube/move.hh>
#include <misc/display.hh>
namespace cube
{

  namespace move
  {

    namespace
    {

      static constexpr int U1_[54] =
      {
        U7, U4, U1, U8, U5, U2, U9, U6, U3,
        B1, B2, B3, R4, R5, R6, R7, R8, R9,
        R1, R2, R3, F4, F5, F6, F7, F8, F9,
        D1, D2, D3, D4, D5, D6, D7, D8, D9,
        F1, F2, F3, L4, L5, L6, L7, L8, L9,
        L1, L2, L3, B4, B5, B6, B7, B8, B9
      };

      static constexpr int U2_[54] =
      {
        U9, U8, U7, U6, U5, U4, U3, U2, U1,
        L1, L2, L3, R4, R5, R6, R7, R8, R9,
        B1, B2, B3, F4, F5, F6, F7, F8, F9,
        D1, D2, D3, D4, D5, D6, D7, D8, D9,
        R1, R2, R3, L4, L5, L6, L7, L8, L9,
        F1, F2, F3, B4, B5, B6, B7, B8, B9
      };

      static constexpr int U3_[54] =
      {
        U3, U6, U9, U2, U5, U8, U1, U4, U7,
        F1, F2, F3, R4, R5, R6, R7, R8, R9,
        L1, L2, L3, F4, F5, F6, F7, F8, F9,
        D1, D2, D3, D4, D5, D6, D7, D8, D9,
        B1, B2, B3, L4, L5, L6, L7, L8, L9,
        R1, R2, R3, B4, B5, B6, B7, B8, B9
      };

      static constexpr int R1_[54] =
      {
        U1, U2, F3, U4, U5, F6, U7, U8, F9,
        R7, R4, R1, R8, R5, R2, R9, R6, R3,
        F1, F2, D3, F4, F5, D6, F7, F8, D9,
        D1, D2, B7, D4, D5, B4, D7, D8, B1,
        L1, L2, L3, L4, L5, L6, L7, L8, L9,
        U9, B2, B3, U6, B5, B6, U3, B8, B9
      };

      static constexpr int R2_[54] =
      {
        U1, U2, D3, U4, U5, D6, U7, U8, D9,
        R9, R8, R7, R6, R5, R4, R3, R2, R1,
        F1, F2, B7, F4, F5, B4, F7, F8, B1,
        D1, D2, U3, D4, D5, U6, D7, D8, U9,
        L1, L2, L3, L4, L5, L6, L7, L8, L9,
        F9, B2, B3, F6, B5, B6, F3, B8, B9
      };

      static constexpr int R3_[54] =
      {
        U1, U2, B7, U4, U5, B4, U7, U8, B1,
        R3, R6, R9, R2, R5, R8, R1, R4, R7,
        F1, F2, U3, F4, F5, U6, F7, F8, U9,
        D1, D2, F3, D4, D5, F6, D7, D8, F9,
        L1, L2, L3, L4, L5, L6, L7, L8, L9,
        D9, B2, B3, D6, B5, B6, D3, B8, B9
      };

      static constexpr int F1_[54] =
      {
        U1, U2, U3, U4, U5, U6, L9, L6, L3,
        U7, R2, R3, U8, R5, R6, U9, R8, R9,
        F7, F4, F1, F8, F5, F2, F9, F6, F3,
        R7, R4, R1, D4, D5, D6, D7, D8, D9,
        L1, L2, D1, L4, L5, D2, L7, L8, D3,
        B1, B2, B3, B4, B5, B6, B7, B8, B9
      };

      static constexpr int F2_[54] =
      {
        U1, U2, U3, U4, U5, U6, D3, D2, D1,
        L9, R2, R3, L6, R5, R6, L3, R8, R9,
        F9, F8, F7, F6, F5, F4, F3, F2, F1,
        U9, U8, U7, D4, D5, D6, D7, D8, D9,
        L1, L2, R7, L4, L5, R4, L7, L8, R1,
        B1, B2, B3, B4, B5, B6, B7, B8, B9
      };

      static constexpr int F3_[54] =
      {
        U1, U2, U3, U4, U5, U6, R1, R4, R7,
        D3, R2, R3, D2, R5, R6, D1, R8, R9,
        F3, F6, F9, F2, F5, F8, F1, F4, F7,
        L3, L6, L9, D4, D5, D6, D7, D8, D9,
        L1, L2, U9, L4, L5, U8, L7, L8, U7,
        B1, B2, B3, B4, B5, B6, B7, B8, B9
      };

      static constexpr int D1_[54] =
      {
        U1, U2, U3, U4, U5, U6, U7, U8, U9,
        R1, R2, R3, R4, R5, R6, F7, F8, F9,
        F1, F2, F3, F4, F5, F6, L7, L8, L9,
        D7, D4, D1, D8, D5, D2, D9, D6, D3,
        L1, L2, L3, L4, L5, L6, B7, B8, B9,
        B1, B2, B3, B4, B5, B6, R7, R8, R9
      };

      static constexpr int D2_[54] =
      {
        U1, U2, U3, U4, U5, U6, U7, U8, U9,
        R1, R2, R3, R4, R5, R6, L7, L8, L9,
        F1, F2, F3, F4, F5, F6, B7, B8, B9,
        D9, D8, D7, D6, D5, D4, D3, D2, D1,
        L1, L2, L3, L4, L5, L6, R7, R8, R9,
        B1, B2, B3, B4, B5, B6, F7, F8, F9
      };

      static constexpr int D3_[54] =
      {
        U1, U2, U3, U4, U5, U6, U7, U8, U9,
        R1, R2, R3, R4, R5, R6, B7, B8, B9,
        F1, F2, F3, F4, F5, F6, R7, R8, R9,
        D3, D6, D9, D2, D5, D8, D1, D4, D7,
        L1, L2, L3, L4, L5, L6, F7, F8, F9,
        B1, B2, B3, B4, B5, B6, L7, L8, L9
      };

      static constexpr int L1_[54] =
      {
        B9, U2, U3, B6, U5, U6, B3, U8, U9,
        R1, R2, R3, R4, R5, R6, R7, R8, R9,
        U1, F2, F3, U4, F5, F6, U7, F8, F9,
        F1, D2, D3, F4, D5, D6, F7, D8, D9,
        L7, L4, L1, L8, L5, L2, L9, L6, L3,
        B1, B2, D7, B4, B5, D4, B7, B8, D1
      };

      static constexpr int L2_[54] =
      {
        D1, U2, U3, D4, U5, U6, D7, U8, U9,
        R1, R2, R3, R4, R5, R6, R7, R8, R9,
        B9, F2, F3, B6, F5, F6, B3, F8, F9,
        U1, D2, D3, U4, D5, D6, U7, D8, D9,
        L9, L8, L7, L6, L5, L4, L3, L2, L1,
        B1, B2, F7, B4, B5, F4, B7, B8, F1
      };

      static constexpr int L3_[54] =
      {
        F1, U2, U3, F4, U5, U6, F7, U8, U9,
        R1, R2, R3, R4, R5, R6, R7, R8, R9,
        D1, F2, F3, D4, F5, F6, D7, F8, F9,
        B9, D2, D3, B6, D5, D6, B3, D8, D9,
        L3, L6, L9, L2, L5, L8, L1, L4, L7,
        B1, B2, U7, B4, B5, U4, B7, B8, U1
      };

      static constexpr int B1_[54] =
      {
        R3, R6, R9, U4, U5, U6, U7, U8, U9,
        R1, R2, D9, R4, R5, D8, R7, R8, D7,
        F1, F2, F3, F4, F5, F6, F7, F8, F9,
        D1, D2, D3, D4, D5, D6, L1, L4, L7,
        U3, L2, L3, U2, L5, L6, U1, L8, L9,
        B7, B4, B1, B8, B5, B2, B9, B6, B3
      };

      static constexpr int B2_[54] =
      {
        D9, D8, D7, U4, U5, U6, U7, U8, U9,
        R1, R2, L7, R4, R5, L4, R7, R8, L1,
        F1, F2, F3, F4, F5, F6, F7, F8, F9,
        D1, D2, D3, D4, D5, D6, U3, U2, U1,
        R9, L2, L3, R6, L5, L6, R3, L8, L9,
        B9, B8, B7, B6, B5, B4, B3, B2, B1
      };

      static constexpr int B3_[54] =
      {
        L7, L4, L1, U4, U5, U6, U7, U8, U9,
        R1, R2, U1, R4, R5, U2, R7, R8, U3,
        F1, F2, F3, F4, F5, F6, F7, F8, F9,
        D1, D2, D3, D4, D5, D6, R9, R6, R3,
        D7, L2, L3, D8, L5, L6, D9, L8, L9,
        B3, B6, B9, B2, B5, B8, B1, B4, B7
      };

      static const int* const moves_table_[6][4] =
      {
        { nullptr, U1_, U2_, U3_ },
        { nullptr, R1_, R2_, R3_ },
        { nullptr, F1_, F2_, F3_ },
        { nullptr, D1_, D2_, D3_ },
        { nullptr, L1_, L2_, L3_ },
        { nullptr, B1_, B2_, B3_ }
      };

      std::string
      make_move(const std::string& solution, const int* move)
      {
        std::string ret(54, ' ');

        for (int i = 0; i < 54; i++)
          ret[i] = solution[move[i]];

        return ret;
      }

      std::string
      make_move(const std::string& solution, std::pair<int, int> move)
      {
        return make_move(solution, moves_table_[move.first][move.second]);
      }

    }

    std::pair<int, int>
    parse_move(const char*& moves)
    {
      while (std::isspace(*moves))
        moves++;
      char move = *moves++;
      int face = get_color(move);
      int times = 0;

      std::cout << move;

      switch (*moves)
      {
        case '2':
          times = 2;
          std::cout << *moves;
          moves++;
          break;
        case '\'':
          times = 3;
          std::cout << *moves;
          moves++;
          break;
        case '1':
          moves++;
        default:
          times = 1;
          break;
      }

      std::cout << std::endl;

      while (std::isspace(*moves))
        moves++;

      return std::pair<int, int>(face, times);
    }

    std::string
    make_moves(std::string solution, const std::string& moves)
    {
      const char* sol_str = moves.c_str();

      while (*sol_str)
      {
        std::cout << "Move: ";
        const auto& move = parse_move(sol_str);
        solution = make_move(solution, move);

        auto face = Face(solution);
        display::Display::Instance().draw_rubiks(solution);
        std::cout << "State: " << solution << std::endl;
        std::cout << "Visual representation:" << std::endl
                  << face << std::endl;
      }

      return solution;
    }

  } // namespace move

} // namespace cube
