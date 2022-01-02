#include "../include/wall.hpp"

Wall::Wall(std::array<float, 4> pos) {
	this->shape.setPosition(sf::Vector2f(pos[0], pos[1]));
	this->shape.setSize(sf::Vector2f(pos[2], pos[3]));
	this->shape.setOrigin(sf::Vector2f(pos[2]/2, pos[3]/2));
	this->shape.setFillColor(sf::Color(WALL_COL));
}

Wall::~Wall() {

}

void Wall::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->shape);
}

void Wall::update(float dt) {

}