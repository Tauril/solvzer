#include <misc/indent.hh>

namespace misc
{

  std::ostream&
  indent(std::ostream& o)
  {
    int indent = 12;
    char fill = o.fill(' ');

    o << std::setw(indent)
      << ""
      << std::setfill(fill);

    return o;
  }

} // namespace misc
