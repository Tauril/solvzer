#pragma once

#include <cube/face.hh>

namespace cube
{

  inline
  const Face::face_t&
  Face::face_get() const
  {
    return face_;
  }

  inline
  Face::face_t&
  Face::face_get()
  {
    return face_;
  }

} // namespace cube
