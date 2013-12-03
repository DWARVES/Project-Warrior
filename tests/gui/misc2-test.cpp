
#include <iostream>
#include <sstream>
#include "gui/gui.hpp"
#include "gui/widget.hpp"
#include "gui/frame.hpp"
#include "gui/list.hpp"
#include "gui/gridlayout.hpp"
#include "gui/theme.hpp"
#include "graphics/graphics.hpp"
#include "graphics/color.hpp"
#include "geometry/aabb.hpp"
#include "core/logger.hpp"
#include "events/events.hpp"
#include "events/key.hpp"
#include "events/keymap.hpp"

enum class Evs : size_t {
    Up, Down, Left, Right,
    Select, Back, First,
    Last, Quit, NB
};
struct EvSt {
    std::string name;
    enum Dev {
        Key, Axis, Button, Hat, None
    };
    Dev dev;
    int id;
};

void updateDevices(gui::List* l, events::Events& ev);
void initState(EvSt* sts);
std::string dumpState(EvSt* sts, const std::string& device);

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

    cont = gfx->openWindow("Test gui::Gui", 1024, 768);
    if(!cont)
        return 1;

    gui::Theme theme(gfx, "guirc");
    if(!theme.load()) {
        std::cout << "Couldn't load theme." << std::endl;
        return 1;
    }

    /* Loading the events */
    EvSt state[(size_t)Evs::NB];
    initState(state);

    /* The list of events */
    gui::List events(gfx);
    theme.apply(&events);
    events.addItem(0, "Quit");
    events.addItem(0, "Last");
    events.addItem(0, "First");
    events.addItem(0, "Back");
    events.addItem(0, "Select");
    events.addItem(0, "Right");
    events.addItem(0, "Left");
    events.addItem(0, "Down");
    events.addItem(0, "Up");

    /* The list of devices */
    gui::List devices(gfx);
    theme.apply(&devices);
    updateDevices(&devices, ev);

    /* The recap of what is setted */
    gui::Text text(gfx);
    theme.apply(&text);
    text.setText(dumpState(state, devices.selectedText()));

    /* The main layout */
    gui::GridLayout layout(gfx, 3, 1);
    theme.apply(&layout);
    layout.addWidget(&devices, 0, 0); 
    layout.addWidget(&events,  1, 0);
    layout.addWidget(&text,    2, 0);

    /* Setting up the global frame */
    gui::Frame glFr(gfx);
    glFr.set(&layout);
    theme.apply(&glFr);

    /* The gui */
    gui::Gui g(gfx);
    g.main(&glFr, geometry::Point(0.0f,0.0f), 1024.0f, 768.0f);

    geometry::AABB bg(800.0f,600.0f);
    graphics::Color bgc(127,127,255);

    while(cont)
    {
        ev.update();
        ev.clearInput(); /* We don't need full input save, so we clear it */
        if(ev.isKeyPressed(events::KeyMap::Escape) || ev.closed())
            cont = false;
        if(ev.earnedState(events::WindowState::InputFocus))
            g.focus(true);
        else if(ev.lostState(events::WindowState::InputFocus))
            g.focus(false);
        g.update(ev);
        text.setText(dumpState(state, devices.selectedText()));

        gfx->beginDraw();
        gfx->draw(bg, bgc);
        g.draw();
        gfx->endDraw();
        SDL_Delay(1000/60);
    }

    delete gfx;
    core::logger::free();
    SDL_Quit();
    return 0;
}

void updateDevices(gui::List* l, events::Events& ev)
{
    /* TODO clear the list */
    ev.closeJoysticks();
    for(int i = 0; i < ev.numJoysticks(); ++i) {
        events::Joystick* j = ev.openJoystick(i);
        if(j != NULL) {
            l->addItem(0, j->name());
        }
    }
    l->addItem(0, "Keyboard");
}

void initState(EvSt* sts)
{
    for(size_t i = 0; i < (size_t)Evs::NB; ++i) {
        sts[i].dev = EvSt::None;
        sts[i].id  = 0;
    }
    sts[(size_t)Evs::Up].name      = "Up";
    sts[(size_t)Evs::Down].name    = "Down";
    sts[(size_t)Evs::Left].name    = "Left";
    sts[(size_t)Evs::Right].name   = "Right";
    sts[(size_t)Evs::Select].name  = "Select";
    sts[(size_t)Evs::Back].name    = "Back";
    sts[(size_t)Evs::First].name   = "First";
    sts[(size_t)Evs::Last].name    = "Last";
    sts[(size_t)Evs::Quit].name    = "Quit";
}

std::string devStr(EvSt::Dev d)
{
    switch(d) {
        case EvSt::Key:    return "key";
        case EvSt::Axis:   return "axis";
        case EvSt::Button: return "button";
        case EvSt::Hat:    return "hat";
        case EvSt::None:   return "none";
        default:           return "bug";
    }
}

std::string dumpState(EvSt* sts, const std::string& device)
{
    std::ostringstream oss;
    oss << "Device : \"" << device << "\"" << std::endl;
    for(size_t i = 0; i < (size_t)Evs::NB; ++i) {
        oss << sts[i].name << " : (" << devStr(sts[i].dev) << ") " << sts[i].id << std::endl;
    }
    return oss.str();
}



