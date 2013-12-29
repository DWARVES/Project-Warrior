
#ifndef DEF_VIDEOMENU
#define DEF_VIDEOMENU

#include "menu.hpp"
#include <string>

/** @brief A menu printing a video in the whole screen. */
class VideoMenu : public Menu
{
    public:
        VideoMenu() = delete;
        VideoMenu(const VideoMenu&) = delete;
        VideoMenu(const std::string& path, const std::string& music = "");
        virtual ~VideoMenu();

        virtual bool prepare();
        virtual bool update();

    private:
        std::string m_name;  /**< @brief The name of the movie in global::gfx /videomenu namespace. */
        std::string m_audio; /**< @brief The name of the music in global::audio /videomenu namespace. */
};

#endif

