#pragma once

#include <iostream>
#include <fstream>
#include <vector>
#include <string>
using std::cout;
using std::cin;
using std::vector;
using std::string;
using std::ifstream;
using std::istream;
#include "Coord.h"

class BoardShapes
{
public:
	static const int HEIGHT = 3;
	static const int WIDTH = 20;
	static const int ROWS = 25;
	static const int COLS = 80;

private:
	int rows, cols;
	vector<vector<char>> board_shape;
	

public:
	BoardShapes(string fileName);

	//GET
	vector<vector<char>> getBoardShape() const { return board_shape; }
	int getRows() const { return rows; }
	int getCols() const { return cols; }

	void handleText(istream& myfile, string line, int maxCols);

};