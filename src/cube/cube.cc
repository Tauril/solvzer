#include <algorithm>
#include <cstdio>
#include <cstdlib>
#include <cstring>

#include <cube/cube.hh>
#include <misc/indent.hh>

#ifndef INVALID
# define INVALID 99
#endif

namespace cube
{

  /// Static data instantiations.

  const Cube Cube::identity_cube_(nullptr);

  unsigned char Cube::corner_ori_inc_[CUBIES];
  unsigned char Cube::corner_ori_dec_[CUBIES];
  unsigned char Cube::corner_ori_neg_strip_[CUBIES];
  unsigned char Cube::mod_24_[2 * CUBIES];

  constexpr char faces_[FACES] = {'U', 'F', 'R', 'D', 'B', 'L'};

  unsigned char Cube::edge_trans_[NMOVES][CUBIES];
  unsigned char Cube::corner_trans_[NMOVES][CUBIES];

  constexpr unsigned char edge_twist_perm_[FACES][4] = {{0, 2, 3,   1},
                                                        {3, 7, 11,  6},
                                                        {2, 5, 10,  7},
                                                        {9, 11, 10, 8},
                                                        {0, 4, 8,   5},
                                                        {1, 6, 9,   4}};

  constexpr unsigned char corner_twist_perm_[FACES][4] = {{0, 1, 3, 2},
                                                          {2, 3, 7, 6},
                                                          {3, 1, 5, 7},
                                                          {4, 6, 7, 5},
                                                          {1, 0, 4, 5},
                                                          {0, 2, 6, 4}};

  constexpr unsigned char edge_change_[FACES] = {0, 0, 1, 0, 0, 1};

  constexpr unsigned char corner_change_[FACES][4] = {{0, 0, 0, 0},
                                                      {1, 2, 1, 2},
                                                      {1, 2, 1, 2},
                                                      {0, 0, 0, 0},
                                                      {1, 2, 1, 2},
                                                      {1, 2, 1, 2}};

  unsigned char Cube::inv_move_[NMOVES];

  char Cube::static_buff_[200];

  static const char* sing_solved_ =
    "UF UR UB UL DF DR DB DL FR FL BR BL UFR URB UBL ULF DRF DFL DLB DBR";

  static const char* const sing_edges_[] =
  {"UB", "BU", "UL", "LU", "UR", "RU", "UF", "FU", "LB", "BL", "RB", "BR", "LF",
   "FL", "RF", "FR", "DB", "BD", "DL", "LD", "DR", "RD", "DF", "FD"};

  static const char* const sing_corners_[] =
  {"UBL", "URB", "ULF", "UFR", "DLB", "DBR", "DFL", "DRF", "LUB", "BUR", "FUL",
   "RUF", "BDL", "RDB", "LDF", "FDR", "BLU", "RBU", "LFU", "FRU", "LBD", "BRD",
   "FLD", "RFD", "ULB", "UBR", "UFL", "URF", "DBL", "DRB", "DLF", "DFR", "LBU",
   "BRU", "FLU", "RFU", "BLD", "RBD", "LFD", "FRD", "BUL", "RUB", "LUF", "FUR",
   "LDB", "BDR", "FDL", "RDF"};

  static unsigned char lookup_edge_cubie_[FACES * FACES];
  static unsigned char lookup_corner_cubie_[FACES * FACES * FACES];
  static unsigned char sing_corner_order_[8];
  static unsigned char sing_edge_order_[12];
  static unsigned char sing_edge_flipped_[12];

  unsigned char Cube::face_map_[M][FACES];
  unsigned char Cube::move_map_[M][NMOVES];
  unsigned char Cube::invm_[M];
  unsigned char Cube::m_m_[M][M];
  unsigned char Cube::rot_edge_[M][CUBIES];
  unsigned char Cube::rot_corner_[M][CUBIES];

  static const char* const axis_permute_map_[] =
    {"UFR", "URF", "FRU", "FUR", "RUF", "RFU"};
  static const char* const axis_negate_map_[] =
    {"UFR", "UFL", "UBL", "UBR", "DBR", "DBL", "DFL", "DFR"};

  unsigned char Cube::canon_seq_[CANONSEQSTATES][NMOVES];
  int Cube::canon_seq_mask_[CANONSEQSTATES];
  int Cube::canon_seq_mask_ext_[CANONSEQSTATES];

