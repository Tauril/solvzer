#include <cube/face.hh>

namespace cube
{

  Face::Face(std::array<int, 9>& face)
  {
    for (auto i = 0; i < 9; i++)
      face_[i] = get_color(face[i]);
  }

  Face::Face(face_t& face)
    : face_(face)
  {}

} // namespace cube
