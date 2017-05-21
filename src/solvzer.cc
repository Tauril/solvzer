#include <iostream>

#include <cube/search.hh>

int main()
{
  std::string init_state =
    "RUBFUBFDBUDDFRUUBFDLLFFUFRLUBFUDLRRDBRLFLLBLRLBDRBDRDU";

  auto face = cube::Face(init_state);

  std::cout << face << std::endl;

  cube::Search search;
  auto solved_state = search.solution(init_state, 24);

  auto sol = cube::Face(solved_state);
  std::cout << sol << std::endl;

  return 0;
}
