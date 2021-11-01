#include "GameManager.h"
#include <cmath>
#include <stdexcept>

GameManager::GameManager() : 
    // First thing we want to do is create a window
    // TODO: Name and size subject to change
    _gameWindow {sf::VideoMode(1280, 720), "Hallowed Soul"}, 
    // Initialize the view (camera) 
    _view {sf::FloatRect(0.0, 0.0, 1280.0 / 2.0, 720.0 / 2.0)}
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
    frozenWindow.create(_gameWindow.getSize().x, _gameWindow.getSize().y);
    this->_wave.setPlayer(this->_player);
}

void GameManager::runGame()
{
    this->_wave.beginWave();

    // Keep going while the window is open
    while(this->_gameWindow.isOpen())
    {

        // Andrew R: moving event polling directly in the main loop.
        // Reason: We have to poll events no matter what or else
        // the window will become non-responsive

        // Event object for the current event we're handling
        sf::Event currentEvent;
        // Get the current event
        _gameWindow.pollEvent(currentEvent);

        // Handle that event
        handleEvent(currentEvent);

        // If we need to exit now, exit
        if (_currentState == GameState::exiting)
        {
            _gameWindow.close();
            break;
        }
        // If playing the game, proceed with the simulation
        else if (_currentState == GameState::playing)
        {
            // Update the game clock and get the frame time
            timeElapsed = gameClock.restart();
            tick(timeElapsed);
        }

        // If just paused, capture the current time
        else if (_currentState == GameState::justPaused)
        {
            timeElapsed = gameClock.restart();
            // DON'T UPDATE THE GAME STATE HERE, this
            // will be done in drawFrame() because we still
            // need to capture the contents of the screen.
        }
        // If just resumed, start ticking but use the saved timeElapsed,
        // not the one recorded by the clock.
        else if (_currentState == GameState::justResumed)
        {
            gameClock.restart();
            tick(timeElapsed);
            _currentState = GameState::playing; // okay to update game state here
        }
        
        // Finally we want to draw the frame
        drawFrame();
    }
}

void GameManager::handleEvent(sf::Event event)
{
    // TODO: Control variables? Maybe some config file?
    // First we want to check what type of event it is 
    switch(event.type)
    {
        case sf::Event::KeyPressed:
        {
            this->handleKeyboardEvent(event);
            break;
        }
        case sf::Event::Closed:
        {
            this->_currentState = GameState::exiting;
            break;
        }
    }       
}

void GameManager::handleMouseEvent(sf::Event &mouseEvent)
{
    // TODO: Do we need this?
    // If we implement menus we will, but consider remove otherwise
}

void GameManager::handleKeyboardEvent(sf::Event &kdbEvent)
{
    switch (_currentState)
    {
        case GameState::playing:
        {
            switch(kdbEvent.key.code)
            {
                case sf::Keyboard::Space:
                {
                    this->_player.dodgeInDirection(sf::Vector2<float>(0, 0));
                    break;
                }
                case sf::Keyboard::Backspace:
                {
                    // THE KILL BUTTON
                    for(int i=0; i<this->_wave.getEnemies(); i++)
                    {
                        try
                        {
                            if(this->_wave.getEnemy(i)->getIsAlive())
                            {
                                this->_wave.getEnemy(i)->kill();
                                this->_wave.updateAliveEnemyCount();
                                break;
                            }
                        }
                        catch(const std::exception& e)
                        {
                            break;
                        }
                    }
                }
                case sf::Keyboard::Escape:
                    _currentState = justPaused;
                default:
                    // Do nothing
                    break;
            }
        }
    }
}

void GameManager::tick(sf::Time timeElapsed)
{
    movePlayer();

    // This can go anywhere, really
    this->_wave.updateWaves();

    // Once input is handled, we now want to update all of our objects
    updateEntities(timeElapsed);

    // Next step is to check the collisions on all of our entities
    checkCollisions();
}

