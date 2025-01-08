#include "Player.hpp"

void Player::loadPlayerTexture(const std::string& filePath)
{
	if (!player_texture.loadFromFile(filePath))
	{
		std::cerr << "Failed to load player texture!" << std::endl;
	}
}
void Player::setPlayerSprites()
{
	player_sprite.setTexture(player_texture);
	player_sprite.setPosition(240.f, 240.f);
	player_sprite.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);

	leg_sprite.setTexture(player_texture);
    leg_sprite.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);
}

void Player::handleInputs()
{
    movement = { 0, 0 };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        player_sprite.setTextureRect(sf::IntRect(0, 0, TileConfig::TILE_WIDTH, TileConfig::TILE_HEIGHT));
        movement.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        player_sprite.setTextureRect(sf::IntRect(32, 0, TileConfig::TILE_WIDTH, TileConfig::TILE_HEIGHT));
        movement.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        player_sprite.setTextureRect(sf::IntRect(48, 0, TileConfig::TILE_WIDTH, TileConfig::TILE_HEIGHT));
        movement.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        player_sprite.setTextureRect(sf::IntRect(16, 0, TileConfig::TILE_WIDTH, TileConfig::TILE_HEIGHT));
        movement.x += speed;
    }

    if (movement != sf::Vector2f(0, 0))
    {
        if (leg_clock.getElapsedTime().asMilliseconds() >= frame_duration)
        {
            current_frame = (current_frame + 1) % leg_frames.size();
            leg_sprite.setTextureRect(leg_frames[current_frame]);
            leg_clock.restart();
        }
    }
    else
    {
        leg_sprite.setTextureRect(sf::IntRect(16, 20, 16, 3));
        player_sprite.setTextureRect(sf::IntRect(32, 16, 16, 16));
    }
}

void Player::playerMovement(MapLoader& map)
{
    // Calculate the new position based on player input
    sf::Vector2f new_position_x = player_sprite.getPosition() + sf::Vector2f(movement.x, 0.f); // Horizontal movement only
    sf::Vector2f new_position_y = player_sprite.getPosition() + sf::Vector2f(0.f, movement.y); // Vertical movement only

    // Create temporary sprites for testing collision
    sf::Sprite tempSpriteX = player_sprite; 
    sf::Sprite tempSpriteY = player_sprite;

    // Test horizontal and vertical movement separately
    tempSpriteX.setPosition(new_position_x); // Simulate horizontal movement
    tempSpriteY.setPosition(new_position_y); // Simulate vertical movement

    // Check collisions
    bool canMoveX = !map.checkCollision(tempSpriteX);
    bool canMoveY = !map.checkCollision(tempSpriteY);

    // Allow movement only in valid directions
    if (canMoveX)
    {
        player_sprite.setPosition(new_position_x); // Move horizontally if no collision
    }
    if (canMoveY)
    {
        player_sprite.setPosition(player_sprite.getPosition().x, new_position_y.y); // Move vertically if no collision
    }

    // Update leg sprite position relative to the player sprite
    leg_sprite.setPosition(player_sprite.getPosition().x, player_sprite.getPosition().y + 13 * GameConfig::MAP_SCALE);
}



sf::Sprite Player::getPlayer()
{
	return player_sprite;
}

sf::Sprite Player::getLegs()
{
    return leg_sprite;
}

sf::Vector2f Player::getMovement()
{
    return movement;
};