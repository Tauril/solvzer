#pragma once

#include <cube/cube.hh>

namespace cube
{

  namespace
  {

    /// T can either be a corner or an edge.
    template <typename T>
    void
    rotate_left(T* arr, int l, int r)
    {
      T tmp = arr[l];

      for (int i = l; i < r; i++)
        arr[i] = arr[i + 1];
      arr[r] = tmp;
    }

    /// T can either be a corner or an edge.
    template <typename T>
    void
    rotate_right(T* arr, int l, int r)
    {
      T tmp = arr[r];

      for (int i = r; i > l; i--)
        arr[i] = arr[i - 1];
      arr[l] = tmp;
    }

    int
    Cnk(int n, int k)
    {
      if (n < k)
        return 0;

      if (k > n / 2)
        k = n - k;

      int i = 0;
      int j = 0;
      int s = 0;

      for (s = 1, i = n, j = 1; i != n - k; i--, j++)
      {
        s *= i;
        s /= j;
      }

      return s;
    }

  }

  inline
  int
  Cube::get_twist() const
  {
    int ret = 0;
    for (int i = URF; i < DRB; i++)
      ret = static_cast<int>(3 * ret + corner_ori_[i]);
    return ret;
  }

  inline
  void
  Cube::set_twist(int twist)
  {
    int twist_parity = 0;
    for (int i = DRB - 1; i >= URF; i--)
    {
      twist_parity += (corner_ori_[i] = static_cast<unsigned char>(twist % 3));
      twist /= 3;
    }

    corner_ori_[DRB] = static_cast<unsigned char>((3 - twist_parity % 3) % 3);
  }

  inline
  int
  Cube::get_flip() const
  {
    int ret = 0;
    for (int i = UR; i < BR; i++)
      ret = static_cast<int>(2 * ret + edge_ori_[i]);
    return ret;
  }

  inline
  void
  Cube::set_flip(int flip)
  {
    int flip_parity = 0;
    for (int i = BR - 1; i >= UR; i--)
    {
      flip_parity += (edge_ori_[i] = static_cast<unsigned char>(flip % 2));
      flip /= 2;
    }

    edge_ori_[BR] = static_cast<unsigned char>((2 - flip_parity % 2) % 2);
  }

  inline
  int
  Cube::corner_parity() const
  {
    int p = 0;
    for (int i = DRB; i >= URF + 1; i--)
      for (int j = i - 1; j >= URF; j--)
        if (corner_perm_[j] > corner_perm_[i])
          p++;

    return static_cast<int>(p % 2);
  }

  inline
  int
  Cube::edge_parity() const
  {
    int p = 0;
    for (int i = BR; i >= UR + 1; i--)
      for (int j = i - 1; j >= UR; j--)
        if (edge_perm_[j] > edge_perm_[i])
          p++;

    return static_cast<int>(p % 2);
  }

  inline
  int
  Cube::get_FR_to_BR() const
  {
    int i = 0;
    int j = 0;
    edge edge[4];

    // Compute the index i < (12 choose 4) and the permutation array.
    for (int k = BR; k >= UR; k--)
    {
      if (FR <= edge_perm_[k] && edge_perm_[k] <= BR)
      {
        i += Cnk(11 - k, j + 1);
        edge[3 - j++] = edge_perm_[k];
      }
    }

    j = 0;

    // Compute the index b < 4! for the permutation in perm
    for (int k = 3; k > 0; k--)
    {
      int l = 0;
      while (edge[k] != k + 8)
      {
        rotate_left(edge, 0, k);
        l++;
      }

      j = (k + 1) * j + l;
    }

    return static_cast<int>(24 * i + j);
  }

  inline
  void
  Cube::set_FR_to_BR(int id)
  {
    for (edge e : edges)
      edge_perm_[e] = DB;

    int i = id % 24;  // Permutation
    edge slice_edge[4] = { FR, FL, BL, BR };
    edge other_edge[8] = { UR, UF, UL, UB, DR, DF, DL, DB };

    // Generate permutation from index i
    for (int j = 1, k = 0; j < 4; j++)
    {
      k = i % (j + 1);
      i /= j + 1;

      while (k-- > 0)
        rotate_right(slice_edge, 0, j);
    }

    int j = 3;
    int k = id / 24;  // Combination

    for (int l = UR; l <= BR; l++)
    {
      if (k - Cnk(11 - l, j + 1) >= 0)
      {
        edge_perm_[l] = slice_edge[3 - j];
        k -= Cnk(11 - l, j-- + 1);
      }
    }

    j = 0;
    for (int l = UR; l <= BR; l++)
      if (edge_perm_[l] == DB)
        edge_perm_[l] = other_edge[j++];
  }

