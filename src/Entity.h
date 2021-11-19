#pragma once

#include "SFML/System/Vector2.hpp"
#include "SFML/Graphics.hpp"

/** Abstract base class which entities are based off of.
 * 
 * Entities are generally anything that can have a sprite and/or a position.
 * 
 * To make your subclass of Entity visible on the screen, you need to do the following:
 * 
 * 1. set _texture
 * 2. set _sprite
 * 3. Somewhere in the main game loop where drawing happens, call your subclass's onDraw()
 * method and draw() the entity's sprite to the screen.
 * 
 * Updating position is handled by update(). Position is set based on the value of _velocity. You can
 * set _position or _velocity in onUpdate() to define the movement behavior of your Entity.
 */

enum EntityType
{
    PLAYER, ENEMY
};

/* A helper struct to store data that has to do with animation */
struct AnimationData {
    /** The current direction */
    enum Direction
    {
        Up,
        Left,
        Down,
        Right,
    } direction;
    /** The number of rows in the sprite sheet */
    int numRows;
    /** The number of columns in the sprite sheet */
    int numCols;
    /** The number of frames in the walking animation */
    int numWalkingFrames;
    /** The 0-indexed row number for the walking up animation */
    int upWalkRow;
    /** The 0-indexed row number for the walking left animation */
    int leftWalkRow;
    /** The 0-indexed row number for the walking down animation */
    int downWalkRow;
    /** The 0-indexed row number for the walking right animation */
    int rightWalkRow;
    /** The number of frames in the attacking animation */
    int numAttackingFrames;
    /** The 0-indexed row number for the attacking up animation */
    int upAttackRow;
    /** The 0-indexed row number for the attacking left animation */
    int leftAttackRow;
    /** The 0-indexed row number for the attacking down animation */
    int downAttackRow;
    /** The 0-indexed row number for the attacking right animation */
    int rightAttackRow;
    /** The (column, row) of the current frame to display */
    sf::Vector2i currentFrame;
    /** The amount of time since the frame was last changed */
    float timeAccumulated;
};

class Entity
{
    public:
        Entity(std::vector<Entity*> *entityVec);

        Entity(std::vector<Entity*> *entityVec, AnimationData animationData);

        /// Getters and Setters
        /**
         * @brief Getter for entity position
         *
         * @return Entity's position 
         */
        const sf::Vector2<float>& getPosition() const;

        /**
         * @brief Getter for entity velocity
         *
         * @return Entity's velocity 
         */
        const sf::Vector2<float>& getVelocity() const;

        /**
         * @brief Getter for entity active sprite
         *
         * @return Sprite to render to window for this entity 
         */
        const sf::Sprite& getSprite() const;

        /**
         * @brief Set the texture of this entity
         *
         * @param path The path of the texture
         */
        void setTexture(std::string path);

        /**
         * @brief Getter for entity width
         *
         * @return Width of entity 
         */
        const int& getWidth() const;


        /**
         * @brief Getter for entity height
         *
         * @return Height of entity 
         */
        const int& getHeight() const;


        /**
         * @brief Getter for entity health
         *
         * @return Health of entity 
         */
        const int& getHealth() const;

        /// Main function to update an entity 

        /**
         * @brief Called from GameManager, will update the entity based off user input
         *
         * @param deltaTime the time between this update and the previous update
         */
        void update(float deltaTime);
 
        /**
         * @brief Getter for is entity is alive
         *
         * @return If entity is alive 
         */
        bool isAlive();

        /// Interface methods with a default implementation, can be overridden

        /**
         * @brief Spawns the entity in at a given location
         *
         * @param spawnLocation The location to spawn the entity at
         */
        virtual void spawn(sf::Vector2<float> spawnLocation);

        /**
         * @brief Will tell this entity to lower it's health
         *
         * @param damage The amount of damage to do
         */
        virtual void doDamage(int damage);

        /**
         * @brief Kills the entity, stops it from being rendered on the scene
         *  and affecting collisions
         */
        virtual void kill();

