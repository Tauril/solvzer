#include <cassert>
#include <iostream>

#include <cube/search.hh>

#define DEPTH 21

int main()
{
  cube::Search search;

  /*std::string init_state =
    "RUBFUBFDBUDDFRUUBFDLLFFUFRLUBFUDLRRDBRLFLLBLRLBDRBDRDU";

  auto face = cube::Face(init_state);

  std::cout << face << std::endl;

  auto solution = search.solution(init_state, DEPTH);
  std::cout << "Solution: " << solution << std::endl;

  if (!search.ack_solution(init_state, solution))
    std::cout << "Solution is wrong" << std::endl; */

  /*
  std::cout << "============================================================\n";

  std::string moves =
    "U2 R F2 D B' L B R F' L2 U' R' B' R' F' D2 F' L2 D2 L B F2 U' L F2 R' U2 B2 R2 B2";

  auto new_state = "BRDRUDULURLFBRFDDFFFFBFDBRBLFRUDFBBDRDLBLULLDLUUUBLRRU";
  auto sol = search.solution(new_state, DEPTH);
  std::cout << sol << std::endl; */

  auto face = cube::Face(cube::Cube::solved_state_);
  std::cout << face << std::endl;
  //std::string moves =
  //  "L' D2 R1 F1 R1 F2 L' F' D' F2 L' F' R2 B' D' F1 D' R1 D' F' R2 B' F2 B2 U2 L1 D1 B2 U' L'";
  std::string moves = "U2 B2 L' B1 U2 B2 R1 D2 F1 R2 U2 L' F2 D1 R2 L2 U2 D2 U2 L' U' R2 D2 L' R1 D' B1 R2 L' B'";
  face.scramble(moves);

  auto sol = search.solution(face, DEPTH);
  //assert(search.ack_solution(face, sol));

  return 0;
}
