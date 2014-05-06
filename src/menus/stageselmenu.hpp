
#ifndef DEF_STAGESELMENU
#define DEF_STAGESELMENU

#include "menu.hpp"
#include "gamemenu.hpp"
#include "gameplay/controler.hpp"
#include "gameplay/stage.hpp"
#include "gui/list.hpp"
#include "gui/button.hpp"
#include "gui/gridlayout.hpp"
#include "gui/widget.hpp"

/** @brief The menu to select the stage to play. */
class StageSelMenu : public Menu
{
    /** @brief A widget to display the preview picture of a stage. */
    class StagePreview : public gui::Widget
    {
        public:
            StagePreview();
            virtual ~StagePreview();
            /** @brief Set the stage to draw. */
            void set(gameplay::Stage* st);
            virtual void draw();

        private:
            gameplay::Stage* m_stage; /**< @brief The stage to display the preview. */
    };

    /** @brief A list indicating when the selection has moved. */
    class List : public gui::List
    {
        public:
            List();
            virtual ~List();
            virtual void select();
            /** @brief Indicates if the selection has changed. */
            bool moved();

        private:
            bool m_moved; /**< @brief Has the selection changed. */
    };

    public:
        StageSelMenu() = delete;
        StageSelMenu(gameplay::Controler* charas[4]);
        virtual ~StageSelMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        gameplay::Controler* m_charas[4]; /**< @brief The list of characters of players. */
        gui::GridLayout* m_layout;        /**< @brief The layout used. */
        List* m_list;                     /**< @brief The list of stages. */
        gui::Button* m_play;              /**< @brief The play button. */
        gui::Button* m_back;              /**< @brief The back button. */
        StagePreview* m_prev;             /**< @brief The preview of the stage. */
        GameMenu* m_menu;                 /**< @brief The game to launch. */

        /** @brief Load the stages and populate the list. */
        void populateList();
        /** @brief Disable all modification of virtual size done by stage. */
        void disableStageSize();
};

#endif

