#pragma once


class Coord
{
private:
	int x, y;

public:
	Coord(const int _x = -1, const int _y = -1) : x(_x), y(_y) {}

	//GET
	int getX() const { return x; }
	int getY() const { return y; }

	//SET
	void setX(const int _x) { x = _x; }
	void setY(const int _y) { y = _y; }
};
