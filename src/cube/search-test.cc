#include <cassert>

#include <cube/search.hh>

void
search_check()
{
  cube::Search search;

  for (int i = 0; i < 200; i++)
  {
    auto face = cube::Face(cube::Cube::solved_state_);
    face.scramble();
    auto solution = search.solution(face, 21);

    assert(search.ack_solution(face, solution));
  }
}

int
main()
{
  search_check();

  return 0;
}
