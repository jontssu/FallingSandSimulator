#include <SFML/Graphics.hpp>

#include "stdafx.h"
#include "Renderer.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({GRID_WIDTH, GRID_HEIGHT}), "CMake SFML Project");
    window.setFramerateLimit(6000);

    Renderer renderer;

    while (window.isOpen())
    {
        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
            {
                window.close();
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
            {
                sf::Vector2 mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 5; ++i)
                {
                    for (int x = 0; x < 5; ++x)
                    {
                        if (mousePos.x >= 0 && mousePos.x < GRID_WIDTH && mousePos.y >= 0 && mousePos.y < GRID_HEIGHT)
                        {
                            Particle& p = renderer.get_p(mousePos.x, mousePos.y);
                            p = Particle(MAT_ID_SAND, 0.f, sf::Vector2f(0.f, 0.f), sf::Color::Yellow);
                            mousePos.x += 2;
                        }
                    }
                    mousePos.y += 2;
                }
            }
            if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
            {
                sf::Vector2 mousePos = sf::Mouse::getPosition(window);
                for (int i = 0; i < 5; ++i)
                {
                    for (int x = 0; x < 5; ++x)
                    {
                        if (mousePos.x >= 0 && mousePos.x < GRID_WIDTH && mousePos.y >= 0 && mousePos.y < GRID_HEIGHT)
                        {
                            Particle& p = renderer.get_p(mousePos.x, mousePos.y);
                            p = Particle(MAT_ID_WATER, 0.f, sf::Vector2f(0.f, 0.f), sf::Color::Yellow);
                            mousePos.x += 5;
                        }
                    }
                    mousePos.y += 10;
                }
            }
        }

        window.clear(sf::Color::Black);


        Particle& p = renderer.get_p(window.getSize().x / 2, window.getSize().y / 2);
        p = Particle(MAT_ID_SAND, 0.f, sf::Vector2f(0.f, 0.f), sf::Color::Blue);
        Particle& p2 = renderer.get_p((window.getSize().x / 2) + 15, (window.getSize().y / 2) + 15);
        p2 = Particle(MAT_ID_WATER, 0.f, sf::Vector2f(0.f, 0.f), sf::Color::Blue);
        renderer.update();
        renderer.render(window);

        window.display();
    }
}
