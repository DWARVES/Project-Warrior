
#include "charaExposure.hpp"

namespace lua
{
    namespace exposure
    {
        gameplay::Character* characters[4];

        const char* Character::className = "Character";
        const Script::Methods<Character> Character::methods[] = {
            {"current",     &Character::setUsed},
            {"size",        &Character::size},
            {"weight",      &Character::weight},
            {"mana",        &Character::setMana},
            {"requireMana", &Character::requireMana},
            {"onGround",    &Character::onGround},
            {"flip",        &Character::flip},
            {"force",       &Character::applyForce},
            {"velocity",    &Character::velocity},
            {"impulse",     &Character::impulse},
            {"attack",      &Character::attack},
            {"damage",      &Character::damage},
            {"stun",        &Character::stun},
            {"impact",      &Character::impact},
            {NULL, NULL}
        };
        const Script::Properties<Character> Character::properties[] = {
            {NULL, NULL, NULL}
        };

        void Character::expose(Script* scr)
        {
            scr->registerClass<Character>();
        }

        int Character::size(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            characters[m_char]->phSize(geometry::AABB((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2)));
            return 0;
        }

        int Character::weight(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            characters[m_char]->phWeight((float)lua_tonumber(st, 1));
            return 0;
        }

        int Character::setMana(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            characters[m_char]->setManaMax((unsigned int)lua_tonumber(st, 1));
            return 0;
        }

        int Character::onGround(lua_State* st)
        {
            return helper::returnBoolean(st, characters[m_char]->onGround());
        }

        int Character::requireMana(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            return helper::returnBoolean(st, characters[m_char]->requireMana((unsigned int)lua_tonumber(st, 1)));
        }

        int Character::flip(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::BOOL)
                return 0;
            characters[m_char]->setFlip(lua_toboolean(st, 1));
            return 0;
        }
                
        int Character::setUsed(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            double c = lua_tonumber(st, 1);
            m_char = static_cast<int>(c);
            if(m_char < 0)
                m_char = 0;
            else if(m_char > 3)
                m_char = 3;
            return 0;
        }
                
        int Character::applyForce(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            physics::Entity* ent = characters[m_char]->entity();
            ent->applyForce((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2));
            return 0;
        }

        int Character::impulse(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            physics::Entity* ent = characters[m_char]->entity();
            ent->applyLinearImpulse((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2));
            return 0;
        }

        int Character::velocity(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            physics::Entity* ent = characters[m_char]->entity();
            ent->setLinearVelocity((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2));
            return 0;
        }
                
        int Character::stun(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            characters[m_char]->stun((unsigned int)lua_tointeger(st, 1));
            return 0;
        }

        int Character::impact(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 2
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER)
                return 0;
            characters[m_char]->impact((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2));
            return 0;
        }
                
        int Character::damage(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(args.size() != 1
                    || args[0] != Script::NUMBER)
                return 0;
            characters[m_char]->inflictDamages((int)lua_tointeger(st, 1));
            return 0;
        }

        int Character::attack(lua_State* st)
        {
            std::vector<Script::VarType> args = helper::listArguments(st);
            if(!(args.size() == 7
                        || (args.size() == 8 && args[7] == Script::BOOL))
                    || args[0] != Script::NUMBER
                    || args[1] != Script::NUMBER
                    || args[2] != Script::BOOL
                    || args[3] != Script::STRING
                    || args[4] != Script::STRING
                    || args[5] != Script::STRING
                    || args[6] != Script::STRING)
                return helper::returnBoolean(st, false);
            bool ret = characters[m_char]->createAttack(geometry::AABB((float)lua_tonumber(st, 1), (float)lua_tonumber(st, 2)),
                    lua_toboolean(st, 3), lua_tostring(st, 4), lua_tostring(st, 5),
                    lua_tostring(st, 6), lua_tostring(st, 7));
            return helper::returnBoolean(st, ret);
        }

    }
}



