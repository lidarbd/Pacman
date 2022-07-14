#include "Strategy.h"
#include "Board.h"

void Strategy::setCurrentPos(const Coord& _newPos)
{
	currentPos = _newPos;
}
void Strategy::setDirection(const DIRECTION& newDirection)
{
	direction = newDirection;
}

void Strategy::setNewPos(const Cell& _newPos)
{
	newPos = _newPos;
}
void Strategy::setOldPos(const Cell& _oldPos)
{
	oldPos = _oldPos;
}

Coord Strategy::getNextCoord(bool load)
{
	int x, y;
	x = currentPos.getX();
	y = currentPos.getY();

	switch (direction)
	{
	case DIRECTION::UP:
		x--;
		break;
	case DIRECTION::DOWN:
		x++;
		break;
	case DIRECTION::LEFT:
		y--;
		break;
	case DIRECTION::RIGHT:
		y++;
		break;
	default:
		break;
	}

	/*Updates the new position according the direction*/
	Coord newPos(x,y);

	/*Checks if the coord is out of the limits of the board and updates to the right position*/
	if (newPos.getX() == board->getRowSize())
	{
		newPos.setX(0);
	}
	else if (newPos.getX() == -1)
	{
		newPos.setX(board->getRowSize() - 1);
	}

	if (newPos.getY() == board->getColSize())
	{
		newPos.setY(0);
	}
	else if (newPos.getY() == -1)
	{
		newPos.setY(board->getColSize() - 1);
	}

	return newPos;
}

void Strategy::move(bool color, bool load)
{
	Coord toWhereCoord = getNextCoord(load); /*get the creature's new direction*/

	newPos.setCoord(toWhereCoord.getX(), toWhereCoord.getY());
	newPos.setSign(board->getBoard()[toWhereCoord.getX()][toWhereCoord.getY()].getSign());

	oldPos = board->getBoard()[currentPos.getX()][currentPos.getY()];

	checkCollision(color, load);

	/*Updates the signs in the cells of the board */
	board->getBoard()[oldPos.getCoord().getX()][oldPos.getCoord().getY()].setSign(oldPos.getSign());
	board->getBoard()[newPos.getCoord().getX()][newPos.getCoord().getY()].setSign(newPos.getSign());
}

void Strategy::printCreatureAfterMove(const char ch, bool isColor,int color, Coord c1, Coord c2)
{
	gotoxy(c1.getY(), c1.getX());
	cout << ' ';

	gotoxy(c2.getY(), c2.getX());
	if (isColor)
		SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), color);
	cout << ch;
}

void Strategy::checkCollision(bool color, bool load)
{

}

/*This function runs until the creature changes it's direction*/
void Strategy::hitsTunnelOrWall()
{
	while (!isValidPos())
	{
		Coord newCoord = getCoordByRandomDirection();
		newPos.setCoord(newCoord.getX(), newCoord.getY());
		newPos.setSign(board->getBoard()[newCoord.getX()][newCoord.getY()].getSign());
	}
}

void Strategy::randomDirection()
{
	int dir = rand() % 4;
	direction = (DIRECTION)dir;
}

Coord Strategy::getCoordByRandomDirection()
{
	randomDirection();

	return Strategy::getNextCoord();
}

bool Strategy::isValidPos()
{
	return ((newPos.getSign() != Cell::SIGN::WALL) && !isTunnel(newPos));
}


bool Strategy::isTunnel(Cell& cell)
{
	return (((cell.getCoord().getX() == 0) || (cell.getCoord().getY() == 0) || (cell.getCoord().getX() == board->getRowSize()-1)
		|| (cell.getCoord().getY() == board->getColSize()-1)) && (cell.getSign() != Cell::SIGN::WALL));
}
