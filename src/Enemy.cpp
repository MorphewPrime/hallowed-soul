#include "Enemy.h"

Enemy::Enemy()
{
    _ammo = 0;
    _position = sf::Vector2<float> (0,0);
    _velocity = sf::Vector2<float>(0, 0);
    _atkTime = 0;
    _attacking = false;

    this->setTexture("assets/textures/test.png");
}

int Enemy::getAmmo()
{
    return _ammo;
}

void Enemy::spawn(sf::Vector2<float> pos)
{
    _position = pos;
}

void Enemy::setPlayer(Player* playerRef)
{
    _player = playerRef;
}

void Enemy::setFriends(std::vector<Enemy*>& friendRef)
{
    _friends = &friendRef;
}

void Enemy::onUpdate(float deltaTime)
{
    if(((_position.x-_player->getPosition().x<30&&_position.x-_player->getPosition().x>-30
            &&_position.y-_player->getPosition().y<30&&_position.y-_player->getPosition().y>-30)
            &&!_player->isDodging())||_attacking)
    {
        _attacking = true;
        _velocity = sf::Vector2<float> (0,0);
        _atkTime += deltaTime;
        if(_atkTime >= 1)
        {
            _attacking = false;
            _atkTime = 0;
            _player->doDamage(10);
        }
    }
    else
    {
        this->_velocity = sf::Vector2<float>(this->_player->getPosition().x-this->_position.x,
                this->_player->getPosition().y-this->_position.y);
        for(int i=0; i<_friends->size(); i++)
        {
            if((_position.x-_friends->at(i)->getPosition().x<35&&_position.x-_friends->at(i)->getPosition().x>-35
                    &&_position.y-_friends->at(i)->getPosition().y<35&&_position.y-_friends->at(i)->getPosition().y>-35)
                    &&_friends->at(i)!=this&&_friends->at(i)->isAlive())
            {
                if(_position.x-_friends->at(i)->getPosition().x<30&&_velocity.x>0)
                {
                    _velocity.x = 0;
                }
                if(_position.x-_friends->at(i)->getPosition().x>-30&&_velocity.x<0)
                {
                    _velocity.x = 0;
                }
                if(_position.y-_friends->at(i)->getPosition().y<30&&_velocity.y>0)
                {
                    _velocity.y = 0;
                }
                if(_position.y-_friends->at(i)->getPosition().y>-30&&_velocity.y<0)
                {
                    _velocity.y = 0;
                }
            }
        }
        if(_velocity!=sf::Vector2<float> (0,0))
        {
            this->_velocity = VectorUtil::getUnitVector(this->_velocity); 
            this->_velocity *= deltaTime * 5000;
        }
    }
}

Enemy::~Enemy()
{

}
