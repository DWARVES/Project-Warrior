
#ifndef DEF_GUI_RADIO
#define DEF_GUI_RADIO

#include "gui/widget.hpp"
#include "gui/checkbox.hpp"
#include "geometry/aabb.hpp"
#include <string>
#include <map>

namespace gui
{
    class Radio : public Widget
    {
        public:
            Radio() = delete;
            Radio(const Radio&) = delete;
            Radio(graphics::Graphics* gfx, const std::string& gr);
            virtual ~Radio();

            /* Size */
            virtual float width(float w);
            virtual float height(float h);
            virtual float width() const;
            virtual float height() const;

            /* Groups */
            static bool existsGroup(const std::string& name);
            static std::vector<Radio*> groupContent(const std::string& name);
            static Radio* getSelected(const std::string& name);
            /* To remove a group, remove all the radio in it */
            void setGroup(const std::string& name);
            std::string group() const;
            
            /* State : you can't deselect only by selecting another one in the same group */
            bool select();
            bool get() const;

            /* Textures */
            void setTexture(bool f, const std::string& empty, const std::string& selected);

            /* Drawing */
            bool square(bool s);
            bool square() const;
            void maxSize(geometry::AABB m);
            void disableMaxSize();
            virtual void draw();

            /* Actions */
            virtual void focus(bool f);
            virtual bool action(Widget::Action a);

        private:
            CheckBox m_box;
            std::string m_group;
            static std::map<std::string, std::vector<Radio*>> m_groups;

            /* Internal functions */
            void deselectGroup();
            /* Return true if the group has been created, false if it already existed */
            bool createGroupIfNotExistant();
            void removeFromGroup();
    };
}

#endif

