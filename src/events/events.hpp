
#ifndef DEF_EVENTS_EVENTS
#define DEF_EVENTS_EVENTS

#include <vector>
#include <string>
#include <unordered_map>
#include "key.hpp"
#include "button.hpp"
#include "joystick.hpp"
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
            /* Test if the key was pressed in last call to update */
            bool keyJustPressed(Key k) const;
            /* Test if the key was released in last call to update */
            bool keyJustReleased(Key k) const;
            /* Time when this key was last pressed */
            unsigned int lastKeyPress(Key k) const;
            /* Time when this key was last released */
            unsigned int lastKeyRelease(Key k) const;
            /* Returns the time during which the key was pressed, or 0 if it's not pressed */
            unsigned int timeKeyPressed(Key k) const;
            /* Position of the pointer when the key was last pressed */
            geometry::Point posKeyPress(Key k) const;
            /* Position of the pointer when the key was last released */
            geometry::Point posKeyRelease(Key k) const;
            bool isKeyPressed(Key k) const;

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
             *     Text input       *
             ************************/
            /* Returns the input capted in the last call to update */
            std::string lastInput() const;
            /* Returns the input capted since the last call to clearInput */
            std::string fullInput() const;
            void clearInput();
            /* Enable the input */
            void enableInput(bool e);
            bool isInputEnabled() const;

            /************************
             *       Buttons        *
             ************************/
            /* Buttons pressed in last update call */
            std::vector<Button> lastButtonsPressed() const;
            /* Buttons released in last update call */
            std::vector<Button> lastButtonsReleased() const;
            /* Test if the button was pressed in last call to update */
            bool keyJustPressed(Button k) const;
            /* Test if the button was released in last call to update */
            bool keyJustReleased(Button k) const;
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
            int numJoysticks() const;
            /* Direct access to state can be done through the Joystick structure */
            /* Returns NULL if couldn't load the joystick
             * The pointer returned will be used to access states an functions
             * It mustn't be free'd by the user
             */
            Joystick* openJoystick(JoystickID id);
            /* The pointer won't be valid anymore 
             * The joysticks which weren't closed by the user are closed when the Events class is destroyed
             */
            void closeJoystick(Joystick* j);
            bool isJoystickLoaded(Joystick* j) const;
            /* The last pressed and released buttons for a joystick */
            std::vector<int> lastJoyButtonsPressed(Joystick* j) const;
            std::vector<int> lastJoyButtonsReleased(Joystick* j) const;
            /* Test if the joystick button was pressed in last call to update */
            bool joyButtonJustPressed(Joystick* j, int b) const;
            /* Test if the joystick button was released in last call to update */
            bool joyButtonJustReleased(Joystick* j, int b) const;
            /* Time of last press and release of a button */
            unsigned int lastJoyButtonPress(Joystick* j, int b) const;
            unsigned int lastJoyButtonRelease(Joystick* j, int b) const;
            unsigned int timeJoyButtonPressed(Joystick* j, int b) const;
            /* Position of the pointer when the buttons where released or pressed */
            geometry::Point posJoyButtonPress(Joystick* j, int b) const;
            geometry::Point posJoyButtonRelease(Joystick* j, int b) const;
            /* What changed, return indexes */
            std::vector<int> lastAxesMoved(Joystick* j) const;
            std::vector<int> lastHatsMoved(Joystick* j) const;
            /* Indicate the joysticks added (their id) during last call to update */
            /* Indicate if the number of joysticks changed
             * Use lastJoystick{Added,Removed} for more prrecise info
             */
            bool joysticksChanged() const;
            std::vector<JoystickID> lastJoysticksAdded() const;
            /* Indicate the loaded joysticks which were removed in last call to update
             * The pointers returned are NOT valid
             * Their corresponding Joystick* structure are free'd when the joystick is unpluged,
             *   so you mustn't close them nor use them anymore
             */
            std::vector<Joystick*> lastJoysticksRemoved() const;

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

            /* Input events */
            std::string m_lastInput;
            std::string m_fullInput;
            bool m_input;

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

            /* Joystick */
            std::vector<JoystickID> m_lastJoyAdded;
            std::vector<Joystick*> m_lastJoyRemoved;
            std::vector<JoystickID> m_joyLoaded;
            struct JoystickButtonEvent {
                unsigned int pressT;
                geometry::Point pressP;
                unsigned int releaseT;
                geometry::Point releaseP;
            };

            struct JoystickEvent {
                std::vector<JoystickButtonEvent> buttons;
                std::vector<int> lpressed;
                std::vector<int> lreleased;
                std::vector<int> laxis;
                std::vector<int> lhats;
            };
            std::unordered_map<Joystick*,JoystickEvent> m_joys;

            /* Miscellaneous */
            geometry::Point m_wheel;
            std::vector<std::string> m_dropped;
            bool m_quit;

            /* Internal functions */
            void initKeys();
            void initButtons();
            void initStates();
            JoystickEvent initEvent(Joystick* j);
            Joystick* getJoyFromID(JoystickID id);
            void clearJoysticks();
            void process(SDL_KeyboardEvent*    ev);
            void process(SDL_MouseMotionEvent* ev);
            void process(SDL_MouseWheelEvent*  ev);
            void process(SDL_MouseButtonEvent* ev);
            void process(SDL_DropEvent*        ev);
            void process(SDL_WindowEvent*      ev);
            void process(SDL_TextInputEvent*   ev);
            void process(SDL_JoyAxisEvent*     ev);
            void process(SDL_JoyHatEvent*      ev);
            void process(SDL_JoyButtonEvent*   ev);
            void process(SDL_JoyDeviceEvent*   ev);
    };
}

#endif

