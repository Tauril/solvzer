#include <iostream>

#include <cube/search.hh>

int main()
{
  cube::Search search;

  std::string init_state =
    "RUBFUBFDBUDDFRUUBFDLLFFUFRLUBFUDLRRDBRLFLLBLRLBDRBDRDU";

  auto face = cube::Face(init_state);

  std::cout << face << std::endl;

  auto solution = search.solution(init_state, 21);
  std::cout << "Solution: " << solution << std::endl;

  if (!search.ack_solution(init_state, solution))
    std::cout << "Solution is wrong" << std::endl;

  /*
  std::cout << "============================================================\n";

  std::string moves =
    "U2 R F2 D B' L B R F' L2 U' R' B' R' F' D2 F' L2 D2 L B F2 U' L F2 R' U2 B2 R2 B2";

  auto new_state = "BRDRUDULURLFBRFDDFFFFBFDBRBLFRUDFBBDRDLBLULLDLUUUBLRRU";
  auto sol = search.solution(new_state, 21);
  std::cout << sol << std::endl; */

  return 0;
}
