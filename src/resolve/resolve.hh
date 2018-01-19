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


/*
 * RESOLVE_HH
 *  Handle physical resolution of the cube
 */
namespace Resolution
{

    class Resolve
    {
        public:
            // Initialisation of faces from state of the cube (str)
            Resolve(std::string& str);
            // Lunch the physical resolution of cube
            void resolve_cube();

        private:
            cube::Face faces_;
            std::string solutions_;

    };

    /*
     *  Take into parameter the move to effectuate.
     *  Because two moves can not be immeditaly done one after the other,
     *  sleep for an amount of secondes.
     */
    bool move_step_motor(std::pair<int, int> move);

    /*
     * MOTORS PIN
     * Related pin for each motor to the raspberry pi.
     * First it's direction's pin then it's step's pin
     */
    static constexpr int motorU_[] = {19, 26}; // WHITE
    static constexpr int motorL_[] = {27, 22}; // GREEN
    static constexpr int motorF_[] = {6, 13};  // RED
    static constexpr int motorR_[] = {20, 21}; // BLUE
    static constexpr int motorB_[] = {17, 18}; // ORANGE/BLACK
    static constexpr int motorD_[] = {23, 24}; // YELLOW
    /*
     *  MOTORS ROTATIONS
     *  custome rotation for each motors, index 0 one rotation, index 1 two
     *  rotations
     */
    static constexpr int rotateU_[] = {405, 805}; // WHITE
    static constexpr int rotateL_[] = {405, 805}; // GREEN
    static constexpr int rotateF_[] = {405, 805};  // RED
    static constexpr int rotateR_[] = {200, 400}; // BLUE
    static constexpr int rotateB_[] = {405, 805}; // ORANGE/BLACK
    static constexpr int rotateD_[] = {405, 805}; // YELLOW

}
