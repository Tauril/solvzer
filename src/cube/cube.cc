#include <cube/cube.hh>

namespace cube
{

  /// Static data
  std::array<Cube, 6> Cube::move_cube_;


  Cube::Cube(c_perm& corner_perm, c_ori& corner_ori,
             e_perm& edge_perm,   e_ori& edge_ori)
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

  void
  Cube::init_move_cube()
  {
    move_cube_[0].corner_perm_ = Cube::corner_perm_U_;
    move_cube_[0].corner_ori_ = Cube::corner_ori_U_;
    move_cube_[0].edge_perm_ = Cube::edge_perm_U_;
    move_cube_[0].edge_ori_ = Cube::edge_ori_U_;

    move_cube_[1].corner_perm_ = Cube::corner_perm_R_;
    move_cube_[1].corner_ori_ = Cube::corner_ori_R_;
    move_cube_[1].edge_perm_ = Cube::edge_perm_R_;
    move_cube_[1].edge_ori_ = Cube::edge_ori_R_;

    move_cube_[2].corner_perm_ = Cube::corner_perm_F_;
    move_cube_[2].corner_ori_ = Cube::corner_ori_F_;
    move_cube_[2].edge_perm_ = Cube::edge_perm_F_;
    move_cube_[2].edge_ori_ = Cube::edge_ori_F_;

    move_cube_[3].corner_perm_ = Cube::corner_perm_D_;
    move_cube_[3].corner_ori_ = Cube::corner_ori_D_;
    move_cube_[3].edge_perm_ = Cube::edge_perm_D_;
    move_cube_[3].edge_ori_ = Cube::edge_ori_D_;

    move_cube_[4].corner_perm_ = Cube::corner_perm_L_;
    move_cube_[4].corner_ori_ = Cube::corner_ori_L_;
    move_cube_[4].edge_perm_ = Cube::edge_perm_L_;
    move_cube_[4].edge_ori_ = Cube::edge_ori_L_;

    move_cube_[5].corner_perm_ = Cube::corner_perm_B_;
    move_cube_[5].corner_ori_ = Cube::corner_ori_B_;
    move_cube_[5].edge_perm_ = Cube::edge_perm_B_;
    move_cube_[5].edge_ori_ = Cube::edge_ori_B_;
  }

  std::ostream&
  operator<<(std::ostream& o, const Cube& c)
  {
    Face face;
    c.to_face(face);
    return o << face;
  }

} // namespace cube
