#pragma once

#include <iostream>
#include <cctype>
using std::cout;
using std::cin;

#include <stdlib.h>
#include <stdio.h>
#include <conio.h>
#include <windows.h>

#include "Coord.h"
class Cell
{
public:
	enum class SIGN { PACMAN, BreadCrumbs, WALL, NONE, GHOST_ON_CRUMB, GHOST_ON_NONE, FRUIT_ON_CRUMB, FRUIT_ON_NONE , TEXT};
	static Coord LTUNNEL;
	static Coord RTUNNEL;
	static const int ESC = 27;

private:
	Coord position;
	SIGN contains;

public:
	Cell() : position(position), contains(SIGN::NONE) {}

	//GET
	SIGN getSign() const
	{
		return contains;
	}
	Coord getCoord()const
	{
		return position;
	}


	//SET
	void setCoord(const int newX, const int newY)
	{
		position.setX(newX);
		position.setY(newY);
	}
	void setSign(const SIGN newSign)
	{
		contains = newSign;
	}
};