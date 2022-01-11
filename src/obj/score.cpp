#include "../include/score.hpp"

Score::Score() {
	
}

Score::Score(std::string username, unsigned int score, float time): username(username), score(score), time(time) {

}


Score::~Score() {

}

void Score::setScore(std::string username, unsigned int score, float time) {
	this->username = username;
	this->score = score;
	this->time = time;
}

void Score::copyScore(Score other) {
	this->username = other.username;
	this->score = other.score;
	this->time = other.time;
}