        /** Called from GameManager to draw the entity.
         * 
         * Calls subclass implementation of onDraw().
         */
        void onDrawBase();

        /// Interface methods that must be overridden 
        
        virtual EntityType getEntityType() = 0;

        /**
         * @brief Called from update, the overridable function for specific entities
         *  to change their members based off user input
         *
         * @param deltaTime The time between this update and the last one in seconds
         */
        virtual void onUpdate(float deltaTime) = 0;

        /**
         * @brief Called from draw, change the active sprite to draw to the scene, and 
         *  set up current sprite for rendering 
         */
        virtual void onDraw();
        
        /**
         * @brief Called from the game manager after a collision with another entity
         *
         * @param hitEntity The other entity this one collider with
         */
        virtual void onCollision(Entity &hitEntity) = 0; 

        /**
         * @brief Does a raycast to detect if it hits any entity in the game world
         *
         * @param ray The direction to cast the ray in
         *
         * @return 
         */
        Entity* rayCast(const sf::Vector2<float> &ray);

        /** Vector for position in world coordinates.
         * 
         * Modify this to reposition the entity in the world.
         */ 
        sf::Vector2<float> _position;

    protected:

        /** Enum which holds what state the entity is in. */
        enum MoveState
        {
            /** The entity is moving. */
            Moving,
            /** The entity is dodging. */
            Dodging,
            /** An attack was just triggered */
            AttackTriggered,
            /** The entity is attacking. */
            Attacking
        } _currentMoveState;

        /** Vector for velocity. 
         * 
         * Modify this to give the entity a new velocity.
         */
        // Vectors for position and velocity
        sf::Vector2<float> _velocity;

        /** Width of this entity */
        int _width;

        /** Height of this entity */
        int _height;

        /** Health of this entity */
        int _health;

        /* If the entity is alive or not */
        bool _isAlive;

        /** The texture that this entity uses.
         * 
         * To give your subclass an image, set this in the constructor like so:
         * 
         *     _texture.loadFromFile("path/to/image.png");
         *     // set the sprite, see _sprite
         * 
         * The texture could potentially be a spritesheet with multiple frames of an animation, 
         * or could be just one drawing.
         */
        sf::Texture _texture;

        /** The sprite that this entity uses.
         * 
         * The active sprite adds a rectangle around a texture, defining the region
         * of the texture that we actually want to display.
         * 
         * To give your subclass a sprite, do something like the following in the constructor
         * after giving your subclass a texture:
         * 
         *     // You've already set _texture, now set _sprite
         *     _sprite.setTexture(_texture);
         *     // set your rectangle, origin, etc. See docs on sf::Sprite.
         *     // Here's an example:
         *     _sprite.setOrigin((int)(_texture.getSize().x / 2), (int)(_texture.getSize().y / 2));
         * 
         * One use case of this is animation; you could use one texture to store all the frames
         * of an animation and update the sprite to change which part of the texture it renders
         * when you want to show a different animation frame.
         */
        // TODO: Sprite array?
        sf::Sprite _sprite;

        /** Sets the sprite direction by reading the entity's velocity*/
        void setSpriteDirection();

        /** Sets the walking frame, general for entities that can walk. */
        void setWalkingFrame();

        /** Sets the attacking frame, general for entities that can attack. */
        void setAttackingFrame();

        /* A helper struct to store data that has to do with animation */
        AnimationData animationData;

        /** Helper function to update the sprite rectangle */
        void updateTextureRect();

        /** Compute seconds per frame based on velocity 
         * 
         * Override this to define how fast you would like your subclass to be animated at any given time.
         * 
         * @returns how many seconds to display the current frame
        */
        virtual float getSecondsPerFrame() const;
        // This is going to be the list of all entities in the game, we need this for ray casting
        std::vector<Entity*> *_entityVec;

        bool linesIntersect(float x1, float y1, float x2, float y2, float x3, float y3,
                float x4, float y4);
};
