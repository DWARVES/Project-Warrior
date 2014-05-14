
#ifndef DEF_LUA_CHARAEXPOSURE
#define DEF_LUA_CHARAEXPOSURE

#include "lua/script.hpp"
#include "lua/helper.hpp"
#include "gameplay/character.hpp"

namespace lua
{
    namespace exposure
    {
        /** @brief The characters exposed to lua. */
        extern gameplay::Character* characters[4];

        /** @brief The exposition of the gameplay::Character class to lua. */
        class Character
        {
            public:
                Character() : isPrecious(false)
            {}
                Character(lua_State*) : isPrecious(false)
            {}

                /* Lua exposure. */
                static const char* className;
                static const Script::Methods<Character> methods[];
                static const Script::Properties<Character> properties[];
                bool isExisting;
                bool isPrecious;
                static void expose(Script* scr);

                /* Methods to expose. */
                int setUsed(lua_State* st); /**< @brief Set the character to use (c in [0-3]). */
                int size(lua_State* st);    /**< @brief Set the character size. */
                int weight(lua_State* st);  /**< @brief Set the character weight. */
                int setMana(lua_State* st); /**< @brief Set the mana max of the character. */
                int onGround(lua_State* st);
                int requireMana(lua_State* st);
                int flip(lua_State* st);
                int setManaRecov(lua_State* st);

                /* Physic exposure. */
                int applyForce(lua_State* st);
                int impulse(lua_State* st);
                int velocity(lua_State* st);
                int attack(lua_State* st);

                /* CCs */
                int damage(lua_State* st);
                int stun(lua_State* st);
                int impact(lua_State* st);

            private:
                int m_char; /**< @brief The character to use [0-3]. */
        };
    }
}

#endif

