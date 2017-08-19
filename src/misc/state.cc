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

  void
  State::search_set(cube::Search& s)
  {
    search_ = s;
  }

  cube::Face&
  State::face_get()
  {
    return face_;
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

  double
  State::compute_time_get() const
  {
    return compute_time_;
  }

  void
  State::compute_time_set(const double d)
  {
    compute_time_ = d;
  }

  std::string
  State::move_get()
  {
    return search_.solution(face_.face_str_get(), cube::Search::DEPTH);
  }

  void
  State::push_move(std::string move)
  {
    moves_.push_back(move);
  }

  void
  State::clear_vector()
  {
    moves_.clear();
  }

  const std::vector<std::string>&
  State::moves_get() const
  {
    return moves_;
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
    std::array<uint8_t, 3> color{{255, 255, 255}};
    dis.draw_text("Computation time: " + strs.str() + " seconds",
                   color, size.first - 1050, size.second -  150, 30);

    // Draw state
    dis.draw_text("State: ", color, size.first - 1050, size.second - 110, 30);
    dis.draw_text(face_str_get(), color, size.first - 950,
                  size.second - 100, 17);

    // Draw move
    if (moves_.size())
      draw_moves(moves_, size);
  }

  void
  draw_moves(const std::vector<std::string>& moves,
             const std::pair<int, int>& size)
  {
    auto& dis = display::Display::Instance();
    std::array<uint8_t, 3> color{{255, 255, 255}};
    auto size_x = size.first - 1050;
    dis.draw_text("Moves: ", color, size_x, size.second - 70, 30);
    size_x += 90;
    for (auto it = moves.begin(); it != std::prev(moves.end(), 1); ++it)
    {
      auto s = *it;
      s[1] == '1' ? s[1] = '\0' : s[1] = s[1];
      it->size() == 2 ? size_x += 40 : size_x += 20;
      dis.draw_text(s + " ", color, size_x, size.second - 63, 20);
    }
    size_x += 35;
    auto s = moves.back();
    s[1] == '1' ? s[1] = '\0' : s[1] = s[1];
    color[1] = 0;
    color[2] = 0;
    dis.draw_text(s, color, size_x, size.second - 70, 30);
  }
}
