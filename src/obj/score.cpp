#include "../include/score.hpp"

Score::Score() {
	
}

Score::Score(std::string username, unsigned int score): username(username), score(score) {

}

Score::~Score() {

}

void Score::setScore(std::string username, unsigned int score) {
	this->username = username;
	this->score = score;
}

void Score::copyScore(Score other) {
	this->username = other.username;
	this->score = other.score;
}