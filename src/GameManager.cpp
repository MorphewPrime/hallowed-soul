#include "GameManager.h"
#include <cmath>
#include <iostream>

GameManager::GameManager() : 
    // First thing we want to do is create a window
    // TODO: Name and size subject to change
    _gameWindow {sf::VideoMode(1280, 720), "Hallowed Soul"}, 
    // Initialize the view (camera) 
    _view {sf::FloatRect(0, 0, 1280.0 / 2.0, 720.0 / 2.0)}
{
    // Set default game state
    // TODO: If we have a main menu, change the default state to that
    _currentState = GameState::playing;

    // This defines where our viewport is set to start
    // TODO: We will probably be spawning the player in the start of the map
    _view.setViewport({0.0f, 0.0f, 1.0f, 1.0f});

    // The view will display the top quarter of the map (_gameWindow),
    // but will take up the full size of the RenderWindow. Therefore,
    // this should zoom in on the gameWindow.
    _gameWindow.setView(_view);
}

void GameManager::runGame()
{
    // Game clock for tracking time
    sf::Clock gameClock;

    // Keep going while the window is open
    while(this->_gameWindow.isOpen())
    {
        // Update the game clock and get the frame time
        sf::Time frameTime = gameClock.restart();

        // This is the main game loop, there's a specific order we want to execute our loop in
        // First we need to consider that the only thing that will change our objects is
        // input from the user, so that's the first thing we want to do
        handleInput();

        // Once input is handled, we now want to update all of our objects
        updateEntities(frameTime);

        // Next step is to check the collisions on all of our entities
        checkCollisions();

        // Finally we want to draw the frame
        drawFrame();

        // We also want to check if the game state is exit, if it is then we break
        if(_currentState == GameState::exiting)
        {
            _gameWindow.close();
            break;
        }
    }
}

void GameManager::handleInput()
{
    // Event object for the current event we're handling
    sf::Event currentEvent;

    // We want to poll all avalible events in the game
    // TODO: This may be unnecessary and slow, since this while loop could be processing up to 50 events at once
    // We may not even need this at all
    while(_gameWindow.pollEvent(currentEvent))
    {
        // TODO: Control variables? Maybe some config file?
        // First we want to check what type of event it is 
        switch(currentEvent.type)
        {
            case sf::Event::Closed:
            {
                this->_currentState = GameState::exiting;
                break;
            }
            default: // Otherwise just do nothing
                break;
        }
    }       

    // The following four if statements will tell the player it needs to be moving
    // in the direction based off of the directional keys pressed.
    // We are using all ifs here because we want diagonal movement to be possible
    // Additionally, we're not using the above loop to check because we don't just want to
    // move up when the up key is pressed, we want to continue moving up while whenever the
    // up key is held, this solves that issue
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Up))
    {
        // Up is negative y direction
        this->_player.moveInDirection(sf::Vector2<float>(0, -1));
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Down))
    {
        // Down is positive y direction
        this->_player.moveInDirection(sf::Vector2<float>(0, 1));
    }
    
    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Left))
    {
        // Left is negative x direction
        this->_player.moveInDirection(sf::Vector2<float>(-1, 0));
    }

    if(sf::Keyboard::isKeyPressed(sf::Keyboard::Right))
    {
        // Right is positive x direction
        this->_player.moveInDirection(sf::Vector2<float>(1, 0));
    }
}

void GameManager::handleMouseEvent(sf::Event &mouseEvent)
{
    // TODO: Do we need this?
    // If we implement menus we will, but consider remove otherwise
}

void GameManager::checkCollisions()
{
    // TODO: Check for collision between dynamic entities and other entities
}

void GameManager::updateEntities(sf::Time frameTime)
{
    this->_player.update(frameTime.asSeconds());
    // TODO: Update other entities
}

void GameManager::drawFrame()
{
    // Clear current buffer
    _gameWindow.clear();

    // need to update the camera before drawing anything
    // updateView();

    drawMap();
    // updateViewLocked();
    updateViewLoose();

    // Drawing an entity has two steps: calling the onDraw method to update the entity's sprite
    // and calling the game window draw function
    this->_player.onDraw();
    this->_gameWindow.draw(this->_player.getSprite());
    // TODO: Add other entities

    // Finally, display the window
    _gameWindow.display();    
}