void GameManager::movePlayer()
{
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

void GameManager::checkCollisions()
{
    // TODO: Check for collision between dynamic entities and other entities
}

void GameManager::updateEntities(sf::Time frameTime)
{
    this->_player.update(frameTime.asSeconds());
    // TODO: Update other entities
    this->_wave.updateEnemies(frameTime.asSeconds());
}

void GameManager::drawFrame()
{
    switch (_currentState)
    {
        case GameState::justPaused:
        {
            captureCurrentFrame();
            _currentState = GameState::paused;
        }
        case GameState::paused:
        {
            // Draw the pause background
            _gameWindow.clear();
            _gameWindow.setView(_view);
            _gameWindow.draw(sf::Sprite(frozenWindow));
            // Generate a mask
            sf::RenderTexture mask;
            mask.create(_gameWindow.getSize().x, _gameWindow.getSize().y);
            mask.clear({0, 0, 0, 127}); // a dark mask with some transparency
            _gameWindow.draw(sf::Sprite(mask.getTexture()));
            _gameWindow.display();

            // TODO: Also draw buttons
        }
        case GameState::playing:
        {
            // Clear current buffer
            _gameWindow.clear();

            // Now update the position of the view as nessisary.
            updateViewLocked();

            // Draw the temporary background before anything else
            drawMap();

            // Drawing an entity has two steps: calling the onDraw method to update the entity's sprite
            // and calling the game window draw function
            this->_player.onDraw();
            this->_wave.waveDraw();
            this->_gameWindow.draw(this->_player.getSprite());
            for(int i=0; i<this->_wave.getEnemies(); i++)
            {
                if(this->_wave.getEnemy(i)->getIsAlive())
                {
                    this->_gameWindow.draw(this->_wave.getEnemy(i)->getSprite());
                }
            }
            // TODO: Add other entities

            // Draw the HUD over most things
            drawHealthHUD();
            drawRoundProgressHUD();

            // Finally, display the window
            _gameWindow.display(); 
        }
    }   
}

void GameManager::updateViewLocked()
{
    sf::View view = _gameWindow.getView();
    const sf::Vector2f &playerLocation = this->_player.getPosition();
    const sf::Vector2f &viewSize = _view.getSize();
    sf::Vector2f mapSize{1500.0, 1125.0}; // this can probably be moved to a member variable later when the map is made.

    view.setCenter(this->_player.getPosition());

    if (playerLocation.x < viewSize.x / 2) // If camera view is extends past left side of the map.
    {
        view.setCenter(sf::Vector2f{viewSize.x / 2, view.getCenter().y});
    }
    else if (playerLocation.x + viewSize.x / 2 > mapSize.x) // If camera view is extends past right side of the map.
    {
        view.setCenter(sf::Vector2f{mapSize.x - (viewSize.x / 2), view.getCenter().y});
    }

    if (playerLocation.y < viewSize.y / 2) // If camera view is extends past top side of the map.
    {
        view.setCenter(sf::Vector2f{view.getCenter().x, viewSize.y / 2});
    }
    else if (playerLocation.y + viewSize.y / 2 > mapSize.y) // If camera view is extends past bottom side of the map.
    {
        view.setCenter(sf::Vector2f{view.getCenter().x, mapSize.y - (viewSize.y / 2)});
    }

    _gameWindow.setView(view);
}

void GameManager::drawMap()
{
    sf::Texture texture;
    texture.loadFromFile("assets/textures/temp_floor_128.png");
    texture.setRepeated(true);

    sf::IntRect rectSourceSprite(0, 0, 1500, 1125);
    sf::Sprite sprite(texture, rectSourceSprite);

    this->_gameWindow.draw(sprite);
}

void GameManager::drawHealthHUD()
{
    const int lineSize = 2;
    const sf::Vector2f viewCenter = _gameWindow.getView().getCenter();
    const sf::Vector2f &viewSize = _view.getSize();
    const sf::Vector2f barOutterSize{100.f, 10.f};
    const sf::Vector2f barInnerSize{barOutterSize.x * ((float)_player.getHealth() / 100), barOutterSize.y};
    const sf::Vector2i padding{5 + lineSize, 5 + lineSize};
    const sf::Vector2f barPosition{viewCenter.x + padding.x - (viewSize.x / 2), viewCenter.y - padding.y - barOutterSize.y + viewSize.y / 2};

    // This is the outside grey/black rectangle.
    sf::RectangleShape outsideRect(barOutterSize);
    outsideRect.setPosition(barPosition);
    outsideRect.setFillColor(sf::Color(45, 45, 45, 255));
    outsideRect.setOutlineColor(sf::Color::Black);
    outsideRect.setOutlineThickness(lineSize);
    _gameWindow.draw(outsideRect);

    // This is the inside red rectangle.
    sf::RectangleShape insideRect(barInnerSize);
    insideRect.setPosition(barPosition);
    insideRect.setFillColor(sf::Color(255, 0, 0, 255));
    _gameWindow.draw(insideRect);
}

void GameManager::drawRoundProgressHUD()
{
    float enemiesAlive = (float)this->_wave.getEnemiesAlive();
    float totalEnemies = (float)this->_wave.getEnemies();
    int currWave = this->_wave.getWave();

    const int lineSize = 2;
    const sf::Vector2f viewCenter = _gameWindow.getView().getCenter();
    const sf::Vector2f &viewSize = _view.getSize();
    const sf::Vector2f barOutterSize{100.f, 5.f};
    const sf::Vector2f barInnerSize{barOutterSize.x * (enemiesAlive / totalEnemies), barOutterSize.y};
    const sf::Vector2i padding{2 + lineSize, 2 + lineSize};
    const sf::Vector2f barPosition{viewCenter.x - padding.x - barOutterSize.x / 2, viewCenter.y + padding.y + barOutterSize.y - viewSize.y / 2};

    // This is the outside grey/black rectangle.
    sf::RectangleShape outsideRect(barOutterSize);
    outsideRect.setPosition(barPosition);
    outsideRect.setFillColor(sf::Color(45, 45, 45, 255));
    outsideRect.setOutlineColor(sf::Color::Black);
    outsideRect.setOutlineThickness(lineSize);
    _gameWindow.draw(outsideRect);

    // This is the inside purple rectangle.
    sf::RectangleShape insideRect(barInnerSize);
    insideRect.setPosition(barPosition);
    insideRect.setFillColor(sf::Color(128, 0, 187, 255));
    _gameWindow.draw(insideRect);

    sf::Text text;
    sf::Font font;

    if (!font.loadFromFile("fonts/Helvetica.ttf"))
    {
        printf("ERROR: font can not be loaded!!");
    }

    // Current wave number text
    text.setFont(font);
    text.setString(std::to_string(currWave));
    text.setCharacterSize(lineSize * 2 + barOutterSize.y);
    text.setFillColor(sf::Color::White);
    text.setOutlineColor(sf::Color::Black);
    text.setOutlineThickness(1);

    text.setPosition(sf::Vector2f{barPosition.x - padding.x - (text.getGlobalBounds().left + text.getGlobalBounds().width), barPosition.y + lineSize - (text.getGlobalBounds().top + text.getGlobalBounds().height) / 2});
    _gameWindow.draw(text);
}

void GameManager::captureCurrentFrame()
{
    // Grab the current buffer into a texture real quick
    frozenWindow.update(_gameWindow);
}
