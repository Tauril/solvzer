#pragma once

#include <array>

#include <cube/color.hh>

namespace cube
{

  class Face
  {
    public:
      using face_t = std::array<color, 9>;

      Face() = default;
      Face(std::array<int, 9>& face);
      Face(face_t& face);

      const face_t& face_get() const;
      face_t& face_get();

    private:
      face_t face_;
  };

} // namespace cube

#include <cube/face.hxx>
