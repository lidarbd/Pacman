#include"Ghost.h"
#include "Board.h"

/*This function sets the ghost's direction*/
Coord Ghost::getNextCoord(bool load)
{
	if (!load)
	{
		switch (level)
		{
		case GHOST_LEVEL::BEST:
			chasePacman();
			break;
		case GHOST_LEVEL::GOOD:
			if (moveCount == MAX_STEPS)
			{
				randomDirection();
			}
			else if (moveCount == (MAX_STEPS + 5))
			{
				chasePacman();
				moveCount = 0;
			}
			else if (moveCount >= 0 && moveCount < MAX_STEPS)
			{
				chasePacman();
			}
			moveCount++;
			break;
		case GHOST_LEVEL::NOVICE:
			if (moveCount == 0)
			{
				randomDirection();
			}
			else if (moveCount == (MAX_STEPS - 1))
			{
				moveCount = 0;
			}
			else
			{
				moveCount++;
			}
			break;
		}
	}
	
	return Strategy::getNextCoord(load);
}


void Ghost::checkCollision(bool color, bool load)
{
	if (!isValidPos())
	{
		hitsTunnelOrWall();
	}

	updatePosition(color);
}

bool Ghost::isValidPos()
{
	return (!isGhostInCell() && (Strategy::isValidPos()));
}

bool Ghost::isGhostInCell()
{
	return (newPos.getSign() == Cell::SIGN::GHOST_ON_CRUMB || newPos.getSign() == Cell::SIGN::GHOST_ON_NONE);
}


/*This fction updates the ghost's new position*/
void Ghost::updatePosition(bool color)
{
	if (newPos.getSign() == Cell::SIGN::BreadCrumbs|| newPos.getSign() == Cell::SIGN::GHOST_ON_CRUMB)
	{
		newPos.setSign(Cell::SIGN::GHOST_ON_CRUMB);
	}
	else if (newPos.getSign() == Cell::SIGN::NONE || newPos.getSign() == Cell::SIGN::GHOST_ON_NONE)
	{
		newPos.setSign(Cell::SIGN::GHOST_ON_NONE);
	}

	if (!silent)
	{
		gotoxy(newPos.getCoord().getY(), newPos.getCoord().getX());
		if (color)
			SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTRED);
		cout << '$';
		gotoxy(oldPos.getCoord().getY(), oldPos.getCoord().getX());
	}

	if (oldPos.getSign() == Cell::SIGN::GHOST_ON_CRUMB || oldPos.getSign() == Cell::SIGN::BreadCrumbs)
	{
		if (!silent)
		{
			if (color)
				SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), (int)Colors::COLORS::LIGHTCYAN);
			cout << '.';
		}
		oldPos.setSign(Cell::SIGN::BreadCrumbs);
	}
	else
	{
		if (!silent)
			cout << ' ';
		oldPos.setSign(Cell::SIGN::NONE);
	}

	currentPos = newPos.getCoord();
}


void Ghost::chasePacman()
{
	int xPac = board->getPacmanPosition().getX();
	int yPac = board->getPacmanPosition().getY();

	int xGhost = currentPos.getX();
	int yGhost = currentPos.getY();

	if (abs(xGhost - xPac) > abs(yGhost - yPac))
	{
		if (xGhost < xPac && board->getBoard()[xGhost + 1][yGhost].getSign() != Cell::SIGN::WALL)
		{
			direction = DIRECTION::DOWN;
		}
		else if (xGhost > xPac && board->getBoard()[xGhost - 1][yGhost].getSign() != Cell::SIGN::WALL)
		{
			direction = DIRECTION::UP;
		}
		else
		{
			if (yGhost < yPac && board->getBoard()[xGhost][yGhost + 1].getSign() != Cell::SIGN::WALL)
			{
				direction = DIRECTION::RIGHT;
			}
			else if (yGhost > yPac && board->getBoard()[xGhost][yGhost - 1].getSign() != Cell::SIGN::WALL)
			{
				direction = DIRECTION::LEFT;
			}
		}
	}
	else
	{
		if (yGhost < yPac && board->getBoard()[xGhost][yGhost+1].getSign() != Cell::SIGN::WALL)
		{
			direction = DIRECTION::RIGHT;
		}
		else if (yGhost > yPac && board->getBoard()[xGhost][yGhost - 1].getSign() != Cell::SIGN::WALL)
		{
			direction = DIRECTION::LEFT;
		}
		else
		{
			if (xGhost < xPac && board->getBoard()[xGhost + 1][yGhost].getSign() != Cell::SIGN::WALL)
			{
				direction = DIRECTION::DOWN;
			}
			else if (xGhost > xPac && board->getBoard()[xGhost - 1][yGhost].getSign() != Cell::SIGN::WALL)
			{
				direction = DIRECTION::UP;
			}
		}
	}


}

