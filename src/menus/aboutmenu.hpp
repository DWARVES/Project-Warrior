
#ifndef DEF_ABOUTMENU
#define DEF_ABOUTMENU

#include "menu.hpp"
#include "gui/text.hpp"
#include "gui/gridlayout.hpp"
#include "gui/button.hpp"
#include "gui/frame.hpp"

/** @brief The about screen. */
class AboutMenu : public Menu
{
    public:
        AboutMenu();
        virtual ~AboutMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gui::Button* m_back;    /**< @brief The button to return to main manu. */
        gui::Text* m_about;     /**< @brief The about text. */
        gui::Frame* m_frame;    /**< @brief The frame around the text widget. */
        gui::GridLayout* m_lay; /**< @brief The layout. */
};

#endif

