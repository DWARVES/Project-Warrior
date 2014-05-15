
#ifndef DEF_GAMEPLAY_CONTROLER
#define DEF_GAMEPLAY_CONTROLER

#include "core/fakefs.hpp"
#include "events/evsave.hpp"
#include "gameplay/character.hpp"
#include <string>
#include <vector>

namespace gameplay
{
    /** @brief Store and load a controler for a character. */
    class Controler
    {
        public:
            /** @brief Open the controler for the setted id.
             * It will fail if the id is unexistant or if it has already been opened.
             */
            Controler(const std::string& id);
            Controler(const Controler&) = delete;
            ~Controler();
            /** @brief Check if the controler could be opened. */
            bool isOpen() const;

            /** @brief Attach a character to control. */
            void attach(Character* ch);
            /** @brief Get the attached character. */
            Character* attached() const;
            /** @brief Process the events and send the corresponding actions to the character. */
            void update();

            /** @brief Load the controlers file. */
            static bool load(const std::string& path);
            /** @brief Free the controlers loaded and save the changes.
             * @return False if couldn't save.
             */
            static bool close();
            /** @brief Get a list of all the controlers available. */
            static std::vector<std::string> getIDs();
            /** @brief Get a list of all the controlers with setup, even if they are not plugged. */
            static std::vector<std::string> listAll();
            /** @brief Add a new controler (default settings will be applyed). Return false if already existant. */
            static bool create(const std::string& name);

            /** @brief The differents controls. */
            enum Controls : unsigned int {
                Left,     /**< @brief The left button (for walking and directional actions). */
                Right,    /**< @brief The right button (for walking and directional actions). */
                Up,       /**< @brief The up button (for walking and directional actions). */
                Down,     /**< @brief The down button (for walking and directional actions). */
                RunLeft,  /**< @brief The button for running to the left. */
                RunRight, /**< @brief The button for running to the right. */
                Attack,   /**< @brief The button for a basic attack. */
                Special,  /**< @brief The button for a special attack (also called spell). */
                Dodge,    /**< @brief The button to dodge. */
                Catch,    /**< @brief The button to catch. */
                Jump,     /**< @brief The button to jump. */
                Last      /**< @brief For internal use, the number of controls. */
            };
            /** @brief Change a control, will only work if loaded. nev mustn't be free'd by the user. */
            void set(Controls ctrl, events::EvSave* nev);
            /** @brief Get the saved string of an event. */
            std::string get(Controls ctrl) const;
            /** @brief Get the name of a control. */
            static std::string controlName(Controls ctrl);

        private:
            /** @brief Store all the configurated controlers. */
            static core::FakeFS<std::string> m_evs;
            /** @brief Store the path of the loaded file. */
            static std::string m_path;

            bool m_loaded;                               /**< @brief Is the controler loaded. */
            std::string m_namespace;                     /**< @brief The namespace of this controler. */
            Character* m_ch;                             /**< @brief The character controled. */
            events::EvSave* m_ctrls[(unsigned int)Last]; /**< @brief The loaded controls. */
            Controls m_actual;                           /**< @brief The actually selected control. */
            Character::Direction m_prevdir;              /**< @brief The direction of the previous move. */
    };
}

#endif

