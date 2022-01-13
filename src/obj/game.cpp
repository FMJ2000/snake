#include "../include/game.hpp"

Game::Game(sf::Font &font): state(ST_INIT), username(""), font(font), time(0), currentSong(rand() % NUM_SONGS) {
	// map and walls
	this->map.setPosition(sf::Vector2f(MAP[0], MAP[1]));
	this->map.setSize(sf::Vector2f(MAP[2], MAP[3]));
	this->map.setFillColor(sf::Color(MAP_COL));
	this->map.setOutlineColor(sf::Color::Yellow);
	this->map.setOutlineThickness(BORDER_WIDTH);
	this->playArea.setPosition(sf::Vector2f(PLAY_AREA[0], PLAY_AREA[1]));
	this->playArea.setSize(sf::Vector2f(PLAY_AREA[2], PLAY_AREA[3]));
	this->playArea.setFillColor(sf::Color::Transparent);
	this->buildWalls();

	// information
	this->scoreText.setPosition(sf::Vector2f(INFO[0], INFO[1]));
	this->scoreText.setFont(this->font);
	this->scoreText.setString("Snake");
	this->popupText.setPosition(sf::Vector2f(POPUP_INIT[0], POPUP_INIT[1]));
	this->popupText.setFont(this->font);
	this->popupText.setFillColor(sf::Color::Black);
	this->popupText.setString(POPUP_STOP);
	this->leaderboardText.setPosition(sf::Vector2f(LEADER[0], LEADER[1]));
	this->leaderboardText.setFont(this->font);
	this->leaderboardText.setString(LEADERBOARD);

	for (int i = 0; i < NUM_SONGS; i++) {
		this->music[i] = new sf::Music();
		this->music[i]->openFromFile(MUSIC[i]);
	}

	for (int i = 0; i < 2; i++) {
		this->soundBuffer[i].loadFromFile(SOUNDS[i]);
		this->sound[i].setBuffer(this->soundBuffer[i]);
	}
}

Game::~Game() {
	for (int i = 0; i < NUM_SONGS; i++) delete this->music[i];
}

void Game::handleInput(sf::Keyboard::Key code, char pressed) {
	if (code == sf::Keyboard::Space && pressed) this->flipState();
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->map);
	for (int i = 0; i < this->walls.size(); i++) target.draw(this->walls[i]);
	for (int i = 0; i < this->fruits.size(); i++) target.draw(this->fruits[i]);
	for (int i = 0; i < this->snakes.size(); i++) target.draw(this->snakes[i]);
	target.draw(this->leaderboardText);
	if (this->state == ST_PLAY) {
		target.draw(this->scoreText);
	}	else target.draw(this->popupText);
}

void Game::update(float dt) {
	if (this->state == ST_PLAY) {
		for (int i = 0; i < this->snakes.size(); i++) {
			this->snakes[i].update(dt);
			int amount = Game::checkCollisionFruit(this->snakes[i].body[0]);
			for (int j = 0; j < amount; j++) this->snakes[i].spawnBody();	
		}
	}

	if (floor(this->time) < floor(this->time + dt)) {
		sf::SoundSource::Status musicStat = this->music[this->currentSong]->getStatus();
		if (this->state == ST_PLAY) {
			if (musicStat != sf::SoundSource::Status::Playing) {
				this->currentSong = rand() % this->music.size();
				this->music[this->currentSong]->play();
			}
		} else if (musicStat == sf::SoundSource::Status::Playing) this->music[this->currentSong]->pause();
	}
	this->time += dt;
}

void Game::buildWalls() {
	std::array<float, 2> centre = { { (float)SNAKE_INIT[0], (float)SNAKE_INIT[1] } };
	for (int i = 0; i < NUM_WALLS; i++) {
		std::array<float, 4> pos = { 0, 0, WALL_WIDTH, WALL_WIDTH };
		char flag;
		do {
			flag = 1;
			pos[0] = 0;
			pos[1] = 0;
			pos[2] = WALL_WIDTH;
			pos[3] = WALL_WIDTH;
			if (rand() % 2) pos[2] = WALL_LENGTH[0] + (float)rand() / RAND_MAX * (WALL_LENGTH[1] - WALL_LENGTH[0]);
			else pos[3] = WALL_LENGTH[0] + (float)rand() / RAND_MAX * (WALL_LENGTH[1] - WALL_LENGTH[0]);
			pos[0] = MAP[0] + pos[2]/2 + (float)rand() / RAND_MAX * (MAP[2] - pos[2]);
			pos[1] = MAP[1] + pos[3]/2 + (float)rand() / RAND_MAX * (MAP[3] - pos[3]);
			std::array<std::array<float, 2>, 4> corners = { {
				{ { pos[0], pos[1] } },
				{ { pos[0] + pos[2], pos[1] } },
				{ { pos[0], pos[1] + pos[3] } },
				{ { pos[0] + pos[2], pos[1] + pos[3] } }
			} };
			for (int i = 0; i < 4; i++) {
				if (this->getDistance(corners[i], centre) < WALL_FREE_ZONE) {
					flag = 0;
					break;
				}
			}
		} while (!flag);
		this->walls.push_back(Wall(pos));
	}
}

