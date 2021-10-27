#include "Player.h"
#include <cmath>

Player::Player() : 
    // Initialize movement vector to <0, 0>
    _moveVec(sf::Vector2<int>(0, 0))
{
    // Set default move state to None
    _currentMoveState = None;
}

// Pure virtuan function from the Entity class
void Player::onUpdate(float deltaTime)
{
    // At this point, we want to slow down if we are not currently moving 
    if(this->_moveVec.x == 0 && this->_moveVec.y == 0) 
    {
        // TODO: Slow down
        this->_velocity = sf::Vector2<float>(0, 0);
    }
    else
    {
        // We want to get the magnitude of the moveVector to make sure we're not moving
        // more when moving diagonally vs laterally
        float moveVecMagnitude = std::sqrt(this->_moveVec.x * this->_moveVec.x + 
                this->_moveVec.y * this->_moveVec.y);

        // Get the unit vector using the above magnitude
        sf::Vector2<float> moveVecUnit = this->_moveVec / moveVecMagnitude;

        // Set the current velocity to the movement vector
        // We want to multiply by a delta time so we can get an exact movement speed
        // no matter what the time difference between frames is,
        // we also want to multiply by the private move speed var so we don't move at
        // a snail's pace, this can be edited for better feeling movement before compile
        this->_velocity = moveVecUnit * this->_moveSpeed * deltaTime;
        
        // Reset the movement vector to <0, 0>
        this->_moveVec = sf::Vector2<float>(0, 0);
    }
}

void Player::onCollision(Entity &hitEntity)
{

}

void Player::moveInDirection(sf::Vector2<float> moveDir)
{
    // We want to round the product of the given moveDir
    this->_moveVec += moveDir;

    // We also want to tell the player it was given input, as to now slow its input down
    this->_currentMoveState = MoveState::Moving;
}

void Player::dodgeInDirection(sf::Vector2<float> dodgeDir)
{
    
}

void Player::attack()
{

}

void Player::counter()
{

}
