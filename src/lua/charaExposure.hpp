
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
        /** @brief Expose the character nb to the script. */
        void exposeChara(Script* scr, int nb);

        /** @brief The exposition of the gameplay::Character class to lua. */
        template<int C> class Character
        {
            public:
                Character() : isPrecious(false)
            {}
                Character(lua_State*) : isPrecious(false)
            {}

                /* Lua exposure. */
                static const char* className;
                static const Script::Methods<Character<C>> methods[];
                static const Script::Properties<Character<C>> properties[];
                bool isExisting;
                bool isPrecious;
                static void expose(Script* scr);

                /* Methods to expose. */
                int size(lua_State* st);    /**< @brief Set the character size. */
                int weight(lua_State* st);  /**< @brief Set the character weight. */
                int setMana(lua_State* st); /**< @brief Set the mana max of the character. */
                int onGround(lua_State* st);
                int requireMana(lua_State* st);
                int flip(lua_State* st);
        };

        /* Template implementation. */
        template <int C> const char* Character<C>::className = "Character";
        template <int C> const Script::Methods<Character<C>> Character<C>::methods[] = {
            {"size",        &Character<C>::size},
            {"weight",      &Character<C>::weight},
            {"mana",        &Character<C>::setMana},
            {"requireMana", &Character<C>::requireMana},
            {"onGround",    &Character<C>::onGround},
            {"flip",        &Character<C>::flip},
            {NULL, NULL}
        };
        template <int C> const Script::Properties<Character<C>> Character<C>::properties[] = {
            {NULL, NULL, NULL}
        };

        template <int C> void Character<C>::expose(Script* scr)
        {
            scr->registerClass<Character<C>>();
        }

        template <int C> int Character<C>::size(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            characters[C]->phSize(geometry::AABB(lua_tonumber(st, 1), lua_tonumber(st, 2)));
            return 0;
        }

        template <int C> int Character<C>::weight(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            characters[C]->phWeight(lua_tonumber(st, 1));
            return 0;
        }

        template <int C> int Character<C>::setMana(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            characters[C]->setManaMax(lua_tonumber(st, 1));
            return 0;
        }

        template <int C> int Character<C>::onGround(lua_State* st)
        {
            return helper::returnBoolean(st, characters[C]->onGround());
        }

        template <int C> int Character<C>::requireMana(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            return helper::returnBoolean(st, characters[C]->requireMana(lua_tonumber(st, 1)));
        }

        template <int C> int Character<C>::flip(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::BOOL)
                return 0;
            characters[C]->setFlip(lua_toboolean(st, 1));
            return 0;
        }

    }
}

#endif

