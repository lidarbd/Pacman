#pragma once

#include <iostream>
#include <cctype>
#include <vector>
#include "Cell.h"
#include "definitions.h"
using std::vector;

class Board
{

private:
	Cell** the_board;
	int row, col;
	Coord textPos;
	Coord pacmanPos;
	Coord ghostCoords[MAX_GHOSTS];
	Coord fruitPosition;
	int currGhosts;
	int numOfBreadCrumbs;

public:
	Board(vector<vector<char>> pattern, int _row, int _col);
	~Board();

	Board(const Board& other) = delete;
	Board& operator=(const Board& other) = delete;

	//GET
	inline const int getRowSize() const { return row; }
	inline const int getColSize() const { return col; }
	const Coord& getPacmanPosition() const { return pacmanPos; };
	Cell** getBoard() const { return the_board; }
	int getNumOfBreadCrumbs()const { return numOfBreadCrumbs; }
	const Coord& getGhostPos(int ghostNum)const;
	Coord getFruitPos() const;
	const Coord& getTextPos()const { return textPos; }
	const int getCurrGhosts()const { return currGhosts; }

	//SET
	void setPacmanPos(const Coord& newPos);

	void printBoard(bool color) const;
	bool isTunnel(Cell& cell) const;
	bool isValidFruitPos(int x, int y) const;
	void deleteTheBoard();


};

