
#ifndef DEF_MUSICMENU
#define DEF_MUSICMENU

#include "menu.hpp"
#include "gui/checkbox.hpp"
#include "gui/fillbar.hpp"
#include "gui/button.hpp"
#include "gui/gridlayout.hpp"

/** @brief A menu to configure music and sounds. */
class MusicMenu : public Menu
{
    public:
        MusicMenu();
        virtual ~MusicMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gui::CheckBox* m_sound;    /**< @brief Must the sounds be played. */
        gui::CheckBox* m_music;    /**< @brief Must the music be played. */
        gui::FillBar* m_sndVol;    /**< @brief The volume of the sounds. */
        gui::FillBar* m_musVol;    /**< @brief The volume of the music. */
        gui::Button* m_back;       /**< @brief The button to get back to config menu. */
        gui::GridLayout* m_layout; /**< @brief The layout of the menu. */
};

#endif

