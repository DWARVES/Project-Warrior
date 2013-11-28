
#include <iostream>
#include <algorithm>
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

void dumpAxis(std::vector<std::pair<int,events::Joystick*>> axis);
void dumpHats(std::vector<std::pair<int,events::Joystick*>> hats);
void dumpJoyButtons(std::vector<std::pair<int,events::Joystick*>> buttons, std::string action);
void dumpAddedJoysticks(std::vector<events::JoystickID> devices);

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

    /* Open all joysticks */
    std::cout << "There are " << ev.numJoysticks() << " joysticks available." << std::endl;
    for(int i = 0; i < ev.numJoysticks(); ++i) {
        if(ev.openJoystick(i) != NULL)
            std::cout << "Opened joystick #" << i << std::endl;
    }

    /* Background */
    geometry::AABB bgaabb(800.0f, 600.0f);
    graphics::Color bgc;
    bgc.r = bgc.g = 127;
    bgc.b = 255;

    ev.enableInput(true);
    while(cont) {
        ev.update();
        ev.clearInput();
        if(ev.closed() || ev.quit() || ev.isKeyPressed(events::Key('Q')))
            cont = false;

        if(ev.joysticksChanged()) {
            std::cout << "Joysticks changed !" << std::endl;
            std::vector<events::JoystickID> plugged  = ev.lastJoysticksAdded();
            for(events::JoystickID id : plugged) {
                if(ev.openJoystick(id) != NULL)
                    std::cout << "Opened joystick #" << id << std::endl;
            }
            dumpAddedJoysticks(plugged);
        }

        dumpAxis(ev.lastAxesMoved());
        dumpHats(ev.lastHatsMoved());
        dumpJoyButtons(ev.lastJoyButtonsPressed(), "pressed");
        dumpJoyButtons(ev.lastJoyButtonsReleased(), "released");

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

void dumpAxis(std::vector<std::pair<int,events::Joystick*>> axis)
{
    for(std::pair<int,events::Joystick*> p : axis) {
        std::cout << "Axis #" << p.first << " moved to " << p.second->axis(p.first) << std::endl;
    }
}

void dumpHats(std::vector<std::pair<int,events::Joystick*>> hats)
{
    for(std::pair<int,events::Joystick*> p : hats) {
        std::cout << "Hat #" << p.first << " changed to \"";
        switch(p.second->hat(p.first)) {
            case events::JoyHatState::Center:    std::cout << "center";    break;
            case events::JoyHatState::Up:        std::cout << "up";        break;
            case events::JoyHatState::Down:      std::cout << "down";      break;
            case events::JoyHatState::Right:     std::cout << "right";     break;
            case events::JoyHatState::Left:      std::cout << "left";      break;
            case events::JoyHatState::RightUp:   std::cout << "rightup";   break;
            case events::JoyHatState::RightDown: std::cout << "rightdown"; break;
            case events::JoyHatState::LeftUp:    std::cout << "leftup";    break;
            case events::JoyHatState::LeftDown:  std::cout << "leftdown";  break;
            default:                             std::cout << "unknown";   break;
        }
        std::cout << "\"" << std::endl;
    }
}

void dumpJoyButtons(std::vector<std::pair<int,events::Joystick*>> buttons, std::string action)
{
    for(std::pair<int,events::Joystick*> p : buttons) {
        std::cout << "Button #" << p.first << " " << action << " in joystick #" << p.second->id() << std::endl;
    }
}

void dumpAddedJoysticks(std::vector<events::JoystickID> devices)
{
    for(events::JoystickID d : devices) {
        std::cout << "Joystick #" << d << " was added !" << std::endl;
    }
}


