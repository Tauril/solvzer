#include <cassert>
#include <iostream>

#include <cube/search.hh>

#define DEPTH 21

int main()
{
  cube::Search search;

  /*auto face = cube::Face(init_state);

  std::cout << face << std::endl;

  auto solution = search.solution(init_state, DEPTH);

  if (!search.ack_solution(init_state, solution))
    std::cout << "Solution is wrong" << std::endl; */

  auto face = cube::Face(cube::Cube::solved_state_);
  std::cout << face << std::endl;

  //std::string moves =
  //  "L'D2R1F1R1F2L'F'D'F2L'F'R2B'D'F1D'R1D'F'R2B'F2B2U2L1D1B2U'L'";
  face.scramble();

  auto sol = search.solution(face, DEPTH);
  //assert(search.ack_solution(face, sol));

  return 0;
}
