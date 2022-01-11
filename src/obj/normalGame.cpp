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
	} else this->snakes[0].turn(0);
}

void NormalGame::update(float dt) {
	Game::update(dt);

	if (this->state == ST_PLAY) {
		spawnFruit();
		for (int i = 0; i < this->snakes.size(); i++) this->snakes[i].update(dt);
		if (Game::checkCollisionMap(this->snakes[0].body[0]) || Game::checkCollisionSnake(this->snakes[0].body[0], this->snakes[0])) this->gameOver(1);
		int amount = Game::checkCollisionFruit(this->snakes[0].body[0]);
		if (amount) for (int i = 0; i < amount; i++) this->snakes[0].spawnBody();
	}

	this->scoreText.setString(this->username + "\nTime: " + std::to_string(round(this->time*100)/100) + "\nScore: " + std::to_string(this->snakes[0].body.size()) + "\nSpeed: " + std::to_string(this->snakes[0].speed));
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
		std::string text = LEADERBOARD;
		for (int i = 0; i < max; i++) {
			text += std::to_string(this->scores[i].score) + "\t" + this->scores[i].username + "\n";
		}
		this->leaderboardText.setString(text);
	}
}