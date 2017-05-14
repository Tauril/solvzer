#pragma once

#include <cube/cube.hh>

namespace cube
{

  inline
  const Cube::cube_t&
  Cube::cube_get() const
  {
    return cube_;
  }

  inline
  Cube::cube_t&
  Cube::cube_get()
  {
    return cube_;
  }

} // namespace cube
