
#include "graphics/color.hpp"
#include <iostream>

int main()
{
    graphics::Color c;
    c.set(0.5f, 0.5f, 0.5f);
    std::cout << "(r:g:b:a) : (" << (int)c.r << ":" << (int)c.g << ":" << (int)c.b << ":" << (int)c.a << ")" << std::endl;
    c.r = 255;
    c.g = 0;
    c.b = 64;
    c.a = 58;
    std::cout << "float(r:g:b:a) : (" << c.fr() << ":" << c.fg() << ":" << c.fb() << ":" << c.fa() << ")" << std::endl;
    return 0;
}

