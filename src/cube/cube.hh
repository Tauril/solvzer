#pragma once

#include <array>
#include <iostream>
#include <vector>

#include <misc/contract.hh>

namespace cube
{

  /*
  ** Cube faces' locations
  **
  **      0   1   2   3   4   5   6   7   8   9   10  11
  **
  **                    `Up`
  **
  **                `---'---'---`
  **  0             | 0 | 1 | 2 |
  **                `---'---'---`
  **  1             | 3 | 4 | 5 |
  **      `Left`    `---'---'---`   `Right` |   `Back`
  **  2             | 6 | 7 | 8 |
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  3 | 45| 46| 47| 9 | 10| 11| 18| 19| 20| 36| 37| 38|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  4 | 48| 49| 50| 12| 13| 14| 21| 22| 23| 39| 40| 41|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  5 | 51| 52| 53| 15| 16| 17| 24| 25| 26| 42| 43| 44|
  **    `---'---'---'---'---'---'---'---'---'---'---'---`
  **  6             | 27| 28| 29|
  **                `---'---'---`
  **  7             | 30| 31| 32|
  **                `---'---'---`
  **  8             | 33| 34| 35|
  **                `---'---'---`
  **
  **                    `Down`
  */

  constexpr int NMOVES = 18;
  constexpr int TWISTS = 3;
  constexpr int FACES = 6;
  constexpr int M = 48;
  constexpr int CUBIES = 24;
  constexpr int ALLMOVEMASK = (1 << NMOVES) - 1;
  constexpr int ALLMOVEMASK_EXT = (1 << NMOVES) - 1;
  constexpr int CANONSEQSTATES = FACES + 1;
  constexpr int CANONSEQSTART = 0;

  class Cube
  {
    public:
      Cube(Cube* cube);

      bool operator<(const Cube& cube) const;
      bool operator==(const Cube& cube) const;
      bool operator!=(const Cube& cube) const;

      /// Convenient to go to and from separate permutation/orientation
      /// to combined cube values, and from cube values to permutations
      /// and orientations.
      static int edge_perm(int cubie_val);
      static int edge_ori(int cubie_val);
      static int corner_perm(int cubie_val);
      static int corner_ori(int cubie_val);
      static int edge_flip(int cubie_val);
      static int edge_val(int perm, int ori);
      static int corner_val(int perm, int ori);

      /// Add the orientation (only) from one cubieval to another one
      /// (for both corners and edges).
      static int edge_ori_add(int cubie_val1, int cubie_val2);
      static int corner_ori_add(int cubie_val1, int cubie_val2);
      static int corner_ori_sub(int cubie_val1, int cubie_val2);

      /// Initialization declaration.
      static void init();

      /// Cubie to slot map move.
      void move(int move);
      static int invert_move(int move);
      static std::vector<int> invert_sequence(const std::vector<int>& sequence);
      void invert_into(Cube& dst) const;

      /// Slot to cubie map move.
      void movesc(int move);

      /// Swap and four-cycle.
      void rot_2(unsigned char* cc, int a, int b);
      void rot_4(unsigned char* cc, int a, int b, int c, int d);
      void rot_2_2(unsigned char* cc, int a, int b, int c, int d);

      /// Flips.
      void edge_4_flip(int a, int b, int c, int d);
      void corner_4_flip(int a, int b, int c, int d);

      /// Multiplications
      static void mul(const Cube& c1, const Cube& c2, Cube& ret);
      static void mulsc(const Cube& c1, const Cube& c2, Cube& ret);

      /// Parsing.
      static void skip_whitespace(const char*& p);
      static int parse_face(const char*& p);
      static int parse_face(char f);
      static void append_face(char*& p, int f);
      static int parse_move(const char*& p);
      static void append_move(char*& p, int move);
      static std::vector<int> parse_move_sequence(const char*& p);
      static void append_move_sequence(char*& p, const std::vector<int>& seq);
      static char* move_sequence_string(const std::vector<int>& seq);

      /// Parsing Singmaster notation.
      const char* parse_sing(const char* p);
      char* sing_string() const;

      /// Remap and canonicalization.
      void remap_into(int m, Cube& dst) const;
      void canon_into_48(Cube& dst) const;
      void canon_into_48_aux(Cube& dst) const;
      void canon_into_96(Cube& dst) const;

      /// Get a random cube position.
      void randomize();

      /// Utility methods.
      static inline int next_cs(int cs, int mv)
      {
        return canon_seq_[cs][mv];
      }

      static inline int cs_mask(int cs)
      {
        return canon_seq_mask_[cs];
      }

      static inline int cs_mask_ext(int cs)
      {
        return canon_seq_mask_ext_[cs];
      }

      static inline double my_rand()
      {
        return static_cast<double>(rand()) / RAND_MAX;
      }

      static inline int random_move()
      {
        return static_cast<int>(NMOVES * my_rand());
      }

      double walltime();
      double duration();
      static void error(const char* s);

      static const Cube identity_cube_;

    public:
      unsigned char cubie_[8];
      unsigned char edge_[12];

      static unsigned char corner_ori_inc_[CUBIES];
      static unsigned char corner_ori_dec_[CUBIES];
      static unsigned char corner_ori_neg_strip_[CUBIES];
      static unsigned char mod_24_[2 * CUBIES];

      static unsigned char edge_trans_[NMOVES][CUBIES];
      static unsigned char corner_trans_[NMOVES][CUBIES];

      static unsigned char inv_move_[NMOVES];

      static char static_buff_[200];

      static unsigned char face_map_[M][FACES];
      static unsigned char move_map_[M][NMOVES];
      static unsigned char invm_[M];
      static unsigned char m_m_[M][M];
      static unsigned char rot_edge_[M][CUBIES];
      static unsigned char rot_corner_[M][CUBIES];

      static unsigned char canon_seq_[CANONSEQSTATES][NMOVES];
      static int canon_seq_mask_[CANONSEQSTATES];
      static int canon_seq_mask_ext_[CANONSEQSTATES];
  };

  std::ostream& operator<<(std::ostream& o, const Cube& c);

} // namespace cube

#include <cube/cube.hxx>
