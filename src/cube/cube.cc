#include <cube/cube.hh>

namespace cube
{

  /// Static data
  std::array<Cube, 6> Cube::move_cube_;
  constexpr char Cube::solved_state_[];

  namespace
  {

    void
    init_move_cube()
    {
      Cube::move_cube_[0].corner_perm_ = Cube::corner_perm_U_;
      Cube::move_cube_[0].corner_ori_ = Cube::corner_ori_U_;
      Cube::move_cube_[0].edge_perm_ = Cube::edge_perm_U_;
      Cube::move_cube_[0].edge_ori_ = Cube::edge_ori_U_;

      Cube::move_cube_[1].corner_perm_ = Cube::corner_perm_R_;
      Cube::move_cube_[1].corner_ori_ = Cube::corner_ori_R_;
      Cube::move_cube_[1].edge_perm_ = Cube::edge_perm_R_;
      Cube::move_cube_[1].edge_ori_ = Cube::edge_ori_R_;

      Cube::move_cube_[2].corner_perm_ = Cube::corner_perm_F_;
      Cube::move_cube_[2].corner_ori_ = Cube::corner_ori_F_;
      Cube::move_cube_[2].edge_perm_ = Cube::edge_perm_F_;
      Cube::move_cube_[2].edge_ori_ = Cube::edge_ori_F_;

      Cube::move_cube_[3].corner_perm_ = Cube::corner_perm_D_;
      Cube::move_cube_[3].corner_ori_ = Cube::corner_ori_D_;
      Cube::move_cube_[3].edge_perm_ = Cube::edge_perm_D_;
      Cube::move_cube_[3].edge_ori_ = Cube::edge_ori_D_;

      Cube::move_cube_[4].corner_perm_ = Cube::corner_perm_L_;
      Cube::move_cube_[4].corner_ori_ = Cube::corner_ori_L_;
      Cube::move_cube_[4].edge_perm_ = Cube::edge_perm_L_;
      Cube::move_cube_[4].edge_ori_ = Cube::edge_ori_L_;

      Cube::move_cube_[5].corner_perm_ = Cube::corner_perm_B_;
      Cube::move_cube_[5].corner_ori_ = Cube::corner_ori_B_;
      Cube::move_cube_[5].edge_perm_ = Cube::edge_perm_B_;
      Cube::move_cube_[5].edge_ori_ = Cube::edge_ori_B_;
    }

  }

  Cube::Cube(const Face& face)
  {
    for (int i = 0; i < 8; i++)
      corner_perm_[i] = URF;
    for (int i = 0; i < 12; i++)
      edge_perm_[i] = UR;

    color c1;
    color c2;

    for (corner i : corners)
    {
      int ori = 0;
      for (; ori < 3; ori++)
        if (face.face_[Face::corner_facelet_[i][ori]] == U
            || face.face_[Face::corner_facelet_[i][ori]] == D)
          break;

      c1 = face.face_[Face::corner_facelet_[i][(ori + 1) % 3]];
      c2 = face.face_[Face::corner_facelet_[i][(ori + 2) % 3]];

      for (corner j : corners)
      {
        if (c1 == Face::corner_color_[j][1] && c2 == Face::corner_color_[j][2])
        {
          corner_perm_[i] = j;
          corner_ori_[i] = static_cast<unsigned char>(ori % 3);
          break;
        }
      }
    }

    for (edge i : edges)
    {
      for (edge j : edges)
      {
        if (face.face_[Face::edge_facelet_[i][0]] == Face::edge_color_[j][0]
            && face.face_[Face::edge_facelet_[i][1]] == Face::edge_color_[j][1])
        {
          edge_perm_[i] = j;
          edge_ori_[i] = 0;
          break;
        }

        if (face.face_[Face::edge_facelet_[i][0]] == Face::edge_color_[j][1]
            && face.face_[Face::edge_facelet_[i][1]] == Face::edge_color_[j][0])
        {
          edge_perm_[i] = j;
          edge_ori_[i] = 1;
          break;
        }
      }
    }
  }

