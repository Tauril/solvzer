#include <algorithm>
#include <cstring>
#include <iostream>
#include <map>
#include <vector>

#include <cube/cube.hh>

namespace cube
{

  const unsigned int allpos[] =
    {1, 18, 243, 3240, 43239, 574908, 7618438, 100803036, 1332343288};
  const unsigned int c48pos[] =
    {1, 2, 9, 75, 934, 12077, 159131, 2101575, 27762103, 366611212};
  const unsigned int c96pos[] =
    {1, 2, 8, 48, 509, 6198, 80178, 1053077, 13890036, 183339529};

  void
  check(const Cube& cp1, const Cube& cp2, const char* msg)
  {
    if (cp1 == cp2)
      return;

    std::cout << "cubies:" << std::endl;
    for (int i = 0; i < 8; i++)
      std::cout << ' ' << (int)cp1.cubie_[i] << ' ' << (int)cp2.cubie_[i] << std::endl;

    std::cout << "edges:" << std::endl;
    for (int i = 0; i < 12; i++)
      std::cout << ' ' << (int)cp1.edge_[i] << ' ' << (int)cp2.edge_[i] << std::endl;


    std::cout << std::endl << msg << std::endl;
    exit(10);
  }

  void
  recurse_search(const Cube& cube, int togo, int canon_state,
                 std::vector<Cube>& v_cube)
  {
    v_cube.emplace_back(cube);

    if (togo--)
    {
      Cube cube2 = Cube::identity_cube_;
      int mask = Cube::cs_mask(canon_state);

      for (int mv = 0; mv < NMOVES; mv++)
      {
        if ((mask >> mv) & 1)
        {
          cube2 = cube;
          cube2.move(mv);
          recurse_search(cube2, togo, Cube::next_cs(canon_state, mv), v_cube);
        }
      }
    }
  }

  std::vector<int> random_move_seq(int len)
  {
    std::vector<int> ret;
    for (int i = 0; i < len; i++)
      ret.emplace_back(Cube::random_move());
    return ret;
  }

} // namespace cube

constexpr unsigned int MAXELEMENTS = 100000;
std::map<cube::Cube, int> world;
std::vector<cube::Cube> q;

