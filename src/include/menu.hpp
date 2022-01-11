#ifndef _MENU_HPP
#define _MENU_HPP

#include <SFML/Graphics.hpp>
#include <iostream>

#include "const.hpp"

class Menu: public sf::Drawable {
	private:
		sf::Text info;
		virtual void draw(sf::RenderTarget &target, sf::RenderStates states) const;

	public:
		Menu(sf::Font &font);
		~Menu();
};

#endif