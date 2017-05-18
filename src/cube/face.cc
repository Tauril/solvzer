#include <cube/face.hh>
#include <misc/contract.hh>

namespace cube
{

  Face::Face(const std::string& cube)
  {
    precondition(cube.size() == 54);

    for (unsigned i = 0; i < cube.size(); i++)
      face_[i] = get_color(cube[i]);
  }

} // namespace cube
