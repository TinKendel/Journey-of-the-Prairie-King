#pragma once

#include <SFML/Graphics.hpp>

/**
 * @class Bullet
 * @brief Handles bullets velocity and location.
 *
 * This class is responsible for loading bullet texture, starting position, direction, velocitiy and drawing the bullets on screen.
 */
class Bullet 
{
private:
    sf::Sprite bullet_sprite;   ///< Bullet sprite 
    sf::Vector2f velocity;      ///< Velocity
    float speed;                ///< Speed 

public:
    /**
    * @brief Parameterized Constructor for setting the bullt up.
    * @param texture of the bullet.
    * @param position starting position of the bullet.
    * @param direction direction in which the bullet will move to.
    * @param speed default argument for the speed at witch the bullet moves.
    */
    Bullet(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& direction, float speed = 2.f);

    /**
    * @brief Moves the bullet based on the inputs given when the object was created.
    * @param deltaTime deltaTime is for keeping the bullet speed constant even if frames go up or down.
    */
    void update(float deltaTime);

    /**
    * @brief Renders bullets.
    * @param window reference to the actual window.
    */
    void draw(sf::RenderWindow& window) const;

    /**
    * @brief Returns the rectangle of the bullet
    */
    sf::FloatRect getBounds() const;
};

