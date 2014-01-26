
#ifndef DEF_EVENTS_EVENTS
#define DEF_EVENTS_EVENTS

#include <vector>
#include <string>
#include <unordered_map>
#include "key.hpp"
#include "button.hpp"
#include "joystick.hpp"
#include "windowstate.hpp"
#include "evsave.hpp"
#include "geometry/point.hpp"
#include "geometry/aabb.hpp"

/** @brief Contains all classes and enums about events handling and control devices support. */
namespace events
{
    /** @brief Main class, get all the events and report them. */
    class Events
    {
        public:
            Events();
            Events(const Events&) = delete;
            ~Events();

            /************************
             *         Main         *
             ************************/
            /** @brief Must be called at each loop. It will get the new events and store them. */
            void update();

            /************************
             *       Keyboard       *
             ************************/
            /** @name Keyboard-related events access.
             * @{
             */
            /** @brief Returns the keys pressed during the last update call. */
            std::vector<Key> lastKeysPressed() const;
            /** @brief Returns the keys released during last update call. */
            std::vector<Key> lastKeysReleased() const;
            /** @brief Indicates if the key was pressed in last call to update. */
            bool keyJustPressed(Key k) const;
            /** @brief Indicates if the key was released in last call to update. */
            bool keyJustReleased(Key k) const;
            /** @brief Time in milliseconds when this key was last pressed. */
            unsigned int lastKeyPress(Key k) const;
            /** @brief Time in milliseconds when this key was last released. */
            unsigned int lastKeyRelease(Key k) const;
            /** @brief Returns the time in milliseconds during which the key was pressed, or 0 if it's not pressed. */
            unsigned int timeKeyPressed(Key k) const;
            /** @brief Returns the position of the pointer when the key was last pressed. */
            geometry::Point posKeyPress(Key k) const;
            /** @brief Returns the position of the pointer when the key was last released. */
            geometry::Point posKeyRelease(Key k) const;
            /** @brief Indicates if the key is pressed. */
            bool isKeyPressed(Key k) const;
            /** @} */

            /************************
             *      Modifiers       *
             ************************/
            /** @name Modifiers state access.
             * @brief These methods check if a modifier is pressed.
             * @{
             */
            bool alt() const;
            bool shift() const;
            bool ctrl() const;
            bool super() const;
            bool num() const;
            bool caps() const;
            /** @} */

            /************************
             *     Text input       *
             ************************/
            /** @name Text input handling.
             * @{
             */
            /** @brief Returns the input capted in the last call to update. */
            std::string lastInput() const;
            /** @brief Returns the input capted since the last call to clearInput. */
            std::string fullInput() const;
            /** @brief Clears all the stored input. */
            void clearInput();
            /** @brief Enable or disable the input. */
            void enableInput(bool e);
            /** @brief Indicates if the input is enabled. */
            bool isInputEnabled() const;
            /** @} */

            /************************
             *       Buttons        *
             ************************/
            /** @name Mouse buttons related events access.
             * @{
             */
            /** @brief Returns the buttons pressed during the last update call. */
            std::vector<Button> lastButtonsPressed() const;
            /** @brief Returns the buttons released in last update call. */
            std::vector<Button> lastButtonsReleased() const;
            /** @brief Indicates if the button was pressed in last call to update. */
            bool buttonJustPressed(Button k) const;
            /** @brief Indicates if the button was released in last call to update. */
            bool buttonJustReleased(Button k) const;
            /** @brief Returns the time when this button was last pressed. */
            unsigned int lastButtonPress(Button k) const;
            /** @brief Returns the time when this button was last released. */
            unsigned int lastButtonRelease(Button k) const;
            /** @brief Returns the time during which the button was pressed, or 0 if it's not pressed. */
            unsigned int timeButtonPressed(Button k) const;
            /** @brief Returns the position of the pointer when the button was last pressed. */
            geometry::Point posButtonPress(Button k) const;
            /** @brief Returns the position of the pointer when the button was last released. */
            geometry::Point posButtonRelease(Button k) const;
            /** @brief Indicates if the button is pressed. */
            bool isButtonPressed(Button k) const;
            /** @} */

