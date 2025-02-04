#include "EnemyManager.hpp"

void EnemyManager::setSpawnPoints(const MapLoader& map)
{
	spawnPoints = map.getSpawnPoints();
}

void EnemyManager::loadEnemyTextures()
{
	if (!orc_texture.loadFromFile("assets\\tiledPNG\\enemies\\orc.png")) {
		std::cerr << "Failed to load Orc texture!" << std::endl;
	}

	if (!spikeball_texture.loadFromFile("assets\\tiledPNG\\enemies\\spikeball.png")) {
		std::cerr << "Failed to load Spikeball texture!" << std::endl;
	}

	if (!ogre_texture.loadFromFile("assets\\tiledPNG\\enemies\\ogre.png")) {
		std::cerr << "Failed to load Ogre texture!" << std::endl;
	}

	if (!mushroom_texture.loadFromFile("assets\\tiledPNG\\enemies\\mushroom.png")) {
		std::cerr << "Failed to load Mushroom texture!" << std::endl;
	}

	if (!evil_butterfly_texture.loadFromFile("assets\\tiledPNG\\enemies\\evil_butterfly.png")) {
		std::cerr << "Failed to load Evil Butterfly texture!" << std::endl;
	}

	if (!mummy_texture.loadFromFile("assets\\tiledPNG\\enemies\\mummy.png")) {
		std::cerr << "Failed to load Mummy texture!" << std::endl;
	}

	if (!imp_texture.loadFromFile("assets\\tiledPNG\\enemies\\imp.png")) {
		std::cerr << "Failed to load Imp texture!" << std::endl;
	}

	if (!death_texture1.loadFromFile("assets\\tiledPNG\\enemies\\death_animation1.png")) {
		std::cerr << "Failed to load death texture number 1!" << std::endl;
	}
	if (!death_texture2.loadFromFile("assets\\tiledPNG\\enemies\\death_animation2.png")) {
		std::cerr << "Failed to load death texture number 2!" << std::endl;
	}
}

void EnemyManager::spawnEnemies(const MapLoader& map, int area)
{
    static std::random_device rd;
    static std::mt19937 rng(rd());

    //static std::uniform_int_distribution<int> dist(1, 8); // Random number of enemies that can spawn
    static std::uniform_int_distribution<int> spawnIndexDist(0, spawnPoints.size() - 1); // Random index for spawn points
    static std::uniform_int_distribution<int> spawnPointCount(1, 8); // Random number to determine number of spawn points that will be used

    static float last_spawn_time = 0.0f;
    float elapsed_time = timer.getElapsedTime().asSeconds();

    if (elapsed_time < 3) // No enemy should spawn until then
    {
        return;
    }

    if (elapsed_time - last_spawn_time >= 3)
    {
        last_spawn_time = elapsed_time;

        std::vector<MapLoader::SpawnPoint> validSpawns;
        for (const auto& sp : spawnPoints)
        {
            if (sp.area == area)
                validSpawns.push_back(sp);
        }

        if (!validSpawns.empty())
        {
            int num_spawns = spawnPointCount(rng); // Randomly choose how many spawn points to use
            
            for (int j = 0; j < num_spawns; j++)
            {
                int random_index = spawnIndexDist(rng) % validSpawns.size(); // To ensure the index does not go out of bounds
                auto& sp = validSpawns[random_index]; // Storing the radnom spawn point index

				// This will be commented since i need to figure out enemy collision between each other
                /*int random_spawn_count = dist(rng);

                for (int i = 0; i < random_spawn_count; i++)
                {*/
                    Enemy enemy(EnemyType::Orc, orc_texture, death_texture1);
                    enemy.setPosition(sp.rect.left, sp.rect.top);
                    enemy.setSpawnPoint(sf::Vector2f(sp.rect.left, sp.rect.top));
                    enemy.setMoveDirection(sf::Vector2f(sp.move_towards));
                    enemies.push_back(enemy);
                //}
            }
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

void EnemyManager::updateEnemyPosition(const sf::Vector2f& target, float delta_time)
{
	for (auto& enemy : enemies)
	{
		enemy.updatePosition(target, delta_time);
		enemy.updateEnemyAnimation();
	}
}

void EnemyManager::checkEnemyToEnemyCollision()
{
	for (int i = 0; i < enemies.size(); ++i)
	{
		for (int j = i + 1; j < enemies.size(); ++j)
		{
			if (enemies[i].getEnemyGlobalBounds().intersects(enemies[j].getEnemyGlobalBounds()))
			{
				resolveEnemyCollision(enemies[i], enemies[j]);
			}
		}
	}
}

void EnemyManager::resolveEnemyCollision(Enemy& enemy1, Enemy& enemy2)
{
	//enemy1.setSpeed();
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
		for (auto bullet = player.getBullets().begin(); bullet != player.getBullets().end();)
		{
			if (bullet->getBounds().intersects(enemy->getEnemyGlobalBounds()))
			{
				bullet = player.getBullets().erase(bullet);
				enemy = enemies.erase(enemy);
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