  static double start = 0;

  namespace
  {

    int
    parse_cubie(const char*& p)
    {
      Cube::skip_whitespace(p);
      int v = 1;
      int f = 0;

      while ((f = Cube::parse_face(p)) >= 0)
      {
        v = v * 6 + f;
        if (v >= 2 * 6 * 6 * 6)
          return -1;
      }

      return v;
    }

    int
    parse_edge(const char*& p)
    {
      int c = parse_cubie(p);
      if ((c < 6 * 6) || (c >= 2 * 6 * 6))
        return -1;

      c = lookup_edge_cubie_[c - 6 * 6];
      if (c == INVALID)
        return -1;

      return c;
    }

    int
    parse_corner(const char*& p)
    {
      int c = parse_cubie(p);
      if ((c < 6 * 6 * 6) || (c >= 2 * 6 * 6 * 6))
        return -1;

      c = lookup_corner_cubie_[c - 6 * 6 * 6];
      if ((c == INVALID) || (c >= CUBIES))
        return -1;

      return c;
    }

    void
    parse_corner_to_facemap(const char* p, unsigned char* a)
    {
      for (int i = 0; i < 3; i++)
      {
        int f = Cube::parse_face(p[i]);
        a[i] = f;
        a[i + 3] = (f + 3) % FACES;
      }
    }

    void
    face_map_multiply(unsigned char* a, unsigned char* b, unsigned char* c)
    {
      for (int i = 0; i < 6; i++)
        c[i] = b[a[i]];
    }

  }

  Cube::Cube(Cube* cube)
  {
    if (cube)
      *this = *cube;
    else
    {
      for (int i = 0; i < 8; i++)
        cubie_[i] = corner_val(i, 0);
      for (int i = 0; i < 12; i++)
        edge_[i] = edge_val(i, 0);

      init();
    }
  }

