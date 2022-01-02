#include "../include/game.hpp"

Game::Game(): state(STOP), username("") {
	this->snake = new Snake();

	// map and walls
	this->map.setPosition(sf::Vector2f(MAP[0], MAP[1]));
	this->map.setSize(sf::Vector2f(MAP[2], MAP[3]));
	this->map.setFillColor(sf::Color(MAP_COL));
	this->map.setOutlineColor(sf::Color::Yellow);
	this->map.setOutlineThickness(BORDER_WIDTH);
	this->playArea.setPosition(sf::Vector2f(PLAY_AREA[0], PLAY_AREA[1]));
	this->playArea.setSize(sf::Vector2f(PLAY_AREA[2], PLAY_AREA[3]));
	this->playArea.setFillColor(sf::Color::Transparent);
	for (int i = 0; i < NUM_WALLS; i++) {
		std::array<float, 4> pos = { 0, 0, WALL_WIDTH, WALL_WIDTH };
		if (rand() % 2) pos[2] = WALL_LENGTH[0] + (float)rand() / RAND_MAX * (WALL_LENGTH[1] - WALL_LENGTH[0]);
		else pos[3] = WALL_LENGTH[0] + (float)rand() / RAND_MAX * (WALL_LENGTH[1] - WALL_LENGTH[0]);
		pos[0] = MAP[0] + pos[2]/2 + (float)rand() / RAND_MAX * (MAP[2] - pos[2]);
		pos[1] = MAP[1] + pos[3]/2 + (float)rand() / RAND_MAX * (MAP[3] - pos[3]);
		this->walls.push_back(Wall(pos));
	}

	// information
	this->font.loadFromFile(FONT_FILE);
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
}

Game::~Game() {
	delete this->snake;
}

void Game::draw(sf::RenderTarget &target, sf::RenderStates states) const {
	target.draw(this->map);
	for (int i = 0; i < this->walls.size(); i++) target.draw(this->walls[i]);
	for (int i = 0; i < this->fruits.size(); i++) target.draw(this->fruits[i]);
	target.draw(*this->snake);
	if (this->state == PLAY) {
		target.draw(this->scoreText);
		target.draw(this->leaderboardText);
	}	else target.draw(this->popupText);
}

void Game::update(float dt) {
	if (this->state == PLAY) {
		while (this->fruits.size() < NUM_FRUIT) {
			std::array<float, 2> pos;
			char flag;
			int index = 0;
			do {
				index++; 
				flag = 1;
				pos[0] = PLAY_AREA[0] + (float)rand() / RAND_MAX * PLAY_AREA[2];
				pos[1] = PLAY_AREA[1] + (float)rand() / RAND_MAX * PLAY_AREA[3];
				for (int i = 0; i < this->snake->body.size(); i++) {
					std::array<float, 2> pos2 = { this->snake->body[i].getPosition().x, this->snake->body[i].getPosition().y };
					if (getDistance(pos, pos2) < SNAKE_R + COL_MOD) {
						flag = 0;
						break;
					}
				}
				for (int i = 0; i < this->walls.size(); i++) {
					sf::FloatRect rect = this->walls[i].shape.getGlobalBounds();
					if (pos[0] > rect.left - FRUIT_R && pos[0] < rect.left + rect.width + FRUIT_R && pos[1] > rect.top - FRUIT_R && pos[1] < rect.top + rect.height + FRUIT_R) {
						flag = 0;
						break;
					}
				}
			} while (!flag && index < MAX_ITER);
			if (flag) this->fruits.push_back(Fruit(pos, this->font));
		}

		this->snake->update(dt);

		// collision detection
		if (!this->playArea.getGlobalBounds().intersects(this->snake->body[0].getGlobalBounds())) this->gameOver();
		std::array<float, 2> pos1 = { this->snake->body[0].getPosition().x, this->snake->body[0].getPosition().y };
		for (int i = 0; i < this->walls.size(); i++) {
			sf::FloatRect rect = this->walls[i].shape.getGlobalBounds();
			if ((pos1[0] > rect.left - SNAKE_R && pos1[0] < rect.left + rect.width + SNAKE_R && pos1[1] > rect.top && pos1[1] < rect.top + rect.height) ||
				(pos1[0] > rect.left && pos1[0] < rect.left + rect.width && pos1[1] > rect.top - SNAKE_R && pos1[1] < rect.top + rect.height + SNAKE_R))
				this->gameOver();
		}
		for (int i = 1; i < this->snake->body.size(); i++) {
			if (this->snake->bodyTime[i] > SPAWN_T) {
				std::array<float, 2> pos2 = { this->snake->body[i].getPosition().x, this->snake->body[i].getPosition().y };
				if (this->getDistance(pos1, pos2) < SNAKE_R + COL_MOD) this->gameOver();
			}
		}
		for (int i = 0; i < this->fruits.size(); i++) {
			if (this->fruits[i].shape.getGlobalBounds().intersects(this->snake->body[0].getGlobalBounds())) {
				for (int j = 0; j < this->fruits[i].amount; j++) this->snake->spawnBody();
				this->fruits.erase(this->fruits.begin() + i);
			}
		}
	}

	this->scoreText.setString(this->username + "\nScore: " + std::to_string(this->snake->body.size()) + "\nSpeed: " + std::to_string(this->snake->speed));
}

float Game::getDistance(std::array<float, 2> pos1, std::array<float, 2> pos2) {
	return sqrt(powf(pos1[0] - pos2[0], 2) + powf(pos1[1] - pos2[1], 2));
}

void Game::gameOver() {
	this->fruits.clear();
	this->popupText.setString("Score: " + std::to_string(this->snake->body.size()) + "\nPress r to restart");
	this->state = PAUSE;
	this->saveScore(this->snake->body.size());
}

void Game::typeUsername(char letter) {
	if (letter == CHR_ENTER) {
		this->getScores();
		this->state = PLAY;
	} else {
		if (letter == CHR_BACKSPACE) this->username.erase(this->username.getSize() - 1, 1);
		else this->username += letter;	
		this->popupText.setString(POPUP_STOP + std::string(this->username));
	}
}

void Game::flipState() {
	this->state ^= 1;
	if (this->state == PAUSE) this->popupText.setString(POPUP_PAUSE);
}

void Game::getScores() {
	// get scores from file
	std::ifstream file(HS_FILE, std::ifstream::in);
	std::string line;
	std::string username;
	unsigned int score;
	if (file) {
		while (std::getline(file, line)) {
			username = line.substr(0, line.find(','));
			score = std::stoul(line.substr(line.find(',') + 1));
			this->scores.push_back(Score(username, score));
		}
	}
	file.close();

	if (this->scores.size()) {
		// sort from highest to lowest
		Score temp;
		unsigned int max = (LEADER_MAX < this->scores.size()) ? LEADER_MAX : this->scores.size();
		for (int i = 0; i < max; i++) {
			for (int j = i+1; j < this->scores.size(); j++) {
				if (this->scores[j].score > this->scores[i].score) {
					temp.copyScore(this->scores[i]);
					this->scores[i].copyScore(this->scores[j]);
					this->scores[j].copyScore(temp);
				}
			}
		}

		// display on leaderboard
		std::string text = LEADERBOARD;
		for (int i = 0; i < max; i++) {
			text += std::to_string(this->scores[i].score) + "\t" + this->scores[i].username + "\n";
		}
		this->leaderboardText.setString(text);
	}
}

void Game::saveScore(unsigned int score) {
	std::ofstream file(HS_FILE, std::ofstream::app);
	file << std::string(this->username) << "," << std::to_string(score) << "\n";
	file.close();
}