#include <cube/face.hh>

namespace cube
{

  inline
  void
  Face::face_set(const std::string& state)
  {
    precondition(state.size() == 54);

    std::map<color, int> cube_colors{};

    for (unsigned i = 0; i < state.size(); i++)
    {
      color c = get_color(state[i]);
      face_[i] = c;
      cube_colors[c] += 1;
    }

    face_str_set(state);

    postcondition(cube_colors.size() == 6);
    postcondition(std::all_of(cube_colors.begin(), cube_colors.end(),
                              [](const auto& m) { return m.second == 9; }));
  }

  inline
  const std::string&
  Face::face_str_get() const
  {
    return face_str_;
  }

  inline
  void
  Face::face_str_set(const std::string& state)
  {
    face_str_ = state;
  }

} // namespace cube
