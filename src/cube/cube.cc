#include <cube/cube.hh>

namespace cube
{

  Cube::Cube(std::array<std::array<int, 9>, 6>& cube)
  {
    for (auto i = 0; i < 6; i++)
        cube_[i] = Face(cube[i]);
  }

  Cube::Cube(cube_t& cube)
  : cube_(cube)
  {}

} // namespace cube
