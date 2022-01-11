#ifndef _SCORE_HPP
#define _SCORE_HPP

#include <iostream>
#include <string>

class Score {
	public:
		std::string username;
		unsigned int score;
		float time;
		Score();
		Score(std::string username, unsigned int score, float time);
		~Score();
		void setScore(std::string username, unsigned int score, float time);
		void copyScore(Score score);
};

#endif