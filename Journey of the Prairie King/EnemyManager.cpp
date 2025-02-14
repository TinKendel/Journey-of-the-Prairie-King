#include "EnemyManager.hpp"

std::map<int, std::map<int, std::vector<EnemyType>>> EnemyManager::enemy_spawn_table = {
	{1, { // Stage 1
		{1, {EnemyType::Orc}},
		{2, {EnemyType::Orc, EnemyType::Spikeball}},
		{3, {EnemyType::Orc, EnemyType::Spikeball, EnemyType::Ogre}},
		{4, {EnemyType::Orc, EnemyType::Spikeball, EnemyType::Ogre}}
}},
	{2, { // Stage 2
		{1, {EnemyType::Ogre, EnemyType::Mushroom, EnemyType::Evil_butterfly}},
		{2, {EnemyType::Ogre, EnemyType::Mushroom, EnemyType::Evil_butterfly}},
		{3, {EnemyType::Ogre, EnemyType::Mushroom, EnemyType::Evil_butterfly}}

}},
	{3, { // Stage 3
		{1, {EnemyType::Mummy, EnemyType::Imp}},
		{2, {EnemyType::Mummy, EnemyType::Imp}},
		{3, {EnemyType::Mummy, EnemyType::Imp}}
}}
};

void EnemyManager::setSpawnPoints(const MapLoader& map)
{
	spawnPoints = map.getSpawnPoints();
}

void EnemyManager::loadEnemyTextures()
{
	if (!orc_texture.loadFromFile("assets\\tiledPNG\\enemies\\orc.png")) 
	{
		std::cerr << "Failed to load Orc texture!" << std::endl;
	}

	if (!spikeball_texture.loadFromFile("assets\\tiledPNG\\enemies\\spikeball.png")) 
	{
		std::cerr << "Failed to load Spikeball texture!" << std::endl;
	}

	if (!ogre_texture.loadFromFile("assets\\tiledPNG\\enemies\\ogre.png")) 
	{
		std::cerr << "Failed to load Ogre texture!" << std::endl;
	}

	if (!mushroom_texture.loadFromFile("assets\\tiledPNG\\enemies\\mushroom.png")) 
	{
		std::cerr << "Failed to load Mushroom texture!" << std::endl;
	}

	if (!evil_butterfly_texture.loadFromFile("assets\\tiledPNG\\enemies\\evil_butterfly.png")) 
	{
		std::cerr << "Failed to load Evil Butterfly texture!" << std::endl;
	}

	if (!mummy_texture.loadFromFile("assets\\tiledPNG\\enemies\\mummy.png")) 
	{
		std::cerr << "Failed to load Mummy texture!" << std::endl;
	}

	if (!imp_texture.loadFromFile("assets\\tiledPNG\\enemies\\imp.png")) 
	{
		std::cerr << "Failed to load Imp texture!" << std::endl;
	}

	if (!death_texture1.loadFromFile("assets\\tiledPNG\\enemies\\death_animation1.png")) 
	{
		std::cerr << "Failed to load death texture number 1!" << std::endl;
	}
	if (!death_texture2.loadFromFile("assets\\tiledPNG\\enemies\\death_animation2.png")) 
	{
		std::cerr << "Failed to load death texture number 2!" << std::endl;
	}
}

void EnemyManager::spawnEnemies(const MapLoader& map, int stage, int area)
{
	if (timer.getElapsedTime().asSeconds() >= 60) {
		return;
	}

	static std::random_device rd;
	static std::mt19937 rng(rd());

	elapsed_time = timer.getElapsedTime().asSeconds();

	if (elapsed_time < 3) { // No enemy should spawn until then
		return;
	}

	if (elapsed_time - last_spawn_time >= 2)
	{
		last_spawn_time = elapsed_time;

		// Filter valid spawn points
		std::vector<MapLoader::SpawnPoint> validSpawns;
		for (const auto& sp : spawnPoints)
		{
			if (sp.area == area)
				validSpawns.push_back(sp);
		}

		// Ensure we have valid spawn points and enemy data
		if (validSpawns.empty() || enemy_spawn_table.count(stage) == 0 || enemy_spawn_table[stage].count(area) == 0)
			return;

		const std::vector<EnemyType>& possible_enemies = enemy_spawn_table[stage][area];

		// Dynamically adjust spawn distributions
		std::uniform_int_distribution<int> spawnPointCount(1, 8);
		std::uniform_int_distribution<int> enemyTypeDist(0, possible_enemies.size() - 1);

		int num_spawns = spawnPointCount(rng);
		for (int j = 0; j < num_spawns; j++)
		{
			if (validSpawns.empty()) break; // Avoid out-of-bounds errors

			// Ensure spawn index is within bounds
			std::uniform_int_distribution<int> spawnIndexDist(0, validSpawns.size() - 1);
			int random_index = spawnIndexDist(rng);
			auto& sp = validSpawns[random_index];

			EnemyType chosenEnemy = possible_enemies[enemyTypeDist(rng)];

			// Spawn the enemy
			Enemy enemy(chosenEnemy, getEnemyTexture(chosenEnemy), death_texture1);
			enemy.setPosition(sp.rect.left, sp.rect.top);
			enemy.setSpawnPoint({ sp.rect.left, sp.rect.top });
			enemy.setMoveDirection({ sp.move_towards });
			enemies.push_back(enemy);
		}
	}
}



