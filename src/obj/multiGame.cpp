#include "../include/multiGame.hpp"

MultiGame::MultiGame(sf::Font &font): Game(font) {
	this->gameType = GT_MULTIPLAYER;
	this->snakes.push_back(Snake(0));
	this->snakes.push_back(Snake(1));
	this->popupText.setString(POPUP_MULTI_STOP);
}

MultiGame::~MultiGame() {

}

void MultiGame::handleInput(sf::Keyboard::Key code, char pressed) {
	Game::handleInput(code, pressed);

	if (pressed) {
		if (code == sf::Keyboard::Left) this->snakes[0].turn(-1);
		if (code == sf::Keyboard::Right) this->snakes[0].turn(1); 
		if (code == sf::Keyboard::Up) this->snakes[0].setSpeed(1);
		if (code == sf::Keyboard::Down) this->snakes[0].setSpeed(-1);

		if (code == sf::Keyboard::A) this->snakes[1].turn(-1);
		if (code == sf::Keyboard::D) this->snakes[1].turn(1); 
		if (code == sf::Keyboard::W) this->snakes[1].setSpeed(1);
		if (code == sf::Keyboard::S) this->snakes[1].setSpeed(-1);
	} else if (code == sf::Keyboard::Left || code == sf::Keyboard::Right) {
		this->snakes[0].turn(0);
	} else if (code == sf::Keyboard::A || code == sf::Keyboard::D) {
		this->snakes[1].turn(0);
	}
}

void MultiGame::update(float dt) {
	Game::update(dt);

	if (this->state == ST_PLAY) {
		spawnFruit();
		if (Game::checkCollisionMap(this->snakes[0].body[0]) || Game::checkCollisionSnake(this->snakes[0].body[0], this->snakes[0]) || Game::checkCollisionSnake(this->snakes[0].body[0], this->snakes[1])) this->gameOver(1);
		if (Game::checkCollisionMap(this->snakes[1].body[0]) || Game::checkCollisionSnake(this->snakes[1].body[0], this->snakes[1]) || Game::checkCollisionSnake(this->snakes[1].body[0], this->snakes[0])) this->gameOver(0);
	}

	snprintf(this->infoStr, INFO_LEN,
		"Time: %.2f\n"
		"Score: %lu\t| %lu\n"
		"Speed: %d\t|%d",
		this->time, this->snakes[0].body.size(), this->snakes[1].body.size(), this->snakes[0].speed, this->snakes[1].speed);
	this->scoreText.setString(this->infoStr);
}

void MultiGame::gameOver(char success) {
	Game::gameOver(success);
	this->popupText.setString("Player " + std::to_string(success + 1) + " has won!\nPress r to restart");
}

void MultiGame::getScores() {

}

void MultiGame::saveScore(unsigned int) {

}