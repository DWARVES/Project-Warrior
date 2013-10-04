
#ifndef DEF_EVENTS_KEY
#define DEF_EVENTS_KEY

#include <SDL.h>
#include <string>

namespace events
{
    /* KeyType must be an integer, the user does not need to know it's real size
     * SDL_Keycode is already a typedef used by SDL to Sint32
     */
    typedef SDL_Keycode KeyType;

    class Key
    {
        public:
            Key();
            Key(char c);
            Key(KeyType k);

            /* Returns the corresponding char of the key */
            char getChar() const;
            /* Returns the human readable name of the key */
            std::string name() const;
            /* Returns the symbol of the key */
            KeyType getSym() const;
            /* Returns the symbol of the key if the keyboard was qwerty, it's a layout-independant information */
            KeyType getPos() const;
            /* Returns the human readable name of the key layout-independant */
            std::string posName() const;

            void setChar(char c);
            void setSym(KeyType sym);
            void setPos(KeyType pos);

        private:
            SDL_Keycode m_key;
    };
}

#endif