            /************************
             *       Pointer        *
             ************************/
            /** @name Pointer position and movement access.
             * @{
             */
            /** @brief Returns the position of the mouse in pixels. */
            geometry::Point mousePos() const;
            /** @brief Returns the move of the mouse between the two previous calls to update. */
            geometry::Point mouseRel() const;
            /** @brief Enable/disable mouse-rel mode.
             *
             * The mouse will disappear and stay at the same place, so only relative mouse movement will be reported if enabled.
             * Returns false if the sdl driver doesn't support it.
             */
            bool mouseRelMode(bool enable);
            /** @brief Indicates if mouse-rel mode is enabled. */
            bool mouseRelMode() const;
            /** @} */

            /************************
             *       Window         *
             ************************/
            /** @name Window states events.
             * @{
             */
            /** @brief Indicates if the window has a certain state. */
            bool hasState(WindowState st) const;
            /** @brief Indicates if the window has earned the state in the last call to update/ */
            bool earnedState(WindowState st) const;
            /** @brief Indicates if the window has lost the state in the last call to update. */
            bool lostState(WindowState st) const;
            /** @brief Indicates if the window was requested to be closed.
             * Once it returned true, it will do so until closeAbort is called.
             */
            bool closed() const;
            /** @brief Stops closed from returning always true. */
            void closeAbort();
            /** @} */

            /************************
             *      Joystick        *
             ************************/
            /** @name Joystick-related events.
             * @{
             */
            /** @brief Returns the number of joysticks actually plugged in the system. */
            int numJoysticks() const;
            /** @brief Open a joystick from its ID.
             * @param id The ID, or index (from 0 to numJoysticks) of the joystick.
             * @return A pointer to the opened joystick, NULL if it couldn't load the joystick.
             * The pointer returned can be used to access states an functions.
             * It mustn't be free'd directly by the user.
             */
            Joystick* openJoystick(JoystickID id);
            /** @brief Opens all the joysticks available. */
            bool openJoysticks();
            /** @brief Closes all the opened joysticks. */
            void closeJoysticks();
            /** @brief Close a joystick.
             *
             * The pointer won't be valid anymore 
             * The joysticks which weren't closed by the user are closed when the Events class is destroyed
             */
            void closeJoystick(Joystick* j);
            /** @brief Indicates if the joystick structure is a valid and loaded one. */
            bool isJoystickLoaded(Joystick* j) const;
            /** @brief Get the names of all joystick loaded. */
            std::vector<std::string> joyNames() const;
            /** @brief Get a previously loaded joystick from its name. Return NULL if the joystick wasn't found. */
            Joystick* joyFromName(const std::string& name) const;
            /** @brief Returns the buttons of joystick j pressed during the last call to update. */
            std::vector<int> lastJoyButtonsPressed(Joystick* j) const;
            /** @brief Returns the buttons of joystick j released during the last call to update. */
            std::vector<int> lastJoyButtonsReleased(Joystick* j) const;
            /** @brief Indicates if a joystick button was pressed in last call to update. */
            bool joyButtonJustPressed(Joystick* j, int b) const;
            /** @brief Indicates if a joystick button was released in last call to update. */
            bool joyButtonJustReleased(Joystick* j, int b) const;
            /** @brief Returns the time of the last press of a joystick button. */
            unsigned int lastJoyButtonPress(Joystick* j, int b) const;
            /** @brief Returns the time of the last release of a joystick button. */
            unsigned int lastJoyButtonRelease(Joystick* j, int b) const;
            /** @brief Returns the time for which the button has been pressed, or 0 if the button isn't pressed. */
            unsigned int timeJoyButtonPressed(Joystick* j, int b) const;
            /** @brief Returns the position of the pointer when the button was pressed. */
            geometry::Point posJoyButtonPress(Joystick* j, int b) const;
            /** @brief Returns the position of the pointer when the button was released. */
            geometry::Point posJoyButtonRelease(Joystick* j, int b) const;
            /** @brief Returns the axes of a joystick that moved during the last call to update. */
            std::vector<int> lastAxesMoved(Joystick* j) const;
            /** @brief Returns the hats of a joystick that moved during the last call to update. */
            std::vector<int> lastHatsMoved(Joystick* j) const;
            /** @brief Indicates if the number of joysticks changed during last call to update.
             * Use lastJoysticksAdded and last JoysticksRemoved for more precise data.
             */
            bool joysticksChanged() const;
            /** @brief Returns the id of the joysticks which were plugged during last call to update. */
            std::vector<JoystickID> lastJoysticksAdded() const;
            /** @brief Returns the loaded joysticks which were removed in last call to update.
             *
             * The pointers returned are NOT valid
             * Their corresponding Joystick* structure are free'd when the joystick is unpluged,
             * so you mustn't close them nor use them anymore.
             */
            std::vector<Joystick*> lastJoysticksRemoved() const;
            /** @brief Returns the joystick buttons and the joystick to which they belong that were pressed during last call to update. */
            std::vector<std::pair<int,Joystick*>> lastJoyButtonsPressed() const;
            /** @brief Returns the joystick buttons and the joystick to which they belong that were released during last call to update. */
            std::vector<std::pair<int,Joystick*>> lastJoyButtonsReleased() const;
            /** @brief Returns the joystick axes and the joystick to which they belong that moved during last call to update. */
            std::vector<std::pair<int,Joystick*>> lastAxesMoved() const;
            /** @brief Returns the joystick hats and the joystick to which they belong that moved during last call to update. */
            std::vector<std::pair<int,Joystick*>> lastHatsMoved() const;
            /** @} */

