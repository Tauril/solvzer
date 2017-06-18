#include "state.hh"

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
}
