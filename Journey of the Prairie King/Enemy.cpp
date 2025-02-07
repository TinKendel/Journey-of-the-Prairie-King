#include "Enemy.hpp"

Enemy::Enemy(EnemyType type, const sf::Texture& texture, const sf::Texture& death_texture)
	: type(type), has_left_spawn(false)
{
	switch (type)
	{
		case EnemyType::Orc:
		{
			enemy_sprite.setTexture(texture);
			death_sprite1.setTexture(death_texture);
			enemy_sprite.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);
			enemy_sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
			has_left_spawn = false;
			hp = 1;
			speed = 1;
			can_fly = false;
			break;
		}
		case EnemyType::Spikeball:
		{
			break;
		}
		case EnemyType::Ogre:
		{
			break;
		}
		case EnemyType::Mushroom:
		{
			break;
		}
		case EnemyType::Evil_butterfly:
		{
			break;
		}
		case EnemyType::Mummy:
		{
			break;
		}
		case EnemyType::Imp:
		{
			break;
		}
	}
}

void Enemy::draw(sf::RenderWindow& window)
{
	window.draw(enemy_sprite);
}

void Enemy::updatePosition(const sf::Vector2f& target, float delta_time, const MapLoader& map)
{
	if (!has_left_spawn)
	{
		float magnitude = std::sqrt(move_direction.x * move_direction.x + move_direction.y * move_direction.y);
		if (magnitude != 0)
		{
			move_direction /= magnitude;
		}

		// Move the enemy in its spawn exit direction
		enemy_sprite.move(move_direction * speed * delta_time);

		// Check if the enemy has moved at least 16 pixels from its spawn
		if (std::abs(enemy_sprite.getPosition().x - spawn_point.x) >= 32 || std::abs(enemy_sprite.getPosition().y - spawn_point.y) >= 32)
		{
			has_left_spawn = true;  // Now the enemy starts chasing the player
		}
		return;  // Exit function, enemy only moves away from spawn first
	}

	sf::Vector2f direction = target - enemy_sprite.getPosition();

	float magnitude = std::sqrt(direction.x * direction.x + direction.y * direction.y);
	if (magnitude != 0)
	{
		direction /= magnitude;
	}

	sf::Sprite temp_sprite = enemy_sprite;
	sf::Vector2f new_position = enemy_sprite.getPosition() + (direction * speed * delta_time);
	temp_sprite.setPosition(new_position);

	if (!map.checkCollision(temp_sprite))
	{
		enemy_sprite.setPosition(new_position);
	}
	
}

void Enemy::updateEnemyAnimation()
{
	if (animation_timer.getElapsedTime().asSeconds() > 0.25 && animation_timer.getElapsedTime().asSeconds() < 0.5)
	{
		enemy_sprite.setTextureRect(sf::IntRect(16, 0, 16, 16));
	}
	else if (animation_timer.getElapsedTime().asSeconds() < 0.25 && animation_timer.getElapsedTime().asSeconds() > 0)
	{
		enemy_sprite.setTextureRect(sf::IntRect(0, 0, 16, 16));
	}
	else
	{
		animation_timer.restart();
	}

}


void Enemy::deathAnimation(EnemyType type)
{
	switch (type)
	{
		case EnemyType::Orc:
		{
			if (death_animation_timer1.getElapsedTime().asSeconds() < 3)
			{
				return;
			}

			break;
		}
		case EnemyType::Spikeball:
		{
			break;
		}
		case EnemyType::Ogre:
		{
			break;
		}
		case EnemyType::Mushroom:
		{
			break;
		}
		case EnemyType::Evil_butterfly:
		{
			break;
		}
		case EnemyType::Mummy:
		{
			break;
		}
		case EnemyType::Imp:
		{
			break;
		}
	}
}

void Enemy::checkCollision(const MapLoader& map)
{
	if (can_fly == false)
	{
		if (map.checkCollision(enemy_sprite))
		{
			std::cout << "After hitting a wall" << move_direction.x << " - " << move_direction.y << "\n";
			enemy_sprite.setPosition(enemy_sprite.getPosition() - move_direction);
		}
	}	
}