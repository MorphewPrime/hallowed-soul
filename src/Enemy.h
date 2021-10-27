#pragma once
#include "Entity.h"

class Enemy : public Entity
{
    private:
        int ammo;
        bool isAlive;
        
    public:
        Enemy();

        void attack();
        void takeDamage(int damage);
        int getHealth();
        int getAmmo();
        bool getIsAlive();

        void onCollision(Entity &hitEntity){}
        void onUpdate(float deltaTime){}

        ~Enemy();
};
