#include "../include/fruit.hpp"

Fruit::Fruit(std::array<float, 2> pos, sf::Font &font) {
	this->shape.setPosition(sf::Vector2f(pos[0], pos[1]));
	this->shape.setRadius(FRUIT_R);
	this->shape.setFillColor(sf::Color(FRUIT_COL));
	this->shape.setOutlineThickness(2);
	this->shape.setOutlineColor(sf::Color::White);

	this->amount = (rand() % MAX_SPAWN) + 1;
	this->info.setPosition(sf::Vector2f(pos[0] + FRUIT_INFO[0], pos[1] + FRUIT_INFO[1]));
	this->info.setFont(font);
	this->info.setCharacterSize(20);
	this->info.setString(std::to_string(this->amount));
}

Fruit::~Fruit() {

}

void Fruit::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->shape);
	target.draw(this->info);
}

void Fruit::update(float dt) {

}