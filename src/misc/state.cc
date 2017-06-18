#include <sstream>
#include "state.hh"
#include "display.hh"

namespace state
{
  State::State()
  : draw_(false)
  {}

  State::~State()
  {}

  State&
  State::Instance()
  {
    static State s;
    return s;
  }

  cube::Search&
  State::search_get()
  {
    return search_;
  }

  cube::Face&
  State::face_get()
  {
    return face_;
  }

  std::string
  State::move_get()
  {
    return search_.solution(face_.face_str_get(), cube::Search::DEPTH);
  }

  void
  State::search_set(cube::Search& s)
  {
    search_ = s;
  }

  void
  State::face_set(cube::Face& f)
  {
    face_ = f;
  }

  const std::string&
  State::face_str_get() const
  {
    return face_.face_str_get();
  }

  void
  State::face_str_set(const std::string& state)
  {
    face_.face_str_set(state);
  }

  void
  State::compute_time_set(const double d)
  {
    compute_time_ = d;
  }

  double
  State::compute_time_get() const
  {
    return compute_time_;
  }

  void
  State::draw_text_data() const
  {
    auto& dis = display::Display::Instance();
    std::pair<int, int> size;
    dis.window_size_get(size);

    // Draw compute time
    std::ostringstream strs;
    strs << compute_time_;
    std::array<uint8_t, 3> color{{255, 0, 0}};
    dis.draw_text("Computation time: " + strs.str(), color, size.first - 450, size.second -  150);
  }
}
