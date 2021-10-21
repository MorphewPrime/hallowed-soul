#include <SFML/Graphics.hpp>
#include "WaveManager.h"
using namespace std;

int main()
{
    sf::RenderWindow window(sf::VideoMode(200, 200), "SFML works!");
    sf::CircleShape shape(100.f);
    shape.setFillColor(sf::Color::Green);

    sf::Clock clock;
    sf::Texture texture;
    texture.loadFromFile("src/Images/sprite_test.png");

    sf::IntRect rectSourceSprite(0, 488, 90, 98);
    sf::Sprite sprite(texture, rectSourceSprite);


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        bool rowCount = 1;
        if(clock.getElapsedTime().asSeconds() > 0.1f)
        {
            if (rectSourceSprite.left == 810)
            {
                rectSourceSprite.left = 0;
                rowCount++;
            } else {
                rectSourceSprite.left += 90;
            }

            // rectSourceSprite.top = ((rowCount % 4)) * 98;

            sprite.setTextureRect(rectSourceSprite);
            clock.restart();

        }

        window.clear();
        window.draw(shape);
        window.draw(sprite);
        window.display();
    }

    return 0;
}
