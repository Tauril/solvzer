#pragma once

#include <utility>

#include <cube/cube.hh>

namespace cube
{

  namespace move
  {

    std::pair<int, int> parse_move(const char*& moves);
    std::string make_move(const std::string& solution, const int* move);
    std::string make_move(const std::string& solution,
                          std::pair<int, int> move);
    std::string make_moves(std::string solution, const std::string& moves);

  }

} // namespace cube