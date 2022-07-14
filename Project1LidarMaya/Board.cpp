#include "Board.h"
#include "Colors.h"
#include <time.h>

using namespace std;

Board::Board(vector<vector<char>> pattern, int _row,  int _col)
	:row(_row), col(_col), currGhosts(0)
{	
	numOfBreadCrumbs = 0;
	the_board = new Cell * [row];
	bool isPac, isGhosts;

	isPac = isGhosts = false;

	for (int i = 0; i < row; i++)
	{
		the_board[i] = new Cell[col];
		for (int j = 0; j < col; j++)
		{
			the_board[i][j].setCoord(i, j);
			switch (pattern[i][j])
			{
			case ' ':
				the_board[i][j].setSign(Cell::SIGN::BreadCrumbs);
				numOfBreadCrumbs++;
				break;
			case '@':
				if (isPac)/*If there is more than one pacman*/
				{
					the_board[i][j].setSign(Cell::SIGN::BreadCrumbs);
					numOfBreadCrumbs++;
				}
				else
				{
					the_board[i][j].setSign(Cell::SIGN::PACMAN);
					pacmanPos.setX(i);
					pacmanPos.setY(j);
					isPac = true;
				}
				break;
			case '#':
				the_board[i][j].setSign(Cell::SIGN::WALL);
				break;
			case '$':
				if (isGhosts)/*If there are more than 4 ghosts*/
				{
					the_board[i][j].setSign(Cell::SIGN::BreadCrumbs);
					numOfBreadCrumbs++;
				}
				else
				{
					the_board[i][j].setSign(Cell::SIGN::GHOST_ON_CRUMB);
					ghostCoords[currGhosts].setX(i);
					ghostCoords[currGhosts].setY(j);
					currGhosts++;

					if (currGhosts == MAX_GHOSTS)
						isGhosts = true;
				}
				break;
			case '%':
				the_board[i][j].setSign(Cell::SIGN::TEXT);
				break;
			case '&':
				the_board[i][j].setSign(Cell::SIGN::TEXT);
				textPos.setX(i);
				textPos.setY(j);
				break;
			default:
				break;
			}
		}
	}
}


void Board::printBoard(bool color)const
{
	for (int i = 0; i < row; i++)
	{
		for (int j = 0; j < col; j++)
		{
			switch (the_board[i][j].getSign())
			{
			case Cell::SIGN::TEXT:
			case Cell::SIGN::NONE:
				cout << ' ';
				break;
			case Cell::SIGN::PACMAN:
				if (color)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::YELLOW);
				cout << '@';
				break;
			case Cell::SIGN::BreadCrumbs:
				if (color)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTCYAN);
				cout << '.';
				break;
			case Cell::SIGN::WALL:
				if (color)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::DARKGRAY);
				cout << '#';
				break;
			case Cell::SIGN::GHOST_ON_CRUMB:
				if (color)
					SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTRED);
				cout << '$';
				break;
			case Cell::SIGN::GHOST_ON_NONE:
				cout << ' ';
				break;
			default:
				break;
			}
		}
		cout << "\n";
	}

}

const Coord& Board::getGhostPos(int ghostNum)const 
{
	return ghostCoords[ghostNum];
}
Coord Board::getFruitPos() const
{
	int x, y;

	srand(time(NULL));
	x = rand() % row;
	y = rand() % col;

	while (!isValidFruitPos(x, y))
	{
		srand(time(NULL));
		x = rand() % row;
		y = rand() % col;
	}

	Coord res;
	res.setX(x);
	res.setY(y);

	return res;
}

bool Board::isValidFruitPos(int x, int y) const
{
	return((the_board[x][y].getSign() != Cell::SIGN::WALL) && (the_board[x][y].getSign() != Cell::SIGN::PACMAN)
		&& (the_board[x][y].getSign() != Cell::SIGN::GHOST_ON_CRUMB) && (the_board[x][y].getSign() != Cell::SIGN::GHOST_ON_NONE)
		&& (the_board[x][y].getSign() != Cell::SIGN::TEXT) && (!isTunnel(the_board[x][y])));
}

bool Board::isTunnel(Cell& cell) const
{
	return (((cell.getCoord().getX() == 0) || (cell.getCoord().getY() == 0) || (cell.getCoord().getX() == row - 1)
		|| (cell.getCoord().getY() == col - 1)) && (cell.getSign() != Cell::SIGN::WALL));
}

void Board::setPacmanPos(const Coord& newPos)
{
	pacmanPos = newPos;
}

void Board::deleteTheBoard()
{
	for (int i = 0; i < row; ++i)
	{
		delete the_board[i];
	}
	delete []the_board;
}

Board:: ~Board()
{
	deleteTheBoard();
}
