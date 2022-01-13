#ifndef _GAME_HPP
#define _GAME_HPP

#include <iostream>
#include <random>
#include <vector>
#include <fstream>
#include <string>
#include <cstring>
#include <SFML/Graphics.hpp>
#include <SFML/Audio.hpp>

#include "const.hpp"
#include "snake.hpp"
#include "fruit.hpp"
#include "wall.hpp"
#include "score.hpp"

class Game: public sf::Drawable {	
	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;
	
	public:
		float time;
		char infoStr[INFO_LEN];
		GameType gameType;
		sf::Font font;
		sf::Text scoreText;
		sf::Text leaderboardText;
		sf::Text popupText;
		sf::RectangleShape map;
		sf::RectangleShape playArea;
		std::vector<Score> scores;
		std::array<sf::Music *, NUM_SONGS> music;
		unsigned int currentSong;
		State state;
		std::vector<Snake> snakes;
		std::vector<Fruit> fruits;
		std::vector<Wall> walls;
		sf::String username;
		std::array<sf::SoundBuffer, 2> soundBuffer;
		std::array<sf::Sound, 2> sound;

		Game(sf::Font &font);
		~Game();
		virtual void handleInput(sf::Keyboard::Key code, char pressed);
		virtual void update(float dt);
		virtual void gameOver(char success);
		virtual void getScores();
		virtual void saveScore(unsigned int score);
		void buildWalls();
		char checkCollisionMap(sf::CircleShape body);
		char checkCollisionSnake(sf::CircleShape body, Snake snake);
		int checkCollisionFruit(sf::CircleShape body);
		void spawnFruit();
		float getDistance(std::array<float, 2> pos1, std::array<float, 2> pos2);
		void typeUsername(char letter);
		void flipState();
};

#endif