            /************************
             *     Saved events     *
             ************************/
            /** @name Personnalized events handling.
             * @{
             */
            /** @brief Add a new personnalized event to handle.
             * @param sv A pointer to the event.
             * @param tosave Indicates if the pointer must be free'd by the Events structure.
             * @return The id of the added event.
             */
            size_t addSaved(EvSave* sv, bool tosave = false);
            /** @brief Returns the event corresponding to the id or NULL. */
            EvSave* getSaved(size_t id) const;
            /** @brief Stop handling a personnalized event. May delete th EvSave. */
            void removeSaved(size_t id);
            /** @brief Stop handling all the setted personnalized events. */
            void removeSaveds();
            /** @brief Indicates if the personnalized event is holded. */
            bool isSavedValid(size_t id) const;
            /** @brief Indicates if the personnalized events has been hold in last call to update. */
            bool isSavedJustValid(size_t id) const;
            /** @brief Returns the personnalized events that were holded during last call to update. */
            std::vector<size_t> lastSavedValidated() const;
            /** @brief Returns the personnalized events that were released during last call to update. */
            std::vector<size_t> lastSavedReleased() const;
            /** @brief Returns the time when the event was last holded. */
            unsigned int lastSavedValid(size_t id) const;
            /** @brief Returns the time when the event was last released. */
            unsigned int lastSavedRelease(size_t id) const;
            /** @brief Returns the time during which the event has been holded, or 0 if it isn't being holded. */
            unsigned int timeSavedValidated(size_t id) const;
            /** @} */

            /************************
             *    Miscellaneous     *
             ************************/
            /** @name Miscellaneous.
             * @{
             */
            /** @brief Returns the amount of wheel action in x and y axis in last update call.
             *
             * Left on x axis give negatives values and down on y give negatives too.
             */
            geometry::Point wheel() const;
            /** @brief Returns the path to files which were dropped onto the window during last call to update.
             * @todo Doesn't work.
             */
            std::vector<std::string> dropped() const;
            /** @brief Indicates if the program was requested to be stop.
             * Once it returned true, it will do so until quitAbort is called.
             */
            bool quit() const;
            /** @brief Stops quit from returning always true. */
            void quitAbort();
            /** @} */

        private:
            /** @brief Store a keyboard event. */
            struct KeyEvent {
                bool state;               /**< @brief Pressed or released. */
                unsigned int pressT;      /**< @brief Time when it was last pressed. */
                geometry::Point pressP;   /**< @brief Position of the pointer when it was last pressed. */
                unsigned int releaseT;    /**< @brief Time when it was last released. */
                geometry::Point releaseP; /**< @brief Position of the pointer when it was last released. */
            };
            KeyEvent m_keys[SDL_NUM_SCANCODES]; /**< @brief The array of keyboard events, indexed by scancode. */
            std::vector<Key> m_lastPressedK;    /**< @brief The key pressed during last call to update. */
            std::vector<Key> m_lastReleasedK;   /**< @brief The key released during last call to update. */


            std::string m_lastInput; /**< @brief The text entered during last call to update. */
            std::string m_fullInput; /**< @brief The complete text entered. */
            bool m_input;            /**< @brief Indicates if the input is enabled. */


