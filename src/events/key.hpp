
#ifndef DEF_EVENTS_KEY
#define DEF_EVENTS_KEY

#include <SDL.h>
#include <string>
#include "keymap.hpp"
#include "keytype.hpp"

namespace events
{
    /** @brief Represent a single key. */
    class Key
    {
        public:
            Key();
            /** @brief Set the key from a character. Only works for ascii caracters. */
            Key(char c);
            /** @brief Set the key from its code in actual layout. */
            Key(KeyType k);
            /** @brief Set the key from its KeyMap in actual layout. */
            Key(KeyMap m);

            /** @brief Returns the corresponding char of the key, only works for ascii characters. */
            char getChar() const;
            /** @brief Returns the human readable name of the key. */
            std::string name() const;
            /** @brief Returns the code of the key in actual layout. */
            KeyType getSym() const;
            /** @brief Returns the code of the key in actual layout. */
            operator KeyType() const;
            /** @brief Returns the symbol of the key if the keyboard was qwerty, it's a layout-independant information. */
            KeyType getPos() const;
            /** @brief Returns the human readable name of the key layout-independant. */
            std::string posName() const;

            /** @brief Set the key from a character. Only works for ascii caracters. */
            void setChar(char c);
            /** @brief Set the key from its code in actual layout. */
            void setSym(KeyType sym);
            /** @brief Set the key from its KeyMap in actual layout. */
            void setSym(KeyMap map);
            /** @brief Set the key from its code in a qwerty layout. */
            void setPos(KeyType pos);

        private:
            /** @brief The code of the key in actual layout. */
            SDL_Keycode m_key;
    };
}

#endif

