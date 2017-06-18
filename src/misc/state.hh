#pragma once

#include "cube/face.hh"
#include "cube/search.hh"

namespace state
{
  class State
  {
    public:
      ~State();
      State(const State& s) = delete;
      State& operator =(const State& s) = delete;
      cube::Search& search_get();
      cube::Face& face_get();
      void search_set(cube::Search& s);
      void face_set(cube::Face& s);
      std::string move_get();
      const std::string& face_str_get() const;
      void face_str_set(const std::string& state);
      void compute_time_set(const double d);
      double compute_time_get() const;
      static State& Instance();
      void draw_text_data() const;

    private:
      State();

    private:
      cube::Search search_;
      cube::Face face_;
      double compute_time_;

    public:
      bool draw_;
  };
}
