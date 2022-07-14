#pragma once
#include "Strategy.h"
#include "definitions.h"

class Ghost: public Strategy
{
public: 
	static const int MAX_STEPS = 20;
private:
	int moveCount;
	GHOST_LEVEL level;

public:
	Ghost (Coord initialPosition, GHOST_LEVEL _level, Board* _board, bool _silent = false) :Strategy(initialPosition, _board, _silent)
	{
		moveCount = 0;
		level =_level;
	}

	virtual Coord getNextCoord(bool load);
	bool isValidPos();
	bool isGhostInCell();
	void chasePacman();
	void checkCollision(bool color, bool load);
	void updatePosition(bool color);
};
