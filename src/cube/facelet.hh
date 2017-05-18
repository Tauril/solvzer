#pragma once

#include <array>
#include <ostream>

#include <cube/color.hh>

namespace cube
{

  enum class facelet
  {
  };

  facelet get_facelet(int f);

  std::ostream& operator<<(std::ostream& o, const Face& f);

} // namespace cube
