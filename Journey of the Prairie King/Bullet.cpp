#include "Bullet.hpp"

Bullet::Bullet(const sf::Texture& texture, const sf::Vector2f& position, const sf::Vector2f& direction, float speed) : speed(speed)
{
    bullet_sprite.setTexture(texture);
    bullet_sprite.setPosition(position);
    bullet_sprite.setScale(2.f, 2.f); // Adjust the size as needed
    velocity = direction * speed;
}

void Bullet::update(float deltaTime)
{
    bullet_sprite.move(velocity * speed * deltaTime);
}

void Bullet::draw(sf::RenderWindow& window) const
{
    window.draw(bullet_sprite);
}

sf::FloatRect Bullet::getBounds() const
{
    return bullet_sprite.getGlobalBounds();
}

