#pragma once

#define DEBUG 1

#include <SFML/Graphics.hpp>
#include <SFML/Graphics/Vertex.hpp>
#include <cmath>
#include <stdexcept>
#include <limits>
#include "Player.h"
#include "WaveManager.h"
#include "UIManager.h"

/** Enum representing the game state. */
enum GameState
{
    /** Game is currently being played. */
    playing,

    /** On the main menu. */
    menu,

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
         * @brief Displays the main menu starting screen
         */
        void displayStartScreen();

    private:
        std::vector<Entity*> _entityVec;

        /**Enemy that is returned by the player's attack call*/
        Entity* _hitEnemy;

        /**Text that displays the damage dealt to the enemy*/
        sf::Text _hitIndicator;

        /**The window we are displaying in */
        sf::RenderWindow _gameWindow;

        /**Window that displays the start screen*/
        sf::RenderWindow _startScreen;

        /**Damage that is dealt to the enemy*/
        int _enemyDamage;

        /** The view, or "camera" that we are using to display the world. */
        sf::View _view;

        /** The current game state. */
        GameState _currentState;

        /** The player. */
        Player _player;

        /** The WaveManager, which owns all Enemies. */
        WaveManager _wave;

        UIManager _UIManager;

        /**Font used to display in-game text*/
        sf::Font _font;

        /**Font used to display the title*/
        sf::Font _titleFont;

        /**Font used to display the Game Over text*/
        sf::Font _deathFont;

        /**Number that records how long the hit indicators have displayed for*/
        float _indicatorTotal;

        std::vector<sf::Vertex> _debugLines;

        int _highScore;

        /**
         * @brief Run the main game loop 
         */
        void runGame();

        /**
         * @brief Draws debug lines from raycast command
         * DEAD CODE
         */
        void debugDraw();

        /**
         * @brief Called from main loop, turns all the user inputs into game instructions
         */
        void handleInput();

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
        void drawFrame(sf::Time frameTime);

        /**
         * @brief Called from drawFrame(),
         *  will move the current view based off of the player's location
         */
        void updateViewLocked();

        /**
         * @brief Called from drawFrame(),
         *  Draw a red glow
         */
        void drawRed();

        /**
         * @brief Called from drawFrame(),
         *  Draws the enemy health bars
         */
        void drawEnemyHealth();

        /**
         * @brief Called from drawFrame(),
         *  Draws the enemy hit indicators
         * 
         * @param e The enemy to draw the indicator on
         * @param frameTime The time to keep track of how long th indicator has displayed for
         * @param damage Damage number to be displayed
         */
        void drawHitIndicator(Entity* e, sf::Time frameTime, int damage);

        /**
         * @brief Called from displayStartScreen(),
         *  Draws the start screen
         */
        void drawStartScreen();

        /**
         * @brief Called from drawStartScreen(),
         *  Draws the start screen
         * 
         * @return Title text
         */
        sf::Text getTitle();

        /**
         * @brief Called from displayStartScreen(),
         *  Draws the start screen
         * 
         * @return Title text
         */
        sf::Text getStartText();

        /**
         * @brief Called from displayStartScreen(),
         *  Checks if the window is closed or if the space bar is pressed to start the game
         */
        void startScreenHandleInput();

        /**
         * @brief Get the player's high score
         * 
         * @return int The player's high score from a previous game
         */
        int getHighScore();

        /**
         * @brief Called from runGame(),
         *  Displays the death screen
         */
        void displayDeathScreen();

        /**
         * @brief Called from drawDeathScreen()
         *  Gets the "Game Over text"
         * 
         * @return the "game over" text
         */
        sf::Text getGameOverText();

        /**
         * @brief Called from displayDeathScreen(),
         *  Draws the death screen
         * 
         * @param time Time to control the fade in of the text
         */
        void drawDeathScreen(double time);

        /**
         * @brief Called from displayDeathScreen(),
         *  Checks if the window is closed or if the space bar is pressed to close the window
         */
        void deathScreenHandleInput();

        /**
         * @brief Called from displayStartScreen(),
         *  Checks if the window is closed or if the space bar is pressed to close 
         */
        sf::Text getHighScoreText();

        /**
         * @brief Helper function that updates the high score file when game is over
         * 
         * @pre Game is over or player is exiting game
         * 
         * @post new high score is written to highscore.txt
         * 
         */
        void updateHighScoreFile();

};
