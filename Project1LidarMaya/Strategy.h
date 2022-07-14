#pragma once

//#include"Board.h"
#include "Coord.h"
#include "Cell.h"
#include "GoToXY.h"
#include "Colors.h"
#include "definitions.h"

class Board;
class Strategy
{
protected:
	Coord startPos;
	Coord currentPos;
	Cell newPos, oldPos;
	DIRECTION direction;
	Board* board;
	bool silent;

public:
	Strategy(Coord initialPosition, Board* _board, bool _silent = false)
		:startPos(initialPosition), currentPos(initialPosition), board(_board), silent(_silent)
	{
		direction = DIRECTION::STAY;
	}


	//GET
	const Coord& getCurrentPos() const { return this->currentPos; }
	const Coord& getStartPos() const { return startPos; }
	const DIRECTION& getDirection() const { return direction; }
	const Cell& getNewPos()const { return newPos; }
	const Cell& getOldPos()const { return oldPos; }

	//SET
	void setCurrentPos(const Coord& );
	void setDirection(const DIRECTION& direction);
	void setNewPos(const Cell& _newPos);
	void setOldPos(const Cell& _oldPos);

	
	virtual void move(bool color, bool load);
	void hitsTunnelOrWall();
	virtual Coord getNextCoord(bool load = false);
	Coord getCoordByRandomDirection();
	void randomDirection();
	virtual bool isValidPos();
	virtual void checkCollision(bool color, bool load);
	bool isTunnel(Cell& cell);
	void printCreatureAfterMove(const char ch, bool isColor,int color, Coord c1, Coord c2);
	
};

