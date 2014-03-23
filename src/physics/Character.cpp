
#include "Character.hpp"
#include "World.hpp"
#include <algorithm>

namespace physics
{
    void Character::character_foot_callback(Entity* ch, Entity* ground, bool start, void*)
    {
        Character* chara = dynamic_cast<Character*>(ch);
        auto it = std::find(chara->m_underfoot.begin(), chara->m_underfoot.end(), ground);
        if(!start && it != chara->m_underfoot.end())
            chara->m_underfoot.erase(it);
        else if(start && it == chara->m_underfoot.end())
            chara->m_underfoot.push_back(ground);
    }

    Character::Character(const std::string& name, World* world, const geometry::Point& position, const geometry::AABB& rect, float weight) : Entity(name, world, position, b2_dynamicBody, Entity::Type::Character), m_world(world)
    {
        createFixture("body", rect, weight, 1);

        geometry::AABB feet(rect.width, 10);
        geometry::Point pos(0, rect.height / 2);
        b2Fixture* sens = createFixture("feet", feet, 1, 1, Type::ThisType, Type::ThisCollideWith, pos, true);
        world->setCallback(this, sens, character_foot_callback);
    }

    Character::~Character()
    {
        b2Fixture* sens = getFixture("feet");
        m_world->removeCallback(this, sens);
    }

    void Character::jump(float vel)
    {
        float force = vel * getFixture("body")->GetDensity();
        applyLinearImpulse(0.0f, force);
        for(Entity* ent : m_underfoot)
            ent->applyLinearImpulse(0.0f, -force);
    }
            
    bool Character::onGround() const
    {
        return !m_underfoot.empty();
    }
}
