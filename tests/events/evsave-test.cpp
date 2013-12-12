
#include <iostream>
#include <algorithm>
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"
#include "events/keysave.hpp"
#include "events/joybuttonsave.hpp"
#include "events/joyhatsave.hpp"
#include "events/joyaxissave.hpp"

int main()
{
    core::logger::init();
    core::logger::addOutput(&std::cout);
    graphics::Graphics* gfx = new graphics::Graphics;

    events::Events ev;
    bool cont = true;

    if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK) < 0) {
        std::cout << "Couldn't load SDL : " << SDL_GetError() << std::endl;
        return 1;
    }

    cont = gfx->openWindow("Test events::Events joysticks handling", 800, 600);
    if(!cont)
        return 1;

    /* Background */
    geometry::AABB bgaabb(800.0f, 600.0f);
    graphics::Color bgc;
    bgc.r = bgc.g = 127;
    bgc.b = 255;

    /* Particular events. */
    events::KeySave kqs (events::Key('Q'));
    events::KeySave others (events::Key(' '));
    size_t kq = ev.addSaved(&kqs);
    ev.addSaved(&others);

    events::Joystick* joystick = ev.openJoystick(0);
    if(!joystick) {
        std::cout << "There must be a joystick." << std::endl;
        return 1;
    }
    events::JoyButtonSave jbs;
    jbs.set(joystick, 0);
    ev.addSaved(&jbs);
    events::JoyHatSave jhs;
    jhs.set(joystick, 0, events::JoyHatState::Up);
    ev.addSaved(&jhs);
    events::JoyAxisSave jas;
    jas.set(joystick, 0, -32768);
    ev.addSaved(&jas);

    ev.enableInput(true);
    while(cont) {
        ev.update();
        ev.clearInput();
        if(ev.closed() || ev.quit() || ev.isSavedValid(kq))
            cont = false;

        std::vector<size_t> l = ev.lastSavedValidated();
        if(!l.empty()) {
            std::cout << l.size() << " events have been validated." << std::endl;
            for(size_t id : l) {
                std::cout << "\t> " << id << " : " << ev.getSaved(id)->save() << " at " << ev.lastSavedValid(id) << std::endl;
            }
        }
        l = ev.lastSavedReleased();
        if(!l.empty()) {
            std::cout << l.size() << " events have been released." << std::endl;
            for(size_t id : l) {
                std::cout << "\t> " << id << " : " << ev.getSaved(id)->save() << " at " << ev.lastSavedRelease(id) << std::endl;
            }
        }

        gfx->beginDraw();
        gfx->draw(bgaabb, bgc);
        gfx->endDraw();

        SDL_Delay(1000/30);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

