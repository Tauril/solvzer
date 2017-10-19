#pragma once


#include <assert.h>
#include <stdlib.h>
#include <cstdlib>
#include <chrono>
#include <thread>
#include <sstream>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

#include "cube/cube.hh"
#include "cube/face.hh"
#include "cube/search.hh"
#include "cube/move.hh"
#include "cube/color.hh"

namespace Resolution
{

    class Resolve
    {
        public:
            Resolve();
            Resolve(std::string& str);
            void resolve_cube();

        private:
            cube::Face faces_;
            std::string solutions_;

    };

    bool move_step_motor(std::pair<int, int> move);
    static constexpr int motorU_[] = {19, 26}; // WHITE
    static constexpr int motorL_[] = {27, 22}; // GREEN
    static constexpr int motorF_[] = {6, 13};  // RED
    static constexpr int motorR_[] = {20, 21}; // BLUE
    static constexpr int motorB_[] = {17, 18}; // ORANGE/BLACK
    static constexpr int motorD_[] = {23, 24}; // YELLOW
    static constexpr int nb_rotate_for_90_dg = 420;
}
