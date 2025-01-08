#pragma once

#include <iostream>
#include "SFML/Graphics.hpp"
#include "Constants.hpp"
#include "MapLoader.hpp"

class Player
{
private:
	sf::Texture player_texture;
	sf::Sprite player_sprite;
	sf::Sprite leg_sprite;

	const std::vector<sf::IntRect> leg_frames = {
		sf::IntRect(16, 16, 16, 3), // Frame 1
		sf::IntRect(16, 20, 16, 3), // Frame 2
		sf::IntRect(16, 25, 16, 3), // Frame 3
		sf::IntRect(16, 29, 16, 3)  // Frame 4
	};
	sf::Clock leg_clock;
	int current_frame = 0;
	int frame_duration = 150;

	float speed = 1.3f;
	sf::Vector2f new_position;
	sf::Vector2f movement;
public:

	void loadPlayerTexture(const std::string& filePath);
	void setPlayerSprites();
	void handleInputs();
	void playerMovement(MapLoader& map);


	sf::Sprite getPlayer();
	sf::Sprite getLegs();
	sf::Vector2f getMovement();
};