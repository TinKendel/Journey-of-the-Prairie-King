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

void Enemy::updatePosition(const sf::Vector2f& target, float delta_time)
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

	enemy_sprite.move(direction * speed * delta_time);
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


//void Enemy::deathAnimation(EnemyType type)
//{
//	switch (type)
//	{
//		case EnemyType::Orc:
//		{
//			break;
//		}
//		case EnemyType::Spikeball:
//		{
//			break;
//		}
//		case EnemyType::Ogre:
//		{
//			break;
//		}
//		case EnemyType::Mushroom:
//		{
//			break;
//		}
//		case EnemyType::Evil_butterfly:
//		{
//			break;
//		}
//		case EnemyType::Mummy:
//		{
//			break;
//		}
//		case EnemyType::Imp:
//		{
//			break;
//		}
//	}
//}