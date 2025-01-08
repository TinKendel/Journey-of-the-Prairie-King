#pragma once

#include <iostream>
#include "Constants.hpp"
#include "SFML/Graphics.hpp"

class HUD
{
private:
	sf::Texture texture_health_point, texture_coin, texture_clock, texture_progress_bar, texture_item_box;
	sf::Sprite sprite_health_point, sprite_coin, sprite_clock, sprite_progress_bar, sprite_item_box;
	sf::Font font;
	sf::Text text_health_point, text_coin;

public:
	void loadHUDTextures();
	void loadFont();
	void setSpriteHealthPoint();
	void setSpriteCoin();
	void setSpriteClock();
	void setSpriteProgressBar();
	void setSpriteItemBox();
	void setTextHealthPoint();
	void setTextCoin();

	void draw(sf::RenderTarget& target);


};