  inline
  int
  Cube::get_URF_to_DLF() const
  {
    int i = 0;
    int j = 0;
    corner corner[6];

    // Compute the index i < (8 choose 6) and the corner permutation.
    for (int k = URF; k <= DRB; k++)
    {
      if (corner_perm_[k] <= DLF)
      {
        i += Cnk(k, j + 1);
        corner[j++] = corner_perm_[k];
      }
    }

    j = 0;

    // Compute the index b < 6! for the permutation in corner
    for (int k = 5; k > 0; k--)
    {
      int l = 0;
      while (corner[k] != k)
      {
        rotate_left(corner, 0, k);
        l++;
      }

      j = (k + 1) * j + l;
    }

    return static_cast<int>(720 * i + j);
  }

  inline
  void
  Cube::set_URF_to_DLF(int id)
  {
    for (corner c : corners)
      corner_perm_[c] = DRB;

    corner slice_corner[6] = { URF, UFL, ULB, UBR, DFR, DLF };
    corner other_corner[2] = { DBL, DRB };

    int i = id % 720; // Permutation

    // Generate permutation from index i
    for (int j = 1, k = 0; j < 6; j++)
    {
      k = i % (j + 1);
      i /= j + 1;

      while (k-- > 0)
        rotate_right(slice_corner, 0, j);
    }

    int j = 5;
    int k = id / 720;  // Combination

    for (int l = DRB; l >= URF; l--)
    {
      if (k - Cnk(l, j + 1) >= 0)
      {
        corner_perm_[l] = slice_corner[j];
        k -= Cnk(l, j-- + 1);
      }
    }

    j = 0;
    for (int l = URF; l <= DRB; l++)
      if (corner_perm_[l] == DRB)
        corner_perm_[l] = other_corner[j++];
  }

  inline
  int
  Cube::get_UR_to_DF() const
  {
    int i = 0;
    int j = 0;
    edge edge[6];

    // Compute the index i < (12 choose 6) and the edge permutation.
    for (int k = UR; k <= BR; k++)
    {
      if (edge_perm_[k] <= DF)
      {
        i += Cnk(k, j + 1);
        edge[j++] = edge_perm_[k];
      }
    }

    j = 0;

    // Compute the index b < 6! for the permutation in edge
    for (int k = 5; k > 0; k--)
    {
      int l = 0;
      while (edge[k] != k)
      {
        rotate_left(edge, 0, k);
        l++;
      }

      j = (k + 1) * j + l;
    }

    return static_cast<int>(720 * i + j);
  }

  inline
  int
  Cube::get_UR_to_DF(int id1, int id2)
  {
    Cube c1;
    Cube c2;

    c1.set_UR_to_UL(id1);
    c2.set_UB_to_DF(id2);

    for (int i = 0; i < 8; i++)
    {
      if (c1.edge_perm_[i] != BR)
      {
        if (c2.edge_perm_[i] != BR)
          return -1;
        else
          c2.edge_perm_[i] = c1.edge_perm_[i];
      }
    }

    return c2.get_UR_to_DF();
  }

  inline
  void
  Cube::set_UR_to_DF(int id)
  {
    for (edge e : edges)
      edge_perm_[e] = BR;

    edge slice_edge[6] = { UR, UF, UL, UB, DR, DF };
    edge other_edge[6] = { DL, DB, FR, FL, BL, BR };

    int i = id % 720; // Permutation

    // Generate permutation from index i
    for (int j = 1, k = 0; j < 6; j++)
    {
      k = i % (j + 1);
      i /= j + 1;

      while (k-- > 0)
        rotate_right(slice_edge, 0, j);
    }

    int j = 5;
    int k = id / 720;  // Combination

    for (int l = BR; l >= UR; l--)
    {
      if (k - Cnk(l, j + 1) >= 0)
      {
        edge_perm_[l] = slice_edge[j];
        k -= Cnk(l, j-- + 1);
      }
    }

    j = 0;
    for (int l = UR; l <= BR; l++)
      if (edge_perm_[l] == BR)
        edge_perm_[l] = other_edge[j++];
  }

  inline
  int
  Cube::get_UR_to_UL() const
  {
    int i = 0;
    int j = 0;
    edge edge[3];

    // Compute the index i < (12 choose 3) and the edge permutation.
    for (int k = UR; k <= BR; k++)
    {
      if (edge_perm_[k] <= UL)
      {
        i += Cnk(k, j + 1);
        edge[j++] = edge_perm_[k];
      }
    }

    j = 0;

    // Compute the index b < 3! for the permutation in edge
    for (int k = 2; k > 0; k--)
    {
      int l = 0;
      while (edge[k] != k)
      {
        rotate_left(edge, 0, k);
        l++;
      }

      j = (k + 1) * j + l;
    }

    return static_cast<int>(6 * i + j);
  }

