#ifndef _MULTI_GAME_HPP
#define _MULTI_GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>

#include "const.hpp"
#include "game.hpp"

class MultiGame: public Game {
	public:
		MultiGame(sf::Font &font);
		~MultiGame();
		virtual void update(float dt) override;
		virtual void handleInput(sf::Keyboard::Key code, char pressed) override;
		virtual void gameOver(char success) override;
		virtual void getScores();
		virtual void saveScore(unsigned int score);
};

#endif