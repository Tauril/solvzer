#pragma once

#include <array>
#include <ostream>

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

      using const_iterator = face_t::const_iterator;
      const_iterator begin() const;
      const_iterator end() const;

      std::ostream& dump_row(std::ostream& o, int i, int x) const;

    private:
      face_t face_;
  };

  std::ostream& operator<<(std::ostream& o, const Face& f);

} // namespace cube

#include <cube/face.hxx>
