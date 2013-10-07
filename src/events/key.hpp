
#ifndef DEF_EVENTS_KEY
#define DEF_EVENTS_KEY

#include <SDL.h>
#include <string>
#include "keymap.hpp"
#include "keytype.hpp"

namespace events
{
    class Key
    {
        public:
            Key();
            Key(char c);
            Key(KeyType k);
            Key(KeyMap m);

            /* Returns the corresponding char of the key */
            char getChar() const;
            /* Returns the human readable name of the key */
            std::string name() const;
            /* Returns the symbol of the key */
            KeyType getSym() const;
            operator KeyType() const;
            /* Returns the symbol of the key if the keyboard was qwerty, it's a layout-independant information */
            KeyType getPos() const;
            /* Returns the human readable name of the key layout-independant */
            std::string posName() const;

            void setChar(char c);
            void setSym(KeyType sym);
            void setSym(KeyMap map);
            void setPos(KeyType pos);

        private:
            SDL_Keycode m_key;
    };
}

#endif

