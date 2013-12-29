
#ifndef DEF_VIDEOMENU
#define DEF_VIDEOMENU

#include "menu.hpp"
#include <string>

/** @brief A menu printing a video in the whole screen. */
class VideoMenu : public Menu
{
    public:
        VideoMenu(const std::string& path);
        virtual ~VideoMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        std::string m_name; /**< @brief The name of the movie in global::gfx /videomenu namespace. */
};

#endif