  Cube::Cube(const c_perm& corner_perm, const c_ori& corner_ori,
             const e_perm& edge_perm,   const e_ori& edge_ori)
  {
    for (int i = 0; i < 8; i++)
    {
      corner_perm_[i] = corner_perm[i];
      corner_ori_[i] = corner_ori[i];
    }

    for (int i = 0; i < 12; i++)
    {
      edge_perm_[i] = edge_perm[i];
      edge_ori_[i] = edge_ori[i];
    }
  }

  void
  Cube::init_statics()
  {
    init_move_cube();
  }

  void
  Cube::corner_multiply(const Cube& cube)
  {
    corner corner_perm[8];
    unsigned char corner_ori[8];

    for (corner c : corners)
    {
      corner_perm[c] = corner_perm_[cube.corner_perm_[c]];

      unsigned char ori1 = corner_ori_[cube.corner_perm_[c]];
      unsigned char ori2 = cube.corner_ori_[c];
      unsigned char ori  = 0;

      // If both cubes are regular cubes
      if (ori1 < 3 && ori2 < 3)
      {
        ori = static_cast<unsigned char>(ori1 + ori2);
        if (ori >= 3)
          // the composition is a regular cube.
          ori -= 3;
      }

      corner_ori[c] = ori;
    }

    for (corner c : corners)
    {
      corner_perm_[c] = corner_perm[c];
      corner_ori_[c] = corner_ori[c];
    }
  }

  void
  Cube::edge_multiply(const Cube& cube)
  {
    edge edge_perm[12];
    unsigned char edge_ori[12];

    for (edge e : edges)
    {
      edge_perm[e] = edge_perm_[cube.edge_perm_[e]];

      unsigned char ori1 = cube.edge_ori_[e];
      unsigned char ori2 = edge_ori_[cube.edge_perm_[e]];

      edge_ori[e] = static_cast<unsigned char>((ori1 + ori2) % 2);
    }

    for (edge e : edges)
    {
      edge_perm_[e] = edge_perm[e];
      edge_ori_[e] = edge_ori[e];
    }
  }

  std::string
  Cube::verify() const
  {
    std::array<int, 12> edge_cnt = { 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0 };
    for (edge e : edges)
      edge_cnt[edge_perm_[e]]++;
    for (int i = 0; i < 12; i++)
      if (edge_cnt[i] != 1)
        return "Wrong edge count!";

    int sum = 0;
    for (int i = 0; i < 12; i++)
      sum += edge_ori_[i];
    if (sum % 2 != 0)
      return "Edge flipped!";

    std::array<int, 8> corner_cnt = { 0, 0, 0, 0, 0, 0, 0, 0 };
    for (corner c : corners)
      corner_cnt[corner_perm_[c]]++;
    for (int i = 0; i < 8; i++)
      if (corner_cnt[i] != 1)
        return "Wrong corner count!";

    sum = 0;
    for (int i = 0; i < 8; i++)
      sum += corner_ori_[i];
    if (sum % 3 != 0)
      return "Corner twisted!";

    if ((edge_parity() ^ corner_parity()) != 0)
      return "Wrong parity!";

    return "";

  }

  void
  Cube::to_face(Face& face) const
  {
    for (corner c : corners)
    {
      int i = c;
      int j = corner_perm_[i];
      unsigned char ori = corner_ori_[i];

      for (int n = 0; n < 3; n++)
      {
        face.face_[Face::corner_facelet_[i][(n + ori) % 3]] =
          Face::corner_color_[j][n];
      }
    }

    for (edge e : edges)
    {
      int i = e;
      int j = edge_perm_[i];
      unsigned char ori = edge_ori_[i];

      for (int n = 0; n < 2; n++)
      {
        face.face_[Face::edge_facelet_[i][(n + ori) % 2]] =
          Face::edge_color_[j][n];
      }
    }
  }

  std::ostream&
  operator<<(std::ostream& o, const Cube& c)
  {
    Face face;
    c.to_face(face);
    return o << face;
  }

} // namespace cube
