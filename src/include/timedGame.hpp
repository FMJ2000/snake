#ifndef _TIMED_GAME_HPP
#define _TIMED_GAME_HPP

#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>
#include <iostream>
#include <random>

#include "const.hpp"
#include "game.hpp"

class TimedGame: public Game {
	public:
		TimedGame(sf::Font &font);
		~TimedGame();
		virtual void update(float dt) override;
		virtual void handleInput(sf::Keyboard::Key code, char pressed) override;
		virtual void gameOver(char success) override;
		virtual void getScores();
		virtual void saveScore(unsigned int score);
};

#endif