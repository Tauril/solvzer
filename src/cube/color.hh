/**
 ** \file cube/color.hh
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Declaration of cube::color.
 **/

#pragma once

#include <ostream>

namespace cube
{

  /** \enum  color
   ** \brief Colors are represented by their positions. See cube.hh for
   **        a more precise description.
   **
   ** We get the locations in the URFDLB order.
   **/
  enum color
  {
    U,  // 0
    R,  // 1
    F,  // 2
    D,  // 3
    L,  // 4
    B,  // 5
    UNKNOWN // Error, used for the detection
  };

  /// \brief      Returns a color from its ASCII representation.
  /// \param c    The ASCII representation.
  /// \return     The color.
  color get_color(char c);

  /// \brief      Returns an ASCII representation from a color
  /// \param c    The color.
  /// \return     The ASCII representation.
  char get_char_from_color(color c);

  /// \brief      Returns a human readable string for the enum representation.
  /// \param c    The enum representation.
  /// \return     The string representing the color.
  std::string color_to_str(enum color c);

  /// Reports \a c on \a o for debugging purpose.
  std::ostream& operator<<(std::ostream& o, const color& c);

} // namespace cube
