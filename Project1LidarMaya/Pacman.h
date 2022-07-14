#pragma once
#include "Strategy.h"
#include "Board.h"

class Pacman : public Strategy
{	
private:
	int lives;
	int score;
	int eatenBreadCrumbs;

public:
	const char PACMAN = '@';

	Pacman(Coord initialPosition, Board* _board, int _lives, int _score, bool _silent = false) :Strategy(initialPosition, _board, _silent)
	{
		lives = _lives;
		score = _score;
		eatenBreadCrumbs = 0;
	}

	//GET
	int getLives()const { return lives; }
	int getScore() const { return score; }
	int getEatenBreadCrumbs() const { return eatenBreadCrumbs; }

	//SET
	void setLives(const int newLives);
	void setScore(const int newScore);
	void setEatenBreadCrumbs(const int newEatenBreadCrumbs);

	void move(bool color, bool load);
	void checkCollision(bool color, bool load);
	void updatePosition(bool color);
	void pacmanHitsGhost(bool color);
	void pacmanHitsFruit(bool color);
};