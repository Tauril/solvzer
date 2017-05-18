#include <cube/facelet.hh>
#include <misc/contract.hh>

namespace cube
{

  facelet
  get_facelet(int f)
  {
    switch (f)
    {
      case 0:
        return facelet::U1;
      case 1:
        return facelet::U2;
      case 2:
        return facelet::U3;
      case 3:
        return facelet::U4;
      case 4:
        return facelet::U5;
      case 5:
        return facelet::U6;
      case 6:
        return facelet::U7;
      case 7:
        return facelet::U8;
      case 8:
        return facelet::U9;
      case 9:
        return facelet::F1;
      case 10:
        return facelet::F2;
      case 11:
        return facelet::F3;
      case 12:
        return facelet::F4;
      case 13:
        return facelet::F5;
      case 14:
        return facelet::F6;
      case 15:
        return facelet::F7;
      case 16:
        return facelet::F8;
      case 17:
        return facelet::F9;
      case 18:
        return facelet::R1;
      case 19:
        return facelet::R2;
      case 20:
        return facelet::R3;
      case 21:
        return facelet::R4;
      case 22:
        return facelet::R5;
      case 23:
        return facelet::R6;
      case 24:
        return facelet::R7;
      case 25:
        return facelet::R8;
      case 26:
        return facelet::R9;
      case 27:
        return facelet::D1;
      case 28:
        return facelet::D2;
      case 29:
        return facelet::D3;
      case 30:
        return facelet::D4;
      case 31:
        return facelet::D5;
      case 32:
        return facelet::D6;
      case 33:
        return facelet::D7;
      case 34:
        return facelet::D8;
      case 35:
        return facelet::D9;
      case 36:
        return facelet::B1;
      case 37:
        return facelet::B2;
      case 38:
        return facelet::B3;
      case 39:
        return facelet::B4;
      case 40:
        return facelet::B5;
      case 41:
        return facelet::B6;
      case 42:
        return facelet::B7;
      case 43:
        return facelet::B8;
      case 44:
        return facelet::B9;
      case 45:
        return facelet::L1;
      case 46:
        return facelet::L2;
      case 47:
        return facelet::L3;
      case 48:
        return facelet::L4;
      case 49:
        return facelet::L5;
      case 50:
        return facelet::L6;
      case 51:
        return facelet::L7;
      case 52:
        return facelet::L8;
      case 53:
        return facelet::L9;
      default:
        die("Can't match an integer to a facelet");
    }
  }

  std::ostream&
  operator<<(std::ostream& o, const facelet& f)
  {
    return o << static_cast<int>(f);
  }

} // namespace cube