void EnemyManager::draw(sf::RenderWindow& window) 
{
	for (auto& enemy : enemies) 
	{
		enemy.draw(window);
	}
}

void EnemyManager::updateEnemyPosition(const sf::Vector2f& target, float delta_time, const MapLoader& map)
{
	for (auto& enemy : enemies)
	{
		if (enemy.getEnemySate() == EnemyState::Dying)
		{
			continue;
		}

		enemy.updatePosition(target, delta_time, map);
		enemy.updateEnemyAnimation();
	}
}

void EnemyManager::resolveDyingEnemies()
{
	for (auto enemy = enemies.begin(); enemy != enemies.end();)
	{
		if (enemy->getEnemySate() == EnemyState::Dying)
		{
			if (enemy->deathAnimation()) // Returns true when animation is done
			{
				enemy = enemies.erase(enemy); // Now remove it
			}
			else
			{
				++enemy;
			}
		}
		else
		{
			++enemy;
		}
	}
}


void EnemyManager::checkEnemyToEnemyCollision(const MapLoader& map)
{
	for (int i = 0; i < enemies.size(); ++i)
	{
		if (enemies[i].getEnemySate() == EnemyState::Dying)
		{
			continue;
		}

		for (int j = i + 1; j < enemies.size(); ++j)
		{
			if (enemies[j].getEnemySate() == EnemyState::Dying)
			{
				continue;
			}

			if (enemies[i].getEnemyGlobalBounds().intersects(enemies[j].getEnemyGlobalBounds()))
			{
				resolveEnemyCollision(enemies[i], enemies[j], map);
			}
		}
	}
}

void EnemyManager::resolveEnemyCollision(Enemy& enemy1, Enemy& enemy2, const MapLoader& map)
{
	sf::Vector2f pos1 = enemy1.getEnemyPosition();
	sf::Vector2f pos2 = enemy2.getEnemyPosition();

	sf::Vector2f diff = pos1 - pos2;
	float distance = std::sqrt(diff.x * diff.x + diff.y * diff.y);

	if (distance == 0)
	{
		// Avoid division by zero by giving a small nudge
		diff = { 1.f, 0.f };
		distance = 1.f;
	}

	// Normalize the direction vector
	sf::Vector2f pushDirection = diff / distance;

	// Push each enemy away from the other
	float pushAmount = 1.0f; // Adjust this value based on how strong the push should be
	sf::Sprite temp_enemy_1 = enemy1.getEnemySprite();
	sf::Sprite temp_enemy_2 = enemy2.getEnemySprite();
	temp_enemy_1.setPosition(pos1.x + pushDirection.x * pushAmount, pos1.y + pushDirection.y * pushAmount);
	temp_enemy_2.setPosition(pos2.x - pushDirection.x * pushAmount, pos2.y - pushDirection.y * pushAmount);

	if (!map.checkCollision(temp_enemy_1) && !map.checkCollision(temp_enemy_2))
	{
		enemy1.setPosition(pos1.x + pushDirection.x * pushAmount, pos1.y + pushDirection.y * pushAmount);
		enemy2.setPosition(pos2.x - pushDirection.x * pushAmount, pos2.y - pushDirection.y * pushAmount);
	}
}

void EnemyManager::checkEnemyToPlayerCollision(Player& player)
{
	for (auto& enemy : enemies)
	{
		if (enemy.getEnemyGlobalBounds().intersects(player.getPlayerBounds()))
		{
			player.playerDeath();
		}
	}
}

void EnemyManager::checkEnemyToBulletCollision(Player& player)
{
	for (auto enemy = enemies.begin(); enemy != enemies.end();)
	{
		if (enemy->getEnemySate() == EnemyState::Dying)
		{
			++enemy;
			continue;
		}

		for (auto bullet = player.getBullets().begin(); bullet != player.getBullets().end();)
		{
			if (bullet->getBounds().intersects(enemy->getEnemyGlobalBounds()))
			{
				bullet = player.getBullets().erase(bullet);
				enemy->isDying();

				//enemy = enemies.erase(enemy);
				break;
			}
			else
			{
				++bullet;
			}
		}

		if (enemy != enemies.end())
		{
			++enemy;
		}	
	}
}

sf::Texture& EnemyManager::getEnemyTexture(EnemyType type)
{
	switch (type)
	{
		case EnemyType::Orc:
		{
			return orc_texture;
		}
		case EnemyType::Spikeball:
		{
			return spikeball_texture;
		}
		case EnemyType::Ogre:
		{
			return ogre_texture;
		}
		case EnemyType::Mushroom:
		{
			return mushroom_texture;
		}
		case EnemyType::Evil_butterfly:
		{
			return evil_butterfly_texture;
		}
		case EnemyType::Mummy:
		{
			return mummy_texture;
		}
		case EnemyType::Imp:
		{
			return imp_texture;
		}
	}
}