  void
  Cube::init()
  {
    static int init = 0;

    if (init)
      return;

    init = 1;

    for (int i = 0; i < CUBIES; i++)
    {
      int perm = corner_perm(i);
      int ori = corner_ori(i);

      corner_ori_inc_[i] = corner_val(perm, (ori + 1) % 3);
      corner_ori_dec_[i] = corner_val(perm, (ori + 2) % 3);
      corner_ori_neg_strip_[i] = corner_val(0, (3 - ori) % 3);
      mod_24_[i] = mod_24_[i + CUBIES] = i;
    }

    for (int m = 0; m < NMOVES; m++)
    {
      for (int c = 0; c < CUBIES; c++)
      {
        edge_trans_[m][c] = c;
        corner_trans_[m][c] = c;
      }
    }

    for (int f = 0; f < FACES; f++)
    {
      for (int t = 0; t < TWISTS; t++)
      {
        int m = f * TWISTS + t;
        int is_quarter = (t == 0 || t == 2);
        int perm_inc = t + 1;

        if (m < 0)
          continue;

        for (int i = 0; i < 4; i++)
        {
          int j = (i + perm_inc) % 4;

          for (int o = 0; o < 2; o++)
          {
            int p = o;  // new orientation.

            if (is_quarter)
              p ^= edge_change_[f];

            edge_trans_[m][edge_val(edge_twist_perm_[f][i], o)] =
              edge_val(edge_twist_perm_[f][j], p);
          }

          for (int o = 0; o < TWISTS; o++)
          {
            int p = o; // new orientation.

            if (is_quarter)
              p = (corner_change_[f][i] + p) % 3;

            corner_trans_[m][corner_val(corner_twist_perm_[f][i], o)] =
              corner_val(corner_twist_perm_[f][j], p);
          }
        }
      }
    }

    for (int i = 0; i < NMOVES; i++)
      inv_move_[i] = TWISTS * (i / TWISTS) + (NMOVES - i - 1) % TWISTS;

    std::memset(lookup_edge_cubie_, INVALID, sizeof (lookup_edge_cubie_));
    std::memset(lookup_corner_cubie_, INVALID, sizeof (lookup_corner_cubie_));

    for (int i = 0; i < CUBIES; i++)
    {
      const char* tmp = 0;

      lookup_corner_cubie_[parse_cubie(tmp = sing_corners_[i]) - 6 * 6 * 6] = i;
      lookup_corner_cubie_[
        parse_cubie(tmp = sing_corners_[CUBIES + i]) - 6 * 6 * 6
      ] = CUBIES + i;
      lookup_edge_cubie_[parse_cubie(tmp = sing_edges_[i]) - 6 * 6] = i;
    }

    const char* p = sing_solved_;

    for (int i = 0; i < 12; i++)
    {
      int cv = parse_edge(p);
      sing_edge_order_[i] = edge_perm(cv);
      sing_edge_flipped_[i] = edge_ori(cv);
    }

    for (int i = 0; i < 8; i++)
      sing_corner_order_[i] = corner_perm(parse_corner(p));

    unsigned char face_to_m[FACES * FACES * FACES];

    for (int i = 0; i < 6; i++)
      parse_corner_to_facemap(axis_permute_map_[i], face_map_[8 * i]);

    for (int i = 0; i < 8; i++)
      parse_corner_to_facemap(axis_negate_map_[i], face_map_[i]);

    for (int i = 1; i < 6; i++)
      for (int j = 1; j < 8; j++)
        face_map_multiply(face_map_[8 * i], face_map_[j], face_map_[8 * i + j]);

    for (int i = 0; i < M; i++)
    {
      int v = face_map_[i][0] * 36 + face_map_[i][1] * 6 + face_map_[i][2];
      face_to_m[v] = i;
    }

    unsigned char tfaces[6];

    for (int i = 0; i < M; i++)
    {
      for (int j = 0; j < M; j++)
      {
        face_map_multiply(face_map_[i], face_map_[j], tfaces);

        int v = tfaces[0] * 36 + tfaces[1] * 6 + tfaces[2];
        m_m_[i][j] = face_to_m[v];

        if (m_m_[i][j] == 0)
          invm_[i] = j;
      }
    }

    for (int m = 0; m < M; m++)
    {
      int is_neg = (m ^ (m >> 3)) & 1;

      for (int f = 0; f < 6; f++)
      {
        for (int t = 0; t < TWISTS; t++)
        {
          if (is_neg)
          {
            move_map_[m][f * TWISTS + t] =
              face_map_[m][f] * TWISTS + TWISTS - 1 - t;
          }
          else
            move_map_[m][f * TWISTS + t] = face_map_[m][f] * TWISTS + t;
        }
      }
    }

    for (int m = 0; m < M; m++)
    {
      for (int c = 0; c < CUBIES; c++)
      {
        int v = 0;
        for (int i = 0; i < 2; i++)
          v = 6 * v + face_map_[m][parse_face(sing_edges_[c][i])];

        rot_edge_[m][c] = lookup_edge_cubie_[v];

        v = 0;
        for (int i = 0; i < 3; i++)
          v = 6 * v + face_map_[m][parse_face(sing_corners_[c][i])];

        rot_corner_[m][c] = mod_24_[lookup_corner_cubie_[v]];
      }
    }

    for (int s = 0; s < CANONSEQSTATES; s++)
    {
      int prev_face = (s - 1) % FACES;
      canon_seq_mask_[s] = (1 << NMOVES) - 1;

      for (int nm = 0; nm < NMOVES; nm++)
      {
        int f = nm / TWISTS;
        int is_plus = 0;

        if (s != 0 && ((prev_face == f) || (prev_face == f + 3))) // illegal
        {
          canon_seq_[s][nm] = INVALID;
          canon_seq_mask_[s] &= ~(1 << nm);
        }
        else
          canon_seq_[s][nm] = f + 1 + FACES * is_plus;
      }

      canon_seq_mask_ext_[s] = canon_seq_mask_[s];
    }
  }

  void
  Cube::move(int move)
  {
    const unsigned char* p = corner_trans_[move];
    cubie_[0] = p[cubie_[0]];
    cubie_[1] = p[cubie_[1]];
    cubie_[2] = p[cubie_[2]];
    cubie_[3] = p[cubie_[3]];
    cubie_[4] = p[cubie_[4]];
    cubie_[5] = p[cubie_[5]];
    cubie_[6] = p[cubie_[6]];
    cubie_[7] = p[cubie_[7]];

    p = edge_trans_[move];
    edge_[0] = p[edge_[0]];
    edge_[1] = p[edge_[1]];
    edge_[2] = p[edge_[2]];
    edge_[3] = p[edge_[3]];
    edge_[4] = p[edge_[4]];
    edge_[5] = p[edge_[5]];
    edge_[6] = p[edge_[6]];
    edge_[7] = p[edge_[7]];
    edge_[8] = p[edge_[8]];
    edge_[9] = p[edge_[9]];
    edge_[10] = p[edge_[10]];
    edge_[11] = p[edge_[11]];
  }

