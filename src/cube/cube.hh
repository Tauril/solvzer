#pragma once

#include <array>

#include <cube/face.hh>

namespace cube
{

  /*
  ** Cube faces' locations
  **
  **      0   1   2   3   4   5   6   7   8   9   10  11
  **
  **                    'Up`
  **
  **                `---'---'---`
  **  0             | 0 | 1 | 2 |
  **                `---'---'---`
  **  1             | 3 | 4 | 5 |
  **      `Left`    `---'---'---`   `Right` |   `Back`
  **  2             | 6 | 7 | 8 |
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  3 | 9 | 10| 11| 18| 19| 20| 27| 28| 29| 36| 37| 38|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  4 | 12| 13| 14| 21| 22| 23| 30| 31| 32| 39| 40| 41|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  5 | 15| 16| 17| 24| 25| 26| 33| 34| 35| 42| 43| 44|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  6             | 45| 46| 47|
  **                `---'---'---`
  **  7             | 48| 49| 50|
  **                `---'---'---`
  **  8             | 51| 52| 53|
  **                `---'---'---`
  **
  **                    `Down`
  */

  class Cube
  {
    public:
      /// Cube type
      using cube_t = std::array<Face, 6>;

      Cube() = default;
      Cube(std::array<std::array<int, 9>, 6>& cube);
      Cube(cube_t& cube);

      const cube_t& cube_get() const;
      cube_t& cube_get();

    private:
      cube_t cube_;
  };

} // namespace cube

#include <cube/cube.hxx>
