#include <SFML/Graphics.hpp>

#include "Constants.hpp"
#include "MapLoader.hpp"
#include "Player.hpp"
#include "HUD.hpp"
#include "EnemyManager.hpp"

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
    if (!map.loadCollision("assets\\tiledMap\\desert2.tmx")) {
        std::cerr << "Failed to load collision data!" << std::endl;
        return -1;
    }
    // Scale the map by a factor of 2x for rendering
    map.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);

    // Load player 
    Player player;
    player.loadPlayerTexture("assets\\tiledPNG\\kingSprite.png");
    player.loadBulletTexture("assets\\tiledPNG\\bullets\\defaultBullet.png");
    player.setPlayerSprites();

    // Load HUD 
    HUD hud;
    hud.loadHUDTextures();
    hud.loadFont();

    sf::Clock clock, area_clock;

    int area = 1; // This is a short time fix, later it needs to be connected with the in game clock (timer)
    EnemyManager enemy_manager;
    enemy_manager.setSpawnPoints(map);
    enemy_manager.loadEnemyTextures();
    //enemy_manager.spawnEnemies(map, area);


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

        float delta_time = clock.restart().asSeconds();
        float frame_rate = delta_time * 60.f;

        // Update the map (animate walls or other elements)
        map.wallAnimation();

        // Update player
        player.handleInputs();
        player.playerMovement(map);
        player.shoot();
        player.updateBullets(frame_rate, map);

        // Update enemies
        enemy_manager.spawnEnemies(map, area);
        enemy_manager.updateEnemyPosition(player.getPlayerPosition(), frame_rate);
        //enemy_manager.checkEnemyToEnemyCollision();
        enemy_manager.checkEnemyToPlayerCollision(player);
        enemy_manager.checkEnemyToBulletCollision(player);
        hud.startAreaTimer(area_clock);


        // Render the scene
        window.clear();        // Clear the window to prepare for a new frame
        window.draw(map);      // Draw the map (includes collision overlays if enabled)
        enemy_manager.draw(window);
        player.drawBullets(window);
        window.draw(player.getLegs());
        window.draw(player.getPlayer());
        hud.draw(window);
        window.display();      // Display the rendered frame
    }

    return 0;
}
