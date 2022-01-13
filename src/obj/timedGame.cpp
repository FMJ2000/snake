#include "../include/timedGame.hpp"

TimedGame::TimedGame(sf::Font &font): Game(font) {
	this->gameType = GT_TIMED;
	this->snakes.push_back(Snake());
}

TimedGame::~TimedGame() {

}

void TimedGame::handleInput(sf::Keyboard::Key code, char pressed) {
	Game::handleInput(code, pressed);

	if (pressed) {
		if (code == sf::Keyboard::Left) this->snakes[0].turn(-1);
		if (code == sf::Keyboard::Right) this->snakes[0].turn(1); 
		if (code == sf::Keyboard::Up) this->snakes[0].setSpeed(1);
		if (code == sf::Keyboard::Down) this->snakes[0].setSpeed(-1);
	} else this->snakes[0].turn(0);
}

void TimedGame::update(float dt) {
	Game::update(dt);

	if (this->state == ST_PLAY) {
		spawnFruit();
		for (int i = 0; i < this->snakes.size(); i++) this->snakes[i].update(dt);
		if (Game::checkCollisionMap(this->snakes[0].body[0]) || Game::checkCollisionSnake(this->snakes[0].body[0], this->snakes[0])) this->gameOver(0);
		if (this->snakes[0].body.size() >= TIMED_MAX) this->gameOver(1);
		int amount = Game::checkCollisionFruit(this->snakes[0].body[0]);
		if (amount) for (int i = 0; i < amount; i++) this->snakes[0].spawnBody();
	}

	snprintf(this->infoStr, INFO_LEN,
		"%s\n"
		"Time: %.2f\n"
		"Score: %lu\n"
		"Speed: %d",
		std::string(this->username).c_str(), this->time, this->snakes[0].body.size(), this->snakes[0].speed);
	this->scoreText.setString(std::string(this->infoStr));
}

void TimedGame::gameOver(char success) {
	Game::gameOver(success);
	if (success) {
		snprint(this->infoStr, INFO_LEN, "%s\nTime: %.2f\nPress r to restart", TIMED_SUCCESS_MSG, this->time);
		this->popupText.setString(this->infoStr);
		this->saveScore(this->snakes[0].body.size());
	} else {
		this->popupText.setString(TIMED_FAIL_MSG);
	}	
}

void TimedGame::saveScore(unsigned int score) {
	std::ofstream file(THS_FILE, std::ofstream::app);
	file << std::string(this->username) << "," << std::to_string(this->time) << "\n";
	file.close();
}

void TimedGame::getScores() {
	// get scores from file
	std::ifstream file(THS_FILE, std::ifstream::in);
	std::string line;
	std::string username;
	float time;
	if (file) {
		while (std::getline(file, line)) {
			username = line.substr(0, line.find(','));
			time = std::stoul(line.substr(line.find(',') + 1));
			this->scores.push_back(Score(username, 0, time));
		}
	}
	file.close();

	if (this->scores.size()) {
		// sort from highest to lowest
		Score temp;
		unsigned int max = (LEADER_MAX < this->scores.size()) ? LEADER_MAX : this->scores.size();
		for (int i = 0; i < max; i++) {
			for (int j = i+1; j < this->scores.size(); j++) {
				if (this->scores[j].time < this->scores[i].time) {
					temp.copyScore(this->scores[i]);
					this->scores[i].copyScore(this->scores[j]);
					this->scores[j].copyScore(temp);
				}
			}
		}

		// display on leaderboard
		int len = snprintf(this->infoStr, INFO_LEN, "%s", LEADERBOARD);
		for (int i = 0; i < max; i++) {
			len += snprintf(this->infoStr + len, INFO_LEN - len, "\n%.2f s\t%s", this->scores[i].time, this->scores[i].username.c_str());
		}
		this->leaderboardText.setString(this->infoStr);
	}
}