  int
  Cube::invert_move(int move)
  {
    return inv_move_[move];
  }

  std::vector<int>
  Cube::invert_sequence(const std::vector<int>& sequence)
  {
    unsigned int len = sequence.size();
    std::vector<int> ret(len);

    for (unsigned int i = 0; i < len; i++)
      ret[len - i - 1] = invert_move(sequence[i]);

    return ret;
  }

  void
  Cube::invert_into(Cube& dst) const
  {
    for (int i = 0; i < 8; i++)
    {
      int c_val = cubie_[i];
      dst.cubie_[corner_perm(c_val)] = corner_ori_sub(i, c_val);
    }

    for (int i = 0; i < 12; i++)
    {
      int e_val = edge_[i];
      dst.edge_[edge_perm(e_val)] = edge_val(i, edge_ori(e_val));
    }
  }

  void
  Cube::movesc(int move)
  {
    switch (move)
    {
      case 0:
        rot_4(edge_, 0, 2, 3, 1);
        rot_4(cubie_, 0, 1, 3, 2);
        break;
      case 1:
        rot_2_2(edge_, 0, 2, 3, 1);
        rot_2_2(cubie_, 0, 1, 3, 2);
        break;
      case 2:
        rot_4(edge_, 1, 3, 2, 0);
        rot_4(cubie_, 2, 3, 1, 0);
        break;
      case 3:
        rot_4(edge_, 3, 7, 11, 6);
        corner_4_flip(3, 7, 6, 2);
        break;
      case 4:
        rot_2_2(edge_, 3, 7, 11, 6);
        rot_2_2(cubie_, 2, 3, 7, 6);
        break;
      case 5:
        rot_4(edge_, 6, 11, 7, 3);
        corner_4_flip(3, 2, 6, 7);
        break;
      case 6:
        edge_4_flip(2, 5, 10, 7);
        corner_4_flip(1, 5, 7, 3);
        break;
      case 7:
        rot_2_2(edge_, 2, 5, 10, 7);
        rot_2_2(cubie_, 3, 1, 5, 7);
        break;
      case 8:
        edge_4_flip(7, 10, 5, 2);
        corner_4_flip(1, 3, 7, 5);
        break;
      case 9:
        rot_4(edge_, 9, 11, 10, 8);
        rot_4(cubie_, 4, 6, 7, 5);
        break;
      case 10:
        rot_2_2(edge_, 9, 11, 10, 8);
        rot_2_2(cubie_, 4, 6, 7, 5);
        break;
      case 11:
        rot_4(edge_, 8, 10, 11, 9);
        rot_4(cubie_, 5, 7, 6, 4);
        break;
      case 12:
        rot_4(edge_, 0, 4, 8, 5);
        corner_4_flip(0, 4, 5, 1);
        break;
      case 13:
        rot_2_2(edge_, 0, 4, 8, 5);
        rot_2_2(cubie_, 1, 0, 4, 5);
        break;
      case 14:
        rot_4(edge_, 5, 8, 4, 0);
        corner_4_flip(0, 1, 5, 4);
        break;
      case 15:
        edge_4_flip(1, 6, 9, 4);
        corner_4_flip(2, 6, 4, 0);
        break;
      case 16:
        rot_2_2(edge_, 1, 6, 9, 4);
        rot_2_2(cubie_, 0, 2, 6, 4);
        break;
      case 17:
        edge_4_flip(4, 9, 6, 1);
        corner_4_flip(2, 0, 4, 6);
        break;
      default:
        die("Invalid move");
    }
  }

  void
  Cube::rot_2(unsigned char* cc, int a, int b)
  {
    unsigned char t = cc[a];
    cc[a] = cc[b];
    cc[b] = t;
  }

  void
  Cube::rot_4(unsigned char* cc, int a, int b, int c, int d)
  {
    unsigned char t = cc[d];
    cc[d] = cc[c];
    cc[c] = cc[b];
    cc[b] = cc[a];
    cc[a] = t;
  }

  void
  Cube::rot_2_2(unsigned char* cc, int a, int b, int c, int d)
  {
    rot_2(cc, a, c);
    rot_2(cc, b, d);
  }