            /** @brief Store a mouse button event. */
            struct ButtonEvent {
                bool state;               /**< @brief Pressed or released. */
                unsigned int pressT;      /**< @brief Time when it was last pressed. */
                geometry::Point pressP;   /**< @brief Position of the pointer when it was last pressed. */
                unsigned int releaseT;    /**< @brief Time when it was last released. */
                geometry::Point releaseP; /**< @brief Position of the pointer when it was last released. */
            };
            ButtonEvent m_buttons[Uint8(Button::Last)]; /**< @brief The array of button events, indexed by button id. */
            std::vector<Button> m_lastPressedB;         /**< @brief The buttons pressed during last call to update. */
            std::vector<Button> m_lastReleasedB;        /**< @brief The buttons released during last call to update. */


            geometry::Point m_mrel; /**< @brief The movement of the mouse in last call to update. */


            /** @brief Store a window state. */
            struct WindowEvent {
                bool state;  /**< @brief Is the state active. */
                bool earned; /**< @brief Has the state been earned in last call to update. */
                bool lost;   /**< @brief Has the state been lost in last call to update. */
            };
            WindowEvent m_wins[Uint8(WindowState::Last)]; /**< @brief The array of window states. */
            bool m_closed;                                /**< @brief Has thhe window been requested to close. */


            std::vector<JoystickID> m_lastJoyAdded;  /**< @brief Array of joysticks added during last call to update. */
            std::vector<Joystick*> m_lastJoyRemoved; /**< @brief Array of joysticks released during last call to update. */
            std::vector<JoystickID> m_joyLoaded;     /**< @brief Array of indexes of joysticks which have already been loaded. */

            /** @brief Store a joystick button event. */
            struct JoystickButtonEvent {
                unsigned int pressT;      /**< @brief The time when it was last pressed. */
                geometry::Point pressP;   /**< @brief The position of the pointer when it was last pressed. */
                unsigned int releaseT;    /**< @brief The time when it was last released. */
                geometry::Point releaseP; /**< @brief The position of the pointer when it was last released. */
            };
            /** @brief Store the events related to a joystick. */
            struct JoystickEvent {
                std::vector<JoystickButtonEvent> buttons; /**< @brief Array of the buttons of the joystick. */
                std::vector<int> lpressed;                /**< @brief Array of buttons pressed during last call to update. */
                std::vector<int> lreleased;               /**< @brief Array of buttons released during last call to update. */
                std::vector<int> laxis;                   /**< @brief Array of axes moved during last call to update. */
                std::vector<int> lhats;                   /**< @brief Array of hats moved during last call to update. */
            };
            std::unordered_map<Joystick*,JoystickEvent> m_joys; /**< @brief Array of joysticks events. */


            /** @brief Store a personnalized event. */
            struct SavedEvent {
                EvSave* ev;            /**< @brief The personnalized event. */
                unsigned int lvalid;   /**< @brief The time when it was last holded. */
                unsigned int lrelease; /**< @brief The time when it was last released. */
                bool tosave;           /**< @brief Must ev be delete'd. */
            };
            std::map<size_t,SavedEvent> m_saved;      /**< @brief The array of the handled personnalized events. */
            size_t m_maxSaved;                        /**< @brief The max id of a personnalized event + 1. */
            std::vector<size_t> m_lastSavedValidated; /**< @brief The array of personnalized events holded during last call to update. */
            std::vector<size_t> m_lastSavedReleased;  /**< @brief The array of personnalized events released during last call to update. */

            /* Miscellaneous */
            geometry::Point m_wheel;            /**< @brief Move of the wheel during last call to update. */
            std::vector<std::string> m_dropped; /**< @brief The files dropped on the window in last call to update. */
            bool m_quit;                        /**< @brief Has the program been requested to stop. */

            /* Internal functions */
            void initKeys();                        /**< @brief Initialises the members related to keyboard events. */
            void initButtons();                     /**< @brief Initialises the members related to mouse buttons events. */
            void initStates();                      /**< @brief Initialises the members related to window states events. */
            void initEvSaves();                     /**< @brief Initialises the members related to personnalized events. */
            JoystickEvent initEvent(Joystick* j);   /**< @brief Create the joystick event structure corresponding to a joystick. */
            Joystick* getJoyFromID(JoystickID id);  /**< @brief Returns the joystick corresponding to an id. */
            void clearJoysticks();                  /**< @brief Clears the structure related to joysticks events in a update call. */
            void processSavedEvents();              /**< @brief Update personnalized events according to events capted. */
            /** @name SDL events processing.
             * @brief Analyse an sdl event and store it in the members.
             * @{
             */
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
            /** @} */
    };
}

#endif

