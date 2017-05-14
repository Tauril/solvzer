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

  inline
  Face::const_iterator
  Face::begin() const
  {
    return face_.begin();
  }

  inline
  Face::const_iterator
  Face::end() const
  {
    return face_.end();
  }

} // namespace cube