  void
  Cube::edge_4_flip(int a, int b, int c, int d)
  {
    unsigned char t = edge_[d];
    edge_[d] = edge_flip(edge_[c]);
    edge_[c] = edge_flip(edge_[b]);
    edge_[b] = edge_flip(edge_[a]);
    edge_[a] = edge_flip(t);
  }

  void
  Cube::corner_4_flip(int a, int b, int c, int d)
  {
    unsigned char t = cubie_[d];
    cubie_[d] = corner_ori_inc_[cubie_[c]];
    cubie_[c] = corner_ori_dec_[cubie_[b]];
    cubie_[b] = corner_ori_inc_[cubie_[a]];
    cubie_[a] = corner_ori_dec_[t];
  }

  void
  Cube::mul(const Cube& c1, const Cube& c2, Cube& ret)
  {
    for (int i = 0; i < 8; i++)
    {
      int cubie = c1.cubie_[i];
      ret.cubie_[i] = corner_ori_add(c2.cubie_[corner_perm(cubie)], cubie);
    }

    for (int i = 0; i < 12; i++)
    {
      int edge = c1.edge_[i];
      ret.edge_[i] = edge_ori_add(c2.edge_[edge_perm(edge)], edge);
    }
  }

  void
  Cube::mulsc(const Cube& c1, const Cube& c2, Cube& ret)
  {
    mul(c2, c1, ret);
  }

  void
  Cube::skip_whitespace(const char*& p)
  {
    while (*p && (*p <= ' '))
      p++;
  }

  int
  Cube::parse_face(const char*& p)
  {
    int f = parse_face(*p);
    if (f >= 0)
      p++;
    return f;
  }

  int
  Cube::parse_face(char f)
  {
    switch (f)
    {
      case 'u':
      case 'U':
        return 0;
      case 'f':
      case 'F':
        return 1;
      case 'r':
      case 'R':
        return 2;
      case 'd':
      case 'D':
        return 3;
      case 'b':
      case 'B':
        return 4;
      case 'l':
      case 'L':
        return 5;
      default:
        return -1;
    }
  }

  void
  Cube::append_face(char*& p, int f)
  {
    *p++ = faces_[f];
  }

  int
  Cube::parse_move(const char*& p)
  {
    skip_whitespace(p);

    const char* q = p;
    int f = parse_face(q);

    if (f < 0)
      return -1;

    int t = 0;

    switch (*q)
    {
      case '1':
      case '+':
        t = 0;
        break;
      case '2':
        t = 1;
        break;
      case '3':
      case '\'':
      case '-':
        t = TWISTS - 1;
        break;
      default:
        return -1;
    }

    p = q + 1;

    return f * TWISTS + t;
  }

  void
  Cube::append_move(char*& p, int move)
  {
    append_face(p, move / TWISTS);
    *p++ = "123"[move % TWISTS];
    *p = 0;
  }

  std::vector<int>
  Cube::parse_move_sequence(const char*& p)
  {
    std::vector<int> ret;
    int move;

    while ((move = parse_move(p)) >= 0)
      ret.emplace_back(move);

    return ret;
  }

  void
  Cube::append_move_sequence(char*& p, const std::vector<int>& seq)
  {
    *p = 0;
    for (unsigned int i = 0; i < seq.size(); i++)
      append_move(p, seq[i]);
  }

  char*
  Cube::move_sequence_string(const std::vector<int>& seq)
  {
    if (seq.size() > 65)
      die("Sequence too long");

    char* p = static_buff_;

    append_move_sequence(p, seq);

    return static_buff_;
  }

  const char*
  Cube::parse_sing(const char* p)
  {
    if (std::strncmp(p, "SING ", 5) == 0)
      p += 5;

    int m = 0;
    for (int i = 0; i < 12; i++)
    {
      int c = parse_edge(p) ^ sing_edge_flipped_[i];

      if (c < 0)
        return "No such edge";

      edge_[edge_perm(c)] = edge_val(sing_edge_order_[i], edge_ori(c));
      m |= 1 << i;
    }

    for (int i = 0; i < 8; i++)
    {
      int cval = parse_corner(p);

      if (cval < 0)
        return "No such corner";

      cubie_[corner_perm(cval)] = corner_ori_sub(sing_corner_order_[i], cval);
      m |= 1 << (i + 12);
    }

    skip_whitespace(p);

    if (*p)
      return "Extra stuff";

    if (m != ((1 << 20) - 1))
      return "Missing at least one cubie";

    return 0;
  }

