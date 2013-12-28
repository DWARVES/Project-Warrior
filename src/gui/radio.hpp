
#ifndef DEF_GUI_RADIO
#define DEF_GUI_RADIO

#include "gui/widget.hpp"
#include "gui/checkbox.hpp"
#include "geometry/aabb.hpp"
#include <string>
#include <map>

namespace gui
{
    /** @brief Represent a radio button, can be grouped. */
    class Radio : public Widget
    {
        public:
            Radio() = delete;
            Radio(const Radio&) = delete;
            /** @brief gr is the group of the radio button. */
            Radio(graphics::Graphics* gfx, const std::string& gr);
            virtual ~Radio();

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /* Groups */
            /** @brief Checks the existence of a group. */
            static bool existsGroup(const std::string& name);
            /** @brief Returns the radio buttons in a group. */
            static std::vector<Radio*> groupContent(const std::string& name);
            /** @brief Get the radio button selected in a group. May return NULL. */
            static Radio* getSelected(const std::string& name);
            /** @brief Change the group of a radio button. An empty group will automaticly be removed. */
            void setGroup(const std::string& name);
            /** @brief Returns the group of the radio button. */
            std::string group() const;
            
            /** @brief Select the radio button : will deselect all the others radio in the same group. */
            bool select();
            /** @brief Is the radio button selected. */
            bool get() const;
            /** @brief Set the label of the radiobutton. */
            void label(const std::string& lab);
            /** @brief Get the label of the radiobutton. */
            std::string label() const;

            /* Textures */
            /** @brief Set the textures used.
             * @param f Textures for focus or not.
             * @param empty The texture used when not selected.
             * @param selected The texture used when selected.
             */
            void setTexture(bool f, const std::string& empty, const std::string& selected);
            /** @brief Set the font to be used.
             * @param f Font for focus or not.
             * @param font The font to use.
             */
            void setFont(bool f, const std::string& font);

            /* Drawing */
            /** @brief Set a maximum size for the checkbox. */
            void maxSize(float m);
            /** @brief Disable the use of a max size. */
            void disableMaxSize();
            virtual void draw();

            /* Actions */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            /** @brief A checkbox used for drawing. */
            CheckBox m_box;
            /** @brief The group of the radio button. */
            std::string m_group;
            /** @brief The groups and there content. */
            static std::map<std::string, std::vector<Radio*>> m_groups;

            /* Internal functions */
            /** @brief Deselect all radio buttons in the group of this. */
            void deselectGroup();
            /** @brief If the current group doesn't exists, create it and return true. Return false elsewhere. */
            bool createGroupIfNotExistant();
            /** @brief Remove this from its actual group and delete the group if empty. */
            void removeFromGroup();
    };
}

#endif

