#include "Enemy.h"

Enemy::Enemy()
{
    _health = 0;
    ammo = 0;
    isAlive = true;
}

void Enemy::attack()
{
    //Attack logic
}

void Enemy::takeDamage(int damage)
{
    _health = _health - damage;
    if(_health<=0)
    {
        // the operator "this" does not exist in c++, just call kill()
        kill();
    }
    else
    {

    }
}

int Enemy::getHealth()
{
    return _health;
}

int Enemy::getAmmo()
{
    return ammo;
}

bool Enemy::getIsAlive()
{
    return isAlive;
}

Enemy::~Enemy()
{

}
