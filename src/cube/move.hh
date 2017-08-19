/**
 ** \file cube/move.hh
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Declaration of cube::move.
 **/

#pragma once

#include <utility>

#include <cube/cube.hh>

namespace cube
{

  namespace move
  {

    static constexpr std::array<char, 6> axis =
      { 'U', 'R', 'F', 'D', 'L', 'B' };
    static constexpr std::array<char, 3> power =
      { '1', '2', '\'' };

    /// Parses the next move from \æ moves.
    std::pair<int, int> parse_move(const char*& moves);
    /// Applies \a moves on \a solution.
    std::string make_moves(std::string solution, const std::string& moves);

    std::string make_move(const std::string& solution, std::pair<int, int> move);

  } // namespace move

} // namespace cube
