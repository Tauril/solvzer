#include "resolve.hh"

namespace Resolution
{

    Resolve::Resolve(std::string& str) : faces_(str)
    {}

    void Resolve::resolve_cube()
    {
        cube::Search search;
        solutions_ = search.solution(faces_, 21);
        assert(search.ack_solution(faces_, solutions_));
    }

    bool move_step_motor(std::pair<int, int> move)
    {
        int nb_rotation = 0;
        bool rotate_right = false;

        if (move.second == 1)
            nb_rotation =  1;
        else if (move.second == 2)
            nb_rotation = 2;
        else if (move.second == 3)
        {
            rotate_right = true;
            nb_rotation = 1;
        }

        // These two motors need more power to turn on the left.
        if ((move.first == cube::color::F || move.first == cube::color::R)
            && rotate_right)
            nb_rotation += 10;

        std::stringstream str;
        str << "python step_motor_final.py "
            << (rotate_right ? "right" : "left");

        switch (move.first)
        {
            case cube::color::U:
                str << " " << (nb_rotation == 1 ? rotateU_[0] : rotateU_[1])
                    << " " <<motorU_[0] << " " << motorU_[1];
                break;
            case cube::color::R:
                str << " " << (nb_rotation == 1 ? rotateR_[0] : rotateR_[1])
                    << " " << motorR_[0] << " " << motorR_[1];
                break;
            case cube::color::F:
                str << " " << (nb_rotation == 1 ? rotateF_[0] : rotateF_[1])
                    << " " <<motorF_[0] << " " << motorF_[1];
                break;
            case cube::color::D:
                str << " " << (nb_rotation == 1 ? rotateD_[0] : rotateD_[1])
                    << " " <<motorD_[0] << " " << motorD_[1];
                break;

                break;
            case cube::color::L:
                str << " " << (nb_rotation == 1 ? rotateL_[0] : rotateL_[1])
                    << " " <<motorL_[0] << " " << motorL_[1];
                break;

                break;
            case cube::color::B:
                str << " " << (nb_rotation == 1 ? rotateB_[0] : rotateB_[1])
                    << " " <<motorB_[0] << " " << motorB_[1];
                break;

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
