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

    // Lambda to simplify setting the players texture
    auto setTextureRect = [&](int left, int top)
    {
        player_sprite.setTextureRect(sf::IntRect(left, top, TileConfig::TILE_WIDTH, TileConfig::TILE_HEIGHT));
    };

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::W))
    {
        if (shooting == false)
        {
            setTextureRect(0, 0);
        }
        movement.y -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::S))
    {
        if (shooting == false)
        {
            setTextureRect(32, 0);
        }
        movement.y += speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::A))
    {
        if (shooting == false)
        {
            setTextureRect(48, 0);
        }
        movement.x -= speed;
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::D))
    {
        if (shooting == false)
        {
            setTextureRect(16, 0);
        }
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
    
    if (movement == sf::Vector2f(0, 0) && direction == sf::Vector2f(0, 0))
    {
        leg_sprite.setTextureRect(sf::IntRect(16, 20, 16, 3)); // Idle legs texture
        setTextureRect(32, 16); // Idle player texture
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

void Player::loadBulletTexture(const std::string& filePath) {
    if (!bullet_texture.loadFromFile(filePath)) {
        std::cerr << "Failed to load bullet texture!" << std::endl;
    }
}

void Player::shoot() {
    if (shoot_clock.getElapsedTime().asSeconds() < shoot_cooldown) 
    {
        return; // Prevent shooting if cooldown is not complete
    }

    shoot_clock.restart();

    // Lambda to simplify setting the players texture
    auto setTextureRect = [&](int left, int top)
    {
        player_sprite.setTextureRect(sf::IntRect(left, top, TileConfig::TILE_WIDTH, TileConfig::TILE_HEIGHT));
    };

    // Determine shooting direction based on key presses
    direction = {0.f, 0.f};

    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Up)) 
    {
        shooting = true;
        setTextureRect(0, 0);
        direction.y = -1.f; // Up
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Down)) 
    {
        shooting = true;
        setTextureRect(32, 0);
        direction.y = 1.f; // Down
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Left)) 
    {
        shooting = true;
        setTextureRect(48, 0);
        direction.x = -1.f; // Left
    }
    if (sf::Keyboard::isKeyPressed(sf::Keyboard::Right)) 
    {
        shooting = true;
        setTextureRect(16, 0);
        direction.x = 1.f; // Right
    }

    // Normalize the direction vector
    if (direction != sf::Vector2f(0.f, 0.f)) {
        float length = std::sqrt(direction.x * direction.x + direction.y * direction.y);
        direction /= length; // Normalize the vector to have a magnitude of 1
    }

    // If no direction is pressed, don't shoot
    if (direction == sf::Vector2f(0.f, 0.f)) 
    {
        shooting = false;
        return;
    }

    // Create the bullet in the calculated direction
    sf::Vector2f player_center = { player_sprite.getPosition().x + 12,  player_sprite.getPosition().y + 12 };
    bullets.emplace_back(bullet_texture, player_center, direction);
}

void Player::updateBullets(float deltaTime, MapLoader map) 
{
    for (auto it = bullets.begin(); it != bullets.end();) 
    {
        it->update(deltaTime);

        if (map.checkBulletCollision(*it)) 
        {
            it = bullets.erase(it); // Remove bullets that are out of bounds
        }
        else 
        {
            ++it;
        }
    }
}

void Player::drawBullets(sf::RenderWindow& window) 
{
    for (const auto& bullet : bullets) 
    {
        bullet.draw(window);
    }
}