void GameManager::updateView()
{
    // TODO: this is a mess, clean it up

    // I want to define a 100x50 rectangle in the middle of the view.
    // If the player walks outside of this rectangle, then we should
    // move the view to follow it and keep it in the rectangle.
    const int centerRectWidth = 100;
    const int centerRectHeight = 50;
    sf::Vector2i posInView = _gameWindow.mapCoordsToPixel(static_cast<sf::Vector2f>(this->_player.getPosition()));
    sf::Vector2i viewCenter = static_cast<sf::Vector2i>(_view.getSize() * 0.5f);
    sf::Vector2i displFromCenter = posInView - viewCenter;
    sf::Vector2i outsideRect {0, 0}; // Vector that gives us how outside of the rectangle we are
    if (displFromCenter.x < -(centerRectWidth/2))
        outsideRect.x = displFromCenter.x + (centerRectWidth/2);
    else if (displFromCenter.x > (centerRectWidth/2))
        outsideRect.x = displFromCenter.x - (centerRectWidth/2);
    
    if (displFromCenter.y < -(centerRectHeight/2))
        outsideRect.y = displFromCenter.y + (centerRectHeight/2);
    else if (displFromCenter.y > (centerRectHeight/2))
        outsideRect.y = displFromCenter.y - (centerRectHeight/2);

    sf::Vector2i topLeft = static_cast<sf::Vector2i>(_gameWindow.mapPixelToCoords({0, 0}));
    sf::Vector2i bottomRight = static_cast<sf::Vector2i>(_gameWindow.mapPixelToCoords(static_cast<sf::Vector2i>(_view.getSize())));
    sf::Vector2i translateView = outsideRect;
    if (topLeft.x + outsideRect.x < 0)
        translateView.x = -topLeft.x;
    else if (bottomRight.x + outsideRect.x > _gameWindow.getSize().x)
        translateView.x = _gameWindow.getSize().x - bottomRight.x;

    if (topLeft.y + outsideRect.y < 0)
        translateView.y = -topLeft.y;
    else if (bottomRight.y + outsideRect.y > _gameWindow.getSize().y)
        translateView.y = _gameWindow.getSize().y - bottomRight.y;

    _view.move(static_cast<sf::Vector2f>(translateView));

    _gameWindow.setView(_view);
}

void GameManager::drawMap()
{
    sf::Texture texture;
    texture.loadFromFile("assets/textures/temp_bg.jpg");

    sf::IntRect rectSourceSprite(0, 0, 1500, 1125);
    sf::Sprite sprite(texture, rectSourceSprite);

    this->_gameWindow.draw(sprite);
}

void GameManager::updateViewLocked()
{
    sf::View view = _gameWindow.getView();

    view.setCenter(this->_player.getPosition());

    _gameWindow.setView(view);
}

void GameManager::updateViewLoose()
{
    //TODO: restrict camera to the map bounds? move to some place else? adjust buffers?
    sf::View view = _gameWindow.getView();
    const sf::Vector2f& viewCenter = view.getCenter();
    const sf::Vector2f& playerLocation = this->_player.getPosition();
    const sf::Vector2u& windowSize = _gameWindow.getSize();
    const float playerCamDistX = playerLocation.x - viewCenter.x;
    const float playerCamDistY = playerLocation.y - viewCenter.y;
    const float xBufferSize = windowSize.x * 3/40;
    const float yBufferSize = windowSize.y * 1/40;

    // If you go past the "buffer" X zone the camera will reposition horizontally.
    if (xBufferSize < std::abs(playerCamDistX)) {
        if (playerLocation.x > viewCenter.x) { // player is to the right of the camera
            view.setCenter(sf::Vector2f{viewCenter.x + (playerCamDistX - xBufferSize), viewCenter.y});
           
        } else if (playerLocation.x < viewCenter.x){ // player is to the left of the camera
            view.setCenter(sf::Vector2f{viewCenter.x - (std::abs(playerCamDistX) - xBufferSize), viewCenter.y});
        }
    }

    // If you go past the "buffer" Y zone the camera will reposition vertically.
    if (yBufferSize < std::abs(playerCamDistY)) {
        if (playerLocation.y > viewCenter.y) { // player is to below the camera
            view.setCenter(sf::Vector2f{viewCenter.x, viewCenter.y + (playerCamDistY - yBufferSize)});
           
        } else if (playerLocation.y < viewCenter.y){ // player is above of the camera
            view.setCenter(sf::Vector2f{viewCenter.x, viewCenter.y - (std::abs(playerCamDistY) - yBufferSize)});
        }
    }

    _gameWindow.setView(view);
}
