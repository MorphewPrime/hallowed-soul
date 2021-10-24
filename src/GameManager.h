#ifndef GAMEMANAGER_H
#define GAMEMANAGER_H

#include <SFML/Graphics.hpp>
#include <forward_list>
#include "Entity.h"
#include "TestEntity.h"

enum GameState
{
    playing, menu, gameOver, exiting
};

class GameManager
{
    public:
        GameManager();

        /** Run the main game loop 
         * 
         * @returns The status code.
         */
        int runGame();

        /** List of all entities to update/draw in the game loop.
         * 
         * Entities register themselves in this list on creation,
         * and remove themselves from this list when destroyed.
         * Therefore, as long as you don't add/remove entities
         * to this yourself, every entity in this list is guaranteed
         * to be valid.
         */
        static std::forward_list<Entity*> entities;

    private:
        sf::RenderWindow _gameWindow;
        GameState _currentState;
        sf::View _view;
        // Player _player;
        TestEntity testEntity;

        void runFrame();
        void handleInput();
        void checkCollisions();
        void drawFrame();
        void updateEntities();
        void updateView();
};

#endif
