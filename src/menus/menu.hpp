
#ifndef DEF_MENU
#define DEF_MENU

/** @brief An abstract class representing a menu. */
class Menu
{
    public:
        Menu();
        virtual ~Menu();

        /** @brief Loads all the gui elements, and send them to the gui. */
        virtual bool prepare() = 0;
        /** @brief Must be called at every frame, update the menu. */
        virtual void update() = 0;
};

#endif