  inline
  void
  Cube::set_UR_to_UL(int id)
  {
    for (edge e : edges)
      edge_perm_[e] = BR;

    edge slice_edge[3] = { UR, UF, UL };

    int i = id % 6; // Permutation

    // Generate permutation from index i
    for (int j = 1, k = 0; j < 3; j++)
    {
      k = i % (j + 1);
      i /= j + 1;

      while (k-- > 0)
        rotate_right(slice_edge, 0, j);
    }

    int j = 2;
    int k = id / 6;  // Combination

    for (int l = BR; l >= UR; l--)
    {
      if (k - Cnk(l, j + 1) >= 0)
      {
        edge_perm_[l] = slice_edge[j];
        k -= Cnk(l, j-- + 1);
      }
    }
  }

  inline
  int
  Cube::get_UB_to_DF() const
  {
    int i = 0;
    int j = 0;
    edge edge[3];

    // Compute the index i < (12 choose 3) and the edge permutation.
    for (int k = UR; k <= BR; k++)
    {
      if (UB <= edge_perm_[k] && edge_perm_[k] <= DF)
      {
        i += Cnk(k, j + 1);
        edge[j++] = edge_perm_[k];
      }
    }

    j = 0;

    // Compute the index b < 3! for the permutation in edge
    for (int k = 2; k > 0; k--)
    {
      int l = 0;
      while (edge[k] != UB + k)
      {
        rotate_left(edge, 0, k);
        l++;
      }

      j = (k + 1) * j + l;
    }

    return static_cast<int>(6 * i + j);
  }

  inline
  void
  Cube::set_UB_to_DF(int id)
  {
    for (edge e : edges)
      edge_perm_[e] = BR;

    edge slice_edge[3] = { UB, DR, DF };

    int i = id % 6; // Permutation

    // Generate permutation from index i
    for (int j = 1, k = 0; j < 3; j++)
    {
      k = i % (j + 1);
      i /= j + 1;

      while (k-- > 0)
        rotate_right(slice_edge, 0, j);
    }

    int j = 2;
    int k = id / 6;  // Combination

    for (int l = BR; l >= UR; l--)
    {
      if (k - Cnk(l, j + 1) >= 0)
      {
        edge_perm_[l] = slice_edge[j];
        k -= Cnk(l, j-- + 1);
      }
    }
  }

  inline
  int
  Cube::get_URF_to_DLB() const
  {
    int i = 0;
    corner corner[6];

    for (int j = 0; i < 8; j++)
      corner[i] = corner_perm_[i];

    // Compute the index i < 8! for the permutation in perm.
    for (int j = DRB; j > URF; j--)
    {
      int k = 0;
      while (corner[j] != j)
      {
        rotate_left(corner, 0, j);
        k++;
      }

      i = (j + 1) * i + k;
    }

    return i;
  }

  inline
  void
  Cube::set_URF_to_DLB(int id)
  {
    int i = 0;
    corner slice_corner[8] = { URF, UFL, ULB, UBR, DFR, DLF, DBL, DRB };

    for (int j = 1; j < 8; j++)
    {
      i = id % (j + 1);
      id /= j + 1;

      while (i-- > 0)
        rotate_right(slice_corner, 0, j);
    }

    int j = 7;

    for (int k = DRB; k >= URF; k--)
      corner_perm_[k] = slice_corner[j--];
  }

  inline
  int
  Cube::get_UR_to_BR() const
  {
    int i = 0;
    edge perm[12];

    for (int j = 0; i < 12; j++)
      perm[i] = edge_perm_[i];

    // Compute the index i < 12! for the permutation in perm.
    for (int j = BR; j > UR; j--)
    {
      int k = 0;
      while (perm[j] != j)
      {
        rotate_left(perm, 0, j);
        k++;
      }

      i = (j + 1) * i + k;
    }

    return i;
  }

  inline
  void
  Cube::set_UR_to_BR(int id)
  {
    int i = 0;
    edge slice_edge[12] = { UR, UF, UL, UB, DR, DF, DL, DB, FR, FL, BL, BR };

    for (int j = 1; j < 12; j++)
    {
      i = id % (j + 1);
      id /= j + 1;

      while (i-- > 0)
        rotate_right(slice_edge, 0, j);
    }

    int j = 11;

    for (int k = BR; k >= UR; k--)
      edge_perm_[k] = slice_edge[j--];
  }

} // namespace cube