char Game::checkCollisionMap(sf::CircleShape body) {
	// map
	std::array<float, 2> pos = { body.getPosition().x, body.getPosition().y };
	if (!this->playArea.getGlobalBounds().intersects(body.getGlobalBounds())) {
		this->sound[SI_CRASH].play();
		return 1;
	}

	// walls
	for (int i = 0; i < this->walls.size(); i++) {
		sf::FloatRect rect = this->walls[i].shape.getGlobalBounds();
		if ((pos[0] > rect.left - SNAKE_R * SNAKE_COL_F && pos[0] < rect.left + rect.width + SNAKE_R * SNAKE_COL_F && pos[1] > rect.top && pos[1] < rect.top + rect.height) ||
			(pos[0] > rect.left && pos[0] < rect.left + rect.width && pos[1] > rect.top - SNAKE_R * SNAKE_COL_F && pos[1] < rect.top + rect.height + SNAKE_R * SNAKE_COL_F)) {
			this->sound[SI_CRASH].play();
			return 1;
		}
	}

	return 0;
}

char Game::checkCollisionSnake(sf::CircleShape body, Snake snake) {
	std::array<float, 2> pos = { body.getPosition().x, body.getPosition().y };
	for (int i = 1; i < snake.body.size(); i++) {
		if (snake.bodyTime[i] > SPAWN_T) {
			std::array<float, 2> pos2 = { snake.body[i].getPosition().x, snake.body[i].getPosition().y };
			if (this->getDistance(pos, pos2) < SNAKE_R + COL_MOD) {
				this->sound[SI_CRASH].play();
				return 1;
			}
		}
	}

	return 0;
}

int Game::checkCollisionFruit(sf::CircleShape body) {
	for (int i = 0; i < this->fruits.size(); i++) {
		if (this->fruits[i].shape.getGlobalBounds().intersects(body.getGlobalBounds())) {
			int amount = this->fruits[i].amount;
			this->fruits.erase(this->fruits.begin() + i);
			this->sound[SI_EAT].play();
			return amount;
		}
	}

	return 0;
}

void Game::spawnFruit() {
	while (this->fruits.size() < NUM_FRUIT) {
		std::array<float, 2> pos;
		char flag;
		int index = 0;
		do {
			index++; 
			flag = 1;
			pos[0] = PLAY_AREA[0] + (float)rand() / RAND_MAX * PLAY_AREA[2];
			pos[1] = PLAY_AREA[1] + (float)rand() / RAND_MAX * PLAY_AREA[3];
			for (int i = 0; i < this->snakes.size() && flag; i++) {
				for (int j = 0; j < this->snakes[0].body.size(); j++) {
					std::array<float, 2> pos2 = { this->snakes[i].body[j].getPosition().x, this->snakes[i].body[j].getPosition().y };
					if (getDistance(pos, pos2) < SNAKE_R + COL_MOD) flag = 0;
				}
			}
			
			for (int i = 0; i < this->walls.size() && flag; i++) {
				sf::FloatRect rect = this->walls[i].shape.getGlobalBounds();
				if (pos[0] > rect.left - FRUIT_R && pos[0] < rect.left + rect.width + FRUIT_R && pos[1] > rect.top - FRUIT_R && pos[1] < rect.top + rect.height + FRUIT_R) flag = 0;
			}
		} while (!flag && index < MAX_ITER);
		if (flag) this->fruits.push_back(Fruit(pos, this->font));
	}
}

float Game::getDistance(std::array<float, 2> pos1, std::array<float, 2> pos2) {
	return sqrt(powf(pos1[0] - pos2[0], 2) + powf(pos1[1] - pos2[1], 2));
}

void Game::gameOver(char success) {
	this->fruits.clear();
	this->state = ST_PAUSE;
}

void Game::typeUsername(char letter) {
	if (this && this->state == ST_INIT && this->time > DEL_TEXT) {
		if (letter == CHR_ENTER) {
			this->getScores();
			this->state = ST_PLAY;
		} else if (this->gameType != GT_MULTIPLAYER) {
			if (letter == CHR_BACKSPACE && this->username.getSize() > 0) this->username.erase(this->username.getSize() - 1, 1);
			else this->username += letter;	
			this->popupText.setString(POPUP_STOP + std::string(this->username));
		}
	}
}

void Game::flipState() {
	if (this->state == ST_PAUSE) this->state = ST_PLAY;
	else if (this->state = ST_PLAY) {
		this->state = ST_PAUSE;
		this->popupText.setString(POPUP_PAUSE);
	}
}

void Game::getScores() {

}

void Game::saveScore(unsigned int) {

}