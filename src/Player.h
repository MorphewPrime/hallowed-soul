#pragma once

#include <math.h>
#include "Entity.h"
#include "VectorUtil.h"

/** Class for the player.
 * 
 * The Player is an Entity which is controlled by the player and responds to keyboard input.
 */
class Player : public Entity
{
    public:
        /**
         * @brief Constructor
         *
         * @param entityVec Vector containing all entities
         */
        Player(std::vector<Entity*> *entityVec);

        /**
         * @brief Tells the player which direction it needs to be moving 
         *
         * @param moveDir The direction from either the controller, or a keyboard input
         *  these will be added later
         */
        void moveInDirection(sf::Vector2<float> moveDir);

        /**
         * @brief Tells the player which direction it needs to be dodging
         *
         * @param dodgeDir Pretty sure we don't need the direction, to be removed
         */
        void dodgeInDirection(sf::Vector2<float> dodgeDir);

        /**
         * @brief Spawns the player at a location
         *
         * @param spawnLocation The location to spawn the player at
         */
        void spawn(sf::Vector2<float> spawnLocation);

        /**
         * @brief Tells the player it needs to be attacking 
         */
        Entity* attack(int damage);

        /**
         * @brief checks if player is dodging
         * 
         * @return true if dodging
         */
        bool isDodging();

        /**
         * @brief checks if player is red
         * 
         * @return true if red
         */
        bool isRed();

        /**
         * @brief override for entity's doDamage
         * 
         * @param damage damage amount
         */
        void doDamage(int damage);

        /**
         * @brief Gets the last direction the player moved in (for attacking)
         *
         * @return Hmmmm, I wonder? Maybe the last direction the player moved in?? 
         */
        const sf::Vector2<float>& getLastMoveDirection() const;

        /**
         * @brief Getter for attack range
         *
         * @return It returns a random number. Just kidding it's the constant attack range of
         *  the player
         */
        float getAttackRange() const;

        /**
         * Implementation of onDraw()
         */
        void onDraw();

        /**
         * @brief Gets the type of entity this is
         *
         * @return The type of entity 
         */
        EntityType getEntityType();

    private:
        // Constants for player movement
        const float _moveSpeed = 200;
        const float _dodgeSpeed = 1100;
        const float _friction = 1600;
        const float _dodgeFriction = 9000;
        const float _attackRange = 40;
        const float _deadZone = 0.01;
        const float _attackTime = 0.6;
        const float _dodgeTime = 0.6;

        // Current direction to move in based off given user input
        sf::Vector2<float> _moveVec;
        sf::Vector2<float> _lastMoveVec;

        // Current time since last attack
        float _lastAttackTime, _lastDodgeTime;

        // tracking red glow
        float _redTime;
        bool _isRed;

        // Current direction player is dodging in, as well as the speed of the dodge
        sf::Vector2<float> _dodgeVec;

        /**
         * @brief Overrided function from Entity, auto called from GameManager
         *  Used to update the entity based off user input
         *
         * @param deltaTime Time between last update and this one
         */
        void onUpdate(float deltaTime);

        /**
         * @brief Overrided function from Entity, auto called from GameManager
         *  Used to handle collisions
         *
         * @param hitEntity
         */
        void onCollision(Entity &hitEntity); 

        /**
         * @brief Handles slowing down when player isn't moving in a particular direction
         *
         * @param velAxis The velocity axis we're checking
         * @param moveAxis The direction we're supposed to be moving
         * @param friction The friction for this type of movement
         * @param deltaTime The time needed to properly adjust for velocity and acceleration
         *
         * @return The new value for the velocity after slow down 
         */
        float checkDeadMoveAxis(float velAxis, float moveAxis, float friction, float deltaTime);

        /**
         * @brief Get the seconds per frame for the player
         * 
         * Overrides the method in Entity because there is only one attack frame,
         * so it needs to be given more time.
         * 
         * @return the seconds per frame to animate at.
         */
        float getSecondsPerFrame() const;
};
