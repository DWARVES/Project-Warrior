
#include "audio/audio.hpp"
#include "graphics/graphics.hpp"
#include "geometry/aabb.hpp"
#include "core/logger.hpp"
#include <iostream>

class CB : public audio::Audio::Callback
{
    public:
        virtual ~CB() {
            /* Do nothing */
        }
        virtual void operator()(audio::Audio* a) {
            std::cout << "The music ended. It had a playing frequence of " << a->frequency() << std::endl;
        }
};

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    graphics::Graphics* gfx = new graphics::Graphics;
    audio::Audio* aud = new audio::Audio;

    bool cont = true;
    SDL_Event ev;

    if(SDL_Init(SDL_INIT_VIDEO) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    cont = gfx->openWindow("Test graphics and audio", 800, 600);
    if(!cont)
        return 1;
    cont = aud->init(44100);
    if(!cont)
        return 1;
    if(!gfx->loadFont("font", "font.png"))
        return 1;
    aud->createNamespace("musics");
    aud->enterNamespace("musics");
    if(!aud->loadRawMusic("raw", "music.ogg"))
        return 1;
    if(!aud->loadMusic("mus", "rc/mus.2fs"))
        return 1;
    aud->enterNamespace("/");
    aud->createNamespace("sounds");
    aud->enterNamespace("sounds");
    if(!aud->loadSound("snd", "sound.wav"))
        return 1;

    geometry::AABB bg(800, 600);
    graphics::Color bgc(127, 127, 255);

    CB cb;
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
                        case SDLK_a:
                            aud->enterNamespace("/sounds");
                            aud->play("snd");
                            break;
                        case SDLK_RETURN:
                            aud->enterNamespace("/musics");
                            aud->play("raw", 1, &cb);
                            break;
                        case SDLK_SPACE:
                            aud->enterNamespace("/musics");
                            aud->play("mus", 3);
                            break;
                        case SDLK_e:
                            aud->enterNamespace("/musics");
                            aud->end("mus");
                            break;
                        default:
                            break;
                    }
                    break;
                default:
                    break;
            }
        }

        aud->update();
        gfx->beginDraw();
        gfx->draw(bg, bgc);
        gfx->move(20.0f, 250.0f);
        gfx->draw("Press A for sound !", "font", 20);
        gfx->endDraw();
    }

    delete gfx;
    delete aud;
    core::logger::free();
    SDL_Quit();
    return 0;
}