  char*
  Cube::sing_string() const
  {
    Cube cp = identity_cube_;
    invert_into(cp);

    char* p = static_buff_;

    for (int i = 0; i < 12; i++)
    {
      if (i != 0)
        *p++ = ' ';

      int j = sing_edge_order_[i];
      const char* q = sing_edges_[cp.edge_[j] ^ sing_edge_flipped_[i]];

      *p++ = *q++;
      *p++ = *q++;
    }

    for (int i = 0; i < 8; i++)
    {
      *p++ = ' ';

      int j = sing_corner_order_[i];
      const char* q = sing_corners_[cp.cubie_[j]];

      *p++ = *q++;
      *p++ = *q++;
      *p++ = *q++;
    }

    *p = 0;

    return static_buff_;
  }

  void
  Cube::remap_into(int m, Cube& dst) const
  {
    int mprime = invm_[m];

    for (int i = 0; i < 8; i++)
    {
      int c1 = rot_corner_[mprime][i];
      int c2 = corner_ori_add(cubie_[corner_perm(c1)], c1);
      dst.cubie_[i] = rot_corner_[m][c2];
    }

    for (int i = 0; i < 12; i++)
    {
      int c1 = rot_edge_[mprime][i * 2];
      int c2 = edge_ori_add(edge_[edge_perm(c1)], c1);
      dst.edge_[i] = rot_edge_[m][c2];
    }
  }

  void
  Cube::canon_into_48(Cube& dst) const
  {
    dst = *this;
    canon_into_48_aux(dst);
  }

  void
  Cube::canon_into_48_aux(Cube& dst) const
  {
    for (int m = 1; m < M; m++)
    {
      bool stop = false;
      int mprime = invm_[m];
      bool is_less = false;

      for (int i = 0; i < 8; i++)
      {
        int c1 = rot_corner_[mprime][i];
        int c2 = corner_ori_add(cubie_[corner_perm(c1)], c1);
        int t = rot_corner_[m][c2];

        if (is_less || (t < dst.cubie_[i]))
        {
          dst.cubie_[i] = t;
          is_less = true;
        }
        else if (t > dst.cubie_[i])
        {
          stop = true;
          break;
        }
      }

      if (stop)
        continue;

      for (int i = 0; i < 12; i++)
      {
        int c1 = rot_edge_[mprime][i * 2];
        int c2 = edge_ori_add(edge_[edge_perm(c1)], c1);
        int t = rot_edge_[m][c2];

        if (is_less || (t < dst.edge_[i]))
        {
          dst.edge_[i] = t;
          is_less = true;
        }
        else if (t > dst.edge_[i])
          break;
      }
    }
  }

  void
  Cube::canon_into_96(Cube& dst) const
  {
    Cube cpi = identity_cube_;

    invert_into(cpi);
    if (*this < cpi)
      dst = *this;
    else
      dst = cpi;

    canon_into_48_aux(dst);
    cpi.canon_into_48_aux(dst);
  }

  void
  Cube::randomize()
  {
    int parity = 0;
    for (int i = 0; i < 7; i++)
    {
      int j = i + static_cast<int>((8 - i) * my_rand());
      if (i != j)
      {
        std::swap(cubie_[i], cubie_[j]);
        parity++;
      }
    }

    for (int i = 0; i < 11; i++)
    {
      int j = i + static_cast<int>((12 - i) * my_rand());
      if (i != j)
      {
        std::swap(edge_[i], edge_[j]);
        parity++;
      }
    }

    if (parity & 1)
      std::swap(edge_[10], edge_[11]);

    int s = 24;
    for (int i = 0; i < 7; i++)
    {
      int a = static_cast<int>(3 * my_rand());
      s -= a;
      cubie_[i] = corner_val(corner_perm(cubie_[i]), a);
    }

    cubie_[7] = corner_val(corner_perm(cubie_[7]), s % 3);
    s = 0;

    for (int i = 0; i < 11; i++)
    {
      int a = static_cast<int>(2 * my_rand());
      edge_[i] = edge_ori_add(edge_[i], a);
      s ^= a;
    }

    edge_[11] ^= s;
  }

  void
  Cube::error(const char* s)
  {
    std::cerr << s << std::endl;
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
