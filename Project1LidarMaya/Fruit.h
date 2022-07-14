#pragma once

#include "Strategy.h"

class Fruit : public Strategy
{
private:
	int value;
	int timeCount, currTimeCount;
	bool disappear;

public:
	Fruit(Coord initialPosition, Board* _board, bool _silent = false): Strategy(initialPosition, _board, _silent)
	{
		value = (rand() % 5) + 5;
		timeCount = (rand() % 40) + 30;
		currTimeCount = timeCount;
		disappear = false;
	}

	//GET
	const int getValue()const { return value; }
	const int getTimeCount()const { return timeCount; }
	const int getCurrTimeCount()const { return currTimeCount; }
	const bool isDisappear()const { return disappear; }

	//SET
	void setValue();
	void setValue(int newVal);
	void setTimeCount(const int newTimeCount);
	void setCurrTimeCount(const int newTimeCount);
	void setDisappear(const bool newVal);

	
	void getCoord(bool color);
	void updatePosition(bool color);
	void checkCollision(bool color, bool load);
	bool isValidCoord(int x, int y);
	void fruitHitsGhostOrPac(bool color, bool load);
	void unShow(bool color, bool load);
	void show(bool color);
	void move(bool color, bool load);
	void disappearance(bool color);

};