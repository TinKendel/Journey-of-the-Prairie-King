#pragma once

#include "Constants.hpp"
#include "EnemyType.hpp"
#include "SFML/Graphics.hpp"

class Enemy
{
private:
	sf::Sprite enemy_sprite;
	sf::Sprite death_sprite1;
	sf::Sprite deat_sprite2;
	int hp;
	float speed;
	sf::Vector2f position;
	EnemyType type;
	bool can_fly;
	bool has_left_spawn;

	sf::Vector2f spawn_point;
	sf::Vector2f move_direction;

	int current_frame = 0;
	int frame_duration = 150;

	sf::Clock animation_timer,
			  death_animation_timer1;

	//const std::vector<sf::IntRect> death_frames1 = { 
	//	sf::IntRect(0, 0, 16, 16), 
	//	sf::IntRect(16, 0, 16, 16), 
	//	sf::IntRect(32, 0, 16, 16),
	//	sf::IntRect(48, 0, 16, 16),  
	//	sf::IntRect(62, 0, 16, 16), 
	//	sf::IntRect(78, 0, 16, 16)  
	//};

	//const std::vector<sf::IntRect> death_frames2 = {
	//	sf::IntRect(0, 0, 16, 16), 
	//	sf::IntRect(16, 0, 16, 16), 
	//	sf::IntRect(32, 0, 16, 16), 
	//	sf::IntRect(48, 0, 16, 16),  
	//	sf::IntRect(62, 0, 16, 16)  
	//};

public:
	Enemy(EnemyType type, const sf::Texture& texture, const sf::Texture& death_texture);

	void draw(sf::RenderWindow& window);

	void setPosition(float x, float y) { enemy_sprite.setPosition(x, y); }

	void updatePosition(const sf::Vector2f& target, float delta_time);

	void updateEnemyAnimation();

	sf::FloatRect getEnemyGlobalBounds() { return enemy_sprite.getGlobalBounds(); }

	void setSpeed(float x) { speed = x; }

	void enemyKilled() { speed = 0; }

	void setSpawnPoint(const sf::Vector2f& point) { spawn_point = point; }

	void setMoveDirection(const sf::Vector2f& direction) { move_direction = direction; }

	//void deathAnimation(EnemyType type);

};