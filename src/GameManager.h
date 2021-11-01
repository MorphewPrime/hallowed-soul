#pragma once

#include <SFML/Graphics.hpp>
#include "Player.h"
#include "WaveManager.h"

/** Enum representing the game state. */
enum GameState
{
    /** Game is currently being played. */
    playing,

    /** Game just recieved the signal to resume */
    justResumed,

    /** On the main menu. */
    menu,

    /** Game just recieved the signal to pause */
    justPaused,

    /** Game is paused. */
    paused,

    /** Game is complete. */
    gameOver,

    /** Window is closing. */
    exiting
};

/** Class which handles the main loop, timing, and game logic. 
 * 
 * Calls drawing and updating functions explicitly on the Player and non-enemy Entities.
 * Enemies are fully handled and owned by WaveManager; GameManager calls WaveManager's
 * drawing and updating functions to control Enemies.
 */
class GameManager
{
    public:
        /**
         * @brief Default constructor
         */
        GameManager();

        /**
         * @brief Run the main game loop 
         */
        void runGame();

    private:
        /** The window we are displaying in */
        sf::RenderWindow _gameWindow;

        /** The view, or "camera" that we are using to display the world. */
        sf::View _view;

        /** The current game state. */
        GameState _currentState;

        /** The player. */
        Player _player;

        /** The WaveManager, which owns all Enemies. */
        WaveManager _wave;

        /** The clock used to get time between frames */
        sf::Clock gameClock;

        /** A texture holding a freeze frame.
         * 
         * Needed as an instance member for lifetime purposes.
         */
        sf::Texture frozenWindow;

        /** The time elapsed since the last frame.
         * 
         * Needed as an instance member for purposes of
         * persisting the time elapsed when game enters paused
         * state.
         */
        sf::Time timeElapsed;

        /**
         * @brief Event handler
         * 
         * @param event The event to handle
         */
        void handleEvent(sf::Event event);

        /**
         * @brief Called from handleInput(), will evaluate a keyboard event
         *  Used if you want an action to happen on a keystroke
         *
         * @param kbdEvent The keyboard event we're handling
         */
        void handleKeyboardEvent(sf::Event &kbdEvent);


        /**
         * @brief Called from handleInput(), will evaluate a mose event
         *  Used if you want an action to happen on scroll, click, or mouse move        *
         * 
         * @param mouseEvent The mose event we're handling
         */
        void handleMouseEvent(sf::Event &mouseEvent);

        /**
         * @brief Called from the main game loop
         *  will update all of our game objects using the read inputs
         *
         * @param frameTime The time between the last frame and this one
         */
        void updateEntities(sf::Time frameTime);

        /**
         * @brief Called from main loop,
         *  will check for collisions between entities and objects
         */
        void checkCollisions();
        
        /**
         * @brief Called from main game loop,
         *  will render all of our objects and entities to the view
         */
        void drawFrame();

        /**
         * @brief Called from drawFrame(),
         *  will move the current view based off of the player's location
         */
        void updateViewLocked();

        /**
         * @brief Called from drawFrame(),
         *  Temporary function to draw a basic background of our map
         */
        void drawMap();
    
        /**
         * @brief Called from drawFrame(),
         *  Draw the players health heads up display
         */
        void drawHealthHUD();

        /**
         * @brief Called from drawFrame(),
         *  Draw a heads up display on the current round information
         */
        void drawRoundProgressHUD();

        /** Helper function to check if key is pressed and move the player if so. */
        void movePlayer();

        /** Helper function for game logic/simulation when in the playing state */
        void tick(sf::Time timeElapsed);

        /** Helper function to get the background for the pause screen. 
         * 
         * @post frozenWindow is set to a dimmed version of the current screen.
         */
        void captureCurrentFrame();
};
