#include <algorithm>

#include <cube/cube.hh>
#include <misc/indent.hh>

namespace cube
{

  void
  Cube::init_move_cube()
  {
    Cube::move_cube_[0] = Cube::corner_perm_U_;
    Cube::move_cube_[0] = Cube::corner_ori_U_;
    Cube::move_cube_[0] = Cube::edge_perm_U_;
    Cube::move_cube_[0] = Cube::edge_ori_U_;

    Cube::move_cube_[1] = Cube::corner_perm_R_;
    Cube::move_cube_[1] = Cube::corner_ori_R_;
    Cube::move_cube_[1] = Cube::edge_perm_R_;
    Cube::move_cube_[1] = Cube::edge_ori_R_;

    Cube::move_cube_[2] = Cube::corner_perm_F_;
    Cube::move_cube_[2] = Cube::corner_ori_F_;
    Cube::move_cube_[2] = Cube::edge_perm_F_;
    Cube::move_cube_[2] = Cube::edge_ori_F_;

    Cube::move_cube_[3] = Cube::corner_perm_D_;
    Cube::move_cube_[3] = Cube::corner_ori_D_;
    Cube::move_cube_[3] = Cube::edge_perm_D_;
    Cube::move_cube_[3] = Cube::edge_ori_D_;

    Cube::move_cube_[4] = Cube::corner_perm_L_;
    Cube::move_cube_[4] = Cube::corner_ori_L_;
    Cube::move_cube_[4] = Cube::edge_perm_L_;
    Cube::move_cube_[4] = Cube::edge_ori_L_;

    Cube::move_cube_[5] = Cube::corner_perm_B_;
    Cube::move_cube_[5] = Cube::corner_ori_B_;
    Cube::move_cube_[5] = Cube::edge_perm_B_;
    Cube::move_cube_[5] = Cube::edge_ori_B_;
  }

  std::ostream&
  operator<<(std::ostream& o, const Cube& c)
  {
    /*auto sep = "`---'---'---`";

    // Up
    for (auto i = 0; i < 3; i++)
    {
      misc::indent(o);

      if (i == 0)
      {
        o << sep
          << std::endl;

        misc::indent(o);
      }

      //c.cube_get()[0].dump_row(o, i, 0);
      o << std::endl;

      if (i < 2)
      {
        misc::indent(o);

        o << sep
          << std::endl;
      }
    }

    for (auto i = 0; i < 4; i++)
    {
      if (i == 0)
        o << '`';
      o << "---'---'---`";
    }

    o << std::endl;


    // Left | Right | Back
    for (auto i = 0; i < 3; i++)
    {
      int x = 0;
      for (auto j = 1; j < 5; j++, x++)
      {
        //c.cube_get()[j].dump_row(o, i, x);
      }
      o << std::endl;

      for (auto k = 0; k < 4; k++)
      {
        if (k == 0)
          o << '`';
        o << "---'---'---`";
      }

      o << std::endl;
    }


    // Down
    for (auto i = 0; i < 3; i++)
    {
      misc::indent(o);

      //c.cube_get()[5].dump_row(o, i, 0);
      o << std::endl;

      misc::indent(o);

      o << sep
        << std::endl;
    }*/

    return o;
  }

} // namespace cube
