#pragma once

#include <iostream>
#include "Constants.hpp"
#include "EnemyType.hpp"
#include "SFML/Graphics.hpp"
#include "MapLoader.hpp"

class Enemy
{
private:
	sf::Sprite enemy_sprite;
	sf::Texture death_texture1;
	sf::Sprite death_sprite1;

	int hp;
	float speed;

	EnemyType type;
	bool can_fly;
	bool has_left_spawn;

	sf::Vector2f spawn_point;
	sf::Vector2f move_direction;

	int current_frame;
	int frame_duration = 70;

	sf::Clock animation_timer,
			  death_animation_timer1;

	std::vector<sf::IntRect> death_frames1 = { 
		sf::IntRect(0, 0, 16, 16), 
		sf::IntRect(16, 0, 16, 16), 
		sf::IntRect(32, 0, 16, 16),
		sf::IntRect(48, 0, 16, 16),  
		sf::IntRect(64, 0, 16, 16), 
		sf::IntRect(80, 0, 16, 16)  
	};

	//const std::vector<sf::IntRect> death_frames2 = {
	//	sf::IntRect(0, 0, 16, 16), 
	//	sf::IntRect(16, 0, 16, 16), 
	//	sf::IntRect(32, 0, 16, 16), 
	//	sf::IntRect(48, 0, 16, 16),  
	//	sf::IntRect(62, 0, 16, 16)  
	//};

	EnemyState state = EnemyState::Alive;

public:
	Enemy(EnemyType type, const sf::Texture& texture, const sf::Texture& death_texture);

	void draw(sf::RenderWindow& window);

	void setPosition(float x, float y) { enemy_sprite.setPosition(x, y); }

	void updatePosition(const sf::Vector2f& target, float delta_time, const MapLoader& map);

	void updateEnemyAnimation();

	void setSpeed(float x) { speed = x; }

	void setSpawnPoint(const sf::Vector2f& point) { spawn_point = point; }

	void setMoveDirection(const sf::Vector2f& direction) { move_direction = direction; }


	void isDying();
	bool deathAnimation();
	EnemyState getEnemySate() { return state; }



	sf::FloatRect getEnemyGlobalBounds() { return enemy_sprite.getGlobalBounds(); }

	sf::Vector2f getEnemyPosition() { return enemy_sprite.getPosition(); }

	void checkCollision(const MapLoader& map);

	EnemyType getEnemyType() { return type; }

	sf::Sprite getEnemySprite() { return enemy_sprite; }
};