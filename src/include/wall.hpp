#ifndef _WALL_HPP
#define _WALL_HPP

#include <iostream>
#include <random>
#include <SFML/Graphics.hpp>

#include "const.hpp"

class Wall: public sf::Drawable {
	private:
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		sf::RectangleShape shape;
		Wall(std::array<float, 4> pos);
		~Wall();
		void update(float dt);
};

#endif