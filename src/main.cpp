#include <SFML/Graphics.hpp>

#include "stdafx.h"
#include "Renderer.h"

int main()
{
    auto window = sf::RenderWindow(sf::VideoMode({GRID_WIDTH, GRID_HEIGHT}), "CMake SFML Project");
    window.setFramerateLimit(6000);

    Renderer renderer;
    
    sf::Clock clock;

    while (window.isOpen())
    {
        // Calculate delta time
        float dt = clock.restart().asSeconds();

        while (const std::optional event = window.pollEvent())
        {
            if (event->is<sf::Event::Closed>())
            {
                window.close();
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape))
        {
            window.close();
        }
        sf::CircleShape cursor(20.f);
        cursor.setFillColor(sf::Color::Transparent);
        cursor.setOutlineColor(sf::Color::White);
        cursor.setOutlineThickness(2.f);
        sf::Vector2 mousePos = sf::Mouse::getPosition(window);
        // Center the circle on the mouse by offsetting by -radius
        cursor.setPosition({static_cast<float>(mousePos.x) - 20.f, static_cast<float>(mousePos.y) - 20.f});

        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Space))
        {
            sf::Vector2 mousePos = sf::Mouse::getPosition(window);
            // Spawn sand particles within the circle
            for (int i = 0; i < 30; ++i)
            {
                // Random angle and distance within circle
                float angle = (rand() % 360) * 3.14159f / 180.f;
                float distance = (rand() % 20);  // Within radius 20
                int spawnX = mousePos.x + static_cast<int>(distance * cos(angle));
                int spawnY = mousePos.y + static_cast<int>(distance * sin(angle));
                
                if (spawnX >= 0 && spawnX < GRID_WIDTH && spawnY >= 0 && spawnY < GRID_HEIGHT)
                {
                    Particle& p = renderer.get_p(spawnX, spawnY);
                    p = Particle(MAT_ID_SAND, 0.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::X))
        {
            sf::Vector2 mousePos = sf::Mouse::getPosition(window);
            // Spawn water particles within the circle
            for (int i = 0; i < 30; ++i)
            {
                // Random angle and distance within circle
                float angle = (rand() % 360) * 3.14159f / 180.f;
                float distance = (rand() % 20);  // Within radius 20
                int spawnX = mousePos.x + static_cast<int>(distance * cos(angle));
                int spawnY = mousePos.y + static_cast<int>(distance * sin(angle));
                
                if (spawnX >= 0 && spawnX < GRID_WIDTH && spawnY >= 0 && spawnY < GRID_HEIGHT)
                {
                    Particle& p = renderer.get_p(spawnX, spawnY);
                    p = Particle(MAT_ID_WATER, 0.f, sf::Vector2f(0.f, 5.f), sf::Color::Cyan);
                }
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::C))
        {
            sf::Vector2 mousePos = sf::Mouse::getPosition(window);
            for (int i = 0; i < 20; ++i)
            {
                if (mousePos.x >= 0 && mousePos.x < GRID_WIDTH && mousePos.y >= 0 && mousePos.y < GRID_HEIGHT)
                {
                    Particle& p = renderer.get_p(mousePos.x, mousePos.y);
                    p = Particle(MAT_ID_WOOD, 5.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                    Particle& p2 = renderer.get_p(mousePos.x, mousePos.y + 1);
                    p2 = Particle(MAT_ID_WOOD, 5.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                    Particle& p3 = renderer.get_p(mousePos.x, mousePos.y + 2);
                    p3 = Particle(MAT_ID_WOOD, 5.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                    Particle& p4 = renderer.get_p(mousePos.x, mousePos.y + 3);
                    p4 = Particle(MAT_ID_WOOD, 5.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                    Particle& p5 = renderer.get_p(mousePos.x, mousePos.y + 4);
                    p5 = Particle(MAT_ID_WOOD, 5.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                    Particle& p6 = renderer.get_p(mousePos.x, mousePos.y + 5);
                    p6 = Particle(MAT_ID_WOOD, 5.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                }
                mousePos.x += 1;
            }
        }
        if (sf::Keyboard::isKeyPressed(sf::Keyboard::Key::V))
        {
            sf::Vector2 mousePos = sf::Mouse::getPosition(window);
            for (int i = 0; i < 5; ++i)
            {
                if (mousePos.x >= 0 && mousePos.x < GRID_WIDTH && mousePos.y >= 0 && mousePos.y < GRID_HEIGHT)
                {
                    Particle& p = renderer.get_p(mousePos.x, mousePos.y);
                    p = Particle(MAT_ID_FIRE, 0.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                    Particle& p2 = renderer.get_p(mousePos.x, mousePos.y + 1);
                    p2 = Particle(MAT_ID_FIRE, 0.f, sf::Vector2f(0.f, 5.f), sf::Color::Yellow);
                }
                mousePos.x += 3;
            }
        }

        window.clear(sf::Color(50, 50, 50)); //gray color

        renderer.update(dt);
        renderer.render(window);
        window.draw(cursor);

        window.display();
    }
}
