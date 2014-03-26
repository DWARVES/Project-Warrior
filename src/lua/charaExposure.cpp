
#include "charaExposure.hpp"

namespace lua
{
    namespace exposure
    {
        gameplay::Character* characters[4];

        void exposeChara(Script* scr, int nb)
        {
            Character<0> chara0;
            Character<1> chara1;
            Character<2> chara2;
            Character<3> chara3;

            switch(nb) {
                case 0:  chara0.expose(scr); break;
                case 1:  chara1.expose(scr); break;
                case 2:  chara2.expose(scr); break;
                case 3:  chara3.expose(scr); break;
                default: chara0.expose(scr); break;
            }
        }
    }
}



