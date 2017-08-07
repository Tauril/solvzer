/**
 ** \file cube/search.hh
 ** \author Guillaume Marques
 ** \version 1.0
 ** \brief Declaration of cube::Search.
 **/

#pragma once

#include <cube/coord.hh>
#include <cube/cube.hh>
#include <cube/face.hh>
#include <cube/move.hh>

namespace cube
{

  /** \class Search
   ** \brief Implements the logic behind the solving algorithm.
   **/
  class Search
  {
  public:
    /** \name Ctor.
     ** \{ */
    /// Initializes the search.
    Search();
    /** \} */

    /** \name Public methods.
     ** \{ */
    /// Searches solution from a state \a facelets.
    std::string solution(const std::string& facelets, int max_depth);
    /** \brief Implementation of the Kociemba solving algorithm of
     **        the Rubik's Cube. The implementation was taken on
     **       <a href="kociemba.org/math/imptwophase.htm">kociemba</a>
     **/
    std::string solution(const Face& face, int max_depth);
    /// Checks the solution \a solution resolves state \a state.
    bool ack_solution(const std::string& state, const std::string& solution);
    bool ack_solution(const Face& face, const std::string& solution);
    /** \} */

    /** \name Public attributes.
     ** \{ */
    /// The max depth of the solving algorithm.
    static constexpr int DEPTH = 21;
    /** \} */

  private:
    /** \name Private methods.
     ** \{ */
    int total_depth(int depth_phase1, int max_depth);
    std::string solution_to_string(int length);
    /** \} */

    /** \name Private attributes.
     ** \{ */
    std::array<int, 31> axis_;
    std::array<int, 31> power_;
    std::array<int, 31> flip_;
    std::array<int, 31> twist_;
    std::array<int, 31> slice_;
    std::array<int, 31> parity_;
    std::array<int, 31> URF_to_DLF_;
    std::array<int, 31> FR_to_BR_;
    std::array<int, 31> UR_to_UL_;
    std::array<int, 31> UB_to_DF_;
    std::array<int, 31> UR_to_DF_;
    std::array<int, 31> min_depth_phase1_;
    std::array<int, 31> min_depth_phase2_;
    /** \} */
  };

} // namespace cube
