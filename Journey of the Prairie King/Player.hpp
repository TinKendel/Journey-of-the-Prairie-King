#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include "Constants.hpp"
#include "MapLoader.hpp"
#include "Bullet.hpp"

/**
 * @class Player
 * @brief Handles loading, setting, tracking input, shooting and drawing sprites.
 *
 * This class is responsible for tracking the input from the keyboard, animating the player and
 * give him the ability to shoot.
 */
class Player
{
private:
	sf::Texture player_texture;		///< Player texture for the playable character
	sf::Sprite player_sprite;		///< Player sprite 
	sf::Sprite leg_sprite;			///< Leg Sprite 

	sf::Vector2f spawn_position = { GameConfig::WINDOW_WIDTH / 2, GameConfig::WINDOW_HEIGHT / 2 };

	const std::vector<sf::IntRect> leg_frames = { // Each element inside the vector holds a different peace of the PNG
		sf::IntRect(16, 16, 16, 3), // Frame 1
		sf::IntRect(16, 20, 16, 3), // Frame 2
		sf::IntRect(16, 25, 16, 3), // Frame 3
		sf::IntRect(16, 29, 16, 3)  // Frame 4
	};

	sf::Clock leg_clock;			///< Leg clock for the walking animations
	int current_frame = 0;			
	int frame_duration = 150;

	float speed = 1.3f;				///< Speed for player movement speed
	sf::Vector2f movement;			///< Movement for player velocity

	std::vector<Bullet> bullets;	///< bullets for keeping track of none destroyed bullets
	sf::Clock shoot_clock;			///< Shoot_clock for making the shooting consistent
	float shoot_cooldown = 0.3f;	///< Shoot_cooldown for defining the the time between shots
	sf::Vector2f direction;
	bool shooting;

	sf::Texture bullet_texture;		///< Bullet_texture for texture

	int area = 1;
public:
	/**
	 * @brief Load the player texture.
	 * @param filePath path to the player image file.
	 */
	void loadPlayerTexture(const std::string& filePath);

	/**
	 * @brief Set the player sprite and his legs.
	 */
	void setPlayerSprites();

	/**
	* @brief Handles the input from the user (W, A, S, D) while giving the player velocity and animates the legs.
	*/
	void handleInputs();

	/**
	* @brief Moves the player, checks for collision and enables "gliding" over walls
	* @param map instance of the background "map"
	*/
	void playerMovement(MapLoader& map);

	/**
	* @brief Loads the bullet texture.
	* @param filePath path to the bullet image file.
	*/
	void loadBulletTexture(const std::string& filePath);

	/**
	* @brief Gives the player the ability to shoot in 8 directions by capturing arrow input.
	*/
	void shoot();

	/**
	* @brief Moves (Shoots) the bullet in a direction while checking if it hits a wall.
	* @param deltaTime for a constant bullet velocity.
	* @param map instance of the background "map".
	*/
	void updateBullets(float deltaTime, MapLoader map);

	/**
	 * @brief Renders the bullets.
	 * @param window reference to the actual window.
	 */
	void drawBullets(sf::RenderWindow& window);

	/**
	* @brief Gets players sprite.
	*/
	sf::Sprite getPlayer();

	/**
	* @brief Gets players legs sprite.
	*/
	sf::Sprite getLegs();

	/**
	* @brief Gets players movement
	*/
	sf::Vector2f getMovement();

	sf::Vector2f getPlayerPosition() const { return player_sprite.getPosition(); }

	std::vector<Bullet>& getBullets() { return bullets; }

	sf::FloatRect getPlayerBounds() { return player_sprite.getGlobalBounds(); }

	void playerDeath();

	void movePlayerToNextArea() { player_sprite.setPosition(spawn_position.x, spawn_position.y + GameConfig::WINDOW_HEIGHT * area++); }
};