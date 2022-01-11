#include "../include/menu.hpp"

Menu::Menu(sf::Font &font) {
	this->info.setFont(font);
	this->info.setString(MENU);
	this->info.setPosition(sf::Vector2f((WINDOW[0] - this->info.getGlobalBounds().width) / 2, (WINDOW[1] - this->info.getGlobalBounds().height)/2));
}

Menu::~Menu() {

}

void Menu::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->info);
}