#include "../include/snake.hpp"

Snake::Snake(): turnState(0), speed(1) {
	this->pos[0] = SNAKE_INIT[0];
	this->pos[1] = SNAKE_INIT[1];
	this->pos[2] = 0;
	this->spawnBody();

	this->head.setFillColor(sf::Color::Yellow);
	this->head.setRadius(SNAKE_HEAD_R);
	this->head.setPointCount(3);
	this->head.setOrigin(sf::Vector2f(SNAKE_HEAD_R, SNAKE_HEAD_R));
}

Snake::~Snake() {

}

void Snake::spawnBody() {
	this->body.push_back(sf::CircleShape());
	this->bodyTime.push_back(0);
	this->body[this->body.size() - 1].setFillColor(sf::Color(SNAKE_COL));
	this->body[this->body.size() - 1].setRadius(SNAKE_R);
	this->body[this->body.size() - 1].setOrigin(sf::Vector2f(SNAKE_R, SNAKE_R));
	this->body[this->body.size() - 1].setPointCount(6);
}

void Snake::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	for (int i = 0; i < this->body.size(); i++) target.draw(this->body[i]);
	target.draw(this->head);
}

void Snake::update(float dt) {
	this->pos[2] += this->turnState * SNAKE_T * dt;
	this->pos[0] += SNAKE_V * this->speed * dt * cos(this->pos[2]);
	this->pos[1] += SNAKE_V * this->speed * dt * sin(this->pos[2]);

	// log
	this->time += dt;
	if (this->time >= LOG_T) {
		this->log.push_back(this->pos);
		//std::cout << this->log[this->log.size() - 1][0] << ", " << this->log[this->log.size() - 1][1] << ", " << this->log[this->log.size() - 1][2] << std::endl;
		this->time -= LOG_T;
	}

	this->body[0].setPosition(sf::Vector2f(this->pos[0], this->pos[1]));
	this->body[0].setRotation(this->pos[2] * 180.0 / M_PI - 30.0);
	this->head.setPosition(sf::Vector2f(this->pos[0], this->pos[1]));
	this->head.setRotation(this->pos[2] * 180.0 / M_PI - 30.0);

	for (int i = 1; i < this->body.size(); i++) {
		int index = this->log.size() - 1 - LOG_D * i;
		if (index < 0) index = 0;
		std::array<float, 3> bodyPos;// = this->log[this->log.size() - 1 - LOG_D * i];
		for (int j = 0; j < 3; j++) bodyPos[j] = this->log[index][j] + (this->time / LOG_T) * (this->log[index+1][j] - this->log[index][j]);
		this->body[i].setPosition(sf::Vector2f(bodyPos[0], bodyPos[1]));
		this->body[i].setRotation(bodyPos[2] * 180.0 / M_PI - 30.0);
		this->bodyTime[i] += dt;
		if (this->bodyTime[i] < SPAWN_T) this->body[i].setFillColor(sf::Color(SNAKE_SPAWN_COL));
		else this->body[i].setFillColor(sf::Color(SNAKE_COL));
	}
}

void Snake::turn(int dir) {
	this->turnState = dir;
}

void Snake::setSpeed(int dir) {
	this->speed += dir;
	if (this->speed < 1) this->speed = 1;
	if (this->speed > SNAKE_MAX_V) this->speed = SNAKE_MAX_V;
}