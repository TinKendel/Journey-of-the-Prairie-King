#pragma once

#include <iostream>
#include "Constants.hpp"
#include "SFML/Graphics.hpp"

/**
 * @class HUD
 * @brief Handles loading, rendering and managing HUD elements.
 *
 * This class is responsible for loading all the HUD elements (power-ups, weapons, healt points, time, coins)
 * and displaying them.
 */
class HUD
{
private:
	sf::Texture texture_health_point,	///< Health point texture for displaying HP
		texture_coin,					///< Coin texture for displaying coins
		texture_clock,					///< Clock texture for displaying a clock
		texture_progress_bar,			///< Progress bar for keeping track of time
		texture_item_box;				///< Item box for displaying players usable power-ups
	sf::Sprite sprite_health_point,		///< Health point sprite
		sprite_coin,					///< Coin sprite
		sprite_clock,					///< Clock sprite
		sprite_progress_bar,			///< Progress bar sprite
		sprite_item_box;				///< Item box sprite
	sf::Font font;						///< Font
	sf::Text text_health_point,			///< Health point text
		text_coin;						///< Coin text

public:
	/**
	 * @brief Loads all textures that will be used for HUD and calls all setSprite functions.
	 */
	void loadHUDTextures();

	/**
	 * @brief Loads font and calls all setText functions.
	 */
	void loadFont();

	/**
	 * @brief Sets the health point sprite.
	 */
	void setSpriteHealthPoint();

	/**
	 * @brief Sets the coin sprite.
	 */
	void setSpriteCoin();

	/**
	 * @brief Sets clock sprite.
	 */
	void setSpriteClock();

	/**
	 * @brief Sets progress bar sprite.
	 */
	void setSpriteProgressBar();

	/**
	 * @brief Sets item box sprite.
	 */
	void setSpriteItemBox();

	/**
	 * @brief Sets health point text.
	 */
	void setTextHealthPoint();

	/**
	 * @brief Sets coin text.
	 */
	void setTextCoin();

	/**
	 * @brief Renders the HUD.
     * @param target the SFML render target.
	 */
	void draw(sf::RenderTarget& target);
};