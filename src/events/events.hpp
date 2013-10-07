
#ifndef DEF_EVENTS_EVENTS
#define DEF_EVENTS_EVENTS

#include <vector>
#include <string>
#include "key.hpp"
#include "button.hpp"
#include "windowstate.hpp"
#include "geometry/point.hpp"
#include "geometry/aabb.hpp"

namespace events
{
    class Events
    {
        public:
            Events();
            Events(const Events&) = delete;
            ~Events();

            /************************
             *         Main         *
             ************************/
            void update();

            /************************
             *       Keyboard       *
             ************************/
            /* Keys pressed in last update call */
            std::vector<Key> lastKeysPressed() const;
            /* Keys released in last update call */
            std::vector<Key> lastKeysReleased() const;
            /* Time when this key was last pressed */
            unsigned int lastKeyPress(KeyType k) const;
            /* Time when this key was last released */
            unsigned int lastKeyRelease(KeyType k) const;
            /* Returns the time during which the key was pressed, or 0 if it's not pressed */
            unsigned int timeKeyPressed(KeyType k) const;
            /* Position of the pointer when the key was last pressed */
            geometry::Point posKeyPress(KeyType k) const;
            /* Position of the pointer when the key was last released */
            geometry::Point posKeyRelease(KeyType k) const;
            bool isKeyPressed(KeyType k) const;

            /************************
             *      Modifiers       *
             ************************/
            bool alt() const;
            bool shift() const;
            bool ctrl() const;
            bool super() const;
            bool num() const;
            bool caps() const;

            /************************
             *       Buttons        *
             ************************/
            /* Buttons pressed in last update call */
            std::vector<Button> lastButtonsPressed() const;
            /* Buttons released in last update call */
            std::vector<Button> lastButtonsReleased() const;
            /* Time when this button was last pressed */
            unsigned int lastButtonPress(Button k) const;
            /* Time when this button was last released */
            unsigned int lastButtonRelease(Button k) const;
            /* Returns the time during which the button was pressed, or 0 if it's not pressed */
            unsigned int timeButtonPressed(Button k) const;
            /* Position of the pointer when the button was last pressed */
            geometry::Point posButtonPress(Button k) const;
            /* Position of the pointer when the button was last released */
            geometry::Point posButtonRelease(Button k) const;
            bool isButtonPressed(Button k) const;

            /************************
             *       Pointer        *
             ************************/
            geometry::Point mousePos() const;
            /* Returns the move of the mouse between the two previous calls to update */
            geometry::Point mouseRel() const;
            /* The mouse will disappear and stay at the same place, so only relative mouse movement will be reported if enabled
             * Returns false if the sdl driver doesn't support it
             */
            bool mouseRelMode(bool enable);
            /* Check if mouseRelMode is enabled */
            bool mouseRelMode() const;

            /************************
             *       Window         *
             ************************/
            bool hasState(WindowState st) const;
            /* Indicates if the state has been earned in the last call to update */
            bool earnedState(WindowState st) const;
            /* Indicates if the state has been lost in the last call to update */
            bool lostState(WindowState st) const;
            /* The window was requested to be closed
             * Once it returned true, it will do so until closeAbort is called
             */
            bool closed() const;
            /* Stop closed returning always true */
            void closeAbort();

            /************************
             *      Joystick        *
             ************************/
            /* TODO */

            /************************
             *    Miscellaneous     *
             ************************/
            /* Returns the amount of wheel action in x and y axis in last update call
             * left on x axis give negatives values and down on y give negatives too
             */
            geometry::Point wheel() const;
            /* If some files were dropped in the window, their paths are returned
             * Put back to zero in each call to update
             * FIXME doesn't seem to be working
             */
            std::vector<std::string> dropped() const;
            /* The program was requested to be end
             * Once it returned true, it will do so until quitAbort is called
             */
            bool quit() const;
            void quitAbort();

        private:
            /* Keybord events */
            struct KeyEvent {
                bool state;               /* Pressed or released */
                unsigned int pressT;      /* Timestamp of when it was last pressed */
                geometry::Point pressP;   /* Position of the pointer when it was last pressed */
                unsigned int releaseT;   /* Timestamp of when it was last released */
                geometry::Point releaseP; /* Position of the pointer when it was last released */
            };
            KeyEvent m_keys[SDL_NUM_SCANCODES]; /* Array of events, indexed by scancodes */
            std::vector<Key> m_lastPressedK;
            std::vector<Key> m_lastReleasedK;

            /* Button events */
            struct ButtonEvent {
                bool state;               /* Pressed or released */
                unsigned int pressT;      /* Timestamp of when it was last pressed */
                geometry::Point pressP;   /* Position of the pointer when it was last pressed */
                unsigned int releaseT;   /* Timestamp of when it was last released */
                geometry::Point releaseP; /* Position of the pointer when it was last released */
            };
            ButtonEvent m_buttons[Uint8(Button::Last)]; /* Array of button */
            std::vector<Button> m_lastPressedB;
            std::vector<Button> m_lastReleasedB;

            /* Pointer */
            geometry::Point m_mrel;

            /* Windows */
            struct WindowEvent {
                bool state;
                bool earned;
                bool lost;
            };
            WindowEvent m_wins[Uint8(WindowState::Last)];
            bool m_closed;

            /* Miscellaneous */
            geometry::Point m_wheel;
            std::vector<std::string> m_dropped;
            bool m_quit;

            /* Internal functions */
            void initKeys();
            void initButtons();
            void initStates();
            void process(SDL_KeyboardEvent* ev);
            void process(SDL_MouseMotionEvent* ev);
            void process(SDL_MouseWheelEvent* ev);
            void process(SDL_MouseButtonEvent* ev);
            void process(SDL_DropEvent* ev);
            void process(SDL_WindowEvent* ev);
    };
}

#endif

