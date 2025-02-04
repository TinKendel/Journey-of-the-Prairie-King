#include "HUD.hpp"

void HUD::loadHUDTextures()
{
	if (!texture_health_point.loadFromFile("assets\\tiledPNG\\HUD\\Items\\Items.png"))
	{
		std::cerr << "Failed to load health point texture!" << std::endl;
	}

	if (!texture_coin.loadFromFile("assets\\tiledPNG\\HUD\\Items\\Items.png"))
	{
		std::cerr << "Failed to load coin texture!" << std::endl;
	}

	if (!texture_clock.loadFromFile("assets\\tiledPNG\\HUD\\Time\\clockblackbg.png"))
	{
		std::cerr << "Failed to load clock texture!" << std::endl;
	}

	if (!texture_progress_bar.loadFromFile("assets\\tiledPNG\\HUD\\Time\\progress.png"))
	{
		std::cerr << "Failed to load proress bar texture!" << std::endl;
	}

	if (!texture_item_box.loadFromFile("assets\\tiledPNG\\HUD\\ItemStorage\\itemStorage.png"))
	{
		std::cerr << "Failed to load item box texture!" << std::endl;
	}

	setSpriteHealthPoint();
	setSpriteCoin();
	setSpriteClock();
	setSpriteProgressBar();
	setSpriteItemBox();
}

void HUD::loadFont()
{
	if (!font.loadFromFile("assets\\font\\PixelOperatorSC.ttf"))
	{
		std::cerr << "Failed to load PixelOperatorSC.ttf font!" << std::endl;
	}

	setTextHealthPoint();
	setTextCoin();
}

void HUD::setSpriteHealthPoint()
{
	sprite_health_point.setTexture(texture_health_point);
	sprite_health_point.setTextureRect(sf::IntRect(127, 0, 16, 16));
	sprite_health_point.setPosition(0.f, 76.f);
	sprite_health_point.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);
}

void HUD::setSpriteCoin()
{
	sprite_coin.setTexture(texture_coin);
	sprite_coin.setTextureRect(sf::IntRect(0, 0, 16, 16));
	sprite_coin.setPosition(0.f, 108.f);
	sprite_coin.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);
}
void HUD::setSpriteClock()
{
	sprite_clock.setTexture(texture_clock);
	sprite_clock.setPosition(58.f, 0.f);
	sprite_clock.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);
}

void HUD::setSpriteProgressBar()
{
	sprite_progress_bar.setTexture(texture_progress_bar);
	sprite_progress_bar.setPosition(94.f, 14.f);
	sprite_progress_bar.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);
}

void HUD::setSpriteItemBox()
{
	sprite_item_box.setTexture(texture_item_box);
	sprite_item_box.setPosition(14.f, 32.f);
	sprite_item_box.setScale(GameConfig::MAP_SCALE, GameConfig::MAP_SCALE);
}

void HUD::setTextHealthPoint()
{
	text_health_point.setFont(font);
	text_health_point.setString("x99");

	text_health_point.setCharacterSize(24);
	text_health_point.setFillColor(sf::Color::White);
	text_health_point.setPosition(32.f, 76.f);
}

void HUD::setTextCoin()
{
	text_coin.setFont(font);
	text_coin.setString("x88");

	text_coin.setCharacterSize(24);
	text_coin.setFillColor(sf::Color::White);
	text_coin.setPosition(32.f, 108.f);
}

void HUD::draw(sf::RenderTarget& target)
{
	target.draw(sprite_health_point);
	target.draw(sprite_coin);
	target.draw(sprite_clock);
	target.draw(sprite_progress_bar);
	target.draw(sprite_item_box);
	target.draw(text_health_point);
	target.draw(text_coin);
}


void HUD::startAreaTimer(sf::Clock& area_clock)
{
	if (area_clock.getElapsedTime().asSeconds() <= 60)
	{
		sprite_progress_bar.setTextureRect(sf::IntRect(0, 0, 241 - (area_clock.getElapsedTime().asSeconds() * 4), 5));
	}
	else
	{
		time_limit_reached = true;
		sprite_progress_bar.setTextureRect(sf::IntRect(0, 0, 241, 5));
	}
}

void HUD::updateHealthPoints()
{
	text_health_point.setString("00");
}