#ifndef _FRUIT_HPP
#define _FRUIT_HPP

#include <iostream>
#include <vector>
#include <random>
#include <SFML/Graphics.hpp>

#include "const.hpp"

class Fruit: public sf::Drawable {
	private:
		sf::Text info;
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		int amount;
		sf::CircleShape shape;
		Fruit(std::array<float, 2> pos, sf::Font &font);
		~Fruit();
		void update(float dt);
};

#endif