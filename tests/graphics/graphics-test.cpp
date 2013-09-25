
#include <SDL.h>
#include <iostream>
#include <sstream>
#include "core/logger.hpp"
#include "graphics/graphics.hpp"

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    graphics::Graphics* gfx = new graphics::Graphics;

    bool cont = true;
    SDL_Event ev;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    cont = gfx->openWindow("Test graphics::Graphics", 800, 600);
    if(!cont)
        return 1;
    std::cout << "Is virtual size setted ? " << gfx->isEnabled() << std::endl;
    gfx->setVirtualSize(300, 200);
    gfx->preserveRatio(true);
    std::cout << "Is virtual size setted ? " << gfx->isEnabled() << std::endl;
    gfx->defaultWidth(5.0f); /* Default width for points and lines */

    /* Useless, just for testing matrix stacking */
    gfx->move(2.0f, 5.0f);
    gfx->push();
    gfx->rotate(45);
    gfx->push();
    while(gfx->pop()) {
        std::cout << "Poping a matrix." << std::endl;
    }

    /* Textures */
    gfx->createNamespace("pictures");
    gfx->enterNamespace("pictures");
    if(!gfx->loadTexture("default", "img.png"))
        return 1;
    gfx->enterNamespace("/");
    gfx->createNamespace("textures");
    gfx->enterNamespace("textures");
    if(!gfx->loadTexture("default", "text.png"))
        return 1;
    if(!gfx->loadFont("font", "font.png"))
        return 1;
    graphics::Color c(255, 255, 255);
    if(!gfx->loadTextureFromText("text", "font", "HY EVERYBODY\nIT WORKS!", c, -1.0f, true))
        return 1;
    gfx->enterNamespace("/");

    /* Primitives */
    geometry::AABB aabb(60.0f, 60.0f);
    geometry::Line line;
    line.p1.x = 25.0f;
    line.p1.y = 100.0f;
    line.p2.x = 100.0f;
    line.p2.y = 25.0f;
    geometry::Polygon poly;
    poly.points.push_back(geometry::Point(.0f,.0f));
    poly.points.push_back(geometry::Point(30.0f,.0f));
    poly.points.push_back(geometry::Point(.0f,30.0f));
    geometry::Circle circle;
    circle.radius = 20.0f;

    graphics::Color color;
    color.g = 255;

    /* Background */
    geometry::AABB bgaabb(300.0f, 200.0f);
    graphics::Color bgc;
    bgc.r = bgc.g = 127;
    bgc.b = 255;

    while(cont) {
        while(SDL_PollEvent(&ev)) {
            switch(ev.type) {
                case SDL_QUIT:
                    cont = false;
                    break;
                case SDL_WINDOWEVENT:
                    if(ev.window.event == SDL_WINDOWEVENT_CLOSE)
                        cont = false;
                    break;
                case SDL_KEYDOWN:
                    switch(ev.key.keysym.sym) {
                        case SDLK_q:
                        case SDLK_ESCAPE:
                            cont = false;
                            break;
                        case SDLK_i:
                            gfx->invertYAxis(!gfx->isYAxisInverted());
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        gfx->beginDraw();
        gfx->draw(bgaabb, bgc);
        gfx->enterNamespace("/");
        gfx->enterNamespace("textures");
        gfx->draw("HELLO WORLD!\nIT WORKS!", "font", 20.0f);
        gfx->move(gfx->getVirtualWidth() / 2 - 30.0f, gfx->getVirtualHeight() / 2 - 30.0f);
        gfx->draw(aabb, "default", 2.0f, 3.0f);

        gfx->enterNamespace("/");
        gfx->enterNamespace("pictures");
        gfx->blitTexture("default", geometry::Point(5.0f, 5.0f));
        gfx->enterNamespace("/");
        gfx->enterNamespace("textures");
        gfx->draw(geometry::Point(-30.0f, -30.0f), color, 20);
        gfx->draw(line, color);
        gfx->blitTexture("text", geometry::Point(30.0f, 30.0f));

        gfx->push();
        gfx->move(70.0f, 70.0f);
        gfx->draw(poly, "default", 10.0f, 10.0f);
        gfx->pop();
        gfx->move(70.0f, -20.0f);
        gfx->draw(circle, "default");
        gfx->endDraw();

        SDL_Delay(1000/30);
    }

    gfx->preserveRatio(false); /* Just for testing logging */
    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

