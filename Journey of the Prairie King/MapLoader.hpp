#pragma once

#include <iostream>
#include <vector>
#include <string>                // Include for std::string
#include "tinyxml2.h"            // For parsing TMX files
#include "Constants.hpp"         
#include "SFML/Graphics.hpp"     // For SFML rendering and transformations
#include "Bullet.hpp"

/**
 * @class MapLoader
 * @brief Handles loading, rendering, and managing maps, including tileset and collision data.
 *
 * This class is responsible for parsing TMX map files, rendering tile layers, managing animated walls,
 * and handling collision data.
 */
class MapLoader : public sf::Transformable, public sf::Drawable
{
public:
    struct SpawnPoint
    {
        int stage;
        int area;
        int spawn_id;
        sf::Vector2f move_towards;
        sf::FloatRect rect;
    };
    /**
     * @brief Loads the map, including tile layers and the tileset image.
     * @param tmxFile path to the TMX map file.
     * @param tilesetImage path to the tileset image file.
     * @param tileSize simensions of each tile (in pixels).
     * @param width width of the map in tiles.
     * @param height height of the map in tiles.
     * @return True if the map loaded successfully, false otherwise.
     */
    bool load(const std::string& tmxFile, const std::string& tilesetImage, sf::Vector2u tileSize, unsigned int width, unsigned int height);

    /**
     * @brief Toggles the visibility of map layers to simulate animations (e.g., animated walls).
     */
    void toggleLayerVisibility();

    /**
     * @brief Updates wall animation based on the elapsed time.
     */
    void wallAnimation();

    /**
     * @brief Loads collision data from the TMX file.
     * @param tmxFile path to the TMX map file.
     * @return True if collision data was loaded successfully, false otherwise.
     */
    bool loadCollision(const std::string& tmxFile);

    /**
     * @brief Checks if a sprites bounding box collides with any collision object.
     * @param sprite the bounding box of the player/enemy.
     * @return True if a collision is detected, false otherwise.
     */
    bool checkCollision(const sf::Sprite& sprite) const;

    /**
     * @brief Checks if a bullet bounding box collides with any collision object.
     * @param bullet the bounding box of the bullet.
     * @return True if a collision is detected, false otherwise.
     */
    bool checkBulletCollision(const Bullet& bullet) const;


    /**
     * @brief Provides access to all collision objects.
     * @return A constant reference to the collision objects vector.
     */
    //const std::vector<CollisionObject>& getCollisionObjects() const { return m_collisionObjects; }

    const std::vector<SpawnPoint>& getSpawnPoints() const { return m_enemySpawnPoints; }

private:
    /**
     * @struct CollisionObject
     * @brief Represents a single collision object with associated metadata.
     */
    struct CollisionObject
    {
        sf::FloatRect rect;       ///< Rectangle representing the collision area
        std::string type;         ///< Type of the object (e.g., "wall", "entrance")
        bool nextArea = false;    ///< Custom property for entrance objects, determines area transitions
        bool hasCollision = false;
    };

    sf::Texture m_tileset;                      ///< Tileset texture for the map
    std::vector<sf::VertexArray> m_layers;      ///< Vertex arrays for rendering map layers
    std::vector<bool> m_layerVisibility;        ///< Visibility toggles for layers (e.g., for animations)
    sf::Clock wall_clock;                        ///< Timer for wall animations

    std::vector<CollisionObject> m_collisionObjects;   ///< Collision objects parsed from the map
    std::vector<sf::RectangleShape> m_collisionShapes; ///< Shapes for debugging/rendering collision areas

    std::vector<SpawnPoint> m_enemySpawnPoints;
    std::vector<sf::RectangleShape> m_enemySpawnPointsShape;

    /**
     * @brief Parses a CSV string from the TMX file to extract tile data.
     * @param csvData the CSV string to parse.
     * @return A vector of integers representing tile IDs.
     */
    std::vector<int> parseCSV(const std::string& csvData);

    /**
     * @brief Renders the map and optional debugging elements.
     * @param target the SFML render target.
     * @param states render states for transformations and blending.
     */
    void draw(sf::RenderTarget& target, sf::RenderStates states) const;

};
