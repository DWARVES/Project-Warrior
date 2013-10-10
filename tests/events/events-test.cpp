
#include <iostream>
#include "core/logger.hpp"
#include "graphics/graphics.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

void dumpKeys(std::vector<events::Key> ks, std::string action);
void dumpButtons(std::vector<events::Button> bs, std::string action);
void dumpDropped(std::vector<std::string> ds);
void dumpInput(const std::string& l, const std::string& f);
void dumpAxis(std::vector<int> axis, events::Joystick* j);
void dumpHats(std::vector<int> hats, events::Joystick* j);
void dumpJoyButtons(std::vector<int> buttons, events::Joystick* j, std::string action);

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

    cont = gfx->openWindow("Test events::Events", 800, 600);
    if(!cont)
        return 1;

    /* Joysticks */
    std::cout << "There are " << ev.numJoysticks() << " joysticks available." << std::endl;
    events::Joystick* joy = NULL;
    if(ev.numJoysticks() > 0) {
        std::cout << "Could open the first joystick ? "
            << ((joy = ev.openJoystick(0)) ? "yes" : "no") << std::endl;
    }

    /* Background */
    geometry::AABB bgaabb(800.0f, 600.0f);
    graphics::Color bgc;
    bgc.r = bgc.g = 127;
    bgc.b = 255;

    ev.enableInput(true);
    while(cont) {
        ev.update();
        if(ev.closed() || ev.quit() || ev.isKeyPressed(events::Key('Q')))
            cont = false;
        dumpKeys(ev.lastKeysPressed(), "pressed");
        dumpKeys(ev.lastKeysReleased(), "released");
        dumpButtons(ev.lastButtonsPressed(), "pressed");
        dumpButtons(ev.lastButtonsReleased(), "released");
        dumpInput(ev.lastInput(), ev.fullInput());
        if(ev.isKeyPressed(events::KeyMap::Escape))
            ev.clearInput();
        if(joy) {
            dumpAxis(ev.lastAxesMoved(joy), joy);
            dumpHats(ev.lastHatsMoved(joy), joy);
            dumpJoyButtons(ev.lastJoyButtonsPressed(joy), joy, "pressed");
            dumpJoyButtons(ev.lastJoyButtonsReleased(joy), joy, "released");
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

void dumpKeys(std::vector<events::Key> ks, std::string action)
{
    for(size_t i = 0; i < ks.size(); ++i) {
        std::cout << "Key " << ks[i].name() << " (" << ks[i].posName() << ") just " << action << std::endl;
    }
}

void dumpButtons(std::vector<events::Button> bs, std::string action)
{
    for(size_t i = 0; i < bs.size(); ++i) {
        std::cout << "Button " << (int)bs[i] << " just " << action << std::endl;
    }
}

void dumpDropped(std::vector<std::string> ds)
{
    for(size_t i = 0; i < ds.size(); ++i) {
        std::cout << "File \"" << ds[i] << "\" just dropped." << std::endl;
    }
}

void dumpInput(const std::string& l, const std::string& f)
{
    if(l.empty())
        return;
    std::cout << "Last input : \"" << l << "\" (full : \"" << f << "\")." << std::endl;
}

void dumpAxis(std::vector<int> axis, events::Joystick* j)
{
    for(int a : axis) {
        std::cout << "Axis #" << a << " moved to " << j->axis(a) << std::endl;
    }
}

void dumpHats(std::vector<int> hats, events::Joystick* j)
{
    for(int h : hats) {
        std::cout << "Hat #" << h << " changed to \"";
        switch(j->hat(h)) {
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

void dumpJoyButtons(std::vector<int> buttons, events::Joystick* j, std::string action)
{
    for(int b : buttons) {
        std::cout << "Button #" << b << " " << action << " in joystick #" << j->id() << std::endl;
    }
}

