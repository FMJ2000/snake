#ifndef _GAME_HPP
#define _GAME_HPP

#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "const.hpp"
#include "snake.hpp"
#include "fruit.hpp"
#include "wall.hpp"
#include "score.hpp"

class Game: public sf::Drawable {
	private:
		sf::Font font;
		sf::Text scoreText;
		sf::Text leaderboardText;
		sf::Text popupText;
		sf::RectangleShape map;
		sf::RectangleShape playArea;
		std::vector<Score> scores;
		sf::Music music;
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
	public:
		Snake * snake;
		std::vector<Fruit> fruits;
		std::vector<Wall> walls;
		sf::String username;
		char state;
		Game();
		~Game();
		void update(float dt);
		float getDistance(std::array<float, 2> pos1, std::array<float, 2> pos2);
		void gameOver();
		void getScores();
		void saveScore(unsigned int score);
		void typeUsername(char letter);
		void flipState();
};

#endif