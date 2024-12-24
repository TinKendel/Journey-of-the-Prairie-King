#pragma once

#include <iostream>
#include <vector>
#include <string>                // Include for std::string
#include "tinyxml2.h"            // For parsing TMX files
#include "Constants.hpp"         
#include "SFML/Graphics.hpp"     // For SFML rendering and transformations

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
    /**
     * @struct CollisionObject
     * @brief Represents a single collision object with associated metadata.
     */
    struct CollisionObject
    {
        sf::FloatRect rect;       ///< Rectangle representing the collision area
        std::string type;         ///< Type of the object (e.g., "wall", "entrance")
        bool nextArea = false;    ///< Custom property for entrance objects, determines area transitions
    };

private:
    sf::Texture m_tileset;                      ///< Tileset texture for the map
    std::vector<sf::VertexArray> m_layers;      ///< Vertex arrays for rendering map layers
    std::vector<bool> m_layerVisibility;        ///< Visibility toggles for layers (e.g., for animations)
    sf::Clock wallClock;                        ///< Timer for wall animations

    std::vector<CollisionObject> m_collisionObjects;   ///< Collision objects parsed from the map
    std::vector<sf::RectangleShape> m_collisionShapes; ///< Shapes for debugging/rendering collision areas

    /**
     * @brief Parses a CSV string from the TMX file to extract tile data.
     * @param csvData The CSV string to parse.
     * @return A vector of integers representing tile IDs.
     */
    std::vector<int> parseCSV(const std::string& csvData);

    /**
     * @brief Renders the map and optional debugging elements.
     * @param target The SFML render target.
     * @param states Render states for transformations and blending.
     */
    virtual void draw(sf::RenderTarget& target, sf::RenderStates states) const override;

public:
    /**
     * @brief Loads the map, including tile layers and the tileset image.
     * @param tmxFile Path to the TMX map file.
     * @param tilesetImage Path to the tileset image file.
     * @param tileSize Dimensions of each tile (in pixels).
     * @param width Width of the map in tiles.
     * @param height Height of the map in tiles.
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
     * @param tmxFile Path to the TMX map file.
     * @return True if collision data was loaded successfully, false otherwise.
     */
    bool loadCollision(const std::string& tmxFile);

    /**
     * @brief Checks if a player's bounding box collides with any collision object.
     * @param playerBounds The bounding box of the player.
     * @return True if a collision is detected, false otherwise.
     */
    bool checkCollision(const sf::FloatRect& playerBounds) const;

    /**
     * @brief Provides access to all collision objects.
     * @return A constant reference to the collision objects vector.
     */
    const std::vector<CollisionObject>& getCollisionObjects() const { return m_collisionObjects; }
};
