#include <SFML/Graphics.hpp>
#include "Constants.hpp"
#include "MapLoader.hpp"

int main() 
{
    // Create a window with the specified resolution and title
    sf::RenderWindow window(sf::VideoMode(GameConfig::WINDOW_WIDTH, GameConfig::WINDOW_HEIGHT), "THE GAME");
    window.setFramerateLimit(60); // Limit the framerate to 60 FPS for smoother gameplay

    // Create a MapLoader instance to manage map rendering and collisions
    MapLoader map;
    if (!map.load("assets\\tiledMap\\desert.tmx", "assets\\tiledPNG\\background.png", sf::Vector2u(TileConfig::TILE_WIDTH, TileConfig::TILE_HEIGHT), 20, 81)) 
    {
        // Exit the program if the map fails to load
        std::cerr << "Failed to load map!" << std::endl;
        return -1;
    }

    // Load collision data from the same map file
    if (!map.loadCollision("assets\\tiledMap\\desert.tmx")) {
        std::cerr << "Failed to load collision data!" << std::endl;
        return -1;
    }
    // Scale the map by a factor of 2x for rendering
    map.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);

    // Main game loop
    while (window.isOpen()) 
    {
        // Handle events
        sf::Event event;
        while (window.pollEvent(event)) 
        {
            if (event.type == sf::Event::Closed || sf::Keyboard::isKeyPressed(sf::Keyboard::Key::Escape)) 
            {
                // Close the window if the user presses Escape or clicks the close button
                window.close();
            }
        }

        // Update the map (e.g., animate walls or other elements)
        map.wallAnimation();

        // Render the scene
        window.clear();        // Clear the window to prepare for a new frame
        window.draw(map);      // Draw the map (includes collision overlays if enabled)
        window.display();      // Display the rendered frame
    }

    return 0;
}
