#include "GameManager.h"
#include <cmath>
#include <iostream>

const sf::Vector2u windowSize {1280, 720};

GameManager::GameManager() : 
    // First thing we want to do is create a window
    // TODO: Name and size subject to change
    _gameWindow {sf::VideoMode(windowSize.x, windowSize.y), "Hallowed Soul"}, 
    // Initialize the view (camera) 
    _view {sf::FloatRect(0, 0, windowSize.x/2, windowSize.y/2)}
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

    updateViewLoose();
    drawMap();
    // updateViewLocked();

    // Drawing an entity has two steps: calling the onDraw method to update the entity's sprite
    // and calling the game window draw function
    this->_player.onDraw();
    this->_gameWindow.draw(this->_player.getSprite());
    // TODO: Add other entities

    // Finally, display the window
    _gameWindow.display();    
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
    const sf::Vector2f& viewCenter = _view.getCenter();
    const sf::Vector2f& playerLocation = this->_player.getPosition();
    const sf::Vector2u& windowSize = _gameWindow.getSize();
    sf::Vector2f viewSize = _view.getSize();
    const float playerCamDistX = playerLocation.x - viewCenter.x;
    const float playerCamDistY = playerLocation.y - viewCenter.y;
    const float xBufferSize = windowSize.x * 3/40;
    const float yBufferSize = windowSize.y * 1/40;

    // Get the corners of the world in the view's coordinate frame
    sf::Vector2f worldTopLeft = static_cast<sf::Vector2f>(_gameWindow.mapCoordsToPixel({0.0f, 0.0f}));
    sf::Vector2f worldBottomRight = static_cast<sf::Vector2f>(_gameWindow.mapCoordsToPixel(static_cast<sf::Vector2f>(windowSize)));

    std::cout << "Top left: " << worldTopLeft.x << " " << worldTopLeft.y << "\n";
    std::cout << "Bottom right: " << worldBottomRight.x << " " << worldBottomRight.y << "\n";

    // Define a "move" vector to determine how much to move the view.
    sf::Vector2f moveView {0.0, 0.0};


    // If you go past the "buffer" X zone the camera will reposition horizontally.
    if (xBufferSize < std::abs(playerCamDistX)) {
        if (playerLocation.x > viewCenter.x) { // player is to the right of the camera
            moveView.x = playerCamDistX - xBufferSize; // init move vector x to the amount which the player is outside the buffer

            // Don't allow the view to go past the edge of the window
            if (worldBottomRight.x < viewSize.x + moveView.x) // if the view would go past the right edge of the world
                moveView.x = worldBottomRight.x - viewSize.x;
           
        } else if (playerLocation.x < viewCenter.x){ // player is to the left of the camera
            moveView.x = -(std::abs(playerCamDistX) - xBufferSize);

            if (moveView.x < worldTopLeft.x) // if the amount we are trying to move is farther than the left edge of the world in the view's coordinate frame
            // i.e. the view would go past the left edge of the world
                moveView.x = worldTopLeft.x;
        }
    }

    // If you go past the "buffer" Y zone the camera will reposition vertically.
    if (yBufferSize < std::abs(playerCamDistY)) {
        if (playerLocation.y > viewCenter.y) { // player is to below the camera
            moveView.y = playerCamDistY - yBufferSize;

            if (moveView.y > worldBottomRight.y - viewSize.y) // if the view would go past the bottom edge of the world
                moveView.y = worldBottomRight.y - viewSize.y;
           
        } else if (playerLocation.y < viewCenter.y){ // player is above of the camera
            moveView.y = -(std::abs(playerCamDistY) - yBufferSize);

            if (moveView.y < worldTopLeft.y) // if the view would go past the top edge of the world
                moveView.y = worldTopLeft.y;
        }
    }

    std::cout << "Move view: " << moveView.x << " " << moveView.y << "\n";

    _view.move(moveView);

    _gameWindow.setView(_view);
}
