#pragma once

#include <vector>
#include <cmath>

#include "Entity.h"
#include "Player.h"
#include "VectorUtil.h"

/** Enemy class */
class Enemy : public Entity
{
    private:
        int _ammo;
        float _atkTime;
        bool _attacking;
        Player* _player;
        std::vector<Enemy*>* _friends;
        float _speed;
        float _stun;

        const float _attackRange = 50; 
        sf::Vector2<float> _attackDir;
        
    public:
        /**
         * @brief Constructor
         *
         * @param entityVec Vector holding all entities
         */
        Enemy(std::vector<Entity*> *entityVec);

        /**
         * @brief changes enemy health amount
         * 
         * @param damage amount to remove from health
         */
        void takeDamage(int damage);

        /**
         * @brief get current ammo
         *  note: ammo not currently in use
         * 
         * @return ammo
         */
        int getAmmo();
        
        /**
         * @brief get if the enemy is alive
         * 
         * @return true if alive
         */
        bool getIsAlive();

        /**
         * @brief sets position to chosen location
         * 
         * @param pos the position chosen to spawn at
         */
        void spawn(sf::Vector2<float> pos);

        /**
         * @brief establishes player pointer
         * 
         * @param playerRef a pointer to the player, as to be sent from WaveManager
         */
        void setPlayer(Player* playerRef);

        /**
         * @brief establishes pointer to other enemies
         * 
         * @param friendRef reference to enemy vector
         */
        void setFriends(std::vector<Enemy*>& friendRef);


        /**
         * @brief basic AI structure, determines movement/attacks
         * 
         * @param deltaTime time since last frame in seconds
         */
        void onUpdate(float deltaTime);

        /**
         * Implementation of onDraw()
         */
        void onDraw();

        /**
         * @brief doDamage override to stun enemies
         * 
         * @param damage damage amount
         */
        void doDamage(int damage);

        /**
         * @brief does nothing, exists for pure virtual override
         */
        void onCollision(Entity &hitEntity) {}

        /**
         * @brief Gets the entity type
         *
         * @return Entity type 
         */
        EntityType getEntityType();
};
