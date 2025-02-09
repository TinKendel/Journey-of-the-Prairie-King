#pragma once

#include <iostream>
#include <random>

#include "SFML/Graphics.hpp"
#include "MapLoader.hpp"
#include "Enemy.hpp"
#include "EnemyType.hpp"
#include "Player.hpp"
#include "Bullet.hpp"

class EnemyManager
{
private:
	std::vector<MapLoader::SpawnPoint> spawnPoints;

	sf::Texture orc_texture,
				spikeball_texture,
				ogre_texture,
				mushroom_texture,
				evil_butterfly_texture,
				mummy_texture,
				imp_texture,
				death_texture1,
				death_texture2;

	std::vector<Enemy> enemies;


	sf::Clock timer;
	float elapsed_time;
	float last_spawn_time;


public:
	void setSpawnPoints(const MapLoader& map);
	
	void loadEnemyTextures();
	void spawnEnemies(const MapLoader& map, int& area);
	void draw(sf::RenderWindow& window);

	void updateEnemyPosition(const sf::Vector2f& target, float delta_time, const MapLoader& map);

	void checkEnemyToEnemyCollision(const MapLoader& map);
	void resolveEnemyCollision(Enemy& enemy1, Enemy& enemy2, const MapLoader& map);
	
	void checkEnemyToPlayerCollision(Player& player);
	void checkEnemyToBulletCollision(Player& player);

	int getNumberOfEnemies() { return enemies.size(); }

	void restartEnemyManagerTimer() { timer.restart(); elapsed_time = 0; last_spawn_time = 0.f; }
};