int main(int argc, char** argv)
{
  auto cp1 = cube::Cube::identity_cube_;
  auto cp2 = cube::Cube::identity_cube_;
  auto cp3 = cube::Cube::identity_cube_;
  auto cp4 = cube::Cube::identity_cube_;

  // Basic tests.

  if (sizeof (int) != 4)
    cube::Cube::error("This code assumes a 4-byte int throughout");
  if (sizeof (short) != 2)
    cube::Cube::error("This code assumes a 2-byte short throughout");

  for (int i = 0; i < 8; i++)
  {
    if (cp1.edge_[i] != cube::Cube::identity_cube_.edge_[i])
      cube::Cube::error("bad initial cp1");
  }
  for (int i = 0; i < 12; i++)
  {
    if (cp1.edge_[i] != cube::Cube::identity_cube_.edge_[i])
      cube::Cube::error("bad initial cp2");
  }
  for (int i = 0; i < 16; i++)
  {
    if (cube::Cube::face_map_[i][0] % 3 != 0)
      cube::Cube::error("up down not preserved in first 16");
  }

  // Move tests.

  for (int i = 0; i < cube::NMOVES; i++)
  {
    cp1.move(i);
    cp1.movesc(i);
    check(cp1, cube::Cube::identity_cube_, "problem verifying moves");
  }

  for (int i = 0; i < cube::FACES; i++)
  {
    for (int j = 0; j < 4; j++)
      cp1.move(i * cube::TWISTS);
    check(cp1, cube::Cube::identity_cube_, "problem verifying generators");
  }

  for (int i = 0; i < cube::FACES; i++)
  {
    for (int j = 0; j < 4; j++)
      cp1.movesc(i * cube::TWISTS);
    check(cp1, cube::Cube::identity_cube_, "problem verifying generators 2");
  }

  // Inversion tests.

  for (int i = 0; i < 100; i++)
  {
    cp1.randomize();
    cp1.invert_into(cp2);
    cp2.invert_into(cp3);
    check(cp1, cp3, "Inversion failed");
  }

  for (int i = 0; i < 100; i++)
  {
    std::vector<int> move_seq1 = cube::random_move_seq(10);
    std::vector<int> move_seq2 = cube::Cube::invert_sequence(move_seq1);
    cp1 = cube::Cube::identity_cube_;
    cp2 = cube::Cube::identity_cube_;

    for (unsigned int j = 0; j < move_seq1.size(); j++)
    {
      cp1.move(move_seq1[j]);
      cp2.move(move_seq2[j]);
    }

    cp1.invert_into(cp3);
    check(cp2, cp3, "Invert move sequence failed");
  }

  // Multiplication tests.

  for (int i = 0; i < 100; i++)
  {
    std::vector<int> ms = cube::random_move_seq(10);
    std::vector<int> ms2 = cube::random_move_seq(10);
    cp1 = cube::Cube::identity_cube_;
    cp2 = cube::Cube::identity_cube_;
    cp3 = cube::Cube::identity_cube_;

    for (unsigned int j = 0; j < ms.size(); j++)
    {
      cp1.move(ms[j]);
      cp3.move(ms[j]);
    }

    for (unsigned int j = 0; j < ms2.size(); j++)
    {
      cp2.move(ms2[j]);
      cp3.move(ms2[j]);
    }

    cube::Cube::mul(cp1, cp2, cp4);
    check(cp4, cp3, "Bad product");

    cp1 = cube::Cube::identity_cube_;
    cp2 = cube::Cube::identity_cube_;
    cp3 = cube::Cube::identity_cube_;

    for (unsigned int j = 0; j < ms.size(); j++)
    {
      cp1.movesc(ms[j]);
      cp3.movesc(ms[j]);
    }

    for (unsigned int j = 0; j < ms2.size(); j++)
    {
      cp2.movesc(ms2[j]);
      cp3.movesc(ms2[j]);
    }

    cube::Cube::mulsc(cp1, cp2, cp4);
    check(cp4, cp3, "Bad product 2");
  }

  // Test parse move.

  for (int i = 0; i < 100; i++)
  {
    std::vector<int> ms = cube::random_move_seq(10);
    char move_buff[100];
    char* p = move_buff;

    for (unsigned int j = 0; j < ms.size(); j++)
      cube::Cube::append_move(p, ms[j]);

    const char* q = move_buff;
    std::vector<int> ms2 = cube::Cube::parse_move_sequence(q);

    if (ms != ms2)
      cube::Cube::error("Bad parse");
  }

  // Test Sing.

  for (int i = 0; i < 100; i++)
  {
    char sing_buff[1000];
    cp1.randomize();
    std::strcpy(sing_buff, cp1.sing_string());

    const char* err = cp2.parse_sing(sing_buff);

    if (err)
      cube::Cube::error(err);

    check(cp1, cp2, "Mismatch between parse and gen");
  }

  // Test remap.

  for (int i = 0; i < 100; i++)
  {
    std::vector<int> ms;
    int m = static_cast<int>(cube::M * cube::Cube::my_rand());
    cp1 = cube::Cube::identity_cube_;
    cp2 = cube::Cube::identity_cube_;

    for (unsigned int j = 0; j < ms.size(); j++)
    {
      cp1.move(ms[j]);
      cp2.move(cube::Cube::move_map_[m][ms[j]]);
    }

    cp1.remap_into(m, cp3);
    check(cp2, cp3, "Move map issue");
  }

  // BFS.

  // 1.
  world.clear();
  q.clear();
  q.emplace_back(cube::Cube::identity_cube_);
  world[cube::Cube::identity_cube_] = 0;

  unsigned int qg = 0;
  int prevd = -1;

  while (qg < q.size())
  {
    int d = world[q[qg]];
    if (d != prevd)
    {
      std::cout << "At lev " << d << " size " << (q.size() - qg) << std::endl;
      if (cube::allpos[d] != q.size() - qg)
        cube::Cube::error("Bad value");
      if (q.size() > MAXELEMENTS)
        break;
      prevd = d;
    }

    for (int i = 0; i < cube::NMOVES; i++)
    {
      cp1 = q[qg];
      cp1.move(i);

      if (world.find(cp1) == world.end())
      {
        world[cp1] = d + 1;
        q.emplace_back(cp1);
      }
    }
    qg++;
  }

  // 2.
  world.clear();
  q.clear();
  q.emplace_back(cube::Cube::identity_cube_);
  world[cube::Cube::identity_cube_] = 0;
  qg = 0;
  prevd = -1;

  while (qg < q.size())
  {
    int d = world[q[qg]];
    if (d != prevd)
    {
      std::cout << "At lev " << d << " size " << (q.size() - qg) << std::endl;
      if (cube::c48pos[d] != q.size() - qg)
        cube::Cube::error("Bad value");
      if (q.size() > MAXELEMENTS)
        break;
      prevd = d;
    }

    for (int i = 0; i < cube::NMOVES; i++)
    {
      cp1 = q[qg];
      cp1.move(i);
      cp1.canon_into_48(cp2);
      if (world.find(cp2) == world.end())
      {
        world[cp2] = d + 1;
        q.emplace_back(cp2);
      }
    }
    qg++;
  }

  // 3.
  world.clear();
  q.clear();
  q.emplace_back(cube::Cube::identity_cube_);
  world[cube::Cube::identity_cube_] = 0;
  qg = 0;
  prevd = -1;

  while (qg < q.size())
  {
    int d = world[q[qg]];
    if (d != prevd)
    {
      std::cout << "At lev " << d << " size " << (q.size() - qg) << std::endl;
      if (cube::c96pos[d] != q.size() - qg)
        cube::Cube::error("Bad value");
      if (q.size() > MAXELEMENTS)
        break;
      prevd = d;
    }

    for (int i = 0; i < cube::NMOVES; i++)
    {
      cp1 = q[qg];
      cp1.move(i);
      cp1.canon_into_96(cp2);

      if (world.find(cp2) == world.end())
      {
        world[cp2] = d + 1;
        q.emplace_back(cp2);
      }

      cp1 = q[qg];
      cp1.movesc(i);
      cp1.canon_into_96(cp2);
      if (world.find(cp2) == world.end())
      {
        world[cp2] = d + 1;
        q.emplace_back(cp2);
      }
    }
    qg++;
  }

  // 4.
  world.clear();
  unsigned int prev_count = 0;

  for (int d = 0; ; d++)
  {
    q.clear();
    recurse_search(cube::Cube::identity_cube_, d, cube::CANONSEQSTART, q);
    std::sort(q.begin(), q.end());
    std::vector<cube::Cube>::iterator end = std::unique(q.begin(), q.end());
    unsigned int sz = end - q.begin();

    std::cout << "Sequences " << q.size() << " positions " << sz << std::endl;
    std::cout << "At lev " << d << " size " << (sz - prev_count) << std::endl;

    if (cube::allpos[d] != sz - prev_count)
      cube::Cube::error("Bad value");

    prev_count = sz;

    if (sz > 3000000)
      break;
  }
}
