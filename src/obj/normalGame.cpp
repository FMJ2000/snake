#include "../include/normalGame.hpp"

NormalGame::NormalGame(sf::Font &font): Game(font) {
	this->gameType = GT_NORMAL;
	this->snakes.push_back(Snake());
}

NormalGame::~NormalGame() {

}

void NormalGame::handleInput(sf::Keyboard::Key code, char pressed) {
	Game::handleInput(code, pressed);
	
	if (pressed) {
		if (code == sf::Keyboard::Left) this->snakes[0].turn(-1);
		if (code == sf::Keyboard::Right) this->snakes[0].turn(1); 
		if (code == sf::Keyboard::Up) this->snakes[0].setSpeed(1);
		if (code == sf::Keyboard::Down) this->snakes[0].setSpeed(-1);
	} else if (code == sf::Keyboard::Left || code == sf::Keyboard::Right) {
		this->snakes[0].turn(0);
	}
}

void NormalGame::update(float dt) {
	Game::update(dt);

	if (this->state == ST_PLAY) {
		spawnFruit();
		if (Game::checkCollisionMap(this->snakes[0].body[0]) || Game::checkCollisionSnake(this->snakes[0].body[0], this->snakes[0])) this->gameOver(1);
	}

	snprintf(this->infoStr, INFO_LEN,
		"%s\n"
		"Time: %.2f\n"
		"Score: %lu\n"
		"Speed: %d",
		std::string(this->username).c_str(), this->time, this->snakes[0].body.size(), this->snakes[0].speed);
	this->scoreText.setString(this->infoStr);
}

void NormalGame::gameOver(char success) {
	Game::gameOver(success);
	this->popupText.setString("Score: " + std::to_string(this->snakes[0].body.size()) + "\nPress r to restart");
	this->saveScore(this->snakes[0].body.size());
}

void NormalGame::saveScore(unsigned int score) {
	std::ofstream file(NHS_FILE, std::ofstream::app);
	file << std::string(this->username) << "," << std::to_string(score) << "\n";
	file.close();
}

void NormalGame::getScores() {
	// get scores from file
	std::ifstream file(NHS_FILE, std::ifstream::in);
	std::string line;
	std::string username;
	unsigned int score;
	if (file) {
		while (std::getline(file, line)) {
			username = line.substr(0, line.find(','));
			score = std::stoul(line.substr(line.find(',') + 1));
			this->scores.push_back(Score(username, score, 0));
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
		int len = snprintf(this->infoStr, INFO_LEN, "%s", LEADERBOARD);
		for (int i = 0; i < max; i++) {
			len += snprintf(this->infoStr + len, INFO_LEN - len, "\n%u\t%s", this->scores[i].score, this->scores[i].username.c_str());
		}
		this->leaderboardText.setString(this->infoStr);
	}
}