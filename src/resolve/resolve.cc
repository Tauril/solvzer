#include "resolve.hh"

namespace Resolution
{

    Resolve::Resolve()
    {
        faces_.face_set(cube::Cube::solved_state_);
        faces_.scramble();
    }

    Resolve::Resolve(std::string& str) : faces_(str)
    {}

    void Resolve::find_solution()
    {
        cube::Search search;
        solutions_ = search.solution(faces_, 21);
        assert(search.ack_solution(faces_, solutions_));
    }

    void Resolve::resolve_cube()
    {
        this->find_solution();
        const char* moves = solutions_.c_str();
        std::pair<int, int> move;
        while (*moves)
        {
            move = cube::move::parse_move(moves);
            if (!this->move_step_motor(move))
                exit(1);
        }
    }

    bool Resolve::move_step_motor(std::pair<int, int> move)
    {
        int nb_rotation = 0;
        bool rotate_right = false;

        if (move.second == 1)
            nb_rotation = nb_rotate_for_90_dg;
        else if (move.second == 2)
            nb_rotation = 2 * nb_rotate_for_90_dg;
        else if (move.second == 3)
        {
            rotate_right = true;
            nb_rotation = nb_rotate_for_90_dg;
        }

        // These two motors need more power to turn on the left.
        if ((move.first == cube::color::F || move.first == cube::color::R)
            && rotate_right)
            nb_rotation += 10;

        std::stringstream str;
        str << "python step_motor_final.py "
            << (rotate_right ? "right" : "left")
            << " " << nb_rotation << " ";

        switch (move.first)
        {
            case cube::color::U:
                str << motorU_[0] << " " << motorU_[1];
                break;
            case cube::color::R:
                str << motorR_[0] << " " << motorR_[1];
                break;
            case cube::color::F:
                str << motorF_[0] << " " << motorF_[1];
                break;
            case cube::color::D:
                str << motorD_[0] << " " << motorD_[1];
                break;
            case cube::color::L:
                str << motorL_[0] << " " << motorL_[1];
                break;
            case cube::color::B:
                str << motorB_[0] << " " << motorB_[1];
                break;
            default:
                exit(4);
                break;

        }

        printf("instruction <%s>\n", str.str().c_str());
        int ret = std::system(str.str().c_str());
        printf("return value %d\n", ret);

        std::this_thread::sleep_for(std::chrono::seconds(3));
       return true; 
    }
}
