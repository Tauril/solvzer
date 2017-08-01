/**
 ** \file cube/facelet.cc
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Implementation for cube/facelet.hh
 **/

#include <map>

#include <cube/facelet.hh>
#include <misc/contract.hh>

namespace cube
{

  facelet
  get_facelet(int f)
  {
    static const std::map<int, facelet> facelets =
    {
      { 0,  facelet::U1 }, { 1,  facelet::U2 }, { 2,  facelet::U3 },
      { 3,  facelet::U4 }, { 4,  facelet::U5 }, { 5,  facelet::U6 },
      { 6,  facelet::U7 }, { 7,  facelet::U8 }, { 8,  facelet::U9 },

      { 9,  facelet::R1 }, { 10, facelet::R2 }, { 11, facelet::R3 },
      { 12, facelet::R4 }, { 13, facelet::R5 }, { 14, facelet::R6 },
      { 15, facelet::R7 }, { 16, facelet::R8 }, { 17, facelet::R9 },

      { 18, facelet::F1 }, { 19, facelet::F2 }, { 20, facelet::F3 },
      { 21, facelet::F4 }, { 22, facelet::F5 }, { 23, facelet::F6 },
      { 24, facelet::F7 }, { 25, facelet::F8 }, { 26, facelet::F9 },

      { 27, facelet::D1 }, { 28, facelet::D2 }, { 29, facelet::D3 },
      { 30, facelet::D4 }, { 31, facelet::D5 }, { 32, facelet::D6 },
      { 33, facelet::D7 }, { 34, facelet::D8 }, { 35, facelet::D9 },

      { 36, facelet::L1 }, { 37, facelet::L2 }, { 38, facelet::L3 },
      { 39, facelet::L4 }, { 40, facelet::L5 }, { 41, facelet::L6 },
      { 42, facelet::L7 }, { 43, facelet::L8 }, { 44, facelet::L9 },

      { 45, facelet::B1 }, { 46, facelet::B2 }, { 47, facelet::B3 },
      { 48, facelet::B4 }, { 49, facelet::B5 }, { 50, facelet::B6 },
      { 51, facelet::B7 }, { 52, facelet::B8 }, { 53, facelet::B9 }
    };

    auto fclt = facelets.find(f);
    if (fclt == facelets.end())
      die("Can't match an integer to a facelet");

    return fclt->second;
  }

  std::ostream&
  operator<<(std::ostream& o, const facelet& f)
  {
    return o << static_cast<int>(f);
  }

} // namespace cube
