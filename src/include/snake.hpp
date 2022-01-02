#ifndef _SNAKE_HPP
#define _SNAKE_HPP

#include <iostream>
#include <random>
#include <vector>
#include <cmath>
#include <array>
#include <SFML/Graphics.hpp>

#include "const.hpp"

class Snake: public sf::Drawable {
	private:
		int turnState;
		float time;
		std::vector<std::array<float, 3>> log;
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		char speed;
		std::array<float, 3> pos;
		std::vector<sf::CircleShape> body;
		std::vector<float> bodyTime;
		sf::CircleShape head;
		Snake();
		~Snake();
		void spawnBody();
		void update(float dt);
		void turn(int dir);
		void setSpeed(int dir);
};

#endif