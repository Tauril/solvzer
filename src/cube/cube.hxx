#pragma once

#include <cstring>

#include <cube/cube.hh>

namespace cube
{

  inline
  bool
  Cube::operator<(const Cube& cube) const
  {
    return std::memcmp(this, &cube, sizeof(cube)) < 0;
  }

  inline
  bool
  Cube::operator==(const Cube& cube) const
  {
    return std::memcmp(this, &cube, sizeof(cube)) == 0;
  }

  inline
  bool
  Cube::operator!=(const Cube& cube) const
  {
    return std::memcmp(this, &cube, sizeof(cube)) != 0;
  }

  inline
  int
  Cube::edge_perm(int cubie_val)
  {
    return cubie_val >> 1;
  }

  inline
  int
  Cube::edge_ori(int cubie_val)
  {
    return cubie_val & 1;
  }

  inline
  int
  Cube::corner_perm(int cubie_val)
  {
    return cubie_val & 7;
  }

  inline
  int
  Cube::corner_ori(int cubie_val)
  {
    return cubie_val >> 3;
  }

  inline
  int
  Cube::edge_flip(int cubie_val)
  {
    return cubie_val ^ 1;
  }

  inline
  int
  Cube::edge_val(int perm, int ori)
  {
    return perm * 2 + ori;
  }

  inline
  int
  Cube::corner_val(int perm, int ori)
  {
    return ori * 8 + perm;
  }

  inline
  int
  Cube::edge_ori_add(int cubie_val1, int cubie_val2)
  {
    return cubie_val1 ^ edge_ori(cubie_val2);
  }

  inline
  int
  Cube::corner_ori_add(int cubie_val1, int cubie_val2)
  {
    return mod_24_[cubie_val1 + (cubie_val2 & 0x18)];
  }

  inline
  int
  Cube::corner_ori_sub(int cubie_val1, int cubie_val2)
  {
    return cubie_val1 + corner_ori_neg_strip_[cubie_val2];
  }

} // namespace cube
